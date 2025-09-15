using UnityEngine;

public class TankMover : MonoBehaviour
{
	public float Speed = 20f;

	public float TurnSpeed = 100f;

	private void Start()
	{
	}

	private void Update()
	{
		base.transform.Rotate(new Vector3(0f, Input.GetAxis("Horizontal") * TurnSpeed * Time.deltaTime, 0f));
		base.transform.position += base.transform.forward * Input.GetAxis("Vertical") * Speed * Time.deltaTime;
	}
}
