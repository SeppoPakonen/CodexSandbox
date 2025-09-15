using System;
using UnityEngine;

[Serializable]
public class Award
{
	public string title;

	public Texture2D texture;

	public int startLevel = 5000;

	public float factor = 2f;

	internal int local;

	public int total;

	public int level;

	public float upper;

	public float progress;

	public string text => GuiClasses.Tr(title);

	public int count
	{
		get
		{
			return Base.PlayerPrefsGetInt(bs._Loader.playerName + title + "Award");
		}
		set
		{
			Base.PlayerPrefsSetInt(bs._Loader.playerName + title + "Award", value);
		}
	}

	public void Add(int i = 0)
	{
		Debug.LogWarning("Award added " + title);
		local += i;
		count += i;
	}

	public void Calculate()
	{
		if (total > 0)
		{
			progress = (float)count / (float)total;
			return;
		}
		float num = 0f;
		float num2 = (float)startLevel / Mathf.Pow(factor, 7f);
		int i;
		for (i = 0; i < bs._Awards.ranks.Length - 2; i++)
		{
			if ((float)count < num2)
			{
				break;
			}
			num = num2;
			num2 *= factor;
		}
		level = i;
		upper = num2;
		progress = ((float)count - num) / (num2 - num);
	}
}
