using UnityEngine;

public class DamageManager : MonoBehaviour
{
	public AudioClip[] HitSound;

	public GameObject Effect;

	public int HP = 100;

	private void Start()
	{
	}

	public void ApplyDamage(int damage)
	{
		if (HP >= 0)
		{
			if (HitSound.Length > 0)
			{
				AudioSource.PlayClipAtPoint(HitSound[Random.Range(0, HitSound.Length)], base.transform.position);
			}
			HP -= damage;
			if (HP <= 0)
			{
				Dead();
			}
		}
	}

	private void Dead()
	{
		if ((bool)Effect)
		{
			Object.Instantiate((Object)Effect, base.transform.position, base.transform.rotation);
		}
		Object.Destroy(base.gameObject);
	}
}
