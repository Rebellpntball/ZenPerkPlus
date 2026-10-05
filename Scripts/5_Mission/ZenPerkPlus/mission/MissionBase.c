modded class MissionBase
{
	override UIScriptedMenu CreateScriptedMenu(int id)
	{
		UIScriptedMenu menu = super.CreateScriptedMenu(id);

		if (!menu)
		{
			if (id == ZenPerkPlusGUIConstants.MENU_ID)
			{
				menu = new ZenPerkPlusGUI();
				menu.SetID(id);
			}
		}

		return menu;
	}
}
