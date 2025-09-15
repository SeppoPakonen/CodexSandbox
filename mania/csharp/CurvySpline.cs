using System.Collections.Generic;
using UnityEngine;

public class CurvySpline : bs
{
	public delegate void OnRefreshEvent(CurvySpline sender);

	public const string Version = "1.5";

	public CurvyInterpolation Interpolation = CurvyInterpolation.CatmulRom;

	public bool Closed;

	public bool AutoEndTangents = true;

	public CurvyInitialUpDefinition InitialUpVector = CurvyInitialUpDefinition.MinAxis;

	public CurvyOrientation Orientation = CurvyOrientation.Tangent;

	public CurvyOrientationSwirl Swirl;

	public float SwirlTurns;

	public int Granularity = 20;

	public bool ShowApproximation;

	public bool ShowOrientation = true;

	public bool ShowTangents;

	public bool AutoRefresh = true;

	public bool AutoRefreshLength = true;

	public bool AutoRefreshOrientation = true;

	public int UserValueSize;

	public bool ShowUserValues;

	public static Color GizmoColor = Color.red;

	public static Color GizmoSelectionColor = Color.white;

	public static float GizmoControlPointSize = 0.15f;

	public static float GizmoOrientationLength = 1f;

	public float Tension;

	public float Continuity;

	public float Bias;

	public bool ShowGizmos = true;

	private Transform mTransform;

	private List<CurvySplineSegment> mControlPoints = new List<CurvySplineSegment>();

	private List<CurvySplineSegment> mSegments = new List<CurvySplineSegment>();

	private float mStepSize;

	public bool rotateTexture;

	public int Count => mSegments.Count;

	public int ControlPointCount => mControlPoints.Count;

	public CurvySplineSegment this[int idx] => (idx <= -1 || idx >= mSegments.Count) ? null : mSegments[idx];

	public List<CurvySplineSegment> Segments => mSegments;

	public List<CurvySplineSegment> ControlPoints => mControlPoints;

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

	public bool IsInitialized
	{
		get;
		private set;
	}

	public float Length
	{
		get;
		private set;
	}

	private int ApproximationPointCount => Count * (Granularity + 1);

	public event OnRefreshEvent OnRefresh;

	public static CurvySpline Create()
	{
		return new GameObject("Curvy Spline", typeof(CurvySpline)).GetComponent<CurvySpline>();
	}

	public void Destroy()
	{
		if (Application.isPlaying)
		{
			Object.Destroy(base.gameObject);
		}
		else
		{
			Object.DestroyImmediate(base.gameObject);
		}
	}

	private void OnEnable()
	{
		if (!Application.isPlaying)
		{
			Refresh(AutoRefreshLength, AutoRefreshOrientation, skipIfInitialized: false);
		}
	}

	private void OnDisable()
	{
		IsInitialized = false;
	}

	private void Start()
	{
		Refresh(AutoRefreshLength, AutoRefreshOrientation, skipIfInitialized: true);
	}

	public void Update()
	{
		if (Application.isPlaying && !IsInitialized)
		{
			Refresh(AutoRefreshLength, AutoRefreshOrientation, skipIfInitialized: false);
		}
		if (Application.isPlaying && !AutoRefresh)
		{
			return;
		}
		bool flag = false;
		for (int i = 0; i < mControlPoints.Count; i++)
		{
			if (mControlPoints[i] != null && (mControlPoints[i]._PositionHasChanged() || mControlPoints[i]._RotationHasChanged()))
			{
				flag = true;
				break;
			}
		}
		if (flag)
		{
			if (!Application.isPlaying)
			{
				Refresh(refreshLength: true, refreshOrientation: true, skipIfInitialized: false);
			}
			else
			{
				Refresh(AutoRefreshLength, AutoRefreshOrientation, skipIfInitialized: false);
			}
		}
	}

	public Vector3 Interpolate(float tf)
	{
		float localF;
		CurvySplineSegment curvySplineSegment = TFToSegment(tf, out localF);
		return (!curvySplineSegment) ? Vector3.zero : curvySplineSegment.Interpolate(localF);
	}

	public Vector3 InterpolateFast(float tf)
	{
		float localF;
		CurvySplineSegment curvySplineSegment = TFToSegment(tf, out localF);
		return (!curvySplineSegment) ? Vector3.zero : curvySplineSegment.InterpolateFast(localF);
	}

	public Vector3 InterpolateUserValue(float tf, int index)
	{
		float localF;
		CurvySplineSegment curvySplineSegment = TFToSegment(tf, out localF);
		return curvySplineSegment.InterpolateUserValue(localF, index);
	}

	public Vector3 InterpolateScale(float tf)
	{
		float localF;
		CurvySplineSegment curvySplineSegment = TFToSegment(tf, out localF);
		return curvySplineSegment.InterpolateScale(localF);
	}

