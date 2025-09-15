using System;
using UnityEngine;

[Serializable]
public class Scene
{
	private bool? m_loaded;

	public string name;

	public string title;

	public int nitro;

	public int j;

	public int mapId;

	public string mapBy;

	internal string url;

	public bool userMap;

	public float rating;

	public Vector3 FinnishPos;

	public MapSets mapSets = new MapSets();

	internal bool loaded
	{
		get
		{
			bool? loaded = m_loaded;
			bool value;
			if (loaded.HasValue)
			{
				value = loaded.Value;
			}
			else
			{
				bool? flag = m_loaded = (Loader.maps.ContainsKey(name.ToLower()) || bs.CanStreamedLevelBeLoaded(name));
				value = flag.Value;
			}
			return value;
		}
	}

	public Texture2D texture => (Texture2D)bs.LoadRes("MapTextures/" + name);
}
