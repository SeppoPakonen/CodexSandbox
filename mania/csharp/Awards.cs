using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using UnityEngine;

public class Awards : GuiClasses
{
	public GUISkin loadingBar;

	public Award xp = new Award();

	public Award CompleteAllTracks = new Award();

	public Award NoFlashbacks = new Award();

	public Award NoCollisions = new Award();

	public Award UnlockAllCars = new Award();

	internal Award Play7Days = new Award();

	public Award ZombieKills = new Award();

	public Award WinInMultiplayerRace = new Award();

	public Award CustomLevel = new Award();

	public Award zombieMode = new Award();

	public Award DeathMatchOrCtf = new Award();

	public Award HardCore = new Award();

	public Award LevelCreator = new Award();

	public Award topDown = new Award();

	public Award warScore = new Award();

	public Award coinsCollected = new Award();

	public Texture2D[] ranks;

	internal List<Award> awards = new List<Award>();

	public GUIStyle rankLabel;

	private int[] ranksTotal = new int[20];

	public void OnEnable()
	{
		awards.Clear();
		IEnumerable<FieldInfo> enumerable = ((IEnumerable<FieldInfo>)GetType().GetFields()).Where((Func<FieldInfo, bool>)((FieldInfo a) => a.FieldType == typeof(Award)));
		foreach (FieldInfo item in enumerable)
		{
			Award award = (Award)item.GetValue(this);
			award.title = item.Name;
			awards.Add(award);
		}
	}

	public void InitArray()
	{
	}

	internal void OnEndGame()
	{
		bool flag = bs._Loader.place == 1;
		float num = 5f;
		int num2 = base.normal ? 2 : ((!base.hard) ? 1 : 3);
		num = ((bs._Game.listOfPlayers.Count <= 3) ? (num + 10f) : (num + (float)(((bs._Loader.place == 1) ? 15 : ((bs._Loader.place == 2) ? 10 : ((bs._Loader.place != 3) ? 2 : 5))) * num2)));
		if (!bs.online)
		{
			if (base.hard && flag && bs._Game.listOfPlayers.Count > 1 && bs._Loader.PlayersCount > 1)
			{
				HardCore.Add();
			}
			if (base.normal && flag && bs._Game.listOfPlayers.Count > 1 && bs._Loader.PlayersCount > 1)
			{
				if (bs._Game.rewindsUsed == 0 && !bs._Loader.enableZombies)
				{
					NoFlashbacks.Add();
					num *= 1.5f;
				}
				if (bs._Loader.bombCar)
				{
					NoCollisions.Add();
					num += (float)(5 * num2);
				}
				if (bs._Game.isCustomLevel)
				{
					CustomLevel.Add();
				}
				if (bs._Loader.enableZombies)
				{
					zombieMode.Add();
				}
			}
		}
		if (bs._Game.topDownTime > bs._Game.timeElapsedLevel / 2f)
		{
			bs._Awards.topDown.Add();
		}
		xp.Add((int)num);
	}

	public void RefreshAwards()
	{
		InitArray();
		MonoBehaviour.print("RefreshAwards");
		foreach (Award award in awards)
		{
			award.local = 0;
		}
		int num = 0;
		foreach (Scene scene in bs._Loader.scenes)
		{
			if (!scene.userMap)
			{
				bs._Loader.prefixMapPl = scene.name + ";" + bs._Loader.playerName + ";";
				if (bs._Loader.record != float.MaxValue)
				{
					num++;
				}
			}
		}
		CompleteAllTracks.count = num;
		CompleteAllTracks.total = bs._Loader.scenes.Count;
		UnlockAllCars.count = bs._Loader.CarSkins.Count((CarSkin a) => a.unlocked && !a.hidden);
		UnlockAllCars.total = bs._Loader.CarSkins.Count((CarSkin a) => !a.hidden);
		warScore.count = bs._Loader.warScore;
	}

	public void DrawAwardsWindow()
	{
		Setup(700, 600, string.Empty);
		LabelCenter("Achievements");
		base.skin.label.alignment = TextAnchor.UpperLeft;
		BeginScrollView();
		LabelCenter("To win rewards you must play normal or hard difficulty", 16, wrap: true);
		GUILayout.BeginHorizontal();
		base.skin.label.imagePosition = ImagePosition.ImageAbove;
		for (int i = 1; i < ranks.Length; i++)
		{
			GUILayout.Label(new GUIContent(ranksTotal[i].ToString(), ranks[i]));
		}
		ranksTotal = new int[ranks.Length];
		GUILayout.EndHorizontal();
		foreach (Award award in awards)
		{
			DrawReward(award);
		}
		GUILayout.EndScrollView();
	}

	public void DrawReward(Award a)
	{
		GUILayout.BeginHorizontal();
		string str = a.count + "/" + (int)((a.total <= 0) ? (a.upper + 1f) : ((float)a.total));
		if (bs._Game == null)
		{
			GUILayout.Label(new GUIContent(a.texture, Tp(a.title)), GUILayout.ExpandWidth(expand: false));
		}
		if (!string.IsNullOrEmpty(GUI.tooltip))
		{
			Rect lastRect = GUILayoutUtility.GetLastRect();
			lastRect.y -= 15f;
			GUI.Label(lastRect, GUI.tooltip);
			GUI.tooltip = string.Empty;
		}
		GUILayout.BeginVertical();
		GUILayout.Label(str + " " + GuiClasses.Tr(a.title));
		GUIStyle label = loadingBar.label;
		label.alignment = TextAnchor.MiddleCenter;
		a.Calculate();
		int num = (a.total == 0) ? Mathf.Min(a.level, ranks.Length - 2) : ((a.total == a.count) ? (ranks.Length - 1) : 0);
		int num2 = 505;
		loadingBar.horizontalSliderThumb.fixedWidth = Mathf.Clamp(a.progress * (float)(num2 - 10), 35f, num2 - 10);
		GUILayout.HorizontalSlider(0f, 0f, 1f, loadingBar.horizontalSlider, loadingBar.horizontalSliderThumb);
		GUILayout.EndVertical();
		GUILayout.Label(new GUIContent(ranks[num]), rankLabel, GUILayout.ExpandWidth(expand: false));
		ranksTotal[num]++;
		GUILayout.EndHorizontal();
		GUILayout.Space(10f);
	}
}