	public Vector3 GetOrientationUpFast(float tf)
	{
		float localF;
		CurvySplineSegment curvySplineSegment = TFToSegment(tf, out localF);
		return (!curvySplineSegment) ? Vector3.zero : curvySplineSegment.GetOrientationUpFast(localF);
	}

	public Quaternion GetOrientationFast(float tf)
	{
		float localF;
		CurvySplineSegment curvySplineSegment = TFToSegment(tf, out localF);
		return (!curvySplineSegment) ? Quaternion.identity : curvySplineSegment.GetOrientationFast(localF);
	}

	public Vector3 GetTangent(float tf)
	{
		float localF;
		CurvySplineSegment curvySplineSegment = TFToSegment(tf, out localF);
		return (!curvySplineSegment) ? Vector3.zero : curvySplineSegment.GetTangent(localF);
	}

	public Vector3 GetTangent(float tf, Vector3 position)
	{
		float localF;
		CurvySplineSegment curvySplineSegment = TFToSegment(tf, out localF);
		return (!curvySplineSegment) ? Vector3.zero : curvySplineSegment.GetTangent(localF, ref position);
	}

	public Vector3 GetTangentFast(float tf)
	{
		float localF;
		CurvySplineSegment curvySplineSegment = TFToSegment(tf, out localF);
		return (!curvySplineSegment) ? Vector3.zero : curvySplineSegment.GetTangentFast(localF);
	}

	public Vector3 Move(ref float tf, ref int direction, float fDistance, CurvyClamping clamping)
	{
		tf += fDistance * (float)direction;
		ClampTF(ref tf, ref direction, clamping);
		return Interpolate(tf);
	}

	public Vector3 MoveFast(ref float tf, ref int direction, float fDistance, CurvyClamping clamping)
	{
		tf += fDistance * (float)direction;
		ClampTF(ref tf, ref direction, clamping);
		return InterpolateFast(tf);
	}

	public Vector3 MoveBy(ref float tf, ref int direction, float distance, CurvyClamping clamping)
	{
		return MoveBy(ref tf, ref direction, distance, clamping, 0.002f);
	}

	public Vector3 MoveBy(ref float tf, ref int direction, float distance, CurvyClamping clamping, float stepSize)
	{
		stepSize = Mathf.Max(0.0001f, stepSize);
		Vector3 vector = Interpolate(tf);
		if (Mathf.Approximately(distance, 0f))
		{
			return vector;
		}
		float num = tf + stepSize * (float)direction;
		if (num < 0f)
		{
			switch (clamping)
			{
			case CurvyClamping.Clamp:
			{
				float magnitude = (Interpolate(0f) - vector).magnitude;
				tf = 0f;
				if (magnitude < distance)
				{
					return Interpolate(0f);
				}
				break;
			}
			case CurvyClamping.PingPong:
			{
				float magnitude = (Interpolate(0f) - vector).magnitude;
				tf = 0f;
				num = 0f;
				if (magnitude < distance)
				{
					direction *= -1;
					return MoveBy(ref tf, ref direction, distance - magnitude, clamping);
				}
				break;
			}
			case CurvyClamping.Loop:
			{
				float magnitude = (Interpolate(0f) - vector).magnitude;
				if (magnitude >= distance)
				{
					num = 0f;
					break;
				}
				tf = 1f;
				return MoveBy(ref tf, ref direction, distance - magnitude, clamping);
			}
			}
		}
		else if (num > 1f)
		{
			switch (clamping)
			{
			case CurvyClamping.Clamp:
			{
				float magnitude2 = (Interpolate(1f) - vector).magnitude;
				tf = 1f;
				if (magnitude2 < distance)
				{
					return Interpolate(1f);
				}
				break;
			}
			case CurvyClamping.PingPong:
			{
				float magnitude2 = (Interpolate(1f) - vector).magnitude;
				num = 1f;
				if (magnitude2 < distance)
				{
					direction *= -1;
					tf = 1f;
					return MoveBy(ref tf, ref direction, distance - magnitude2, clamping);
				}
				break;
			}
			case CurvyClamping.Loop:
			{
				float magnitude2 = (Interpolate(1f) - vector).magnitude;
				if (magnitude2 >= distance)
				{
					num = 1f;
					break;
				}
				tf = 0f;
				return MoveBy(ref tf, ref direction, distance - magnitude2, clamping);
			}
			}
		}
		Vector3 a = Interpolate(num);
		float magnitude3 = (a - vector).magnitude;
		if (magnitude3 != 0f)
		{
			tf += 1f / magnitude3 * stepSize * distance * (float)direction;
		}
		return Interpolate(tf);
	}

	public Vector3 MoveByFast(ref float tf, ref int direction, float distance, CurvyClamping clamping)
	{
		return MoveByFast(ref tf, ref direction, distance, clamping, 0.002f);
	}

