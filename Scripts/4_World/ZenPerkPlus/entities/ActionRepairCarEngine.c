modded class ActionRepairCarEngine
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);
		if (!action_data)
			return;
		PlayerBase player = PlayerBase.Cast(action_data.m_Player);
		if (!player)
			return;
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableDriverSkill)
			return;
		int exp = cfg.DriverRepairEXP;
		if (exp <= 0)
			exp = 10;
		float bonus = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.DRIVER, ZenPerkPlusPerks.DRIVER_FIELD_MECHANIC);
		float scaled = exp * (1.0 + bonus);
		int award = scaled;
		if (award < 1)
			award = 1;
		ZenPerkPlusHelpers.AwardRaw(player, ZenPerkPlusSkills.DRIVER, award);
	}
}
