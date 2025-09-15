using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(MeshFilter))]
[RequireComponent(typeof(MeshRenderer))]
[ExecuteInEditMode]
public class SplinePathMeshBuilder : MonoBehaviour
{
	public enum MeshCapShape
	{
		Line,
		NGon,
		Rectangle,
		Custom
	}

	public enum MeshExtrusion
	{
		FixedF,
		FixedDistance,
		Adaptive
	}

	public enum MeshUV
	{
		StretchV,
		StretchVSegment,
		Absolute
	}

	public enum MeshScaleModifier
	{
		None,
		ControlPoint,
		UserValue,
		Delegate,
		AnimationCurve
	}

	public delegate Vector3 OnGetScaleEvent(SplinePathMeshBuilder sender, float tf);

	public CurvySpline Spline;

	public float FromTF;

	public float ToTF = 1f;

	public bool FastInterpolation;

	public bool UseWorldPosition;

	public MeshExtrusion Extrusion = MeshExtrusion.Adaptive;

	public float ExtrusionParameter = 1f;

	public MeshCapShape CapShape = MeshCapShape.NGon;

	public float CapWidth = 1f;

	public float CapHeight = 0.5f;

	public float CapHollow;

	public int CapSegments = 9;

	public bool StartCap = true;

	public Mesh StartMesh;

	public bool EndCap = true;

	public Mesh EndMesh;

	public MeshUV UV;

	public float UVParameter = 1f;

	public MeshScaleModifier ScaleModifier;

	public int ScaleModifierUserValueSlot;

	public AnimationCurve ScaleModifierCurve;

	public bool AutoRefresh = true;

	public float AutoRefreshSpeed;

	public Mesh mMesh;

	private MeshFilter mMeshFilter;

	private Transform mTransform;

	private Vector3[] mVerts;

	private Vector2[] mUV;

	private int[] mTris;

	private float mLastRefresh;

	private List<CurvyMeshSegmentInfo> mSegmentInfo = new List<CurvyMeshSegmentInfo>();

	private MeshInfo StartMeshInfo;

	private MeshInfo EndMeshInfo;

	public CurvySpline2 curvySpline2;

	public int VertexCount => mMesh ? mMesh.vertexCount : 0;

	public int TriangleCount => (mTris != null) ? (mTris.Length / 3) : 0;

	public Mesh Mesh => mMesh;

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

	private int SegmentSteps => mSegmentInfo.Count;

	public event OnGetScaleEvent OnGetScale;

	private void OnEnable()
	{
		mMeshFilter = GetComponent<MeshFilter>();
		if (mMeshFilter == null)
		{
			mMeshFilter = base.gameObject.AddComponent<MeshFilter>();
			base.gameObject.AddComponent<MeshRenderer>();
		}
		if (Application.isPlaying)
		{
			mMesh = mMeshFilter.mesh;
		}
		else
		{
			mMesh = mMeshFilter.sharedMesh;
		}
		mMesh = new Mesh();
		mMesh.name = "CurvyMesh";
		mMeshFilter.sharedMesh = mMesh;
		if (!Application.isPlaying)
		{
			Refresh();
		}
		if (Spline != null)
		{
			Spline.OnRefresh += OnSplineRefresh;
		}
	}

	private void OnDisable()
	{
		if ((bool)Spline)
		{
			Spline.OnRefresh -= OnSplineRefresh;
		}
	}

	private void OnDestroy()
	{
		if (Application.isPlaying)
		{
			Object.Destroy(mMeshFilter.sharedMesh);
		}
		else
		{
			Object.DestroyImmediate(mMeshFilter.sharedMesh);
		}
	}

	private IEnumerator Start()
	{
		if ((bool)Spline)
		{
			while (!Spline.IsInitialized)
			{
				yield return null;
			}
			Refresh();
		}
		Spline.OnRefresh += OnSplineRefresh;
	}

	public static SplinePathMeshBuilder Create()
	{
		return new GameObject("CurvyMeshPath", typeof(SplinePathMeshBuilder)).GetComponent<SplinePathMeshBuilder>();
	}

