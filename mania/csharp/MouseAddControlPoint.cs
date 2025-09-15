using System.Collections;
using UnityEngine;

public class MouseAddControlPoint : MonoBehaviour
{
	public bool RemoveUnusedSegments = true;

	private CurvySpline mSpline;

	private SplineWalkerDistance Walker;

	private IEnumerator Start()
	{
		mSpline = GetComponent<CurvySpline>();
		Walker = (Object.FindObjectOfType(typeof(SplineWalkerDistance)) as SplineWalkerDistance);
		while (!mSpline.IsInitialized)
		{
			yield return null;
		}
	}

	private void Update()
	{
		if (!Input.GetMouseButtonDown(0))
		{
			return;
		}
		Vector3 vector = Input.mousePosition;
		vector.z = 10f;
		vector = Camera.main.ScreenToWorldPoint(vector);
		mSpline.Add(vector);
		if (RemoveUnusedSegments)
		{
			CurvySplineSegment x = mSpline.DistanceToSegment(Walker.Distance);
			if (x != mSpline[0])
			{
				Walker.Distance -= mSpline[0].Length;
				mSpline.Delete(mSpline[0]);
			}
		}
	}
}
