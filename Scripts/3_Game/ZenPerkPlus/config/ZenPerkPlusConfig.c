ref ZenPerkPlus g_ZenPerkPlusConfig;

static ZenPerkPlus GetZenPerkPlusConfig()
{
	if (!g_ZenPerkPlusConfig) GetZenConfigRegister().RegisterConfig(ZenPerkPlus);
	return g_ZenPerkPlusConfig;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload();
		RegisterType(ZenPerkPlus);
	}
}

class ZenPerkPlus: ZenConfigBase
{
	int ConfigVersion;
	bool EnableFirearmsSkill;
	bool EnableCombatOpsSkill;
	bool EnableDriverSkill;
	bool EnableKillNotifications;
	bool EnableDebugLogging;

	bool EnableSprintZoom;
	bool EnableExperimentalSprintShooting;
	bool EnableAutoSidearmSwap;
	bool SidearmReadyAllowShotguns;
	float RifleDisciplineChance;
	bool RifleDisciplineCanCreateAmmo;
	int FirearmShotEXP;
	int FirearmKillEXP;
	ref array<string> SMGTypes;
	ref array<string> RifleTypes;
	ref array<string> SniperTypes;
	ref array<string> ShotgunTypes;
	ref array<string> PistolTypes;
	bool FirearmJamReductionEnabled;
	bool FirearmWearReductionEnabled;

	bool EnableExpansionAIKillEXP;
	int ExpansionAIKillCombatOpsEXP;
	int InfectedKillCombatOpsEXP;
	int PlayerKillCombatOpsEXP;
	int AnimalKillCombatOpsEXP;
	bool EnableCombatRadioStatic;
	bool CombatRadioRequiresPoweredRadio;
	float CombatRadioRangeMeters;
	float CombatRadioCooldownSeconds;
	bool CombatRadioDetectPlayers;
	bool CombatRadioDetectExpansionAI;
	int CombatRadioWorkStartEXP;
	int CombatRadioWorkStopEXP;
	bool EnableBloodTrail;
	float BloodTrailRangeMeters;
	float BloodTrailCooldownSeconds;
	bool BloodTrailExactMarkers;
	float SuppressionVeteranBuffSeconds;
	float SuppressionVeteranHealthRegenBonus;
	float SuppressionVeteranBloodRegenBonus;
	float OperatorAIExpBonusPercent;
	float ManhunterPVPExpBonusPercent;

	bool EnableDriverEXP;
	int DriverDistanceEXP;
	float DriverDistanceMetersPerEXP;
	bool EnableMechanicsEar;
	float MechanicsEarCooldownSeconds;
	bool EnableCrashControl;
	float CrashControlDamageReductionPercent;
	bool EnableFuelSaver;
	bool EnableBatteryCare;
	bool EnableVehicleSpeedBoost;
	float ConvoyDriverSpeedBoostPercent;
	bool EnableExperimentalVehicleGripAssist;
	float MasterDriverGripBoostPercent;
	float MasterDriverSpeedBoostPercent;

	override void OnRegistered()
	{
		g_ZenPerkPlusConfig = this;
	}

	override string GetFolderName() { return "ZenPerkPlus"; }
	override string GetCurrentVersion() { return "2"; }
	override bool ShouldLoadOnServer() { return true; }
	override bool ShouldSyncToClient() { return true; }

	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenPerkPlus>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenPerkPlus>.SaveFile(path, this, err);
	}

	override void SetDefaults()
	{
		ConfigVersion = 2;
		EnableFirearmsSkill = true;
		EnableCombatOpsSkill = true;
		EnableDriverSkill = true;
		EnableKillNotifications = true;
		EnableDebugLogging = false;

		EnableSprintZoom = true;
		EnableExperimentalSprintShooting = false;
		EnableAutoSidearmSwap = false;
		SidearmReadyAllowShotguns = true;
		RifleDisciplineChance = 0.75;
		RifleDisciplineCanCreateAmmo = false;
		FirearmShotEXP = 1;
		FirearmKillEXP = 20;
		SMGTypes = new array<string>();
		SMGTypes.Insert("mp5");
		SMGTypes.Insert("ump");
		SMGTypes.Insert("pp19");
		SMGTypes.Insert("vikhr");
		RifleTypes = new array<string>();
		RifleTypes.Insert("ak");
		RifleTypes.Insert("m4");
		RifleTypes.Insert("fal");
		RifleTypes.Insert("aug");
		RifleTypes.Insert("sks");
		SniperTypes = new array<string>();
		SniperTypes.Insert("svd");
		SniperTypes.Insert("mosin");
		SniperTypes.Insert("winchester");
		SniperTypes.Insert("b95");
		SniperTypes.Insert("m70");
		ShotgunTypes = new array<string>();
		ShotgunTypes.Insert("shotgun");
		ShotgunTypes.Insert("saiga");
		ShotgunTypes.Insert("izh43");
		ShotgunTypes.Insert("mp133");
		PistolTypes = new array<string>();
		PistolTypes.Insert("ij70");
		PistolTypes.Insert("fnx");
		PistolTypes.Insert("cz75");
		PistolTypes.Insert("glock");
		PistolTypes.Insert("longhorn");
		PistolTypes.Insert("magnum");
		FirearmJamReductionEnabled = true;
		FirearmWearReductionEnabled = true;

		EnableExpansionAIKillEXP = true;
		ExpansionAIKillCombatOpsEXP = 25;
		InfectedKillCombatOpsEXP = 25;
		PlayerKillCombatOpsEXP = 100;
		AnimalKillCombatOpsEXP = 5;
		EnableCombatRadioStatic = true;
		CombatRadioRequiresPoweredRadio = true;
		CombatRadioRangeMeters = 300;
		CombatRadioCooldownSeconds = 180;
		CombatRadioDetectPlayers = false;
		CombatRadioDetectExpansionAI = true;
		CombatRadioWorkStartEXP = 5;
		CombatRadioWorkStopEXP = 1;
		EnableBloodTrail = true;
		BloodTrailRangeMeters = 75;
		BloodTrailCooldownSeconds = 120;
		BloodTrailExactMarkers = false;
		SuppressionVeteranBuffSeconds = 120;
		SuppressionVeteranHealthRegenBonus = 0.25;
		SuppressionVeteranBloodRegenBonus = 0.25;
		OperatorAIExpBonusPercent = 15;
		ManhunterPVPExpBonusPercent = 15;

		EnableDriverEXP = true;
		DriverDistanceEXP = 1;
		DriverDistanceMetersPerEXP = 1000;
		EnableMechanicsEar = true;
		MechanicsEarCooldownSeconds = 120;
		EnableCrashControl = true;
		CrashControlDamageReductionPercent = 10;
		EnableFuelSaver = true;
		EnableBatteryCare = true;
		EnableVehicleSpeedBoost = false;
		ConvoyDriverSpeedBoostPercent = 10;
		EnableExperimentalVehicleGripAssist = false;
		MasterDriverGripBoostPercent = 0;
		MasterDriverSpeedBoostPercent = 0;
	}
}
