modded class CarScript
{
	protected bool m_ZenPerkPlusAdjustingFuel;

	override void OnFluidChanged(CarFluid fluid, float newValue, float oldValue)
	{
		super.OnFluidChanged(fluid, newValue, oldValue);
		if (m_ZenPerkPlusAdjustingFuel)
			return;
		if (fluid != CarFluid.FUEL)
			return;
		if (newValue >= oldValue - 0.00001)
			return;
		if (!EngineIsOn())
			return;
		PlayerBase driver = ZenPerkPlus_GetDriverPlayer();
		float frac = ZenPerkPlusHelpers.GetFuelRefundFraction(driver);
		if (frac <= 0)
			return;
		float refund = (oldValue - newValue) * frac;
		if (refund < 0.00005)
			return;
		m_ZenPerkPlusAdjustingFuel = true;
		Fill(CarFluid.FUEL, refund);
		m_ZenPerkPlusAdjustingFuel = false;
	}

	override void OnUpdate(float dt)
	{
		super.OnUpdate(dt);
		#ifdef SERVER
		ZenPerkPlus_UpdateBatteryCare(dt);
		#endif
	}

	void ZenPerkPlus_UpdateBatteryCare(float dt)
	{
		if (dt <= 0 || !EngineIsOn())
			return;
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableBatteryCare || !cfg.EnableDriverSkill)
			return;
		PlayerBase driver = ZenPerkPlus_GetDriverPlayer();
		float rate = ZenPerkPlusHelpers.GetBatteryEnergyPerSecond(driver);
		if (rate <= 0)
			return;
		ItemBase battery = ItemBase.Cast(FindAttachmentBySlotName("CarBattery"));
		if (!battery)
			battery = ItemBase.Cast(FindAttachmentBySlotName("TruckBattery"));
		if (!battery)
			return;
		ComponentEnergyManager em = battery.GetCompEM();
		if (!em)
			return;
		em.AddEnergy(rate * dt);
	}

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
		return PlayerBase.Cast(driverHuman);
	}
}
