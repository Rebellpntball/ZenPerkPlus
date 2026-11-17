// Radio contact reuses the vanilla hit indicator.
// Style, scatter, duration, and static/dynamic stay on cfggameplay.json.
// Color is always amber so it does not read as a bullet hit.
modded class HitDirectionEffectBase
{
	static bool s_ZPP_Amber;

	static bool ZenPerkPlus_IndicatorsOn()
	{
		CheckValues();
		return m_Mode != HitDirectionModes.DISABLED;
	}

	static void ZenPerkPlus_QueueAmber()
	{
		s_ZPP_Amber = true;
	}

	override void Init(DayZPlayer player, float hit_direction, float intensity_max)
	{
		int saved = m_Color;
		if (s_ZPP_Amber)
			m_Color = ARGB(230, 255, 168, 32);
		s_ZPP_Amber = false;
		super.Init(player, hit_direction, intensity_max);
		m_Color = saved;
	}
}
