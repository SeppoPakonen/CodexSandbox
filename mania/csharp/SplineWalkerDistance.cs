using System.Collections;
using UnityEngine;

[ExecuteInEditMode]
public class SplineWalkerDistance : MonoBehaviour
{
	public CurvySpline Spline;

	public CurvyClamping Clamping;

	public bool SetOrientation = true;

	public bool FastInterpolation;

	public float InitialDistance;

	public float Speed;

	private float mDistance;

	private int mDir;

	private Transform mTransform;

	public float Distance
	{
		get
		{
			return mDistance;
		}
		set
		{
			mDistance = value;
		}
	}

	private IEnumerator Start()
	{
		mDistance = InitialDistance;
		mDir = ((Speed >= 0f) ? 1 : (-1));
		Speed = Mathf.Abs(Speed);
		mTransform = base.transform;
		if ((bool)Spline)
		{
			while (!Spline.IsInitialized)
			{
				yield return null;
			}
			InitPosAndRot();
		}
	}

	private void Update()
	{
		if (!Spline || !Spline.IsInitialized)
		{
			return;
		}
		if (Application.isPlaying)
		{
			float tf = Spline.DistanceToTF(mDistance);
			mTransform.position = ((!FastInterpolation) ? Spline.MoveBy(ref tf, ref mDir, Speed * Time.deltaTime, Clamping) : Spline.MoveByFast(ref tf, ref mDir, Speed * Time.deltaTime, Clamping));
			mDistance = Spline.TFToDistance(tf);
			if (SetOrientation)
			{
				base.transform.rotation = Spline.GetOrientationFast(tf);
			}
		}
		else
		{
			InitPosAndRot();
		}
	}

	private void InitPosAndRot()
	{
		if ((bool)Spline)
		{
			float tf = Spline.DistanceToTF(InitialDistance);
			mTransform.position = Spline.Interpolate(tf);
			if (SetOrientation)
			{
				mTransform.rotation = Spline.GetOrientationFast(tf);
			}
		}
	}
}
