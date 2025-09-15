using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using UnityEngine;

public class MapLoader : GuiClasses
{
	public struct Vector5
	{
		public Vector3 v;

		public float dist;

		public float width;

		public Vector5(float x, float y, float z, float dist, float width)
		{
			v = new Vector3(x, y, z);
			this.dist = dist;
			this.width = width;
		}
	}

	public Transform water;

	public Terrain terrain;

	internal float nitro;

	public Light mylight;

	public static string loadMap;

	internal bool userMapSucces;

	internal int laps = 1;

	internal int materialId;

	public TreeInstance[] oldTreeInstances;

	public List<Vector2> minimap = new List<Vector2>();

	private static Vector2 miniMapCenter = new Vector2(0.5f, 0.7f);

	private Vector3 ratio;

	public bool OneMesh;

	internal List<GameObject> nodes = new List<GameObject>();

	internal bool showTrees;

	private List<Collider> cls = new List<Collider>();

	internal bool mapLoading;

	internal ModelItem modelLibCur;

	private ModelLibrary m_modelLib;

	public static string unityMap;

	internal bool finnish;

	private int shapeCnt = 1;

	internal WWW www;

	internal Transform start;

	internal float swirl;

	internal float scale = 1f;

	internal Transform drag;

	public bool shapeEditor;

	internal bool affectNext;

	internal bool affectPrev;

	public List<CurvySpline2> shapes = new List<CurvySpline2>();

	public List<CurvySpline2> brushShapes = new List<CurvySpline2>();

	internal CurvySplineSegment segment;

	internal CurvySpline2 spline;

	private float[,] heights;

	private float[,,] alphas;

	public List<CurvySpline2> splines = new List<CurvySpline2>();

	public float miny = float.MaxValue;

	private bool[,] used;

	private bool warkingShown;

	internal bool hideTerrain
	{
		get
		{
			return terrain == null || !terrain.enabled || !terrain.gameObject.activeSelf;
		}
		set
		{
			if (!(terrain == null))
			{
				Collider collider = ((Component)terrain).get_collider();
				bool enabled = !value;
				terrain.enabled = enabled;
				collider.enabled = enabled;
			}
		}
	}

	public TerrainData td => terrain.terrainData;

	internal static Bounds levelBounds
	{
		get
		{
			return bs._GameSettings.levelBounds;
		}
		set
		{
			bs._GameSettings.levelBounds = value;
		}
	}

	internal ModelLibrary modelLib
	{
		get
		{
			if (m_modelLib == null)
			{
				GameObject gameObject = (GameObject)bs.LoadRes("ModelLibrary");
				if (gameObject != null)
				{
					m_modelLib = gameObject.GetComponent<ModelLibrary>();
					modelLibCur = m_modelLib.RootItem;
				}
			}
			return m_modelLib;
		}
	}

