class ZenPerkPlusRPC
{
	static const int RADIO_GLOW = 47823101;
}

// Same idea as a proximity radio: track living PlayerBase (real players and eAI).
// Infected and animals are not PlayerBase, so they never trip the static.
// Text stays vague. The amber mark reuses the vanilla hit-direction splash.
class ZenPerkPlusRadioWatch
{
	protected static ref array<PlayerBase> s_Men;

	static void Register(PlayerBase man)
	{
		if (!s_Men)
			s_Men = new array<PlayerBase>;
		if (man && s_Men.Find(man) == -1)
			s_Men.Insert(man);
	}

	static void Unregister(PlayerBase man)
	{
		if (!s_Men || !man)
			return;
		int idx = s_Men.Find(man);
		if (idx != -1)
			s_Men.Remove(idx);
	}

	static bool NearestInRange(PlayerBase owner, float range, out bool outIsPlayer, out vector outPos, out float outDist)
	{
		outIsPlayer = false;
		outPos = vector.Zero;
		outDist = -1;
		if (!s_Men || !owner || range <= 0)
			return false;
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg)
			return false;
		vector pos = owner.GetPosition();
		float best = range * range;
		bool found = false;
		for (int i = s_Men.Count() - 1; i >= 0; i--)
		{
			PlayerBase man = s_Men[i];
			if (!man)
			{
				s_Men.Remove(i);
				continue;
			}
			if (man == owner || !man.IsAlive())
				continue;
			bool isPlayer = man.GetIdentity() != null;
			if (isPlayer && !cfg.CombatRadioDetectPlayers)
				continue;
			if (!isPlayer && !cfg.CombatRadioDetectExpansionAI)
				continue;
			float d2 = vector.DistanceSq(pos, man.GetPosition());
			if (d2 <= best)
			{
				best = d2;
				found = true;
				outIsPlayer = isPlayer;
				outPos = man.GetPosition();
			}
		}
		if (found)
			outDist = Math.Sqrt(best);
		return found;
	}
}

static void ZenPerkPlus_HandleKilledEntity(EntityAI deadEntity, Object killer)
{
	#ifdef SERVER
	ZenPerkPlus cfg = GetZenPerkPlusConfig();
	if (!cfg || !deadEntity || !cfg.EnableCombatOpsSkill)
		return;

	PlayerBase player = ZenPerkPlusHelpers.GetPlayerFromKiller(killer);
	if (!player)
		return;

	EntityAI killerEntity = EntityAI.Cast(killer);
	if (!killerEntity)
		killerEntity = EntityAI.Cast(player.GetHumanInventory().GetEntityInHands());

	#ifdef EXPANSIONMODAI
	eAIBase ai = eAIBase.Cast(deadEntity);
	if (ai)
	{
		if (cfg.EnableExpansionAIKillEXP)
		{
			player.AddZenSkillEXP(ZenPerkPlusSkills.COMBAT_OPS, cfg.ExpansionAIKillCombatOpsEXP);
			if (cfg.EnableKillNotifications)
				ZenPerkPlusHelpers.Notify(player, "Operator", "Hostile AI down.");
			ZenPerkPlus_TryCombatRadioStatic(player, deadEntity, true, false);
		}
		ZenPerkPlus_AwardFirearmKillEXP(player, killerEntity, cfg);
		player.ZenPerkPlus_OnCombatKill();
		return;
	}
	#endif

	PlayerBase killedPlayer = PlayerBase.Cast(deadEntity);
	if (killedPlayer)
	{
		player.AddZenSkillEXP(ZenPerkPlusSkills.COMBAT_OPS, cfg.PlayerKillCombatOpsEXP);
		ZenPerkPlus_TryCombatRadioStatic(player, deadEntity, false, true);
		ZenPerkPlus_AwardFirearmKillEXP(player, killerEntity, cfg);
		player.ZenPerkPlus_OnCombatKill();
		return;
	}

	if (deadEntity.IsInherited(ZombieBase))
		player.AddZenSkillEXP(ZenPerkPlusSkills.COMBAT_OPS, cfg.InfectedKillCombatOpsEXP);
	else
		player.AddZenSkillEXP(ZenPerkPlusSkills.COMBAT_OPS, cfg.AnimalKillCombatOpsEXP);

	ZenPerkPlus_AwardFirearmKillEXP(player, killerEntity, cfg);
	player.ZenPerkPlus_OnCombatKill();
	#endif
}

