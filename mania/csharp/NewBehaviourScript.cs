using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class NewBehaviourScript : MonoBehaviour
{
	private List<WWW> ts = new List<WWW>();

	private List<string> ss = new List<string>();

	private float st;

	private IEnumerator Start()
	{
		for (int i = 0; i < 10; i++)
		{
			Debug.LogWarning("Start");
			st = Time.time;
			ts.Add(StartThread("scripts/login3.php?name="));
			ts.Add(StartThread("usermaps/"));
			ts.Add(StartThread("web2/"));
			ts.Add(StartThread("players/"));
			ts.Add(StartThread("players2/"));
			ts.Add(StartThread("replays/"));
			float time = Time.time;
			while (Time.time - time < 10f && ts.Any((WWW a) => a != null))
			{
				yield return null;
			}
			ts.Clear();
			ss.Clear();
		}
	}

	private WWW StartThread(string url)
	{
		string arg = "http://server.critical-missions.com/tm/";
		ss.Add(url);
		return new WWW(arg + url + Random.value);
	}

	private void Update()
	{
		for (int num = ts.Count - 1; num >= 0; num--)
		{
			if (ts[num] != null && ts[num].isDone)
			{
				MonoBehaviour.print(ss[num] + " " + (Time.time - st));
				ts[num] = null;
			}
		}
	}
}
