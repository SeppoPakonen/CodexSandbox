using UnityEngine;

public class Painter : GuiClasses
{
	public static Texture2D PaintLine(Vector2 from, Vector2 to, float rad, Color col, float hardness, Texture2D tex)
	{
		float num = Mathf.Clamp(Mathf.Min(from.y, to.y) - rad, 0f, tex.height);
		float num2 = Mathf.Clamp(Mathf.Min(from.x, to.x) - rad, 0f, tex.width);
		float num3 = Mathf.Clamp(Mathf.Max(from.y, to.y) + rad, 0f, tex.height);
		float num4 = Mathf.Clamp(Mathf.Max(from.x, to.x) + rad, 0f, tex.width) - num2;
		float num5 = num3 - num;
		float num6 = (rad + 1f) * (rad + 1f);
		Color[] pixels = tex.GetPixels((int)num2, (int)num, (int)num4, (int)num5, 0);
		Vector2 b = new Vector2(num2, num);
		for (int i = 0; (float)i < num5; i++)
		{
			for (int j = 0; (float)j < num4; j++)
			{
				Vector2 a = new Vector2(j, i) + b;
				Vector2 vector = a + new Vector2(0.5f, 0.5f);
				float sqrMagnitude = (vector - Mathfx.NearestPointStrict(from, to, vector)).sqrMagnitude;
				if (sqrMagnitude <= num6)
				{
					sqrMagnitude = Mathfx.GaussFalloff(Mathf.Sqrt(sqrMagnitude), rad) * hardness;
					int num7 = (int)((float)i * num4) + j;
					Color color = (!(sqrMagnitude > 0f)) ? pixels[num7] : Color.Lerp(pixels[num7], col, sqrMagnitude);
					pixels[num7] = color;
				}
			}
		}
		tex.SetPixels((int)b.x, (int)b.y, (int)num4, (int)num5, pixels, 0);
		return tex;
	}
}