	public Vector3 MoveByFast(ref float tf, ref int direction, float distance, CurvyClamping clamping, float stepSize)
	{
		stepSize = Mathf.Max(0.0001f, stepSize);
		Vector3 vector = InterpolateFast(tf);
		if (Mathf.Approximately(distance, 0f))
		{
			return vector;
		}
		float num = tf + stepSize * (float)direction;
		if (num < 0f)
		{
			switch (clamping)
			{
			case CurvyClamping.Clamp:
				tf = 0f;
				return InterpolateFast(0f);
			case CurvyClamping.PingPong:
			{
				float magnitude = (Interpolate(0f) - vector).magnitude;
				direction *= -1;
				tf = 0f;
				return MoveByFast(ref tf, ref direction, distance - magnitude, clamping);
			}
			case CurvyClamping.Loop:
			{
				float magnitude = (Interpolate(0f) - vector).magnitude;
				tf = 1f;
				return MoveByFast(ref tf, ref direction, distance - magnitude, clamping);
			}
			}
		}
		else if (num > 1f)
		{
			switch (clamping)
			{
			case CurvyClamping.Clamp:
				tf = 1f;
				return InterpolateFast(1f);
			case CurvyClamping.PingPong:
			{
				float magnitude2 = (InterpolateFast(1f) - vector).magnitude;
				direction *= -1;
				tf = 1f;
				return MoveByFast(ref tf, ref direction, distance - magnitude2, clamping);
			}
			case CurvyClamping.Loop:
			{
				float magnitude2 = (InterpolateFast(1f) - vector).magnitude;
				tf = 0f;
				return MoveByFast(ref tf, ref direction, distance - magnitude2, clamping);
			}
			}
		}
		Vector3 a = InterpolateFast(num);
		float magnitude3 = (a - vector).magnitude;
		if (magnitude3 != 0f)
		{
			tf += 1f / magnitude3 * stepSize * distance * (float)direction;
		}
		return InterpolateFast(tf);
	}

	public Vector3 MoveByAngle(ref float tf, ref int direction, float angle, CurvyClamping clamping, float stepSize)
	{
		if (clamping == CurvyClamping.PingPong)
		{
			Debug.LogError("CurvySpline.MoveByAngle: PingPong clamping isn't supported!");
			return Vector3.zero;
		}
		stepSize = Mathf.Max(0.0001f, stepSize);
		Vector3 vector = Interpolate(tf);
		Vector3 tangent = GetTangent(tf, vector);
		Vector3 vector2 = Vector3.zero;
		int num = 10000;
		while (num-- > 0)
		{
			tf += stepSize * (float)direction;
			if (tf > 1f)
			{
				if (clamping != CurvyClamping.Loop)
				{
					tf = 1f;
					return Interpolate(1f);
				}
				tf -= 1f;
			}
			else if (tf < 0f)
			{
				if (clamping != CurvyClamping.Loop)
				{
					tf = 0f;
					return Interpolate(0f);
				}
				tf += 1f;
			}
			vector2 = Interpolate(tf);
			Vector3 to = vector2 - vector;
			if (Vector3.Angle(tangent, to) >= angle)
			{
				return vector2;
			}
		}
		return vector2;
	}

	public Vector3 MoveByAngleFast(ref float tf, ref int direction, float angle, CurvyClamping clamping, float stepSize)
	{
		if (clamping == CurvyClamping.PingPong)
		{
			Debug.LogError("CurvySpline.MoveByAngle: PingPong clamping isn't supported!");
			return Vector3.zero;
		}
		stepSize = Mathf.Max(0.0001f, stepSize);
		Vector3 b = InterpolateFast(tf);
		Vector3 tangentFast = GetTangentFast(tf);
		Vector3 vector = Vector3.zero;
		int num = 10000;
		while (num-- > 0)
		{
			tf += stepSize * (float)direction;
			if (tf > 1f)
			{
				if (clamping != CurvyClamping.Loop)
				{
					tf = 1f;
					return InterpolateFast(1f);
				}
				tf -= 1f;
			}
			else if (tf < 0f)
			{
				if (clamping != CurvyClamping.Loop)
				{
					tf = 0f;
					return InterpolateFast(0f);
				}
				tf += 1f;
			}
			vector = InterpolateFast(tf);
			Vector3 to = vector - b;
			if (Vector3.Angle(tangentFast, to) >= angle)
			{
				return vector;
			}
		}
		return vector;
	}

	public float TFToDistance(float tf)
	{
		float localF;
		CurvySplineSegment curvySplineSegment = TFToSegment(tf, out localF);
		return curvySplineSegment.Distance + curvySplineSegment.LocalFToDistance(localF);
	}

	public CurvySplineSegment TFToSegment(float tf)
	{
		float localF;
		return TFToSegment(tf, out localF);
	}

