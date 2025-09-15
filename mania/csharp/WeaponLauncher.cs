using UnityEngine;

[RequireComponent(typeof(AudioSource))]
public class WeaponLauncher : WeaponBase2
{
	public Transform[] MissileOuter;

	public GameObject Missile;

	public float FireRate = 0.1f;

	public float Spread = 1f;

	public float ForceShoot = 8000f;

	public int NumBullet = 1;

	public int Ammo = 10;

	public int AmmoMax = 10;

	public bool InfinityAmmo;

	public float ReloadTime = 1f;

	public bool ShowHUD = true;

	public Texture2D TargetLockOnTexture;

	public Texture2D TargetLockedTexture;

	public float DistanceLock = 200f;

	public float TimeToLock = 2f;

	public float AimDirection = 0.8f;

	public bool Seeker;

	public GameObject Shell;

	public float ShellLifeTime = 4f;

	public Transform[] ShellOuter;

	public int ShellOutForce = 300;

	public GameObject Muzzle;

	public float MuzzleLifeTime = 2f;

	public AudioClip[] SoundGun;

	public AudioClip SoundReloading;

	public AudioClip SoundReloaded;

	private float timetolockcount;

	private float nextFireTime;

	private GameObject target;

	private Vector3 torqueTemp;

	private float reloadTimeTemp;

	private AudioSource audio;

	[HideInInspector]
	public bool Reloading;

	[HideInInspector]
	public float ReloadingProcess;

	private int currentOuter;

	private void Start()
	{
		if (!Owner)
		{
			Owner = base.transform.root.gameObject;
		}
		if (!audio)
		{
			audio = GetComponent<AudioSource>();
			if (!audio)
			{
				base.gameObject.AddComponent<AudioSource>();
			}
		}
	}

	private void Update()
	{
		if ((bool)TorqueObject)
		{
			TorqueObject.transform.Rotate(torqueTemp * Time.deltaTime);
			torqueTemp = Vector3.Lerp(torqueTemp, Vector3.zero, Time.deltaTime);
		}
		if (Seeker)
		{
			for (int i = 0; i < TargetTag.Length; i++)
			{
				if (GameObject.FindGameObjectsWithTag(TargetTag[i]).Length <= 0)
				{
					continue;
				}
				GameObject[] array = GameObject.FindGameObjectsWithTag(TargetTag[i]);
				float num = 2.14748365E+09f;
				for (int j = 0; j < array.Length; j++)
				{
					if ((bool)array[j])
					{
						Vector3 normalized = (array[j].transform.position - base.transform.position).normalized;
						float num2 = Vector3.Dot(normalized, base.transform.forward);
						float num3 = Vector3.Distance(array[j].transform.position, base.transform.position);
						if (num2 >= AimDirection && DistanceLock > num3 && num > num3 && timetolockcount + TimeToLock < Time.time)
						{
							num = num3;
							target = array[j];
						}
					}
				}
			}
		}
		if ((bool)target)
		{
			float num4 = Vector3.Distance(base.transform.position, target.transform.position);
			Vector3 normalized2 = (target.transform.position - base.transform.position).normalized;
			float num5 = Vector3.Dot(normalized2, base.transform.forward);
			if (num4 > DistanceLock || num5 <= AimDirection)
			{
				Unlock();
			}
		}
		if (Reloading)
		{
			ReloadingProcess = 1f / ReloadTime * (reloadTimeTemp + ReloadTime - Time.time);
			if (Time.time >= reloadTimeTemp + ReloadTime)
			{
				Reloading = false;
				if ((bool)SoundReloaded && (bool)audio)
				{
					audio.PlayOneShot(SoundReloaded);
				}
				Ammo = AmmoMax;
			}
		}
		else if (Ammo <= 0)
		{
			Unlock();
			Reloading = true;
			reloadTimeTemp = Time.time;
			if ((bool)SoundReloading && (bool)audio)
			{
				audio.PlayOneShot(SoundReloading);
			}
		}
	}

	private void DrawTargetLockon(Transform aimtarget, bool locked)
	{
		if (!ShowHUD || !Camera.current)
		{
			return;
		}
		Vector3 normalized = (aimtarget.position - ((Component)Camera.current).get_camera().transform.position).normalized;
		float num = Vector3.Dot(normalized, ((Component)Camera.current).get_camera().transform.forward);
		if (!(num > 0.5f))
		{
			return;
		}
		Vector3 vector = ((Component)Camera.current).get_camera().WorldToScreenPoint(aimtarget.transform.position);
		float f = Vector3.Distance(base.transform.position, aimtarget.transform.position);
		if (locked)
		{
			if ((bool)TargetLockedTexture)
			{
				GUI.DrawTexture(new Rect(vector.x - (float)(TargetLockedTexture.width / 2), (float)Screen.height - vector.y - (float)(TargetLockedTexture.height / 2), TargetLockedTexture.width, TargetLockedTexture.height), TargetLockedTexture);
			}
			GUI.Label(new Rect(vector.x + 40f, (float)Screen.height - vector.y, 200f, 30f), aimtarget.name + " " + Mathf.Floor(f) + "m.");
		}
		else if ((bool)TargetLockOnTexture)
		{
			GUI.DrawTexture(new Rect(vector.x - (float)(TargetLockOnTexture.width / 2), (float)Screen.height - vector.y - (float)(TargetLockOnTexture.height / 2), TargetLockOnTexture.width, TargetLockOnTexture.height), TargetLockOnTexture);
		}
	}

