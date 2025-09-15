using UnityEngine;

public class ModelObject : bs
{
	public Material[] oldMaterials;

	public Vector3 initPos;

	public Vector3 key;

	public string name2;

	private float m_scale = 1f;

	private Renderer m_renderer;

	public float scale
	{
		get
		{
			return m_scale;
		}
		set
		{
			if (value != m_scale)
			{
				m_scale = value;
				base.transform.localScale = Vector3.one * m_scale;
			}
		}
	}

	public Collider collider => ((Component)renderer).get_collider();

	public Renderer renderer
	{
		get
		{
			if (m_renderer == null)
			{
				m_renderer = GetComponentInChildren<Renderer>();
			}
			return m_renderer;
		}
	}

	public override void Awake()
	{
		if (string.IsNullOrEmpty(name2))
		{
			name2 = base.name;
		}
	}

	public void SetColor(Color c)
	{
		Material[] materials = renderer.materials;
		foreach (Material material in materials)
		{
			material.shader = bs.res.diffuse;
			material.color = new Color(c.r, c.g, c.b, 0.85f);
		}
	}

	public void ResetColor()
	{
		renderer.materials = oldMaterials;
	}

	public void OnDestroy()
	{
		if (bs._Loader.levelEditor != null)
		{
			bs._Loader.levelEditor.selection.Remove(this);
		}
	}
}
