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
		if (player.GetZenPerkReward(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusPerks.FIREARMS_DEADEYE) <= 0)
			return;
		HumanInputController hic = GetInputController();
		if (!hic)
			return;
		if (!m_MovementState.IsRaised() && hic.IsZoomToggle())
		{
			m_CameraEyeZoomLevel = ECameraZoomType.NORMAL;
		}
	}
}

modded class StaminaHandler
{
	override void DepleteStaminaEx(EStaminaModifiers modifier, float dT = -1, float coeff = 1)
	{
		PlayerBase player = PlayerBase.Cast(m_Player);
		if (player)
			coeff = coeff * ZenPerkPlusHelpers.GetStaminaDrainMultiplier(player);
		super.DepleteStaminaEx(modifier, dT, coeff);
	}
}
