// Latest ZenSkills (c1a9255 / 1.29 + ZenModCore):
// - Stock U/highscores loop every db.Skills key and LoadImageFile on missing widgets (NRE).
// - Perk nodes: ZenSkills/data/gui/images/<skillKey>/<slot>_<level>.paa
// - Tab icons: skill_<FirstLetterUppercase>.edds
// HUD toast: skill_<skillKey>.edds (lowercase key)
// Yank add-on keys while Zen's menus Init, then restore. Our menu loads mapped Zen art
// (or custom folders if UseCustomPerkIcons).

class ZenPerkPlusGUICompat
{
	static bool IsAddonSkill(string skillKey)
	{
		return skillKey == ZenPerkPlusSkills.FIREARMS || skillKey == ZenPerkPlusSkills.COMBAT_OPS || skillKey == ZenPerkPlusSkills.DRIVER;
	}

	static void CollectAddonKeys(array<string> keys)
	{
		keys.Clear();
		keys.Insert(ZenPerkPlusSkills.FIREARMS);
		keys.Insert(ZenPerkPlusSkills.COMBAT_OPS);
		keys.Insert(ZenPerkPlusSkills.DRIVER);
	}

	static void YankAddonSkills(map<string, ref ZenSkill> store)
	{
		if (!store)
			return;

		ZenSkillsPlayerDB db = GetZenSkillsPlugin().GetSkillsDB();
		if (!db || !db.Skills)
			return;

		array<string> keys = new array<string>;
		CollectAddonKeys(keys);
		foreach (string key : keys)
		{
			ZenSkill skill = db.Skills.Get(key);
			if (!skill)
				continue;

			store.Set(key, skill);
			db.Skills.Remove(key);
		}
	}

	static void RestoreAddonSkills(map<string, ref ZenSkill> store)
	{
		if (!store)
			return;

		ZenSkillsPlayerDB db = GetZenSkillsPlugin().GetSkillsDB();
		if (!db || !db.Skills)
			return;

		for (int i = 0; i < store.Count(); i++)
		{
			string key = store.GetKey(i);
			ZenSkill skill = store.GetElement(i);
			if (key == "" || !skill)
				continue;
			if (!db.Skills.Contains(key))
				db.Skills.Insert(key, skill);
		}
	}

	static string GetZenArtFolder(string skillKey)
	{
		if (skillKey == ZenPerkPlusSkills.FIREARMS)
			return "hunting";
		if (skillKey == ZenPerkPlusSkills.COMBAT_OPS)
			return "gathering";
		if (skillKey == ZenPerkPlusSkills.DRIVER)
			return "crafting";
		return skillKey;
	}

	static string GetPerkNodeIconPath(string skillKey, string slot, int level)
	{
		if (level < 0)
			level = 0;
		if (level > 3)
			level = 3;

		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (cfg && cfg.UseCustomPerkIcons)
			return "ZenPerkPlus/data/gui/images/" + skillKey + "/" + slot + "_" + level.ToString() + ".paa";

		return "ZenSkills/data/gui/images/" + GetZenArtFolder(skillKey) + "/" + slot + "_" + level.ToString() + ".paa";
	}

	static string GetSkillTabIconPath(string skillKey)
	{
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (cfg && cfg.UseCustomPerkIcons)
			return "ZenPerkPlus/data/gui/images/skill_" + ZenSkillFunctions.FirstLetterUppercase(skillKey) + ".edds";

		string folder = GetZenArtFolder(skillKey);
		return "ZenSkills/data/gui/images/skill_" + ZenSkillFunctions.FirstLetterUppercase(folder) + ".edds";
	}

	static string GetSkillNotifyIconPath(string skillKey)
	{
		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (cfg && cfg.UseCustomPerkIcons)
			return "ZenPerkPlus/data/gui/images/skill_" + skillKey + ".edds";

		string folder = GetZenArtFolder(skillKey);
		return "ZenSkills/data/gui/images/skill_" + ZenSkillFunctions.FirstLetterUppercase(folder) + ".edds";
	}

	static void LoadPerkNodeIcon(ImageWidget iw, string skillKey, string slot, int level)
	{
		if (!iw)
			return;
		iw.LoadImageFile(0, GetPerkNodeIconPath(skillKey, slot, level));
	}
}
