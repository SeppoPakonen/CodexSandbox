using System;
using UnityEngine;

public class GameGui : bsNetwork
{
	private Vector3 mp;

	public Texture2D helpScreen;

	public Texture2D helpScreenRus;

	internal float lastHelpTime;

	internal bool chatEnabled;

	private string chat = string.Empty;

	private bool firstTimeChat;

	private Vector2 smoothAxis;

	private string[] colorstr = new string[10]
	{
		"black",
		"blue",
		"cyan",
		"lime",
		"red",
		"white",
		"yellow",
		"magenta",
		"brown",
		"grey"
	};

	public static Color[] colors = new Color[10]
	{
		Color.black,
		Color.blue,
		Color.cyan,
		Color.green,
		Color.red,
		Color.white,
		Color.yellow,
		Color.magenta,
		new Color(0.647058845f, 14f / 85f, 14f / 85f, 1f),
		new Color(128f / 255f, 128f / 255f, 128f / 255f, 1f)
	};

	private string chatOutput = string.Empty;

	private GUIContent[] cache = new GUIContent[30];

	public void Tutorial()
	{
	}

	public void ShowHelpScreen(float time)
	{
		lastHelpTime = Time.time + time;
	}

	[RPC]
	public void Chat(string Obj)
	{
		if (bs._Loader.enableChat)
		{
			if (!firstTimeChat)
			{
				firstTimeChat = true;
				bs._Game.centerText(GuiClasses.Tr("press enter to reply"), 2f);
			}
			string text = Obj + "\r\n";
			if (!chatOutput.EndsWith(text))
			{
				chatOutput += text;
				ClearChat();
			}
			((Component)bs._Game).get_audio().PlayOneShot(bs.res.chat, bs._Loader.soundVolume);
		}
	}

	private void ClearChat()
	{
		//IL_0037: Unknown result type (might be due to invalid IL or missing references)
		//IL_0041: Expected O, but got Unknown
		if (bs.SplitString(chatOutput).Length > 5)
		{
			chatOutput = RemoveFirstLine(chatOutput);
			return;
		}
		StartCoroutine(Base.AddMethod(20f, (Action)(object)(Action)delegate
		{
			chatOutput = RemoveFirstLine(chatOutput);
		}));
	}

	public string RemoveFirstLine(string s)
	{
		int num = s.IndexOf("\r\n", StringComparison.Ordinal) + 2;
		return s.Substring(num, s.Length - num);
	}

	public void OnGUI()
	{
		CustomWindow.GUIMatrix(0.7f);
		GUI.depth = 3;
		if (bs._Loader.Beginner && bs._Player.checkPointsPass.Count < 1 && !bs.android && bs._Loader.enableMouse)
		{
			mp = Vector3.Lerp(mp, Input.mousePosition, Time.deltaTime * 5f);
			float num = (float)Screen.height - mp.y - 80f;
			Texture2D texture2D = (!(GuiClasses.assetDictionary.name == "Russian")) ? helpScreen : helpScreenRus;
			bool flag = mp.x > (float)Screen.width / 2f;
			bs.res.arrow.normal = ((!flag) ? bs.res.arrow.active : bs.res.arrow.hover);
			if (Mathf.Abs(mp.x / (float)Screen.width - 0.5f) > 0.1f)
			{
				GUI.Label(Rect.MinMaxRect((float)Screen.width / 2f, num, mp.x, num), string.Format(GuiClasses.Tr("<size=30>Steering {0}</size>\r\nto steer {1} move cursor to {1} side"), (!flag) ? GuiClasses.Tr("Left") : GuiClasses.Tr("Right"), (!flag) ? "Right" : "Left"), bs.res.arrow);
			}
			if (Time.time - lastHelpTime < 0f)
			{
				GUI.DrawTexture(new Rect((float)Screen.width / 2f - (float)texture2D.width / 2f, (float)Screen.height / 2f - (float)texture2D.height / 2f, texture2D.width, texture2D.height), texture2D);
			}
		}
		Rect screenRect = Rect.MinMaxRect(((!bs.setting.enableLog) ? 0f : 0.3f) * (float)Screen.width, 0.13f * (float)Screen.height, Screen.width, Screen.height);
		if (bs.isDebug)
		{
			smoothAxis += new Vector2(Input.GetAxis("Mouse X"), Input.GetAxis("Mouse Y"));
			float value = UnityEngine.Random.value;
			smoothAxis = Vector2.Lerp(smoothAxis, Vector2.zero, Time.deltaTime * value * value * value * 5f);
			smoothAxis += UnityEngine.Random.insideUnitCircle * smoothAxis.magnitude * Time.deltaTime;
			screenRect.x -= smoothAxis.x;
			screenRect.y += smoothAxis.y;
		}
		GUILayout.BeginArea(screenRect);
		GUI.skin.label.wordWrap = false;
		GUI.skin.label.fontSize = 20;
		if (bs.online)
		{
			GUILayout.Label(chatOutput);
		}
		if (chatEnabled)
		{
			GUI.SetNextControlName("Chat");
			chat = GUILayout.TextField(chat);
			GUI.FocusControl("Chat");
		}
		if (bs.win.act == null && Event.current.keyCode == KeyCode.Return && Event.current.isKey && (Event.current.type == EventType.KeyUp || chat.Length > 0) && (bs.online || bs.isDebug))
		{
			chatEnabled = !chatEnabled;
			if (!chatEnabled && chat.Length > 0 && !bs._Loader.banned)
			{
				CallRPC(Chat, bs._Loader.playerNamePrefixed + ": " + chat);
			}
			chat = string.Empty;
		}
		bool flag2 = bs._Loader.PlayersCount < 1 && !bs.online;
		for (int i = 0; i < ((!flag2) ? bs._Game.listOfPlayers.Count : bs._Loader.replays.Count); i++)
		{
			Replay replay = (!flag2) ? bs._Game.listOfPlayers[i].replay : bs._Loader.replays[i];
			if (cache[i] == null || FramesElapsed(10))
			{
				string text = replay.getText(i + 1);
				cache[i] = GUIContent("<color=" + colorstr[replay.textColor] + ">" + ((!bs.isDebug) ? "#" : colorstr[replay.textColor]) + "</color>" + text, bs.res.GetAvatar(replay.avatarId, replay.avatarUrl));
			}
			GUI.skin.label.fontSize = (int)((float)((!replay.ghost) ? 30 : 20) * ((Screen.width >= 1000) ? 1f : 0.7f));
			GUILayout.Label(cache[i], GUILayout.Height((!replay.ghost) ? 60 : 40), GUILayout.ExpandWidth(expand: false));
		}
		GUILayout.EndArea();
	}
}
