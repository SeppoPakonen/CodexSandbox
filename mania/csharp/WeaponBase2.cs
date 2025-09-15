using UnityEngine;

public class WeaponBase2 : MonoBehaviour
{
	[HideInInspector]
	public GameObject Owner;

	[HideInInspector]
	public GameObject Target;

	public string[] TargetTag = new string[1]
	{
		"Enemy"
	};

	public bool RigidbodyProjectile;

	public Vector3 TorqueSpeedAxis;

	public GameObject TorqueObject;
}
