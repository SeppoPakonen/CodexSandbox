using System;
using System.Linq;
using UnityEngine;

public class VoiceChatBase : MonoBehaviour
{
	private float middle;

	public void NormalizeSample(float[] sample)
	{
		float num = Math.Max(middle, Mathf.Lerp(middle, sample.Max() * 10f, 0.1f));
		if (num > middle)
		{
			MonoBehaviour.print("Set Max" + middle);
		}
		middle = num;
		for (int i = 0; i < sample.Length; i++)
		{
			sample[i] *= 20f / middle;
		}
	}
}
