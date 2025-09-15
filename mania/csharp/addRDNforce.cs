using UnityEngine;

public class addRDNforce : MonoBehaviour
{
	public GameObject emitter;

	private void Update()
	{
		if (Input.GetMouseButtonDown(0))
		{
			emitter.get_rigidbody().AddForce(new Vector3(Random.Range(-1f, 1f) * 10000f, Random.Range(-1f, 1f) * 10000f, Random.Range(-1f, 1f) * 10000f));
		}
	}
}
