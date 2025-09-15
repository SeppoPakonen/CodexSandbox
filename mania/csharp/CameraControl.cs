using UnityEngine;

public class CameraControl : MonoBehaviour
{
	public Transform Character;

	public float Distance = 10f;

	public float Height = 2f;

	private Transform mTransform;

	private void Start()
	{
		mTransform = base.transform;
	}

	private void Update()
	{
		Vector3 position = Character.position;
		Vector3 vector = new Vector3(0f, position.y, 0f);
		Vector3 position2 = Character.position;
		Vector3 vector2 = new Ray(vector, position2 - vector).GetPoint((position2 - vector).magnitude + Distance) + new Vector3(0f, Height, 0f);
		Transform transform = mTransform;
		Vector3 position3 = mTransform.position;
		float x = Mathf.Lerp(position3.x, vector2.x, 0.08f);
		Vector3 position4 = mTransform.position;
		float y = Mathf.Lerp(position4.y, vector2.y, 0.01f);
		Vector3 position5 = mTransform.position;
		transform.position = new Vector3(x, y, Mathf.Lerp(position5.z, vector2.z, 0.08f));
		mTransform.LookAt(vector);
	}
}
