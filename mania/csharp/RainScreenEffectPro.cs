using UnityEngine;

public class RainScreenEffectPro : MonoBehaviour
{
	public GameObject cam1;

	public float effectIntensity = 0.3f;

	public float transitionSpeed = 0.5f;

	public void LateUpdate()
	{
		float @float = base.get_renderer().material.GetFloat("_BumpAmt");
		Material material = base.get_renderer().material;
		Vector3 localEulerAngles = cam1.transform.localEulerAngles;
		material.SetFloat("_BumpAmt", Mathf.Lerp(@float, localEulerAngles.x * effectIntensity, Time.deltaTime * transitionSpeed));
	}
}
