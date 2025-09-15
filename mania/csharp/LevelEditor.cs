using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class LevelEditor : MapLoader
{
	public enum Tool
	{
		Draw,
		Erase,
		Height,
		Move,
		Rotate,
		Scale,
		Insert,
		CheckPoint,
		StartPoint,
		Brush,
		BrushErase,
		CameraMove,
		CameraRotate,
		CameraZoom,
		Models
	}

	public enum Tool2
	{
		Draw,
		Select,
		Duplicate,
		Move
	}

	public const float cellSize = 1.75f;

	private Camera camera;

	public Transform cursor;

	public static bool scriptRefresh;

	private string[] toolStrs = new string[15]
	{
		null,
		null,
		"Height",
		"Move",
		"Rotate",
		"Scale",
		"Insert",
		"CheckPoint",
		"StartPoint",
		"Brush",
		null,
		null,
		"Cam. Rot",
		null,
		"Models"
	};

	private string[] toolStrsAndroid = new string[14]
	{
		null,
		"Erase",
		"Height",
		"Move",
		"Rotate",
		"Scale",
		null,
		"CheckPoint",
		"StartPoint",
		"Brush",
		"Insert",
		"Cam.Move",
		"Cam.Rot",
		"Cam.Zoom"
	};

	private string[] toolStrsShape = new string[14]
	{
		null,
		null,
		null,
		"Move",
		null,
		null,
		"Insert",
		null,
		null,
		null,
		null,
		"Cam.Move",
		null,
		"cam zoom"
	};

	internal Tool tool = Tool.Height;

	private RaycastHit hit;

	private Vector3 rotateCamPivot;

	public List<Material> materials;

	public Texture[] textures;

	private float mouseScroll;

	internal bool mouseButtonDown1;

	private bool mouseButton1;

	private bool mouseButtonDown0;

	internal bool mouseButtonAny;

	internal bool mouseButtonDownAny;

	internal bool mouseButtonDown2;

	private bool mouseButtonUp0;

	private bool mouseButtonUp1;

	private bool mouseButton0;

	private bool onHeight;

	private bool alt;

	private bool onRotate;

	private bool onScale;

	private bool onRotate2;

	private bool onScale2;

	private bool onMove2;

	private bool onHeight2;

	private bool windowHit;

	public Camera defCamera;

	private Ray ray;

	private string search = string.Empty;

	internal bool hitTest;

	internal CurvySplineSegment hitTestSegment;

	internal CurvySplineSegment np;

	private bool mouseButton2;

	private Vector3 mpos;

	private Vector2 mouseDelta;

	private bool mouseButtonUpAny;

	private bool shift;

	private Vector3 dragStart;

	private Vector3 dragMove;

	private Transform checkPointDrag;

	private Vector3 oldCursorPos;

	internal bool autoRefreshTerrain;

	private bool onMove;

	public bool flying;

	private bool hideMenu;

	private string[] tutorialUrls;

	private List<Texture2D> tutorialTextures;

	private int selectedSlide;

	private float windowScroll;

	public Vector2 roadShapesScroll;

	private int brushToolsTab;

	private bool drawRoad = true;

	private string[] roadTypes;

	private int curFolder;

	internal bool submitMapPublish;

	private bool starting;

	protected bool enableClosed2 = true;

	private List<GameObject> mbs = new List<GameObject>();

	public MapSets mapSets = new MapSets
	{
		levelFlags = LevelFlags.race
	};

	private GameObject selectedGameObject;

	public Tool2 tool2;

	internal float scalePow;

	public Grid grid;

	private Transform duplicate;

	private Stack<ModelItem> stack = new Stack<ModelItem>();

	public ModelObject sgo;

	public ModelObject lastSgo;

	public List<ModelObject> selection = new List<ModelObject>();

	public Vector3 mouseDrag;

	public bool drawDragRect;

	public Vector3 startDrag;

	private Tool2 tooltmp;

	internal bool showGrid;

	private string modelSearch = string.Empty;

	private List<ModelFile> recent = new List<ModelFile>();

	private GUIStyle folderStype;

	private Vector3 modelViewOffset;

	public Camera shapeCamera;

	public string shapeName = "My Shape";

	private Vector3 cursorPos
	{
		get
		{
			return cursor.position;
		}
		set
		{
		}
	}

	private bool draw => tool != Tool.Erase && tool != Tool.CheckPoint && tool != Tool.StartPoint && tool != Tool.Insert && tool != Tool.BrushErase && tool != Tool.CameraMove && tool != Tool.CameraRotate && tool != Tool.CameraZoom && tool != Tool.Brush && !Input.GetKey(KeyCode.LeftShift) && tool != Tool.Models && drawRoad;

	public Camera curCamera => (!shapeEditor) ? camera : shapeCamera;

	private float scaleFactor => Mathf.Max(2f, (pos - cursorPos).magnitude * 0.01f);

	public List<string> myMaps
	{
		get
		{
			return Base.PlayerPrefsGetStrings("savedMaps", string.Empty);
		}
		set
		{
			Base.PlayerPrefsSetStringList("savedMaps", value);
		}
	}

	public bool tutorialPopup
	{
		get
		{
			return PlayerPrefs.GetInt("tutorialPopup", 1) == 1;
		}
		set
		{
			PlayerPrefs.SetInt("tutorialPopup", value ? 1 : 0);
		}
	}

	private string mapName
	{
		get
		{
			return Base.PlayerPrefsGetString("mapName", string.Empty);
		}
		set
		{
			Base.PlayerPrefsSetString("mapName", value);
		}
	}

	public new Vector3 pos
	{
		get
		{
			return camera.transform.position;
		}
		set
		{
			camera.transform.position = value;
		}
	}

	public new void Start()
	{
		//IL_00f7: Unknown result type (might be due to invalid IL or missing references)
		//IL_0102: Expected O, but got Unknown
		//IL_010f: Unknown result type (might be due to invalid IL or missing references)
		//IL_011a: Expected O, but got Unknown
		if (!bs._Loader.menuLoaded)
		{
			bs._Loader.SetOffline(offline: true);
		}
		if (bs.isDebug)
		{
			hideMenu = false;
		}
		if (!bs._Loader.menuLoaded)
		{
			bs._Loader.sGameType = SGameType.VsPlayers;
		}
		Time.timeScale = 1f;
		List<Material> list = new List<Material>();
		Material[] levelTextures = bs.res.levelTextures;
		foreach (Material item in levelTextures)
		{
			list.Add(item);
		}
		materials = list;
		textures = ((IEnumerable<Material>)list).Select<Material, Texture>((Func<Material, Texture>)((Material a) => a.mainTexture)).ToArray();
		MonoBehaviour.print("Editor Start");
		camera = base.get_camera();
		bs._Loader.levelEditor = this;
		GuiClasses.LoadTranslate();
		if (tutorialPopup && !bs.android)
		{
			ShowWindow((Action)(object)new Action(TutorialPopup));
		}
		else
		{
			ShowWindow((Action)(object)new Action(OnEditorWindow));
		}
		StartCoroutine(bs._AutoQuality.OnLevelWasLoaded2(0));
		ModeViewStart();
		if (MapLoader.loadMap != null)
		{
			StartLoadMap();
		}
		if (MapLoader.unityMap != null)
		{
			StartCoroutine(LoadTestMap2());
		}
		base.Start();
	}

	private IEnumerator LoadTestMap2()
	{
		base.hideTerrain = true;
		yield return StartCoroutine(LoadUnityMap(MapLoader.unityMap));
		ResetCam();
	}

	public void TutorialPopup()
	{
		//IL_0086: Unknown result type (might be due to invalid IL or missing references)
		//IL_0091: Expected O, but got Unknown
		bs.win.Setup(400, 200, "Tutorials", Dock.Center, null, null, 1f);
		Label("Check out video tutorials on Youtube");
		VideoYoutube();
		GUILayout.FlexibleSpace();
		GUILayout.BeginHorizontal();
		tutorialPopup = !GUILayout.Toggle(!tutorialPopup, "don't show me this again");
		if (Button("Continue"))
		{
			ShowWindow((Action)(object)new Action(OnEditorWindow));
		}
		GUILayout.EndHorizontal();
	}

	private void VideoYoutube()
	{
		//IL_0058: Unknown result type (might be due to invalid IL or missing references)
		//IL_0062: Expected O, but got Unknown
		if (bs.android)
		{
			if (Button("Video Tutorial"))
			{
				Application.OpenURL("http://www.youtube.com/playlist?list=PLAAP2vqx0GEEtwLHD9_ah38jLRhP5512o");
			}
		}
		else if (GUILayout.Button("Video Tutorial", GUILayout.ExpandWidth(expand: false)))
		{
			ShowWindow((Action)(object)(Action)delegate
			{
				GUILayout.TextField("http://www.youtube.com/playlist?list=PLAAP2vqx0GEFZ6sk5QDmkEmk16GcNFAHW");
				GUILayout.Label("Link to tutorial, use Ctrl+C to copy");
			}, bs.win.act);
			Screen.fullScreen = false;
			bs.ExternalEval("ShowYoutube()");
		}
	}

	private void StartLoadMap()
	{
		//IL_000d: Unknown result type (might be due to invalid IL or missing references)
		//IL_0019: Expected O, but got Unknown
		//IL_0027: Unknown result type (might be due to invalid IL or missing references)
		//IL_0033: Unknown result type (might be due to invalid IL or missing references)
		//IL_003d: Expected O, but got Unknown
		//IL_003d: Expected O, but got Unknown
		Popup("LoadingMap", (Action)(object)new Action(OnEditorWindow));
		StartCoroutine(LoadMap(MapLoader.loadMap, (Action)(object)(Action)delegate
		{
			//IL_0013: Unknown result type (might be due to invalid IL or missing references)
			//IL_002a: Expected O, but got Unknown
			Popup2(www.error, (Action)(object)new Action(OnEditorWindow));
		}, (Action)(object)(Action)delegate
		{
			//IL_0014: Unknown result type (might be due to invalid IL or missing references)
			//IL_001f: Expected O, but got Unknown
			Reset();
			ResetCam();
			ShowWindow((Action)(object)new Action(OnEditorWindow));
		}));
	}

	private void Clear()
	{
		if (start != null)
		{
			UnityEngine.Object.DestroyImmediate(start.gameObject);
		}
		MapLoader.unityMap = null;
		CurvySpline2[] array = splines.ToArray();
		foreach (CurvySpline2 curvySpline in array)
		{
			if (!curvySpline.shape)
			{
				UnityEngine.Object.DestroyImmediate(curvySpline.gameObject);
			}
		}
		tool = Tool.Height;
		ResetCam();
		affectPrev = (affectNext = false);
		UpdateTerrain(null, refresh: true);
		ModelObject[] array2 = UnityEngine.Object.FindObjectsOfType<ModelObject>();
		foreach (ModelObject modelObject in array2)
		{
			UnityEngine.Object.Destroy(modelObject.gameObject);
		}
		Application.LoadLevel("level");
	}

	internal void ResetCam()
	{
		Bounds bounds = default(Bounds);
		MeshCollider[] array = UnityEngine.Object.FindObjectsOfType<MeshCollider>();
		foreach (MeshCollider meshCollider in array)
		{
			bounds.Encapsulate(meshCollider.bounds);
		}
		base.transform.position = bounds.center + Vector3.up * 168.399f;
		base.transform.forward = Vector3.down + Vector3.forward;
		cursor.position = bounds.center + Vector3.up * 50f;
	}

	private void DrawLine(Vector3 a, Vector3 b)
	{
		GL.Vertex3(a.x, a.y, a.z);
		GL.Vertex3(b.x, b.y, b.z);
	}

	private void DrawLine2D(Vector3 a, Vector3 b)
	{
		a = camera.WorldToViewportPoint(a);
		b = camera.WorldToViewportPoint(b);
		b.z = (a.z = 0f);
		GL.PushMatrix();
		bs.res.lineMaterialYellow.SetPass(0);
		GL.LoadOrtho();
		GL.Color(Color.yellow);
		GL.Begin(1);
		GL.Vertex3(a.x, a.y, a.z);
		GL.Vertex3(b.x, b.y, a.z);
		GL.End();
		GL.PopMatrix();
	}

	private Color Transparement(Color c)
	{
		c.a = 0.5f;
		return c;
	}

	public void UpdateAlways()
	{
		ray = camera.ScreenPointToRay(mpos);
		UpdateNodeRenderer();
		if (segment != null && segment.Spline2.shape && !mapLoading)
		{
			segment.Spline2.materialId = materialId;
		}
		if (Input.GetKeyDown(KeyCode.R) && bs.isDebug)
		{
			Application.LoadLevel(Application.loadedLevelName);
		}
		mouseButton1 = (!bs.android && Input.GetMouseButton(1));
		mouseButtonDown0 = Input.GetMouseButtonDown(0);
		mouseButtonDown1 = (!bs.android && Input.GetMouseButtonDown(1));
		mouseButtonDown2 = (!bs.android && Input.GetMouseButtonDown(2));
		mouseButtonAny = (mouseButton0 || mouseButton1 || mouseButton2);
		mouseButtonDownAny = (mouseButtonDown0 || mouseButtonDown1 || mouseButtonDown2);
		mouseButtonUp0 = Input.GetMouseButtonUp(0);
		mouseButtonUp1 = ((!bs.android && Input.GetMouseButtonUp(1)) || (mouseButtonUp0 && tool == Tool.Erase));
		mouseButtonUpAny = Input.GetMouseButtonUp(0);
		mouseButton0 = (Input.GetMouseButton(0) || Input.GetMouseButtonUp(0));
		if (mouseButton0 || mouseButtonDown0 || !bs.android)
		{
			mpos = Input.mousePosition;
		}
		mouseButton2 = (!bs.android && Input.GetMouseButton(2));
		alt = (Input.GetKey(KeyCode.LeftAlt) && !shapeEditor);
		shift = Input.GetKey(KeyCode.LeftShift);
		if (!mouseButton0)
		{
			onScale2 = (onRotate2 = (onMove2 = (onHeight2 = false)));
		}
		onScale = (onScale2 = (Input.GetKey(KeyCode.LeftControl) || Input.GetKey(KeyCode.R) || onScale2));
		onRotate = (onRotate2 = (Input.GetKey(KeyCode.Tab) || Input.GetKey(KeyCode.E) || onRotate2));
		onMove = (onMove2 = (Input.GetKey(KeyCode.CapsLock) || onMove2));
		onHeight = (onHeight2 = (shift || onHeight2));
		if (!onScale && !onRotate && !onMove && !onHeight)
		{
			onScale = (tool == Tool.Scale);
			onRotate = (tool == Tool.Rotate);
			onMove = (tool == Tool.Move || tool == Tool.Insert || tool == Tool.Draw);
			onHeight = (tool == Tool.Height);
		}
		mouseScroll = Input.GetAxis("Mouse ScrollWheel");
		mouseDelta = getMouseDelta();
		if (shapeEditor && tool == Tool.Height)
		{
			tool = Tool.Move;
		}
		else
		{
			int i = 0;
			int num = 0;
			for (; i < toolStrs.Length; i++)
			{
				if (toolStrs[i] != null)
				{
					if (Input.GetKeyDown((KeyCode)(49 + num)))
					{
						tool = (Tool)i;
					}
					num++;
				}
			}
		}
		UpdateMove();
		if (mouseButtonUpAny && !starting && autoRefreshTerrain && segment != null && !segment.flying)
		{
			UpdateTerrain((!segment.IsValidSegment) ? segment.PreviousControlPoint : segment, affectPrev || affectNext);
		}
		if (!mouseButton0)
		{
			windowHit = false;
		}
		if (!bs.android || mouseButtonDown0 || mouseButtonUp0)
		{
			Vector3 mousePosition = Input.mousePosition;
			if (mousePosition.y > (float)Screen.height - 23f * bs.win.scale.y || bs.win.WindowHit)
			{
				windowHit = true;
			}
		}
	}

	private void UpdateMove()
	{
		if (bs.android || ((!alt || mouseButton2) && !mouseButton0))
		{
			Vector3 zero = Vector3.zero;
			zero += new Vector3(Input.GetAxisRaw("Horizontalz"), 0f, Input.GetAxisRaw("Verticalz")) * Time.deltaTime * 100f;
			if (mouseButton2 || (mouseButton0 && tool == Tool.CameraMove))
			{
				zero += new Vector3(0f - mouseDelta.x, 0f, 0f - mouseDelta.y) * scaleFactor * 2f;
			}
			zero = Quaternion.LookRotation(bs.ZeroY(shapeEditor ? Vector3.forward : base.transform.forward, 0f)) * zero * ((!shapeEditor) ? 1f : 0.3f);
			if (shapeEditor)
			{
				zero = new Vector3(0f - zero.x, 0f, zero.z);
			}
			if (zero != Vector3.zero)
			{
				pos += zero;
			}
		}
	}

	public void UpdateCheckPoint()
	{
		if (oldCursorPos != Vector3.zero)
		{
			dragMove += cursor.position - oldCursorPos;
		}
		oldCursorPos = cursor.position;
		if (!mouseButtonAny)
		{
			dragMove = Vector3.zero;
		}
		if (tool == Tool.CheckPoint && Input.GetMouseButtonUp(1) && Physics.Raycast(camera.ScreenPointToRay(mpos), out RaycastHit hitInfo, 10000f) && hitInfo.collider.gameObject.layer == Layer.CheckPoint)
		{
			UnityEngine.Object.Destroy(hitInfo.collider.gameObject);
		}
		if (tool != Tool.CheckPoint && tool != Tool.StartPoint)
		{
			return;
		}
		if (np != null)
		{
			float dist = np.dist;
			Vector2 bounds = np.GetBounds();
			if (dist < bounds.x / 2f && mouseButtonDown0)
			{
				dragStart = np.point;
				Vector3 position = dragStart;
				cursor.position = position;
				oldCursorPos = position;
				Vector3 normalized = (np.NextControlPoint.Position - np.Position).normalized;
				dragMove = normalized * 10f;
				if (tool == Tool.CheckPoint)
				{
					checkPointDrag = null;
					SetCheckPoint(np.point).forward = normalized;
				}
				if (tool == Tool.StartPoint)
				{
					SetStartPoint(np.point + Vector3.up, normalized);
				}
				return;
			}
		}
		if (!Physics.Raycast(camera.ScreenPointToRay(mpos), out hitInfo, 10000f, Layer.levelMask))
		{
			return;
		}
		if (mouseButtonDown0)
		{
			dragStart = hitInfo.point;
			if (tool == Tool.CheckPoint)
			{
				checkPointDrag = SetCheckPoint(dragStart + Vector3.up);
			}
		}
		if (mouseButton0 && dragStart != Vector3.zero)
		{
			if (tool == Tool.StartPoint)
			{
				SetStartPoint(dragStart + Vector3.up, dragMove);
			}
			if (tool == Tool.CheckPoint && checkPointDrag != null)
			{
				checkPointDrag.up = hitInfo.normal;
				checkPointDrag.forward = dragStart - hitInfo.point;
			}
		}
	}

	public void Update()
	{
		//IL_0a56: Unknown result type (might be due to invalid IL or missing references)
		//IL_0a66: Expected O, but got Unknown
		UpdateAlways();
		if (windowHit)
		{
			return;
		}
		if (((!onHeight || (!mouseButton0 && !shift)) && !bs.android) || (bs.android && mouseButtonAny))
		{
			Vector3 point = GetPoint();
			cursor.position = point;
			cursorPos = point;
		}
		if (mouseScroll != 0f)
		{
			UpdateHeight(mouseScroll * 5f, scroll: true);
		}
		if (!scriptRefresh)
		{
			UnityEngine.Object[] array = UnityEngine.Object.FindObjectsOfType(typeof(SplinePathMeshBuilder));
			for (int i = 0; i < array.Length; i++)
			{
				SplinePathMeshBuilder splinePathMeshBuilder = (SplinePathMeshBuilder)array[i];
				splinePathMeshBuilder.Refresh();
			}
		}
		scriptRefresh = true;
		if (KeyDebug(KeyCode.Alpha1, "break"))
		{
			Debug.Break();
		}
		IEnumerable<CurvySplineSegment> source = ((IEnumerable<CurvySpline2>)splines).Where((Func<CurvySpline2, bool>)((CurvySpline2 a) => !a.shape)).SelectMany<CurvySpline2, CurvySplineSegment>((Func<CurvySpline2, IEnumerable<CurvySplineSegment>>)((CurvySpline2 a) => a.Segments));
		np = (from a in source.Where((Func<CurvySplineSegment, bool>)((CurvySplineSegment a) => a.dist < 10f))
			orderby a.z
			select a).FirstOrDefault();
		if (np == null)
		{
			np = (from a in source.Where((Func<CurvySplineSegment, bool>)((CurvySplineSegment a) => a.dist != float.MaxValue))
				orderby a.dist
				select a).FirstOrDefault();
		}
		if (np != null)
		{
			np.point = np.Spline2.Interpolate(np.Spline2.GetNearestPointTF(np.pivot));
			Debug.DrawLine(cursorPos, np.pivot, Color.blue);
		}
		if (!mouseButton0 || (mouseButtonDown0 && bs.android))
		{
			rotateCamPivot = GetPoint();
		}
		if (Input.GetKeyDown(KeyCode.Z) && segment != null && segment.NextControlPoint == null)
		{
			if (segment.Spline.Closed && enableClosed2)
			{
				segment.Spline.Closed = false;
			}
			else
			{
				Remove(segment.gameObject);
			}
		}
		if (Input.GetKeyDown(KeyCode.Delete) && spline != null)
		{
			UnityEngine.Object.Destroy(spline.gameObject);
		}
		if (mouseButton0 && draw)
		{
			Cursor.SetCursor(bs._Loader.guiSkins.updownCursor, Vector2.one * 16f, CursorMode.Auto);
		}
		else
		{
			Cursor.SetCursor(null, Vector3.zero, CursorMode.Auto);
		}
		if (mouseButton0 && (alt || tool == Tool.CameraRotate))
		{
			base.transform.RotateAround(rotateCamPivot, Vector3.up, mouseDelta.x * 10f);
			base.transform.RotateAround(rotateCamPivot, base.transform.right, (0f - mouseDelta.y) * 5f);
		}
		if (mouseButton1 || (mouseButton0 && tool == Tool.CameraZoom))
		{
			if (!shapeEditor)
			{
				pos += base.transform.forward * mouseDelta.y * scaleFactor * 2f;
			}
			else
			{
				shapeCamera.orthographicSize += mouseDelta.y;
			}
		}
		if (alt)
		{
			return;
		}
		UpdateCheckPoint();
		UpdateModelView();
		hitTestSegment = null;
		if (tool != Tool.Brush && tool != Tool.BrushErase && tool != Tool.CheckPoint)
		{
			if (bs.android)
			{
				hitTest = Physics.SphereCast(ray, 3f, out hit, 1000f, 1 << Layer.node);
			}
			else
			{
				hitTest = Physics.Raycast(ray, out hit, 1000f, 1 << Layer.node);
			}
			if (hitTest)
			{
				hitTestSegment = hit.transform.parent.GetComponent<CurvySplineSegment>();
			}
			if (hitTest && mouseButtonUp1)
			{
				Transform checkPoint = getCheckPoint(hit.transform.parent);
				if ((bool)checkPoint)
				{
					UnityEngine.Object.Destroy(checkPoint.gameObject);
				}
				else
				{
					Remove(hit.transform.parent.gameObject);
				}
				return;
			}
			if (mouseButtonDown0)
			{
				if (hitTest)
				{
					CurvySplineSegment component = hit.transform.parent.GetComponent<CurvySplineSegment>();
					if (enableClosed2 && segment != null && component != segment && component.isEnd && segment.isEnd && component.Spline == segment.Spline && draw && !component.Spline2.shape && segment.Spline2.Count > 2)
					{
						component.Spline.Closed = true;
						component.Spline.Update();
						MonoBehaviour.print("Close");
						return;
					}
					segment = component;
					drag = segment.transform;
					CopyFrom(segment, segment.Position);
					segment.dragOffset = drag.position - cursor.position;
					if (draw)
					{
						return;
					}
				}
				else if (!drawRoad)
				{
					segment = null;
				}
			}
		}
		if (!mouseButton0)
		{
			drag = null;
		}
		if (onHeight && (mouseButton0 || shift))
		{
			UpdateHeight(mouseDelta.y, drag == null);
			if (tool != Tool.Height)
			{
				return;
			}
		}
		if (np != null && np.dist < 100f && drag == null)
		{
			CurvySplineSegment curvySplineSegment = np;
			if (!mouseButtonDown0 && mouseButton0 && tool == Tool.Brush)
			{
				curvySplineSegment.spls = brushShapes.ToList();
			}
			if (tool == Tool.Insert && mouseButtonDown0)
			{
				CopyFrom(curvySplineSegment, np.point);
				AddPoint(np.point, curvySplineSegment, before: false, insert: true);
				return;
			}
		}
		if (segment != null)
		{
			float y = mouseDelta.y;
			if (mouseButton0 && drag != null && onRotate)
			{
				if (affectNext || affectPrev)
				{
					foreach (CurvySplineSegment item in GetAffectBy())
					{
						item.transform.RotateAround(segment.transform.position, Vector3.up, y * 6f);
					}
				}
				else
				{
					swirl += y * 3f;
					UpdateSwirl();
				}
			}
			if (mouseButton0 && onScale && drag != null)
			{
				UpdateScale(y);
				scale += y;
			}
			if (mouseButton0 && onMove && drag != null)
			{
				Vector3 vector = cursorPos - segment.Position + segment.dragOffset;
				foreach (CurvySplineSegment item2 in GetAffectBy())
				{
					item2.Position += vector;
				}
			}
		}
		if (mouseButtonDown0 && draw)
		{
			if (segment == null || segment.PreviousControlPoint == null || segment.NextControlPoint == null)
			{
				if (shapeEditor)
				{
					mapSets.usedAdvancedTools = true;
				}
				if (!(segment == null))
				{
					if (brushShapes.Count == 0)
					{
						brushShapes.Add(shapes[0]);
					}
					bool flag = Vector3.Dot(segment.transform.forward, (cursorPos - segment.Position).normalized) > 0f || segment.IsFirstSegment || segment.IsLastSegment;
					AddPoint(cursorPos, (!flag) ? segment.PreviousControlPoint : segment, segment == segment.Spline.ControlPoints[0] && segment.Spline.ControlPoints.Count > 1);
					return;
				}
				StartCoroutine(CreateSpline((Action)(object)(Action)delegate
				{
					AddPoint(cursorPos);
				}, shapeEditor));
			}
			else
			{
				Reset();
			}
		}
		if (mouseButtonUp1 && bs.win.mouseDrag < (float)((!bs.android) ? 1 : 5) / 1024f)
		{
			Reset();
		}
		UpdateSwirl2();
	}

	private void UpdateNodeRenderer()
	{
		foreach (CurvySpline2 spline2 in splines)
		{
			foreach (CurvySplineSegment controlPoint in spline2.ControlPoints)
			{
				Transform transform = controlPoint.transform.Find("model");
				if (controlPoint != null && controlPoint.transform != null && controlPoint.transform.childCount > 0 && ((Component)transform).get_renderer() != null)
				{
					((Component)transform).get_renderer().material.color = Transparement((controlPoint == segment) ? Color.red : ((!(controlPoint == hitTestSegment)) ? Color.white : Color.green));
					Renderer renderer = ((Component)transform).get_renderer();
					bool enabled = !spline2.hide;
					((Component)transform).get_collider().enabled = enabled;
					renderer.enabled = enabled;
					if (!controlPoint.Spline2.shape)
					{
						transform.localScale = Vector3.one * (3f + (transform.position - camera.transform.position).magnitude * 0.03f);
					}
				}
			}
			if (spline2.pivot != null)
			{
				Transform child = spline2.pivot.transform.GetChild(0);
				Collider collider = ((Component)child).get_collider();
				bool enabled = !spline2.hide;
				((Component)child).get_renderer().enabled = enabled;
				collider.enabled = enabled;
			}
		}
	}

	private Transform getFinnish(Transform cv)
	{
		return cv.transform.Find("Finnish");
	}

	private Transform getCheckPoint(Transform cv)
	{
		Transform transform = cv.transform.Find(bs.res.CheckPoint.name);
		if (transform != null)
		{
			return transform;
		}
		return getFinnish(cv);
	}

	private void CopyFrom(CurvySplineSegment segment, Vector3 position)
	{
		swirl = segment.swirl;
		scale = segment.scale;
		flying = segment.flying;
		spline = (CurvySpline2)segment.Spline;
		if (spline.shape)
		{
			materialId = spline.materialId;
		}
		float y = position.y;
		Vector3 position2 = cursor.position;
		if (Math.Abs(y - position2.y) > 0.01f)
		{
			cursor.position = position;
			Vector3 point = GetPoint();
			cursor.position = point;
			cursorPos = point;
		}
	}

	public void SaveMapWindow()
	{
		//IL_006c: Unknown result type (might be due to invalid IL or missing references)
		//IL_007c: Expected O, but got Unknown
		if (www == null)
		{
			Label("Map Name");
			mapName = Loader.Filter(GUILayout.TextField(mapName, 20));
			if (Button("Save"))
			{
				if (mapName.Length < 3)
				{
					Popup("Enter Map Name", (Action)(object)new Action(SaveMapWindow), "Continue");
				}
				else
				{
					StartCoroutine(SaveMap());
				}
			}
		}
		else
		{
			Label(Trs("Uploading: ") + (int)(www.progress * 100f) + "%");
			if (www.isDone)
			{
				GUILayout.Label((!string.IsNullOrEmpty(www.error)) ? www.error : www.text);
			}
		}
	}

	public IEnumerator SaveMap()
	{
		tool = Tool.Height;
		Update();
		UpdateModelView();
		using (BinaryWriter ms = new BinaryWriter())
		{
			ms.Write(9);
			ms.Write(bs.setting.version);
			Debug.Log("write map name " + MapLoader.unityMap);
			if (!string.IsNullOrEmpty(MapLoader.unityMap))
			{
				ms.Write(26);
				ms.Write(MapLoader.unityMap);
			}
			ms.Write(7);
			ms.Write(nitro);
			if (base.hideTerrain)
			{
				ms.Write(21);
			}
			ms.Write(14);
			ms.Write(bs._GameSettings.gravitationAntiFly);
			ms.Write(bs._GameSettings.gravitationFactor);
			ms.Write(25);
			ms.Write(bs._GameSettings.levelTime);
			if (start != null)
			{
				ms.Write(8);
				ms.Write(start.position);
				ms.Write(start.forward);
			}
			UnityEngine.Object[] array = UnityEngine.Object.FindObjectsOfType(typeof(ModelObject));
			for (int j = 0; j < array.Length; j++)
			{
				ModelObject a2 = (ModelObject)array[j];
				if (base.modelLib != null && base.modelLib.dict.ContainsKey(a2.name2))
				{
					ms.Write(20);
					ms.Write(a2.name2);
					ms.Write(a2.pos);
					ms.Write(a2.rot.eulerAngles);
					ms.Write(a2.transform.localScale);
				}
				else
				{
					MonoBehaviour.print(a2.name2 + " not found");
				}
			}
			CurvySpline2[] splines = base.splines.OrderByDescending<CurvySpline2, bool>((CurvySpline2 a) => a.shape).ToArray();
			for (int i = 0; i < splines.Length; i++)
			{
				splines[i].saveId = i;
			}
			foreach (GameObject a3 in ((IEnumerable<GameObject>)bs.checkPoints).Where((Func<GameObject, bool>)((GameObject a) => a.name.StartsWith("CheckPoint2"))))
			{
				ms.Write(22);
				ms.Write(a3.transform.position);
				ms.Write(a3.transform.eulerAngles);
			}
			CurvySpline2[] array2 = splines;
			foreach (CurvySpline2 sp in array2)
			{
				if (sp.shape)
				{
					ms.Write(11);
					ms.Write(sp.pivot.Position);
					ms.Write(sp.saveId);
					ms.Write(sp.tunnel);
					ms.Write(sp.materialId);
					ms.Write(sp.color);
					ms.Write(sp.name);
					if (sp.thumb != null)
					{
						ms.Write(16);
						ms.Write(sp.thumb.url);
						ms.Write(24);
						ms.Write(sp.thumb.material.mainTextureScale);
					}
					ms.Write(17);
					ms.Write((byte)sp.roadType);
					if (sp.wallTexture)
					{
						ms.Write(18);
					}
					if (sp.rotateTexture)
					{
						ms.Write(23);
					}
				}
				else
				{
					ms.Write(0);
				}
				foreach (CurvySplineSegment b in sp.ControlPoints)
				{
					ms.Write(1);
					ms.Write(b.Position);
					ms.Write(b.swirl);
					ms.Write(4);
					ms.Write(b.scale);
					ms.Write(15);
					ms.Write(b.flying);
					if ((bool)getCheckPoint(b.transform))
					{
						if ((bool)getFinnish(b.transform))
						{
							ms.Write(10);
						}
						MonoBehaviour.print("Write Checkpoint");
					}
					if (sp.shape)
					{
						continue;
					}
					foreach (CurvySpline2 c in b.spls)
					{
						ms.Write(12);
						ms.Write(c.saveId);
					}
				}
				if (sp.Closed)
				{
					ms.Write(19);
				}
			}
			ms.Write(5);
			ms.Write(laps);
			www = bs.Download(bs.mainSite + "scripts/sendMap.php", null, true, "name", bs._Loader.playerNamePrefixed, "map", mapName, "file", ms.ToArray(), "version", bs.setting.levelEditorVersion, "flags", (int)mapSets.levelFlags);
			yield return www;
			www = null;
			myMaps.Add(bs._Loader.playerNamePrefixed + "." + mapName);
			myMaps = myMaps.Distinct().ToList();
			ShowWindow((Action)(object)new Action(TestMapWindow));
		}
	}

	private void TestMapWindow()
	{
		//IL_0038: Unknown result type (might be due to invalid IL or missing references)
		//IL_0043: Expected O, but got Unknown
		Label("Do you want to publish map? you have to test it before it get published");
		GUILayout.BeginHorizontal();
		if (Button("No"))
		{
			ShowWindow((Action)(object)new Action(OnEditorWindow));
		}
		if (Button("Yes"))
		{
			if (mapSets.enableDm && GameObject.FindGameObjectsWithTag("Spawn").Length < 3)
			{
				Popup("You must add at least 5 spawns");
				return;
			}
			if (mapSets.enableCtf && (!GameObject.FindGameObjectWithTag("BlueSpawn") || !GameObject.FindGameObjectWithTag("RedSpawn")))
			{
				Popup("You must add red and blue team spawns");
				return;
			}
			if (mapSets.enableDm || mapSets.enableCtf || bs.isDebug)
			{
				SubmitMap();
			}
			else if (mapSets.race)
			{
				StartCoroutine(StartTest(submit: true));
			}
		}
		GUILayout.EndHorizontal();
	}

	public IEnumerable<CurvySplineSegment> GetAffectBy()
	{
		if (affectNext)
		{
			for (int j = segment.ControlPointIndex; j < segment.Spline.ControlPointCount; j++)
			{
				yield return segment.Spline.ControlPoints[j];
			}
		}
		if (affectPrev)
		{
			for (int i = segment.ControlPointIndex - (affectNext ? 1 : 0); i >= 0; i--)
			{
				yield return segment.Spline.ControlPoints[i];
			}
		}
		if ((affectPrev || affectNext) && segment.Spline2.pivot != null)
		{
			yield return segment.Spline2.pivot;
		}
		if (!affectNext && !affectPrev)
		{
			yield return segment;
		}
	}

	protected void Menu()
	{
	}

	public void OnGUI()
	{
		//IL_0011: Unknown result type (might be due to invalid IL or missing references)
		//IL_001b: Expected O, but got Unknown
		//IL_00c0: Unknown result type (might be due to invalid IL or missing references)
		//IL_00cc: Unknown result type (might be due to invalid IL or missing references)
		//IL_00d6: Expected O, but got Unknown
		//IL_00d6: Expected O, but got Unknown
		//IL_00f4: Unknown result type (might be due to invalid IL or missing references)
		//IL_0100: Unknown result type (might be due to invalid IL or missing references)
		//IL_010a: Expected O, but got Unknown
		//IL_010a: Expected O, but got Unknown
		//IL_0137: Unknown result type (might be due to invalid IL or missing references)
		//IL_0143: Unknown result type (might be due to invalid IL or missing references)
		//IL_014d: Expected O, but got Unknown
		//IL_014d: Expected O, but got Unknown
		//IL_018c: Unknown result type (might be due to invalid IL or missing references)
		//IL_0197: Expected O, but got Unknown
		//IL_01ee: Unknown result type (might be due to invalid IL or missing references)
		//IL_01f9: Expected O, but got Unknown
		if ((MulticastDelegate)(object)bs.win.act != (MulticastDelegate)new Action(OnEditorWindow) && !bs.isDebug)
		{
			return;
		}
		CustomWindow.GUIMatrix(1f);
		GUI.depth = -1;
		GUILayout.BeginHorizontal(string.Empty, base.skin.GetStyle("flow background"));
		if (Button("Start Test"))
		{
			if (shapeEditor)
			{
				EnableShapeEditor(b: false);
			}
			else
			{
				StartCoroutine(StartTest(submit: false));
			}
		}
		if (!hideMenu)
		{
			if (Button("Level Settings"))
			{
				ShowWindow((Action)(object)new Action(LevelSettingsWindow), (Action)(object)new Action(OnEditorWindow));
			}
			if (Button("Save"))
			{
				ShowWindow((Action)(object)new Action(SaveMapWindow), (Action)(object)new Action(OnEditorWindow));
			}
			if (Button("Load"))
			{
				bs._Loader.userMaps.Clear();
				ShowWindow((Action)(object)new Action(LoadMapWindow), (Action)(object)new Action(OnEditorWindow));
				StartCoroutine(bs._Loader.DownloadUserMaps(0, 0, string.Empty));
			}
		}
		else if (Button("Show More Tools"))
		{
			ShowWindow((Action)(object)new Action(OnEditorWindow));
			hideMenu = false;
		}
		if (Button("Clear"))
		{
			Clear();
		}
		if (Button("Reset Camera"))
		{
			ResetCam();
		}
		if (BackButton("Back to menu"))
		{
			ShowWindow((Action)(object)(Action)delegate
			{
				//IL_0054: Unknown result type (might be due to invalid IL or missing references)
				//IL_005f: Expected O, but got Unknown
				Label("Exit level editor?");
				GUILayout.BeginHorizontal();
				if (Button("Yes"))
				{
					BackToMenu();
				}
				if (Button("No"))
				{
					ShowWindow((Action)(object)new Action(OnEditorWindow));
				}
				GUILayout.EndHorizontal();
			});
		}
		GUILayout.EndHorizontal();
		if (Event.current.type == EventType.Repaint && !string.IsNullOrEmpty(GUI.tooltip))
		{
			bs.win.tooltip = GUI.tooltip;
		}
	}

	public void LoadTestMapWindow()
	{
		BeginScrollView();
		foreach (Scene item in ((IEnumerable<Scene>)bs._Loader.scenes).Where((Func<Scene, bool>)((Scene a) => !a.userMap)))
		{
			if (Button(item.name))
			{
				Clear();
				string text = MapLoader.unityMap = (mapName = item.name);
			}
		}
		if (Button("test"))
		{
			Clear();
			string text = MapLoader.unityMap = (mapName = "a02");
		}
		GUILayout.EndScrollView();
	}

	private IEnumerator LoadTutorial()
	{
		tutorialTextures = new List<Texture2D>();
		WWW w2 = new WWW(bs.mainSite + "/docs/tutorial/getFiles.php");
		yield return w2;
		tutorialUrls = bs.SplitString(w2.text);
		string[] array = tutorialUrls;
		for (int i = 0; i < array.Length; i++)
		{
			w2 = new WWW(string.Concat(str2: Uri.EscapeUriString(array[i]), str0: bs.mainSite, str1: "/docs/tutorial/"));
			yield return w2;
			if (w2.texture != null)
			{
				tutorialTextures.Add(w2.texture);
			}
		}
	}

	private void Tutorial()
	{
		//IL_005d: Unknown result type (might be due to invalid IL or missing references)
		//IL_0068: Expected O, but got Unknown
		//IL_0151: Unknown result type (might be due to invalid IL or missing references)
		//IL_015c: Expected O, but got Unknown
		Setup(Screen.width, Screen.height, "Tutorial");
		Label(string.Empty);
		if (tutorialTextures.Count == 0)
		{
			VideoYoutube();
			Label("Loading");
			if (BackButtonLeft())
			{
				ShowWindow((Action)(object)new Action(OnEditorWindow));
			}
			return;
		}
		GUILayout.BeginHorizontal();
		GUILayout.FlexibleSpace();
		if (selectedSlide > 0 && Button("<<Prev"))
		{
			selectedSlide--;
		}
		GUILayout.Label(selectedSlide + 1 + "/" + tutorialUrls.Length + " Slide");
		if (selectedSlide < tutorialTextures.Count - 1 && Button("Next>>"))
		{
			selectedSlide++;
		}
		GUILayout.FlexibleSpace();
		if (GUILayout.Button("Close"))
		{
			ShowWindow((Action)(object)new Action(OnEditorWindow));
		}
		GUILayout.EndHorizontal();
		Texture2D image = tutorialTextures[selectedSlide];
		GUILayout.Label(image, GUILayout.Height(Screen.height - 50));
	}

	private void BackToMenu()
	{
		Application.LoadLevel("!1");
	}

	public void OnEditorWindow()
	{
		if (hideMenu)
		{
			bs.win.CloseWindow();
			return;
		}
		WinSetupScroll();
		if (!shapeEditor && BeginVertical("Road Shapes"))
		{
			if (Button("Road Shape Editor"))
			{
				EnableShapeEditor(b: true);
			}
			foreach (CurvySpline2 shape in shapes)
			{
				List<CurvySpline2> brushShapes = base.brushShapes;
				bool flag = brushShapes.Contains(shape);
				bool flag2 = GUILayout.Toggle(flag, shape.name);
				if (flag2 != flag)
				{
					if (flag2)
					{
						brushShapes.Add(shape);
					}
					else
					{
						brushShapes.Remove(shape);
					}
				}
			}
			GUILayout.EndVertical();
		}
		DrawTools();
		if (BeginVertical("Models"))
		{
			if (!base.modelLib)
			{
				Label("Loading");
			}
			else
			{
				DrawModelView();
			}
			GUILayout.EndVertical();
		}
		if (BeginVertical("Terrain"))
		{
			bool flag3 = GUILayout.Toggle(base.hideTerrain, "Hide Terrain");
			if (flag3 != base.hideTerrain)
			{
				CreateTerrain();
				base.hideTerrain = flag3;
			}
			if (!flag3)
			{
				if (Button("Refresh Terrain"))
				{
					UpdateTerrain(null, refresh: true, showTrees);
				}
				autoRefreshTerrain = Toggle(autoRefreshTerrain, "Auto Refresh Terrain");
				if (bs.isDebug)
				{
					showTrees = GUILayout.Toggle(showTrees, "Show Trees");
				}
			}
			GUILayout.EndVertical();
		}
		TutorialHelp();
		GUILayout.EndArea();
	}

	private void WinSetupScroll()
	{
		int num = (!bs.android) ? 250 : 170;
		bs.win.Setup(num, 1000, string.Empty, Dock.Left, null, null, 1.3f);
		if (windowHit && mouseScroll != 0f)
		{
			windowScroll = Mathf.Min(0f, windowScroll + mouseScroll * 600f);
		}
		GUILayout.BeginArea(new Rect(0f, windowScroll + 30f, num - 2, 1000f));
	}

	private void DrawTools()
	{
		if (!BeginVertical("Road Tools"))
		{
			return;
		}
		if (!shapeEditor)
		{
			drawRoad = Toggle(drawRoad, "Draw Road and");
		}
		Tool tool = (Tool)Toolbar((int)this.tool, shapeEditor ? toolStrsShape : ((!bs.android) ? toolStrs : toolStrsAndroid), expand: true, center: false, 99, 2);
		if (this.tool != tool && tool == Tool.Brush)
		{
			ToggleTab("Road Shapes");
		}
		this.tool = tool;
		if (!base.hideTerrain)
		{
			flying = Toggle(flying, "In Air");
			if (segment != null)
			{
				segment.flying = flying;
			}
		}
		affectPrev = Toggle(affectPrev, "Affect to Left Side");
		affectNext = Toggle(affectNext, "Affect to Right Side");
		GUILayout.EndVertical();
	}

	private void LevelSettingsWindow()
	{
		//IL_002f: Unknown result type (might be due to invalid IL or missing references)
		//IL_0043: Expected O, but got Unknown
		BeginScrollView();
		if (Button("Game Settings"))
		{
			bs._Loader.ShowWindow((Action)(object)new Action(bs._Loader.SettingsWindow), bs.win.act);
		}
		Label(Trs("Laps:") + laps);
		laps = (int)GUILayout.HorizontalSlider(laps, 1f, 10f);
		Label(Trs("Nitro:") + nitro);
		nitro = (int)GUILayout.HorizontalSlider(nitro, 1f, 10f);
		Label(Trs("AntiFly:") + bs._GameSettings.gravitationFactor);
		bs._GameSettings.gravitationFactor = GUILayout.HorizontalSlider(bs._GameSettings.gravitationFactor, 0f, 3f);
		Label(Trs("Gravity:") + bs._GameSettings.gravitationAntiFly);
		bs._GameSettings.gravitationAntiFly = GUILayout.HorizontalSlider(bs._GameSettings.gravitationAntiFly, 1f, 3f);
		LabelCenter("Game Type:" + mapSets.levelFlags);
		GUILayout.BeginHorizontal();
		GUILayout.EndHorizontal();
		if (bs.isDebug)
		{
			Toggle(mapSets.usedAdvancedTools, "Advanced Tools");
		}
		GUILayout.EndScrollView();
	}

	private void TutorialHelp()
	{
		//IL_00c8: Unknown result type (might be due to invalid IL or missing references)
		//IL_00d3: Expected O, but got Unknown
		if (BeginVertical("Help"))
		{
			if (bs.android)
			{
				VideoYoutube();
			}
			if (!bs.android)
			{
				base.skin.label.alignment = TextAnchor.UpperLeft;
				base.skin.label.wordWrap = true;
				base.skin.label.fontSize = 11;
				if (!string.IsNullOrEmpty(bs.win.tooltip))
				{
					GUILayout.Label(bs.win.tooltip);
				}
				else
				{
					VideoYoutube();
					if (Button("View tutorial"))
					{
						if (tutorialTextures == null)
						{
							StartCoroutine(LoadTutorial());
						}
						ShowWindow((Action)(object)new Action(Tutorial));
					}
					GUILayout.Label(GuiClasses.Tr((!shapeEditor) ? "editorhelp" : "shapeeditorhelp"));
					GUILayout.Label(Tp(toolStrsAndroid[(int)tool]));
				}
			}
			GUILayout.EndVertical();
		}
		GUILayout.FlexibleSpace();
	}

	private void UpdateHeight(float height, bool scroll = false)
	{
		Vector3 vector = Vector3.up * height * 4f;
		pos += vector;
		cursorPos = (cursor.position += vector);
		if (scroll)
		{
			return;
		}
		foreach (CurvySplineSegment item in GetAffectBy())
		{
			if (item != null)
			{
				item.Position += vector;
				item.Spline.Refresh();
			}
		}
	}

	protected void UpdateRotation()
	{
	}

	private void UpdateSwirl()
	{
		segment.swirl = swirl;
		UpdateSwirl2();
		segment.Spline.Refresh();
	}

	private void UpdateScale(float dt)
	{
		foreach (CurvySplineSegment item in GetAffectBy())
		{
			Vector3 localScale = item.transform.localScale;
			item.scale = localScale.x + dt;
		}
		segment.Spline.Refresh();
	}

	public void LoadMapWindow()
	{
		//IL_008f: Unknown result type (might be due to invalid IL or missing references)
		//IL_00a3: Expected O, but got Unknown
		GUILayout.BeginHorizontal();
		search = GUILayout.TextField(search);
		if (Button("Search"))
		{
			StartCoroutine(bs._Loader.DownloadUserMaps(0, (int)bs._Loader.mapSets.levelFlags, search));
		}
		GUILayout.EndHorizontal();
		BeginScrollView();
		if (Button("Standart Maps"))
		{
			ShowWindow((Action)(object)new Action(LoadTestMapWindow), bs.win.act);
		}
		if (Button("Sample Map"))
		{
			Popup2("Loading Map");
			mapName = "primer1";
			MapLoader.loadMap = "usermaps/soulkey.primer1.map";
			Application.LoadLevel("level");
		}
		foreach (Scene userMap in bs._Loader.userMaps)
		{
			if (Button(userMap.name))
			{
				Debug.LogWarning("Loading Map\n" + userMap.title + "\n" + userMap.url);
				Popup2("Loading Map\n" + userMap.title + "\n" + userMap.url);
				mapName = userMap.title;
				MapLoader.loadMap = userMap.url;
				Application.LoadLevel("level");
			}
		}
		GUILayout.EndScrollView();
	}

	private IEnumerator StartTest(bool submit)
	{
		bs._Loader.gameType = GameType.race;
		tool = Tool.Height;
		UpdateModelView();
		if (submit && bs.checkPoints.Length < 2)
		{
			Popup("You need to put at least 2 CheckPoints", (Action)(object)new Action(OnEditorWindow));
			yield break;
		}
		starting = true;
		if (submit)
		{
			UpdateTerrain(null, refresh: true, showTrees);
		}
		submitMapPublish = submit;
		bs.win.CloseWindow();
		bs._Loader.replays.Clear();
		bs._GameSettings.laps = laps;
		bs._Loader.night = (bs._Loader.rain = false);
		bs._Loader.mapName = bs._Loader.playerNamePrefixed + "." + mapName;
		Time.timeScale = 1f;
		yield return StartCoroutine(ActiveEditor(editor: false));
		if (bs._Game == null)
		{
			LoadLevelAdditive("!2game");
		}
		else
		{
			ReEnable();
			bs._Player.cam.gameObject.SetActive(value: true);
			bs._Game.SetStartPoint();
			bs._Game.Reset();
			if (submit || bs._Game.finnish || !bs._Game.started)
			{
				bs._Game.started = true;
				bs._Game.RestartLevel();
			}
		}
		base.gameObject.SetActive(value: false);
		starting = false;
	}

	public IEnumerator Resume()
	{
		if ((bs._Game.finnish || bs.isDebug) && submitMapPublish && (bs._Game.timeElapsed > 20f || bs.isDebug))
		{
			bool yes = false;
			bool no = false;
			ShowWindow((Action)(object)(Action)delegate
			{
				Label("Do you want to publish map right now?");
				GUILayout.BeginHorizontal();
				no = Button("No");
				yes = Button("Yes");
				GUILayout.EndHorizontal();
			});
			while (!no && !yes)
			{
				yield return null;
			}
			if (yes)
			{
				SubmitMap();
				yield break;
			}
		}
		yield return null;
		Time.timeScale = 0f;
		ShowWindow((Action)(object)new Action(OnEditorWindow));
		bs._Player.cam.gameObject.SetActive(value: false);
		Disable(typeof(GameSettings));
		Disable(typeof(Player));
		Disable(typeof(Game));
		Disable(typeof(Hud));
		base.gameObject.SetActive(value: true);
		StartCoroutine(ActiveEditor(editor: true));
		if (submitMapPublish && bs._Game.finnish)
		{
			Popup("Your map is too short for publishing");
		}
	}

	private void SubmitMap()
	{
		//IL_006b: Unknown result type (might be due to invalid IL or missing references)
		//IL_0077: Expected O, but got Unknown
		bs._Awards.LevelCreator.count++;
		bs._Loader.mapName = bs._Loader.playerNamePrefixed + "." + mapName;
		bs._Loader.AproveMap(1, mapSets.usedAdvancedTools ? 1 : 0);
		Popup("Map published ", (Action)(object)(Action)delegate
		{
			BackToMenu();
		});
	}

	private void Reset()
	{
		if (segment != null && segment.Spline.ControlPointCount < ((!segment.Spline2.shape) ? 2 : 3))
		{
			Remove(segment.gameObject);
		}
		spline = null;
		segment = null;
		affectPrev = (affectNext = false);
		MonoBehaviour.print("Reset");
	}

	protected void Destroy()
	{
	}

	private void Remove(GameObject o)
	{
		CurvySplineSegment component = o.GetComponent<CurvySplineSegment>();
		if (component.Spline2.pivot == component)
		{
			return;
		}
		if (enableClosed2 && (component.IsFirstSegment || component.IsLastSegment) && component.Spline2.Closed && !component.Spline2.shape)
		{
			component.Spline2.Closed = false;
			return;
		}
		CurvySplineSegment curvySplineSegment = (!(component.PreviousSegment == null)) ? component.PreviousSegment : component.NextSegment;
		segment = ((!(component.NextControlPoint == null)) ? component.NextControlPoint : component.PreviousControlPoint);
		if (component.Spline.Segments.Count <= ((!shapeEditor) ? 1 : 2))
		{
			MonoBehaviour.print("Destr");
			UnityEngine.Object.Destroy(component.Spline.gameObject);
		}
		else
		{
			component.Spline.Delete(component);
		}
		if (segment != null && segment.NextControlPoint != null && segment.PreviousControlPoint != null)
		{
			segment = null;
		}
		if (curvySplineSegment != null)
		{
			foreach (SplinePathMeshBuilder sb in curvySplineSegment.sbs)
			{
				sb.Refresh();
			}
			curvySplineSegment.Spline2.Refresh();
			UpdateTerrain(curvySplineSegment);
		}
		if (segment != null)
		{
			CopyFrom(segment, segment.Position);
		}
	}

	private Vector3 GetPoint()
	{
		new Plane(Vector3.up, cursor.position).Raycast(ray, out float enter);
		Vector3 point = ray.GetPoint(enter);
		if (shapeEditor)
		{
			point.x -= bs.Mod(point.x + 0.5f, 1f) - 0.5f;
			point.z -= bs.Mod(point.z + 0.5f, 1f) - 0.5f;
		}
		return point;
	}

	public void OnPostRender()
	{
		bs.res.lineMaterialYellow.SetPass(0);
		GL.Color(Color.green);
		GL.Begin(1);
		if (drawDragRect)
		{
			Vector3 mousePosition = Input.mousePosition;
			Vector3 vector = new Vector3(mouseDrag.x / (float)Screen.width, mouseDrag.y / (float)Screen.height, 10f);
			Vector3 vector2 = new Vector3(mousePosition.x / (float)Screen.width, mousePosition.y / (float)Screen.height, 10f);
			int num = 20;
			GL.Vertex(camera.ViewportToWorldPoint(new Vector3(vector.x, vector.y, num)));
			GL.Vertex(camera.ViewportToWorldPoint(new Vector3(vector2.x, vector.y, num)));
			GL.Vertex(camera.ViewportToWorldPoint(new Vector3(vector2.x, vector.y, num)));
			GL.Vertex(camera.ViewportToWorldPoint(new Vector3(vector2.x, vector2.y, num)));
			GL.Vertex(camera.ViewportToWorldPoint(new Vector3(vector2.x, vector2.y, num)));
			GL.Vertex(camera.ViewportToWorldPoint(new Vector3(vector.x, vector2.y, num)));
			GL.Vertex(camera.ViewportToWorldPoint(new Vector3(vector.x, vector2.y, num)));
			GL.Vertex(camera.ViewportToWorldPoint(new Vector3(vector.x, vector.y, num)));
		}
		GL.Vertex(cursorPos);
		GL.Vertex(cursorPos + Vector3.down * 100f);
		if (np != null && (tool == Tool.Insert || tool == Tool.Brush))
		{
			DrawLine2D(cursorPos, np.point);
		}
		if (alt)
		{
			GL.Color(Color.white);
			DrawLine(cursorPos, rotateCamPivot);
		}
		else if (segment != null && draw && !windowHit)
		{
			GL.Color(Color.white);
			DrawLine(cursorPos, segment.Position);
		}
		GL.End();
	}

	private void ReEnable()
	{
		foreach (GameObject mb in mbs)
		{
			if (mb != null)
			{
				mb.SetActive(value: true);
			}
		}
	}

	private void Disable(Type type)
	{
		UnityEngine.Object[] array = UnityEngine.Object.FindObjectsOfType(type);
		for (int i = 0; i < array.Length; i++)
		{
			MonoBehaviour monoBehaviour = (MonoBehaviour)array[i];
			monoBehaviour.gameObject.SetActive(value: false);
			mbs.Add(monoBehaviour.gameObject);
		}
	}

	public void ModeViewStart()
	{
		folderStype = buttonSetup(bs.win.skinDefault.button, 130f);
		if ((bool)base.modelLib)
		{
			GameObject gameObj = base.modelLib.models[0].gameObj;
			sgo = MapLoader.InitModel((GameObject)UnityEngine.Object.Instantiate((UnityEngine.Object)gameObj), gameObj.name);
		}
		grid = new GameObject("Grid").AddComponent<Grid>();
		grid.renderAwalys = true;
		grid.enabled = false;
		grid.cellBig = 0f;
	}

	public void UpdateModelView()
	{
		grid.enabled = (tool == Tool.Models && showGrid);
		grid.cellSmall = 1.75f;
		grid.cellBig = 0f;
		grid.pos = ModPos(cursor.position);
		grid.cellSmallColor = new Color(0f, 1f, 0f, 0.5f);
		if ((bool)sgo)
		{
			sgo.gameObject.SetActive(tool == Tool.Models && tool2 == Tool2.Draw);
		}
		if ((tool != Tool.Models || tool2 != Tool2.Duplicate) && duplicate != null)
		{
			UnityEngine.Object.Destroy(duplicate.gameObject);
		}
		if (Input.GetKeyDown(KeyCode.T))
		{
			Vector3 position = camera.transform.position;
			Vector3 position2 = cursor.position;
			position.x = position2.x;
			Vector3 position3 = cursor.position;
			position.z = position3.z;
			camera.transform.position = position;
			camera.transform.eulerAngles = new Vector3(90f, 0f, 0f);
		}
		if (tool != Tool.Models)
		{
			return;
		}
		if (mouseButtonDownAny)
		{
			mouseDrag = Input.mousePosition;
		}
		drawDragRect = false;
		if (tool != Tool.Models || !(sgo != null))
		{
			return;
		}
		Vector3 p = cursor.position - bs.ZeroY(sgo.renderer.bounds.center - sgo.transform.position, 0f);
		p = ModPos(p);
		sgo.transform.position = p + modelViewOffset;
		Ray ray = camera.ScreenPointToRay(mpos);
		RaycastHit hitInfo;
		bool flag = Physics.Raycast(ray, out hitInfo, 1000f, 1 << Layer.block);
		ModelObject modelObject = (!flag) ? null : hitInfo.transform.root.GetComponent<ModelObject>();
		if (tool2 == Tool2.Move)
		{
			if (mouseButtonDown0)
			{
				startDrag = ModPos(cursor.position);
			}
			if (mouseButton0)
			{
				Vector3 a = startDrag - ModPos(cursor.position);
				foreach (ModelObject item in selection)
				{
					item.transform.Translate(-a, Space.World);
					item.initPos = item.pos - modelViewOffset;
				}
				startDrag = ModPos(cursor.position);
			}
		}
		if (tool2 == Tool2.Select)
		{
			Vector3 mousePosition = Input.mousePosition;
			if (mouseButton0)
			{
				drawDragRect = true;
			}
			if (mouseButtonUp0)
			{
				if (!Input.GetKey(KeyCode.LeftControl))
				{
					ClearSelection();
				}
				if (flag && (mousePosition - mouseDrag).magnitude < 5f)
				{
					SelectObject(modelObject);
				}
			}
			if (mouseButtonUp0)
			{
				UnityEngine.Object[] array = UnityEngine.Object.FindObjectsOfType(typeof(ModelObject));
				for (int i = 0; i < array.Length; i++)
				{
					ModelObject modelObject2 = (ModelObject)array[i];
					Vector3 point = camera.WorldToScreenPoint(modelObject2.renderer.bounds.center);
					if (Rect.MinMaxRect(Mathf.Min(mouseDrag.x, mousePosition.x), Mathf.Min(mouseDrag.y, mousePosition.y), Mathf.Max(mousePosition.x, mouseDrag.x), Mathf.Max(mousePosition.y, mouseDrag.y)).Contains(point))
					{
						SelectObject(modelObject2);
					}
				}
			}
			if (selection.Count > 0)
			{
				Vector3 pos = selection[0].pos;
				float x = pos.x % 1.75f;
				Vector3 pos2 = selection[0].pos;
				float y = pos2.y % 1.75f;
				Vector3 pos3 = selection[0].pos;
				modelViewOffset = new Vector3(x, y, pos3.z % 1.75f);
			}
			if (Input.GetKeyDown(KeyCode.Delete))
			{
				Delete();
			}
		}
		if (mouseButtonUp1 && flag && (Input.mousePosition - mouseDrag).magnitude < 3f)
		{
			MonoBehaviour.print((Input.mousePosition - mouseDrag).magnitude);
			UnityEngine.Object.Destroy(modelObject.gameObject);
		}
		if (tool2 != Tool2.Select && tool2 != Tool2.Move && selection.Count > 0)
		{
			ClearSelection();
		}
		if (tool2 == Tool2.Duplicate && duplicate != null)
		{
			duplicate.position = ModPos(cursor.position);
			if (mouseButtonDown0)
			{
				Transform transform = (Transform)UnityEngine.Object.Instantiate((UnityEngine.Object)duplicate);
				ModelObject[] componentsInChildren = transform.GetComponentsInChildren<ModelObject>();
				foreach (ModelObject modelObject3 in componentsInChildren)
				{
					modelObject3.ResetColor();
				}
				transform.DetachChildren();
				UnityEngine.Object.Destroy(transform.gameObject);
			}
		}
		if (tool2 == Tool2.Draw && mouseButtonDown0)
		{
			mapSets.usedAdvancedTools = true;
			ModelObject modelObject4 = lastSgo = (ModelObject)UnityEngine.Object.Instantiate((UnityEngine.Object)sgo, sgo.transform.position, sgo.transform.rotation);
			modelObject4.ResetColor();
			if (modelObject4.collider != null)
			{
				modelObject4.collider.enabled = true;
			}
			modelObject4.initPos = sgo.transform.position;
		}
	}

	private static Vector3 ModPos(Vector3 p)
	{
		float num = 0.875f;
		p.x -= bs.Mod(p.x + num, num * 2f) - num;
		p.z -= bs.Mod(p.z + num, num * 2f) - num;
		p.y -= bs.Mod(p.y + num, num * 2f) - num;
		return p;
	}

	private static Vector3 ModPos2(Vector3 p)
	{
		float num = 0.875f;
		p.x -= bs.Mod(p.x, num * 2f);
		p.z -= bs.Mod(p.z, num * 2f);
		p.y -= bs.Mod(p.y, num * 2f);
		return p;
	}

	private void Delete()
	{
		foreach (ModelObject item in selection)
		{
			UnityEngine.Object.Destroy(item.gameObject);
		}
		selection.Clear();
	}

	private void SelectObject(ModelObject hitedObject)
	{
		if (!selection.Remove(hitedObject))
		{
			selection.Add(hitedObject);
			hitedObject.SetColor(Color.Lerp(Color.white, Color.green, 0.2f));
		}
		else
		{
			hitedObject.ResetColor();
		}
	}

	private void ClearSelection()
	{
		foreach (ModelObject item in selection)
		{
			item.ResetColor();
		}
		selection.Clear();
	}

	private static void splitGui(int j)
	{
		if (j % 4 == 0)
		{
			GUILayout.EndHorizontal();
			GUILayout.BeginHorizontal();
		}
	}

	private GUIStyle buttonSetup(GUIStyle guiStyle, float width)
	{
		GUIStyle gUIStyle = new GUIStyle(guiStyle);
		gUIStyle.wordWrap = true;
		gUIStyle.alignment = TextAnchor.LowerCenter;
		gUIStyle.imagePosition = ImagePosition.ImageAbove;
		gUIStyle.fixedWidth = width;
		gUIStyle.fixedHeight = width;
		return gUIStyle;
	}

	private void DrawFile(ModelFile file, bool big = true)
	{
		if (GUILayout.Button(new GUIContent((!big) ? null : file.name, file.thumb), buttonSetup(base.skin.button, (!big) ? 65 : 130)))
		{
			selectedGameObject = file.gameObj;
			if ((bool)sgo)
			{
				UnityEngine.Object.Destroy(sgo.gameObject);
			}
			recent.Remove(file);
			recent.Insert(0, file);
			if (recent.Count > 8)
			{
				recent.RemoveAt(recent.Count - 1);
			}
			lastSgo = null;
			sgo = MapLoader.InitModel((GameObject)UnityEngine.Object.Instantiate((UnityEngine.Object)selectedGameObject), selectedGameObject.name);
			sgo.oldMaterials = sgo.renderer.sharedMaterials;
			if (sgo.collider != null)
			{
				sgo.collider.enabled = false;
			}
			sgo.SetColor(Color.white);
			tool2 = Tool2.Draw;
			tool = Tool.Models;
			bs.win.Back();
		}
	}

	private IEnumerable<ModelFile> GetFiles(ModelItem cur)
	{
		foreach (ModelFile file in cur.files)
		{
			yield return file;
		}
		foreach (ModelItem a in cur.dirs)
		{
			foreach (ModelFile file2 in GetFiles(a))
			{
				yield return file2;
			}
		}
	}

	private void ModelPick()
	{
		int num = 0;
		Setup(600, 600, string.Empty);
		Label("Search:");
		modelSearch = GUILayout.TextField(modelSearch);
		if (stack.Count > 0 && GUILayout.Button(".. Back", GUILayout.ExpandWidth(expand: false)))
		{
			if (string.IsNullOrEmpty(modelSearch))
			{
				modelLibCur = stack.Pop();
			}
			else
			{
				modelSearch = string.Empty;
			}
		}
		modelLibCur.scroll = GUILayout.BeginScrollView(modelLibCur.scroll);
		if (recent.Count > 0)
		{
			GUILayout.BeginHorizontal();
			foreach (ModelFile item in recent)
			{
				DrawFile(item, big: false);
			}
			GUILayout.EndHorizontal();
			GUILayout.Space(10f);
		}
		GUILayout.BeginHorizontal();
		if (!string.IsNullOrEmpty(modelSearch))
		{
			string[] sr = modelSearch.ToLower().Split(' ');
			ModelFile[] array = GetFiles(modelLibCur).Where((Func<ModelFile, bool>)((ModelFile a) => sr.Any((string b) => a.name.ToLower().Contains(b)))).ToArray();
			ModelFile[] array2 = array;
			foreach (ModelFile file in array2)
			{
				splitGui(num++);
				DrawFile(file);
			}
		}
		else
		{
			foreach (ModelItem dir in modelLibCur.dirs)
			{
				if (dir.dirs.Count > 0 || dir.files.Count > 0)
				{
					splitGui(num++);
					if (GUILayout.Button(new GUIContent(dir.Name, dir.FolderTexture), folderStype))
					{
						stack.Push(modelLibCur);
						modelLibCur = dir;
					}
				}
			}
			foreach (ModelFile file2 in modelLibCur.files)
			{
				splitGui(num++);
				DrawFile(file2);
			}
		}
		GUILayout.EndHorizontal();
		GUILayout.EndScrollView();
	}

	public void DrawModelView()
	{
		//IL_003e: Unknown result type (might be due to invalid IL or missing references)
		//IL_0053: Expected O, but got Unknown
		if (GUILayout.Button(GuiClasses.Tr("Pick Model") + " (P)") || Input.GetKeyDown(KeyCode.P))
		{
			bs.win.ShowWindow((Action)(object)new Action(ModelPick), bs.win.act, skip: true);
		}
		tool2 = (Tool2)Toolbar((int)((tool != Tool.Models) ? ((Tool2)(-1)) : tool2), new string[4]
		{
			"Draw",
			"Select2",
			null,
			(selection.Count <= 0) ? null : "Move"
		}, expand: false);
		if (tool2 != (Tool2)(-1))
		{
			tool = Tool.Models;
		}
		if (tool != Tool.Models)
		{
			return;
		}
		ModelObject modelObject = (tool2 != Tool2.Select && tool2 != Tool2.Move) ? sgo : selection.FirstOrDefault();
		if (selection.Count > 0)
		{
			GUILayout.Label(selection[0].name2);
			if (Button("Duplicate"))
			{
				if (selection.Count == 1)
				{
					sgo = (ModelObject)UnityEngine.Object.Instantiate((UnityEngine.Object)selection[0]);
					sgo.SetColor(Color.white);
					tool2 = Tool2.Draw;
				}
				else
				{
					Bounds bounds = default(Bounds);
					foreach (ModelObject item in selection)
					{
						bounds.Encapsulate(item.renderer.bounds);
					}
					GameObject gameObject = new GameObject("Duplicate");
					duplicate = gameObject.transform;
					duplicate.position = selection[0].pos;
					for (int i = 0; i < selection.Count; i++)
					{
						ModelObject modelObject2 = (ModelObject)UnityEngine.Object.Instantiate((UnityEngine.Object)selection[i]);
						modelObject2.transform.parent = duplicate;
					}
					tool2 = Tool2.Duplicate;
				}
				Vector3 position = cursor.position;
				Vector3 pos = selection[0].pos;
				position.y = pos.y;
				cursor.position = position;
			}
			if (Button("Delete"))
			{
				Delete();
			}
		}
		showGrid = Toggle(showGrid, "Show Grid");
		if (tool2 == Tool2.Duplicate)
		{
			DrawControls(duplicate);
		}
		if (modelObject != null && modelObject.gameObject.activeSelf)
		{
			DrawControls(modelObject.transform);
		}
	}

	private void DrawControls(Transform sgo)
	{
		GUILayout.BeginHorizontal();
		if (Button("Rotate Left"))
		{
			sgo.transform.Rotate(Vector3.up, -45f, Space.World);
		}
		if (Button("Rotate Right"))
		{
			sgo.transform.Rotate(Vector3.up, 45f, Space.World);
		}
		GUILayout.EndHorizontal();
		GUILayout.BeginHorizontal();
		if (GUILayout.Button("X-"))
		{
			sgo.transform.Rotate(-45f, 0f, 0f, Space.Self);
		}
		if (GUILayout.Button("X+"))
		{
			sgo.transform.Rotate(45f, 0f, 0f, Space.Self);
		}
		if (GUILayout.Button("Y-"))
		{
			sgo.transform.Rotate(0f, -45f, 0f, Space.Self);
		}
		if (GUILayout.Button("Y+"))
		{
			sgo.transform.Rotate(0f, 45f, 0f, Space.Self);
		}
		if (GUILayout.Button("Z-"))
		{
			sgo.transform.Rotate(0f, 0f, -45f, Space.Self);
		}
		if (GUILayout.Button("Z+"))
		{
			sgo.transform.Rotate(0f, 0f, 45f, Space.Self);
		}
		GUILayout.EndHorizontal();
		int num = (int)Mathf.Pow(2f, scalePow);
		if (Button(Trs("Scale:") + num))
		{
			scalePow = (scalePow + 1f) % 2f;
		}
		sgo.transform.localScale = num * Vector3.one;
		if (!BeginVertical("Offset"))
		{
			return;
		}
		Vector3 b = modelViewOffset;
		if (Button("Reset Offset"))
		{
			modelViewOffset = Vector3.zero;
		}
		Label("x:" + (int)(modelViewOffset.x * 100f));
		modelViewOffset.x = GUILayout.HorizontalSlider(modelViewOffset.x, -3.5f, 3.5f);
		Label("y:" + (int)(modelViewOffset.y * 100f));
		modelViewOffset.y = GUILayout.HorizontalSlider(modelViewOffset.y, -3.5f, 3.5f);
		Label("z:" + (int)(modelViewOffset.z * 100f));
		modelViewOffset.z = GUILayout.HorizontalSlider(modelViewOffset.z, -3.5f, 3.5f);
		sgo.position += modelViewOffset - b;
		foreach (ModelObject item in ((IEnumerable<ModelObject>)selection).Where((Func<ModelObject, bool>)((ModelObject a) => a.transform != sgo)))
		{
			item.pos += modelViewOffset - b;
		}
		if (lastSgo != null && tool2 == Tool2.Draw)
		{
			lastSgo.pos += modelViewOffset - b;
		}
		GUILayout.EndVertical();
	}

	private void EnableShapeEditor(bool b)
	{
		//IL_0015: Unknown result type (might be due to invalid IL or missing references)
		//IL_0020: Expected O, but got Unknown
		//IL_002d: Unknown result type (might be due to invalid IL or missing references)
		//IL_0038: Expected O, but got Unknown
		drawRoad = true;
		if (b)
		{
			ShowWindow((Action)(object)new Action(ShapeGuiWindow));
		}
		else
		{
			ShowWindow((Action)(object)new Action(OnEditorWindow));
		}
		if (!b)
		{
			foreach (CurvySpline2 spline2 in splines)
			{
				spline2.GenerateMesh();
			}
		}
		if (b)
		{
			tool = Tool.Move;
		}
		shapeEditor = b;
		shapeCamera.enabled = b;
		defCamera.enabled = !b;
		camera = ((!b) ? defCamera : shapeCamera);
		if (b)
		{
			cursor.transform.position = Vector3.up * 10f;
		}
		Reset();
	}

	public void ShapeGuiWindow()
	{
		//IL_0171: Unknown result type (might be due to invalid IL or missing references)
		//IL_0185: Expected O, but got Unknown
		GuiClasses.cnt = 10;
		WinSetupScroll();
		Label(string.Empty);
		Label("ShapeEditor");
		if (BackButtonLeft())
		{
			EnableShapeEditor(b: false);
		}
		if (spline != null)
		{
			if (BeginVertical("Shape Settings"))
			{
				Label("Shape Name:");
				spline.name = GUILayout.TextField(spline.name);
				Label("Shape Terrain Height:" + Mathf.Round(spline.heightOffset));
				spline.heightOffset = GUILayout.HorizontalSlider(spline.heightOffset, -10f, 10f);
				if (roadTypes == null)
				{
					roadTypes = Enum.GetNames(typeof(RoadType));
				}
				spline.roadType = (RoadType)GUILayout.Toolbar((int)spline.roadType, roadTypes);
				if (bs._Loader.thumbnails.Count > 0)
				{
					GUILayout.EndVertical();
				}
			}
			if (BeginVertical("Texture"))
			{
				if (Button("Pick Texture"))
				{
					ShowWindow((Action)(object)new Action(PickTexture), bs.win.act);
				}
				if (spline.thumb != null)
				{
					spline.wallTexture = Toggle(spline.wallTexture, "Wall Texture");
					spline.rotateTexture = Toggle(spline.rotateTexture, "Rotate Texture");
					Vector2 tile = spline.thumb.tile;
					Label(Trs("horizontal tile:") + Math.Round(tile.x, 2));
					tile.x = 10f - GUILayout.HorizontalSlider(10f - tile.x, 0f, 9.9f);
					Label(Trs("vertical tile:") + Math.Round(tile.y, 2));
					tile.y = 10f - GUILayout.HorizontalSlider(10f - tile.y, 0f, 9.9f);
					spline.thumb.tile = tile;
				}
				GUILayout.EndVertical();
			}
		}
		if (BeginVertical("Show/Hide"))
		{
			foreach (CurvySpline2 shape in shapes)
			{
				shape.hide = !GUILayout.Toggle(!shape.hide, shape.name);
			}
			GUILayout.EndVertical();
		}
		DrawTools();
		TutorialHelp();
		GUILayout.EndArea();
	}

	private void PickTexture()
	{
		Setup(800, 700, string.Empty);
		BeginScrollView(null, showHorizontal: true);
		curFolder = Toolbar(curFolder, bs._Loader.thumbnailKeys, expand: true, center: false, 99, 1);
		GUIStyle gUIStyle = new GUIStyle(base.skin.button);
		gUIStyle.fixedHeight = 150f;
		gUIStyle.fixedWidth = 150f;
		GUIStyle gUIStyle2 = gUIStyle;
		int num = 0;
		GUILayout.BeginHorizontal();
		foreach (Thumbnail item in bs._Loader.thumbnails[bs._Loader.thumbnailKeys[curFolder]])
		{
			if (num % 5 == 0)
			{
				GUILayout.EndHorizontal();
				GUILayout.BeginHorizontal();
			}
			num++;
			Texture2D background = gUIStyle2.normal.background;
			gUIStyle2.normal.background = ((item != spline.thumb) ? gUIStyle2.normal.background : gUIStyle2.active.background);
			if (GUILayout.Button(item.material.mainTexture, gUIStyle2))
			{
				spline.thumb = item;
				bs.win.Back();
			}
			gUIStyle2.normal.background = background;
		}
		GUILayout.EndHorizontal();
		GUILayout.EndScrollView();
	}
}
