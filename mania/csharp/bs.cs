using Ionic.Zlib;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Security.Cryptography;
using System.Text;
using UnityEngine;

public class bs : Base
{
	private static Transform m_tempTr;

	public static AutoQuality m_AutoQuality;

	public static Game _Game;

	public static TerrainHelper _TerrainHelper;

	public static LoaderScene _LoaderScene;

	public static GameSettings m_GameSettings;

	public static Res res;

	public static bool isDebug;

	public static Music _music;

	public static LoadingScreen m_LoadingScreen;

	private static CustomWindow m_CustomWindow;

	public static bool android;

	private static Integration m_Integration;

	public static Loader _Loader;

	public static GameGui m_GameGui;

	public static CarSelectMenu m_CarSelectMenu;

	public static ResLoader setting;

	protected static StringBuilder sbuilder = new StringBuilder();

	protected static StringBuilder sbuilder2 = new StringBuilder();

	protected static StringBuilder sbuilderRight = new StringBuilder();

	internal static string m_mainSite = "://server.critical-missions.com/tm/";

	public static bool serverOffline;

	private static HashSet<string> eventsCommited = new HashSet<string>();

	public static string m_platformPrefix;

	public static Vector2 touchDelta;

	public static List<KeyCode> recordKeys = new List<KeyCode>
	{
		KeyCode.W,
		KeyCode.A,
		KeyCode.D,
		KeyCode.S,
		KeyCode.Space,
		KeyCode.LeftAlt,
		KeyCode.LeftShift,
		KeyCode.Tab,
		KeyCode.F
	};

	public static List<AssetBundle> assetBundle = new List<AssetBundle>();

	public static int rnd = new System.Random().Next(9999) + 123;

	public static bool online => !_Loader.offlineMode;

	public static Transform tempTr => m_tempTr ? m_tempTr : (m_tempTr = new GameObject("tempTr").transform);

	public static InputManager input => _Loader.inputManger;

	public static AutoQuality _AutoQuality => (!(m_AutoQuality == null)) ? m_AutoQuality : (m_AutoQuality = (AutoQuality)UnityEngine.Object.FindObjectOfType(typeof(AutoQuality)));

	public static MapLoader _MapLoader => _Loader.mapLoader;

	public static Awards _Awards => _Loader._Awards;

	public static GameSettings _GameSettings
	{
		get
		{
			if (!m_GameSettings)
			{
				m_GameSettings = (GameSettings)UnityEngine.Object.FindObjectOfType(typeof(GameSettings));
				if (!m_GameSettings)
				{
					m_GameSettings = (GameSettings)UnityEngine.Object.Instantiate((UnityEngine.Object)res.gameSettings);
				}
			}
			return m_GameSettings;
		}
	}

	public static Player _Player
	{
		get
		{
			return _Game.m_Player;
		}
		set
		{
			_Game.m_Player = value;
		}
	}

	public static Player _Player2
	{
		get
		{
			return _Game.m_Player2;
		}
		set
		{
			_Game.m_Player2 = value;
		}
	}

	public static bool isMod => _Loader.modType >= ModType.mod;

	public static bool ios => setting.m_ios;

	public static LoadingScreen _LoadingScreen => (!(m_LoadingScreen == null)) ? m_LoadingScreen : (m_LoadingScreen = (LoadingScreen)UnityEngine.Object.FindObjectOfType(typeof(LoadingScreen)));

	public static CustomWindow win => (!(m_CustomWindow == null)) ? m_CustomWindow : (m_CustomWindow = (CustomWindow)UnityEngine.Object.FindObjectOfType(typeof(CustomWindow)));

	public static bool androidPlatform => Application.platform == RuntimePlatform.Android || Application.platform == RuntimePlatform.IPhonePlayer || Application.platform == RuntimePlatform.WP8Player;

	public static Integration _Integration => (!(m_Integration == null)) ? m_Integration : (m_Integration = (Integration)UnityEngine.Object.FindObjectOfType(typeof(Integration)));

	public static GameGui _GameGui => (!(m_GameGui == null)) ? m_GameGui : (m_GameGui = (GameGui)UnityEngine.Object.FindObjectOfType(typeof(GameGui)));

