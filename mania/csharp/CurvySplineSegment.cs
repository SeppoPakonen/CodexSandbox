using System;
using System.Collections.Generic;
using UnityEngine;

public class CurvySplineSegment : MonoBehaviour
{
	public bool SynchronizeTCB = true;

	public bool SmoothEdgeTangent;

	public bool OverrideGlobalTension;

	public bool OverrideGlobalContinuity;

	public bool OverrideGlobalBias;

	public float StartTension;

	public float StartContinuity;

	public float StartBias;

	public float EndTension;

	public float EndContinuity;

	public float EndBias;

	public Vector3[] UserValues = new Vector3[0];

	[HideInInspector]
	public Vector3[] Approximation = new Vector3[0];

	[HideInInspector]
	public float[] ApproximationDistances = new float[0];

	[HideInInspector]
	public Vector3[] ApproximationUp = new Vector3[0];

	[HideInInspector]
	public Vector3[] ApproximationT = new Vector3[0];

	private Transform mTransform;

	private Vector3 mPosition;

	private Quaternion mRotation;

	public CurvySpline mSpline;

	private float mStepSize;

	private int mControlPointIndex;

	private int mSegmentIndex;

	public float swirl;

	public List<SplinePathMeshBuilder> sbs = new List<SplinePathMeshBuilder>();

	public List<CurvySpline2> spls = new List<CurvySpline2>();

	internal Vector3 pivot;

	internal Vector3 pivotFv;

	internal float dist = float.MaxValue;

	internal float z;

	internal Vector3 point;

	public bool flying;

	public Vector3 dragOffset;

	public Transform Transform
	{
		get
		{
			if (!mTransform)
			{
				mTransform = base.transform;
			}
			return mTransform;
		}
	}

	public float Length
	{
		get;
		private set;
	}

	public float Distance
	{
		get;
		private set;
	}

	public Vector3 Position
	{
		get
		{
			return Transform.position;
		}
		set
		{
			Transform.position = value;
		}
	}

	public bool IsValidSegment
	{
		get
		{
			switch (Spline.Interpolation)
			{
			case CurvyInterpolation.Linear:
				return (bool)Transform && (bool)NextTransform;
			case CurvyInterpolation.CatmulRom:
			case CurvyInterpolation.TCB:
				return (bool)Transform && (bool)PreviousTransform && (bool)NextControlPoint && (bool)NextControlPoint.NextTransform;
			default:
				return false;
			}
		}
	}

	public bool IsFirstSegment => !PreviousSegment || (Spline.Closed && PreviousSegment == Spline[Spline.Count - 1]);

	public bool IsLastSegment => !NextSegment || (Spline.Closed && NextSegment == Spline[0]);

	public CurvySplineSegment NextControlPoint => Spline.NextControlPoint(this);

	public CurvySplineSegment PreviousControlPoint => Spline.PreviousControlPoint(this);

	public Transform NextTransform => Spline.NextTransform(this);

	public Transform PreviousTransform => Spline.PreviousTransform(this);

	public CurvySplineSegment NextSegment => Spline.NextSegment(this);

	public CurvySplineSegment PreviousSegment => Spline.PreviousSegment(this);

	public int SegmentIndex => mSegmentIndex;

	public int ControlPointIndex => mControlPointIndex;

	public CurvySpline Spline
	{
		get
		{
			if (!mSpline)
			{
				mSpline = base.transform.parent.GetComponent<CurvySpline>();
			}
			return mSpline;
		}
	}

	public float scale
	{
		get
		{
			Vector3 localScale = base.transform.localScale;
			return localScale.x;
		}
		set
		{
			base.transform.localScale = new Vector3(value, 1f, 1f);
		}
	}

	public bool isEnd => NextControlPoint == null || PreviousControlPoint == null;

	public CurvySpline2 Spline2 => (CurvySpline2)Spline;

	private void OnDrawGizmos()
	{
		DoGizmos(selected: false);
	}

