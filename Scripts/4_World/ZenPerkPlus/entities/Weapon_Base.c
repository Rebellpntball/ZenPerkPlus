modded class Weapon_Base
{
	override void EEFired(int muzzleType, int mode, string ammoType)
	{
		super.EEFired(muzzleType, mode, ammoType);
		ZenPerkPlus_AwardFirearmShotEXP();
		ZenPerkPlus_ApplySafeDurabilityRefund();
		ZenPerkPlus_CheckSidearmReady();
	}

	void ZenPerkPlus_AwardFirearmShotEXP()
	{
		#ifdef SERVER
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableFirearmsSkill)
			return;
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (!player)
			return;
		ZenPerkPlusHelpers.AwardRaw(player, ZenPerkPlusSkills.FIREARMS, cfg.FirearmShotEXP);
		#endif
	}

	void ZenPerkPlus_ApplySafeDurabilityRefund()
	{
		#ifdef SERVER
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableFirearmsSkill || !cfg.FirearmWearReductionEnabled)
			return;
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (!player)
			return;
		float pct = Math.Clamp(player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusHelpers.GetFirearmWearPerk(this)), 0, 0.12);
		float gunfighter = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusPerks.FIREARMS_GUNFIGHTER);
		pct = Math.Clamp(pct + gunfighter * 0.05, 0, 0.15);
		if (pct <= 0)
			return;
		float maxHp = GetMaxHealth("", "");
		if (maxHp > 0 && GetHealthLevel() < GameConstants.STATE_RUINED)
			AddHealth("", "", maxHp * 0.001 * pct);
		EntityAI suppressor = FindAttachmentBySlotName("weaponMuzzle");
		if (suppressor && suppressor.GetHealthLevel() < GameConstants.STATE_RUINED)
			suppressor.AddHealth("", "", suppressor.GetMaxHealth("", "") * 0.001 * pct);
		#endif
	}

	void ZenPerkPlus_CheckSidearmReady()
	{
		#ifdef SERVER
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableFirearmsSkill || cfg.EnableAutoSidearmSwap)
			return;
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (!player)
			return;
		if (player.GetZenPerkReward(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusPerks.FIREARMS_SIDEARM_READY) <= 0)
			return;
		if (!ZenPerkPlus_IsPrimaryDry())
			return;
		ZenPerkPlusHelpers.Notify(player, "Sidearm Ready", "Primary dry \u2014 check your loaded sidearm.");
		#endif
	}

	bool ZenPerkPlus_IsPrimaryDry()
	{
		int muzzle = GetCurrentMuzzle();
		if (IsChamberFull(muzzle))
			return false;
		Magazine mag = GetMagazine(muzzle);
		if (mag && mag.GetAmmoCount() > 0)
			return false;
		return true;
	}

	override float GetChanceToJam()
	{
		float chance = super.GetChanceToJam();
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (!player)
			return chance;
		return chance * ZenPerkPlusHelpers.GetJamChanceMultiplier(player, this);
	}
}
