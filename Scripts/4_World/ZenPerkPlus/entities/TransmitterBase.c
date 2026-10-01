modded class TransmitterBase
{
	override void OnWorkStart()
	{
		super.OnWorkStart();
		ZenPerkPlus_AwardCombatRadioWorkEXP(true);
	}

	override void OnWorkStop()
	{
		super.OnWorkStop();
		ZenPerkPlus_AwardCombatRadioWorkEXP(false);
	}

	void ZenPerkPlus_AwardCombatRadioWorkEXP(bool workStart)
	{
		#ifdef SERVER
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableCombatOpsSkill)
			return;

		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (!player)
			return;

		if (workStart)
			ZenPerkPlusHelpers.AwardRaw(player, ZenPerkPlusSkills.COMBAT_OPS, cfg.CombatRadioWorkStartEXP);
		else
			ZenPerkPlusHelpers.AwardRaw(player, ZenPerkPlusSkills.COMBAT_OPS, cfg.CombatRadioWorkStopEXP);
		#endif
	}
}
