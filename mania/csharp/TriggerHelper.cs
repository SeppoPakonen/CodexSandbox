using UnityEngine;

public class TriggerHelper : bs
{
	public WeaponFlameTower pl;

	private void OnParticleCollision(GameObject other)
	{
		pl.particleSystem = base.get_particleSystem();
		pl.OnParticleCollision(other);
	}
}