	public static CarSelectMenu _CarSelectMenu => (!(m_CarSelectMenu == null)) ? m_CarSelectMenu : (m_CarSelectMenu = (CarSelectMenu)UnityEngine.Object.FindObjectOfType(typeof(CarSelectMenu)));

	public Vector3 pos
	{
		get
		{
			return base.transform.position;
		}
		set
		{
			base.transform.position = value;
		}
	}

	public virtual Quaternion rot
	{
		get
		{
			return base.transform.rotation;
		}
		set
		{
			base.transform.rotation = value;
		}
	}

	internal static string http => (!flash && Application.platform != RuntimePlatform.NaCl && Application.absoluteURL != null && Application.absoluteURL.ToLower().StartsWith("https")) ? "https" : "http";

	internal static string _MainSite => http + m_mainSite;

	public bool easy => _Loader.difficulty == Difficulty.Easy;

	public bool normal => _Loader.difficulty == Difficulty.Normal;

	public bool hard => _Loader.difficulty == Difficulty.Hard;

	internal static string mainSiteHttps => "https://" + m_mainSite;

	internal static string mainSite => (!setting.localhost) ? _MainSite : "http://localhost/";

	public static bool guest
	{
		get
		{
			return _Loader.m_guest;
		}
		set
		{
			_Loader.m_guest = value;
		}
	}

	public static bool flash => Application.platform == RuntimePlatform.FlashPlayer;

	public bool vsFriendsOrPlayers => _Loader.sGameType == SGameType.VsFriends || _Loader.sGameType == SGameType.VsPlayers || _Loader.sGameType == SGameType.Clan;

	public bool vsFriendsOrClan => _Loader.sGameType == SGameType.VsFriends || _Loader.sGameType == SGameType.Clan;

	public static bool splitScreen => _Loader.sGameType == SGameType.SplitScreen;

	public static bool vsPlayersOrSplitscreen => splitScreen || _Loader.sGameType == SGameType.VsPlayers;

	public static bool standAlone => Application.platform == RuntimePlatform.WindowsPlayer || Application.platform == RuntimePlatform.LinuxPlayer;

	public static string platformPrefix2 => platformPrefix + ((!(platformPrefix == "android")) ? resEditor.resLoader.MapVersion : resEditor.resLoader.androidMapVersion);

	public Everyplay everyPlay => Everyplay.SharedInstance;

	public static string platformPrefix => (m_platformPrefix != null) ? m_platformPrefix : (m_platformPrefix = ((Application.platform == RuntimePlatform.WP8Player) ? "wp8" : ((Application.platform == RuntimePlatform.OSXEditor || Application.platform == RuntimePlatform.IPhonePlayer) ? "ios" : ((Application.platform == RuntimePlatform.Android) ? "android" : ((Application.platform != RuntimePlatform.FlashPlayer) ? "web" : "flash")))));

	public static ResEditor resEditor => (ResEditor)Resources.LoadAssetAtPath("Assets/!Prefabs/resEditor.prefab", typeof(ResEditor));

	public static bool lowOrAndorid => android || _Loader.quality <= Quality2.Low;

	public static bool lowestOrAndorid => android || _Loader.quality == Quality2.Lowest;

	public static bool medium => _Loader.quality >= Quality2.Medium;

	public static bool UltraQuality => _Loader.quality >= Quality2.Ultra;

	public static bool highQuality => _Loader.quality >= Quality2.High;

	public Quality2 quality
	{
		get
		{
			return _Loader._quality;
		}
		set
		{
			_Loader._quality = value;
		}
	}

	public static bool highOrNotAndroid => _Loader.quality >= Quality2.High || !android;

	public static bool mediumAndroidHigh
	{
		get
		{
			if (flash || android)
			{
				return highQuality;
			}
			return medium;
		}
	}

	public static bool lowestQuality => _Loader.quality <= Quality2.Lowest;

	public static bool lowQualityAndAndroid => (_Loader.quality <= Quality2.Low && android) || _Loader.quality == Quality2.Lowest;

	public static bool lowQuality => _Loader.quality <= Quality2.Low;