static void ZenPerkPlus_AwardFirearmKillEXP(PlayerBase player, EntityAI killerEntity, ZenPerkPlus cfg)
{
	#ifdef SERVER
	if (!player || !cfg || !cfg.EnableFirearmsSkill)
		return;
	string firearmAction = ZenPerkPlusHelpers.GetFirearmKillAction(killerEntity);
	if (firearmAction != "")
		player.AddZenSkillEXP(ZenPerkPlusSkills.FIREARMS, cfg.FirearmKillEXP);
	#endif
}

static void ZenPerkPlus_TryCombatRadioStatic(PlayerBase player, EntityAI deadEntity, bool isAIThreat, bool isPlayerThreat)
{
	#ifdef SERVER
	ZenPerkPlus cfg = GetZenPerkPlusConfig();
	if (!cfg || !cfg.EnableCombatRadioStatic || !player || !deadEntity)
		return;
	if (player.GetZenPerkReward(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusPerks.COMBAT_RADIO_DISCIPLINE) <= 0)
		return;
	if (isPlayerThreat && !cfg.CombatRadioDetectPlayers)
		return;
	if (isAIThreat && !cfg.CombatRadioDetectExpansionAI)
		return;
	if (!ZenPerkPlusHelpers.CanReceiveCombatRadio(player))
		return;
	float dist = vector.Distance(player.GetPosition(), deadEntity.GetPosition());
	if (dist > ZenPerkPlusHelpers.GetCombatRadioRange(player))
		return;
	ZenPerkPlusHelpers.AwardAction(player, ZenPerkPlusActions.COMBAT_RADIO_STATIC);
	if (isPlayerThreat)
		player.ZenPerkPlus_NotifyCombatRadio("Signal spike. Someone is close.");
	else
		player.ZenPerkPlus_NotifyCombatRadio("Weak hostile chatter nearby.");
	#endif
}

