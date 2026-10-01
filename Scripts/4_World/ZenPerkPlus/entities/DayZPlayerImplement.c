modded class DayZPlayerImplement
{
	override void HandleView()
	{
		super.HandleView();

		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableFirearmsSkill || !cfg.EnableSprintZoom)
			return;

		PlayerBase player = PlayerBase.Cast(this);
		if (!player)
			return;

		if (player.GetZenPerkReward(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusPerks.FIREARMS_MASTER_GUNFIGHTER) <= 0)
			return;

		HumanInputController hic = GetInputController();
		if (!hic)
			return;

		// Tactical eye zoom while unraised/sprinting only. This does not enable ADS while sprinting
		// and intentionally does not alter firing restrictions.
		if (!m_MovementState.IsRaised() && hic.IsZoomToggle())
		{
			m_CameraEyeZoomLevel = ECameraZoomType.NORMAL;
		}
	}
}
