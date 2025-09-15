using UnityEngine;

public class rotate : MonoBehaviour
{
	public float rot;

	private void Update()
	{
		base.transform.Rotate(new Vector3(0f, rot * Time.deltaTime, 0f));
	}
}
