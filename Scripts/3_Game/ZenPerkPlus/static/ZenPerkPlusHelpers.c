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
		float operatorPct = player.GetZenPerkRewardPercent01(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusPerks.COMBAT_OPERATOR);
		return range * (1.0 + pct + operatorPct);
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

	static string GetFirearmSkillKey(EntityAI weapon)
	{
		if (!weapon || !weapon.IsWeapon())
			return "";

		return ZenPerkPlusSkills.FIREARMS;
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
		if (category == "shotgun") return ZenPerkPlusPerks.FIREARMS_SHOTGUN_HANDLING;
		if (category == "smg") return ZenPerkPlusPerks.FIREARMS_SMG_CONTROL;
		if (category == "sniper") return ZenPerkPlusPerks.FIREARMS_MARKSMAN;
		if (category == "rifle") return ZenPerkPlusPerks.FIREARMS_RIFLE_DISCIPLINE;
		return ZenPerkPlusPerks.FIREARMS_FIELD_MAINTENANCE;
	}

	static string GetFirearmWearPerk(EntityAI weapon)
	{
		string category = GetFirearmCategory(weapon);
		if (category == "shotgun") return ZenPerkPlusPerks.FIREARMS_SHOTGUN_HANDLING;
		if (category == "smg") return ZenPerkPlusPerks.FIREARMS_SMG_CONTROL;
		return ZenPerkPlusPerks.FIREARMS_CLEAN_SHOOTER;
	}

	static string GetFirearmKillAction(EntityAI weapon)
	{
		if (!weapon || !weapon.IsWeapon())
			return "";

		return ZenPerkPlusActions.KILLED_WITH_FIREARM;
	}
}
