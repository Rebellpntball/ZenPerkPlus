// Injects Gunner / Operator / Wheelman as Zen-style skill trees.
// Progression (EXP cost, max perks, reset, refund) stays under ZenSkills control.

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

		ZenPerkPlus cfg = GetZenPerkPlusConfig();
		if (!cfg || cfg.EnableFirearmsSkill)
			ZenPerkPlus_EnsureFirearmsSkill();
		if (!cfg || cfg.EnableCombatOpsSkill)
			ZenPerkPlus_EnsureCombatOpsSkill();
		if (!cfg || cfg.EnableDriverSkill)
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
		skill.EXP_Per_Perk = 1000;
		skill.EXP_Perk_Modifier = 0.01;
		skill.EXP_Refund_Modifier = 0.5;
		skill.MaxAllowedPerks = 8;
		if (!skill.Perks)
			skill.Perks = new map<string, ref ZenPerkDef>();
	}

	void ZenPerkPlus_EnsureFirearmsSkill()
	{
		ZenSkillDef skill = ZenPerkPlus_GetOrCreateSkillDef(ZenPerkPlusSkills.FIREARMS, "Gunner", "Left: run and gun. Right: marksman. Reset uses ZenSkills rules.");
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_1, new ZenPerkDef("Weapon Familiar", "Baseline Gunner training. Improves Firearms EXP gain.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_2, new ZenPerkDef("Hip Ready", "Run & Gun: faster ready from the hip / close-in handling.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_3, new ZenPerkDef("Steady Grip", "Marksman: mild ADS sway reduction.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_2_1, new ZenPerkDef("Close Quarters", "Run & Gun: better control while moving in close range.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_2_2, new ZenPerkDef("Sidearm Ready", "Shared: notice when a loaded sidearm is available if primary runs dry.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_1, new ZenPerkDef("Controlled Burst", "Run & Gun: less recoil climb on rapid fire.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_2, new ZenPerkDef("Clean Chamber", "Marksman: fewer jams on aimed fire.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_3, new ZenPerkDef("Field Maintenance", "Shared: reduced weapon and suppressor wear.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_4_1, new ZenPerkDef("Gunfighter", "Run & Gun signature: strong close-quarters control.", "%", 8, 12, 18));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_4_2, new ZenPerkDef("Deadeye", "Marksman signature: strong steady ADS; optional tactical zoom when configured.", "%", 8, 12, 18));
	}

	void ZenPerkPlus_EnsureCombatOpsSkill()
	{
		ZenSkillDef skill = ZenPerkPlus_GetOrCreateSkillDef(ZenPerkPlusSkills.COMBAT_OPS, "Operator", "Left: scout. Right: field medic. Reset uses ZenSkills rules.");
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_1, new ZenPerkDef("Ops Training", "Baseline Operator training. Improves Combat Ops EXP gain.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_2, new ZenPerkDef("Second Wind", "Scout: faster stamina recovery after sprint.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_3, new ZenPerkDef("Quick Wrap", "Field Medic: faster bandage / rag actions.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_2_1, new ZenPerkDef("Light Step", "Scout: quieter movement / slightly lower infected notice.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_2_2, new ZenPerkDef("Field Splint", "Field Medic: faster splint and limb care.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_1, new ZenPerkDef("Long Push", "Scout: sprint drains slower briefly after combat or radio.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_2, new ZenPerkDef("Radio Discipline", "Shared: powered radios can give vague combat static warnings.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_3, new ZenPerkDef("Stay With Me", "Field Medic: mild shock / recovery support.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_4_1, new ZenPerkDef("Ghost Pace", "Scout signature: strong post-fight stamina recovery (not infinite sprint).", "%", 8, 12, 18));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_4_2, new ZenPerkDef("Combat Medic", "Field Medic signature: faster core patch-up under pressure.", "%", 8, 12, 18));
	}

	void ZenPerkPlus_EnsureDriverSkill()
	{
		ZenSkillDef skill = ZenPerkPlus_GetOrCreateSkillDef(ZenPerkPlusSkills.DRIVER, "Wheelman", "Linear vehicle tree. Reset uses ZenSkills rules.");
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_1, new ZenPerkDef("Driver Training", "Baseline Wheelman training. Improves Driver EXP gain.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_2, new ZenPerkDef("Soft Hands", "Less player damage from minor crashes while driving.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_1_3, new ZenPerkDef("Road Sense", "Better Driver EXP from stable distance driven.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_2_1, new ZenPerkDef("Iron Chassis", "Vehicle takes less crash damage while you drive.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_2_2, new ZenPerkDef("Fuel Saver", "Mild fuel efficiency while driving.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_1, new ZenPerkDef("Crash Control", "Stronger crash/contact damage reduction while driving.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_2, new ZenPerkDef("Field Mechanic", "Bonus support for vehicle part install and repair EXP.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_3_3, new ZenPerkDef("Battery Care", "Reduced battery drain where safe.", "%", 5, 10, 15));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_4_1, new ZenPerkDef("Convoy Driver", "Top-tier vehicle operation and durability support.", "%", 8, 12, 18));
		ZenPerkPlus_EnsurePerk(skill, ZenPerkPlusSlots.SLOT_4_2, new ZenPerkDef("Getaway", "Wheelman signature: strong crash mitigation when it counts.", "%", 8, 12, 18));
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
