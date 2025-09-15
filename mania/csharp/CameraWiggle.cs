using UnityEngine;

public class CameraWiggle : MonoBehaviour
{
	public bool wiggleX;

	public bool wiggleY;

	public bool wiggleZ;

	public bool xCos;

	public bool yCos;

	public bool zCos;

	public float amountX;

	public float amountY;

	public float amountZ;

	public float animationSpeed;

	private float animationProgres;

	private Vector3 tempVector;

	private void Start()
	{
		tempVector = base.transform.position;
	}

	private void Update()
	{
		if (wiggleX)
		{
			float num = 0f;
			num = ((!xCos) ? (tempVector.x + Mathf.Sin(animationProgres) * amountX) : (tempVector.x + Mathf.Cos(animationProgres) * amountX));
			Transform transform = base.transform;
			float x = num;
			Vector3 position = base.transform.position;
			float y = position.y;
			Vector3 position2 = base.transform.position;
			transform.position = new Vector3(x, y, position2.z);
		}
		if (wiggleY)
		{
			float num2 = 0f;
			num2 = ((!yCos) ? (tempVector.y + Mathf.Sin(animationProgres) * amountY) : (tempVector.y + Mathf.Cos(animationProgres) * amountY));
			Transform transform2 = base.transform;
			Vector3 position3 = base.transform.position;
			float x2 = position3.x;
			float y2 = num2;
			Vector3 position4 = base.transform.position;
			transform2.position = new Vector3(x2, y2, position4.z);
		}
		if (wiggleZ)
		{
			float num3 = 0f;
			num3 = ((!zCos) ? (tempVector.z + Mathf.Sin(animationProgres) * amountZ) : (tempVector.z + Mathf.Cos(animationProgres) * amountZ));
			Transform transform3 = base.transform;
			Vector3 position5 = base.transform.position;
			float x3 = position5.x;
			Vector3 position6 = base.transform.position;
			transform3.position = new Vector3(x3, position6.y, num3);
		}
		animationProgres += animationSpeed;
	}
}