	public CurvySplineSegment TFToSegment(float tf, out float localF)
	{
		tf = Mathf.Clamp01(tf);
		localF = 0f;
		if (Count == 0)
		{
			return null;
		}
		float num = tf * (float)Count;
		int num2 = (int)num;
		localF = num - (float)num2;
		if (num2 == Count)
		{
			num2--;
			localF = 1f;
		}
		return this[num2];
	}

	public Vector3 InterpolateByDistance(float distance)
	{
		return Interpolate(DistanceToTF(distance));
	}

	public Vector3 InterpolateByDistanceFast(float distance)
	{
		return InterpolateFast(DistanceToTF(distance));
	}

	public Vector3 GetTangentByDistance(float distance)
	{
		return GetTangent(DistanceToTF(distance));
	}

	public Vector3 GetTangentByDistanceFast(float distance)
	{
		return GetTangentFast(DistanceToTF(distance));
	}

	public float DistanceToTF(float distance)
	{
		float localDistance;
		CurvySplineSegment curvySplineSegment = DistanceToSegment(distance, out localDistance);
		return (!curvySplineSegment) ? 0f : SegmentToTF(curvySplineSegment, curvySplineSegment.DistanceToLocalF(localDistance));
	}

	public CurvySplineSegment DistanceToSegment(float distance)
	{
		float localDistance;
		return DistanceToSegment(distance, out localDistance);
	}

	public CurvySplineSegment DistanceToSegment(float distance, out float localDistance)
	{
		distance = Mathf.Clamp(distance, 0f, Length);
		localDistance = 0f;
		CurvySplineSegment curvySplineSegment = mSegments[0];
		while ((bool)curvySplineSegment && curvySplineSegment.Distance + curvySplineSegment.Length < distance)
		{
			curvySplineSegment = NextSegment(curvySplineSegment);
		}
		if (curvySplineSegment == null)
		{
			curvySplineSegment = this[Count - 1];
		}
		localDistance = distance - curvySplineSegment.Distance;
		return curvySplineSegment;
	}

	public CurvySplineSegment Add()
	{
		return Add(null, refresh: true);
	}

	public CurvySplineSegment Add(CurvySplineSegment insertAfter)
	{
		return Add(insertAfter, refresh: true);
	}

	public CurvySplineSegment[] Add(params Vector3[] controlPoints)
	{
		CurvySplineSegment[] array = new CurvySplineSegment[controlPoints.Length];
		for (int i = 0; i < controlPoints.Length; i++)
		{
			array[i] = Add(null, refresh: false);
			array[i].Position = controlPoints[i];
		}
		Refresh();
		return array;
	}

	public CurvySplineSegment Add(CurvySplineSegment insertAfter, bool refresh)
	{
		GameObject gameObject = new GameObject("NewCP", typeof(CurvySplineSegment));
		gameObject.transform.parent = base.transform;
		CurvySplineSegment component = gameObject.GetComponent<CurvySplineSegment>();
		int index = mControlPoints.Count;
		if ((bool)insertAfter)
		{
			if (insertAfter.IsValidSegment)
			{
				gameObject.transform.position = insertAfter.Interpolate(0.5f);
			}
			else if ((bool)insertAfter.NextTransform)
			{
				gameObject.transform.position = Vector3.Lerp(insertAfter.NextTransform.position, insertAfter.Transform.position, 0.5f);
			}
			index = insertAfter.ControlPointIndex + 1;
		}
		mControlPoints.Insert(index, component);
		_nameControlPoints();
		if (refresh)
		{
			Refresh();
		}
		return component;
	}

	public CurvySplineSegment Add(bool refresh, CurvySplineSegment insertBefore)
	{
		GameObject gameObject = new GameObject("NewCP", typeof(CurvySplineSegment));
		gameObject.transform.parent = base.transform;
		CurvySplineSegment component = gameObject.GetComponent<CurvySplineSegment>();
		int index = 0;
		if ((bool)insertBefore)
		{
			if ((bool)insertBefore.PreviousSegment)
			{
				gameObject.transform.position = insertBefore.PreviousSegment.Interpolate(0.5f);
			}
			else if ((bool)insertBefore.PreviousTransform)
			{
				gameObject.transform.position = Vector3.Lerp(insertBefore.PreviousTransform.position, insertBefore.Transform.position, 0.5f);
			}
			index = Mathf.Max(0, insertBefore.ControlPointIndex);
		}
		mControlPoints.Insert(index, component);
		_nameControlPoints();
		if (refresh)
		{
			Refresh();
		}
		return component;
	}

