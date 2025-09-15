using System.Collections;
using UnityEngine;

[ExecuteInEditMode]
public class SplinePathCloneBuilder : MonoBehaviour
{
	public delegate GameObject OnGetCloneEvent(SplinePathCloneBuilder sender, GameObject source);

	public delegate void OnReleaseCloneEvent(SplinePathCloneBuilder sender, GameObject clone);

	private const int MAXCLONES = 2000;

	public CurvySpline Spline;

	public bool UseWorldPosition;

	public GameObject[] Source = new GameObject[0];

	public float Gap;

	public SplinePathCloneBuilderMode Mode;

	public bool AutoRefresh = true;

	public float AutoRefreshSpeed;

	private Transform mTransform;

	private float mLastRefresh;

	public int ObjectCount => mTransform.childCount;

	public event OnGetCloneEvent OnGetClone;

	public event OnReleaseCloneEvent OnReleaseClone;

	private void OnEnable()
	{
		mTransform = base.transform;
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
		Clear();
	}

	private void Update()
	{
		if ((bool)Spline && Spline.IsInitialized)
		{
			if (AutoRefresh)
			{
				Spline.OnRefresh -= OnSplineRefresh;
				Spline.OnRefresh += OnSplineRefresh;
			}
			else
			{
				Spline.OnRefresh -= OnSplineRefresh;
			}
		}
	}

	public static SplinePathCloneBuilder Create()
	{
		return new GameObject("CurvyClonePath", typeof(SplinePathCloneBuilder)).GetComponent<SplinePathCloneBuilder>();
	}

	public void Refresh(bool force)
	{
		if (Spline == null || !Spline.IsInitialized)
		{
			return;
		}
		if (!Spline.AutoRefreshLength)
		{
			Spline.Refresh(refreshLength: true, refreshOrientation: false, skipIfInitialized: false);
		}
		checkSources();
		if (Source.Length == 0)
		{
			Clear();
			return;
		}
		float total;
		float[] sourceDepths = getSourceDepths(out total);
		int num = 0;
		if (!Mathf.Approximately(0f, total))
		{
			SplinePathCloneBuilderMode mode = Mode;
			if (mode == SplinePathCloneBuilderMode.CloneGroup)
			{
				num = Mathf.FloorToInt(Spline.Length / total) * Source.Length;
			}
			else
			{
				float num2 = Spline.Length;
				int num3 = 0;
				while (num2 > 0f)
				{
					num2 -= sourceDepths[num3++] + Gap;
					num++;
					if (num3 == Source.Length)
					{
						num3 = 0;
					}
				}
				num--;
			}
		}
		if (num > 2000)
		{
			Debug.LogError("SplinePathCloneBuilder: Do you really want to clone more than " + 2000 + " objects? Ensure to have proper colliders in place!");
		}
		if (force)
		{
			Clear();
		}
		else
		{
			Clear(num);
		}
		int num4 = 0;
		float num5 = 0f;
		int num6 = -1;
		int objectCount = ObjectCount;
		while (++num6 < num)
		{
			float tf = Spline.DistanceToTF(num5 + sourceDepths[num4] / 2f);
			if (num6 < objectCount)
			{
				Transform child = mTransform.GetChild(num6);
				if (UseWorldPosition)
				{
					child.position = Spline.Interpolate(tf);
				}
				else
				{
					child.localPosition = Spline.Interpolate(tf);
				}
				child.rotation = Spline.GetOrientationFast(tf) * Source[num4].transform.rotation;
			}
			else
			{
				GameObject gameObject = (this.OnGetClone == null) ? CloneObject(Source[num4]) : this.OnGetClone(this, Source[num4]);
				if ((bool)gameObject)
				{
					Transform transform = gameObject.transform;
					transform.parent = base.transform;
					gameObject.name = $"{num6:0000}" + gameObject.name;
					if (UseWorldPosition)
					{
						transform.position = Spline.Interpolate(tf);
					}
					else
					{
						transform.localPosition = Spline.Interpolate(tf);
					}
					transform.rotation = Spline.GetOrientationFast(tf) * Source[num4].transform.rotation;
				}
			}
			num5 += sourceDepths[num4] + Gap;
			if (++num4 == Source.Length)
			{
				num4 = 0;
			}
		}
	}

	public void Clear()
	{
		Transform[] componentsInChildren = base.transform.GetComponentsInChildren<Transform>();
		if (this.OnReleaseClone != null)
		{
			for (int num = componentsInChildren.Length - 1; num > 0; num--)
			{
				this.OnReleaseClone(this, componentsInChildren[num].gameObject);
			}
		}
		else
		{
			for (int num2 = componentsInChildren.Length - 1; num2 > 0; num2--)
			{
				DestroyObject(componentsInChildren[num2].gameObject);
			}
		}
	}

	public void Clear(int index)
	{
		int childCount = mTransform.childCount;
		if (this.OnReleaseClone != null)
		{
			for (int num = childCount - 1; num >= index; num--)
			{
				this.OnReleaseClone(this, mTransform.GetChild(num).gameObject);
			}
		}
		else
		{
			for (int num2 = childCount - 1; num2 >= index; num2--)
			{
				DestroyObject(mTransform.GetChild(num2).gameObject);
			}
		}
	}

	public Transform Detach()
	{
		Transform transform = new GameObject().transform;
		transform.name = "CurvyClonePath_Detached";
		Detach(transform);
		return transform;
	}

	public void Detach(Transform to)
	{
		Transform[] componentsInChildren = base.transform.GetComponentsInChildren<Transform>();
		for (int num = componentsInChildren.Length - 1; num > 0; num--)
		{
			componentsInChildren[num].parent = to;
		}
	}

	private GameObject CloneObject(GameObject source)
	{
		return (!(source != null)) ? null : (Object.Instantiate((Object)source) as GameObject);
	}

	private void DestroyObject(GameObject obj)
	{
		if (Application.isPlaying)
		{
			Object.Destroy(obj);
		}
		else
		{
			Object.DestroyImmediate(obj);
		}
	}

	private void checkSources()
	{
		ArrayList arrayList = new ArrayList();
		for (int i = 0; i < Source.Length; i++)
		{
			if (Source[i] != null)
			{
				arrayList.Add(Source[i]);
			}
		}
		Source = ((arrayList.Count != 0) ? ((GameObject[])arrayList.ToArray(typeof(GameObject))) : new GameObject[0]);
	}

	private float getDepth(GameObject o)
	{
		if (!o)
		{
			return 0f;
		}
		Bounds bounds = new Bounds(o.transform.position, Vector3.zero);
		Collider[] componentsInChildren = o.GetComponentsInChildren<Collider>(includeInactive: true);
		for (int i = 0; i < componentsInChildren.Length; i++)
		{
			bounds.Encapsulate(componentsInChildren[i].bounds);
		}
		Vector3 size = bounds.size;
		return size.z;
	}

	private float[] getSourceDepths(out float total)
	{
		float[] array = new float[Source.Length];
		total = 0f;
		for (int i = 0; i < Source.Length; i++)
		{
			array[i] = getDepth(Source[i]);
			total += array[i];
		}
		total += (float)array.Length * Gap;
		return array;
	}

	private void OnSplineRefresh(CurvySpline sender)
	{
		if (Time.realtimeSinceStartup - mLastRefresh > AutoRefreshSpeed)
		{
			mLastRefresh = Time.realtimeSinceStartup;
			Refresh(force: false);
		}
	}
}