	public void UpdateTerrain(CurvySplineSegment segment, bool refresh = false, bool refreshTrees = false)
	{
		//IL_0996: Unknown result type (might be due to invalid IL or missing references)
		//IL_09a0: Expected O, but got Unknown
		if ((!refresh && bs._Loader.levelEditor != null && !bs._Loader.levelEditor.autoRefreshTerrain) || bs.lowQualityAndAndroid)
		{
			return;
		}
		UnityEngine.Random.seed = 0;
		if (hideTerrain || (segment == null && !refresh))
		{
			return;
		}
		if (refresh)
		{
			((TerrainHelper)UnityEngine.Object.FindObjectOfType(typeof(TerrainHelper))).OnDisable();
		}
		Debug.Log("update Terrain " + refresh + " " + refreshTrees);
		if (heights == null || refresh)
		{
			heights = td.GetHeights(0, 0, td.heightmapHeight, td.heightmapWidth);
			alphas = td.GetAlphamaps(0, 0, td.alphamapHeight, td.alphamapWidth);
			if (miny == float.MaxValue)
			{
				for (int i = 0; i < td.heightmapHeight; i++)
				{
					for (int j = 0; j < td.heightmapWidth; j++)
					{
						miny = Mathf.Min(heights[i, j], miny);
					}
				}
			}
		}
		List<Vector5> list = new List<Vector5>();
		UnityEngine.Object[] array = UnityEngine.Object.FindObjectsOfType(typeof(CurvySpline2));
		for (int k = 0; k < array.Length; k++)
		{
			CurvySpline2 curvySpline = (CurvySpline2)array[k];
			if (curvySpline.shape)
			{
				continue;
			}
			for (int l = 0; (float)l < curvySpline.Length; l += ((!refresh) ? 10 : 3))
			{
				CurvySplineSegment curvySplineSegment = curvySpline.DistanceToSegment(l);
				if (curvySplineSegment.flying)
				{
					continue;
				}
				Vector2 bounds = curvySplineSegment.GetBounds();
				Vector3 v = curvySpline.InterpolateByDistance(l) + Vector3.down * bounds.y;
				if (refresh)
				{
					list.Add(new Vector5(v.x, v.y, v.z, 0f, bounds.x));
					continue;
				}
				float num = CurvySpline2.DistancePointLine(bs.ZeroY(v, 0f), bs.ZeroY(segment.Position, 0f), bs.ZeroY(segment.NextControlPoint.Position, 0f));
				if (num < 50f)
				{
					list.Add(new Vector5(v.x, v.y, v.z, num, bounds.x));
				}
			}
		}
		if (refresh)
		{
			UnityEngine.Object[] array2 = UnityEngine.Object.FindObjectsOfType(typeof(ModelObject));
			for (int m = 0; m < array2.Length; m++)
			{
				ModelObject modelObject = (ModelObject)array2[m];
				Vector3 center = modelObject.renderer.bounds.center;
				Vector3 min = modelObject.renderer.bounds.min;
				center.y = min.y;
				if (modelObject.renderer == null)
				{
					UnityEngine.Object.Destroy(modelObject.gameObject);
					continue;
				}
				Vector3 size = modelObject.renderer.bounds.size;
				list.Add(new Vector5(center.x, center.y - 0.1f, center.z, 0f, Mathf.Min(size.magnitude, 3f)));
				int num2 = 20;
				if (size.magnitude < (float)num2)
				{
					list.Add(new Vector5(center.x, center.y - 0.1f, center.z, 0f, size.magnitude));
					continue;
				}
				for (float num3 = num2 / 2; num3 < size.x + (float)(num2 / 2); num3 += (float)num2)
				{
					for (float num4 = num2 / 2; num4 < size.z + (float)(num2 / 2); num4 += (float)num2)
					{
						list.Add(new Vector5(center.x + (num3 - size.x / 2f), center.y - 0.1f, center.z + (num4 - size.z / 2f), 0f, num2 + 10));
					}
				}
			}
		}
		MonoBehaviour.print("Update Terrain " + list.Count);
		List<Vector2> list2 = new List<Vector2>();
		if (refresh || used == null)
		{
			used = new bool[td.heightmapWidth, td.heightmapHeight];
		}
		bool[,] array3 = new bool[td.heightmapWidth, td.heightmapHeight];
		foreach (Vector5 item in from a in list
			orderby a.dist > 25f, a.v.y descending
			select a)
		{
			Vector3 v2 = item.v;
			Debug.DrawRay(v2, Vector3.up, Color.red, 1f);
			if (!((Component)terrain).get_collider().Raycast(new Ray(v2 + Vector3.up * 100f, Vector3.down), out RaycastHit hitInfo, 1000f))
			{
				continue;
			}
			Vector2 textureCoord = hitInfo.textureCoord;
			int num5 = (int)(textureCoord.x * (float)td.heightmapWidth);
			Vector2 textureCoord2 = hitInfo.textureCoord;
			int num6 = (int)(textureCoord2.y * (float)td.heightmapHeight);
			float num7 = item.width * 2f + 20f;
			Vector3 heightmapScale = td.heightmapScale;
			int num8 = (int)(num7 / heightmapScale.x);
			TerrainHelper terrainHelper = (TerrainHelper)UnityEngine.Object.FindObjectOfType(typeof(TerrainHelper));
			float[,] oldheights = terrainHelper.oldheights;
			for (int n = -num8; n < num8; n++)
			{
				for (int num9 = -num8; num9 < num8; num9++)
				{
					float num10 = 1f - new Vector2(n, num9).magnitude / (float)num8;
					num10 = Mathf.Clamp01(num10 * 1.3f);
					int num11 = num6 + n;
					int num12 = num5 + num9;
					if (num11 > 0 && num11 < td.heightmapWidth && num12 > 0 && num12 < td.heightmapHeight)
					{
						if (num10 >= 1f)
						{
							used[num11, num12] = true;
						}
						if (!array3[num11, num12])
						{
							list2.Add(new Vector2((float)num11 / (float)td.heightmapWidth, (float)num12 / (float)td.heightmapHeight));
							array3[num11, num12] = true;
						}
						float y = v2.y;
						Vector3 position = terrain.transform.position;
						float num13 = y - position.y;
						Vector3 heightmapScale2 = td.heightmapScale;
						float b = num13 / heightmapScale2.y;
						float num14 = Mathf.Lerp(oldheights[num11, num12], b, num10);
						if ((num14 > heights[num11, num12] && (!used[num11, num12] || num10 >= 1f) && item.dist < 25f) || (num10 >= 1f && num14 <= heights[num11, num12]))
						{
							heights[num11, num12] = num14;
						}
					}
				}
			}
		}
		td.SetHeights(0, 0, heights);
		foreach (Vector2 item2 in list2)
		{
			Vector3 interpolatedNormal = td.GetInterpolatedNormal(item2.y, item2.x);
			int x = (int)(item2.x * (float)td.alphamapWidth);
			int y2 = (int)(item2.y * (float)td.alphamapHeight);
			float num15 = 1f - interpolatedNormal.y * interpolatedNormal.y * interpolatedNormal.y;
			if (num15 > 0.96f)
			{
				if (Mathf.Abs(interpolatedNormal.x) > Mathf.Abs(interpolatedNormal.z))
				{
					SetAlpha(x, y2, num15, 4);
				}
				else
				{
					SetAlpha(x, y2, num15, 5);
				}
			}
			else
			{
				SetAlpha(x, y2, num15, 1);
			}
		}
		if (refreshTrees)
		{
			List<TreeInstance> list3 = new List<TreeInstance>(oldTreeInstances);
			td.SetAlphamaps(0, 0, alphas);
			((Component)terrain).get_collider().enabled = false;
			StartCoroutine(Base.AddMethod((Action)(object)(Action)delegate
			{
				((Component)terrain).get_collider().enabled = true;
			}));
			RefreshTrees(used, list3);
			Debug.LogWarning("Set Trees " + list3.Count);
			td.treeInstances = list3.ToArray();
		}
		if (!refresh)
		{
			try
			{
				td.GetType().GetMethod("SetBasemapDirty", BindingFlags.Instance | BindingFlags.NonPublic).Invoke(td, new object[1]
				{
					false
				});
			}
			catch (Exception ex)
			{
				Debug.Log(ex.Message);
			}
		}
	}

