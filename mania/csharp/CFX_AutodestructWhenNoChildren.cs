using UnityEngine;

public class CFX_AutodestructWhenNoChildren : MonoBehaviour
{
	private void Update()
	{
		if (base.transform.GetChildCount() == 0)
		{
			Object.Destroy(base.gameObject);
		}
	}
}
