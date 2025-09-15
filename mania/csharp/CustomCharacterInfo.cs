using UnityEngine;

public class CustomCharacterInfo
{
	public bool flipped;

	public Rect uv = default(Rect);

	public Rect vert = default(Rect);

	public float width;

	public CustomCharacterInfo ScaleClone(float scale)
	{
		CustomCharacterInfo customCharacterInfo = new CustomCharacterInfo();
		customCharacterInfo.flipped = flipped;
		customCharacterInfo.uv = new Rect(uv);
		customCharacterInfo.vert = new Rect(vert);
		customCharacterInfo.width = width;
		customCharacterInfo.vert.x /= scale;
		customCharacterInfo.vert.y /= scale;
		customCharacterInfo.vert.width /= scale;
		customCharacterInfo.vert.height /= scale;
		customCharacterInfo.width /= scale;
		return customCharacterInfo;
	}
}
