using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class Music : bs
{
	public List<WWW> aus = new List<WWW>();

	private WWW w;

	private int id;

	private bool canPlay => aus.Count > 0 && bs._Loader.musicVolume != 0f;

	public IEnumerator Start()
	{
		if (bs.android)
		{
			yield break;
		}
		bs._music = this;
		base.get_audio().priority = 0;
		base.get_audio().ignoreListenerVolume = true;
		base.get_audio().loop = true;
		if (bs._Loader.musicVolume == 0f || Application.isEditor)
		{
			yield break;
		}
		while (bs.assetBundle.Count < 2)
		{
			yield return null;
		}
		w = new WWW(bs.mainSite + "scripts/music.php");
		yield return w;
		if (!string.IsNullOrEmpty(w.error))
		{
			yield break;
		}
		string[] splitString = bs.SplitString(w.text);
		foreach (string a2 in splitString.OrderBy<string, float>((string a) => UnityEngine.Random.value))
		{
			if (bs._Loader.musicVolume == 0f)
			{
				break;
			}
			w = new WWW(bs.mainSite + Uri.EscapeUriString(a2));
			yield return w;
			MonoBehaviour.print("loading " + w.url + "\n" + w.error);
			if (string.IsNullOrEmpty(w.error))
			{
				aus.Add(w);
			}
			if (!base.get_audio().isPlaying)
			{
				PlayRandom();
			}
		}
	}

	public void Update()
	{
		if (w != null)
		{
		}
		if (KeyDebug(KeyCode.E))
		{
			PlayRandom();
		}
		if (base.get_audio().isPlaying)
		{
			base.get_audio().volume = bs._Loader.musicVolume;
		}
	}

	public void PlayRandom()
	{
		if (canPlay)
		{
			base.get_audio().clip = aus[id % aus.Count].GetAudioClip(false, true);
			base.get_audio().Play();
			id++;
		}
	}
}
