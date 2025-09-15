using System;
using UnityEngine;

[Serializable]
public class ModelFile
{
	public string name;

	public string path;

	private bool loaded;

	private GameObject m_gameObj;

	private Texture2D m_thumb;

	private bool thumbLoaded;

	public GameObject gameObj
	{
		get
		{
			string text = path.Substring(0, path.LastIndexOf('.')).Replace('\\', '/');
			if (!loaded)
			{
				m_gameObj = (GameObject)bs.LoadRes(text);
				loaded = true;
			}
			return m_gameObj;
		}
	}

	public Texture2D thumb
	{
		get
		{
			if (!thumbLoaded)
			{
				thumbLoaded = true;
				m_thumb = (Texture2D)bs.LoadRes("FileIcons/" + name);
			}
			return m_thumb;
		}
	}
}
