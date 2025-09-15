using UnityEngine;

public class AILook : MonoBehaviour
{
	public string[] TargetTag = new string[1]
	{
		"Enemy"
	};

	private int indexWeapon;

	private GameObject target;

	private WeaponController weapon;

	private float timeAIattack;

	private void Start()
	{
		weapon = GetComponent<WeaponController>();
	}

	private void Update()
	{
		if ((bool)target)
		{
			Quaternion b = Quaternion.LookRotation(target.transform.position - base.transform.position);
			base.transform.rotation = Quaternion.Lerp(base.transform.rotation, b, Time.deltaTime * 3f);
			Vector3 normalized = (target.transform.position - base.transform.position).normalized;
			float num = Vector3.Dot(normalized, base.transform.forward);
			if (num > 0.9f && (bool)weapon)
			{
				weapon.LaunchWeapon(indexWeapon);
			}
			if (Time.time > timeAIattack + 3f)
			{
				target = null;
			}
			return;
		}
		for (int i = 0; i < TargetTag.Length; i++)
		{
			if (GameObject.FindGameObjectsWithTag(TargetTag[i]).Length <= 0)
			{
				continue;
			}
			GameObject[] array = GameObject.FindGameObjectsWithTag(TargetTag[i]);
			float num2 = 2.14748365E+09f;
			for (int j = 0; j < array.Length; j++)
			{
				float num3 = Vector3.Distance(array[j].transform.position, base.transform.position);
				if (num2 > num3)
				{
					num2 = num3;
					target = array[j];
					if ((bool)weapon)
					{
						indexWeapon = Random.Range(0, weapon.WeaponLists.Length);
					}
					timeAIattack = Time.time;
				}
			}
		}
	}
}
