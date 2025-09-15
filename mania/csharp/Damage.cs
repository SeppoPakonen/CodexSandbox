using UnityEngine;

public class Damage : DamageBase
{
	public bool Explosive;

	public float ExplosionRadius = 20f;

	public float ExplosionForce = 1000f;

	public bool HitedActive = true;

	public float TimeActive;

	private float timetemp;

	private void Start()
	{
		if ((bool)Owner && (bool)Owner.get_collider())
		{
			Physics.IgnoreCollision(base.get_collider(), Owner.get_collider());
			timetemp = Time.time;
		}
	}

	private void Update()
	{
		if (!HitedActive && Time.time >= timetemp + TimeActive)
		{
			Active();
		}
	}

	public void Active()
	{
		if ((bool)Effect)
		{
			GameObject obj = (GameObject)Object.Instantiate((Object)Effect, base.transform.position, base.transform.rotation);
			Object.Destroy(obj, 3f);
		}
		if (Explosive)
		{
			ExplosionDamage();
		}
		Object.Destroy(base.gameObject);
	}

	private void ExplosionDamage()
	{
		Collider[] array = Physics.OverlapSphere(base.transform.position, ExplosionRadius);
		foreach (Collider collider in array)
		{
			if ((bool)collider)
			{
				if ((bool)collider.gameObject.GetComponent<DamageManager>() && (bool)collider.gameObject.GetComponent<DamageManager>())
				{
					collider.gameObject.GetComponent<DamageManager>().ApplyDamage(Damage);
				}
				if ((bool)((Component)collider).get_rigidbody())
				{
					((Component)collider).get_rigidbody().AddExplosionForce(ExplosionForce, base.transform.position, ExplosionRadius, 3f);
				}
			}
		}
	}

	private void NormalDamage(Collision collision)
	{
		if ((bool)collision.gameObject.GetComponent<DamageManager>())
		{
			collision.gameObject.GetComponent<DamageManager>().ApplyDamage(Damage);
		}
	}

	private void OnCollisionEnter(Collision collision)
	{
		if (HitedActive && collision.gameObject.tag != "Particle" && collision.gameObject.tag != base.gameObject.tag)
		{
			if (!Explosive)
			{
				NormalDamage(collision);
			}
			Active();
		}
	}
}