	private void SetAlpha(int x, int y, float f, int layer)
	{
		float num = 0f;
		for (int i = 0; i < td.alphamapLayers; i++)
		{
			if (i != layer)
			{
				num += alphas[x, y, i];
			}
		}
		if (num == 0f)
		{
			f = 1f;
		}
		else
		{
			for (int j = 0; j < td.alphamapLayers; j++)
			{
				if (j != layer)
				{
					alphas[x, y, j] *= (1f - f) / num;
				}
			}
		}
		alphas[x, y, layer] = f;
	}

	private void RefreshTrees(bool[,] used, List<TreeInstance> trees)
	{
		for (int i = 0; i < td.alphamapWidth; i++)
		{
			for (int j = 0; j < td.alphamapHeight; j++)
			{
				Vector3 position = terrain.transform.position;
				float num = position.y + td.GetInterpolatedHeight((float)j / (float)td.alphamapWidth, (float)i / (float)td.alphamapHeight) - (float)UnityEngine.Random.Range(0, 2);
				Vector3 position2 = water.transform.position;
				if (num < position2.y)
				{
					for (int k = 0; k < td.alphamapLayers; k++)
					{
						alphas[i, j, k] = 0f;
					}
					alphas[i, j, 0] = 1f;
					used[i, j] = true;
				}
			}
		}
		Vector3[][] array = new Vector3[td.treePrototypes.Length][];
		for (int l = 0; l < array.Length; l++)
		{
			array[l] = new Vector3[4];
			for (int m = 0; m < 4; m++)
			{
				ref Vector3 reference = ref array[l][m];
				reference = new Vector3(UnityEngine.Random.value, UnityEngine.Random.value);
			}
		}
		SetGrass(used);
		int num2 = (!bs.android) ? 10000 : 3000;
		for (int n = 0; n < num2; n++)
		{
			Vector3 vector = new Vector3(UnityEngine.Random.value, UnityEngine.Random.value, UnityEngine.Random.value);
			int num3 = (int)(vector.x * (float)td.heightmapWidth);
			int num4 = (int)(vector.z * (float)td.heightmapHeight);
			if (used[num4, num3])
			{
				continue;
			}
			Vector3 interpolatedNormal = td.GetInterpolatedNormal(vector.x, vector.z);
			if (!(interpolatedNormal.y > 0.96f))
			{
				continue;
			}
			float interpolatedHeight = td.GetInterpolatedHeight(vector.x, vector.z);
			Vector3 heightmapScale = td.heightmapScale;
			vector.y = interpolatedHeight / heightmapScale.y;
			float num5 = float.MaxValue;
			int prototypeIndex = 0;
			if (UnityEngine.Random.value < 0.3f)
			{
				prototypeIndex = UnityEngine.Random.Range(0, array.Length);
			}
			else
			{
				for (int num6 = 0; num6 < array.Length; num6++)
				{
					Vector3[] array2 = array[num6];
					foreach (Vector3 a in array2)
					{
						float num8 = Vector3.Distance(a, vector);
						if (num8 < num5)
						{
							prototypeIndex = num6;
							num5 = num8;
						}
					}
				}
			}
			trees.Add(CreateTree(vector, prototypeIndex));
		}
	}

