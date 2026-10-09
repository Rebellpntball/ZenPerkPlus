modded class DayZPlayerImplement
{
	override void HandleView()
	{
		super.HandleView();
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableFirearmsSkill || !cfg.EnableDeadeyeZoom)
			return;
		PlayerBase player = PlayerBase.Cast(this);
		if (!player)
			return;
		if (player.GetZenPerkReward(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusPerks.FIREARMS_DEADEYE) <= 0)
			return;
		if (!m_MovementState.IsRaised())
			return;
		HumanInputController hic = GetInputController();
		if (!hic || !hic.IsZoomToggle())
			return;
		m_CameraEyeZoomLevel = ECameraZoomType.SHALLOW;
	}

	override void AddNoise(NoiseParams noisePar, float noiseMultiplier = 1.0)
	{
		PlayerBase player = PlayerBase.Cast(this);
		if (player)
			noiseMultiplier = noiseMultiplier * ZenPerkPlusHelpers.GetNoiseMultiplier(player);
		super.AddNoise(noisePar, noiseMultiplier);
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
