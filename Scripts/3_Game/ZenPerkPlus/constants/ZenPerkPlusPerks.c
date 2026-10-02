// ZenPerkPlus perk slots — same grid language as ZenSkills (tier_slot).
// Tree reading for Gunner/Operator: left column = path A, right = path B.
// Unlock/reset/costs stay in ZenSkills; these are IDs only.

class ZenPerkPlusSkills
{
	static const string FIREARMS = "firearms";		// Gunner
	static const string COMBAT_OPS = "combat_ops";	// Operator
	static const string DRIVER = "driver";			// Wheelman
}

class ZenPerkPlusSlots
{
	static const string SLOT_1_1 = "1_1";
	static const string SLOT_1_2 = "1_2";
	static const string SLOT_1_3 = "1_3";
	static const string SLOT_2_1 = "2_1";
	static const string SLOT_2_2 = "2_2";
	static const string SLOT_3_1 = "3_1";
	static const string SLOT_3_2 = "3_2";
	static const string SLOT_3_3 = "3_3";
	static const string SLOT_4_1 = "4_1";
	static const string SLOT_4_2 = "4_2";
}

class ZenPerkPlusPerks
{
	static const string EXP_BOOST = ZenPerkPlusSlots.SLOT_1_1;

	// GUNNER (firearms) — Left: Run & Gun | Right: Marksman
	static const string FIREARMS_WEAPON_FAMILIAR = ZenPerkPlusSlots.SLOT_1_1;
	static const string FIREARMS_HIP_READY = ZenPerkPlusSlots.SLOT_1_2;
	static const string FIREARMS_STEADY_GRIP = ZenPerkPlusSlots.SLOT_1_3;
	static const string FIREARMS_CLOSE_QUARTERS = ZenPerkPlusSlots.SLOT_2_1;
	static const string FIREARMS_SIDEARM_READY = ZenPerkPlusSlots.SLOT_2_2;
	static const string FIREARMS_CONTROLLED_BURST = ZenPerkPlusSlots.SLOT_3_1;
	static const string FIREARMS_CLEAN_CHAMBER = ZenPerkPlusSlots.SLOT_3_2;
	static const string FIREARMS_FIELD_MAINTENANCE = ZenPerkPlusSlots.SLOT_3_3;
	static const string FIREARMS_GUNFIGHTER = ZenPerkPlusSlots.SLOT_4_1;
	static const string FIREARMS_DEADEYE = ZenPerkPlusSlots.SLOT_4_2;

	// Legacy aliases (existing entity/helper code)
	static const string FIREARMS_SMG_CONTROL = FIREARMS_HIP_READY;
	static const string FIREARMS_SHOTGUN_HANDLING = FIREARMS_STEADY_GRIP;
	static const string FIREARMS_RIFLE_DISCIPLINE = FIREARMS_CLOSE_QUARTERS;
	static const string FIREARMS_MARKSMAN = FIREARMS_CONTROLLED_BURST;
	static const string FIREARMS_CLEAN_SHOOTER = FIREARMS_CLEAN_CHAMBER;
	static const string FIREARMS_RUN_AND_GUN = FIREARMS_GUNFIGHTER;
	static const string FIREARMS_MASTER_GUNFIGHTER = FIREARMS_DEADEYE;

	// OPERATOR (combat_ops) — Left: Scout | Right: Field Medic
	static const string COMBAT_OPS_FOUNDATION = ZenPerkPlusSlots.SLOT_1_1;
	static const string COMBAT_SECOND_WIND = ZenPerkPlusSlots.SLOT_1_2;
	static const string COMBAT_QUICK_WRAP = ZenPerkPlusSlots.SLOT_1_3;
	static const string COMBAT_LIGHT_STEP = ZenPerkPlusSlots.SLOT_2_1;
	static const string COMBAT_FIELD_SPLINT = ZenPerkPlusSlots.SLOT_2_2;
	static const string COMBAT_LONG_PUSH = ZenPerkPlusSlots.SLOT_3_1;
	static const string COMBAT_RADIO_DISCIPLINE = ZenPerkPlusSlots.SLOT_3_2;
	static const string COMBAT_STAY_WITH_ME = ZenPerkPlusSlots.SLOT_3_3;
	static const string COMBAT_GHOST_PACE = ZenPerkPlusSlots.SLOT_4_1;
	static const string COMBAT_COMBAT_MEDIC = ZenPerkPlusSlots.SLOT_4_2;

