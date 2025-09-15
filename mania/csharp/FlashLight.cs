using UnityEngine;

public class FlashLight : MonoBehaviour
{
	public float LightMult = 2f;

	private void Update()
	{
		if ((bool)base.get_light())
		{
			base.get_light().intensity -= LightMult * Time.deltaTime;
		}
	}
}
