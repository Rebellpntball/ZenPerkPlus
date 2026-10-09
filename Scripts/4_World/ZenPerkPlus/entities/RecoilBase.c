modded class RecoilBase
{
	override void Update(SDayZPlayerAimingModel pModel, out float axis_mouse_x, out float axis_mouse_y, out float axis_hands_x, out float axis_hands_y, float pDt)
	{
		super.Update(pModel, axis_mouse_x, axis_mouse_y, axis_hands_x, axis_hands_y, pDt);
		Weapon_Base weapon = GetWeapon();
		if (!weapon)
			return;
		PlayerBase player = PlayerBase.Cast(weapon.GetHierarchyRootPlayer());
		if (!player)
			return;
		float mult = ZenPerkPlusHelpers.GetRecoilMultiplier(player);
		if (mult >= 0.999)
			return;
		axis_mouse_x = axis_mouse_x * mult;
		axis_mouse_y = axis_mouse_y * mult;
		axis_hands_x = axis_hands_x * mult;
		axis_hands_y = axis_hands_y * mult;
	}
}