	private void OnDrawGizmosSelected()
	{
		DoGizmos(selected: true);
	}

	private void DoGizmos(bool selected)
	{
	}

	private void OnEnable()
	{
		mPosition = Transform.position;
		mRotation = Transform.rotation;
	}

	public void Delete()
	{
		Spline.Delete(this);
	}

	public Vector3 Interpolate(float localF)
	{
		localF = Mathf.Clamp01(localF);
		switch (Spline.Interpolation)
		{
		case CurvyInterpolation.CatmulRom:
			return Spline.CatmulRom(PreviousTransform.position, Transform.position, NextTransform.position, NextControlPoint.NextTransform.position, localF);
		case CurvyInterpolation.TCB:
		{
			float fT = StartTension;
			float fT2 = EndTension;
			float fC = StartContinuity;
			float fC2 = EndContinuity;
			float fB = StartBias;
			float fB2 = EndBias;
			if (!OverrideGlobalTension)
			{
				fT = (fT2 = Spline.Tension);
			}
			if (!OverrideGlobalContinuity)
			{
				fC = (fC2 = Spline.Continuity);
			}
			if (!OverrideGlobalBias)
			{
				fB = (fB2 = Spline.Bias);
			}
			return Spline.TCB(PreviousTransform.position, Transform.position, NextTransform.position, NextControlPoint.NextTransform.position, localF, fT, fC, fB, fT2, fC2, fB2);
		}
		default:
			return Vector3.Lerp(Transform.position, NextTransform.position, localF);
		}
	}

	public Vector3 InterpolateFast(float localF)
	{
		float frag;
		int approximationIndex = getApproximationIndex(localF, out frag);
		return Vector3.Lerp(Approximation[approximationIndex], Approximation[approximationIndex + 1], frag);
	}

	public Vector3 InterpolateUserValue(float localF, int index)
	{
		if (index >= Spline.UserValueSize || NextControlPoint == null)
		{
			return Vector3.zero;
		}
		return Vector3.Lerp(UserValues[index], NextControlPoint.UserValues[index], localF);
	}

	public Vector3 InterpolateScale(float localF)
	{
		Transform nextTransform = NextTransform;
		return (!nextTransform) ? Transform.lossyScale : Vector3.Lerp(Transform.lossyScale, nextTransform.lossyScale, localF);
	}

	public Vector3 GetTangent(float localF)
	{
		localF = Mathf.Clamp01(localF);
		Vector3 position = Interpolate(localF);
		return GetTangent(localF, ref position);
	}

	public Vector3 GetTangent(float localF, ref Vector3 position)
	{
		float num = localF + 0.01f;
		if (num > 1f)
		{
			if ((bool)NextSegment)
			{
				return (NextSegment.Interpolate(num - 1f) - position).normalized;
			}
			num = localF - 0.01f;
			return (position - Interpolate(num)).normalized;
		}
		return (Interpolate(num) - position).normalized;
	}

	public Vector3 GetTangentFast(float localF)
	{
		float frag;
		int approximationIndex = getApproximationIndex(localF, out frag);
		return Vector3.Lerp(ApproximationT[approximationIndex], ApproximationT[approximationIndex + 1], frag);
	}

	public Quaternion GetOrientationFast(float localF)
	{
		Vector3 tangentFast = GetTangentFast(localF);
		if (tangentFast != Vector3.zero)
		{
			return Quaternion.LookRotation(GetTangentFast(localF), GetOrientationUpFast(localF));
		}
		return Quaternion.identity;
	}

	public Vector3 GetOrientationUpFast(float localF)
	{
		float frag;
		int approximationIndex = getApproximationIndex(localF, out frag);
		return Vector3.Lerp(ApproximationUp[approximationIndex], ApproximationUp[approximationIndex + 1], frag);
	}

