modded class Weapon_Base
{
	override void EEFired(int muzzleType, int mode, string ammoType)
	{
		super.EEFired(muzzleType, mode, ammoType);
		ZenPerkPlus_AwardFirearmShotEXP();
		ZenPerkPlus_ApplySafeDurabilityRefund();
		ZenPerkPlus_CheckSidearmReady();
		// TODO: Investigate true sprint shooting only behind EnableExperimentalSprintShooting in a later optional pass.
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

		float pct = Math.Clamp(player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusHelpers.GetFirearmWearPerk(this)), 0, 0.10);
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
		if (!cfg || cfg.EnableAutoSidearmSwap)
			return;

		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (!player)
			return;

		if (player.GetZenPerkReward(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusPerks.FIREARMS_SIDEARM_READY) <= 0)
			return;

		// Safe v1: notification-only. No automatic inventory/weapon-manager swap is attempted.
		ZenPerkPlusHelpers.Notify(player, "Sidearm Ready", "If your weapon runs dry, check your loaded sidearm or shotgun.");
		#endif
	}

	override float GetChanceToJam()
	{
		float chance = super.GetChanceToJam();
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableFirearmsSkill || !cfg.FirearmJamReductionEnabled)
			return chance;

		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (!player)
			return chance;

		float categoryReduction = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusHelpers.GetFirearmReliabilityPerk(this));
		float maintenanceReduction = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusPerks.FIREARMS_FIELD_MAINTENANCE);
		float reduction = Math.Clamp(categoryReduction + maintenanceReduction, 0, 0.15);
		return chance * (1.0 - reduction);
	}
}