	private void SetGrass(bool[,] used)
	{
		int num = td.detailPrototypes.Length;
		float[] array = new float[num];
		int[,] array2 = new int[td.detailWidth, td.detailHeight];
		for (int i = 0; i < num; i++)
		{
			for (int j = 0; j < td.detailWidth; j++)
			{
				for (int k = 0; k < td.detailHeight; k++)
				{
					if (!used[j, k])
					{
						float num2 = 0f;
						for (int l = 0; l < num; l++)
						{
							num2 += (array[l] = UnityEngine.Random.value * (float)(3 + num - l));
						}
						for (int m = 0; m < num; m++)
						{
							array[m] /= num2;
						}
						float num3 = Mathf.Clamp(alphas[j, k, 3] - 0.5f, 0f, 0.3f);
						array2[j, k] = (int)(num3 * (float)((!bs.android) ? 30 : 15) * array[i]);
					}
				}
			}
			td.SetDetailLayer(0, 0, i, array2);
		}
	}

	private TreeInstance CreateTree(Vector3 Position, int PrototypeIndex)
	{
		float num = UnityEngine.Random.Range(0.7f, 1f);
		TreeInstance result = default(TreeInstance);
		result.set_position(Position);
		result.set_heightScale(UnityEngine.Random.Range(0.7f, 1f));
		result.set_widthScale(1f);
		result.set_color(new Color(num, num, num, 1f));
		result.set_lightmapColor(Color.white);
		result.set_prototypeIndex(PrototypeIndex);
		return result;
	}

	public override void Awake()
	{
		bs._Loader.mapLoader = this;
		CreateTerrain();
		base.Awake();
	}

	public void CreateTerrain()
	{
		if (!bs.lowQualityAndAndroid && terrain == null)
		{
			UnityEngine.Object @object = bs.LoadRes("terrain");
			if (@object != null)
			{
				terrain = ((GameObject)UnityEngine.Object.Instantiate(@object)).GetComponent<Terrain>();
				oldTreeInstances = td.treeInstances;
			}
			else
			{
				MonoBehaviour.print("couldn't load terrain");
			}
		}
	}

	public void Start()
	{
		bs._GameSettings.gravitationFactor = (bs._GameSettings.gravitationAntiFly = 1.5f);
	}

	public void UpdateMinimap()
	{
		minimap = new List<Vector2>();
		MapLoader.levelBounds = new Bounds(Vector3.zero, -Vector3.one * 10000f);
		foreach (CurvySpline2 spline2 in splines)
		{
			if (!spline2.shape)
			{
				Bounds bounds = spline2.GetBounds(local: false);
				ref Bounds levelBounds = ref bs._GameSettings.levelBounds;
				Vector3 min = MapLoader.levelBounds.min;
				float x = min.x;
				Vector3 min2 = bounds.min;
				float x2 = Mathf.Min(x, min2.x);
				Vector3 min3 = MapLoader.levelBounds.min;
				float z = min3.z;
				Vector3 min4 = bounds.min;
				levelBounds.min = new Vector3(x2, 0f, Mathf.Min(z, min4.z));
				ref Bounds levelBounds2 = ref bs._GameSettings.levelBounds;
				Vector3 max = MapLoader.levelBounds.max;
				float x3 = max.x;
				Vector3 max2 = bounds.max;
				float x4 = Mathf.Max(x3, max2.x);
				Vector3 max3 = MapLoader.levelBounds.max;
				float z2 = max3.z;
				Vector3 max4 = bounds.max;
				levelBounds2.max = new Vector3(x4, 0f, Mathf.Max(z2, max4.z));
			}
		}
		foreach (CurvySpline2 spline3 in splines)
		{
			if (spline3.shape)
			{
				continue;
			}
			foreach (CurvySplineSegment controlPoint in spline3.ControlPoints)
			{
				Vector2 item = ResizeToMinimap(controlPoint.Position);
				minimap.Add(item);
			}
			minimap.Add(Vector2.zero);
		}
	}

