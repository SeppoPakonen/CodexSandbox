using UnityEngine;

public class LookAtCamera : MonoBehaviour
{
	private Camera cam;

	private Transform camT;

	public void Start()
	{
	}

	public void Update()
	{
		if (cam == null || !cam.enabled)
		{
			cam = Camera.main;
			camT = cam.transform;
		}
		if (cam != null)
		{
			base.transform.LookAt(camT, camT.up);
		}
	}
}