	public static bool Android2 => Application.platform == RuntimePlatform.Android;

	public static bool Nancl => Application.platform == RuntimePlatform.NaCl;

	public static bool webPlayer => Application.platform == RuntimePlatform.WindowsWebPlayer || Application.platform == RuntimePlatform.WindowsWebPlayer || Application.isEditor;

	public static int totalSeconds => (int)(DateTime.Now.Ticks / 10000000) - int.MaxValue;

	public static bool wp8 => Application.platform == RuntimePlatform.WP8Player;

	internal static GameObject[] checkPoints => GameObject.FindGameObjectsWithTag("CheckPoint");

	internal int myId => PhotonNetwork.player.ID;

	[Conditional("UNITY_EDITOR")]
	public static void Log(object s)
	{
		Log(s, important: false);
	}

	public static void Log(object s, bool important)
	{
		if (setting.enableLog || important)
		{
			if (Time.deltaTime == Time.fixedDeltaTime)
			{
				sbuilder2.Append(string.Concat(s, "\r\n"));
			}
			else
			{
				sbuilder.Append(string.Concat(s, "\r\n"));
			}
		}
	}

	[Conditional("UNITY_EDITOR")]
	public static void LogRight(object s)
	{
		sbuilderRight.Append(string.Concat(s, "\r\n"));
	}

	public virtual void Awake()
	{
	}

	public float ClampAngle(float angle, float min, float max)
	{
		if (angle < 90f || angle > 270f)
		{
			if (angle > 180f)
			{
				angle -= 360f;
			}
			if (max > 180f)
			{
				max -= 360f;
			}
			if (min > 180f)
			{
				min -= 360f;
			}
		}
		angle = Mathf.Clamp(angle, min, max);
		if (angle < 0f)
		{
			angle += 360f;
		}
		return angle;
	}

	public static Vector3 ZeroZ(Vector3 v)
	{
		v.z = 0f;
		return v;
	}

	public static Vector3 ZeroY(Vector3 v, float a = 0f)
	{
		v.y *= a;
		return v;
	}

	public static WWW Download2(string s, Action<string> a, bool post, params object[] prms)
	{
		return Download(s, delegate(string txt, bool b)
		{
			a((!b) ? string.Empty : txt);
		}, post, prms);
	}

	public static WWW DownloadAcc(string s, Action<string, bool> a, bool post, params object[] prms)
	{
		if (guest)
		{
			return null;
		}
		object[] array = new object[8]
		{
			"name",
			_Loader.playerName,
			"password",
			_Loader.password,
			"vkPassword",
			_Loader.vkPassword,
			"msgid",
			_Loader.msgId++
		}.Concat(prms.ToArray()).ToArray();
		StringBuilder stringBuilder = new StringBuilder("dsajhdjehfjhghvcxu");
		for (int i = 0; i < array.Length; i += 2)
		{
			if (!(array[i + 1] is byte[]))
			{
				stringBuilder.Append(array[i]);
				stringBuilder.Append(array[i + 1]);
			}
		}
		array = array.Concat(new object[2]
		{
			"md5",
			GetMD5Hash(stringBuilder.ToString())
		}).ToArray();
		return Download(mainSite + "scripts/" + s + ".php", a, post, array);
	}

	public void DebugPrint(object s)
	{
		MonoBehaviour.print(s);
	}

	public static IEnumerable<Transform> GetTransforms(Transform ts)
	{
		yield return ts;
		foreach (Transform t in ts)
		{
			foreach (Transform transform in GetTransforms(t))
			{
				yield return transform;
			}
		}
	}

	public static IEnumerable<T> GetComponents<T>(Transform ts) where T : Component
	{
		return GetTransforms(ts).Select<Transform, T>((Func<Transform, T>)((Transform a) => a.GetComponent<T>())).Where((Func<T, bool>)((T a) => (UnityEngine.Object)a != (UnityEngine.Object)null));
	}

