// Keep stock U / highscores from NRE on injected firearms/combat_ops/driver keys.
// Latest ZenSkillsGUI.Init() LoadImageFile's every db.Skills widget with no null check.

modded class ZenSkillsGUI
{
	override Widget Init()
	{
		map<string, ref ZenSkill> yanked = new map<string, ref ZenSkill>;
		ZenPerkPlusGUICompat.YankAddonSkills(yanked);
		Widget w = super.Init();
		ZenPerkPlusGUICompat.RestoreAddonSkills(yanked);
		return w;
	}

	override void UpdateSkillPerkLabels()
	{
		ZenSkillsPlayerDB db = GetZenSkillsPlugin().GetSkillsDB();
		if (!db)
			return;

		foreach (string key, ZenSkill skill : db.Skills)
		{
			if (ZenPerkPlusGUICompat.IsAddonSkill(key))
				continue;

			TextWidget perkLabel = m_SkillPerkLabels.Get(key);
			ProgressBarWidget perkBar = m_SkillPerkBars.Get(key);
			if (!skill)
				continue;

			int perkCount = skill.CountPerksUnused();
			if (perkLabel)
			{
				if (perkCount == 0)
					perkLabel.SetText("");
				else
					perkLabel.SetText("+" + perkCount);
			}

			if (perkBar)
				perkBar.SetCurrent(skill.ProgressToNextPerk());
		}
	}
}

modded class ZenSkillsHighscores
{
	override Widget Init()
	{
		map<string, ref ZenSkill> yanked = new map<string, ref ZenSkill>;
		ZenPerkPlusGUICompat.YankAddonSkills(yanked);
		Widget w = super.Init();
		ZenPerkPlusGUICompat.RestoreAddonSkills(yanked);
		return w;
	}
}

modded class ZenSkillsHUD
{
	override void ZenShowPerk(ZenSkillsNotification n)
	{
		if (!n)
			return;

		PlaySoundGUI();
		if (m_NewPerkFrame)
			m_NewPerkFrame.Show(true);

		string icon = "ZenSkills/data/gui/images/skill_" + n.m_SkillKey + ".edds";
		if (ZenPerkPlusGUICompat.IsAddonSkill(n.m_SkillKey))
			icon = ZenPerkPlusGUICompat.GetSkillNotifyIconPath(n.m_SkillKey);

		if (m_NewPerkIcon)
			m_NewPerkIcon.LoadImageFile(0, icon);

		if (m_NewPerkLabel)
			m_NewPerkLabel.SetText(n.m_Text);
		if (m_NewPerkPanel)
			m_NewPerkPanel.Show(true);
		if (m_NewPerkIcon)
			m_NewPerkIcon.Show(true);
		if (m_NewPerkLabel)
			m_NewPerkLabel.Show(true);
		if (m_NewPerkHint)
			m_NewPerkHint.Show(true);
		if (m_NewPerkPanel)
			m_NewPerkPanel.SetAlpha(PERK_PANEL_INIT_ALPHA);
		if (m_NewPerkIcon)
			m_NewPerkIcon.SetAlpha(1);
		if (m_NewPerkLabel)
			m_NewPerkLabel.SetAlpha(1);
		if (m_NewPerkHint)
			m_NewPerkHint.SetAlpha(1);

		g_Game.GetCallQueue(CALL_CATEGORY_GUI).Remove(StartFadeDelayedPerk);
		g_Game.GetCallQueue(CALL_CATEGORY_GUI).Remove(ZenOnCurrentFinishedNotify);
		g_Game.GetCallQueue(CALL_CATEGORY_GUI).Remove(ZenOnSlotEndNotify);
		g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(ZenOnSlotEndNotify, ZEN_NOTIFY_DISPLAY_MS, false);
	}
}
