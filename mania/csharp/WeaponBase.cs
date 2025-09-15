using UnityEngine;

public class WeaponBase : bs
{
	public WeaponEnum weaponEnum;

	public float radiusLimit;

	public bool shootForward;

	public float rotationSpeed = 1f;

	public bool slide;

	internal Player pl;

	internal int id;

	public AudioClip draw;

	public Transform[] barrels;

	public Texture2D cursor;

	internal bool shooting;

	public Transform turret;

	public AudioClip[] bulletHitSound;

	public Vector3 recoilPos;

	public Vector3 recoil;

	public float RecoilRev = 10f;

	internal float shootTm = float.MaxValue;

	public float shootInterval = 0.100111f;

	public bool show => bs.GetFlag((int)bs._Loader.weaponEnum, (int)weaponEnum);

	public Transform turretCannon => barrels[0];

	public Vector3 plPos => pl.pos;

	public override void Awake()
	{
		if (turret == null)
		{
			turret = turretCannon;
		}
		base.Awake();
	}

	public virtual void SetShoot(bool b)
	{
	}

	public void OnSelect()
	{
		pl.PlayOneShot(draw, 1f);
	}

	public virtual void Shoot(int plId, Vector3 dist, PhotonMessageInfo info)
	{
	}

	private Vector3 ClampAngle(Vector3 angle)
	{
		return new Vector3(ClampAngle(angle.x), ClampAngle(angle.y), ClampAngle(angle.z));
	}

	private float ClampAngle(float angle)
	{
		if (angle > 180f)
		{
			angle -= 360f;
		}
		return angle;
	}

	public virtual void Update()
	{
		recoilPos = Vector3.ClampMagnitude(recoilPos, 5f);
		recoilPos = Vector3.MoveTowards(recoilPos, Vector3.zero, Time.deltaTime * RecoilRev);
		recoilPos = Vector3.Lerp(recoilPos, Vector3.zero, Time.deltaTime * RecoilRev * 0.01f);
		if (pl.IsMine)
		{
			bool flag = bs.input.GetKey(KeyCode.Mouse0) && Screen.lockCursor;
			if (flag != shooting && (shootTm > shootInterval || this is WeaponFlameTower))
			{
				pl.CallRPC(SetShoot, flag);
			}
		}
	}

	public virtual void UpdateAlways()
	{
		Vector3 angle = Quaternion.LookRotation(base.transform.InverseTransformDirection(pl.turretDirection)).eulerAngles;
		if (radiusLimit != 0f)
		{
			angle = ClampAngle(angle);
			angle = Vector3.ClampMagnitude(angle, radiusLimit);
		}
		if (turret != null)
		{
			Vector3 localEulerAngles = turret.localEulerAngles;
			localEulerAngles.y = Mathf.MoveTowardsAngle(localEulerAngles.y, angle.y + recoilPos.y, Time.deltaTime * 100f * rotationSpeed);
			turret.localEulerAngles = localEulerAngles;
			Vector3 localEulerAngles2 = turretCannon.localEulerAngles;
			localEulerAngles2.x = Mathf.MoveTowardsAngle(localEulerAngles2.x, angle.x + recoilPos.x, Time.deltaTime * 100f * rotationSpeed);
			Transform[] array = barrels;
			foreach (Transform transform in array)
			{
				transform.localEulerAngles = localEulerAngles2;
			}
		}
	}
}
