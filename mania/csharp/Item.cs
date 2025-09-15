using UnityEngine;

public class Item : MonoBehaviour
{
	public float speed = 3.5f;

	public void Start()
	{
		if (speed == 0f)
		{
			speed = 3.5f;
		}
	}
}