	public static Vector2 ResizeToMinimap(Vector3 pos)
	{
		Vector3 vector = pos - levelBounds.min;
		float x = vector.x;
		Vector3 size = levelBounds.size;
		vector.x = x / size.x;
		float z = vector.z;
		Vector3 size2 = levelBounds.size;
		vector.z = z / size2.z;
		Vector3 size3 = levelBounds.size;
		float z2 = size3.z;
		Vector3 size4 = levelBounds.size;
		if (z2 > size4.x)
		{
			float x2 = vector.x;
			Vector3 size5 = levelBounds.size;
			float x3 = size5.x;
			Vector3 size6 = levelBounds.size;
			vector.x = x2 * (x3 / size6.z);
		}
		else
		{
			float z3 = vector.z;
			Vector3 size7 = levelBounds.size;
			float z4 = size7.z;
			Vector3 size8 = levelBounds.size;
			vector.z = z3 * (z4 / size8.x);
		}
		return new Vector2(vector.x, vector.z) * 0.2f + miniMapCenter;
	}

	internal IEnumerator ActiveEditor(bool editor)
	{
		Time.timeScale = 1f;
		if (editor)
		{
			foreach (GameObject a2 in nodes)
			{
				if (a2 != null)
				{
					a2.SetActive(value: true);
				}
			}
		}
		else
		{
			nodes.Clear();
			GameObject[] array = GameObject.FindGameObjectsWithTag("node");
			foreach (GameObject a in array)
			{
				nodes.Add(a);
				a.SetActive(value: false);
			}
		}
		OneMesh = !editor;
		if (OneMesh)
		{
			ClearSbs();
			yield return null;
		}
		UnityEngine.Object[] sbs = UnityEngine.Object.FindObjectsOfType(typeof(SplinePathMeshBuilder));
		MonoBehaviour.print("ActiveEditor " + sbs.Length);
		UnityEngine.Object[] array2 = sbs;
		for (int j = 0; j < array2.Length; j++)
		{
			SplinePathMeshBuilder sb = (SplinePathMeshBuilder)array2[j];
			if (editor)
			{
				sb.ExtrusionParameter = 1f;
				foreach (Collider a3 in cls)
				{
					UnityEngine.Object.Destroy(a3.gameObject);
				}
				cls.Clear();
				continue;
			}
			if (OneMesh)
			{
				Transform t = new GameObject("Coll").transform;
				MeshCollider c2 = t.gameObject.AddComponent<MeshCollider>();
				c2.smoothSphereCollisions = true;
				c2.tag = sb.tag;
				c2.sharedMesh = (Mesh)UnityEngine.Object.Instantiate((UnityEngine.Object)sb.Mesh);
				cls.Add(c2);
				continue;
			}
			sb.ExtrusionParameter = 0.1f;
			sb.StartCap = (sb.EndCap = false);
			Mesh old = sb.mMesh;
			sb.mMesh = new Mesh();
			sb.Refresh();
			MeshCollider c = sb.gameObject.AddComponent<MeshCollider>();
			cls.Add(c);
			c.smoothSphereCollisions = true;
			((MeshCollider)((Component)sb).get_collider()).sharedMesh = sb.mMesh;
			sb.mMesh = old;
			sb.StartCap = (sb.EndCap = true);
			sb.ExtrusionParameter = 1f;
		}
		OneMesh = false;
		ClearSbs();
		yield return null;
		if (mylight != null)
		{
			mylight.enabled = editor;
		}
		if (start != null && editor)
		{
			start.gameObject.SetActive(value: true);
		}
		yield return null;
		if (terrain != null)
		{
			terrain.heightmapPixelError = (editor ? 1 : 50);
		}
	}

	private void ClearSbs()
	{
		foreach (CurvySpline2 spline2 in splines)
		{
			foreach (CurvySplineSegment controlPoint in spline2.ControlPoints)
			{
				foreach (SplinePathMeshBuilder sb in controlPoint.sbs)
				{
					UnityEngine.Object.Destroy(sb.gameObject);
				}
				controlPoint.sbs.Clear();
			}
		}
	}

	public IEnumerator LoadUnityMap(string map)
	{
		Debug.Log("Load unity map " + map);
		unityMap = map;
		Action act = bs.win.act;
		bs.win.ShowWindow((Action)(object)(Action)delegate
		{
			Label(bs._Loader.LoadingLabelMap());
			if (!bs._Loader.isLoading)
			{
				bs.win.ShowWindow(act, null, skip: true);
			}
		}, null, skip: true);
		if (!bs.CanStreamedLevelBeLoaded(map))
		{
			yield return StartCoroutine(LoadingScreen.LoadMap(map));
		}
		LoadLevelAdditive(map);
	}

