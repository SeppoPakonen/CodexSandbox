using System;
using UnityEngine;

public class Weapon : WeaponBase
{
	public Bullet bullet;

	public float accuracy = 0.04f;

	public int bullets;

	public int bulletsTotal;

	public float carSpeed;

	public float damage = 11f;

	public Light muzzleFlashLight;

	public Renderer muzzleFlash;

	public ParticleEmitter[] capsule;

	public AudioClip[] shootSound;

	private float lastShoot;

	private int shootCount;

	public override void Update()
	{
		if (pl.dead || pl.froozen)
		{
			return;
		}
		base.Update();
		if (muzzleFlash != null)
		{
			muzzleFlash.enabled = (Time.time - lastShoot < 0.03f);
			muzzleFlashLight.enabled = (Time.time - lastShoot < 0.05f);
		}
		if (shooting && shootTm >= shootInterval)
		{
			shootTm %= shootInterval;
			lastShoot = Time.time;
			if (shootSound.Length > 0)
			{
				pl.PlayOneShot(shootSound[UnityEngine.Random.Range(0, shootSound.Length)], 1f);
			}
			ParticleEmitter[] array = capsule;
			foreach (ParticleEmitter particleEmitter in array)
			{
				particleEmitter.Emit();
			}
			if (pl.IsMine)
			{
				pl.CallRPC((Action<int, Vector3, PhotonMessageInfo>)Shoot, pl.TargetPlayerId, pl.distanceToCursor2);
			}
		}
	}

	public override void UpdateAlways()
	{
		shootTm += Time.deltaTime;
		base.UpdateAlways();
	}

	public override void Shoot(int plId, Vector3 dist, PhotonMessageInfo info)
	{
		if ((plId == -1 || bs._Game.photonPlayers.ContainsKey(plId)) && this.bullet != null)
		{
			shootCount++;
			Transform transform = barrels[shootCount % barrels.Length];
			pl.distanceToCursor2 = dist;
			pl.TargetPlayerId = plId;
			pl.UpdateTurretEuler(shooting: true);
			Bullet bullet = (Bullet)UnityEngine.Object.Instantiate((UnityEngine.Object)this.bullet, transform.position, Quaternion.LookRotation(pl.shootDirection.normalized));
			bullet.gameObject.hideFlags = HideFlags.HideInHierarchy;
			bullet.bulletSpeed += pl.velm;
			bullet.maxSpeed += (int)pl.velm;
			bullet.wep = this;
			bullet.extraTime = (float)(PhotonNetwork.time - info.timestamp);
		}
	}

	public override void SetShoot(bool b)
	{
		shootTm = shootInterval;
		shooting = b;
	}
}
