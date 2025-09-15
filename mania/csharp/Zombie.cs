using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class Zombie : bs
{
	public bool dead;

	private Rigidbody[] rigidbodies;

	private Collider[] colliders;

	private Renderer[] renderers;

	private Vector3 startPos;

	private Quaternion startRot;

	public Collider trigger;

	public Transform hips;

	public AudioClip[] zombieHit;

	public AudioClip[] zombieGroan;

	private float visTime;

	private bool oldVisible;

	private bool oldRagDoll;

	public CharacterController cc;

	public AnimationClip walk;

	public AnimationClip run;

	public bool visible;

	private Vector3 forward;

	private float lastGroan;

	public static int zombieKills;

	public void Start()
	{
		colliders = ((IEnumerable<Collider>)GetComponentsInChildren<Collider>()).Where((Func<Collider, bool>)((Collider a) => a.name != "Cube")).ToArray();
		rigidbodies = GetComponentsInChildren<Rigidbody>();
		renderers = GetComponentsInChildren<Renderer>();
		Collider[] array = colliders;
		foreach (Collider collider in array)
		{
			collider.material = new PhysicMaterial();
			collider.tag = "zombie";
			collider.enabled = false;
		}
		Rigidbody[] array2 = rigidbodies;
		foreach (Rigidbody rigidbody in array2)
		{
			rigidbody.mass *= 0.01f;
			rigidbody.isKinematic = true;
		}
		Renderer[] array3 = renderers;
		foreach (Renderer renderer in array3)
		{
			renderer.enabled = false;
		}
		dead = false;
		forward = bs.ZeroY(UnityEngine.Random.insideUnitSphere, 0f).normalized;
		startPos = base.pos;
		startRot = rot;
		bs._Game.zombies.Add(this);
	}

	public void Update()
	{
		Vector3 point = bs._Player.camera.WorldToViewportPoint(hips.position);
		bool flag = new Rect(0f, 0f, 1f, 1f).Contains(point) && point.z > 0f;
		visible = (flag && !Physics.Linecast(bs._Player.camera.transform.position, hips.position, out RaycastHit _, Layer.levelMask));
		if (visible)
		{
			Debug.DrawLine(base.pos, bs._Player.pos, Color.white);
		}
		bool flag2 = visible && dead;
		if (visible != oldVisible)
		{
			Renderer[] array = renderers;
			foreach (Renderer renderer in array)
			{
				renderer.enabled = visible;
			}
		}
		if (flag2 != oldRagDoll)
		{
			Rigidbody[] array2 = rigidbodies;
			foreach (Rigidbody rigidbody in array2)
			{
				if (rigidbody.isKinematic == flag2)
				{
					rigidbody.WakeUp();
				}
				rigidbody.isKinematic = !flag2;
			}
			Collider[] array3 = colliders;
			foreach (Collider collider in array3)
			{
				collider.enabled = flag2;
			}
		}
		if (visible && !dead && Time.time - lastGroan > 3f)
		{
			lastGroan = Time.time + UnityEngine.Random.value * 3f;
			base.get_audio().clip = zombieGroan[UnityEngine.Random.Range(0, zombieGroan.Length)];
			base.get_audio().Play();
		}
		trigger.enabled = (visible && !flag2);
		CharacterController characterController = cc;
		bool enabled = !dead && visible;
		base.get_animation().enabled = enabled;
		characterController.enabled = enabled;
		if (visible)
		{
			if (cc.enabled)
			{
				Vector3 velocity = cc.velocity;
				if (velocity.y < -30f && visTime > 5f)
				{
					Reset();
				}
			}
			visTime += Time.deltaTime;
			Player player = bs._Player;
			bool flag3 = (player.pos - base.pos).magnitude < 20f;
			if (base.get_animation().enabled)
			{
				if (flag3)
				{
					forward = bs.ZeroY(base.pos - player.pos, 0f).normalized;
				}
				Vector3 speed = ((!flag3) ? 1 : 7) * forward;
				cc.SimpleMove(speed);
				base.transform.forward = forward;
				base.get_animation().CrossFade((!flag3) ? walk.name : run.name);
			}
		}
		oldVisible = visible;
		oldRagDoll = flag2;
	}

	public void Reset()
	{
		base.pos = startPos;
		hips.transform.position = base.pos + Vector3.up;
		rot = startRot;
		dead = false;
	}

	public void Hit()
	{
		base.get_audio().clip = zombieHit[UnityEngine.Random.Range(0, zombieHit.Length)];
		base.get_audio().Play();
		dead = true;
		base.get_animation().enabled = false;
		Collider[] array = colliders;
		foreach (Collider collider in array)
		{
			collider.enabled = true;
		}
		Rigidbody[] array2 = rigidbodies;
		foreach (Rigidbody rigidbody in array2)
		{
			rigidbody.isKinematic = false;
		}
		zombieKills++;
		bs._Awards.ZombieKills.Add();
		bs._Player.AddScore(100f + bs._Player.velm, Color.red);
	}
}
