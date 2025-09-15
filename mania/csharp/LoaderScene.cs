using System;
using UnityEngine;

public class LoaderScene : GuiClasses
{
	public GameObject support;

	internal bool loadingUrl;

	private float width;

	public GUIText medalText;

	private static int medalsChangeSended;

	private static bool carChangeSended;

	public void Start()
	{
		bs._LoaderScene = this;
		if (bs.android || bs._Loader.vk)
		{
			UnityEngine.Object.Destroy(support);
		}
		bs._Loader.lifeDef = 300f;
		bs._Loader.mapName = "!1";
		bs._Loader.menuLoaded = true;
		MonoBehaviour.print("LoaderScene Start");
		if (bs.splitScreen && bs._Loader.reverseSplitScreen)
		{
			Screen.orientation = ScreenOrientation.LandscapeLeft;
		}
		Resources.UnloadUnusedAssets();
		bs._Loader.WindowPool();
		if (bs._Loader.loggedIn && !bs.guest)
		{
			bs._Loader.SetValue(string.Concat("reputation=", bs._Loader.reputation, "&friends=", bs._Loader.friendCount, "&medals=", bs._Loader.medals, "&gameplays=", bs._Loader.playedTimes), string.Empty);
		}
		bs._Awards.RefreshAwards();
	}

	public void OnEnable()
	{
		bs._LoaderScene = this;
	}

	public void OnGUI()
	{
		NewsWindow();
	}

	private void NewsWindow()
	{
		//IL_0015: Unknown result type (might be due to invalid IL or missing references)
		//IL_001f: Expected O, but got Unknown
		if ((MulticastDelegate)(object)bs.win.act != (MulticastDelegate)new Action(bs._Loader.MenuWindow) || bs._Integration.posts.Count == 0)
		{
			return;
		}
		GUI.skin = base.skin;
		Rect screenRect = ConvertRect(new Rect(0.5f, 0.1f, 0.5f, 0.8f));
		GUILayout.BeginArea(screenRect, GuiClasses.Tr("News"), bs.win.editorSkin.window);
		GuiClasses.scroll.x = 10f;
		base.skin.scrollView.fixedWidth = screenRect.width;
		GuiClasses.scroll = GUILayout.BeginScrollView(GuiClasses.scroll, false, false, GUIStyle.none, base.skin.verticalScrollbar);
		bs._Loader.LoadingLabelAssetBundle();
		foreach (Posts post in bs._Integration.posts)
		{
			base.skin.label.wordWrap = true;
			GUILayout.Space(10f);
			GUILayout.BeginVertical(post.date.ToShortDateString() + " " + post.title, bs.win.skin.window);
			base.skin.label.imagePosition = ImagePosition.ImageAbove;
			base.skin.label.alignment = TextAnchor.UpperLeft;
			GUILayout.Label(new GUIContent(post.image));
			GUILayout.Label(new GUIContent(post.msg));
			base.skin.label.imagePosition = ImagePosition.ImageLeft;
			GUILayout.BeginHorizontal();
			GUILayout.FlexibleSpace();
			if (Button(GuiClasses.Tr("Comments:") + post.comments))
			{
				string text = string.Format((!bs._Loader.vkSite) ? "https://www.facebook.com/trackracingonline/posts/{0}" : "https://vk.com/trackracing?w=wall-59755500_{0}%2Fall", post.id);
				if (Application.isWebPlayer)
				{
					Application.ExternalEval($"window.top.location = '{text}';");
				}
				else
				{
					Application.OpenURL(text);
				}
			}
			GUILayout.EndHorizontal();
			GUILayout.EndVertical();
		}
		GUILayout.EndScrollView();
		GUILayout.EndArea();
	}

	public Rect ConvertRect(Rect r)
	{
		return new Rect(r.x * (float)Screen.width, r.y * (float)Screen.height, r.width * (float)Screen.width, r.height * (float)Screen.height);
	}

	public void JoinVkGroupWindow()
	{
		bs.win.showBackButton = false;
		LabelCenter("Вступи в группу игры vk (+20 репутации)", 16, wrap: true);
		GUILayout.BeginHorizontal();
		if (GUILayout.Button("Вступить"))
		{
			bs.SaveStrings();
			Loader loader = bs._Loader;
			loader.reputation = (int)loader.reputation + 20;
			PlayerPrefs.SetInt(bs._Loader.playerName + "groupJoin", 1);
			bs.win.Back();
			Application.ExternalEval("OpenVkGroup()");
		}
		if (GUILayout.Button("Не вступать"))
		{
			bs.win.Back();
		}
		GUILayout.EndHorizontal();
	}

	public void ShowHostTHisGameWindow()
	{
		Setup(600, 400, string.Empty);
		BeginScrollView();
		base.skin.textArea.wordWrap = true;
		GUILayout.TextArea(bs.setting.hostTHisGame);
		GUILayout.EndScrollView();
	}

	public override void Awake()
	{
		bs._Loader.rain = (bs._Loader.night = false);
		bs._AutoQuality.enabled = false;
		base.Awake();
	}

	public void Update()
	{
		if (KeyDebug(KeyCode.T))
		{
			bs._Loader.SetValue(string.Concat("reputation=", bs._Loader.reputation, "&friends=", bs._Loader.friendCount, "&medals=", bs._Loader.medals, "&gameplays=", bs._Loader.playedTimes), string.Empty);
		}
	}
}
