using System;
using UnityEngine;

public class Bullet : bs
{
	public bool freeze;

	public float speedUp;

	public int maxSpeed = 150;

	public float bulletSpeed = 1500f;

	public bool targetable;

	public GameObject explosion;

	public Weapon wep;

	public float randomMax;

	private Vector3 vel;

	public float extraTime;

	public float velm;

	private int frame;

	public bool rocket;

	public float explosionForce = 10000f;

	public float explosionRadius = 25f;

	public bool remote = true;

	public GameObject hole;

	public void Start()
	{
		float d = UnityEngine.Random.Range(0f, randomMax);
		foreach (Transform item in base.transform)
		{
			item.position += base.transform.forward * d;
		}
	}

	public void Update()
	{
		float num = Mathf.Min(extraTime, Time.deltaTime * 3f);
		extraTime -= num;
		Update2(Time.deltaTime + num);
	}

	public void Update2(float deltaTime)
	{
		//IL_025c: Unknown result type (might be due to invalid IL or missing references)
		//IL_0266: Expected O, but got Unknown
		if (deltaTime == 0f)
		{
			return;
		}
		frame++;
		ParticleSystem smoke = bs._Game.smoke2;
		smoke.transform.position = base.pos;
		if (frame < 5)
		{
			smoke.Emit(1);
		}
		vel = base.transform.forward * bulletSpeed * deltaTime;
		if (velm > 1000f)
		{
			UnityEngine.Object.Destroy(base.gameObject);
		}
		velm += vel.magnitude;
		if (speedUp != 0f)
		{
			bulletSpeed += speedUp * deltaTime;
			bulletSpeed = Mathf.Min(bulletSpeed, maxSpeed);
		}
		RaycastHit[] array = Physics.RaycastAll(base.transform.position, base.transform.forward, vel.magnitude, Layer.allmask);
		for (int i = 0; i < array.Length; i++)
		{
			RaycastHit raycastHit = array[i];
			bool flag = wep.pl == bs._Player;
			Player component = raycastHit.transform.root.GetComponent<Player>();
			if (component != null && bs.online && raycastHit.collider.tag == "HitBox")
			{
				bool flag2 = (!remote) ? flag : (!flag && component.IsMine);
				bool flag3 = component != wep.pl;
				if (flag2 && flag3 && !component.dead)
				{
					if (wep.pl != null && (wep.pl.team != component.team || bs._Loader.dmNoTeam))
					{
						component.CallRPC((Action<float, int>)component.SetLife, (float)component.life - wep.damage, wep.pl.playerId);
						if (freeze)
						{
							component.CallRPC((Action)(object)new Action(component.Freeze));
						}
					}
					bs._Game.Emit(raycastHit.point, raycastHit.normal, bs._Game.sparks);
				}
			}
			if (component != wep.pl || component == null)
			{
				if (explosion != null)
				{
					if (rocket && (flag || bs._Player.team != wep.pl.team))
					{
						bs._Player.rigidbody.AddExplosionForce(explosionForce, raycastHit.point, explosionRadius);
						float num = (explosionRadius - (raycastHit.point - bs._Player.pos).magnitude) / explosionRadius * wep.damage;
						if (num > 0f)
						{
							bs._Player.CallRPC((Action<float, int>)bs._Player.SetLife, (float)bs._Player.life - num, wep.pl.playerId);
						}
						UnityEngine.Object.Destroy(base.gameObject);
					}
					UnityEngine.Object.Destroy(UnityEngine.Object.Instantiate((UnityEngine.Object)explosion, raycastHit.point + raycastHit.normal, Quaternion.identity), 2f);
				}
				if (wep.bulletHitSound.Length > 0 && checkVis(base.pos) && Vector3.Distance(bs._Player.camera.transform.position, base.pos) < 100f)
				{
					AudioClip audioClip = wep.bulletHitSound[UnityEngine.Random.Range(0, wep.bulletHitSound.Length)];
					PlayAtPosition(raycastHit.point, audioClip, (!component || !component.IsMine) ? 200 : 0);
				}
			}
			if (component == null)
			{
				if (!bs.lowQuality)
				{
					bs._Game.Emit(raycastHit.point, raycastHit.normal, bs._Game.bulletHit);
					Hole((!(hole != null)) ? bs.res.hole : hole, raycastHit.point, raycastHit.normal);
				}
				UnityEngine.Object.Destroy(base.gameObject);
			}
		}
		if (wep.pl.IsMine && targetable)
		{
			bs._Game.cursorTexture.enabled = false;
			bs._Game.cursorTexture2.enabled = true;
			Ray ray = bs._Player.camera.ViewportPointToRay(new Vector3(0.5f, 0.5f));
			Vector3 origin = ray.origin;
			Vector3 a = CurvySpline2.ProjectPointLine(base.pos, origin, origin + ray.direction * 1000f);
			base.transform.forward = Vector3.RotateTowards(base.transform.forward, (a - origin).normalized, 1f * Time.deltaTime, 0f);
		}
		if (wep.slide && Physics.Raycast(base.pos, -base.transform.up, out RaycastHit _, 2f, Layer.levelMask))
		{
			base.pos += Vector3.up * Time.deltaTime * 5f;
		}
		base.transform.position += vel;
	}

	public static void Hole(GameObject original, Vector3 position, Vector3 vector3)
	{
		GameObject gameObject = (GameObject)UnityEngine.Object.Instantiate((UnityEngine.Object)original, position, Quaternion.LookRotation(vector3) * Quaternion.Euler(0f, 0f, UnityEngine.Random.Range(0, 360)));
		gameObject.hideFlags = HideFlags.HideInHierarchy;
		UnityEngine.Object.Destroy(gameObject, (!bs.highQuality) ? 20 : 100);
	}

	private static void PlayAtPosition(Vector3 position, AudioClip audioClip, int priority = 200)
	{
		GameObject gameObject = new GameObject();
		gameObject.hideFlags = HideFlags.HideInHierarchy;
		AudioSource audioSource = gameObject.AddComponent<AudioSource>();
		audioSource.transform.position = position;
		audioSource.clip = audioClip;
		audioSource.priority = priority;
		audioSource.Play();
		UnityEngine.Object.Destroy(gameObject, audioClip.length * Time.timeScale);
	}
}
