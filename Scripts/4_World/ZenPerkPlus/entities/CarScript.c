modded class CarScript
{
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		PlayerBase driver = ZenPerkPlus_GetDriverPlayer();
		float mult = 1.0;
		if (driver)
			mult = ZenPerkPlusHelpers.GetCrashDamageMultiplier(driver);

		float hpBefore = GetHealth("", "");
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		if (mult >= 0.999 || !driver)
			return;

		float hpAfter = GetHealth("", "");
		float lost = hpBefore - hpAfter;
		if (lost <= 0)
			return;

		float refund = lost * (1.0 - mult);
		if (refund > 0)
			AddHealth("", "", refund);
	}

	PlayerBase ZenPerkPlus_GetDriverPlayer()
	{
		Human driverHuman = CrewMember(0);
		PlayerBase pb = PlayerBase.Cast(driverHuman);
		if (pb)
			return pb;

		array<Human> crew = new array<Human>;
		CrewList(crew);
		foreach (Human h : crew)
		{
			PlayerBase p = PlayerBase.Cast(h);
			if (p)
				return p;
		}
		return null;
	}
}