	public Transform Detach()
	{
		GameObject gameObject = new GameObject();
		gameObject.transform.position = base.transform.position;
		MeshFilter meshFilter = gameObject.AddComponent<MeshFilter>();
		MeshRenderer meshRenderer = gameObject.AddComponent<MeshRenderer>();
		gameObject.name = "CurvyMeshPath_detached";
		meshFilter.sharedMesh = (Object.Instantiate((Object)Mesh) as Mesh);
		meshFilter.sharedMesh.name = "CurvyMesh";
		meshFilter.sharedMesh.RecalculateBounds();
		meshRenderer.sharedMaterials = GetComponent<MeshRenderer>().sharedMaterials;
		return gameObject.transform;
	}

	public void Refresh()
	{
		StartMeshInfo = null;
		EndMeshInfo = null;
		mMesh.Clear();
		if ((bool)Spline && Spline.IsInitialized)
		{
			BuildCaps();
			Prepare();
			if ((bool)StartMesh && StartMeshInfo != null && ToTF - FromTF != 0f)
			{
				mVerts = new Vector3[getTotalVertexCount()];
				mUV = new Vector2[mVerts.Length];
				mTris = new int[getTotalTriIndexCount()];
				Extrude();
				mMesh.vertices = mVerts;
				mMesh.uv = mUV;
				mMesh.triangles = mTris;
				mMesh.RecalculateNormals();
			}
		}
	}

	private void BuildCaps()
	{
		switch (CapShape)
		{
		case MeshCapShape.Line:
			StartMesh = MeshHelper.CreateLineMesh(CapWidth);
			break;
		case MeshCapShape.Rectangle:
			StartMesh = MeshHelper.CreateRectangleMesh(CapWidth, CapHeight, CapHollow);
			break;
		case MeshCapShape.NGon:
			StartMesh = MeshHelper.CreateNgonMesh(CapSegments, CapWidth, CapHollow);
			break;
		}
	}

	private void Prepare()
	{
		if (!Spline || !StartMesh || !(ExtrusionParameter > 0f))
		{
			return;
		}
		StartMeshInfo = new MeshInfo(StartMesh, calculateLoops: true, mirror: false);
		if ((bool)EndMesh)
		{
			EndMeshInfo = new MeshInfo(EndMesh, calculateLoops: false, mirror: true);
		}
		else
		{
			EndMeshInfo = new MeshInfo(StartMesh, calculateLoops: false, mirror: true);
		}
		float tf = FromTF;
		mSegmentInfo.Clear();
		FromTF = Mathf.Clamp01(FromTF);
		ToTF = Mathf.Max(FromTF, Mathf.Clamp01(ToTF));
		if (FromTF == ToTF)
		{
			return;
		}
		Vector3 scale;
		switch (Extrusion)
		{
		case MeshExtrusion.FixedF:
			for (; tf < ToTF; tf += ExtrusionParameter)
			{
				scale = getScale(tf);
				mSegmentInfo.Add(new CurvyMeshSegmentInfo(this, tf, scale));
			}
			break;
		case MeshExtrusion.FixedDistance:
		{
			float num = Spline.TFToDistance(FromTF);
			for (tf = Spline.DistanceToTF(num); tf < ToTF; tf = Spline.DistanceToTF(num))
			{
				scale = getScale(tf);
				mSegmentInfo.Add(new CurvyMeshSegmentInfo(this, tf, num, scale));
				num += ExtrusionParameter;
			}
			break;
		}
		case MeshExtrusion.Adaptive:
			while (tf < ToTF)
			{
				scale = getScale(tf);
				mSegmentInfo.Add(new CurvyMeshSegmentInfo(this, tf, scale));
				int direction = 1;
				Spline.MoveByAngle(ref tf, ref direction, ExtrusionParameter, CurvyClamping.Clamp, 0.001f);
			}
			break;
		}
		if (!Mathf.Approximately(tf, ToTF))
		{
			tf = ToTF;
		}
		scale = getScale(tf);
		mSegmentInfo.Add(new CurvyMeshSegmentInfo(this, tf, scale));
	}

