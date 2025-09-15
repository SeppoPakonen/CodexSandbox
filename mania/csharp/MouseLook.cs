using UnityEngine;

[AddComponentMenu("Camera-Control/Mouse Look")]
public class MouseLook : MonoBehaviour
{
	public enum RotationAxes
	{
		MouseXAndY,
		MouseX,
		MouseY
	}

	public RotationAxes axes;

	public float sensitivityX = 15f;

	public float sensitivityY = 15f;

	public float minimumX = -360f;

	public float maximumX = 360f;

	public float minimumY = -60f;

	public float maximumY = 60f;

	private float rotationY;

	private void Update()
	{
		if (axes == RotationAxes.MouseXAndY)
		{
			Vector3 localEulerAngles = base.transform.localEulerAngles;
			float y = localEulerAngles.y + Input.GetAxis("Mouse X") * sensitivityX;
			rotationY += Input.GetAxis("Mouse Y") * sensitivityY;
			rotationY = Mathf.Clamp(rotationY, minimumY, maximumY);
			base.transform.localEulerAngles = new Vector3(0f - rotationY, y, 0f);
		}
		else if (axes == RotationAxes.MouseX)
		{
			base.transform.Rotate(0f, Input.GetAxis("Mouse X") * sensitivityX, 0f);
		}
		else
		{
			rotationY += Input.GetAxis("Mouse Y") * sensitivityY;
			rotationY = Mathf.Clamp(rotationY, minimumY, maximumY);
			Transform transform = base.transform;
			float x = 0f - rotationY;
			Vector3 localEulerAngles2 = base.transform.localEulerAngles;
			transform.localEulerAngles = new Vector3(x, localEulerAngles2.y, 0f);
		}
	}

	private void Start()
	{
		if ((bool)base.get_rigidbody())
		{
			base.get_rigidbody().freezeRotation = true;
		}
	}
}
