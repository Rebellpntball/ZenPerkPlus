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
				ZenPerkPlusHelpers.Notify(player, "Combat Ops", "Expansion AI neutralized.");
			ZenPerkPlus_TryCombatRadioStatic(player, deadEntity, true, false);
		}

		ZenPerkPlus_AwardFirearmKillEXP(player, killerEntity, cfg);
		return;
	}
	#endif

	PlayerBase killedPlayer = PlayerBase.Cast(deadEntity);
	if (killedPlayer)
	{
		player.AddZenSkillEXP(ZenPerkPlusSkills.COMBAT_OPS, cfg.PlayerKillCombatOpsEXP);
		ZenPerkPlus_TryCombatRadioStatic(player, deadEntity, false, true);
		ZenPerkPlus_AwardFirearmKillEXP(player, killerEntity, cfg);
		return;
	}

	if (deadEntity.IsInherited(ZombieBase))
		player.AddZenSkillEXP(ZenPerkPlusSkills.COMBAT_OPS, cfg.InfectedKillCombatOpsEXP);
	else
		player.AddZenSkillEXP(ZenPerkPlusSkills.COMBAT_OPS, cfg.AnimalKillCombatOpsEXP);

	ZenPerkPlus_TryCombatRadioStatic(player, deadEntity, true, false);
	ZenPerkPlus_AwardFirearmKillEXP(player, killerEntity, cfg);
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
