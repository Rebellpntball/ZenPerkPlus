// One-shot tint so the vanilla blood-direction splash can render amber
// without changing the color of a real hit.
modded class HitDirectionEffectBase
{
	static bool s_ZPP_QueueAmber;

	override void Init(DayZPlayer player, float hit_direction, float intensity_max)
	{
		bool amber = s_ZPP_QueueAmber;
		s_ZPP_QueueAmber = false;
		super.Init(player, hit_direction, intensity_max);
		if (amber && m_Image)
			m_Image.SetColor(ARGB(230, 255, 168, 32));
	}
}