	public static WWW Download(string s, Action<string, bool> a, bool post, params object[] prms)
	{
		s = Uri.EscapeUriString(s);
		StringBuilder stringBuilder = new StringBuilder();
		WWW wWW;
		if (prms.Length > 0)
		{
			WWWForm wWWForm = new WWWForm();
			for (int i = 0; i < prms.Length; i += 2)
			{
				if (post)
				{
					if (prms[i + 1] is byte[])
					{
						wWWForm.AddBinaryData(prms[i].ToString(), (byte[])prms[i + 1]);
					}
					else
					{
						wWWForm.AddField(prms[i].ToString(), prms[i + 1].ToString());
					}
				}
				stringBuilder.Append((i == 0) ? "?" : "&");
				stringBuilder.Append(string.Concat(prms[i], "=", WWW.EscapeURL(prms[i + 1].ToString())));
			}
			wWW = ((!post) ? new WWW(s + stringBuilder) : new WWW(s, wWWForm));
		}
		else
		{
			wWW = new WWW(s);
		}
		string text = (!post) ? wWW.url : (wWW.url + stringBuilder);
		MonoBehaviour.print(text);
		if (_Loader != null)
		{
			_Loader.StartCoroutine(DownloadCor(a, wWW, text));
		}
		else
		{
			_LoadingScreen.StartCoroutine(DownloadCor(a, wWW, text));
		}
		return wWW;
	}

	private static IEnumerator DownloadCor(Action<string, bool> a, WWW w, string url)
	{
		bool hasCache = UnityEngine.PlayerPrefs.HasKey(url) && setting.wwwCache;
		if (!hasCache)
		{
			yield return w;
		}
		if (setting.delayLoading)
		{
			yield return new WaitForSeconds(1f);
		}
		if (!hasCache && string.IsNullOrEmpty(w.error))
		{
			string text;
			string trim = text = w.text.Trim();
			if ((!text.StartsWith("<") || !trim.EndsWith(">")) && !setting.offline)
			{
				if (a != null)
				{
					if (!Application.isWebPlayer)
					{
						PlayerPrefs.SetString2(url, w.text);
					}
					a.Invoke(w.text, true);
				}
				UnityEngine.Debug.Log("reply from " + url + "\n" + w.text);
				yield break;
			}
		}
		if (a != null)
		{
			MonoBehaviour.print("Read Cache " + url + "\n" + UnityEngine.PlayerPrefs.GetString(url));
			if (UnityEngine.PlayerPrefs.HasKey(url) && (!isDebug || setting.wwwCache))
			{
				a.Invoke(UnityEngine.PlayerPrefs.GetString(url), true);
			}
			else
			{
				a.Invoke((w.error != null) ? w.error : ("Failed to Parse" + w.text), false);
			}
		}
		if (!hasCache)
		{
			UnityEngine.Debug.LogWarning(w.error + w.url);
		}
	}

	public static string TimeToStr(float s, bool draw = false, bool skip = false)
	{
		if (skip)
		{
			return ((int)s).ToString();
		}
		float num = Mathf.Abs(s);
		return ((s < 0f) ? "-" : ((!draw) ? string.Empty : "+")) + (int)num / 60 + ":" + ((int)(num % 60f)).ToString().PadLeft(2, '0') + "." + ((int)(num % 1f * 100f)).ToString().PadLeft(2, '0');
	}

	public bool TimeElapsed(float seconds, float offset = 0f)
	{
		float num = Time.deltaTime + offset;
		if (num > seconds || seconds == 0f)
		{
			return true;
		}
		if (Time.time % seconds < (Time.time - num) % seconds)
		{
			return true;
		}
		return false;
	}

	public bool FramesElapsed(int tm, int random = 0)
	{
		return (Time.frameCount + random) % tm == 0 || Time.timeScale == 0f;
	}

	public bool FramesElapsedA(int tm, int random = 0)
	{
		return (Time.frameCount + random) % tm == 0 || (!android && !isDebug);
	}

	public GUIContent GUIContent(Texture texture, string p2 = null)
	{
		return GUIContent(null, texture, p2);
	}

	public GUIContent GUIContent(string p1, Texture texture = null, string p2 = null)
	{
		if (p1 != null && texture != null)
		{
			return new GUIContent(p1, texture);
		}
		if (texture != null && p2 != null)
		{
			return new GUIContent(texture, p2);
		}
		if (texture != null)
		{
			return new GUIContent(texture);
		}
		if (p1 == null)
		{
			return new GUIContent(string.Empty);
		}
		if (p2 != null)
		{
			return new GUIContent(p1, p2);
		}
		return new GUIContent(p1);
	}