	private void OnSplineRefresh(CurvySpline sender)
	{
		if (Time.realtimeSinceStartup - mLastRefresh > AutoRefreshSpeed)
		{
			mLastRefresh = Time.realtimeSinceStartup;
			Refresh();
		}
	}

	private int getTotalVertexCount()
	{
		int num = 0;
		if ((bool)StartMesh)
		{
			if (StartCap)
			{
				num += StartMesh.vertexCount;
			}
			if (EndCap)
			{
				num += ((!EndMesh) ? StartMesh.vertexCount : EndMesh.vertexCount);
			}
			num = ((StartMeshInfo.LoopVertexCount <= 0) ? (num + SegmentSteps * StartMeshInfo.VertexCount) : (num + SegmentSteps * StartMeshInfo.LoopVertexCount));
		}
		return num;
	}

	private int getTotalTriIndexCount()
	{
		int num = 0;
		if ((bool)StartMesh)
		{
			if (StartCap)
			{
				num += StartMesh.triangles.Length;
			}
			if (EndCap)
			{
				num += ((!EndMesh) ? StartMesh.triangles.Length : EndMesh.triangles.Length);
			}
			num = ((StartMeshInfo.LoopVertexCount != 0) ? (num + (SegmentSteps - 1) * StartMeshInfo.LoopTriIndexCount) : (num + (SegmentSteps - 1) * Mathf.Max(2, StartMeshInfo.VertexCount - 1) * 2 * 3));
		}
		return num;
	}

	private float getV(CurvyMeshSegmentInfo info, int step, int stepsTotal)
	{
		switch (UV)
		{
		case MeshUV.StretchVSegment:
			return (float)step * UVParameter;
		case MeshUV.Absolute:
			return info.Distance / UVParameter;
		default:
			return (float)step / (float)stepsTotal * UVParameter;
		}
	}

	private Vector3 getScale(float tf)
	{
		switch (ScaleModifier)
		{
		case MeshScaleModifier.ControlPoint:
			return Spline.InterpolateScale(tf);
		case MeshScaleModifier.UserValue:
			return Spline.InterpolateUserValue(tf, ScaleModifierUserValueSlot);
		case MeshScaleModifier.Delegate:
			return (this.OnGetScale == null) ? Vector3.one : this.OnGetScale(this, tf);
		case MeshScaleModifier.AnimationCurve:
		{
			Vector3 one = Vector3.one;
			if (ScaleModifierCurve != null)
			{
				return one * ScaleModifierCurve.Evaluate(tf);
			}
			return one;
		}
		default:
			return Vector3.one;
		}
	}

