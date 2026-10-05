class ZenPerkPlusGUI extends UIScriptedMenu
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
		m_ConfirmPanel = layoutRoot.FindAnyWidget("ConfirmPanel");
		m_ConfirmLabel = TextWidget.Cast(layoutRoot.FindAnyWidget("ConfirmLabel"));
		m_ConfirmButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ConfirmButton"));

		if (m_ConfirmPanel) m_ConfirmPanel.Show(false);
		if (m_UnlockButton) m_UnlockButton.Show(false);

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
		slots. steInsert("3_1"); slots.Insert("3_2"); slots.Insert("3_3");
		slots.Insert("4_1"); slots.Insert("4_2");
		return slots;
	}
}
