using UnityEngine;

public class FullScreenImage : bs
{
	public void Update()
	{
		Texture texture = base.get_guiTexture().texture;
		Vector3 one = Vector3.one;
		float num = (float)Screen.width / (float)Screen.height / ((float)texture.width / (float)texture.height);
		float x = (float)Screen.height / (float)Screen.width / ((float)texture.height / (float)texture.width);
		if (num < 1f)
		{
			one.x = x;
		}
		else
		{
			one.y = num;
		}
		base.transform.localScale = one;
	}
}