	public float DistanceToLocalF(float localDistance)
	{
		localDistance = Mathf.Clamp(localDistance, 0f, Length);
		if (ApproximationDistances.Length == 0 || localDistance == 0f)
		{
			return 0f;
		}
		if (Mathf.Approximately(localDistance, Length))
		{
			return 1f;
		}
		int num = ApproximationDistances.Length - 1;
		while (ApproximationDistances[num] > localDistance)
		{
			num--;
		}
		float num2 = (localDistance - ApproximationDistances[num]) / (ApproximationDistances[num + 1] - ApproximationDistances[num]);
		float num3 = _getApproximationLocalF(num);
		float num4 = _getApproximationLocalF(num + 1);
		return num3 + (num4 - num3) * num2;
	}

	public float LocalFToDistance(float localF)
	{
		localF = Mathf.Clamp01(localF);
		float frag;
		int approximationIndex = getApproximationIndex(localF, out frag);
		float num = ApproximationDistances[approximationIndex + 1] - ApproximationDistances[approximationIndex];
		return ApproximationDistances[approximationIndex] + num * frag;
	}

	public float LocalFToTF(float localF)
	{
		return Spline.SegmentToTF(this, localF);
	}

	public bool SnapToFitSplineLength(float newSplineLength, float stepSize)
	{
		if (stepSize == 0f || Mathf.Approximately(newSplineLength, Spline.Length))
		{
			return true;
		}
		float length = Spline.Length;
		Vector3 position = Transform.position;
		Vector3 vector = Transform.up * stepSize;
		Transform.position += vector;
		Spline.Refresh(refreshLength: true, refreshOrientation: false, skipIfInitialized: false);
		bool flag = Spline.Length > length;
		int num = 30000;
		Transform.position = position;
		if (newSplineLength > length)
		{
			if (!flag)
			{
				vector *= -1f;
			}
			while (Spline.Length < newSplineLength)
			{
				num--;
				length = Spline.Length;
				Transform.position += vector;
				Spline.Refresh(refreshLength: true, refreshOrientation: false, skipIfInitialized: false);
				if (length > Spline.Length)
				{
					return false;
				}
				if (num == 0)
				{
					Debug.LogError("CurvySplineSegment.SnapToFitSplineLength exceeds 30000 loops, considering this a dead loop! This shouldn't happen, please report this as an error!");
					return false;
				}
			}
		}
		else
		{
			if (flag)
			{
				vector *= -1f;
			}
			while (Spline.Length > newSplineLength)
			{
				num--;
				length = Spline.Length;
				Transform.position += vector;
				Spline.Refresh(refreshLength: true, refreshOrientation: false, skipIfInitialized: false);
				if (length < Spline.Length)
				{
					return false;
				}
				if (num == 0)
				{
					Debug.LogError("CurvySplineSegment.SnapToFitSplineLength exceeds 30000 loops, considering this a dead loop! This shouldn't happen, please report this as an error!");
					return false;
				}
			}
		}
		return true;
	}

	public float _getApproximationLocalF(int idx)
	{
		return (float)idx * mStepSize;
	}

	private int getApproximationIndex(float localF, out float frag)
	{
		localF = Mathf.Clamp01(localF);
		if (localF == 1f)
		{
			frag = 1f;
			return Approximation.Length - 2;
		}
		float num = localF / mStepSize;
		int num2 = (int)num;
		frag = num - (float)num2;
		return num2;
	}

	public void _InitializeControlPoint()
	{
		mStepSize = 1f / (float)Spline.Granularity;
		mControlPointIndex = Spline.ControlPoints.IndexOf(this);
		Approximation = new Vector3[0];
		ApproximationDistances = new float[0];
		ApproximationUp = new Vector3[0];
		ApproximationT = new Vector3[0];
		if (UserValues.Length != Spline.UserValueSize)
		{
			Resize(ref UserValues, Spline.UserValueSize);
		}
	}

