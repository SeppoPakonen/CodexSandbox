using UnityEngine;

public class scaleRotation : MonoBehaviour
{
	private float p;

	public float speed;

	private Vector3 oS;

	private void Awake()
	{
		oS = base.transform.localScale;
	}

	private void Update()
	{
		base.transform.localScale = new Vector3(0.1f + oS.x + Mathf.Sin(p) / 2f, 0.1f + oS.y + Mathf.Sin(p) / 2f, 0.1f + oS.z + Mathf.Sin(p) / 2f);
		p += speed * Time.deltaTime;
		base.transform.Rotate(new Vector3(1f, 1f, 0f) * 20f * Time.deltaTime);
	}
}
