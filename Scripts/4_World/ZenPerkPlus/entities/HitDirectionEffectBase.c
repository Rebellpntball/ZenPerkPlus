// Radio contact reuses the vanilla hit indicator.
// Color, style, scatter, duration and static/dynamic all stay on cfggameplay.json
// unless RadioGlowColor is set. That hex tints only the radio pulse.
modded class HitDirectionEffectBase
{
	static int s_ZPP_Color;
	static bool s_ZPP_UseColor;

	static bool ZenPerkPlus_IndicatorsOn()
	{
		CheckValues();
		return m_Mode != HitDirectionModes.DISABLED;
	}

	static void ZenPerkPlus_QueueColor(string hex)
	{
		s_ZPP_UseColor = false;
		if (!hex || hex == "")
			return;
		int color = ZenPerkPlus_ParseArgb(hex);
		if (color == 0)
			return;
		s_ZPP_Color = color;
		s_ZPP_UseColor = true;
	}

	static int ZenPerkPlus_ParseArgb(string hex)
	{
		if (hex == "")
			return 0;
		hex.Replace("0x", "");
		hex.Replace("0X", "");
		hex.Replace("#", "");
		if (hex.Length() != 8)
			return 0;
		int color = 0;
		for (int i = 0; i < hex.Length(); i++)
		{
			string ch = hex.Get(i);
			ch.ToLower();
			int n = "0123456789abcdef".IndexOf(ch);
			if (n < 0)
				return 0;
			color = (color << 4) + n;
		}
		return color;
	}

	override void Init(DayZPlayer player, float hit_direction, float intensity_max)
	{
		int saved = m_Color;
		if (s_ZPP_UseColor)
			m_Color = s_ZPP_Color;
		s_ZPP_UseColor = false;
		super.Init(player, hit_direction, intensity_max);
		m_Color = saved;
	}
}