	public static void LogEvent(EventGroup eg, string group, string name)
	{
		GA.API.Design.NewEvent($"{eg}:{group.Replace(':', ' ')}:{name.Replace(':', ' ')}");
	}

	private static void LogEvent(string group, string name)
	{
		GA.API.Design.NewEvent($"{group}:{name.Replace(':', ' ')}");
	}

	public static void LogEvent2(string name)
	{
		if (!eventsCommited.Contains(name) && !isDebug)
		{
			Download(mainSite + "scripts/count.php", null, true, "submit", platformPrefix + setting.version + "/" + name);
			eventsCommited.Add(name);
		}
	}

	public static void LogEvent(string name)
	{
		LogEvent(EventGroup.Other.ToString(), name);
	}

	public static void LogEvent(EventGroup eg, string name)
	{
		LogEvent(eg.ToString(), name);
	}

	public void LoadLevel(string s, string s2 = null)
	{
		//IL_005a: Unknown result type (might be due to invalid IL or missing references)
		//IL_0064: Expected O, but got Unknown
		UnityEngine.Debug.LogWarning("Load level " + s);
		_Loader.oldLevel = _Loader.mapName;
		_Loader.mapName = (s2 ?? s.ToLower());
		Loader.loadingLevelQuit = true;
		_Loader.StartCoroutine(Base.AddMethod(0.1f, (Action)(object)(Action)delegate
		{
			Loader.loadingLevelQuit = false;
		}));
		Application.LoadLevel(s.ToLower());
	}

	public static bool CanStreamedLevelBeLoaded(string fileName)
	{
		return Application.CanStreamedLevelBeLoaded(fileName.ToLower());
	}

	public void LoadLevelAdditive(string fileName)
	{
		Application.LoadLevelAdditive(fileName.ToLower());
	}

	public static string GetFileNameWithoutExtension(string path)
	{
		int num = path.LastIndexOf('/') + 1;
		int num2 = path.LastIndexOf('.');
		if (num2 == -1)
		{
			num2 = path.Length;
		}
		return path.Substring(num, num2 - num);
	}

	public static string[] SplitString(string text)
	{
		return text.Split(new char[2]
		{
			'\r',
			'\n'
		}, StringSplitOptions.RemoveEmptyEntries);
	}

	public void ApplicationQuit()
	{
		//IL_0015: Unknown result type (might be due to invalid IL or missing references)
		//IL_002a: Expected O, but got Unknown
		SaveStrings();
		win.ShowWindow((Action)(object)new Action(_Loader.CloseWindow), win.act);
	}

	public Vector2 getMouseDelta(bool returnMouse = true)
	{
		if (Input.touchCount > 0)
		{
			return touchDelta / 10f;
		}
		return (!returnMouse) ? Vector2.zero : new Vector2(Input.GetAxis("Mouse X"), Input.GetAxis("Mouse Y"));
	}

	public static string GetFileName(string s)
	{
		return s.Substring(s.LastIndexOf('/') + 1);
	}

	public static string unescape(string d)
	{
		return d.Replace("\\r", "\r").Replace("\\n", "\n");
	}

	public static string GetMD5Hash(string input)
	{
		MD5CryptoServiceProvider mD5CryptoServiceProvider = new MD5CryptoServiceProvider();
		byte[] bytes = Encoding.UTF8.GetBytes(input);
		bytes = mD5CryptoServiceProvider.ComputeHash(bytes);
		StringBuilder stringBuilder = new StringBuilder();
		byte[] array = bytes;
		foreach (byte b in array)
		{
			stringBuilder.Append(b.ToString("x2").ToLower());
		}
		return stringBuilder.ToString();
	}

	public static bool Hotkey()
	{
		return Input.GetKeyDown(KeyCode.G) && Input.GetKey(KeyCode.A) && Input.GetKey(KeyCode.LeftShift);
	}

