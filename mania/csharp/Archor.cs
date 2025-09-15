using UnityEngine;

[ExecuteInEditMode]
public class Archor : bs
{
	public Camera camera;

	private Vector3 oldPos;

	public float x = 0.5f;

	public float y = 0.5f;

	private float height;

	private float width;

	private Vector3 Scale;

	public Player player;

	private ScreenOrientation oldor;

	private Vector3? m_size;

	public Color m_color;

	public float scale
	{
		set
		{
			base.transform.localScale = Scale * value;
			m_size = null;
		}
	}

	public new bool enabled
	{
		get
		{
			return base.get_renderer().enabled;
		}
		set
		{
			base.get_renderer().enabled = value;
		}
	}

	public Vector3 size
	{
		get
		{
			Vector3? size = m_size;
			Vector3 value;
			if (size.HasValue)
			{
				value = size.Value;
			}
			else
			{
				Vector3? vector = m_size = Abs(camera.WorldToScreenPoint(base.get_renderer().bounds.min) - camera.WorldToScreenPoint(base.get_renderer().bounds.max));
				value = vector.Value;
			}
			return value;
		}
	}

	public Color color
	{
		get
		{
			return m_color;
		}
		set
		{
			if (m_color != value)
			{
				base.get_renderer().sharedMaterial.SetColor("_Emission", m_color = value);
			}
		}
	}

	private static bool skip => bs.setting == null || (!bs.setting.enableGuiEdit && !Application.isPlaying);

	public Vector3 screenPos
	{
		set
		{
			inversePos = new Vector3(value.x / (float)Screen.width, value.y / (float)Screen.height, value.z);
		}
	}

	private bool reverse => player.secondPlayer && bs._Loader.reverseSplitScreen;

	public Vector3 inversePos
	{
		get
		{
			return new Vector3((!reverse) ? x : (1f - x), (!reverse) ? y : (1f - y), 1f);
		}
		set
		{
			x = ((!reverse) ? value.x : (1f - value.x));
			y = ((!reverse) ? value.y : (1f - value.y));
		}
	}

	public new Vector3 pos
	{
		get
		{
			return new Vector3(x, y, 1f);
		}
		set
		{
			x = value.x;
			y = value.y;
		}
	}

	public override void Awake()
	{
		Init2();
		if (!skip && player == null)
		{
			player = base.transform.root.GetComponent<Player>();
		}
	}

	private void Init2()
	{
		width = Screen.width;
		height = Screen.height;
		oldPos = pos;
		Scale = base.transform.localScale;
		oldor = Screen.orientation;
	}

	public void Start()
	{
		Init2();
		if (!skip && Application.isPlaying)
		{
			Invoke("ResolutionChanged", 0.5f);
		}
	}

	public void Update()
	{
		if (!skip && width != 0f && height != 0f && ((float)Screen.width != width || (float)Screen.height != height || pos != oldPos || oldor != Screen.orientation))
		{
			ResolutionChanged();
		}
	}

	private void ResolutionChanged()
	{
		m_size = null;
		if (bs._Loader != null)
		{
			oldPos = pos;
			width = Screen.width;
			height = Screen.height;
			oldor = Screen.orientation;
			base.transform.position = camera.ViewportToWorldPoint(inversePos);
		}
	}

	public bool HitTest(Vector2 vector2)
	{
		return enabled && base.get_renderer().bounds.IntersectRay(camera.ScreenPointToRay(new Vector3(vector2.x, vector2.y, 1f)));
	}

	public Vector3 Abs(Vector3 v)
	{
		return new Vector3(Mathf.Abs(v.x), Mathf.Abs(v.y), Mathf.Abs(v.z));
	}
}
