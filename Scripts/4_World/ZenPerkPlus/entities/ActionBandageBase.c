modded class ActionBandageSelfCB
{
	override void CreateActionComponent()
	{
		float t = UATimeSpent.BANDAGE;
		PlayerBase player = PlayerBase.Cast(m_ActionData.m_Player);
		if (player)
			t = t * ZenPerkPlusHelpers.GetMedicActionTimeMultiplier(player);
		m_ActionData.m_ActionComponent = new CAContinuousTime(t);
	}
}

modded class ActionBandageTargetCB
{
	override void CreateActionComponent()
	{
		float t = UATimeSpent.BANDAGE;
		PlayerBase player = PlayerBase.Cast(m_ActionData.m_Player);
		if (player)
			t = t * ZenPerkPlusHelpers.GetMedicActionTimeMultiplier(player);
		m_ActionData.m_ActionComponent = new CAContinuousTime(t);
	}
}

modded class ActionSplintSelfCB
{
	override void CreateActionComponent()
	{
		float t = UATimeSpent.APPLY_SPLINT;
		PlayerBase player = PlayerBase.Cast(m_ActionData.m_Player);
		if (player)
			t = t * ZenPerkPlusHelpers.GetMedicActionTimeMultiplier(player);
		m_ActionData.m_ActionComponent = new CAContinuousTime(t);
	}
}

modded class ActionSplintTargetCB
{
	override void CreateActionComponent()
	{
		float t = UATimeSpent.APPLY_SPLINT;
		PlayerBase player = PlayerBase.Cast(m_ActionData.m_Player);
		if (player)
			t = t * ZenPerkPlusHelpers.GetMedicActionTimeMultiplier(player);
		m_ActionData.m_ActionComponent = new CAContinuousTime(t);
	}
}
