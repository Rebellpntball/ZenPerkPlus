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

	override void OnScheduledTick(float deltaTime)
	{
		super.OnScheduledTick(deltaTime);
		#ifdef SERVER
		ZenPerkPlus_UpdateDriverDistance(deltaTime);
		#endif
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
