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
	override void DepleteStamina(float value, float drainCap = -1)
	{
		PlayerBase player = PlayerBase.Cast(m_Player);
		if (player)
			value = value * ZenPerkPlusHelpers.GetStaminaDrainMultiplier(player);
		super.DepleteStamina(value, drainCap);
	}
}
