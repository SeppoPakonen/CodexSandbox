using UnityEngine;

public class DamageBase : MonoBehaviour
{
	public GameObject Effect;

	[HideInInspector]
	public GameObject Owner;

	public int Damage = 20;

	public string[] TargetTag = new string[1]
	{
		"Enemy"
	};
}
