using System;
using UnityEngine;

public class WeaponFlameTower : WeaponBase
{
	public ParticleSystem[] emitors;

	public Light fireLight;

	public AudioSource fireSound;

	public float soundStart = 1.3f;

	public float soundEnd = 2f;

	private CollisionEvent[] collisionEvents = (CollisionEvent[])(object)new CollisionEvent[16];

	public ParticleSystem particleSystem;

	public override void Update()
	{
		ParticleSystem[] array = emitors;
		foreach (ParticleSystem particleSystem in array)
		{
			particleSystem.enableEmission = shooting;
		}
		fireLight.enabled = shooting;
		if (pl.dead || pl.froozen)
		{
			shooting = false;
			return;
		}
		if (shooting)
		{
			fireLight.range = Mathf.Lerp(fireLight.range, UnityEngine.Random.Range(4, 7), Time.deltaTime * 50f);
			if (!fireSound.isPlaying)
			{
				fireSound.time = 0f;
				fireSound.Play();
			}
			if (fireSound.time > soundEnd)
			{
				fireSound.time = soundStart;
			}
		}
		else if (fireSound.isPlaying)
		{
			fireSound.Stop();
		}
		base.Update();
	}

	public override void SetShoot(bool b)
	{
		shooting = b;
		base.SetShoot(b);
	}

	public void OnParticleCollision(GameObject other)
	{
		//IL_004b: Unknown result type (might be due to invalid IL or missing references)
		//IL_0050: Unknown result type (might be due to invalid IL or missing references)
		//IL_0175: Unknown result type (might be due to invalid IL or missing references)
		//IL_017f: Expected O, but got Unknown
		int safeCollisionEventSize = particleSystem.get_safeCollisionEventSize();
		if (collisionEvents.Length < safeCollisionEventSize)
		{
			collisionEvents = (CollisionEvent[])(object)new CollisionEvent[safeCollisionEventSize];
		}
		int max = particleSystem.GetCollisionEvents(other, collisionEvents);
		CollisionEvent val = collisionEvents[UnityEngine.Random.Range(0, max)];
		if (UnityEngine.Random.value < 0.1f && other.tag == "Untagged")
		{
			UnityEngine.Object.Destroy(UnityEngine.Object.Instantiate((UnityEngine.Object)bs.res.fire, ((CollisionEvent)(ref val)).get_intersection(), Quaternion.identity), 5f);
			Bullet.Hole(bs.res.hole2, ((CollisionEvent)(ref val)).get_intersection(), ((CollisionEvent)(ref val)).get_normal());
		}
		else
		{
			if (!pl.IsMine || other.layer != Layer.hitBox)
			{
				return;
			}
			Player component = other.transform.root.GetComponent<Player>();
			if ((bool)component && component != bs._Player && !component.sameTeam)
			{
				component.life = (float)component.life - 5f;
				if (Time.time - component.lastHitTime > 0.1f)
				{
					component.CallRPC((Action<float, int>)component.SetLife, (float)component.life, pl.playerId);
					component.CallRPC((Action)(object)new Action(component.SetOnFire));
				}
			}
		}
	}
}
