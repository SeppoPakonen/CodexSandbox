using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class CurvySpline2 : CurvySpline
{
	public RoadType roadType;

	internal Color color = new Color(0.7f, 0.7f, 0.7f, 1f);

	public int materialId;

	internal Thumbnail thumb;

	public bool shape;

	private LevelEditor levelEditor;

	public Mesh mesh;

	internal bool tunnel;

	internal bool hide;

	public bool offset;

	public CurvySplineSegment pivot;

	private GUIText guiText;

	public int saveId;

	private int random;

	public float heightOffset;

	public bool wallTexture;

	public int SetGranularity
	{
		set
		{
			Granularity = value;
			Interpolation = ((value != 1) ? CurvyInterpolation.CatmulRom : CurvyInterpolation.Linear);
		}
	}

	public override void Awake()
	{
		MonoBehaviour.print("Spline Awake");
		bs._MapLoader.splines.Add(this);
		base.Awake();
	}

	public IEnumerator Start()
	{
		random = UnityEngine.Random.Range(0, 100);
		levelEditor = bs._Loader.levelEditor;
		if (!bs._MapLoader.shapes.Contains(this) && shape)
		{
			bs._MapLoader.shapes.Add(this);
		}
		while (!base.IsInitialized || base.ControlPointCount == 0)
		{
			yield return null;
		}
		if (shape)
		{
			GenerateMesh();
		}
	}

	public override void Refresh(bool refreshLength, bool refreshOrientation, bool skipIfInitialized)
	{
		base.Refresh(refreshLength, refreshOrientation, skipIfInitialized);
		SplineUpdateTf();
	}

	public static float DistancePointLine(Vector3 point, Vector3 lineStart, Vector3 lineEnd)
	{
		return Vector3.Magnitude(ProjectPointLine(point, lineStart, lineEnd) - point);
	}

	public static Vector3 ProjectPointLineVector2(Vector3 point, Vector3 lineStart, Vector3 lineEnd)
	{
		Vector3 v = point - lineStart;
		Vector3 vector = lineEnd - lineStart;
		float magnitude = vector.magnitude;
		Vector3 vector2 = vector;
		if (magnitude > 1E-06f)
		{
			vector2 /= magnitude;
		}
		float d = Mathf.Clamp(Vector2.Dot(vector2, v), 0f, magnitude);
		return lineStart + vector2 * d;
	}

	public static Vector3 ProjectPointLine(Vector3 point, Vector3 lineStart, Vector3 lineEnd)
	{
		Vector3 rhs = point - lineStart;
		Vector3 vector = lineEnd - lineStart;
		float magnitude = vector.magnitude;
		Vector3 vector2 = vector;
		if (magnitude > 1E-06f)
		{
			vector2 /= magnitude;
		}
		float d = Mathf.Clamp(Vector3.Dot(vector2, rhs), 0f, magnitude);
		return lineStart + vector2 * d;
	}

	public new void Update()
	{
		if (KeyDebug(KeyCode.E) && shape)
		{
			MonoBehaviour.print(string.Concat(base.name, GetBounds(local: true), GetBounds(local: true).max));
		}
		bool flag = bs._Game != null && bs._Game.gameObject.activeSelf;
		if (flag && FramesElapsed(10, random))
		{
			foreach (CurvySplineSegment segment in base.Segments)
			{
				float num = DistancePointLine(bs._Player.pos, segment.Position, segment.NextControlPoint.Position);
				float num2 = (!(num < 100f)) ? 3f : 0.1f;
				foreach (SplinePathMeshBuilder sb in segment.sbs)
				{
					if (sb.ExtrusionParameter != num2)
					{
						sb.ExtrusionParameter = num2;
						sb.Refresh();
					}
				}
			}
		}
		if (flag)
		{
			return;
		}
		if (levelEditor == null)
		{
			Closed = (shape || Closed);
		}
		else
		{
			if (levelEditor.shapeEditor && shape)
			{
				bool flag2 = shape && levelEditor.spline != this;
				if (Closed != flag2)
				{
					Closed = flag2;
					if (flag2)
					{
						SetPivot();
					}
					Refresh();
				}
			}
			if (shape && guiText == null)
			{
				guiText = new GameObject(base.name + " Text").AddComponent<GUIText>();
			}
			if (guiText != null)
			{
				guiText.enabled = (Closed && levelEditor.shapeEditor);
				guiText.text = base.name;
				guiText.transform.position = levelEditor.shapeCamera.WorldToViewportPoint(GetCenter() + Vector3.forward * 3f);
			}
		}
		if (!shape && GenerateSbs())
		{
			Refresh();
		}
		Bounds bounds = default(Bounds);
		bounds.SetMinMax(Vector3.zero, new Vector3(Screen.width, Screen.height, 10000f));
		if (bs._Loader.levelEditor != null)
		{
			foreach (CurvySplineSegment segment2 in base.Segments)
			{
				Vector3 mousePosition = Input.mousePosition;
				Camera camera = ((Component)bs._Loader.levelEditor).get_camera();
				Vector3 vector = camera.WorldToScreenPoint(segment2.Position);
				Vector3 vector2 = camera.WorldToScreenPoint(segment2.NextControlPoint.Position);
				segment2.z = (segment2.dist = float.MaxValue);
				if (bounds.Contains(vector) || bounds.Contains(vector2))
				{
					Vector3 vector3 = ProjectPointLineVector2(mousePosition, vector, vector2);
					if (bounds.Contains(vector3))
					{
						segment2.z = vector3.z;
						Vector3 a = camera.ScreenToWorldPoint(mousePosition + Vector3.forward * vector3.z);
						segment2.dist = (a - (segment2.pivot = camera.ScreenToWorldPoint(vector3))).magnitude;
					}
				}
			}
		}
		base.Update();
	}

	public bool GenerateSbs()
	{
		bool result = false;
		using (List<CurvySplineSegment>.Enumerator enumerator = base.ControlPoints.GetEnumerator())
		{
			CurvySplineSegment segment;
			while (enumerator.MoveNext())
			{
				segment = enumerator.Current;
				if (segment.IsValidSegment)
				{
					using (List<CurvySpline2>.Enumerator enumerator2 = segment.spls.GetEnumerator())
					{
						CurvySpline2 spl;
						while (enumerator2.MoveNext())
						{
							spl = enumerator2.Current;
							if (segment.sbs.Any((SplinePathMeshBuilder b) => b.StartMesh == spl.mesh))
							{
								continue;
							}
							if (!levelEditor)
							{
								bool flag = false;
								using (IEnumerator<string> enumerator3 = ((IEnumerable<string>)new string[3]
								{
									"Road",
									"LargeRoad",
									"Speed Track"
								}).SkipWhile((Func<string, bool>)((string a) => a != spl.name)).Skip(1).GetEnumerator())
								{
									string a2;
									while (enumerator3.MoveNext())
									{
										a2 = enumerator3.Current;
										if (segment.spls.Any((CurvySpline2 b) => b.name == a2))
										{
											flag = true;
										}
									}
								}
								if (flag)
								{
									continue;
								}
							}
							SplinePathMeshBuilder item;
							if (bs._MapLoader.OneMesh && segment.PreviousSegment != null && (item = segment.PreviousSegment.sbs.FirstOrDefault((SplinePathMeshBuilder a) => a.StartMesh == spl.mesh)) != null)
							{
								segment.sbs.Add(item);
							}
							else
							{
								item = new GameObject(segment.name + spl.name).AddComponent<SplinePathMeshBuilder>();
								segment.sbs.Add(item);
								item.transform.parent = base.transform;
								item.transform.localRotation = Quaternion.identity;
								item.Spline = this;
								item.ScaleModifier = SplinePathMeshBuilder.MeshScaleModifier.ControlPoint;
								item.CapShape = ((!(spl.mesh == null)) ? SplinePathMeshBuilder.MeshCapShape.Custom : SplinePathMeshBuilder.MeshCapShape.Rectangle);
								item.StartMesh = spl.mesh;
								item.EndCap = (item.StartCap = bs._MapLoader.OneMesh);
								item.CapHeight = 1f;
								item.CapWidth = 10f;
								item.UV = SplinePathMeshBuilder.MeshUV.Absolute;
								item.curvySpline2 = spl;
								item.UVParameter = 10f;
								item.gameObject.layer = Layer.level;
								if (spl.thumb != null)
								{
									((Component)item).get_renderer().sharedMaterial = spl.thumb.material;
								}
								else if (spl.materialId < bs.res.levelTextures.Length)
								{
									((Component)item).get_renderer().material = bs.res.levelTextures[Mathf.Max(0, spl.materialId)];
									((Component)item).get_renderer().sharedMaterial.color = spl.color;
								}
								item.gameObject.tag = spl.roadType.ToString();
								item.ExtrusionParameter = ((!bs._MapLoader.OneMesh) ? 1f : 0.1f);
							}
							result = true;
						}
					}
				}
				object obj;
				if (segment.IsValidSegment)
				{
					IEnumerable<SplinePathMeshBuilder> enumerable = ((IEnumerable<SplinePathMeshBuilder>)segment.sbs).Where((Func<SplinePathMeshBuilder, bool>)((SplinePathMeshBuilder a) => !segment.spls.Any((CurvySpline2 b) => b.mesh == a.StartMesh)));
					obj = enumerable;
				}
				else
				{
					obj = segment.sbs;
				}
				IEnumerable<SplinePathMeshBuilder> source = (IEnumerable<SplinePathMeshBuilder>)obj;
				SplinePathMeshBuilder[] array = source.ToArray();
				foreach (SplinePathMeshBuilder splinePathMeshBuilder in array)
				{
					MonoBehaviour.print("Spline Removed");
					segment.sbs.Remove(splinePathMeshBuilder);
					if (!bs._MapLoader.OneMesh || ((!(segment.NextSegment != null) || !segment.NextSegment.sbs.Contains(splinePathMeshBuilder)) && (!(segment.PreviousControlPoint != null) || !segment.PreviousControlPoint.sbs.Contains(splinePathMeshBuilder))))
					{
						UnityEngine.Object.Destroy(splinePathMeshBuilder.gameObject);
					}
					result = true;
				}
			}
			return result;
		}
	}

	public void SplineUpdateTf()
	{
		List<SplinePathMeshBuilder> list = new List<SplinePathMeshBuilder>();
		foreach (CurvySplineSegment segment in base.Segments)
		{
			foreach (SplinePathMeshBuilder sb in segment.sbs)
			{
				if (sb.UseWorldPosition)
				{
					sb.transform.position = segment.Position;
				}
				else
				{
					sb.transform.position = segment.Spline.pos;
				}
				sb.EndCap = (sb.StartCap = true);
				if (bs._MapLoader.OneMesh)
				{
					if (!list.Contains(sb))
					{
						sb.FromTF = segment.LocalFToTF(0f);
						list.Add(sb);
					}
					sb.ToTF = segment.LocalFToTF(1f);
				}
				else
				{
					sb.FromTF = segment.LocalFToTF(0f);
					sb.ToTF = segment.LocalFToTF(1f);
				}
			}
			if (!bs._MapLoader.OneMesh)
			{
				continue;
			}
			for (int num = list.Count - 1; num >= 0; num--)
			{
				if (!segment.sbs.Contains(list[num]))
				{
					list.Remove(list[num]);
				}
			}
		}
	}

	public void GenerateMesh()
	{
		MonoBehaviour.print("GenerateMeshes " + base.ControlPointCount);
		if (shape && base.ControlPointCount > 1)
		{
			SetPivot();
			Closed = true;
			Refresh();
			CurvyUtility.isPlanar(this, out int ignoreAxis);
			Debug.Log("IgnoreAxis: " + ignoreAxis);
			mesh = MeshHelper.CreateSplineMesh(this, tunnel ? 1 : 2, close: true);
		}
	}

	public void SetPivot()
	{
		if (!shape)
		{
			return;
		}
		Vector3 zero = Vector3.zero;
		foreach (CurvySplineSegment controlPoint in base.ControlPoints)
		{
			controlPoint.transform.parent = null;
		}
		zero = ((pivot != null) ? pivot.Position : ((base.ControlPoints.Count <= 0) ? base.pos : GetCenter()));
		base.transform.position = zero;
		base.transform.eulerAngles = new Vector3(-90f, 0f, 180f);
		foreach (CurvySplineSegment controlPoint2 in base.ControlPoints)
		{
			controlPoint2.transform.parent = base.transform;
		}
		if (pivot == null)
		{
			CreatePivot(base.transform.position);
		}
	}

	private Vector3 GetCenter()
	{
		Vector3 zero = Vector3.zero;
		foreach (CurvySplineSegment controlPoint in base.ControlPoints)
		{
			zero += controlPoint.Position;
		}
		zero /= (float)base.ControlPointCount;
		if (!tunnel)
		{
			foreach (CurvySplineSegment controlPoint2 in base.ControlPoints)
			{
				Vector3 position = controlPoint2.Position;
				zero.z = Math.Max(position.z, zero.z);
			}
			return zero;
		}
		return zero;
	}

	public void CreatePivot(Vector3 pos)
	{
		if (pivot == null)
		{
			Transform transform = new GameObject().transform;
			Transform transform2 = (Transform)UnityEngine.Object.Instantiate((UnityEngine.Object)bs.res.dot);
			transform2.parent = transform;
			transform2.localScale = Vector3.one * 2f;
			((Component)transform2).get_renderer().material.color = Color.yellow;
			transform2.gameObject.layer = Layer.node;
			transform2.name = "model";
			transform.name = base.name + " pivot";
			pivot = transform.gameObject.AddComponent<CurvySplineSegment>();
			pivot.mSpline = this;
		}
		pivot.transform.position = pos;
	}

	public void OnDestroy()
	{
		if (!Loader.loadingLevelQuit)
		{
			if (bs._MapLoader != null)
			{
				bs._MapLoader.splines.Remove(this);
			}
			if (pivot != null)
			{
				UnityEngine.Object.Destroy(pivot.gameObject);
			}
			if (guiText != null)
			{
				UnityEngine.Object.Destroy(guiText.gameObject);
			}
			UnityEngine.Object[] array = UnityEngine.Object.FindObjectsOfType(typeof(CurvySplineSegment));
			for (int i = 0; i < array.Length; i++)
			{
				CurvySplineSegment curvySplineSegment = (CurvySplineSegment)array[i];
				curvySplineSegment.spls.Remove(this);
			}
			bs._MapLoader.shapes.Remove(this);
			bs._MapLoader.brushShapes.Remove(this);
		}
	}

	public void OnApplicationQuit()
	{
		Loader.loadingLevelQuit = true;
	}

	private void DrawLine(Vector3 a, Vector3 b)
	{
		GL.Vertex3(a.x, a.y, a.z);
		GL.Vertex3(b.x, b.y, b.z);
	}

	public void OnRenderObject()
	{
		if (bs._Loader.levelEditor == null || Camera.current != bs._Loader.levelEditor.curCamera)
		{
			return;
		}
		Material lineMaterialYellow = bs.res.lineMaterialYellow;
		lineMaterialYellow.SetPass(0);
		GL.Begin(1);
		foreach (CurvySplineSegment controlPoint in base.ControlPoints)
		{
			Color color = this.color;
			if (hide)
			{
				color.a = 0.7f;
			}
			GL.Color((!(bs._Loader.levelEditor.np != null) || !(bs._Loader.levelEditor.np == controlPoint)) ? color : Color.red);
			if (controlPoint.Approximation.Length != 0)
			{
				Vector3 a = controlPoint.Approximation[0];
				for (int i = 1; i < controlPoint.Approximation.Length; i++)
				{
					Vector3 vector = controlPoint.Approximation[i];
					DrawLine(a, vector);
					a = vector;
				}
			}
		}
		GL.End();
	}
}