modded class PlayerBase
{
	protected float m_ZenPerkPlusLastCombatRadioTime;
	protected float m_ZenPerkPlusCombatBoostUntil;
	protected vector m_ZenPerkPlusLastDrivePos;
	protected float m_ZenPerkPlusDriveAccumMeters;
	protected float m_ZenPerkPlusContactScan;
	protected bool m_ZPP_GlowPending;
	protected float m_ZPP_GlowDir;
	protected float m_ZPP_GlowIntensity;

	override void EEInit()
	{
		super.EEInit();
		if (GetGame().IsServer())
			ZenPerkPlusRadioWatch.Register(this);
	}

	override void EEDelete(EntityAI parent)
	{
		if (GetGame().IsServer())
			ZenPerkPlusRadioWatch.Unregister(this);
		super.EEDelete(parent);
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		if (rpc_type != ZenPerkPlusRPC.RADIO_GLOW)
			return;
		if (!g_Game || !g_Game.IsClient())
			return;
		Param2<float, float> payload = new Param2<float, float>(0, 0);
		if (!ctx.Read(payload))
			return;
		m_ZPP_GlowDir = payload.param1;
		m_ZPP_GlowIntensity = payload.param2;
		m_ZPP_GlowPending = true;
	}

	bool ZenPerkPlus_ConsumeRadioGlow(out float dir, out float intensity)
	{
		if (!m_ZPP_GlowPending)
			return false;
		m_ZPP_GlowPending = false;
		dir = m_ZPP_GlowDir;
		intensity = m_ZPP_GlowIntensity;
		return true;
	}

	override void EEKilledZen(notnull Object killer)
	{
		super.EEKilledZen(killer);
		ZenPerkPlus_HandleKilledEntity(this, killer);
	}

	void ZenPerkPlus_NotifyCombatRadio(string message)
	{
		#ifdef SERVER
		float now = g_Game.GetTime() * 0.001;
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (cfg && now - m_ZenPerkPlusLastCombatRadioTime < cfg.CombatRadioCooldownSeconds)
			return;
		m_ZenPerkPlusLastCombatRadioTime = now;
		ZenPerkPlusHelpers.Notify(this, "Radio Static", message);
		#endif
	}

	void ZenPerkPlus_OnCombatKill()
	{
		#ifdef SERVER
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableCombatOpsSkill)
			return;
		float ghost = GetZenPerkRewardPercent01(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusPerks.COMBAT_GHOST_PACE);
		float second = GetZenPerkRewardPercent01(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusPerks.COMBAT_SECOND_WIND);
		if (ghost <= 0 && second <= 0)
			return;
		float boostSecs = cfg.ScoutPostCombatBoostSeconds * (0.5 + ghost + second * 0.5);
		m_ZenPerkPlusCombatBoostUntil = g_Game.GetTime() * 0.001 + boostSecs;
		StaminaHandler sh = GetStaminaHandler();
		if (sh)
		{
			float add = cfg.ScoutPostCombatStaminaReturn * (0.5 + ghost);
			sh.SetStamina(Math.Clamp(sh.GetStamina() + add, 0, sh.GetStaminaCap()));
		}
		#endif
	}

	bool ZenPerkPlus_HasScoutBoost()
	{
		return (g_Game.GetTime() * 0.001) < m_ZenPerkPlusCombatBoostUntil;
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		float hpBefore = GetHealth("", "Health");
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		#ifdef SERVER
		ZenPerkPlus_RefundDriverImpact(hpBefore, ammo);
		#endif
	}

	void ZenPerkPlus_RefundDriverImpact(float hpBefore, string ammo)
	{
		if (!IsAlive() || !GetCommand_Vehicle())
			return;
		string low = ammo;
		low.ToLower();
		bool crash = low.Contains("crash") || low.Contains("transport") || low.Contains("vehicle") || low.Contains("fall");
		if (!crash)
			return;
		float refund = ZenPerkPlusHelpers.GetPlayerCrashRefund(this);
		float lost = hpBefore - GetHealth("", "Health");
		if (refund > 0 && lost > 0)
			AddHealth("", "Health", lost * refund);
	}

	override void OnScheduledTick(float deltaTime)
	{
		super.OnScheduledTick(deltaTime);
		#ifdef SERVER
		ZenPerkPlus_UpdateDriverDistance(deltaTime);
		ZenPerkPlus_UpdateShock(deltaTime);
		ZenPerkPlus_UpdateRadioPresence(deltaTime);
		#endif
	}

	void ZenPerkPlus_UpdateShock(float deltaTime)
	{
		float per = ZenPerkPlusHelpers.GetShockPerSecond(this);
		if (per <= 0 || deltaTime <= 0)
			return;
		float maxShock = GetMaxHealth("", "Shock");
		if (maxShock <= 0)
			return;
		float cur = GetHealth("", "Shock");
		if (cur >= maxShock * 0.85)
			return;
		AddHealth("", "Shock", per * deltaTime);
	}

	void ZenPerkPlus_UpdateRadioPresence(float deltaTime)
	{
		m_ZenPerkPlusContactScan -= deltaTime;
		if (m_ZenPerkPlusContactScan > 0)
			return;
		m_ZenPerkPlusContactScan = 3;
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableCombatOpsSkill || !cfg.EnableCombatRadioStatic || !cfg.EnableAIContactPing)
			return;
		if (GetZenPerkReward(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusPerks.COMBAT_RADIO_DISCIPLINE) <= 0)
			return;
		if (!ZenPerkPlusHelpers.CanReceiveCombatRadio(this))
			return;
		float range = cfg.AIContactPingRangeMeters;
		if (range <= 0)
			range = 80;
		float pct = GetZenPerkRewardPercent01(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusPerks.COMBAT_RADIO_DISCIPLINE);
		range = Math.Clamp(range * (1.0 + pct), 20, 150);
		bool isPlayer;
		vector nearestPos;
		float nearestDist;
		if (!ZenPerkPlusRadioWatch.NearestInRange(this, range, isPlayer, nearestPos, nearestDist))
			return;
		if (cfg.EnableRadioDirectionGlow)
		{
			float closeness = 0;
			if (range > 0 && nearestDist >= 0)
				closeness = 1.0 - (nearestDist / range);
			float intensity = Math.Clamp(0.65 + closeness * 0.35, 0.65, 1.0);
			ZenPerkPlus_PushRadioGlow(ZenPerkPlus_RelativeYaw(nearestPos), intensity);
		}
		float now = g_Game.GetTime() * 0.001;
		float cd = cfg.AIContactPingCooldownSeconds;
		if (cd < 8)
			cd = 40;
		if (now - m_ZenPerkPlusLastCombatRadioTime < cd)
			return;
		m_ZenPerkPlusLastCombatRadioTime = now;
		if (isPlayer)
			ZenPerkPlusHelpers.Notify(this, "Radio", "Signal spike. Someone is close.");
		else
			ZenPerkPlusHelpers.Notify(this, "Radio", "Weak hostile chatter nearby.");
	}

	float ZenPerkPlus_RelativeYaw(vector targetPos)
	{
		vector dir = targetPos - GetPosition();
		dir[1] = 0;
		if (dir.LengthSq() < 0.25)
			return 0;
		float rel = Math.NormalizeAngle(dir.VectorToAngles()[0] - GetOrientation()[0]);
		return Math.NormalizeAngle(rel + Math.RandomFloatInclusive(-12, 12));
	}

	void ZenPerkPlus_PushRadioGlow(float relYaw, float intensity)
	{
		if (!GetIdentity())
			return;
		Param2<float, float> payload = new Param2<float, float>(relYaw, intensity);
		g_Game.RPCSingleParam(this, ZenPerkPlusRPC.RADIO_GLOW, payload, true, GetIdentity());
	}

	void ZenPerkPlus_UpdateDriverDistance(float deltaTime)
	{
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableDriverSkill || !cfg.EnableDriverEXP)
			return;
		HumanCommandVehicle hcv = GetCommand_Vehicle();
		if (!hcv || !hcv.GetTransport())
		{
			m_ZenPerkPlusLastDrivePos = vector.Zero;
			return;
		}
		CarScript car = CarScript.Cast(hcv.GetTransport());
		if (!car)
			return;
		vector pos = GetPosition();
		if (m_ZenPerkPlusLastDrivePos == vector.Zero)
		{
			m_ZenPerkPlusLastDrivePos = pos;
			return;
		}
		float dist = vector.Distance(pos, m_ZenPerkPlusLastDrivePos);
		m_ZenPerkPlusLastDrivePos = pos;
		if (dist < 0.5 || dist > 50)
			return;
		m_ZenPerkPlusDriveAccumMeters += dist;
		float per = cfg.DriverDistanceMetersPerEXP;
		if (per <= 0)
			per = 1000;
		while (m_ZenPerkPlusDriveAccumMeters >= per)
		{
			m_ZenPerkPlusDriveAccumMeters -= per;
			ZenPerkPlusHelpers.AwardRaw(this, ZenPerkPlusSkills.DRIVER, cfg.DriverDistanceEXP);
		}
	}
}