	public Bounds GetBounds(bool local)
	{
		Vector3[] approximation = GetApproximation(local);
		Vector3 vector = new Vector3(float.MaxValue, float.MaxValue, float.MaxValue);
		Vector3 a = new Vector3(float.MinValue, float.MinValue, float.MinValue);
		for (int i = 0; i < approximation.Length; i++)
		{
			vector.x = Mathf.Min(vector.x, approximation[i].x);
			vector.y = Mathf.Min(vector.y, approximation[i].y);
			vector.z = Mathf.Min(vector.z, approximation[i].z);
			a.x = Mathf.Max(a.x, approximation[i].x);
			a.y = Mathf.Max(a.y, approximation[i].y);
			a.z = Mathf.Max(a.z, approximation[i].z);
		}
		return new Bounds(vector + (a - vector) / 2f, a - vector);
	}

	public void Clear()
	{
		foreach (CurvySplineSegment mControlPoint in mControlPoints)
		{
			if (Application.isPlaying)
			{
				Object.Destroy(mControlPoint.gameObject);
			}
			else
			{
				Object.DestroyImmediate(mControlPoint.gameObject);
			}
		}
		mControlPoints.Clear();
		Refresh();
	}

	public void Delete(CurvySplineSegment controlPoint)
	{
		Delete(controlPoint, refreshSpline: true);
	}

	public void Delete(CurvySplineSegment controlPoint, bool refreshSpline)
	{
		if (Application.isPlaying)
		{
			Object.Destroy(controlPoint.gameObject);
		}
		else
		{
			Object.DestroyImmediate(controlPoint.gameObject);
		}
		mControlPoints.Remove(controlPoint);
		_nameControlPoints();
		Refresh();
	}

	public Vector3[] GetApproximation()
	{
		return GetApproximation(local: false);
	}

	public Vector3[] GetApproximation(bool local)
	{
		Vector3[] array = new Vector3[Count * Granularity + 1];
		int num = 0;
		for (int i = 0; i < Count; i++)
		{
			this[i].Approximation.CopyTo(array, num);
			num += Mathf.Max(0, this[i].Approximation.Length - 1);
		}
		if (local)
		{
			Matrix4x4 worldToLocalMatrix = Transform.worldToLocalMatrix;
			for (int j = 0; j < array.Length; j++)
			{
				ref Vector3 reference = ref array[j];
				reference = worldToLocalMatrix.MultiplyPoint3x4(array[j]);
			}
		}
		return array;
	}

	public Vector3[] GetApproximationT()
	{
		Vector3[] array = new Vector3[Count * Granularity + 1];
		int num = 0;
		for (int i = 0; i < Count; i++)
		{
			this[i].ApproximationT.CopyTo(array, num);
			num += Mathf.Max(0, this[i].ApproximationT.Length - 1);
		}
		return array;
	}

	public Vector3[] GetApproximationUpVectors()
	{
		Vector3[] array = new Vector3[Count * Granularity + 1];
		int num = 0;
		for (int i = 0; i < Count; i++)
		{
			this[i].ApproximationUp.CopyTo(array, num);
			num += Mathf.Max(0, this[i].ApproximationUp.Length - 1);
		}
		return array;
	}

	public float GetNearestPointTF(Vector3 p)
	{
		if (Count == 0)
		{
			return -1f;
		}
		float[] array = new float[ApproximationPointCount];
		CurvySplineSegment curvySplineSegment = mSegments[0];
		int num = 0;
		int num2 = 0;
		float num3 = float.MaxValue;
		int num4 = 0;
		CurvySplineSegment curvySplineSegment2 = null;
		while ((bool)curvySplineSegment && num < array.Length)
		{
			if (curvySplineSegment.Approximation.Length == 0)
			{
				return -1f;
			}
			array[num] = (curvySplineSegment.Approximation[num2] - p).sqrMagnitude;
			if (array[num] < num3)
			{
				num3 = array[num];
				num4 = num2;
				curvySplineSegment2 = curvySplineSegment;
			}
			num++;
			num2++;
			if (num2 == curvySplineSegment.Approximation.Length)
			{
				curvySplineSegment = NextSegment(curvySplineSegment);
				num2 = 0;
			}
		}
		CurvySplineSegment[] array2 = new CurvySplineSegment[3];
		int[] array3 = new int[3];
		array2[1] = curvySplineSegment2;
		array3[1] = num4;
		if (!getPreviousApproximationPoint(curvySplineSegment2, num4, out array2[0], out array3[0]))
		{
			array2[0] = array2[1];
			array3[0] = array3[1];
		}
		if (!getNextApproximationPoint(curvySplineSegment2, num4, out array2[2], out array3[2]))
		{
			array2[2] = array2[1];
			array3[2] = array3[1];
		}
		float[] array4 = new float[2];
		float[] array5 = new float[2]
		{
			LinePointDistanceSqr(array2[0].Approximation[array3[0]], array2[1].Approximation[array3[1]], p, out array4[0]),
			LinePointDistanceSqr(array2[1].Approximation[array3[1]], array2[2].Approximation[array3[2]], p, out array4[1])
		};
		if (array5[0] < array5[1])
		{
			return array2[0].LocalFToTF(array2[0]._getApproximationLocalF(array3[0]) + array4[0] * mStepSize);
		}
		return array2[1].LocalFToTF(array2[1]._getApproximationLocalF(array3[1]) + array4[1] * mStepSize);
	}

