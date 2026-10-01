/*
	ZenPerkPlus - optional ZenSkills add-on.
	This is a separate mod source folder and does not edit or replace ZenSkills.
	Expansion AI detection is optional and guarded by EXPANSIONMODAI.
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
		author = "Zenarchist";
		type = "mod";
		dependencies[] = { "Game", "World", "Mission" };
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = { "ZenPerkPlus/Scripts/3_Game" };
			};
			class worldScriptModule
			{
				value = "";
				files[] = { "ZenPerkPlus/Scripts/4_World" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "ZenPerkPlus/Scripts/5_Mission" };
			};
		};
	};
};
