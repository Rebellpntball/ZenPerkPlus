/*
	ZenPerkPlus - optional ZenSkills add-on.
	Separate mod; does not edit ZenSkills source.
	Progression control (unlock/reset/costs) stays in ZenSkills.
*/

class CfgPatches
{
	class ZenPerkPlus
	{
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"ZenModCore",
			"ZenSkills"
		};
	};
};

class CfgMods
{
	class ZenPerkPlus
	{
		author = "ZenPerkPlus";
		type = "mod";
		inputs = "ZenPerkPlus/data/inputs.xml";
		dependencies[] = { "Game", "World", "Mission" };
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = { "ZenPerkPlus/Scripts/3_Game" };
			}
			class worldScriptModule
			{
				value = "";
				files[] = { "ZenPerkPlus/Scripts/4_World" };
			}
			class missionScriptModule
			{
				value = "";
				files[] = { "ZenPerkPlus/Scripts/5_Mission" };
			}
		};
	};
};