	public static void Resize(ref Vector3[] userValues, int userValueSize)
	{
		Vector3[] array = new Vector3[userValueSize];
		int num = Mathf.Min(userValueSize, userValues.Length);
		for (int i = 0; i < num; i++)
		{
			ref Vector3 reference = ref array[i];
			reference = userValues[i];
		}
		userValues = array;
	}

	public static void Resize(ref Vector2[] userValues, int userValueSize)
	{
		Vector2[] array = new Vector2[userValueSize];
		int num = Mathf.Min(userValueSize, userValues.Length);
		for (int i = 0; i < num; i++)
		{
			ref Vector2 reference = ref array[i];
			reference = userValues[i];
		}
		userValues = array;
	}

	public void _UpdateApproximation()
	{
		if (IsValidSegment)
		{
			mSegmentIndex = Spline.Segments.IndexOf(this);
			mStepSize = 1f / (float)Spline.Granularity;
			Approximation = new Vector3[Spline.Granularity + 1];
			ApproximationUp = new Vector3[Spline.Granularity + 1];
			ApproximationT = new Vector3[Spline.Granularity + 1];
			ref Vector3 reference = ref Approximation[0];
			reference = Position;
			ref Vector3 reference2 = ref Approximation[Spline.Granularity];
			reference2 = NextTransform.position;
			int num = Approximation.Length - 1;
			for (int i = 1; i < num; i++)
			{
				ref Vector3 reference3 = ref Approximation[i];
				reference3 = Interpolate((float)i * mStepSize);
			}
		}
	}

	public float _UpdateLength()
	{
		CurvySplineSegment previousSegment = PreviousSegment;
		Distance = ((!previousSegment || !previousSegment.IsValidSegment || !(Spline[0] != this)) ? 0f : (previousSegment.Distance + previousSegment.Length));
		Length = 0f;
		if (IsValidSegment)
		{
			ApproximationDistances = new float[Approximation.Length];
			int num = ApproximationDistances.Length;
			for (int i = 1; i < num; i++)
			{
				ApproximationDistances[i] = ApproximationDistances[i - 1] + (Approximation[i] - Approximation[i - 1]).magnitude;
			}
			Length = ApproximationDistances[ApproximationDistances.Length - 1];
		}
		return Length;
	}

	public void _UpdateTangents()
	{
		if (IsValidSegment)
		{
			mStepSize = 1f / (float)Spline.Granularity;
			int num = ApproximationT.Length;
			for (int i = 0; i < num; i++)
			{
				ref Vector3 reference = ref ApproximationT[i];
				reference = GetTangent((float)i * mStepSize, ref Approximation[i]);
			}
			if (SmoothEdgeTangent && !IsFirstSegment)
			{
				ref Vector3 reference2 = ref PreviousSegment.ApproximationT[Spline.Granularity];
				reference2 = Vector3.Lerp(PreviousSegment.ApproximationT[Spline.Granularity - 1], ApproximationT[0], 0.5f);
				ref Vector3 reference3 = ref ApproximationT[0];
				reference3 = PreviousSegment.ApproximationT[Spline.Granularity];
			}
			if (IsLastSegment && (bool)NextSegment && Spline[0].SmoothEdgeTangent)
			{
				ref Vector3 reference4 = ref ApproximationT[Spline.Granularity];
				reference4 = Vector3.Lerp(ApproximationT[Spline.Granularity - 1], Spline[0].ApproximationT[0], 0.5f);
				ref Vector3 reference5 = ref Spline[0].ApproximationT[0];
				reference5 = ApproximationT[Spline.Granularity];
			}
		}
	}

	private Vector3 ParallelTransportFrame(ref Vector3 lastUp, ref Vector3 T1, ref Vector3 T2, float swirlangle)
	{
		Vector3 vector = Vector3.Cross(T1, T2);
		float num = Mathf.Atan2(vector.magnitude, Vector3.Dot(T1, T2));
		Quaternion quaternion = Quaternion.AngleAxis(57.29578f * num, vector.normalized);
		if (Spline.Swirl != 0)
		{
			return quaternion * Quaternion.AngleAxis(swirlangle, T2) * lastUp;
		}
		return quaternion * lastUp;
	}