modded class ZombieBase
{
	override void EEInit()
	{
		super.EEInit();
		if (GetGame().IsServer())
			ZenPerkPlusRadioWatch.Register(this);
	}

	override void EEDelete(EntityAI parent)
	{
		if (GetGame().IsServer())
			ZenPerkPlusRadioWatch.Unregister(this);
		super.EEDelete(parent);
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		if (rpc_type != ZenPerkPlusRPC.RADIO_GLOW)
			return;
		if (!g_Game || !g_Game.IsClient())
			return;
		Param2<float, float> payload = new Param2<float, float>(0, 0);
		if (!ctx.Read(payload))
			return;
		m_ZPP_GlowDir = payload.param1;
		m_ZPP_GlowIntensity = payload.param2;
		m_ZPP_GlowPending = true;
	}

	bool ZenPerkPlus_ConsumeRadioGlow(out float dir, out float intensity)
	{
		if (!m_ZPP_GlowPending)
			return false;
		m_ZPP_GlowPending = false;
		dir = m_ZPP_GlowDir;
		intensity = m_ZPP_GlowIntensity;
		return true;
	}

	override void EEKilledZen(notnull Object killer)
	{
		super.EEKilledZen(killer);
		ZenPerkPlus_HandleKilledEntity(this, killer);
	}
}

#ifdef EXPANSIONMODAI
modded class eAIBase
{
	override void EEKilledZen(notnull Object killer)
	{
		super.EEKilledZen(killer);
	}
}
#endif
