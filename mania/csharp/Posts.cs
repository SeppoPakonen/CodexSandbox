using System;
using System.Collections;
using UnityEngine;

[Serializable]
public class Posts
{
	public string title;

	public string msg;

	public DateTime date;

	public int comments;

	public string imageUrl;

	public Texture2D image;

	public string id;

	public IEnumerator Load()
	{
		if (!string.IsNullOrEmpty(imageUrl))
		{
			WWW w = new WWW(imageUrl);
			yield return w;
			image = w.texture;
		}
	}
}
