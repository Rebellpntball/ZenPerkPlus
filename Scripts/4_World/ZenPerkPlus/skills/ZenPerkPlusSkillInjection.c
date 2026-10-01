modded class ZenSkillsConfig
{
	override void SetDefaults()
	{
		super.SetDefaults();
		ZenPerkPlus_VerifySkillDefs();
	}

	override void AfterLoad()
	{
		super.AfterLoad();
		ZenPerkPlus_VerifySkillDefs();
	}

	void ZenPerkPlus_VerifySkillDefs()
	{
		if (!SkillDefs)
			SkillDefs = new map<string, ref ZenSkillDef>();

		ZenPerkPlus_EnsureFirearmsSkill();
		ZenPerkPlus_EnsureCombatOpsSkill();
		ZenPerkPlus_EnsureDriverSkill();
	}

	ZenSkillDef ZenPerkPlus_GetOrCreateSkillDef(string key, string name, string desc)
	{
		ZenSkillDef skill = SkillDefs.Get(key);
		if (!skill)
		{
			skill = new ZenSkillDef(name, desc);
			ZenPerkPlus_ApplyZenDefaults(skill);
			SkillDefs.Insert(key, skill);
		}
		else if (!skill.Perks)
		{
			skill.Perks = new map<string, ref ZenPerkDef>();
		}

		return skill;
	}

	void ZenPerkPlus_ApplyZenDefaults(ZenSkillDef skill)
	{
		// Match ZenSkills' ZenSkillDef constructor defaults for newly created add-on skill trees.
		skill.EXP_Per_Perk = 1000;
		skill.EXP_Perk_Modifier = 0.01;
		skill.EXP_Refund_Modifier = 0.5;
		skill.MaxAllowedPerks = 20;

		if (!skill.Perks)
			skill.Perks = new map<string, ref ZenPerkDef>();
	}

	void ZenPerkPlus_EnsureFirearmsSkill()
	{
		ZenSkillDef skill = ZenPerkPlus_GetOrCreateSkillDef(ZenPerkPlusSkills.FIREARMS, "Firearms", "Weapon handling, firearm reliability, combat movement, and field maintenance.");
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_1, new ZenPerkDef("Firearms Training", "Gain more Firearms EXP.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_2, new ZenPerkDef("SMG Control", "SMG reliability and wear bonus.", "%", 3, 7, 10));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_3, new ZenPerkDef("Shotgun Handling", "Shotgun reliability and handling bonus.", "%", 3, 7, 10));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_2_1, new ZenPerkDef("Rifle Discipline", "Rifle reliability and ammo discipline bonus.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_2_2, new ZenPerkDef("Sidearm Ready", "Warns when a loaded sidearm or shotgun is available while your current gun runs dry.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_1, new ZenPerkDef("Marksman", "Rifle and precision weapon reliability bonus.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_2, new ZenPerkDef("Clean Shooter", "Reduced weapon and suppressor wear.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_3, new ZenPerkDef("Field Maintenance", "Stronger jam reduction and maintenance bonus.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_4_1, new ZenPerkDef("Run and Gun", "SMG movement-combat reliability bonus; sprint shooting remains disabled in v1.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_4_2, new ZenPerkDef("Master Gunfighter", "Enables tactical eye zoom while unraised or sprinting when configured.", "%", 5, 10, 15));
	}

	void ZenPerkPlus_EnsureCombatOpsSkill()
	{
		ZenSkillDef skill = ZenPerkPlus_GetOrCreateSkillDef(ZenPerkPlusSkills.COMBAT_OPS, "Combat Ops", "AI/PVE combat, PVP kills, radio discipline, tracking, and post-combat recovery.");
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_1, new ZenPerkDef("Combat Training", "Gain more Combat Ops EXP.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_2, new ZenPerkDef("AI Hunter", "Bonus rewards from infected and optional Expansion AI kills.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_3, new ZenPerkDef("Survivor's Edge", "Bonus rewards from PVP kills.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_2_1, new ZenPerkDef("Adrenaline Control", "Post-AI-kill recovery support.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_2_2, new ZenPerkDef("Killer Focus", "Post-PVP-kill recovery support.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_1, new ZenPerkDef("Radio Discipline", "Powered radios provide vague combat radio-static warnings.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_2, new ZenPerkDef("Blood Trail", "Vague wounded-target awareness without exact markers.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_3, new ZenPerkDef("Suppression Veteran", "Timed recovery support after combat kills.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_4_1, new ZenPerkDef("Operator", "Top-tier AI/PVE rewards and radio discipline bonuses without exact locations.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_4_2, new ZenPerkDef("Manhunter", "Top-tier PVP rewards and blood trail bonuses without exact locations.", "%", 5, 10, 15));
	}

	void ZenPerkPlus_EnsureDriverSkill()
	{
		ZenSkillDef skill = ZenPerkPlus_GetOrCreateSkillDef(ZenPerkPlusSkills.DRIVER, "Driver", "Vehicle operation, vehicle awareness, crash control, battery/fuel care, and road survival.");
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_1, new ZenPerkDef("Driver Training", "Gain more Driver EXP.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_2, new ZenPerkDef("Smooth Start", "Vehicle start and driving efficiency bonus.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_3, new ZenPerkDef("Road Sense", "Driver EXP from stable driving time and distance.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_2_1, new ZenPerkDef("Mechanic's Ear", "Warns about damaged vehicle parts.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_2_2, new ZenPerkDef("Fuel Saver", "Reduced fuel and coolant loss where safe.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_1, new ZenPerkDef("Crash Control", "Reduces crash/contact damage while the perk owner is driving.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_2, new ZenPerkDef("Field Mechanic", "Bonus EXP and support for vehicle part install and repair.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_3, new ZenPerkDef("Battery Care", "Reduced battery drain or better battery handling where safe.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_4_1, new ZenPerkDef("Convoy Driver", "Top-tier vehicle operation and durability support; speed boost is disabled by default.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_4_2, new ZenPerkDef("Master Driver", "Top-tier vehicle survival support; grip/speed assists are experimental and disabled by default.", "%", 5, 10, 15));
	}

	void ZenPerkPlus_EnsurePerk(ZenSkillDef skill, string slot, ZenPerkDef perkDef)
	{
		if (!skill.Perks.Contains(slot))
			skill.Perks.Insert(slot, perkDef);
	}
}

modded class ZenSkillsEXP
{
	override void SetDefaults()
	{
		super.SetDefaults();
		ZenPerkPlus_VerifyEXPDefs();
	}

	override void AfterLoad()
	{
		super.AfterLoad();
		ZenPerkPlus_VerifyEXPDefs();
	}

	void ZenPerkPlus_VerifyEXPDefs()
	{
		if (!ExpDefs)
			ExpDefs = new map<string, ref ZenSkillsEXPDefHolder>();

		ZenPerkPlus_EnsureEXPDef(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusActions.COMBAT_RADIO_WORK_START, new ZenSkillsEXPDef(5));
		ZenPerkPlus_EnsureEXPDef(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusActions.COMBAT_RADIO_WORK_STOP, new ZenSkillsEXPDef(1));
		ZenPerkPlus_EnsureEXPDef(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusActions.COMBAT_RADIO_STATIC, new ZenSkillsEXPDef(5, true));
		ZenPerkPlus_EnsureEXPDef(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusActions.KILLED_AI, new ZenSkillsEXPDef(25, true));
		ZenPerkPlus_EnsureEXPDef(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusActions.KILLED_INFECTED, new ZenSkillsEXPDef(25, true));
		ZenPerkPlus_EnsureEXPDef(ZenPerkPlusSkills.COMBAT_OPS, ZenPerkPlusActions.KILLED_PLAYER, new ZenSkillsEXPDef(100, true));
		ZenPerkPlus_EnsureEXPDef(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusActions.FIRED_SMG, new ZenSkillsEXPDef(1, true));
		ZenPerkPlus_EnsureEXPDef(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusActions.FIRED_RIFLE, new ZenSkillsEXPDef(1, true));
		ZenPerkPlus_EnsureEXPDef(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusActions.FIRED_SNIPER, new ZenSkillsEXPDef(1, true));
		ZenPerkPlus_EnsureEXPDef(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusActions.FIRED_SHOTGUN, new ZenSkillsEXPDef(1, true));
		ZenPerkPlus_EnsureEXPDef(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusActions.FIRED_PISTOL, new ZenSkillsEXPDef(1, true));
		ZenPerkPlus_EnsureEXPDef(ZenPerkPlusSkills.FIREARMS, ZenPerkPlusActions.KILLED_WITH_FIREARM, new ZenSkillsEXPDef(20, true));
		ZenPerkPlus_EnsureEXPDef(ZenPerkPlusSkills.DRIVER, ZenPerkPlusActions.DRIVER_DISTANCE, new ZenSkillsEXPDef(1, true));
		ZenPerkPlus_EnsureEXPDef(ZenPerkPlusSkills.DRIVER, ZenPerkPlusActions.DRIVER_REPAIR, new ZenSkillsEXPDef(10, true));
	}

	void ZenPerkPlus_EnsureEXPDef(string skillKey, string actionKey, ZenSkillsEXPDef expDef)
	{
		ZenSkillsEXPDefHolder holder = ZenPerkPlus_GetOrCreateHolder(skillKey);
		if (!holder.ExpDefs.Contains(actionKey))
			holder.ExpDefs.Insert(actionKey, expDef);
	}

	ZenSkillsEXPDefHolder ZenPerkPlus_GetOrCreateHolder(string skillKey)
	{
		ZenSkillsEXPDefHolder holder;
		if (ExpDefs.Find(skillKey, holder))
			return holder;

		holder = new ZenSkillsEXPDefHolder();
		ExpDefs.Insert(skillKey, holder);
		return holder;
	}
}
