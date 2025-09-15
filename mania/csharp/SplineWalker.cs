using System.Collections;
using UnityEngine;

[ExecuteInEditMode]
public class SplineWalker : MonoBehaviour
{
	public CurvySpline Spline;

	public CurvyClamping Clamping;

	public bool SetOrientation = true;

	public bool FastInterpolation;

	public bool MoveByWorldUnits;

	public float InitialF;

	public float Speed;

	private float mTF;

	private int mDir;

	private Transform mTransform;

	public float TF
	{
		get
		{
			return mTF;
		}
		set
		{
			mTF = value;
		}
	}

	private IEnumerator Start()
	{
		mTF = InitialF;
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
		if (!Spline)
		{
			return;
		}
		if (Application.isPlaying)
		{
			if (MoveByWorldUnits)
			{
				mTransform.position = ((!FastInterpolation) ? Spline.MoveBy(ref mTF, ref mDir, Speed * Time.deltaTime, Clamping) : Spline.MoveByFast(ref mTF, ref mDir, Speed * Time.deltaTime, Clamping));
			}
			else
			{
				mTransform.position = ((!FastInterpolation) ? Spline.Move(ref mTF, ref mDir, Speed * Time.deltaTime, Clamping) : Spline.MoveFast(ref mTF, ref mDir, Speed * Time.deltaTime, Clamping));
			}
			if (SetOrientation)
			{
				base.transform.rotation = Spline.GetOrientationFast(mTF);
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
			mTransform.position = Spline.Interpolate(InitialF);
			if (SetOrientation)
			{
				mTransform.rotation = Spline.GetOrientationFast(InitialF);
			}
		}
	}
}
