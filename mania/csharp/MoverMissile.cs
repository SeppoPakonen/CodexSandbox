using UnityEngine;

public class MoverMissile : WeaponBase2
{
	public float Damping = 3f;

	public float Speed = 80f;

	public float SpeedMax = 80f;

	public float SpeedMult = 1f;

	public Vector3 Noise = new Vector3(20f, 20f, 20f);

	public float TargetLockDirection = 0.5f;

	public int DistanceLock = 70;

	public int DurationLock = 40;

	public bool Seeker;

	public float LifeTime = 5f;

	private bool locked;

	private int timetorock;

	private float timeCount;

	private void Start()
	{
		timeCount = Time.time;
		Object.Destroy(base.gameObject, LifeTime);
	}

	private void FixedUpdate()
	{
		Rigidbody rigidbody = base.get_rigidbody();
		Vector3 forward = base.transform.forward;
		float x = forward.x * Speed * Time.fixedDeltaTime;
		Vector3 forward2 = base.transform.forward;
		float y = forward2.y * Speed * Time.fixedDeltaTime;
		Vector3 forward3 = base.transform.forward;
		rigidbody.velocity = new Vector3(x, y, forward3.z * Speed * Time.fixedDeltaTime);
		base.get_rigidbody().velocity += new Vector3(Random.Range(0f - Noise.x, Noise.x), Random.Range(0f - Noise.y, Noise.y), Random.Range(0f - Noise.z, Noise.z));
		if (Speed < SpeedMax)
		{
			Speed += SpeedMult * Time.fixedDeltaTime;
		}
	}

	private void Update()
	{
		if (Time.time >= timeCount + LifeTime - 0.5f && (bool)GetComponent<Damage>())
		{
			GetComponent<Damage>().Active();
		}
		if ((bool)Target)
		{
			Quaternion b = Quaternion.LookRotation(Target.transform.position - base.transform.transform.position);
			base.transform.rotation = Quaternion.Slerp(base.transform.rotation, b, Time.deltaTime * Damping);
			Vector3 normalized = (Target.transform.position - base.transform.position).normalized;
			float num = Vector3.Dot(normalized, base.transform.forward);
			if (num < TargetLockDirection)
			{
				Target = null;
			}
		}
		if (!Seeker)
		{
			return;
		}
		if (timetorock > DurationLock)
		{
			if (!locked && !Target)
			{
				float num2 = 2.14748365E+09f;
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
						Vector3 normalized2 = (array[j].transform.position - base.transform.position).normalized;
						float num3 = Vector3.Dot(normalized2, base.transform.forward);
						float num4 = Vector3.Distance(array[j].transform.position, base.transform.position);
						if (num3 >= TargetLockDirection && (float)DistanceLock > num4)
						{
							if (num2 > num4)
							{
								num2 = num4;
								Target = array[j];
							}
							locked = true;
						}
					}
				}
			}
		}
		else
		{
			timetorock++;
		}
		if (!Target)
		{
			locked = false;
		}
	}
}
