modded class AnimalBase
{
	override void EEKilledZen(notnull Object killer)
	{
		super.EEKilledZen(killer);
		ZenPerkPlus_HandleKilledEntity(this, killer);
	}
}