	private void Extrude()
	{
		int segmentSteps = SegmentSteps;
		int num = SegmentSteps - 1;
		int num2 = 0;
		int num3 = 0;
		int num4 = 0;
		int num5 = 0;
		if (StartMeshInfo.LoopVertexCount == 0)
		{
			for (int i = 0; i < segmentSteps; i++)
			{
				float v = getV(mSegmentInfo[i], i, segmentSteps);
				for (int j = 0; j < StartMeshInfo.VertexCount; j++)
				{
					ref Vector3 reference = ref mVerts[num2 + j];
					reference = mSegmentInfo[i].Matrix.MultiplyPoint3x4(StartMeshInfo.Vertices[j]);
					ref Vector2 reference2 = ref mUV[num2 + j];
					reference2 = new Vector2(StartMeshInfo.UVs[j].x, v);
				}
				num2 += StartMeshInfo.VertexCount;
			}
		}
		else
		{
			bool flag = false;
			bool flag2 = false;
			if (curvySpline2 != null)
			{
				flag = curvySpline2.wallTexture;
				flag2 = curvySpline2.rotateTexture;
			}
			for (int k = 0; k < segmentSteps; k++)
			{
				float num6 = getV(mSegmentInfo[k], k, segmentSteps) * 0.5f;
				for (int l = 0; l < StartMeshInfo.EdgeLoops.Length; l++)
				{
					EdgeLoop edgeLoop = StartMeshInfo.EdgeLoops[l];
					for (int m = 0; m < edgeLoop.vertexCount; m++)
					{
						ref Vector3 reference3 = ref mVerts[num2 + m];
						reference3 = mSegmentInfo[k].Matrix.MultiplyPoint3x4(StartMeshInfo.EdgeVertices[edgeLoop.vertexIndex[m]]);
						Vector2 vector = StartMeshInfo.EdgeUV[edgeLoop.vertexIndex[m]];
						Vector2 vector2 = new Vector2((!flag) ? vector.x : num6, (!flag) ? num6 : vector.y);
						if (flag2)
						{
							float y = vector2.y;
							vector2.y = vector2.x;
							vector2.x = y;
						}
						mUV[num2 + m] = vector2;
					}
					num2 += edgeLoop.vertexCount;
				}
			}
		}
		if (StartCap)
		{
			MeshInfo startMeshInfo = StartMeshInfo;
			CurvyMeshSegmentInfo curvyMeshSegmentInfo = mSegmentInfo[0];
			startMeshInfo.TRSVertices(curvyMeshSegmentInfo.Matrix).CopyTo(mVerts, num2);
			StartMeshInfo.UVs.CopyTo(mUV, num2);
			num4 = num2;
			num2 += StartMeshInfo.VertexCount;
		}
		if (EndCap)
		{
			MeshInfo endMeshInfo = EndMeshInfo;
			CurvyMeshSegmentInfo curvyMeshSegmentInfo2 = mSegmentInfo[segmentSteps - 1];
			endMeshInfo.TRSVertices(curvyMeshSegmentInfo2.Matrix).CopyTo(mVerts, num2);
			EndMeshInfo.UVs.CopyTo(mUV, num2);
			num5 = num2;
			num2 += EndMeshInfo.VertexCount;
		}
		if (StartMeshInfo.LoopVertexCount == 0)
		{
			int vertexCount = StartMeshInfo.VertexCount;
			for (int n = 0; n < num; n++)
			{
				int num7 = vertexCount * n;
				int num8 = vertexCount * (n + 1);
				for (int num9 = 0; num9 < StartMeshInfo.VertexCount - 1; num9++)
				{
					mTris[num3] = num7 + num9;
					mTris[num3 + 1] = num8 + num9;
					mTris[num3 + 2] = num8 + 1 + num9;
					mTris[num3 + 3] = num8 + 1 + num9;
					mTris[num3 + 4] = num7 + 1 + num9;
					mTris[num3 + 5] = num7 + num9;
					num3 += 6;
				}
			}
		}
		else
		{
			int loopVertexCount = StartMeshInfo.LoopVertexCount;
			for (int num10 = 0; num10 < num; num10++)
			{
				int num11 = loopVertexCount * num10;
				int num12 = loopVertexCount * (num10 + 1);
				for (int num13 = 0; num13 < StartMeshInfo.EdgeLoops.Length; num13++)
				{
					EdgeLoop edgeLoop2 = StartMeshInfo.EdgeLoops[num13];
					for (int num14 = 0; num14 < edgeLoop2.vertexCount; num14++)
					{
						mTris[num3] = num11 + edgeLoop2.vertexIndex[num14];
						mTris[num3 + 1] = num12 + edgeLoop2.vertexIndex[num14];
						mTris[num3 + 2] = num11 + edgeLoop2.vertexIndex[num14 + 1];
						mTris[num3 + 3] = num12 + edgeLoop2.vertexIndex[num14];
						mTris[num3 + 4] = num12 + edgeLoop2.vertexIndex[num14 + 1];
						mTris[num3 + 5] = num11 + edgeLoop2.vertexIndex[num14 + 1];
						num3 += 6;
					}
				}
			}
		}
		if (StartCap)
		{
			for (int num15 = 0; num15 < StartMeshInfo.Triangles.Length; num15++)
			{
				mTris[num3 + num15] = StartMeshInfo.Triangles[num15] + num4;
			}
			num3 += StartMeshInfo.Triangles.Length;
		}
		if (EndCap)
		{
			for (int num16 = 0; num16 < EndMeshInfo.Triangles.Length; num16++)
			{
				mTris[num3 + num16] = EndMeshInfo.Triangles[num16] + num5;
			}
		}
	}
}
