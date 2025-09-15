using System.Collections.Generic;
using UnityEngine;

public class TerrainHelper : bs
{
	internal float[,] oldheights;

	private float[,,] oldalphamaps;

	private TreeInstance[] oldtrees;

	private List<int[,]> olddetails = new List<int[,]>();

	public TerrainData m_td;

	public Terrain m_terrain;

	public TerrainData td
	{
		get
		{
			if (!m_td)
			{
				m_td = terrain.terrainData;
			}
			return m_td;
		}
	}

	private Terrain terrain
	{
		get
		{
			if (!m_terrain)
			{
				m_terrain = (Terrain)Object.FindObjectOfType(typeof(Terrain));
			}
			return m_terrain;
		}
	}

	public override void Awake()
	{
		bs._TerrainHelper = this;
		base.Awake();
	}

	public void Start()
	{
	}

	public void OnEnable()
	{
		if (!bs.wp8)
		{
			oldheights = td.GetHeights(0, 0, td.heightmapWidth, td.heightmapHeight);
			oldalphamaps = td.GetAlphamaps(0, 0, td.alphamapWidth, td.alphamapHeight);
			for (int i = 0; i < td.detailPrototypes.Length; i++)
			{
				olddetails.Add(td.GetDetailLayer(0, 0, td.detailWidth, td.detailHeight, i));
			}
			oldtrees = td.treeInstances;
		}
	}

	public void OnDisable()
	{
		if (!bs.wp8)
		{
			Debug.LogWarning("Terrain Helper OnDisable");
			td.SetHeights(0, 0, oldheights);
			td.SetAlphamaps(0, 0, oldalphamaps);
			for (int i = 0; i < olddetails.Count; i++)
			{
				td.SetDetailLayer(0, 0, i, olddetails[i]);
			}
			td.treeInstances = oldtrees;
		}
	}
}