	public Vector3 GetExtrusionPoint(Vector3 P, Vector3 Tangent, Vector3 Up, float radius, float angle)
	{
		Quaternion rotation = Quaternion.AngleAxis(angle, Tangent);
		return P + rotation * Up * radius;
	}

	public CurvySplineSegment NextControlPoint(CurvySplineSegment controlpoint)
	{
		if (mControlPoints.Count == 0)
		{
			return null;
		}
		int num = controlpoint.ControlPointIndex + 1;
		if (num >= mControlPoints.Count)
		{
			return (!Closed) ? null : mControlPoints[0];
		}
		return mControlPoints[num];
	}

	public CurvySplineSegment PreviousControlPoint(CurvySplineSegment controlpoint)
	{
		if (mControlPoints.Count == 0)
		{
			return null;
		}
		int num = controlpoint.ControlPointIndex - 1;
		if (num < 0)
		{
			return (!Closed) ? null : mControlPoints[mControlPoints.Count - 1];
		}
		return mControlPoints[num];
	}

	public CurvySplineSegment NextSegment(CurvySplineSegment segment)
	{
		if (mSegments.Count == 0)
		{
			return null;
		}
		int num = segment.SegmentIndex + 1;
		if (num >= mSegments.Count)
		{
			return (!Closed) ? null : mSegments[0];
		}
		return mSegments[num];
	}

	public CurvySplineSegment PreviousSegment(CurvySplineSegment segment)
	{
		if (mSegments.Count == 0)
		{
			return null;
		}
		int num = segment.SegmentIndex - 1;
		if (num < 0)
		{
			return (!Closed) ? null : mSegments[mSegments.Count - 1];
		}
		return mSegments[num];
	}

	public Transform NextTransform(CurvySplineSegment controlpoint)
	{
		CurvySplineSegment curvySplineSegment = NextControlPoint(controlpoint);
		if ((bool)curvySplineSegment)
		{
			return curvySplineSegment.Transform;
		}
		if (AutoEndTangents && Interpolation != 0)
		{
			return controlpoint.Transform;
		}
		return null;
	}

	public Transform PreviousTransform(CurvySplineSegment controlpoint)
	{
		CurvySplineSegment curvySplineSegment = PreviousControlPoint(controlpoint);
		if ((bool)curvySplineSegment)
		{
			return curvySplineSegment.Transform;
		}
		if (AutoEndTangents && Interpolation != 0)
		{
			return controlpoint.Transform;
		}
		return null;
	}

	public void Refresh()
	{
		Refresh(AutoRefreshLength, AutoRefreshOrientation, skipIfInitialized: false);
	}

	public virtual void Refresh(bool refreshLength, bool refreshOrientation, bool skipIfInitialized)
	{
		if (skipIfInitialized && IsInitialized)
		{
			return;
		}
		mStepSize = 1f / (float)Granularity;
		removeEmptyControlPoints();
		if (ControlPointCount == 0)
		{
			ReloadControlPoints();
		}
		mControlPoints.Sort((CurvySplineSegment a, CurvySplineSegment b) => string.Compare(a.name, b.name));
		mSegments.Clear();
		for (int i = 0; i < mControlPoints.Count; i++)
		{
			mControlPoints[i]._InitializeControlPoint();
		}
		for (int j = 0; j < mControlPoints.Count; j++)
		{
			if (mControlPoints[j].IsValidSegment)
			{
				mSegments.Add(mControlPoints[j]);
				mControlPoints[j]._UpdateApproximation();
			}
		}
		if (refreshOrientation)
		{
			doRefreshTangents();
			doRefreshOrientation();
		}
		if (refreshLength)
		{
			doRefreshLength();
		}
		if (this.OnRefresh != null)
		{
			this.OnRefresh(this);
		}
		IsInitialized = true;
	}

	public void ReloadControlPoints()
	{
		mControlPoints = new List<CurvySplineSegment>(GetComponentsInChildren<CurvySplineSegment>(includeInactive: false));
	}

	public float SegmentToTF(CurvySplineSegment segment)
	{
		return SegmentToTF(segment, 0f);
	}

	public float SegmentToTF(CurvySplineSegment segment, float localF)
	{
		return (float)segment.SegmentIndex / (float)Count + 1f / (float)Count * localF;
	}

