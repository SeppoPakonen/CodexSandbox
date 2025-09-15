using UnityEngine;

public class GA_ExampleBall : MonoBehaviour
{
	public float Speed = 1f;

	private void Start()
	{
		base.get_rigidbody().velocity = new Vector3(Random.value * 0.2f - 0.1f, -1f, 0f) * Speed;
	}

	private void Update()
	{
		base.get_rigidbody().AddForce(Vector3.down * 0.0001f);
		base.get_rigidbody().velocity = base.get_rigidbody().velocity.normalized * Speed;
	}
}
