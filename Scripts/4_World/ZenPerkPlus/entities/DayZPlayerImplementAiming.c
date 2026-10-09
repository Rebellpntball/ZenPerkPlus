// relife-style shake cut: scale aim offsets after vanilla sway, never zero them out.
modded class DayZPlayerImplementAiming
{
	override bool ProcessAimFilters(float pDt, SDayZPlayerAimingModel pModel, int stance_index)
	{
		bool ok = super.ProcessAimFilters(pDt, pModel, stance_index);
		if (!pModel || !m_PlayerPb)
			return ok;
		PlayerBase player = PlayerBase.Cast(m_PlayerPb);
		if (!player)
			return ok;
		float mult = ZenPerkPlusHelpers.GetSwayMultiplier(player);
		if (mult >= 0.999)
			return ok;
		pModel.m_fAimXHandsOffset = pModel.m_fAimXHandsOffset * mult;
		pModel.m_fAimYHandsOffset = pModel.m_fAimYHandsOffset * mult;
		pModel.m_fAimXCamOffset = pModel.m_fAimXCamOffset * mult;
		pModel.m_fAimYCamOffset = pModel.m_fAimYCamOffset * mult;
		return ok;
	}
}