	internal IEnumerator LoadMap(string s, Action onError = null, Action onLoaded = null)
	{
		mapLoading = true;
		loadMap = null;
		swirl = 0f;
		scale = 1f;
		MonoBehaviour.print(bs.mainSite + s);
		WWW www = new WWW(bs.mainSite + s);
		yield return www;
		if (!string.IsNullOrEmpty(www.error))
		{
			if (onError != null)
			{
				onError.Invoke();
			}
			yield break;
		}
		BinaryReader ms = new BinaryReader(www.bytes);
		MonoBehaviour.print("Loading Map " + ms.Length);
		int version = 0;
		Dictionary<int, CurvySpline2> saveIds = new Dictionary<int, CurvySpline2>();
		while (ms.Position < ms.Length)
		{
			LevelPackets P = (LevelPackets)ms.ReadInt();
			if (P == LevelPackets.unityMap)
			{
				string map = ms.ReadString();
				yield return StartCoroutine(LoadUnityMap(map));
			}
			if (P == LevelPackets.Spline)
			{
				yield return StartCoroutine(CreateSpline());
			}
			if (P == LevelPackets.ClosedSpline)
			{
				spline.Closed = true;
			}
			if (P == LevelPackets.shape)
			{
				yield return StartCoroutine(CreateSpline(null, shape: true));
				spline.CreatePivot(ms.ReadVector());
				spline.saveId = ms.ReadInt();
				saveIds[spline.saveId] = spline;
				spline.tunnel = ms.ReadBool();
				spline.materialId = ms.ReadInt();
				spline.color = ms.readColor();
				spline.name = ms.ReadString();
			}
			try
			{
				if (P == LevelPackets.CheckPoint2)
				{
					Transform t = SetCheckPoint(ms.ReadVector());
					t.eulerAngles = ms.ReadVector();
				}
				if (P == LevelPackets.disableTerrain)
				{
					hideTerrain = true;
				}
				if (P == LevelPackets.Block)
				{
					string readString2 = ms.ReadString();
					GameObject go;
					if (!modelLib)
					{
						Debug.LogWarning("Model lib not loaded");
						go = GameObject.CreatePrimitive(PrimitiveType.Cube);
					}
					else if (modelLib.dict.ContainsKey(readString2) && modelLib.dict[readString2].gameObj != null)
					{
						go = (GameObject)UnityEngine.Object.Instantiate((UnityEngine.Object)modelLib.dict[readString2].gameObj);
					}
					else
					{
						go = GameObject.CreatePrimitive(PrimitiveType.Cube);
						Debug.LogWarning(readString2 + " not found " + GuiClasses.dict.Count);
					}
					go.name = readString2;
					ModelObject g = InitModel(go, readString2);
					g.transform.position = ms.ReadVector();
					g.transform.eulerAngles = ms.ReadVector();
					g.transform.localScale = ms.ReadVector();
				}
				if (P == LevelPackets.roadtype)
				{
					spline.roadType = (RoadType)ms.ReadByte2();
				}
				if (P == LevelPackets.Wall)
				{
					spline.wallTexture = true;
				}
				if (P == LevelPackets.shapeMaterial)
				{
					string readString = ms.ReadString();
					spline.thumb = new Thumbnail
					{
						url = readString
					};
				}
				if (P == LevelPackets.textureTile)
				{
					spline.thumb.material.mainTextureScale = ms.ReadVector();
				}
				if (P == LevelPackets.AntiFly)
				{
					bs._GameSettings.gravitationAntiFly = ms.ReadFloat();
					bs._GameSettings.gravitationFactor = ms.ReadFloat();
				}
				if (P == LevelPackets.Flying)
				{
					segment.flying = ms.ReadBool();
				}
				if (P == LevelPackets.heightOffset)
				{
					spline.heightOffset = ms.ReadFloat();
				}
				if (P == LevelPackets.Version)
				{
					version = ms.ReadInt();
					if (version >= 702)
					{
						CurvySpline2[] array = shapes.ToArray();
						foreach (CurvySpline2 a in array)
						{
							MonoBehaviour.print("removing default brush " + a.name);
							UnityEngine.Object.Destroy(a.gameObject);
						}
					}
				}
				if (P == LevelPackets.Nitro)
				{
					Debug.Log("Nitro Loaded " + nitro);
					nitro = ms.ReadFloat();
				}
				if (P == LevelPackets.Point)
				{
					Vector3 readVector = ms.ReadVector();
					AddPoint(readVector);
					if (version >= 702)
					{
						segment.spls = new List<CurvySpline2>();
					}
					else
					{
						segment.spls = new List<CurvySpline2>(new CurvySpline2[1]
						{
							brushShapes[0]
						});
					}
					segment.swirl = ms.ReadFloat();
				}
				if (P == LevelPackets.brush)
				{
					segment.spls.Add(saveIds[ms.ReadInt()]);
				}
				if (P == LevelPackets.Material)
				{
					spline.materialId = ms.ReadInt();
					MonoBehaviour.print("Set Material " + spline.materialId);
					spline.color = ms.readColor();
				}
				if (P == LevelPackets.Finnish)
				{
					finnish = true;
				}
				if (P == LevelPackets.CheckPoint)
				{
					SetCheckPoint(segment);
				}
				if (P == LevelPackets.Start)
				{
					SetStartPoint(segment);
					start.transform.parent = segment.transform;
				}
				if (P == LevelPackets.StartPos)
				{
					SetStartPoint(ms.ReadVector(), ms.ReadVector());
				}
				if (P == LevelPackets.Laps)
				{
					laps = ms.ReadInt();
					bs._GameSettings.laps = laps;
				}
				if (P == LevelPackets.scale)
				{
					segment.scale = ms.ReadFloat();
				}
				if (P == LevelPackets.levelTime)
				{
					bs._GameSettings.levelTime = ms.ReadFloat();
				}
			}
			catch (Exception e)
			{
				Debug.LogError(e);
			}
		}
		MonoBehaviour.print("Map Version " + version);
		if (onLoaded != null)
		{
			onLoaded.Invoke();
		}
		userMapSucces = true;
		UpdateTerrain(null, refresh: true, bs._Loader.levelEditor == null);
		if (!bs._Loader.levelEditor)
		{
			Optimize2();
		}
		mapLoading = false;
	}

