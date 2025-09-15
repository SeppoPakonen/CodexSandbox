using UnityEngine;

public class RainAnimator : MonoBehaviour
{
	public Texture2D[] frames;

	public float framesPerSecond = 16f;

	public static bool on;

	public void Start()
	{
		on = true;
	}

	public void Update()
	{
		if (on)
		{
			int num = (int)(Time.time * framesPerSecond);
			num %= frames.Length;
			base.get_renderer().material.SetTexture("_BumpMap", frames[num]);
		}
	}
}
