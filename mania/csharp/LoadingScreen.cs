using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LoadingScreen : bs
{
	public static ResLoader original;

	public static int version;

	public ResLoader preRes;

	public GUISkin loadingBar;

	public float x = 400f;

	private static bool newVersionAvaible;

	public static bool block;

	public static string newVersionAvaibleText = "Please Update Game at http://trackracingonline.com/";

	private string error;

	public static string folderPath = "./";

	public static string folderUrl = "file://";

	private static string loadingText = ".you can download free android version here!\n<color=#00F2FF>http://trackracingonline.com/</color>";

	public static Dictionary<string, string> webSettings = new Dictionary<string, string>();

	public static Dictionary<string, string> cheats = new Dictionary<string, string>();

	public static string[] tips = new string[1]
	{
		"none"
	};

	public static bool mapsLoaded;

	private static Map packageMap;

	private float halfDownloaded;

	private static WWW www
	{
		get
		{
			return Loader.mapWww;
		}
		set
		{
			Loader.mapWww = value;
		}
	}

	public static string randomTip => tips[(int)Time.time / 10 % tips.Length].Replace("\\r", "\r").Replace("\\n", "\n");

	public void Start()
	{
		version = bs.setting.version;
		original = bs.setting;
		Screen.sleepTimeout = -1;
		preRes.InitSettings();
		if (bs.android)
		{
			loadingText = "Attention! This is beta version, if you got this game from store please report to soulkey4@gmail.com";
		}
		StartCoroutine(StartLoadPackages());
		bs.LogEvent("StartLoading");
	}

	private IEnumerator StartLoadPackages()
	{
		string asd = (!bs.webPlayer) ? "\nPlease update game at http://trackracingonline.com" : ", try to refresh page (F5)";
		float gameStartTime = Time.realtimeSinceStartup;
		MonoBehaviour.print("StartLoadLevels ");
		yield return StartCoroutine(StartLoadLevels());
		if (!Application.CanStreamedLevelBeLoaded("!1"))
		{
			string[] array = new string[2]
			{
				"packages",
				"assets"
			};
			foreach (string a in array)
			{
				MonoBehaviour.print("Loading " + a);
				string pcgName = a + bs.setting.packageVersion;
				if (!Loader.maps.ContainsKey(pcgName))
				{
					MonoBehaviour.print(error = "Key Not Found " + pcgName + asd);
					bs.LogEvent("Failed Key Not Found");
					yield break;
				}
				yield return StartCoroutine(LoadMap(pcgName));
				halfDownloaded = 0.5f;
			}
			if (!Application.CanStreamedLevelBeLoaded("!1"))
			{
				error = "Couldn't Load Packages" + asd;
				bs.LogEvent("Failed Packages");
				yield break;
			}
		}
		else
		{
			yield return new WaitForSeconds(2f);
		}
		Application.LoadLevel("!1");
		bs.LogEvent(EventGroup.LoadedIn, "loaded in " + (int)((Time.realtimeSinceStartup - gameStartTime) / 60f) + "minutes");
	}

	public static string GetRandomTip()
	{
		return randomTip;
	}

	public static string[] Split(string s)
	{
		List<string> list = new List<string>();
		int num = 0;
		for (int i = 0; i < 10; i++)
		{
			int num2 = s.IndexOf("##", num, StringComparison.Ordinal);
			if (num2 == -1)
			{
				list.Add(s.Substring(num, s.Length - num));
				break;
			}
			list.Add(s.Substring(num, num2 - num));
			num = Math.Min(s.Length - 1, num2 + 2);
		}
		return list.ToArray();
	}

	public static string[] SplitOnce(string s)
	{
		int num = s.IndexOf("\r\n", StringComparison.Ordinal);
		int num2 = num + 2;
		if (num == -1)
		{
			num = s.IndexOf("\n", StringComparison.Ordinal);
			num2 = num + 1;
		}
		if (num != -1)
		{
			return new string[2]
			{
				s.Substring(0, num),
				s.Substring(num2, s.Length - num2)
			};
		}
		return new string[1]
		{
			s
		};
	}

	public static IEnumerator StartLoadLevels()
	{
		if (mapsLoaded)
		{
			yield break;
		}
		Application.RegisterLogCallback(Loader.OnLogCallBack);
		bs.Download(bs.mainSite + "scripts/count.php?platform=" + bs.platformPrefix, delegate
		{
		}, false);
		mapsLoaded = true;
		try
		{
			InitFolder();
		}
		catch (Exception e5)
		{
			MonoBehaviour.print(e5.Message);
		}
		if (LoadingScreen.version == 0)
		{
			LoadingScreen.version = bs.setting.version;
		}
		MonoBehaviour.print("Loading Maps");
		www = new WWW(bs.http + "://server.critical-missions.com/tm/tm.txt");
		yield return www;
		string txt;
		if (string.IsNullOrEmpty(www.error))
		{
			Debug.Log(www.url);
			string text2;
			txt = (text2 = www.text);
			PlayerPrefs.SetString("tm.txt", text2);
		}
		else
		{
			txt = PlayerPrefs.GetString("tm.txt", string.Empty);
			Debug.LogWarning(www.url + www.error);
			bs.LogEvent("Failed tm.txt");
		}
		if (!string.IsNullOrEmpty(txt))
		{
			string[] ss = Split(txt);
			string[] array = ss;
			foreach (string s in array)
			{
				string[] sp = SplitOnce(s);
				if (sp.Length > 1)
				{
					webSettings[sp[0].Trim()] = sp[1].Trim();
				}
			}
			if (webSettings.ContainsKey("tips"))
			{
				try
				{
					List<string> t = new List<string>();
					string[] array2 = bs.SplitString(webSettings["tips"]);
					foreach (string a in array2)
					{
						string[] ar = new string[2]
						{
							"(android)",
							"(pc)"
						};
						string[] array3 = ar;
						foreach (string b in array3)
						{
							int i = a.IndexOf(b, StringComparison.Ordinal);
							if (i != -1)
							{
								if ((!(b == ar[0]) || bs.android) && (!(b == ar[1]) || !bs.android))
								{
									t.Add(a.Substring(i + b.Length));
								}
								break;
							}
							if (b == ar[ar.Length - 1])
							{
								t.Add(a);
							}
						}
					}
					tips = t.ToArray();
				}
				catch (Exception ex)
				{
					Exception e4 = ex;
					Debug.LogError(e4);
				}
			}
			SiteBlockCheck(Application.absoluteURL);
			if (webSettings.ContainsKey("hostthisgame"))
			{
				bs.setting.hostTHisGame = webSettings["hostthisgame"];
			}
			if (webSettings.ContainsKey("blockversions"))
			{
				string[] array4 = bs.SplitString(webSettings["blockversions"]);
				foreach (string a3 in array4)
				{
					MonoBehaviour.print(a3);
					if (LoadingScreen.version.ToString().StartsWith(a3))
					{
						block = (newVersionAvaible = true);
					}
				}
			}
			try
			{
				string[] array5 = bs.SplitString(webSettings["cheats"]);
				foreach (string s2 in array5)
				{
					string[] a2 = s2.Split('=');
					cheats[a2[0].Trim().ToLower()] = a2[1].Trim();
				}
			}
			catch (Exception e3)
			{
				Debug.LogWarning(e3);
			}
			try
			{
				bs.m_mainSite = webSettings["server"].Trim();
				string[] split = bs.SplitString(webSettings["versions"]);
				for (int j = 0; j < split.Length; j++)
				{
					if (split[j] == bs.platformPrefix)
					{
						newVersionAvaible = (int.Parse(split[j + 1].Trim()) > bs.setting.version);
						newVersionAvaibleText = bs.unescape(split[j + 2]);
						loadingText = bs.unescape(split[j + 3]);
					}
				}
			}
			catch (Exception e2)
			{
				Debug.LogWarning(e2);
			}
		}
		if (newVersionAvaible && bs._Loader != null)
		{
			bs.win.ShowWindow((Action)(object)(Action)delegate
			{
				bs.win.Setup(600, 300, string.Empty, Dock.Center, null, null, 1f);
				bs.win.skin.button.wordWrap = true;
				if (GUILayout.Button(newVersionAvaibleText, GUILayout.MinHeight(100f)))
				{
					Application.OpenURL(newVersionAvaibleText.Substring(newVersionAvaibleText.IndexOf("http", StringComparison.Ordinal)));
				}
			}, bs.win.act);
		}
		string getMapsUrl = bs.mainSite + "scripts/getMaps.php";
		www = new WWW(getMapsUrl);
		yield return www;
		try
		{
			string text = (!string.IsNullOrEmpty(www.error)) ? PlayerPrefs.GetString("backup", string.Empty) : www.text;
			List<Map> maps = new List<Map>();
			string[] splitString = bs.SplitString(text);
			string[] array6 = splitString;
			foreach (string path2 in array6)
			{
				string[] ss2 = path2.Split(':');
				string path = ss2[0];
				if (ss2.Length > 1 && path.EndsWith(bs.platformPrefix2))
				{
					string fileName = bs.GetFileNameWithoutExtension(path).Trim();
					int version = Mathf.Abs(int.Parse(ss2[1]));
					string url = bs.mainSite + path;
					maps.Add(new Map
					{
						name = fileName,
						url = url,
						version = version
					});
				}
			}
			PlayerPrefs.SetString("backup", text);
			if (maps.Count > 3)
			{
				bs.setting.maps = maps;
			}
		}
		catch (Exception e)
		{
			bs.LogEvent("Failed getMaps");
			Debug.LogError(e);
		}
	}

	public static void SiteBlockCheck(string url)
	{
		if (!webSettings.ContainsKey("blocksites"))
		{
			return;
		}
		string[] array = bs.SplitString(webSettings["blocksites"]);
		foreach (string text in array)
		{
			if (url != null && url.Contains(text.Trim()))
			{
				block = (newVersionAvaible = true);
			}
		}
	}

	public void OnGUI()
	{
		if (www == null)
		{
			www = new WWW(string.Empty);
		}
		GUILayout.Label("Version:" + version + " Pkg:" + bs.setting.packageVersion);
		if (packageMap != null)
		{
			GUILayout.Label("Package " + packageMap.version);
		}
		GUI.skin = loadingBar;
		GUILayout.BeginArea(new Rect((float)Screen.width / 2f - x / 2f, (float)Screen.height / 2f - 50f, x, 600f));
		if (newVersionAvaible)
		{
			loadingBar.window.wordWrap = true;
			if (GUILayout.Button(newVersionAvaibleText, loadingBar.window, GUILayout.MinHeight(100f)))
			{
				Application.OpenURL(newVersionAvaibleText.Substring(newVersionAvaibleText.IndexOf("http", StringComparison.Ordinal)));
			}
		}
		else
		{
			GUI.skin.label.alignment = TextAnchor.MiddleCenter;
			float num = www.progress / 2f + halfDownloaded;
			loadingBar.horizontalSliderThumb.fixedWidth = Mathf.Clamp(num * (x - 10f), 35f, x - 10f);
			GUILayout.HorizontalSlider(0f, 0f, 1f);
			GUILayout.Label(GuiClasses.Tr("Loading ") + (int)(num * 100f) + "%");
			GUILayout.Label(loadingText, loadingBar.window);
			GUI.skin.box.wordWrap = true;
			if (!string.IsNullOrEmpty(www.error))
			{
				GUILayout.Box(www.error);
			}
			else if (!string.IsNullOrEmpty(error))
			{
				GUILayout.Box(error);
			}
		}
		GUILayout.EndArea();
	}

	public void Update()
	{
		if (Input.GetKeyDown(KeyCode.Escape))
		{
			ApplicationQuit();
		}
	}

	private static void InitFolder()
	{
		if (Application.platform != RuntimePlatform.LinuxPlayer)
		{
		}
	}

	public static IEnumerator LoadMap(string packages)
	{
		packageMap = Loader.maps[packages];
		www = null;
		if (bs.android || bs.standAlone)
		{
			MonoBehaviour.print(folderUrl + bs.GetFileName(packageMap.url));
			www = Loader.WWW(folderUrl + bs.GetFileName(packageMap.url), packageMap.version);
			yield return www;
			Debug.Log(www.error);
		}
		if (www == null || !string.IsNullOrEmpty(www.error))
		{
			www = Loader.WWW(packageMap.url, packageMap.version);
			yield return www;
		}
		if (!string.IsNullOrEmpty(www.error))
		{
			MonoBehaviour.print(www.error);
		}
		if (packages.StartsWith("asset"))
		{
			Debug.LogWarning("obsolete AssetBundleSet");
		}
		MonoBehaviour.print("Loaded AssetBundle:" + www.assetBundle);
	}
}
