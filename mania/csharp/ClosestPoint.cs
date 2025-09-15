using System.Collections;
using UnityEngine;

public class ClosestPoint : MonoBehaviour
{
	public CurvySpline Target;

	public Transform TargetTransform;

	private IEnumerator Start()
	{
		while (!Target.IsInitialized)
		{
			yield return null;
		}
	}

	private void Update()
	{
		if ((bool)Target && (bool)TargetTransform)
		{
			float nearestPointTF = Target.GetNearestPointTF(base.transform.position);
			TargetTransform.position = Target.Interpolate(nearestPointTF);
		}
	}
}