	static const string COMBAT_AI_HUNTER = COMBAT_SECOND_WIND;
	static const string COMBAT_SURVIVORS_EDGE = COMBAT_QUICK_WRAP;
	static const string COMBAT_ADRENALINE_CONTROL = COMBAT_LIGHT_STEP;
	static const string COMBAT_KILLER_FOCUS = COMBAT_FIELD_SPLINT;
	static const string COMBAT_BLOOD_TRAIL = COMBAT_LONG_PUSH;
	static const string COMBAT_SUPPRESSION_VETERAN = COMBAT_STAY_WITH_ME;
	static const string COMBAT_OPERATOR = COMBAT_GHOST_PACE;
	static const string COMBAT_MANHUNTER = COMBAT_COMBAT_MEDIC;

	// WHEELMAN (driver) — linear
	static const string DRIVER_FOUNDATION = ZenPerkPlusSlots.SLOT_1_1;
	static const string DRIVER_SOFT_HANDS = ZenPerkPlusSlots.SLOT_1_2;
	static const string DRIVER_ROAD_SENSE = ZenPerkPlusSlots.SLOT_1_3;
	static const string DRIVER_IRON_CHASSIS = ZenPerkPlusSlots.SLOT_2_1;
	static const string DRIVER_FUEL_SAVER = ZenPerkPlusSlots.SLOT_2_2;
	static const string DRIVER_CRASH_CONTROL = ZenPerkPlusSlots.SLOT_3_1;
	static const string DRIVER_FIELD_MECHANIC = ZenPerkPlusSlots.SLOT_3_2;
	static const string DRIVER_BATTERY_CARE = ZenPerkPlusSlots.SLOT_3_3;
	static const string DRIVER_CONVOY = ZenPerkPlusSlots.SLOT_4_1;
	static const string DRIVER_GETAWAY = ZenPerkPlusSlots.SLOT_4_2;

	static const string DRIVER_SMOOTH_START = DRIVER_SOFT_HANDS;
	static const string DRIVER_MECHANICS_EAR = DRIVER_IRON_CHASSIS;
	static const string DRIVER_CONVOY_DRIVER = DRIVER_CONVOY;
	static const string DRIVER_MASTER_DRIVER = DRIVER_GETAWAY;
}

class ZenPerkPlusActions
{
	static const string COMBAT_RADIO_WORK_START = "ZenPerkPlus_CombatRadioWorkStart";
	static const string COMBAT_RADIO_WORK_STOP = "ZenPerkPlus_CombatRadioWorkStop";
	static const string COMBAT_RADIO_STATIC = "ZenPerkPlus_CombatRadioStatic";
	static const string KILLED_AI = "ZenPerkPlus_Killed_AI";
	static const string KILLED_INFECTED = "ZenPerkPlus_Killed_Infected";
	static const string KILLED_PLAYER = "ZenPerkPlus_Killed_Player";
	static const string FIRED_SMG = "ZenPerkPlus_Fired_SMG";
	static const string FIRED_RIFLE = "ZenPerkPlus_Fired_Rifle";
	static const string FIRED_SNIPER = "ZenPerkPlus_Fired_Sniper";
	static const string FIRED_SHOTGUN = "ZenPerkPlus_Fired_Shotgun";
	static const string FIRED_PISTOL = "ZenPerkPlus_Fired_Pistol";
	static const string KILLED_WITH_FIREARM = "ZenPerkPlus_Killed_With_Firearm";
	static const string DRIVER_DISTANCE = "ZenPerkPlus_DriverDistance";
	static const string DRIVER_REPAIR = "ZenPerkPlus_DriverRepair";
}
