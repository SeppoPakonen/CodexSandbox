using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
public class Thumbnail
{
	public string url;

	public string name;

	public WWW w;

	public Material m;

	private static Dictionary<string, Material> list = new Dictionary<string, Material>();

	public Material material
	{
		get
		{
			if (m == null)
			{
				bs._Loader.StartCoroutine(startDownload());
			}
			return m;
		}
	}

	public Vector2 tile
	{
		get
		{
			return material.mainTextureScale;
		}
		set
		{
			material.mainTextureScale = value;
		}
	}

	private IEnumerator startDownload()
	{
		if (list.TryGetValue(url, out Material tmp))
		{
			m = tmp;
			yield break;
		}
		m = new Material(bs.res.roadMaterial);
		m.name = url;
		list.Add(url, m);
		if (string.IsNullOrEmpty(url))
		{
			yield break;
		}
		w = new WWW(url);
		yield return w;
		if (w.error != null)
		{
			Debug.Log(w.error + url);
			yield break;
		}
		Texture2D t = w.textureNonReadable;
		m.mainTexture = t;
		if (t.format == TextureFormat.ARGB32)
		{
			m.shader = bs.res.transparmentCutout;
		}
	}
}
