using UnityEngine;

public class Mathfx
{
	public static float GaussFalloff(float distance, float inRadius)
	{
		return Mathf.Clamp01(Mathf.Pow(360f, 0f - Mathf.Pow(distance / inRadius, 2.5f) - 0.01f));
	}

	public static Vector2 NearestPointStrict(Vector2 lineStart, Vector2 lineEnd, Vector2 point)
	{
		Vector2 p = lineEnd - lineStart;
		Vector2 vector = Normalize(p);
		float value = Vector2.Dot(point - lineStart, vector) / Vector2.Dot(vector, vector);
		return lineStart + Mathf.Clamp(value, 0f, p.magnitude) * vector;
	}

	public static Vector2 Normalize(Vector2 p)
	{
		float magnitude = p.magnitude;
		return p / magnitude;
	}
}
