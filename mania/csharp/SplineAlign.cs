using System.Collections;
using UnityEngine;

[ExecuteInEditMode]
public class SplineAlign : MonoBehaviour
{
	public CurvySpline Spline;

	public float Distance;

	public bool UseWorldUnits;

	public bool SetOrientation = true;

	private IEnumerator Start()
	{
		if ((bool)Spline)
		{
			while (!Spline.IsInitialized)
			{
				yield return null;
			}
			Set();
		}
	}

	private void Update()
	{
		if ((bool)Spline && !Application.isPlaying)
		{
			Set();
		}
	}

	private void Set()
	{
		float tf;
		if (UseWorldUnits)
		{
			if (Distance >= Spline.Length)
			{
				Distance -= Spline.Length;
			}
			else if (Distance < 0f)
			{
				Distance += Spline.Length;
			}
			tf = Spline.DistanceToTF(Distance);
		}
		else
		{
			if (Distance >= 1f)
			{
				Distance -= 1f;
			}
			else if (Distance < 0f)
			{
				Distance += 1f;
			}
			tf = Distance;
		}
		base.transform.position = Spline.Interpolate(tf);
		if (SetOrientation)
		{
			base.transform.rotation = Spline.GetOrientationFast(tf);
		}
	}
}