	public Vector3 CatmulRom(Vector3 T0, Vector3 P0, Vector3 P1, Vector3 T1, float f)
	{
		double num = -0.5;
		double num2 = 1.5;
		double num3 = -1.5;
		double num4 = 0.5;
		double num5 = -2.5;
		double num6 = 2.0;
		double num7 = -0.5;
		double num8 = -0.5;
		double num9 = 0.5;
		double num10 = num * (double)T0.x + num2 * (double)P0.x + num3 * (double)P1.x + num4 * (double)T1.x;
		double num11 = (double)T0.x + num5 * (double)P0.x + num6 * (double)P1.x + num7 * (double)T1.x;
		double num12 = num8 * (double)T0.x + num9 * (double)P1.x;
		double num13 = P0.x;
		double num14 = num * (double)T0.y + num2 * (double)P0.y + num3 * (double)P1.y + num4 * (double)T1.y;
		double num15 = (double)T0.y + num5 * (double)P0.y + num6 * (double)P1.y + num7 * (double)T1.y;
		double num16 = num8 * (double)T0.y + num9 * (double)P1.y;
		double num17 = P0.y;
		double num18 = num * (double)T0.z + num2 * (double)P0.z + num3 * (double)P1.z + num4 * (double)T1.z;
		double num19 = (double)T0.z + num5 * (double)P0.z + num6 * (double)P1.z + num7 * (double)T1.z;
		double num20 = num8 * (double)T0.z + num9 * (double)P1.z;
		double num21 = P0.z;
		float x = (float)(((num10 * (double)f + num11) * (double)f + num12) * (double)f + num13);
		float y = (float)(((num14 * (double)f + num15) * (double)f + num16) * (double)f + num17);
		float z = (float)(((num18 * (double)f + num19) * (double)f + num20) * (double)f + num21);
		return new Vector3(x, y, z);
	}

	public Vector3 TCB(Vector3 T0, Vector3 P0, Vector3 P1, Vector3 T1, float f, float FT0, float FC0, float FB0, float FT1, float FC1, float FB1)
	{
		double num = (1f - FT0) * (1f + FC0) * (1f + FB0);
		double num2 = (1f - FT0) * (1f - FC0) * (1f - FB0);
		double num3 = (1f - FT1) * (1f - FC1) * (1f + FB1);
		double num4 = (1f - FT1) * (1f + FC1) * (1f - FB1);
		double num5 = 2.0;
		double num6 = (0.0 - num) / num5;
		double num7 = (4.0 + num - num2 - num3) / num5;
		double num8 = (-4.0 + num2 + num3 - num4) / num5;
		double num9 = num4 / num5;
		double num10 = 2.0 * num / num5;
		double num11 = (-6.0 - 2.0 * num + 2.0 * num2 + num3) / num5;
		double num12 = (6.0 - 2.0 * num2 - num3 + num4) / num5;
		double num13 = (0.0 - num4) / num5;
		double num14 = (0.0 - num) / num5;
		double num15 = (num - num2) / num5;
		double num16 = num2 / num5;
		double num17 = 2.0 / num5;
		double num18 = num6 * (double)T0.x + num7 * (double)P0.x + num8 * (double)P1.x + num9 * (double)T1.x;
		double num19 = num10 * (double)T0.x + num11 * (double)P0.x + num12 * (double)P1.x + num13 * (double)T1.x;
		double num20 = num14 * (double)T0.x + num15 * (double)P0.x + num16 * (double)P1.x;
		double num21 = num17 * (double)P0.x;
		double num22 = num6 * (double)T0.y + num7 * (double)P0.y + num8 * (double)P1.y + num9 * (double)T1.y;
		double num23 = num10 * (double)T0.y + num11 * (double)P0.y + num12 * (double)P1.y + num13 * (double)T1.y;
		double num24 = num14 * (double)T0.y + num15 * (double)P0.y + num16 * (double)P1.y;
		double num25 = num17 * (double)P0.y;
		double num26 = num6 * (double)T0.z + num7 * (double)P0.z + num8 * (double)P1.z + num9 * (double)T1.z;
		double num27 = num10 * (double)T0.z + num11 * (double)P0.z + num12 * (double)P1.z + num13 * (double)T1.z;
		double num28 = num14 * (double)T0.z + num15 * (double)P0.z + num16 * (double)P1.z;
		double num29 = num17 * (double)P0.z;
		float x = (float)(((num18 * (double)f + num19) * (double)f + num20) * (double)f + num21);
		float y = (float)(((num22 * (double)f + num23) * (double)f + num24) * (double)f + num25);
		float z = (float)(((num26 * (double)f + num27) * (double)f + num28) * (double)f + num29);
		return new Vector3(x, y, z);
	}

	private void doRefreshLength()
	{
		Length = 0f;
		for (int i = 0; i < Count; i++)
		{
			Length += this[i]._UpdateLength();
		}
	}

	private void doRefreshTangents()
	{
		for (int i = 0; i < Count; i++)
		{
			this[i]._UpdateTangents();
		}
	}

