using UnityEngine;

public class GameSettings : MonoBehaviour
{
	public float miny = -200f;

	public bool InitColliders;

	public float PlayerFriq = 1f;

	public float Rotation = 1f;

	public float Brake = 1f;

	public bool ownGravity = true;

	public float speed = 1f;

	public bool enableBorderHit = true;

	public float gravitationAntiFly = 1f;

	public float gravitationFactor = 1f;

	public float sendWmpOffset;

	public int laps;

	public bool disableFlyDir;

	public float drag = 0.06f;

	internal Bounds levelBounds;

	internal float levelTime = 180f;

	public void Start()
	{
		if (gravitationFactor == 0f)
		{
			gravitationFactor = 1f;
		}
		if (gravitationAntiFly == 0f)
		{
			gravitationAntiFly = 1f;
		}
		if (speed == 0f)
		{
			speed = 1f;
		}
		if (gravitationAntiFly == 1f)
		{
			gravitationAntiFly = 1.5f;
		}
	}
}
