using UnityEngine;

public class MoverBullet : WeaponBase2
{
	public int Lifetime;

	public float Speed = 80f;

	public float SpeedMax = 80f;

	public float SpeedMult = 1f;

	private void Start()
	{
		Object.Destroy(base.gameObject, Lifetime);
	}

	private void FixedUpdate()
	{
		if ((bool)base.get_rigidbody())
		{
			if (!RigidbodyProjectile)
			{
				base.get_rigidbody().velocity = base.transform.forward * Speed;
			}
			else if (base.get_rigidbody().velocity.normalized != Vector3.zero)
			{
				base.transform.forward = base.get_rigidbody().velocity.normalized;
			}
			if (Speed < SpeedMax)
			{
				Speed += SpeedMult * Time.fixedDeltaTime;
			}
		}
	}
}