	private void OnGUI()
	{
		if (!Seeker)
		{
			return;
		}
		if ((bool)target)
		{
			DrawTargetLockon(target.transform, locked: true);
		}
		for (int i = 0; i < TargetTag.Length; i++)
		{
			if (GameObject.FindGameObjectsWithTag(TargetTag[i]).Length <= 0)
			{
				continue;
			}
			GameObject[] array = GameObject.FindGameObjectsWithTag(TargetTag[i]);
			for (int j = 0; j < array.Length; j++)
			{
				if (!array[j])
				{
					continue;
				}
				Vector3 normalized = (array[j].transform.position - base.transform.position).normalized;
				float num = Vector3.Dot(normalized, base.transform.forward);
				if (num >= AimDirection)
				{
					float num2 = Vector3.Distance(array[j].transform.position, base.transform.position);
					if (DistanceLock > num2)
					{
						DrawTargetLockon(array[j].transform, locked: false);
					}
				}
			}
		}
	}

	private void Unlock()
	{
		timetolockcount = Time.time;
		target = null;
	}

	public void Shoot()
	{
		if (InfinityAmmo)
		{
			Ammo = 1;
		}
		if (Ammo <= 0 || !(Time.time > nextFireTime + FireRate))
		{
			return;
		}
		nextFireTime = Time.time;
		torqueTemp = TorqueSpeedAxis;
		Ammo--;
		Vector3 position = base.transform.position;
		Quaternion rotation = base.transform.rotation;
		if (MissileOuter.Length > 0)
		{
			rotation = MissileOuter[currentOuter].transform.rotation;
			position = MissileOuter[currentOuter].transform.position;
		}
		if (MissileOuter.Length > 0)
		{
			currentOuter++;
			if (currentOuter >= MissileOuter.Length)
			{
				currentOuter = 0;
			}
		}
		if ((bool)Muzzle)
		{
			GameObject gameObject = (GameObject)Object.Instantiate((Object)Muzzle, position, rotation);
			gameObject.transform.parent = base.transform;
			Object.Destroy(gameObject, MuzzleLifeTime);
			if (MissileOuter.Length > 0)
			{
				gameObject.transform.parent = MissileOuter[currentOuter].transform;
			}
		}
		for (int i = 0; i < NumBullet; i++)
		{
			if (!Missile)
			{
				continue;
			}
			Vector3 b = new Vector3(Random.Range(0f - Spread, Spread), Random.Range(0f - Spread, Spread), Random.Range(0f - Spread, Spread)) / 100f;
			Vector3 vector = base.transform.forward + b;
			GameObject gameObject2 = (GameObject)Object.Instantiate((Object)Missile, position, rotation);
			if ((bool)gameObject2.GetComponent<DamageBase>())
			{
				gameObject2.GetComponent<DamageBase>().Owner = Owner;
			}
			if ((bool)gameObject2.GetComponent<WeaponBase2>())
			{
				gameObject2.GetComponent<WeaponBase2>().Owner = Owner;
				gameObject2.GetComponent<WeaponBase2>().Target = target;
			}
			gameObject2.transform.forward = vector;
			if (RigidbodyProjectile && (bool)gameObject2.get_rigidbody())
			{
				if (Owner != null && (bool)Owner.get_rigidbody())
				{
					gameObject2.get_rigidbody().velocity = Owner.get_rigidbody().velocity;
				}
				gameObject2.get_rigidbody().AddForce(vector * ForceShoot);
			}
		}
		if ((bool)Shell)
		{
			Transform transform = base.transform;
			if (ShellOuter.Length > 0)
			{
				transform = ShellOuter[currentOuter];
			}
			GameObject gameObject3 = (GameObject)Object.Instantiate((Object)Shell, transform.position, Random.rotation);
			Object.Destroy(gameObject3.gameObject, ShellLifeTime);
			if ((bool)gameObject3.get_rigidbody())
			{
				gameObject3.get_rigidbody().AddForce(transform.forward * ShellOutForce);
			}
		}
		if (SoundGun.Length > 0 && (bool)audio)
		{
			audio.PlayOneShot(SoundGun[Random.Range(0, SoundGun.Length)]);
		}
		nextFireTime += FireRate;
	}
}
