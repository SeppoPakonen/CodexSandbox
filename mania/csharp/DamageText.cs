using UnityEngine;

public class DamageText : MonoBehaviour
{
	public TextMesh tmMesh;

	public int damage;

	private float time;

	private Vector3 vel;

	private Camera main;

	public void Start()
	{
		main = Camera.main;
		base.transform.LookAt(main.transform);
		vel = Random.insideUnitSphere + Vector3.up * 3f;
	}

	public void Update()
	{
		float deltaTime = Time.deltaTime;
		float d = Mathf.Sqrt((base.transform.position - main.transform.position).magnitude);
		tmMesh.color = new Color(1f, 1f, 1f, 1f - time);
		base.transform.position += vel * d * deltaTime;
		vel += Vector3.down * 10f * deltaTime;
		if (time > 2f)
		{
			Object.Destroy(base.gameObject);
		}
		tmMesh.transform.localScale = new Vector3(-1f, 1f, 1f) * d * 0.2f;
		time += deltaTime;
	}
}