	protected void Combine()
	{
	}

	public void Optimize2()
	{
		UnityEngine.Object[] source = UnityEngine.Object.FindObjectsOfType(typeof(ModelObject));
		Transform transform = new GameObject("combine").transform;
		foreach (ModelObject item in ((IEnumerable<UnityEngine.Object>)source).Where((Func<UnityEngine.Object, bool>)((UnityEngine.Object a) => a.name != "coin")))
		{
			item.gameObject.isStatic = true;
			item.transform.parent = transform;
		}
		StaticBatchingUtility.Combine(transform.gameObject);
	}

	protected void Optimize()
	{
		UnityEngine.Object[] array = UnityEngine.Object.FindObjectsOfType(typeof(ModelObject));
		if (array.Length == 0)
		{
			return;
		}
		Dictionary<KeyValuePair<int, int>, List<ModelObject>> dictionary = new Dictionary<KeyValuePair<int, int>, List<ModelObject>>();
		foreach (ModelObject item in ((IEnumerable<UnityEngine.Object>)array).Where((Func<UnityEngine.Object, bool>)((UnityEngine.Object a) => a.name != "coin")))
		{
			Vector3 pos = item.pos;
			int key = (int)(pos.x / 100f);
			Vector3 pos2 = item.pos;
			KeyValuePair<int, int> key2 = new KeyValuePair<int, int>(key, (int)(pos2.z / 100f));
			if (!dictionary.TryGetValue(key2, out List<ModelObject> value))
			{
				List<ModelObject> list2 = dictionary[key2] = new List<ModelObject>();
				value = list2;
			}
			value.Add(item);
		}
		foreach (KeyValuePair<KeyValuePair<int, int>, List<ModelObject>> item2 in dictionary)
		{
			Transform transform = null;
			bool flag = false;
			foreach (ModelObject item3 in item2.Value)
			{
				if (!flag)
				{
					flag = true;
					transform = new GameObject("combine").transform;
					transform.gameObject.isStatic = true;
					transform.position = item3.pos;
				}
				item3.gameObject.isStatic = true;
				item3.transform.parent = transform;
			}
			if (flag)
			{
				StaticBatchingUtility.Combine(transform.gameObject);
			}
		}
	}

	public static ModelObject InitModel(GameObject go, string s)
	{
		ModelObject modelObject = go.AddComponent<ModelObject>();
		modelObject.renderer.gameObject.layer = Layer.block;
		if (s.Contains("Turbo"))
		{
			Transform[] componentsInChildren = go.GetComponentsInChildren<Transform>();
			foreach (Transform transform in componentsInChildren)
			{
				transform.tag = "Speed";
			}
		}
		modelObject.name2 = s;
		return modelObject;
	}

	public Transform SetCheckPoint(Vector3 pos)
	{
		return (Transform)UnityEngine.Object.Instantiate((UnityEngine.Object)bs.res.CheckPoint, pos, Quaternion.identity);
	}

	public void SetCheckPoint(CurvySplineSegment segment)
	{
		Transform transform = (Transform)UnityEngine.Object.Instantiate((UnityEngine.Object)bs.res.CheckPoint);
		transform.name = bs.res.CheckPoint.name;
		transform.position = segment.Position;
		transform.rotation = segment.transform.rotation;
		transform.parent = segment.transform;
		if (finnish)
		{
			transform.name = "Finnish";
		}
		finnish = false;
	}

