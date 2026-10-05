class ZenPerkPlusHelpers
{
	static bool IsRadioLike(EntityAI item)
	{
		if (!item)
			return false;
		return item.IsInherited(TransmitterBase) || item.IsTransmitter();
	}

	static bool IsPoweredRadio(EntityAI item)
	{
		if (!IsRadioLike(item))
			return false;
		ComponentEnergyManager em = item.GetCompEM();
		return em && em.IsWorking();
	}

	static EntityAI FindPoweredRadio(EntityAI root)
	{
		if (!root)
			return null;
		PlayerBase player = PlayerBase.Cast(root);
		if (player && player.GetHumanInventory())
		{
			EntityAI hands = player.GetHumanInventory().GetEntityInHands();
			if (IsPoweredRadio(hands))
				return hands;
		}
		array<EntityAI> items = new array<EntityAI>();
		root.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item : items)
		{
			if (IsPoweredRadio(item))
				return item;
		}
		return null;
	}

	static bool CanReceiveCombatRadio(PlayerBase player)
	{
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.CombatRadioRequiresPoweredRadio)
			return true;
		return FindPoweredRadio(player) != null;
	}

	static float GetCombatRadioRange(PlayerBase player)
	{
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg)
			return 0;
		float range = cfg.CombatRadioRangeMeters;
		float pct = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusPerks.COMBAT_RADIO_DISCIPLINE);
		float ghostPct = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusPerks.COMBAT_GHOST_PACE);
		return range * (1.0 + pct + ghostPct * 0.5);
	}

	static void Notify(PlayerBase player, string title, string text)
	{
		if (!player)
			return;
		string message = text;
		if (title != "")
			message = title + ": " + text;
		ZenSkillFunctions.SendPlayerMessage(player, message);
	}

	static void AwardAction(PlayerBase player, string actionKey, float modifier = 1.0)
	{
		#ifdef SERVER
		if (player && actionKey != "")
			GetZenSkillsPlugin().AddEXP_Action(player, actionKey, modifier);
		#endif
	}

	static void AwardRaw(PlayerBase player, string skillKey, int exp)
	{
		#ifdef SERVER
		if (player && skillKey != "" && exp > 0)
			GetZenSkillsPlugin().AddEXP(player, skillKey, exp, "ZenPerkPlus", true, false);
		#endif
	}

	static PlayerBase GetPlayerFromKiller(Object killer)
	{
		PlayerBase player = PlayerBase.Cast(killer);
		if (player)
			return player;
		EntityAI entity = EntityAI.Cast(killer);
		if (entity)
			return PlayerBase.Cast(entity.GetHierarchyRootPlayer());
		return null;
	}

	static bool TypeMatches(array<string> typeList, string type)
	{
		if (!typeList)
			return false;
		type.ToLower();
		foreach (string token : typeList)
		{
			token.ToLower();
			if (token != "" && type.Contains(token))
				return true;
		}
		return false;
	}

	static string GetFirearmCategory(EntityAI weapon)
	{
		if (!weapon || !weapon.IsWeapon())
			return "";
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		string type = weapon.GetType();
		type.ToLower();
		if (cfg)
		{
			if (TypeMatches(cfg.ShotgunTypes, type)) return "shotgun";
			if (TypeMatches(cfg.SniperTypes, type)) return "sniper";
			if (TypeMatches(cfg.SMGTypes, type)) return "smg";
			if (TypeMatches(cfg.PistolTypes, type)) return "pistol";
			if (TypeMatches(cfg.RifleTypes, type)) return "rifle";
		}
		return "rifle";
	}

	static string GetFirearmShotAction(EntityAI weapon)
	{
		string category = GetFirearmCategory(weapon);
		if (category == "shotgun") return ZenPerkPlusActions.FIRED_SHOTGUN;
		if (category == "sniper") return ZenPerkPlusActions.FIRED_SNIPER;
		if (category == "smg") return ZenPerkPlusActions.FIRED_SMG;
		if (category == "pistol") return ZenPerkPlusActions.FIRED_PISTOL;
		if (category == "rifle") return ZenPerkPlusActions.FIRED_RIFLE;
		return "";
	}

	static string GetFirearmReliabilityPerk(EntityAI weapon)
	{
		string category = GetFirearmCategory(weapon);
		if (category == "smg" || category == "pistol" || category == "shotgun")
			return ZenPerkPlusPerks.FIREARMS_HIP_READY;
		if (category == "sniper")
			return ZenPerkPlusPerks.FIREARMS_CLEAN_CHAMBER;
		return ZenPerkPlusPerks.FIREARMS_CONTROLLED_BURST;
	}

	static string GetFirearmWearPerk(EntityAI weapon)
	{
		return ZenPerkPlusPerks.FIREARMS_FIELD_MAINTENANCE;
	}

	static string GetFirearmKillAction(EntityAI weapon)
	{
		if (!weapon || !weapon.IsWeapon())
			return "";
		return ZenPerkPlusActions.KILLED_WITH_FIREARM;
	}

	static float GetStaminaDrainMultiplier(PlayerBase player)
	{
		if (!player)
			return 1.0;
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableCombatOpsSkill)
			return 1.0;
		float secondWind = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusPerks.COMBAT_SECOND_WIND);
		float longPush = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusPerks.COMBAT_LONG_PUSH);
		float ghost = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusPerks.COMBAT_GHOST_PACE);
		float reduce = Math.Clamp(secondWind * 0.15 + longPush * 0.20 + ghost * 0.25, 0, cfg.ScoutMaxStaminaDrainReduce);
		return Math.Clamp(1.0 - reduce, 0.55, 1.0);
	}

	static float GetMedicActionTimeMultiplier(PlayerBase player)
	{
		if (!player)
			return 1.0;
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableCombatOpsSkill)
			return 1.0;
		float wrap = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusPerks.COMBAT_QUICK_WRAP);
		float splint = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusPerks.COMBAT_FIELD_SPLINT);
		float medic = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusPerks.COMBAT_COMBAT_MEDIC);
		float reduce = Math.Clamp(wrap * 0.20 + splint * 0.15 + medic * 0.25, 0, cfg.MedicMaxActionSpeedReduce);
		return Math.Clamp(1.0 - reduce, 0.50, 1.0);
	}

	static float GetCrashDamageMultiplier(PlayerBase player)
	{
		if (!player)
			return 1.0;
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableDriverSkill || !cfg.EnableCrashControl)
			return 1.0;
		float soft = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.DRIVER, ZenPerkPlusPerks.DRIVER_SOFT_HANDS);
		float chassis = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.DRIVER, ZenPerkPlusPerks.DRIVER_IRON_CHASSIS);
		float crash = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.DRIVER, ZenPerkPlusPerks.DRIVER_CRASH_CONTROL);
		float getaway = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.DRIVER, ZenPerkPlusPerks.DRIVER_GETAWAY);
		float reduce = Math.Clamp(soft * 0.10 + chassis * 0.15 + crash * 0.20 + getaway * 0.25 + (cfg.CrashControlDamageReductionPercent * 0.01), 0, 0.55);
		return Math.Clamp(1.0 - reduce, 0.45, 1.0);
	}

	static float GetJamChanceMultiplier(PlayerBase player, EntityAI weapon)
	{
		if (!player)
			return 1.0;
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || !cfg.EnableFirearmsSkill || !cfg.FirearmJamReductionEnabled)
			return 1.0;
		float cat = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.FIREARMS, GetFirearmReliabilityPerk(weapon));
		float clean = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusPerks.FIREARMS_CLEAN_CHAMBER);
		float maint = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusPerks.FIREARMS_FIELD_MAINTENANCE);
		float gunfighter = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusPerks.FIREARMS_GUNFIGHTER);
		float reduce = Math.Clamp(cat + clean + maint * 0.5 + gunfighter * 0.3, 0, 0.35);
		return Math.Clamp(1.0 - reduce, 0.65, 1.0);
	}
}
