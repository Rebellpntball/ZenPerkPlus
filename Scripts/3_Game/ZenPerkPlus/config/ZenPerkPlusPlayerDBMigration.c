modded class ZenSkillsPlayerDB
{
	override void SetDefinitions(bool firstInit = false)
	{
		ZenPerkPlus_EnsureAllSkillDefEntries();
		super.SetDefinitions(firstInit);
	}

	void ZenPerkPlus_EnsureAllSkillDefEntries()
	{
		if (!Skills || !GetZenSkillsConfig() || !GetZenSkillsConfig().SkillDefs)
			return;

		foreach (string skillKey, ZenSkillDef skillDef : GetZenSkillsConfig().SkillDefs)
		{
			ZenPerkPlus_EnsureSkillEntry(skillKey, skillDef);
		}
	}

	void ZenPerkPlus_EnsureSkillEntry(string skillKey, ZenSkillDef skillDef)
	{
		if (!skillDef)
			return;

		ZenSkill skill = Skills.Get(skillKey);
		if (!skill)
		{
			skill = new ZenSkill(skillDef.StartingEXP);
			Skills.Set(skillKey, skill);
		}

		if (!skill.Perks)
			skill.Perks = new map<string, ref ZenPerk>();

		foreach (string perkKey, ZenPerkDef perkDef : skillDef.Perks)
		{
			if (!skill.Perks.Contains(perkKey))
				skill.Perks.Set(perkKey, new ZenPerk());
		}
	}
}
