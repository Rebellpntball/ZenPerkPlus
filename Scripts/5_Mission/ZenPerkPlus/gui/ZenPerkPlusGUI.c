class ZenPerkPlusGUI extends ZenSkillsGUIBase
{
	static const int COLOR_LOCKED = ARGB(255, 128, 0, 0);
	static const int COLOR_OWNED = ARGB(255, 0, 128, 32);
	static const int COLOR_SELECTED = ARGB(255, 200, 170, 60);
	static const int COLOR_TAB = ARGB(255, 64, 64, 64);
	static const int COLOR_TAB_ON = ARGB(255, 100, 90, 40);

	static string m_SelectedSkill = ZenPerkPlusSkills.FIREARMS;
	static string m_SelectedPerkKey = "1_1";

	protected ref TextWidget m_TitleWidget;
	protected ref TextWidget m_PerkCountLabel;
	protected ref TextWidget m_SkillPointCountLabel;
	protected ref TextWidget m_SkillRightDescWidget;
	protected ref TextWidget m_SkillRightTitleWidget;
	protected ref ButtonWidget m_UnlockButton;
	protected ref ButtonWidget m_ResetButton;
	protected ref ButtonWidget m_CloseButton;
	protected ref ButtonWidget m_GunnerButton;
	protected ref ButtonWidget m_OperatorButton;
	protected ref ButtonWidget m_WheelmanButton;
	protected ref Widget m_ConfirmPanel;
	protected ref TextWidget m_ConfirmLabel;
	protected ref ButtonWidget m_ConfirmButton;
	protected ref ImageWidget m_GunnerIcon;
	protected ref ImageWidget m_OperatorIcon;
	protected ref ImageWidget m_WheelmanIcon;

	protected ref map<string, ref ButtonWidget> m_PerkTreeButtons;
	protected ref map<string, ref ImageWidget> m_PerkTreeIcons;
	protected ref map<string, ref TextWidget> m_PerkTreeLevels;

	protected int m_LastConfirmDialog;
	protected int m_AvailableSkillPoints;

	override Widget Init()
	{
		layoutRoot = g_Game.GetWorkspace().CreateWidgets(ZenPerkPlusGUIConstants.LAYOUT_FILE);

		if (!ZenSkillsPlayerDB.RECEIVED_DATA)
		{
			g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(Close, 10);
			return layoutRoot;
		}

		m_TitleWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("TitleWidget"));
		m_PerkCountLabel = TextWidget.Cast(layoutRoot.FindAnyWidget("PerkCountLabel"));
		m_SkillPointCountLabel = TextWidget.Cast(layoutRoot.FindAnyWidget("SkillPointCountLabel"));
		m_SkillRightDescWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("SkillDescWidget"));
		m_SkillRightTitleWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("PerkDescTitle"));
		m_UnlockButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("SubmitButton"));
		m_ResetButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ResetButton"));
		m_CloseButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CloseButton"));
		m_GunnerButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("GunnerButton"));
		m_OperatorButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("OperatorButton"));
		m_WheelmanButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("WheelmanButton"));
		m_GunnerIcon = ImageWidget.Cast(layoutRoot.FindAnyWidget("GunnerButtonImage"));
		m_OperatorIcon = ImageWidget.Cast(layoutRoot.FindAnyWidget("OperatorButtonImage"));
		m_WheelmanIcon = ImageWidget.Cast(layoutRoot.FindAnyWidget("WheelmanButtonImage"));
		m_ConfirmPanel = layoutRoot.FindAnyWidget("ConfirmPanel");
		m_ConfirmLabel = TextWidget.Cast(layoutRoot.FindAnyWidget("ConfirmLabel"));
		m_ConfirmButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ConfirmButton"));

		if (m_ConfirmPanel) m_ConfirmPanel.Show(false);
		if (m_UnlockButton) m_UnlockButton.Show(false);

		if (m_GunnerIcon) m_GunnerIcon.LoadImageFile(0, ZenPerkPlusGUICompat.GetSkillTabIconPath(ZenPerkPlusSkills.FIREARMS));
		if (m_OperatorIcon) m_OperatorIcon.LoadImageFile(0, ZenPerkPlusGUICompat.GetSkillTabIconPath(ZenPerkPlusSkills.COMBAT_OPS));
		if (m_WheelmanIcon) m_WheelmanIcon.LoadImageFile(0, ZenPerkPlusGUICompat.GetSkillTabIconPath(ZenPerkPlusSkills.DRIVER));

		m_PerkTreeButtons = new map<string, ref ButtonWidget>;
		m_PerkTreeIcons = new map<string, ref ImageWidget>;
		m_PerkTreeLevels = new map<string, ref TextWidget>;

		array<string> slots = GetSlotList();
		foreach (string slot : slots)
		{
			m_PerkTreeButtons.Set(slot, ButtonWidget.Cast(layoutRoot.FindAnyWidget("PerkBtn" + slot)));
			m_PerkTreeIcons.Set(slot, ImageWidget.Cast(layoutRoot.FindAnyWidget("PerkIcon" + slot)));
			m_PerkTreeLevels.Set(slot, TextWidget.Cast(layoutRoot.FindAnyWidget("PerkLabel" + slot)));
		}

		SelectSkill(m_SelectedSkill, false);
		return layoutRoot;
	}

	array<string> GetSlotList()
	{
		array<string> slots = new array<string>;
		slots.Insert("1_1"); slots.Insert("1_2"); slots.Insert("1_3");
		slots.Insert("2_1"); slots.Insert("2_2");
		slots.Insert("3_1"); slots.Insert("3_2"); slots.Insert("3_3");
		slots.Insert("4_1"); slots.Insert("4_2");
		return slots;
	}

	override void OnShow()
	{
		super.OnShow();
		GetGame().GetInput().ChangeGameFocus(1);
		GetGame().GetUIManager().ShowUICursor(true);
		ZenMissionFunctions.FreezePlayerControls();
		SelectSkill(m_SelectedSkill, false);
	}

	override void OnHide()
	{
		super.OnHide();
		ZenMissionFunctions.UnfreezePlayerControls();
		GetGame().GetInput().ResetGameFocus();
		GetGame().GetUIManager().ShowUICursor(false);
	}

	override void ForceUpdateFromServer(int playSound)
	{
		RefreshSelection();
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (!w || button != MouseState.LEFT)
			return super.OnClick(w, x, y, button);

		string name = w.GetName();
		name.ToLower();

		if (m_ConfirmPanel && m_ConfirmPanel.IsVisible())
		{
			if (name == "cancelbutton") { m_ConfirmPanel.Show(false); RefreshSelection(); return true; }
			if (name == "confirmbutton")
			{
				if (m_LastConfirmDialog == 1) SendUnlockRequest();
				else if (m_LastConfirmDialog == 2) SendResetRequest();
				return true;
			}
			return true;
		}

		if (w == m_CloseButton || name == "closebutton") { Close(); return true; }
		if (w == m_GunnerButton || name == "gunnerbutton") { SelectSkill(ZenPerkPlusSkills.FIREARMS); return true; }
		if (w == m_OperatorButton || name == "operatorbutton") { SelectSkill(ZenPerkPlusSkills.COMBAT_OPS); return true; }
		if (w == m_WheelmanButton || name == "wheelmanbutton") { SelectSkill(ZenPerkPlusSkills.DRIVER); return true; }
		if (w == m_UnlockButton || name == "submitbutton") { RequestUnlock(); return true; }
		if (w == m_ResetButton || name == "resetbutton") { RequestReset(); return true; }

		if (name.Contains("perkbtn"))
		{
			string key = name;
			key.Replace("perkbtn", "");
			SelectPerk(key);
			return true;
		}
		return super.OnClick(w, x, y, button);
	}

	void SelectSkill(string skillKey, bool playSound = true)
	{
		m_SelectedSkill = skillKey;
		if (m_GunnerButton) m_GunnerButton.SetColor(COLOR_TAB);
		if (m_OperatorButton) m_OperatorButton.SetColor(COLOR_TAB);
		if (m_WheelmanButton) m_WheelmanButton.SetColor(COLOR_TAB);
		if (skillKey == ZenPerkPlusSkills.FIREARMS && m_GunnerButton) m_GunnerButton.SetColor(COLOR_TAB_ON);
		else if (skillKey == ZenPerkPlusSkills.COMBAT_OPS && m_OperatorButton) m_OperatorButton.SetColor(COLOR_TAB_ON);
		else if (skillKey == ZenPerkPlusSkills.DRIVER && m_WheelmanButton) m_WheelmanButton.SetColor(COLOR_TAB_ON);
		if (m_TitleWidget)
		{
			string title = "ZenPerkPlus";
			if (skillKey == ZenPerkPlusSkills.FIREARMS) title = "Gunner";
			else if (skillKey == ZenPerkPlusSkills.COMBAT_OPS) title = "Operator";
			else if (skillKey == ZenPerkPlusSkills.DRIVER) title = "Wheelman";
			m_TitleWidget.SetText(title);
		}
		UpdateSkillPoints();
		RefreshPerkNodes();
		SelectPerk(m_SelectedPerkKey);
	}

	void SelectPerk(string perkKey)
	{
		if (perkKey == "") perkKey = "1_1";
		m_SelectedPerkKey = perkKey;
		ZenSkillsPlayerDB db = GetZenSkillsPlugin().GetSkillsDB();
		if (!db || !db.Skills) return;
		ZenSkill skill = db.Skills.Get(m_SelectedSkill);
		if (!skill || !skill.Perks) return;
		ZenPerk perk = skill.Perks.Get(perkKey);
		ZenPerkDef def;
		if (perk) def = perk.GetDef();
		if (m_SkillRightTitleWidget)
		{
			if (def) m_SkillRightTitleWidget.SetText(def.DisplayName);
			else m_SkillRightTitleWidget.SetText("Perk " + perkKey);
		}
		if (m_SkillRightDescWidget)
		{
			if (def)
			{
				string txt = def.Description;
				if (perk) txt = txt + "\n\nLevel: " + perk.Level.ToString();
				m_SkillRightDescWidget.SetText(txt);
			}
			else m_SkillRightDescWidget.SetText("No definition for this slot.");
		}
		bool canUnlock = false;
		if (db && perkKey != "") canUnlock = db.CanUnlockPerk(m_SelectedSkill, perkKey);
		if (m_UnlockButton) m_UnlockButton.Show(canUnlock);
		array<string> slots = GetSlotList();
		foreach (string slot : slots)
		{
			ButtonWidget btn = m_PerkTreeButtons.Get(slot);
			if (!btn) continue;
			if (slot == perkKey) btn.SetColor(COLOR_SELECTED);
			else ColorNodeByState(slot, btn, skill);
		}
	}

	void ColorNodeByState(string slot, ButtonWidget btn, ZenSkill skill)
	{
		ZenPerk perk = skill.Perks.Get(slot);
		if (perk && perk.Level > 0) btn.SetColor(COLOR_OWNED);
		else btn.SetColor(COLOR_LOCKED);
	}

	void RefreshPerkNodes()
	{
		ZenSkillsPlayerDB db = GetZenSkillsPlugin().GetSkillsDB();
		if (!db || !db.Skills) return;
		ZenSkill skill = db.Skills.Get(m_SelectedSkill);
		if (!skill || !skill.Perks) return;
		array<string> slots = GetSlotList();
		foreach (string slot : slots)
		{
			ZenPerk perk = skill.Perks.Get(slot);
			TextWidget lvl = m_PerkTreeLevels.Get(slot);
			ButtonWidget btn = m_PerkTreeButtons.Get(slot);
			ImageWidget iw = m_PerkTreeIcons.Get(slot);
			int level = 0;
			int maxLvl = 3;
			if (perk)
			{
				level = perk.Level;
				ZenPerkDef def = perk.GetDef();
				if (def && def.Rewards) maxLvl = def.Rewards.Count();
			}
			if (lvl) lvl.SetText(level.ToString() + "/" + maxLvl.ToString());
			if (btn) ColorNodeByState(slot, btn, skill);
			ZenPerkPlusGUICompat.LoadPerkNodeIcon(iw, m_SelectedSkill, slot, level);
		}
	}

	void UpdateSkillPoints()
	{
		ZenSkillsPlayerDB db = GetZenSkillsPlugin().GetSkillsDB();
		if (!db || !db.Skills) return;
		ZenSkill skill = db.Skills.Get(m_SelectedSkill);
		if (!skill || !skill.GetDef()) return;
		int expPer = skill.GetDef().EXP_Per_Perk;
		if (expPer <= 0) expPer = 1000;
		m_AvailableSkillPoints = skill.EXP / expPer;
		int perkCount = skill.CountPerksUnlocked();
		int maxPerks = skill.GetDef().MaxAllowedPerks;
		if (m_SkillPointCountLabel)
		{
			string pts = m_AvailableSkillPoints.ToString();
			if (m_AvailableSkillPoints > 0) pts = "+" + pts;
			m_SkillPointCountLabel.SetText("Skill Points: " + pts);
		}
		if (m_PerkCountLabel) m_PerkCountLabel.SetText("Perks: " + perkCount.ToString() + "/" + maxPerks.ToString());
		if (m_ResetButton) m_ResetButton.Show(GetZenSkillsConfig().SharedConfig.AllowResetPerks && perkCount > 0);
	}

	void RefreshSelection()
	{
		UpdateSkillPoints();
		RefreshPerkNodes();
		SelectPerk(m_SelectedPerkKey);
	}

	void RequestUnlock()
	{
		ZenSkillsPlayerDB db = GetZenSkillsPlugin().GetSkillsDB();
		if (!db) return;
		ZenSkill skill = db.Skills.Get(m_SelectedSkill);
		if (!skill || !skill.Perks) return;
		ZenPerk perk = skill.Perks.Get(m_SelectedPerkKey);
		if (!perk || !perk.GetDef()) return;
		if (m_ConfirmLabel) m_ConfirmLabel.SetText("Unlock " + perk.GetDef().DisplayName + "?");
		if (m_ConfirmPanel) m_ConfirmPanel.Show(true);
		if (m_UnlockButton) m_UnlockButton.Show(false);
		if (m_ConfirmButton) m_ConfirmButton.Show(true);
		m_LastConfirmDialog = 1;
	}

	void RequestReset()
	{
		if (!GetZenSkillsConfig().SharedConfig.AllowResetPerks) return;
		ZenSkillsPlayerDB db = GetZenSkillsPlugin().GetSkillsDB();
		if (!db) return;
		ZenSkill skill = db.Skills.Get(m_SelectedSkill);
		if (!skill) return;
		int perkRefund; int totalPerksLeft;
		GetZenSkillsPlugin().GetResultingRefundPerksEXP(skill, perkRefund, totalPerksLeft);
		if (m_ConfirmLabel) m_ConfirmLabel.SetText("Reset this role's perks? Partial EXP refund uses ZenSkills rules.");
		if (m_ConfirmPanel) m_ConfirmPanel.Show(true);
		if (m_UnlockButton) m_UnlockButton.Show(false);
		if (m_ConfirmButton) m_ConfirmButton.Show(true);
		m_LastConfirmDialog = 2;
	}

	protected void SendUnlockRequest()
	{
		ZenSkillsPlayerDB db = GetZenSkillsPlugin().GetSkillsDB();
		if (!db) { Close(); return; }
		if (!db.CanUnlockPerk(m_SelectedSkill, m_SelectedPerkKey))
		{
			if (m_ConfirmLabel) m_ConfirmLabel.SetText("Not enough skill points.");
			if (m_ConfirmButton) m_ConfirmButton.Show(false);
			return;
		}
		if (m_ConfirmPanel) m_ConfirmPanel.Show(false);
		GetRPCManager().SendRPC(ZenSkillConstants.RPC, ZenSkillConstants.RPC_ServerReceive_PerkUnlock, new Param2<string, string>(m_SelectedSkill, m_SelectedPerkKey), true, null);
		g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(RefreshSelection, 300);
	}

	protected void SendResetRequest()
	{
		ZenSkillsPlayerDB db = GetZenSkillsPlugin().GetSkillsDB();
		if (!db) { Close(); return; }
		if (m_ConfirmPanel) m_ConfirmPanel.Show(false);
		GetRPCManager().SendRPC(ZenSkillConstants.RPC, ZenSkillConstants.RPC_ServerReceive_PerkReset, new Param1<string>(m_SelectedSkill), true, null);
		g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(RefreshSelection, 300);
	}
}