	public Vector3 _UpdateOrientation(Vector3 lastUpVector)
	{
		float swirlangle = 0f;
		switch (Spline.Swirl)
		{
		case CurvyOrientationSwirl.Segment:
			swirlangle = Spline.SwirlTurns * 360f / (float)Spline.Granularity + swirl;
			break;
		case CurvyOrientationSwirl.Spline:
			swirlangle = Spline.SwirlTurns * 360f / (float)Spline.Count / (float)Spline.Granularity + swirl;
			break;
		}
		ApproximationUp[0] = lastUpVector;
		if (Spline.Orientation == CurvyOrientation.Tangent)
		{
			int num = Approximation.Length;
			for (int i = 1; i < num; i++)
			{
				lastUpVector = ParallelTransportFrame(ref lastUpVector, ref ApproximationT[i - 1], ref ApproximationT[i], swirlangle);
				ApproximationUp[i] = lastUpVector;
			}
		}
		else if (Spline.Orientation == CurvyOrientation.ControlPoints)
		{
			int num2 = Approximation.Length;
			for (int i = 0; i < num2; i++)
			{
				ref Vector3 reference = ref ApproximationUp[i];
				reference = Vector3.Lerp(Transform.up, NextTransform.up, (float)i * mStepSize);
			}
			lastUpVector = ApproximationUp[Spline.Granularity];
		}
		return lastUpVector;
	}

	public Vector3 _SmoothOrientation(Vector3 lastUpVector, ref float angleaccu, float angle)
	{
		ApproximationUp[0] = lastUpVector;
		for (int i = 1; i < ApproximationUp.Length; i++)
		{
			ref Vector3 reference = ref ApproximationUp[i];
			reference = Quaternion.AngleAxis(angleaccu, ApproximationT[i]) * ApproximationUp[i];
			angleaccu += angle;
		}
		return ApproximationUp[ApproximationUp.Length - 1];
	}

	public bool _PositionHasChanged()
	{
		if (!Transform)
		{
			return true;
		}
		bool result = Transform.position != mPosition;
		mPosition = Transform.position;
		return result;
	}

	public bool _RotationHasChanged()
	{
		if (!Transform)
		{
			return true;
		}
		bool result = Transform.rotation != mRotation;
		mRotation = Transform.rotation;
		return result;
	}

	public Vector2 GetBounds()
	{
		Vector2 vector = new Vector2(0f, 0f);
		foreach (CurvySpline2 spl in spls)
		{
			if ((bool)spl)
			{
				Bounds bounds = spl.GetBounds(local: true);
				float y = vector.y;
				Vector3 extents = bounds.extents;
				float y2 = extents.y;
				Vector3 center = bounds.center;
				vector.y = Mathf.Max(y, y2 - center.y);
				float x = vector.x;
				Vector3 size = bounds.size;
				vector.x = Math.Max(x, size.x);
			}
		}
		if (vector == Vector2.zero)
		{
			vector = new Vector2(25f, -1f);
		}
		if (NextControlPoint != null || PreviousControlPoint != null)
		{
			float a = Mathf.Abs(swirl);
			if (NextControlPoint != null)
			{
				a = Mathf.Max(a, Mathf.Abs(NextControlPoint.swirl));
			}
			if (PreviousControlPoint != null)
			{
				a = Mathf.Max(a, Mathf.Abs(PreviousControlPoint.swirl));
			}
			vector = Quaternion.Euler(0f, 0f, a) * vector;
		}
		vector.x *= scale;
		return vector;
	}

	public void OnDestroy()
	{
		foreach (SplinePathMeshBuilder sb in sbs)
		{
			UnityEngine.Object.Destroy(sb.gameObject);
		}
	}
}