	public IEnumerator CreateSpline(Action onCreated = null, bool shape = false)
	{
		spline = new GameObject((!shape) ? "Spline" : ("Shape" + shapeCnt)).AddComponent<CurvySpline2>();
		if (shape)
		{
			shapeCnt++;
		}
		spline.Closed = false;
		spline.AutoRefresh = true;
		spline.Orientation = CurvyOrientation.ControlPoints;
		spline.Granularity = 1;
		spline.Interpolation = ((!shape) ? CurvyInterpolation.CatmulRom : CurvyInterpolation.Linear);
		spline.ShowGizmos = false;
		spline.shape = shape;
		while (!spline.IsInitialized)
		{
			yield return null;
		}
		if (onCreated != null)
		{
			onCreated.Invoke();
		}
		MonoBehaviour.print("Spline Created");
		yield return null;
	}

	internal void SetStartPoint(CurvySplineSegment s)
	{
		if (start == null)
		{
			start = (Transform)UnityEngine.Object.Instantiate((UnityEngine.Object)bs.res.startPrefab);
			start.name = bs.res.startPrefab.name;
		}
		start.position = s.Position + Vector3.up * 3f + s.transform.forward * 6f;
		start.rotation = s.transform.rotation;
		start.parent = s.transform;
	}

	internal void SetStartPoint(Vector3 pos, Vector3 fv)
	{
		if (start == null)
		{
			start = (Transform)UnityEngine.Object.Instantiate((UnityEngine.Object)bs.res.startPrefab);
			start.name = bs.res.startPrefab.name;
		}
		start.position = pos;
		start.forward = fv;
	}

	internal void UpdateSwirl2()
	{
		if (segment == null || segment.Spline.ControlPoints.Count <= 1)
		{
			return;
		}
		foreach (CurvySplineSegment controlPoint in segment.Spline.ControlPoints)
		{
			Vector3 vector = (controlPoint.PreviousControlPoint == null) ? (-controlPoint.Position + controlPoint.NextControlPoint.Position) : ((!(controlPoint.NextControlPoint == null)) ? controlPoint.GetTangent(0f) : (-controlPoint.PreviousControlPoint.Position + controlPoint.Position));
			if (vector == Vector3.zero)
			{
				controlPoint.Position += Vector3.right;
				MonoBehaviour.print("move right");
			}
			controlPoint.transform.forward = vector;
			Vector3 localEulerAngles = controlPoint.transform.localEulerAngles;
			localEulerAngles.z = controlPoint.swirl;
			controlPoint.transform.localEulerAngles = localEulerAngles;
			Debug.DrawRay(controlPoint.transform.position, controlPoint.transform.forward * 5f, Color.red);
		}
	}

	internal void AddPoint(Vector3 point, CurvySplineSegment after = null, bool before = false, bool insert = false)
	{
		affectNext = (affectPrev = false);
		if (shapeEditor && segment != null && (point - segment.Spline.ControlPoints[0].Position).magnitude < 1f)
		{
			return;
		}
		if (after != null)
		{
			spline = (CurvySpline2)after.Spline;
		}
		CurvySplineSegment curvySplineSegment = segment;
		if (terrain != null && !bs.res.terrainBounds.Contains(point) && !spline.shape)
		{
			if (!hideTerrain && bs._Loader.levelEditor != null)
			{
				Popup("Warning you are out of terrain bounds, terrain will be disabled");
			}
			Debug.LogWarning("Hide Terrain");
			hideTerrain = true;
		}
		if (before)
		{
			segment = spline.Add(refresh: false, null);
		}
		else
		{
			segment = spline.Add(after, refresh: false);
		}
		if (after != null && start == null && bs._Loader.levelEditor != null && !shapeEditor)
		{
			SetStartPoint(after);
		}
		segment.Position = point;
		if (insert)
		{
			segment.spls = new List<CurvySpline2>(after.spls);
		}
		else if (after != null && !before)
		{
			segment.spls = (after.spls = brushShapes);
		}
		else
		{
			segment.spls = brushShapes;
		}
		brushShapes = new List<CurvySpline2>(brushShapes);
		Transform transform = (Transform)UnityEngine.Object.Instantiate((UnityEngine.Object)bs.res.dot, point, Quaternion.identity);
		transform.parent = segment.transform;
		transform.localScale = ((!spline.shape) ? (Vector3.one * 6f) : (Vector3.one * 2f));
		transform.name = "model";
		if (curvySplineSegment != null)
		{
			segment.transform.forward = segment.Position - curvySplineSegment.Position;
		}
		drag = segment.transform;
		segment.scale = scale;
		segment.swirl = swirl;
		spline.Refresh();
		UpdateSwirl2();
	}
}
