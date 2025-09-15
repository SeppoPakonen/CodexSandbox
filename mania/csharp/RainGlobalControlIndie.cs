using UnityEngine;

public class RainGlobalControlIndie : MonoBehaviour
{
	public float rainLevel = 1f;

	public int transitionSpeed = 2;

	public void Update()
	{
		rainLevel = Mathf.Clamp(rainLevel, 0f, 2f);
		GameObject[] array = GameObject.FindGameObjectsWithTag("RainParticles");
		for (int i = 0; i < array.Length; i++)
		{
			Color color = array[i].get_renderer().material.color;
			Mathf.Lerp(color.a, rainLevel * 0.2f, Time.deltaTime * (float)transitionSpeed);
		}
		GameObject[] array2 = GameObject.FindGameObjectsWithTag("RainPro");
		for (int j = 0; j < array2.Length; j++)
		{
			float @float = array2[j].get_renderer().material.GetFloat("_BumpAmt");
			array2[j].get_renderer().material.SetFloat("_BumpAmt", Mathf.Lerp(@float, rainLevel * 60f, Time.deltaTime * (float)transitionSpeed));
		}
		GameObject[] array3 = GameObject.FindGameObjectsWithTag("RainRipples");
		for (int k = 0; k < array3.Length; k++)
		{
			Color color2 = array3[k].get_renderer().material.color;
			Mathf.Lerp(color2.a, rainLevel * 0.3f, Time.deltaTime * (float)transitionSpeed);
		}
		GameObject[] array4 = GameObject.FindGameObjectsWithTag("RainSound");
		for (int l = 0; l < array4.Length; l++)
		{
			array4[l].get_audio().volume = Mathf.Lerp(array4[l].get_audio().volume, rainLevel, Time.deltaTime * (float)transitionSpeed);
		}
		MonoBehaviour.print("Rain Level " + rainLevel);
	}
}
