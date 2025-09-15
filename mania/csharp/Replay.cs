using CodeStage.AntiCheat.ObscuredTypes;
using System;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
public class Replay : IComparer<Replay>
{
	public Team team;

	public Color? color;

	public Difficulty dif = Difficulty.Normal;

	public string Name;

	public string playerName;

	public Player pl;

	public bool selected;

	public int avatarId;

	public int carSkin;

	public int backupRetries;

	public CountryCodes contry;

	internal float m_finnishTime;

	internal ObscuredFloat m_finnishTimeSec;

	public List<PosVel> posVels = new List<PosVel>();

	public List<KeyDown> keyDowns = new List<KeyDown>();

	internal bool[] OnKey = new bool[500];

	internal bool[] OnKeyDown = new bool[500];

	internal bool[] OnKeyUp = new bool[500];

	internal string clanTag = string.Empty;

	internal bool ally;

	internal float voiceChatTime = float.MinValue;

	internal int textColor;

	public static int id;

	public string avatarUrl;

	public bool blue => team == Team.Blue;

	public bool red => team == Team.Red;

	public bool finnished => pl == null || pl.finnished;

	public bool ghost => pl == null || pl.ghost;

	internal float finnishTime
	{
		get
		{
			return (!bs.online) ? m_finnishTime : ((float)m_finnishTimeSec);
		}
		set
		{
			if (bs.online)
			{
				m_finnishTimeSec = value;
			}
			else
			{
				m_finnishTime = value;
			}
		}
	}

	internal string playerNameClan => clanTag + playerName;

	public Replay()
	{
		textColor = id % (GameGui.colors.Length - 1);
		id++;
	}

	public string getText(int place)
	{
		bool offlineMode = bs._Loader.offlineMode;
		float num = (bs._Loader.dmOrCoins && pl != null) ? ((float)(int)(float)pl.score) : ((!(pl != null) || !offlineMode) ? finnishTime : pl.finnishTime);
		string text = (Time.realtimeSinceStartup - voiceChatTime < 0.3f) ? GuiClasses.Tr("(voice chat)") : ((num == 0f) ? string.Empty : (" [" + ((!bs._Loader.dmOrCoins) ? bs.TimeToStr(num) : num.ToString()) + "]"));
		return string.Format("<color={3}>{0}. {1}</color><color=blue>{2}</color>", place, playerNameClan, text, (!bs._Loader.dm) ? "#15FF00" : ((team != Team.Blue) ? "red" : "blue"));
	}

	public int Compare(Replay x, Replay y)
	{
		return x.finnishTime.CompareTo(y.finnishTime);
	}

	public Replay CloneAndClear()
	{
		Replay replay = (Replay)MemberwiseClone();
		replay.posVels = new List<PosVel>();
		return replay;
	}
}
