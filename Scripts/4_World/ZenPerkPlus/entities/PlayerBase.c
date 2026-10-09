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

	ZenPerkPlus_TryCombatRadioStatic(player, deadEntity, true, false);
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
		ZenPerkPlus_UpdateAIContact(deltaTime);
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

	void ZenPerkPlus_UpdateAIContact(float deltaTime)
	{
		m_ZenPerkPlusContactScan -= deltaTime;
		if (m_ZenPerkPlusContactScan > 0)
			return;
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableCombatOpsSkill || !cfg.EnableAIContactPing)
		{
			m_ZenPerkPlusContactScan = 8;
			return;
		}
		float ghost = GetZenPerkRewardPercent01(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusPerks.COMBAT_GHOST_PACE);
		float step = GetZenPerkRewardPercent01(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusPerks.COMBAT_LIGHT_STEP);
		if (ghost <= 0 && step <= 0)
		{
			m_ZenPerkPlusContactScan = 6;
			return;
		}
		float range = cfg.AIContactPingRangeMeters;
		if (range <= 0)
			range = 45;
		range = Math.Clamp(range * (0.55 + ghost + step * 0.35), 12, 75);
		array<Object> objs = new array<Object>;
		array<CargoBase> cargos = new array<CargoBase>;
		g_Game.GetObjectsAtPosition3D(GetPosition(), range, objs, cargos);
		Object nearest;
		float best = range + 1;
		int checked = 0;
		foreach (Object obj : objs)
		{
			checked++;
			if (checked > 80)
				break;
			if (!ZenPerkPlusHelpers.IsTrackedHostileAI(obj, this))
				continue;
			EntityAI ent = EntityAI.Cast(obj);
			if (!ent || !ent.IsAlive())
				continue;
			float dist = vector.Distance(GetPosition(), obj.GetPosition());
			if (dist < best)
			{
				best = dist;
				nearest = obj;
			}
		}
		float cd = cfg.AIContactPingCooldownSeconds;
		if (cd < 8)
			cd = 22;
		m_ZenPerkPlusContactScan = cd;
		if (!nearest)
			return;
		string bucket = "Far";
		if (best < 15)
			bucket = "Close";
		else if (best < 32)
			bucket = "Near";
		string bearing = ZenPerkPlusHelpers.RoughBearing(this, nearest.GetPosition());
		ZenPerkPlusHelpers.Notify(this, "Contact", bucket + " hostile " + bearing + ".");
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