	public void doRefreshOrientation()
	{
		if (Count <= 0)
		{
			return;
		}
		Vector3 tangent = (InitialUpVector != 0) ? minAxis(this[0].GetTangent(0f)) : this[0].Transform.up;
		Vector3.OrthoNormalize(ref this[0].ApproximationT[0], ref tangent);
		for (int i = 0; i < Count; i++)
		{
			tangent = this[i]._UpdateOrientation(tangent);
		}
		if (Closed && Orientation == CurvyOrientation.Tangent)
		{
			float num = AngleSigned(this[Count - 1].ApproximationUp[Granularity - 1], this[0].ApproximationUp[0], this[0].ApproximationT[0]) / (float)(Count * Granularity);
			float angleaccu = num;
			tangent = this[0].ApproximationUp[0];
			for (int j = 0; j < Count; j++)
			{
				tangent = this[j]._SmoothOrientation(tangent, ref angleaccu, num);
			}
			ref Vector3 reference = ref this[Count - 1].ApproximationUp[Granularity];
			reference = this[0].ApproximationUp[0];
		}
	}

	public float AngleSigned(Vector3 v1, Vector3 v2, Vector3 n)
	{
		return Mathf.Atan2(Vector3.Dot(n, Vector3.Cross(v1, v2)), Vector3.Dot(v1, v2)) * 57.29578f;
	}

	public void _nameControlPoints()
	{
		removeEmptyControlPoints();
		for (int i = 0; i < mControlPoints.Count; i++)
		{
			mControlPoints[i].name = "CP" + $"{i:000}";
		}
	}

	private void removeEmptyControlPoints()
	{
		if (mControlPoints == null)
		{
			return;
		}
		for (int num = mControlPoints.Count - 1; num > -1; num--)
		{
			if (mControlPoints[num] == null)
			{
				mControlPoints.RemoveAt(num);
			}
		}
	}

	private Vector3 minAxis(Vector3 v)
	{
		return (v.x < v.y) ? ((!(v.x < v.z)) ? new Vector3(0f, 0f, 1f) : new Vector3(1f, 0f, 0f)) : ((!(v.y < v.z)) ? new Vector3(0f, 0f, 1f) : new Vector3(0f, 1f, 0f));
	}

	private Vector3 GetUpVector(Vector3 P, Vector3 P1, Vector3 LastUp)
	{
		Vector3 normalized = (P - P1).normalized;
		Vector3 normalized2 = Vector3.Cross(normalized, LastUp).normalized;
		return Vector3.Cross(normalized2, normalized).normalized;
	}

	private bool ClampTF(ref float tf, ref int dir, CurvyClamping clamping)
	{
		switch (clamping)
		{
		case CurvyClamping.Loop:
			if (tf < 0f)
			{
				tf = 1f - tf;
				return true;
			}
			if (tf > 1f)
			{
				tf -= 1f;
				return true;
			}
			break;
		case CurvyClamping.PingPong:
			if (tf < 0f)
			{
				tf *= -1f;
				dir *= -1;
				return true;
			}
			if (tf > 1f)
			{
				tf = 2f - tf;
				dir *= -1;
				return true;
			}
			break;
		default:
			tf = Mathf.Clamp01(tf);
			break;
		}
		return false;
	}

	private bool getPreviousApproximationPoint(CurvySplineSegment seg, int idx, out CurvySplineSegment res, out int residx)
	{
		residx = idx - 1;
		res = seg;
		if (residx < 0)
		{
			res = PreviousSegment(seg);
			if ((bool)res)
			{
				residx = res.Approximation.Length - 2;
			}
			return res != null;
		}
		return true;
	}

	private bool getNextApproximationPoint(CurvySplineSegment seg, int idx, out CurvySplineSegment res, out int residx)
	{
		residx = idx + 1;
		res = seg;
		if (residx == seg.Approximation.Length)
		{
			res = NextSegment(seg);
			residx = 1;
			return res != null;
		}
		return true;
	}

	private bool iterateApproximationPoints(ref CurvySplineSegment seg, ref int idx)
	{
		idx++;
		if (idx == seg.Approximation.Length)
		{
			seg = NextSegment(seg);
			idx = 1;
			return seg != null && !seg.IsFirstSegment;
		}
		return true;
	}

	private float LinePointDistanceSqr(Vector3 l1, Vector3 l2, Vector3 p, out float frag)
	{
		Vector3 vector = l2 - l1;
		Vector3 lhs = p - l1;
		float num = Vector3.Dot(lhs, vector);
		if (num <= 0f)
		{
			frag = 0f;
			return (p - l1).sqrMagnitude;
		}
		float num2 = Vector3.Dot(vector, vector);
		if (num2 <= num)
		{
			frag = 1f;
			return (p - l2).sqrMagnitude;
		}
		frag = num / num2;
		Vector3 b = l1 + frag * vector;
		return (p - b).sqrMagnitude;
	}
}
