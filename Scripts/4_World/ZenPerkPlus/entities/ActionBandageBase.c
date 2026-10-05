modded class ActionBandageBase
{
	override float GetProgressTime(ActionData action_data)
	{
		float t = super.GetProgressTime(action_data);
		if (!action_data || !action_data.m_Player)
			return t;
		PlayerBase player = PlayerBase.Cast(action_data.m_Player);
		if (!player)
			return t;
		return t * ZenPerkPlusHelpers.GetMedicActionTimeMultiplier(player);
	}
}
