// Client: open ZenPerkPlus tree (default key I — rebind in controls under ZEN)
modded class MissionGameplay
{
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		if (!g_Game)
			return;

		ZenPerkPlus_UpdateInput();
		ZenPerkPlus_UpdateRadioGlow();
	}

	void ZenPerkPlus_UpdateRadioGlow()
	{
		if (!g_Game.IsClient())
			return;
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (!player)
			return;
		float dir;
		float intensity;
		if (!player.ZenPerkPlus_ConsumeRadioGlow(dir, intensity))
			return;
		if (!HitDirectionEffectBase.ZenPerkPlus_IndicatorsOn())
			return;
		Hud hud = GetHud();
		if (!hud)
			return;
		HitDirectionEffectBase.ZenPerkPlus_QueueAmber();
		hud.SpawnHitDirEffect(player, dir, intensity);
	}

	void ZenPerkPlus_UpdateInput()
	{
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (!player || !player.IsAlive() || player.IsUnconscious())
			return;

		if (!ZenPerkPlus_CheckInput(ZenPerkPlusGUIConstants.INPUT_OPEN))
			return;

		if (!g_Game.GetUIManager())
			return;

		UIScriptedMenu current = g_Game.GetUIManager().GetMenu();
		if (current && current.GetID() == ZenPerkPlusGUIConstants.MENU_ID)
		{
			current.Close();
			return;
		}

		if (current != NULL)
			return;

		if (!ZenSkillsPlayerDB.RECEIVED_DATA)
			return;

		g_Game.GetUIManager().EnterScriptedMenu(ZenPerkPlusGUIConstants.MENU_ID, NULL);
	}

	bool ZenPerkPlus_CheckInput(string inputName)
	{
		if (!GetUApi())
			return false;

		UAInput uai = GetUApi().GetInputByName(inputName);
		if (uai && uai.LocalPress())
			return true;

		return false;
	}
}
