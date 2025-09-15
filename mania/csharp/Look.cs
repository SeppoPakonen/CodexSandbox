using UnityEngine;

public class Look : MonoBehaviour
{
	public Transform Target;

	private Transform mTransform;

	private void Start()
	{
		mTransform = base.transform;
	}

	private void LateUpdate()
	{
		if ((bool)Target)
		{
			mTransform.LookAt(Target);
		}
	}
}