	public bool KeyDebug(KeyCode key, string desc = null)
	{
		if (setting.debug || Application.isEditor)
		{
			return Input.GetKeyDown(key) && Input.GetKey(KeyCode.LeftShift);
		}
		return false;
	}

	public static float Mod(float a, float n)
	{
		return (a % n + n) % n;
	}

	public static int Mod(int a, int n)
	{
		return (a % n + n) % n;
	}

	public static UnityEngine.Object LoadRes(string name)
	{
		foreach (AssetBundle item in assetBundle)
		{
			UnityEngine.Object @object = item.Load("@" + name);
			if (@object != null)
			{
				return @object;
			}
		}
		return Resources.Load(name);
	}

	public static void SaveStrings()
	{
		if (setting.disablePlayerPrefs)
		{
			Base.SetStrings.Clear();
			return;
		}
		StringBuilder stringBuilder = new StringBuilder();
		stringBuilder.AppendLine("saved keys" + Base.playerPrefKeys.Count);
		foreach (KeyValuePair<string, string> setString in Base.SetStrings)
		{
			PlayerPrefs.SetString(setString.Key.ToLower(), setString.Value);
			stringBuilder.AppendLine(setString.Key + "\t\t\t" + setString.Value);
		}
		MonoBehaviour.print("Save strings " + Base.SetStrings.Count + "\n" + stringBuilder.Length);
		Base.SetStrings.Clear();
		StringBuilder stringBuilder2 = new StringBuilder();
		foreach (string playerPrefKey in Base.playerPrefKeys)
		{
			stringBuilder2.Append(playerPrefKey).Append(",");
		}
		if (android)
		{
			PlayerPrefs.SetString2("keysnew3", stringBuilder2.ToString());
		}
		else
		{
			string text = Convert.ToBase64String(GZipStream.CompressString(stringBuilder2.ToString()));
			MonoBehaviour.print(stringBuilder2.Length + " vs " + text.Length);
			PlayerPrefs.SetString2("keysnew3", text);
		}
		PlayerPrefs.Save();
	}

	public static string Ordinal(int number)
	{
		string empty = string.Empty;
		int num = number % 10;
		int num2 = (int)Math.Floor((decimal)number / 10m) % 10;
		if (num2 == 1)
		{
			empty = "th";
		}
		else
		{
			switch (num)
			{
			case 1:
				empty = "st";
				break;
			case 2:
				empty = "nd";
				break;
			case 3:
				empty = "rd";
				break;
			default:
				empty = "th";
				break;
			}
		}
		return $"{number}{empty}";
	}

	public void LogVar(string s)
	{
		Type type = GetType();
		PropertyInfo property = type.GetProperty(s, BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
		if (property != null)
		{
			MonoBehaviour.print(property.GetValue(this, null));
		}
		else
		{
			MonoBehaviour.print(type.GetField(s, BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic).GetValue(this));
		}
	}

	public static void ExternalEval(string script)
	{
		_Loader.evalCors.Enqueue(new EvalCor
		{
			s = script
		});
	}

	public static void ExternalCall(string functionName, params object[] args)
	{
		_Loader.evalCors.Enqueue(new EvalCor
		{
			s = functionName,
			args = args
		});
	}

	public bool checkVis(Vector3 point)
	{
		Vector3 position = _Player.camera.transform.position;
		Vector3 point2 = _Player.camera.WorldToViewportPoint(point);
		bool flag = (new Rect(0f, 0f, 1f, 1f).Contains(point2) && point2.z > 0f && !Physics.Linecast(position, point, Layer.levelMask)) || Vector3.Distance(position, point) < 50f;
		if (flag)
		{
			UnityEngine.Debug.DrawLine(position, point);
		}
		return flag;
	}

	public static void PlayOneShotGui(AudioClip sound)
	{
		((Component)_Loader).get_audio().PlayOneShot(sound, _Loader.soundVolume);
	}

	public static int SetFlag(int levelFlags, int flag, bool value)
	{
		if (value)
		{
			return levelFlags | flag;
		}
		return levelFlags & ~flag;
	}

	public static bool GetFlag(int levelFlags, int flag)
	{
		return (levelFlags & flag) != 0;
	}
}
