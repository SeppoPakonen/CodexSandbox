using CodeStage.AntiCheat;
using CodeStage.AntiCheat.ObscuredTypes;
using ExitGames.Client.Photon;
using Ionic.Zlib;
using LitJson;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using UnityEngine;

public class Loader : GuiClasses
{
	[Flags]
	public enum ReplayFlags
	{
		dm = 0x1,
		stunts = 0x2,
		online = 0x4,
		rain = 0x8,
		bombcar = 0x10
	}

	private struct stringTime
	{
		public string s;

		public float f;

		public override bool Equals(object obj)
		{
			stringTime stringTime = (stringTime)obj;
			return stringTime.s == s;
		}

		public override int GetHashCode()
		{
			return s.GetHashCode();
		}
	}

	internal enum TopMaps
	{
		Staff_Pick,
		Top,
		New
	}

	private enum props
	{
		mapname,
		difficulty,
		rain,
		night,
		topdown,
		rewinds,
		wait,
		dm,
		version,
		life,
		wallCollision,
		team,
		gametype,
		bombCar,
		havePassword,
		weapons,
		speedLimit,
		mapId
	}

	private const int MaxLength = 500;

	private const string _DefinePrefsTime = "savePrefsTime";

	internal LevelEditor levelEditor;

	internal MapLoader mapLoader;

	internal Game game;

	public AudioClip pushButton;

	public AudioClip click;

	public ResLoader m_setting;

	public new Res res;

	public new Awards _Awards;

	public GUIText guiText;

	public GUIText guiTextRight;

	internal string url = string.Empty;

	internal bool urlReceived;

	public InputManager inputManger;

	internal float gameStartTime;

	protected int maxLength = 12;

	protected int minLength = 3;

	internal string confirmPassword = string.Empty;

	internal int wonMedals;

	public static WWW mapWww;

	internal static string lastLog = string.Empty;

	protected float levelLoadTime;

	internal string oldLevel;

	public TextAsset assetDictionaryMergeTo;

	public TextAsset assetDictionaryMerge;

	internal bool gamePlayed;

	private bool wonCarShown;

	internal List<Replay> replays = new List<Replay>();

	protected List<Replay> tempReplays = new List<Replay>();

	public static List<string> prints = new List<string>();

	public Font font;

	protected List<Replay> previews = new List<Replay>();

	public SGameType sGameType = SGameType.VsPlayers;

	public GameType gameType;

	internal bool loggedIn;

	internal int CullMode;

	public bool enableCollision;

	internal int lastVersion;

	internal WWW wwwAssetBundle;

	internal bool menuLoaded;

	internal DateTime? serverTime;

	public GuiSkins guiSkins;

	public Dictionary<string, List<Thumbnail>> m_thumbnails;

	internal string[] thumbnailKeys;

	internal List<Thumbnail>[] thumbnailValues;

	public static string resourcesPath = "Assets/Resources/MapTextures/";

	internal string lastError;

	public float deltaTime = 0.01f;

	private Vector2 oldTouch;

	internal bool isOdnoklasniki;

	internal bool vk;

	internal bool vkSite;

	public static StringBuilder log = new StringBuilder();

	protected bool disableEveryPlay;

	public Queue<EvalCor> evalCors = new Queue<EvalCor>();

	public bool m_guest = true;

	internal string mapName;

	internal string levelName;

	protected string friendName = string.Empty;

	internal List<Scene> scenes;

	private TextAsset mapsTxt;

	public TextAsset scenesTxt;

	public GUITexture fullScreen;

	private Color m_fullScreenColor;

	internal static int errors;

	private static float oldTime;

	protected List<string> clanMembers = new List<string>();

	public bool m_offlineMode;

	private bool includeFriends;

	public bool dontUploadReplay;

	internal List<Scene> userMaps = new List<Scene>();

	internal int page;

	public static bool loadingLevelQuit;

	public bool bombCar;

	public static Dictionary<string, Map> m_maps;

	public bool Beginner;

	public MapSets mapSets = new MapSets();

	private Queue<stringTime> centerTextList = new Queue<stringTime>();

	internal float lastTextTime = float.MinValue;

	public GUIText CenterText;

	public GUITexture CenterTextBackground;

	public Texture2D[] flags = new Texture2D[5];

	private string showPass;

	private string email = string.Empty;

	public bool everyplayRecorded;

	private KeyValuePair<string, string>[] keyValuePairs;

	internal bool resLoaded2;

	internal bool resLoaded;

	private bool inclan;

	private ChatPhp _ChatPhp = new ChatPhp();

	internal int tabSelected;

	private WWW wwwUserMaps;

	internal TopMaps topMaps;

	private string searchMap = string.Empty;

	internal bool guestGui;

	internal string scoreBoard;

	internal bool advancedOptions;

	private string cheat = string.Empty;

	private string console = string.Empty;

	private Vector2 consoleScroll = new Vector2(0f, float.MaxValue);

	internal bool tankCheat;

	internal bool carsCheat;

	internal bool levelsCheat;

	public int userId;

	private string roomName;

	public float lifeDef = 300f;

	private bool havePassword;

	private string gamePassword = string.Empty;

	public WeaponEnum weaponEnum = (WeaponEnum)(-1);

	internal int waitTime = 4;

	internal int rewinds = 3;

	private string tableFormat;

	internal string prefixMapPl;

	internal string playerNamePrefixed = string.Empty;

	internal string m_clanTag;

	internal string m_playerName;

	protected bool rememberPassword = true;

	private string m_password = string.Empty;

	internal float voiceChatVolume = 1f;

	internal bool enableChat = true;

	public int? m_controls;

	public bool m_enableMouse;

	public int? m_drawDistance;

	public int? m_modType;

	public Quality2? m_quality;

	public Difficulty? m_difficulty;

	public bool? m_autoQuality;

	public bool? m_rearCamera;

	internal bool rain;

	internal float speedLimit = float.MaxValue;

	internal bool speedLimitEnabled;

	internal bool snow;

	public bool night;

	internal bool topdown;

	public bool? m_reverseSplitScreen;

	protected float? m_audioVolume;

	internal float soundVolume = 1f;

	protected float? m_musicVolume;

	public float? m_flying;

	internal float fieldOfView;

	public float? m_sensivity;

	private bool showScreenshotText;

	public bool mh;

	public VarCheck medalsCheck = new VarCheck();

	public VarCheck carsCheck = new VarCheck();

	public bool dm => gameType == GameType.dm || gameType == GameType.team || gameType == GameType.ctf;

	public bool dmNoTeam => gameType == GameType.dm;

	public bool race => gameType == GameType.race;

	public bool team => gameType == GameType.team || gameType == GameType.ctf;

	public bool stunts => gameType == GameType.zombies;

	public bool enableZombies
	{
		get
		{
			return stunts;
		}
		set
		{
			gameType = (value ? GameType.zombies : GameType.race);
		}
	}

	public bool ctf => gameType == GameType.ctf;

	public bool dmOrCoins => dm || gameType == GameType.zombies;

	internal int banTime
	{
		get
		{
			return PlayerPrefs.GetInt("ban");
		}
		set
		{
			PlayerPrefs.SetInt("ban", value);
		}
	}

	public bool banned => bs.totalSeconds - banTime < 0;

	public int dayBonus
	{
		get
		{
			return Base.PlayerPrefsGetInt(playerName + "daybonus");
		}
		set
		{
			Base.PlayerPrefsSetInt(playerName + "daybonus", value);
		}
	}

	public int lastDayPlayed
	{
		get
		{
			return Base.PlayerPrefsGetInt(playerName + "lastDayPlayed");
		}
		set
		{
			Base.PlayerPrefsSetInt(playerName + "lastDayPlayed", value);
		}
	}

	public Dictionary<string, List<Thumbnail>> thumbnails
	{
		get
		{
			if (m_thumbnails == null)
			{
				StartCoroutine(LoadTextres());
			}
			return m_thumbnails;
		}
	}

	private string[] titles
	{
		get
		{
			return res.titles;
		}
		set
		{
			res.titles = value;
		}
	}

	public bool disableRep => !bs._Loader.vkSite;

	public Scene curSceneDef => curScene ?? curSceneDef;

	public Scene curScene => scenes.FirstOrDefault((Scene a) => a.name.ToLower() == mapName.ToLower());

	public bool isLoading => mapWww != null && !mapWww.isDone;

	private Color fullScreenColor
	{
		set
		{
			if (value != m_fullScreenColor)
			{
				fullScreen.color = value;
				m_fullScreenColor = value;
			}
		}
	}

	public ReplayFlags replayFlags
	{
		get
		{
			ReplayFlags replayFlags = (ReplayFlags)0;
			if (dm)
			{
				replayFlags |= ReplayFlags.dm;
			}
			if (stunts)
			{
				replayFlags |= ReplayFlags.stunts;
			}
			if (bs.online)
			{
				replayFlags |= ReplayFlags.online;
			}
			if (rain)
			{
				replayFlags |= ReplayFlags.rain;
			}
			if (bombCar)
			{
				replayFlags |= ReplayFlags.bombcar;
			}
			return replayFlags;
		}
	}

	public string mapNamePrefixed => mapName + "@" + bs.setting.physicVersion + (dm ? "@dm" : (stunts ? "@zombies" : (bs.online ? "@online" : ((!rain) ? string.Empty : "@rain")))) + ((!bombCar) ? string.Empty : "@bombcar");

	public bool offlineMode
	{
		get
		{
			return m_offlineMode;
		}
		set
		{
			PhotonNetwork.offlineMode = (m_offlineMode = value);
		}
	}

	public List<CarSkin> CarSkins
	{
		get
		{
			if (res.CarSkins.Count == 0)
			{
				LoadCarSkins();
			}
			return res.CarSkins;
		}
	}

	public static Dictionary<string, Map> maps
	{
		get
		{
			if (m_maps == null)
			{
				m_maps = new Dictionary<string, Map>();
				foreach (Map map in bs.setting.maps)
				{
					m_maps[map.name] = map;
				}
			}
			return m_maps;
		}
	}

	public int SceneCount => scenes.Count;

	protected CarSkin WonCar
	{
		get
		{
			for (int i = 0; i < CarSkins.Count; i++)
			{
				CarSkin carSkin = GetCarSkin(i, mine: true);
				if (!wonCarShown && carSkin.medalsNeeded > (int)medals - wonMedals && carSkin.medalsNeeded <= (int)medals)
				{
					wonCarShown = true;
					return carSkin;
				}
			}
			return null;
		}
	}

	public string replayLinkPrefix => bs._Loader.url.Split('?')[0] + "?";

	public string replayLink => bs._Loader.playerNamePrefixed + "." + bs._Loader.mapName + ".rep";

	public bool showBanner
	{
		get
		{
			return PlayerPrefs.GetInt("showbanner", 1) == 1;
		}
		set
		{
			PlayerPrefs.SetInt("showbanner", value ? 1 : 0);
		}
	}

	public int loggedInTime
	{
		get
		{
			return PlayerPrefs.GetInt(playerName + "SaveTime");
		}
		set
		{
			PlayerPrefs.SetInt(playerName + "SaveTime", value);
		}
	}

	public int uploadPrefsTime
	{
		get
		{
			return PlayerPrefs.GetInt(playerName + "UploadTime");
		}
		set
		{
			PlayerPrefs.SetInt(playerName + "UploadTime", value);
		}
	}

	internal int serverVersion => (PhotonNetwork.room != null) ? CustomProperty(PhotonNetwork.room, props.version, 0) : bs.setting.version;

	internal string prefixplat => playerName + bs.platformPrefix;

	internal string contry
	{
		get
		{
			return Base.PlayerPrefsGetString("country", string.Empty);
		}
		set
		{
			Base.PlayerPrefsSetString("country", value);
		}
	}

	public CountryCodes Country
	{
		get
		{
			return (CountryCodes)Base.PlayerPrefsGetInt("country2", (int)Country2);
		}
		set
		{
			Base.PlayerPrefsSetInt("country2", (int)value);
		}
	}

	private CountryCodes Country2
	{
		get
		{
			//Discarded unreachable code: IL_003b
			try
			{
				return (CountryCodes)((!string.IsNullOrEmpty(contry)) ? ((int)Enum.Parse(typeof(CountryCodes), contry.ToLower())) : 0);
			}
			catch (ArgumentException)
			{
			}
			return CountryCodes.fi;
		}
	}

	internal string clanTag
	{
		get
		{
			return m_clanTag ?? (m_clanTag = Base.PlayerPrefsGetString("clanTag", string.Empty));
		}
		set
		{
			Base.PlayerPrefsSetString("clanTag", m_clanTag = value);
		}
	}

	internal string playerName
	{
		get
		{
			return m_playerName ?? (m_playerName = Base.PlayerPrefsGetString("playerName", string.Empty));
		}
		set
		{
			Base.PlayerPrefsSetString("playerName", m_playerName = value);
		}
	}

	internal string clanName
	{
		get
		{
			return Base.PlayerPrefsGetString("clanName", string.Empty);
		}
		set
		{
			Base.PlayerPrefsSetString("clanName", value);
		}
	}

	internal string password
	{
		get
		{
			return (!rememberPassword) ? m_password : PlayerPrefs.GetString(playerName + "password", string.Empty);
		}
		set
		{
			if (rememberPassword)
			{
				PlayerPrefs.SetString(playerName + "password", value);
			}
			else
			{
				m_password = value;
			}
		}
	}

	internal string vkPassword
	{
		get
		{
			return PlayerPrefs.GetString("vkpassword", string.Empty);
		}
		set
		{
			PlayerPrefs.SetString("vkpassword", value);
		}
	}

	public bool accelometer => controls == Contr.acel;

	public bool autoFullScreen
	{
		get
		{
			return Base.PlayerPrefsGetBool("autoFullScreen", Application.isWebPlayer && !bs.Nancl);
		}
		set
		{
			Base.PlayerPrefsSetBool("autoFullScreen", value);
		}
	}

	public bool showYourGhost
	{
		get
		{
			return Base.PlayerPrefsGetBool(prefixplat + "showYourGhost", !bs.android && !Application.isEditor);
		}
		set
		{
			Base.PlayerPrefsSetBool(prefixplat + "showYourGhost", value);
		}
	}

	public bool statsSaved
	{
		get
		{
			return Base.PlayerPrefsGetBool(playerName + "StatsSaved2");
		}
		set
		{
			Base.PlayerPrefsSetBool(playerName + "StatsSaved2", value);
		}
	}

	public float record
	{
		get
		{
			return Base.PlayerPrefsGetFloat(6 + prefixMapPl + "record", float.MaxValue);
		}
		set
		{
			Base.PlayerPrefsSetFloat(6 + prefixMapPl + "record", value);
		}
	}

	public int avatar
	{
		get
		{
			return Base.PlayerPrefsGetInt(playerName + "avatar", (!bs.isDebug) ? (-1) : 0);
		}
		set
		{
			Base.PlayerPrefsSetInt(playerName + "avatar", value);
		}
	}

	public Texture2D Avatar => res.GetAvatar(avatar, avatarUrl);

	internal string avatarUrl
	{
		get
		{
			return Base.PlayerPrefsGetString("avatarUrl", string.Empty);
		}
		set
		{
			Base.PlayerPrefsSetString("avatarUrl", value);
		}
	}

	public int carskin
	{
		get
		{
			return Base.PlayerPrefsGetInt(playerName + "car");
		}
		set
		{
			Base.PlayerPrefsSetInt(playerName + "car", value);
		}
	}

	public int place
	{
		get
		{
			return Base.PlayerPrefsGetInt(prefixMapPl + "medal", 4);
		}
		set
		{
			Base.PlayerPrefsSetInt(prefixMapPl + "medal", value);
		}
	}

	public int PlayersCount
	{
		get
		{
			return Base.PlayerPrefsGetInt(prefixplat + "PlayersCount", (!bs.android) ? 3 : 0);
		}
		set
		{
			Base.PlayerPrefsSetInt(prefixplat + "PlayersCount", value);
		}
	}

	public bool scaleButtons
	{
		get
		{
			return Base.PlayerPrefsGetBool(prefixplat + "scaleButtons", bs.android);
		}
		set
		{
			Base.PlayerPrefsSetBool(prefixplat + "scaleButtons", value);
		}
	}

	public bool enableBloom
	{
		get
		{
			return Base.PlayerPrefsGetBool(prefixplat + "enableBloom");
		}
		set
		{
			Base.PlayerPrefsSetBool(prefixplat + "enableBloom", value);
		}
	}

	public List<string> favorites
	{
		get
		{
			return Base.PlayerPrefsGetStrings("favs", string.Empty);
		}
		set
		{
			Base.PlayerPrefsSetStringList("favs", value);
		}
	}

	public List<string> friends
	{
		get
		{
			return Base.PlayerPrefsGetStrings(playerName + "friends", playerNamePrefixed);
		}
		set
		{
			Base.PlayerPrefsSetStringList(playerName + "friends", value);
		}
	}

	public ObscuredInt medals
	{
		get
		{
			return GetSecureOff(Base.PlayerPrefsGetInt(playerName + "medals"), "zxc", 0);
		}
		set
		{
			Base.PlayerPrefsSetInt(playerName + "medals", SetSecure(value, "zxc"));
		}
	}

	public ObscuredInt warScore
	{
		get
		{
			return Base.PlayerPrefsGetInt(playerName + "warPoints");
		}
		set
		{
			Base.PlayerPrefsSetInt(playerName + "warPoints", value);
		}
	}

	public ObscuredInt reputation
	{
		get
		{
			return GetSecureOff(Base.PlayerPrefsGetInt(playerName + "reputation"), "dsa", 0);
		}
		set
		{
			Base.PlayerPrefsSetInt(playerName + "reputation", SetSecure(value, "dsa"));
		}
	}

	public ObscuredInt friendCount
	{
		get
		{
			return Base.PlayerPrefsGetInt("friendCount", -1);
		}
		set
		{
			Base.PlayerPrefsSetInt("friendCount", value);
		}
	}

	public ObscuredInt score
	{
		get
		{
			return GetSecureOff(Base.PlayerPrefsGetInt(playerName + "score"), "dsa2", 0);
		}
		set
		{
			Base.PlayerPrefsSetInt(playerName + "score", SetSecure(value, "dsa2"));
		}
	}

	public int controls
	{
		get
		{
			int? controls = m_controls;
			int value;
			if (controls.HasValue)
			{
				value = controls.Value;
			}
			else
			{
				int? num = m_controls = Base.PlayerPrefsGetInt(prefixplat + "controls2", Contr.keys);
				value = num.Value;
			}
			return value;
		}
		set
		{
			string s = prefixplat + "controls2";
			int? num = m_controls = value;
			Base.PlayerPrefsSetInt(s, num.Value);
		}
	}

	public bool enableMouse
	{
		get
		{
			return !bs._Loader.dm && m_enableMouse;
		}
		set
		{
			m_enableMouse = value;
		}
	}

	public int drawDistance
	{
		get
		{
			int? drawDistance = m_drawDistance;
			int value;
			if (drawDistance.HasValue)
			{
				value = drawDistance.Value;
			}
			else
			{
				int? num = m_drawDistance = Base.PlayerPrefsGetInt(prefixplat + "drawDistance", 1000);
				value = num.Value;
			}
			return value;
		}
		set
		{
			string s = prefixplat + "drawDistance";
			int? num = m_drawDistance = value;
			Base.PlayerPrefsSetInt(s, num.Value);
		}
	}

	public int modTypeInt
	{
		get
		{
			int? modType = m_modType;
			int value;
			if (modType.HasValue)
			{
				value = modType.Value;
			}
			else
			{
				int? num = m_modType = Base.PlayerPrefsGetInt(prefixplat + "modType");
				value = num.Value;
			}
			return value;
		}
		set
		{
			string s = prefixplat + "modType";
			int? num = m_modType = value;
			Base.PlayerPrefsSetInt(s, num.Value);
		}
	}

	public ModType modType => (ModType)((!bs.isDebug) ? modTypeInt : 100);

	public Quality2 _quality
	{
		get
		{
			Quality2? quality = m_quality;
			Quality2 value;
			if (quality.HasValue)
			{
				value = quality.Value;
			}
			else
			{
				Quality2? quality2 = m_quality = (Quality2)Base.PlayerPrefsGetInt(prefixplat + "quality", 1);
				value = quality2.Value;
			}
			return value;
		}
		set
		{
			string s = prefixplat + "quality";
			Quality2? quality = m_quality = value;
			Base.PlayerPrefsSetInt(s, (int)quality.Value);
		}
	}

	public Difficulty difficulty
	{
		get
		{
			Difficulty? difficulty = m_difficulty;
			Difficulty value;
			if (difficulty.HasValue)
			{
				value = difficulty.Value;
			}
			else
			{
				Difficulty? difficulty2 = m_difficulty = (Difficulty)Base.PlayerPrefsGetInt(prefixplat + "difficulty", 1);
				value = difficulty2.Value;
			}
			return value;
		}
		set
		{
			string s = prefixplat + "difficulty";
			Difficulty? difficulty = m_difficulty = value;
			Base.PlayerPrefsSetInt(s, (int)difficulty.Value);
		}
	}

	public bool autoQuality
	{
		get
		{
			bool? autoQuality = m_autoQuality;
			bool value;
			if (autoQuality.HasValue)
			{
				value = autoQuality.Value;
			}
			else
			{
				bool? flag = m_autoQuality = Base.PlayerPrefsGetBool(prefixplat + "autoQuality", !bs.androidPlatform);
				value = flag.Value;
			}
			return value;
		}
		set
		{
			string s = prefixplat + "autoQuality";
			bool? flag = m_autoQuality = value;
			Base.PlayerPrefsSetBool(s, flag.Value);
		}
	}

	public bool rearCamera
	{
		get
		{
			bool? rearCamera = m_rearCamera;
			bool value;
			if (rearCamera.HasValue)
			{
				value = rearCamera.Value;
			}
			else
			{
				bool? flag = m_rearCamera = Base.PlayerPrefsGetBool(prefixplat + "rearCamera");
				value = flag.Value;
			}
			return value;
		}
		set
		{
			string s = prefixplat + "rearCamera";
			bool? flag = m_rearCamera = value;
			Base.PlayerPrefsSetBool(s, flag.Value);
		}
	}

	public int playedTimes
	{
		get
		{
			return Base.PlayerPrefsGetInt("playedTimes");
		}
		set
		{
			Base.PlayerPrefsSetInt("playedTimes", value);
		}
	}

	public int msgId
	{
		get
		{
			return Base.PlayerPrefsGetInt("msgId");
		}
		set
		{
			Base.PlayerPrefsSetInt("msgId", value);
		}
	}

	public bool reverseSplitScreen
	{
		get
		{
			bool? reverseSplitScreen = m_reverseSplitScreen;
			bool value;
			if (reverseSplitScreen.HasValue)
			{
				value = reverseSplitScreen.Value;
			}
			else
			{
				bool? flag = m_reverseSplitScreen = Base.PlayerPrefsGetBool(prefixplat + "reverseSplitScreen", bs.android && !ouya);
				value = flag.Value;
			}
			return value;
		}
		set
		{
			string s = prefixplat + "reverseSplitScreen";
			bool? flag = m_reverseSplitScreen = value;
			Base.PlayerPrefsSetBool(s, flag.Value);
		}
	}

	private static bool ouya => !string.IsNullOrEmpty(SystemInfo.deviceModel) && SystemInfo.deviceModel.ToLower().Contains("ouya");

	internal float soundVolume2
	{
		get
		{
			float? audioVolume = m_audioVolume;
			float value;
			if (audioVolume.HasValue)
			{
				value = audioVolume.Value;
			}
			else
			{
				float? num = m_audioVolume = ((!bs.flash || bs.isDebug) ? Base.PlayerPrefsGetFloat("audioVolume2", 0.5f) : 0f);
				value = num.Value;
			}
			return value;
		}
		set
		{
			float? num = m_audioVolume = value;
			Base.PlayerPrefsSetFloat("audioVolume2", num.Value);
		}
	}

	public float musicVolume
	{
		get
		{
			float? musicVolume = m_musicVolume;
			float value;
			if (musicVolume.HasValue)
			{
				value = musicVolume.Value;
			}
			else
			{
				float? num = m_musicVolume = Base.PlayerPrefsGetFloat(prefixplat + "musicVolume2", 0.5f);
				value = num.Value;
			}
			return value;
		}
		set
		{
			string s = prefixplat + "musicVolume2";
			float? num = m_musicVolume = value;
			Base.PlayerPrefsSetFloat(s, num.Value);
		}
	}

	public float flying
	{
		get
		{
			float? flying = m_flying;
			float value;
			if (flying.HasValue)
			{
				value = flying.Value;
			}
			else
			{
				float? num = m_flying = Base.PlayerPrefsGetFloat(prefixMapPl + "flying", 0f);
				value = num.Value;
			}
			return value;
		}
		set
		{
			string s = prefixMapPl + "flying";
			float? num = m_flying = value;
			Base.PlayerPrefsSetFloat(s, num.Value);
		}
	}

	public float sensivity
	{
		get
		{
			float? sensivity = m_sensivity;
			float value;
			if (sensivity.HasValue)
			{
				value = sensivity.Value;
			}
			else
			{
				float? num = m_sensivity = Base.PlayerPrefsGetFloat(prefixplat + "sensivity", 1f);
				value = num.Value;
			}
			return value;
		}
		set
		{
			string s = prefixplat + "sensivity";
			float? num = m_sensivity = value;
			Base.PlayerPrefsSetFloat(s, num.Value);
		}
	}

	public bool shadows
	{
		get
		{
			return Base.PlayerPrefsGetBool(prefixplat + "shadows") && !bs.androidPlatform;
		}
		set
		{
			Base.PlayerPrefsSetBool(prefixplat + "shadows", value);
		}
	}

	public bool enableMotionBlur
	{
		get
		{
			return Base.PlayerPrefsGetBool(prefixplat + "motionblur", defValue: true);
		}
		set
		{
			Base.PlayerPrefsSetBool(prefixplat + "motionblur", value);
		}
	}

	public bool screenshotTaken
	{
		get
		{
			return Base.PlayerPrefsGetBool("screenshotTaken");
		}
		set
		{
			Base.PlayerPrefsSetBool("screenshotTaken", value);
		}
	}

	public void OnInit()
	{
		if (UnityEngine.Object.FindObjectsOfType(typeof(Loader)).Length <= 1)
		{
			UpdateGlobals();
		}
	}

	private void UpdateGlobals()
	{
		bs._Loader = this;
		bs.setting = bs._Loader.m_setting;
		bs.res = res;
		bs.isDebug = bs.setting.debug;
		bs.android = bs.setting.m_android;
	}

	public void OnEnable()
	{
		MonoBehaviour.print("Set cultureInfo");
		if (Application.platform == RuntimePlatform.WP8Player)
		{
			Thread.CurrentThread.CurrentCulture = CultureInfo.InvariantCulture;
			Thread.CurrentThread.CurrentUICulture = CultureInfo.InvariantCulture;
		}
	}

	public override void Awake()
	{
		if (UnityEngine.Object.FindObjectsOfType(typeof(Loader)).Length > 1)
		{
			UnityEngine.Object.Destroy(base.gameObject);
			return;
		}
		try
		{
			PlayerPrefsObscured.SetString("temp", new string('A', 1024));
		}
		catch (Exception)
		{
			bs.LogEvent2("Overload");
			Debug.LogError("Overload");
			PlayerPrefsObscured.DeleteAll();
		}
		PlayerPrefsObscured.DeleteKey("temp");
		MonoBehaviour.print("Loader Awake");
		scenes = new List<Scene>(res.scenes);
		for (int i = 0; i < scenes.Count; i++)
		{
			scenes[i].mapId = 10000000 + i;
		}
		lastVersion = PlayerPrefs.GetInt("lastVersion");
		PlayerPrefs.SetInt("lastVersion", bs.setting.version);
		UpdateGlobals();
		bs.setting.InitSettings();
		UpdateGlobals();
		gameStartTime = Time.realtimeSinceStartup;
		UnityEngine.Object.DontDestroyOnLoad(base.gameObject);
		GA.SettingsGA.Build = bs.setting.version.ToString();
		if (Resources.Load("test") != null || bs.setting.dontLoadAssets)
		{
			resLoaded = (resLoaded2 = true);
		}
		base.Awake();
	}

	private IEnumerator LoadAssetBundle()
	{
		if (resLoaded)
		{
			yield break;
		}
		if (!maps.TryGetValue("resources" + bs.setting.packageVersion, out Map map2))
		{
			Debug.LogError("resources" + bs.setting.packageVersion + " not found");
			yield break;
		}
		wwwAssetBundle = WWW(map2.url, map2.version);
		yield return wwwAssetBundle;
		if (wwwAssetBundle.assetBundle != null)
		{
			bs.assetBundle.Add(wwwAssetBundle.assetBundle);
			MonoBehaviour.print("asset bundle loaded");
		}
		else
		{
			Debug.LogError("www.assetBundle is null");
		}
		map2 = maps["resourcesa" + bs.setting.packageVersion2];
		wwwAssetBundle = WWW(map2.url, map2.version);
		yield return wwwAssetBundle;
		if (wwwAssetBundle.assetBundle != null)
		{
			bs.assetBundle.Add(wwwAssetBundle.assetBundle);
		}
		else
		{
			Debug.LogError("www.assetBundle2 is null");
		}
		resLoaded2 = (bs.assetBundle.Count >= 1);
		resLoaded = true;
	}

	public void Start()
	{
		//IL_025a: Unknown result type (might be due to invalid IL or missing references)
		//IL_0264: Expected O, but got Unknown
		GA.API.Quality.NewEvent("Quality Test", "test");
		SetOffline(offline: true);
		if (bs.setting.autoHost)
		{
			playerName = "host";
		}
		if (bs.setting.autoConnect)
		{
			playerName = "client";
		}
		res.waterMaterial.shader = res.waterMaterialDef;
		if (bs.setting.autoConnect || bs.setting.autoHost)
		{
			playerName += UnityEngine.Random.Range(0, 99);
			bs._Loader.carskin = UnityEngine.Random.Range(0, bs._Loader.CarSkins.Count - 1);
			bs._Loader.avatar = UnityEngine.Random.Range(0, res.avatars.Length);
		}
		if (!UnityEngine.PlayerPrefs.HasKey("playerName") && !UnityEngine.PlayerPrefs.HasKey("firstTime"))
		{
			StartCoroutine(FirstTime());
		}
		bs.LogEvent("Start");
		if (!bs.android && !string.IsNullOrEmpty(Application.absoluteURL))
		{
			try
			{
				string text = Application.absoluteURL.Split('/')[0];
				if (!string.IsNullOrEmpty(text))
				{
					bs.LogEvent(EventGroup.SiteOld, "referers/" + text);
				}
			}
			catch (Exception)
			{
			}
		}
		bs.Download(bs.mainSite + "scripts/country.php", delegate(string s, bool b)
		{
			if (b)
			{
				bs._Loader.contry = s.ToLower();
			}
		}, false);
		StartCoroutine(StartLoad());
		MonoBehaviour.print("Init Banner");
		if (showBanner)
		{
			SamsungAd.Instance().Init("xv0d00000002jl", "xv0d00000002jl");
		}
		Everyplay.SharedInstance.RecordingStopped += delegate
		{
			everyplayRecorded = true;
		};
		Everyplay.SharedInstance.UploadDidComplete += delegate
		{
			showBanner = false;
		};
		if (bs.setting.skipLogin || bs.setting.autoHost || bs.setting.autoConnect)
		{
			bs.guest = (loggedIn = true);
			StartCoroutine(Base.AddMethod(0.1f, (Action)(object)(Action)delegate
			{
				OnLoggedIn(g: true);
			}));
		}
		bs.Download(bs.mainSite + "scripts/time.php", delegate(string s, bool b)
		{
			if (b)
			{
				serverTime = DateTime.Parse(s);
			}
			MonoBehaviour.print("Serve rTime " + serverTime);
		}, false);
		GA.API.Quality.NewEvent(bs.setting.version.ToString());
		DownloadUserMaps2(bs._Loader.page);
	}

	public IEnumerator StartLoad()
	{
		yield return StartCoroutine(LoadingScreen.StartLoadLevels());
		if (!bs.setting.dontLoadAssets)
		{
			StartCoroutine(LoadAssetBundle());
		}
		StartCoroutine(LoadTextres());
		if (bs.setting.unitTest)
		{
			sGameType = SGameType.VsPlayers;
			mapName = "a02";
			StartCoroutine(StartLoadLevel(mapName));
		}
	}

	public bool BonusCheck()
	{
		DateTime? dateTime = serverTime;
		if (!dateTime.HasValue || bs.isDebug)
		{
			return false;
		}
		bool result = false;
		int num = (int)(serverTime.Value.Ticks / 864000000000L);
		if (num - lastDayPlayed > 0)
		{
			if (num - lastDayPlayed == 1)
			{
				dayBonus++;
			}
			else
			{
				dayBonus = 0;
			}
			result = true;
		}
		lastDayPlayed = num;
		return result;
	}

	private void BonusWindow()
	{
		Setup(750, 350, string.Empty);
		bs.win.showBackButton = false;
		bs.win.windowTexture = guiSkins.windowTexture;
		base.skin.window.contentOffset = new Vector2(0f, -135f);
		GUILayout.Label(GuiClasses.Tr("By playing this game every day, your bonus is increased"), guiSkins.bonusText);
		GUILayout.BeginHorizontal();
		GUILayout.FlexibleSpace();
		int num = dayBonus % 7 + 1;
		if (num == 7)
		{
			_Awards.Play7Days.count++;
		}
		for (int i = 1; i <= 7; i++)
		{
			GUIStyle gUIStyle = new GUIStyle(guiSkins.bonus);
			if (i <= num)
			{
				gUIStyle.normal.background = guiSkins.selectedBonus;
			}
			GUILayout.Button(new GUIContent(i + GuiClasses.Tr(" day"), (i > 4) ? guiSkins.bonusTexture : guiSkins.selectedBonusTexture), gUIStyle);
		}
		GUILayout.FlexibleSpace();
		GUILayout.EndHorizontal();
		int num2 = Mathf.Max(num - 4, 0) * 2;
		int num3 = num * 2;
		GUILayout.Label(string.Format(GuiClasses.Tr("Today you got: {0} medals and {1} reputation points"), num3, num2), guiSkins.bonusText);
		GUILayout.FlexibleSpace();
		if (Button("Take"))
		{
			medals = (int)medals + num3;
			reputation = (int)reputation + num2;
			WindowPool();
		}
	}

	public void OnLevelWasLoaded(int level)
	{
		MonoBehaviour.print("OnLevelWasLoaded " + level);
		errors = 0;
		if (level != 0 && showBanner)
		{
			SamsungAd.Instance().DestroyBannerAd();
		}
	}

	public IEnumerator LoadTextres()
	{
		m_thumbnails = new Dictionary<string, List<Thumbnail>>();
		if (thumbnailKeys != null)
		{
			for (int i = 0; i < thumbnailKeys.Length; i++)
			{
				thumbnails[thumbnailKeys[i]] = thumbnailValues[i];
			}
			yield break;
		}
		WWW w = new WWW(bs.mainSite + "/docs/textures/getfiles.php");
		yield return w;
		string[] array = bs.SplitString(w.text);
		foreach (string a in array)
		{
			string[] ss = a.Split('/');
			Thumbnail thumbnail = new Thumbnail
			{
				url = bs.mainSite + "/docs/textures/" + Uri.EscapeUriString(a),
				name = ss[1]
			};
			if (thumbnails.TryGetValue(ss[0], out List<Thumbnail> ths))
			{
				ths.Add(thumbnail);
				continue;
			}
			thumbnails[ss[0]] = new List<Thumbnail>(new Thumbnail[1]
			{
				thumbnail
			});
		}
		thumbnailKeys = thumbnails.Keys.ToArray();
		thumbnailValues = thumbnails.Values.ToArray();
	}

	private IEnumerator FirstTime()
	{
		yield return null;
		UnityEngine.PlayerPrefs.SetString("firstTime", "1");
		bs.LogEvent("First Time");
		MonoBehaviour.print("Beginner");
		Beginner = true;
	}

	private void LoadScenes()
	{
		MonoBehaviour.print("LoadScenes " + userMaps.Count);
		res.scenes = new List<Scene>();
		string[] array = bs.SplitString(scenesTxt.text);
		List<string> list = new List<string>();
		int num = 0;
		for (int i = 1; i < array.Length; i++)
		{
			string text = array[i];
			string[] array2 = text.Split('\t');
			string text2 = array2[0];
			if (text2.StartsWith("_"))
			{
				string item = text2.Substring(1);
				list.Add(item);
				num++;
			}
			else
			{
				scenes.Add(new Scene
				{
					name = text2,
					j = Mathf.Max(0, num - 1),
					nitro = ((num > 3) ? 3 : 0)
				});
			}
		}
		scenes.AddRange(userMaps);
		Tag.userTab = list.Count;
		list.Add("user");
		titles = list.ToArray();
		SetDirty(res);
	}

	public void Update()
	{
		//IL_020d: Unknown result type (might be due to invalid IL or missing references)
		//IL_0219: Unknown result type (might be due to invalid IL or missing references)
		//IL_0223: Expected O, but got Unknown
		//IL_0223: Expected O, but got Unknown
		if (KeyDebug(KeyCode.T))
		{
			StartCoroutine(SavePlayerPrefs(skip: true));
		}
		if (KeyDebug(KeyCode.N))
		{
			Url("https://vk.com/app3935060?soulkey.stas2001.dasdasdasd.rep.Игорь левочкин");
		}
		if (Input.touchCount > 0)
		{
			Vector2 touchDelta = Vector2.zero;
			if (oldTouch != Vector2.zero)
			{
				touchDelta = Input.touches[0].position - oldTouch;
			}
			oldTouch = Input.touches[0].position;
			bs.touchDelta = touchDelta;
		}
		else
		{
			oldTouch = Vector2.zero;
		}
		if (KeyDebug(KeyCode.Alpha1, "Refresh Keys"))
		{
			RefreshPrefs();
		}
		UpdateGlobals();
		if (KeyDebug(KeyCode.I, "Print all assets"))
		{
			MonoBehaviour.print("Test");
			Resources.UnloadUnusedAssets();
			Mesh[] array = (from Mesh a in Resources.FindObjectsOfTypeAll(typeof(Mesh))
				orderby a.vertexCount descending
				select a).ToArray();
			Texture2D[] textures = Resources.FindObjectsOfTypeAll(typeof(Texture2D)).Cast<Texture2D>().OrderBy<Texture2D, float>(delegate(Texture2D a)
			{
				Vector2 texelSize = a.texelSize;
				float x = texelSize.x;
				Vector2 texelSize2 = a.texelSize;
				return x + texelSize2.y;
			})
				.ToArray();
			StringBuilder stringBuilder = new StringBuilder("Meshes: " + array.Length + " Textues:" + textures.Length).AppendLine();
			Mesh[] array2 = array;
			foreach (Mesh mesh in array2)
			{
				stringBuilder.Append(mesh.name).Append(",");
			}
			string message = stringBuilder.ToString();
			MonoBehaviour.print(message);
			ShowWindow((Action)(object)(Action)delegate
			{
				base.skin.label.wordWrap = true;
				Setup(2000, 2000, string.Empty);
				GUILayout.Label(message);
				GuiClasses.scroll = GUILayout.BeginScrollView(GuiClasses.scroll);
				Texture2D[] array3 = textures;
				foreach (Texture2D image in array3)
				{
					GUILayout.Label(image);
				}
				GUILayout.EndScrollView();
			}, (Action)(object)new Action(MenuWindow));
		}
		enableMouse = (controls == Contr.acel || controls == Contr.mouse);
		playerNamePrefixed = ((!bs.guest) ? playerName : ("-" + playerName));
		AudioListener.volume = soundVolume2;
		if (oldTime != 0f)
		{
			deltaTime = Mathf.Min(0.3f, Time.realtimeSinceStartup - oldTime);
		}
		oldTime = Time.realtimeSinceStartup;
		if (KeyDebug(KeyCode.F))
		{
			Application.CaptureScreenshot("ScreenShot" + UnityEngine.Random.Range(0, 1000) + ".png", 2);
		}
		if (KeyDebug(KeyCode.J, "substractmecals"))
		{
			medals = (int)medals - 3;
		}
		if (Input.GetKeyDown(KeyCode.F1))
		{
			TakeScreenshot(showText: true);
		}
		if (Input.GetKeyDown(KeyCode.F12) || Input.GetKeyDown(KeyCode.F11))
		{
			FullScreen(fullscreen: true);
		}
		if (KeyDebug(KeyCode.K))
		{
			medals = (int)medals + 10;
			reputation = (int)reputation + 10;
		}
		if (KeyDebug(KeyCode.U, "Reset Settings"))
		{
			bs._Loader.ResetPrefs();
		}
		bs.Log(LoadingScreen.version + bs._Integration.site.ToString(), important: true);
		bs.Log(errors, errors > 0);
		if (lastError != null)
		{
			bs.Log(lastError, important: true);
		}
		bs.sbuilder.Append(bs.sbuilder2);
		guiText.text = bs.sbuilder.ToString();
		guiTextRight.text = bs.sbuilderRight.ToString();
		bs.sbuilderRight = new StringBuilder();
		bs.sbuilder = new StringBuilder();
		if (bs.setting.printPlayerPrefs)
		{
			PrintPrefs();
		}
		if (bs.flash || bs.android || bs.Nancl || bs._Loader.levelEditor != null || !vkSite)
		{
			fullScreen.enabled = false;
		}
		else
		{
			fullScreen.enabled = true;
			bool flag = fullScreen.HitTest(Input.mousePosition) && !bs.win.WindowHit;
			fullScreenColor = ((!flag) ? new Color(0.5f, 0.5f, 0.5f, 0.1f) : new Color(0.5f, 0.5f, 0.5f, 1f));
			if (flag && Input.GetMouseButtonDown(0))
			{
				TakeScreenshot(showText: true);
			}
		}
		if (evalCors.Count > 0)
		{
			EvalCor evalCor = evalCors.Dequeue();
			if (evalCor.args == null)
			{
				Application.ExternalEval(evalCor.s);
			}
			else
			{
				Application.ExternalCall(evalCor.s, evalCor.args);
			}
		}
		UpdateCenterText();
		UpdateMh();
	}

	public void SetValue(string values = "reputation=123", string comment = "")
	{
		bs.DownloadAcc("setValue2", delegate(string s, bool b)
		{
			Debug.Log(s);
		}, true, "values", values, "comment", comment);
	}

	public void FixedUpdate()
	{
		bs.sbuilder2 = new StringBuilder();
	}

	public void Url(string s)
	{
		MonoBehaviour.print("Url received " + s);
		LoadingScreen.SiteBlockCheck(s);
		urlReceived = true;
		url = s;
		StartCoroutine(bs._Integration.KongParse(s));
		isOdnoklasniki = url.ToLower().Contains("odnoklassniki.ru");
		bool flag = url.ToLower().Contains("vk.com");
		MonoBehaviour.print("UrlReceived odno " + isOdnoklasniki + " " + url);
		if (!string.IsNullOrEmpty(url) && (flag || isOdnoklasniki))
		{
			vkSite = (vk = true);
			GuiClasses.curDict = 1;
		}
		bs.LogEvent(EventGroup.Site, s);
	}

	protected IEnumerator StartLoadLevel(string map, bool online = false)
	{
		if (bs._Game != null)
		{
			yield break;
		}
		if (online)
		{
			replays.Clear();
		}
		bs.LogEvent(EventGroup.Maps, map + " Start Load");
		bs.LogEvent("Load Map Start");
		Debug.LogWarning(map);
		string text;
		map = (text = map.ToLower().Trim());
		mapName = text;
		if (curScene == null)
		{
			yield return StartCoroutine(DownloadUserMaps(0, (int)mapSets.levelFlags, mapName, 0, top: false, direct: true));
		}
		if (curScene != null && curScene.userMap)
		{
			Popup2("Loading");
			LoadLevel("levelLoader", map);
			yield break;
		}
		LoadingScreen.GetRandomTip();
		bs.win.ShowWindow((Action)(object)new Action(LoadingLevelMapWindow), null, skip: true);
		if (!bs.CanStreamedLevelBeLoaded(map) && maps.ContainsKey(map))
		{
			yield return StartCoroutine(LoadingScreen.LoadMap(map));
		}
		lastLog = string.Empty;
		yield return null;
		if (!bs.CanStreamedLevelBeLoaded("!2game"))
		{
			Popup2("game cant be loaded", (Action)(object)new Action(bs._Loader.MenuWindow));
			bs.LogEvent("Failed load game");
		}
		else if (!bs.CanStreamedLevelBeLoaded(map))
		{
			Popup2(GuiClasses.Tr("Loading Level failed ") + map, (Action)(object)new Action(bs._Loader.MenuWindow));
			bs.LogEvent(EventGroup.Maps, map + " Load Failed");
		}
		else
		{
			LoadLevel(map);
			LoadLevelAdditive("!2game");
		}
	}

	public static void OnLogCallBack(string condition, string stacktrace, LogType type)
	{
		log.Append(type).Append(':').Append(condition);
		if (type == LogType.Exception)
		{
			log.Append(":").Append(stacktrace);
		}
		log.AppendLine();
		lastLog = condition;
		if (type == LogType.Exception)
		{
			errors++;
		}
		if (!Application.isWebPlayer && Application.platform != RuntimePlatform.NaCl)
		{
			return;
		}
		string text = "Unity:" + condition + ((type != LogType.Exception) ? string.Empty : ("\r\n" + stacktrace));
		if (Application.platform == RuntimePlatform.NaCl)
		{
			Application.ExternalEval(text + "\r\n");
		}
		else
		{
			object str;
			switch (type)
			{
			case LogType.Exception:
				str = "error";
				break;
			case LogType.Log:
				str = "log";
				break;
			default:
				str = "warn";
				break;
			}
			Application.ExternalCall("console." + (string)str, text);
		}
		if (prints.Count > 5)
		{
			prints.RemoveAt(0);
		}
		prints.Add(condition);
	}

	private void LoadingLevelMapWindow()
	{
		//IL_00be: Unknown result type (might be due to invalid IL or missing references)
		//IL_00c9: Expected O, but got Unknown
		Setup(600, 600, "Loading");
		GUILayout.Label(curScene.texture, GUILayout.Height(100f), GUILayout.ExpandWidth(expand: true));
		LabelCenter(LoadingLabelMap());
		if (!bs.android)
		{
			autoFullScreen = Toggle(autoFullScreen, "Full Screen");
		}
		if (!bs.setting.vk2)
		{
			LabelCenter("Tip");
			GUILayout.Box(GuiClasses.Tr(LoadingScreen.randomTip));
		}
		if (Input.GetKeyDown(KeyCode.Escape))
		{
			ShowWindow((Action)(object)new Action(MenuWindow));
		}
	}

	public void OnLevelLoaded()
	{
		bs.SaveStrings();
		Time.timeScale = 1f;
		levelLoadTime = Time.time;
		MonoBehaviour.print("OnLevelLoaded: " + mapName + " " + oldLevel);
		if (bs._MapLoader == null)
		{
			bs.LogEvent(EventGroup.Maps, mapName);
		}
		else
		{
			bs.LogEvent("userMap");
		}
	}

	protected void PlayGuest()
	{
		//IL_0016: Unknown result type (might be due to invalid IL or missing references)
		//IL_002b: Expected O, but got Unknown
		bs.guest = true;
		bs.win.ShowWindow((Action)(object)new Action(bs._Loader.SetNickNameWindow), bs.win.act);
	}

	private static void Xor(byte[] array)
	{
		for (int i = 0; i < array.Length; i++)
		{
			array[i] = (byte)(array[i] ^ (i + 25));
		}
	}

	private static Dictionary<string, string> ParseDict(string text)
	{
		string[] array = bs.SplitString(text);
		Dictionary<string, string> dictionary = new Dictionary<string, string>();
		string[] array2 = array;
		foreach (string text2 in array2)
		{
			string[] array3 = text2.Split(':');
			if (array3.Length > 1)
			{
				dictionary[array3[0]] = array3[1];
			}
		}
		return dictionary;
	}

	protected void OnLoggedIn(bool g = false)
	{
		Debug.LogWarning("OnLoggedIn guest:" + g);
		bs._Integration.OnLoggedIn();
		if (!Beginner && !bs.guest && userId != 0)
		{
			bs.Download(bs.mainSite + "scripts/onPlayerJoin2.php", null, false, "name", userId);
		}
		loggedIn = true;
		bs.guest = g;
		StartCoroutine(MhSend(null));
		bs.SaveStrings();
		loggedInTime = bs.totalSeconds;
	}

	private void GoOffline()
	{
		OnLoggedIn(g: true);
		WindowPool();
	}

	public static string Cyrilic(string s)
	{
		char[] array = null;
		string text = "qwertyuiopasdfghjklzxcvbnm";
		string text2 = "яшертыуиопасдфгхйклзьчвбнм";
		for (int i = 0; i < s.Length; i++)
		{
			for (int j = 0; j < text2.Length; j++)
			{
				if (s[i] == text2[j])
				{
					if (array == null)
					{
						array = s.ToCharArray();
					}
					array[i] = text[j];
					break;
				}
			}
		}
		return (array != null) ? new string(array) : s;
	}

	public static string Filter(string textField)
	{
		string text = "qwertyuiopasdfghjklzxcvbnm1234567890";
		textField = Cyrilic(textField);
		List<char> list = new List<char>(text.ToCharArray());
		list.AddRange(text.ToUpper().ToCharArray());
		for (int i = 0; i < textField.Length; i++)
		{
			if (!list.Contains(textField[i]))
			{
				textField = textField.Remove(i, 1);
			}
		}
		return textField;
	}

	public string LoadingLabelMap()
	{
		if (isLoading)
		{
			return GuiClasses.Tr("Loading ", sk: true) + bs.GetFileNameWithoutExtension(mapWww.url) + " " + (int)((mapWww != null) ? (mapWww.progress * 100f) : 0f) + "%\n";
		}
		return string.Empty;
	}

	protected void KeyboardSetup()
	{
		Setup(700, 600, "Keyboard");
		foreach (KeyValue key in inputManger.keys)
		{
			string text = string.Empty;
			int num = 0;
			KeyCode[] keyCodeAlt = key.keyCodeAlt;
			foreach (KeyCode keyCode in keyCodeAlt)
			{
				num++;
				text = text + ((!(text == string.Empty)) ? ", " : string.Empty) + keyCode;
				if (num == 2)
				{
					break;
				}
			}
			Label((Trs(key.descr) + ":").PadRight(30) + text);
		}
	}

	public void StartLoadReplays()
	{
		//IL_002f: Unknown result type (might be due to invalid IL or missing references)
		//IL_003b: Expected O, but got Unknown
		bs._Loader.replays.Clear();
		if (bs._Game == null)
		{
			bs.win.ShowWindow((Action)(object)new Action(bs._Loader.LoadReplayWindow));
		}
		bs.Download(bs.mainSite + "scripts/getReplay3.php", LoadReplay, true, "hard", (difficulty != 0) ? ((difficulty != Difficulty.Normal) ? 1 : 2) : 0, "map", curScene.mapId, "flags", (int)replayFlags, "version", bs.setting.replayVersion, "count", (PlayersCount != 0) ? PlayersCount : 3);
	}

	private void LoadReplay(string text, bool success)
	{
		try
		{
			if (success)
			{
				StartCoroutine(LoadReadReplay(JsonMapper.ToObject(text)));
				return;
			}
		}
		catch (Exception message)
		{
			Debug.LogError(message);
		}
		if (bs._Game == null)
		{
			StartCoroutine(StartLoadLevel(mapName));
		}
	}

	protected IEnumerator LoadReadReplay(JsonData data)
	{
		tempReplays = new List<Replay>();
		List<WWW> wwws = new List<WWW>();
		IEnumerator enumerator = ((IEnumerable)data["replays"]).GetEnumerator();
		try
		{
			while (enumerator.MoveNext())
			{
				string url = string.Concat(arg1: ((JsonData)enumerator.Current)["path"], arg0: bs.mainSite);
				wwws.Add(new WWW(Uri.EscapeUriString(url)));
			}
		}
		finally
		{
			IDisposable disposable = enumerator as IDisposable;
			if (disposable != null)
			{
				disposable.Dispose();
			}
		}
		dontUploadReplay = (bool)data["dontUploadReplay"];
		MonoBehaviour.print("dontUploadReplay " + dontUploadReplay);
		while (wwws.Count > 0)
		{
			yield return null;
			for (int i = wwws.Count - 1; i >= 0; i--)
			{
				WWW www = wwws[i];
				if (www.isDone)
				{
					wwws.Remove(www);
					byte[] buffer2 = null;
					bool hasCache = UnityEngine.PlayerPrefs.HasKey(www.url) && bs.setting.wwwCache;
					if (!hasCache)
					{
						yield return www;
					}
					if (!string.IsNullOrEmpty(www.error) || bs.setting.offline || hasCache)
					{
						Debug.LogError("error:" + www.error + " " + www.url);
					}
					else
					{
						buffer2 = www.bytes;
						try
						{
							Replay rep = ReadReplay(buffer2);
							if (rep.posVels.Count < 100)
							{
								MonoBehaviour.print("replay is empty " + www.url);
							}
							else
							{
								tempReplays.Add(rep);
								GuiClasses.cnt--;
							}
						}
						catch (Exception ex)
						{
							Exception e = ex;
							Debug.LogError(e);
						}
					}
				}
			}
		}
		MonoBehaviour.print("Replays Loaded: " + tempReplays.Count);
		replays = tempReplays;
		yield return new WaitForSeconds(1f);
		StartCoroutine(StartLoadLevel(mapName));
	}

	private Replay ReadReplay(byte[] buffer)
	{
		Replay replay = new Replay();
		replay.contry = bs._Loader.Country;
		using (BinaryReader binaryReader = new BinaryReader(buffer))
		{
			Vector3 vector = Vector3.zero;
			float num = 0f;
			PosVel posVel = null;
			int num2 = 0;
			while (binaryReader.Position < binaryReader.Length)
			{
				int num3 = binaryReader.ReadByte();
				switch (num3)
				{
				case 13:
					replay.playerName = binaryReader.ReadString();
					break;
				case 14:
					replay.clanTag = binaryReader.ReadString();
					break;
				case 9:
					num2 = binaryReader.ReadInt();
					break;
				case 1:
					posVel = new PosVel();
					posVel.pos = binaryReader.ReadVector();
					if (vector == Vector3.zero)
					{
						vector = posVel.pos;
					}
					num = (posVel.meters = num + (posVel.pos - vector).magnitude);
					vector = posVel.pos;
					posVel.rot.eulerAngles = binaryReader.ReadVector();
					posVel.vel = binaryReader.ReadVector();
					replay.posVels.Add(posVel);
					break;
				case 12:
					posVel.score = binaryReader.ReadInt();
					break;
				case 5:
					posVel.mouserot = binaryReader.ReadFloat();
					break;
				case 8:
					posVel.skid = binaryReader.ReadFloat();
					break;
				case 2:
				{
					KeyCode keyCode = (KeyCode)binaryReader.ReadInt();
					float time = binaryReader.ReadFloat();
					bool down = binaryReader.ReadBool();
					replay.keyDowns.Add(new KeyDown
					{
						down = down,
						keyCode = keyCode,
						time = time
					});
					break;
				}
				case 3:
					replay.avatarId = binaryReader.ReadInt();
					break;
				case 4:
					replay.carSkin = binaryReader.ReadInt();
					break;
				case 6:
					replay.finnishTime = binaryReader.ReadFloat();
					break;
				case 7:
				{
					CountryCodes countryCodes = replay.contry = (CountryCodes)binaryReader.ReadByte();
					break;
				}
				case 10:
					replay.color = new Color(binaryReader.ReadFloat(), binaryReader.ReadFloat(), binaryReader.ReadFloat());
					break;
				case 11:
					replay.avatarUrl = binaryReader.ReadString();
					break;
				default:
					Debug.LogError("byte unknown type " + num3);
					break;
				}
			}
			MonoBehaviour.print("Replay version " + num2);
			return replay;
		}
	}

	public IEnumerator DownloadUserMaps(int tested, int levelFlags, string search = "", int p = 0, bool top = false, bool direct = false, int id = 0)
	{
		if (!bs.online)
		{
			levelFlags = 0;
		}
		if (userMaps.Count == 0)
		{
			p = 0;
		}
		string s = $"{bs.mainSite}scripts/getUserMaps2.php?version={bs.setting.levelEditorVersion}&map={search}&tested={tested}&page={p}&top={(top ? 1 : 0)}&direct={(direct ? 1 : 0)}&flags={levelFlags}&id={id}";
		if (tested == 0)
		{
			s = s + "&user=" + playerNamePrefixed;
		}
		if (bs.isDebug)
		{
			s = s + "&r=" + UnityEngine.Random.value;
		}
		WWW w = wwwUserMaps = new WWW(s);
		MonoBehaviour.print(w.url);
		yield return w;
		if (!string.IsNullOrEmpty(w.error))
		{
			MonoBehaviour.print(w.error);
			yield break;
		}
		userMaps.Clear();
		string[] ss = bs.SplitString(w.text.Trim());
		string[] array = ss;
		foreach (string a3 in array)
		{
			string[] a2 = a3.Split(';');
			string a = a2[0];
			string substring = a.Substring(a.LastIndexOf("/") + 1);
			string[] strings = substring.Split('.');
			string plName = strings[0];
			Scene sc = new Scene
			{
				mapBy = plName,
				name = strings[0] + "." + strings[1],
				title = strings[1],
				j = Tag.userTab,
				url = a,
				userMap = true,
				rating = float.Parse(a2[1])
			};
			if (a2.Length > 2)
			{
				sc.mapSets.levelFlags = (LevelFlags)int.Parse(a2[2]);
				sc.mapId = int.Parse(a2[3]);
			}
			userMaps.Add(sc);
		}
		page = p;
		MonoBehaviour.print("userMaps loaded:" + userMaps.Count);
		GuiClasses.scroll = Vector2.zero;
		scenes = new List<Scene>(res.scenes);
		scenes.AddRange(userMaps);
	}

	protected void VoteMapOB()
	{
	}

	public WWW AproveMap(int tested = 1, int advanced = 0)
	{
		return bs.Download($"{bs.mainSite}scripts/aproveMap2.php?map={bs._Loader.mapName}&tested={tested}&aprovedBy={playerNamePrefixed}&advanced={advanced}", null, false);
	}

	public void RateMapWindow()
	{
		Setup(400, 400, string.Empty);
		bs.win.showBackButton = false;
		GUILayout.Label(GuiClasses.Tr("Please rate map: ") + bs._Loader.mapName);
		if (bs.isMod && curSceneDef.userMap)
		{
			GUILayout.BeginHorizontal();
			if (Button("Remove"))
			{
				AproveMap(0);
			}
			if (Button("Aprove"))
			{
				AproveMap();
			}
			if (Button("Fav"))
			{
				AproveMap(2);
			}
			GUILayout.EndHorizontal();
		}
		for (int i = 1; i <= 5; i++)
		{
			if (GUILayout.Button(res.rating[i * 2]))
			{
				bs.Download(bs.mainSite + "scripts/rateMap2.php?map=" + bs._Loader.curScene.mapId + "&rate=" + i, null, false);
				Base.PlayerPrefsSetBool("voted" + mapName, value: true);
				if (i == 5 && !bs._Loader.favorites.Contains(mapName))
				{
					bs._Loader.favorites.Add(mapName);
				}
				bs.win.Back();
			}
		}
	}

	public void FullScreen(bool fullscreen)
	{
		Screen.SetResolution(Screen.currentResolution.width, Screen.currentResolution.height, fullscreen);
	}

	private void PrintPrefs()
	{
		foreach (string playerPrefKey in Base.playerPrefKeys)
		{
		}
		if (Application.platform == RuntimePlatform.WindowsWebPlayer)
		{
			for (int num = prints.Count - 1; num >= 0; num--)
			{
			}
		}
	}

	public void RestartLevel()
	{
		MonoBehaviour.print("Restart Level " + bs._Loader.mapName);
		LoadLevel(bs._Loader.mapName);
	}

	public void OnApplicationQuit()
	{
		MonoBehaviour.print("OnApplicationQuit");
		loadingLevelQuit = true;
		bs.SaveStrings();
		if (!Application.isEditor || bs.isDebug)
		{
			bs.LogEvent(EventGroup.playedTime, "played " + (int)((Time.realtimeSinceStartup - gameStartTime) / 60f) + "minutes");
		}
	}

	public static WWW WWW(string url, int version)
	{
		if (Application.platform == RuntimePlatform.FlashPlayer)
		{
			return new WWW(url + "?rnd=" + version);
		}
		return UnityEngine.WWW.LoadFromCacheOrDownload(url, (!bs.setting.noLevelCache) ? version : UnityEngine.Random.Range(0, 1000));
	}

	public void LoadCarSkins()
	{
		res.CarSkins = new List<CarSkin>();
		string[] array = bs.SplitString(bs.setting.carSkinsTxt.text);
		for (int i = 0; i < array.Length; i++)
		{
			string prefabName = array[i];
			CarSkin carSkin = new CarSkin();
			carSkin.prefabName = prefabName;
			if (i == array.Length - 1)
			{
				carSkin.friendsNeeded = 3;
			}
			else
			{
				carSkin.medalsNeeded = (int)((float)i / (float)array.Length * (float)(SceneCount * 2));
			}
			res.CarSkins.Add(carSkin);
		}
	}

	public CarSkin GetCarSkin(int id, bool mine)
	{
		return CarSkins[(bs._Loader.quality != 0 || mine) ? Mathf.Clamp(id, 0, CarSkins.Count) : 0];
	}

	public void UpdateCenterText()
	{
		if (Time.time - lastTextTime > 0f && centerTextList.Count > 0)
		{
			stringTime stringTime = centerTextList.Dequeue();
			CenterText.text = stringTime.s;
			lastTextTime = Time.time + stringTime.f;
		}
		GUITexture centerTextBackground = CenterTextBackground;
		bool enabled = Time.time - lastTextTime < 0f;
		CenterText.enabled = enabled;
		centerTextBackground.enabled = enabled;
	}

	[RPC]
	public void centerText(string s, float seconds = 4f)
	{
		stringTime stringTime = default(stringTime);
		stringTime.f = seconds;
		stringTime.s = s;
		stringTime item = stringTime;
		if (!centerTextList.Contains(item))
		{
			centerTextList.Enqueue(item);
		}
	}

	public void WindowPool()
	{
		StartCoroutine(WindowPoolCor());
	}

	public IEnumerator WindowPoolCor()
	{
		Debug.Log("Window Pool");
		if (bs._Loader.levelEditor != null)
		{
			yield break;
		}
		if (bs.online && PhotonNetwork.connected)
		{
			bs.win.ShowWindow((Action)(object)new Action(MultiplayerWindow));
		}
		else if (bs._Game != null)
		{
			bs.win.ShowWindow((Action)(object)new Action(bs._Game.MenuWindow));
		}
		else if (GuiClasses.curDict == -1)
		{
			bs.win.ShowWindow((Action)(object)new Action(SelectLangWindow));
		}
		else if (!loggedIn)
		{
			bs.win.ShowWindow((Action)(object)new Action(LoginWindow));
		}
		else if (avatar == -1)
		{
			bs.win.ShowWindow((Action)(object)new Action(SelectAvatar), bs.win.act);
		}
		else if (carskin == -1)
		{
			LoadLevel("!2carselect");
		}
		else if (WonCar != null)
		{
			ShowWindow((Action)(object)new Action(WonCarWindow));
		}
		else if (gamePlayed)
		{
			gamePlayed = false;
			if (sGameType == SGameType.Clan || sGameType == SGameType.VsPlayers)
			{
				bs.win.ShowWindow((Action)(object)new Action(SelectMapWindow));
			}
			else
			{
				bs.win.ShowWindow((Action)(object)new Action(MenuWindow));
			}
			StartCoroutine(SavePlayerPrefs());
		}
		else if (BonusCheck())
		{
			ShowWindow((Action)(object)new Action(BonusWindow), bs.win.act);
		}
		else
		{
			bs.win.ShowWindow((Action)(object)new Action(MenuWindow));
		}
	}

	public void QuitWindow()
	{
	}

	public void CloseWindow()
	{
		bs.win.showBackButton = false;
		Label("Do you want to exit?");
		GUILayout.BeginHorizontal();
		if (Button("Yes") || Input.GetKeyDown(KeyCode.Escape))
		{
			MonoBehaviour.print("Application QUIT!");
			Application.Quit();
		}
		if (Button("No"))
		{
			bs.win.Back();
		}
		GUILayout.EndHorizontal();
	}

	private void WonCarWindow()
	{
		Setup(400, 230, string.Empty);
		LabelCenter("Congratulations! Unlocked new car, do you want to view it?", 16, wrap: true);
		GUILayout.BeginHorizontal();
		if (Button("No"))
		{
			WindowPool();
		}
		GUILayout.FlexibleSpace();
		if (Button("Yes"))
		{
			LoadLevel("!2carselect");
		}
		GUILayout.EndHorizontal();
	}

	public void SelectLangWindow()
	{
		//IL_00a9: Unknown result type (might be due to invalid IL or missing references)
		//IL_00b5: Unknown result type (might be due to invalid IL or missing references)
		//IL_00bf: Expected O, but got Unknown
		//IL_00bf: Expected O, but got Unknown
		Setup(400, 500, string.Empty);
		for (int i = 0; i < bs.setting.assetDictionaries.Length; i++)
		{
			if (ButtonTexture(bs.setting.assetDictionaries[i].name, flags[i]))
			{
				GuiClasses.curDict = i;
				GuiClasses.trcache.Clear();
				WindowPool();
			}
		}
		if (!bs._Loader.vk && !Beginner && Button("Help translate to other languages"))
		{
			ShowWindow((Action)(object)new Action(HelpTranslateWindow), (Action)(object)new Action(SelectLangWindow));
		}
	}

	private void HelpTranslateWindow()
	{
		Setup(700, 300, string.Empty);
		GUILayout.TextArea("Translate txt file here https://www.dropbox.com/sh/jhz61bchvbu1bd1/Amn12QIjrZ and send me to soulkey4@gmail.com, Thanks!");
	}

	private string PasswordField(string txt, char c)
	{
		bool flag = GuiClasses.JoystickButton2(base.skin.textField);
		string text = "MyTextField" + CustomWindow.buttonId;
		if (flag)
		{
			GUI.FocusControl(text);
		}
		GUI.SetNextControlName(text);
		return GUILayout.PasswordField(txt, c);
	}

	private string TextField(string txt, int i = -1)
	{
		bool flag = GuiClasses.JoystickButton2(base.skin.textField);
		string text = "MyTextField" + CustomWindow.buttonId;
		if (flag)
		{
			GUI.FocusControl(text);
		}
		GUI.SetNextControlName(text);
		return GUILayout.TextField(txt, i);
	}

	internal void LoginWindow()
	{
		//IL_017d: Unknown result type (might be due to invalid IL or missing references)
		//IL_0189: Unknown result type (might be due to invalid IL or missing references)
		//IL_0194: Expected O, but got Unknown
		//IL_0194: Expected O, but got Unknown
		//IL_0317: Unknown result type (might be due to invalid IL or missing references)
		//IL_032b: Expected O, but got Unknown
		bs.win.Setup(500, 400, string.Empty, Dock.Center, null, null, 1f);
		if (LoadingScreen.block)
		{
			bs.win.Setup(400, 200, string.Empty, Dock.Center, null, null, 1f);
			base.skin.button.wordWrap = true;
			if (GUILayout.Button(LoadingScreen.newVersionAvaibleText))
			{
				string text2 = LoadingScreen.newVersionAvaibleText.Substring(LoadingScreen.newVersionAvaibleText.IndexOf("http", StringComparison.Ordinal));
				Application.OpenURL(text2);
			}
			return;
		}
		if (Input.GetKeyDown(KeyCode.Escape))
		{
			ApplicationQuit();
		}
		gamePlayed = false;
		Label((!bs._Loader.vk) ? "Login:" : "Enter your nick:");
		playerName = TextField(Filter(playerName), maxLength);
		if (bs.Hotkey())
		{
			OnLoginFailed(string.Empty);
		}
		if (!bs._Loader.vk)
		{
			Label("password:");
			password = PasswordField(password, '*');
			rememberPassword = Toggle(rememberPassword, "Remember password");
			GUILayout.BeginHorizontal();
			if (Button("Register"))
			{
				bs.win.ShowWindow((Action)(object)new Action(RegisterWindow), (Action)(object)new Action(LoginWindow));
			}
		}
		else
		{
			GUILayout.BeginHorizontal();
		}
		if (Button((!bs._Loader.vk) ? "Login" : "ОК"))
		{
			bs.SaveStrings();
			if (vk && !bs._Integration.vkLoggedIn)
			{
				GoOffline();
			}
			else if (CheckLength())
			{
				Popup2("Logging in");
				bs.Download(bs.mainSite + "scripts/" + ((!bs._Loader.vk) ? "login3.php" : "loginRegister2.php"), delegate(string text, bool success)
				{
					if (text.StartsWith("registered") || text.StartsWith("login success") || bs.setting.ForceLogin)
					{
						StartCoroutine(LoadPlayerPrefs(text));
					}
					else
					{
						OnLoginFailed(text);
					}
				}, false, "name", playerName, "password", password, "vkPassword", vkPassword, "deviceId", SystemInfo.deviceUniqueIdentifier);
			}
		}
		GUILayout.EndHorizontal();
		GUILayout.FlexibleSpace();
		GUILayout.BeginHorizontal();
		if (!bs._Loader.vk && Button("Play as guest", expandWidth: false))
		{
			PlayGuest();
		}
		if (!bs._Loader.vk && Button("Restore password", expandWidth: false))
		{
			ShowWindow((Action)(object)new Action(RestorePasswordWindow), bs.win.act);
		}
		GUILayout.EndHorizontal();
		if (bs._Loader.vk && !bs._Integration.vkLoggedIn)
		{
			GUILayout.Label("1");
		}
	}

	private void OnLoginFailed(string text)
	{
		//IL_001c: Unknown result type (might be due to invalid IL or missing references)
		//IL_0028: Unknown result type (might be due to invalid IL or missing references)
		//IL_0032: Expected O, but got Unknown
		//IL_0032: Expected O, but got Unknown
		ShowWindow((Action)(object)(Action)delegate
		{
			if (Button("play as guest"))
			{
				GoOffline();
			}
			Label(text);
		}, (Action)(object)new Action(LoginWindow));
	}

	private void RestorePasswordWindow()
	{
		Label("Enter your email");
		email = TextField(email.ToLower());
		if (Button("Send Password"))
		{
			bs.Download(bs.mainSite + "/emails/" + bs.GetMD5Hash(email) + ".txt", delegate(string s, bool b)
			{
				if (b)
				{
					showPass = s;
				}
				else
				{
					Popup2("Email not found", bs.win.act);
				}
			}, false);
		}
		if (showPass != null)
		{
			GUILayout.TextField(GuiClasses.Tr("Your password:") + showPass);
		}
	}

	private void RegisterWindow()
	{
		//IL_017c: Unknown result type (might be due to invalid IL or missing references)
		//IL_0193: Expected O, but got Unknown
		bs.win.Setup(400, 400, string.Empty, Dock.Center, null, null, 1f);
		Label("Name:");
		playerName = Filter(TextField(playerName, maxLength));
		Label("password:");
		password = PasswordField(password, '*');
		Label("confirm password:");
		confirmPassword = PasswordField(confirmPassword, '*');
		Label("Email");
		email = TextField(email.ToLower());
		GUILayout.BeginHorizontal();
		GUILayout.FlexibleSpace();
		if (Button("Register"))
		{
			if (!Debug.isDebugBuild && !Regex.IsMatch(email, "[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?", RegexOptions.IgnoreCase))
			{
				Popup2("Please enter valid email address, required for password recovery", bs.win.act);
			}
			else if (confirmPassword != password)
			{
				Popup2("Passwords not match", bs.win.act);
			}
			else if (CheckLength())
			{
				Popup2("Registering", (Action)(object)new Action(RegisterWindow));
				bs.Download(bs.mainSite + "scripts/register.php", delegate(string text, bool success)
				{
					//IL_0032: Unknown result type (might be due to invalid IL or missing references)
					//IL_0049: Expected O, but got Unknown
					if (text.StartsWith("registration success"))
					{
						statsSaved = true;
						OnLoggedIn();
						WindowPool();
					}
					else
					{
						Popup2(text, (Action)(object)new Action(RegisterWindow));
					}
				}, false, "name", playerName, "password", password, "email", email);
			}
		}
		GUILayout.EndHorizontal();
	}

	public void SetNickNameWindow()
	{
		//IL_00ab: Unknown result type (might be due to invalid IL or missing references)
		//IL_00c2: Expected O, but got Unknown
		bs.win.Setup(300, 200, string.Empty, Dock.Center, null, null, 1f);
		if (Input.GetKeyDown(KeyCode.Escape))
		{
			bs.win.Back();
		}
		bs.win.showBackButton = false;
		LabelCenter("Enter Your nick name");
		playerName = TextField(Filter(playerName), maxLength);
		if (ButtonLeft("OK", null, 0f))
		{
			if (playerName.Length < minLength)
			{
				Popup2("Enter Your Name", (Action)(object)new Action(SetNickNameWindow));
			}
			else
			{
				GoOffline();
			}
		}
	}

	public void OnWindowShow(Action a)
	{
		//IL_0008: Unknown result type (might be due to invalid IL or missing references)
		//IL_0012: Expected O, but got Unknown
		//IL_0031: Unknown result type (might be due to invalid IL or missing references)
		//IL_003b: Expected O, but got Unknown
		if ((MulticastDelegate)(object)a == (MulticastDelegate)new Action(MenuWindow))
		{
			SetOffline(offline: true);
		}
		if (showBanner)
		{
			if ((MulticastDelegate)(object)a == (MulticastDelegate)new Action(MenuWindow))
			{
				SamsungAd.Instance().LoadBannerAd(SamsungAd.BannerAdType.Universal_Banner_320x50, SamsungAd.AdLayout.Top_Right, hide: false);
			}
			else
			{
				SamsungAd.Instance().DestroyBannerAd();
			}
		}
	}

	public void MenuWindow()
	{
		//IL_00fc: Unknown result type (might be due to invalid IL or missing references)
		//IL_0110: Expected O, but got Unknown
		//IL_0135: Unknown result type (might be due to invalid IL or missing references)
		//IL_0140: Expected O, but got Unknown
		//IL_01a1: Unknown result type (might be due to invalid IL or missing references)
		//IL_01b5: Expected O, but got Unknown
		//IL_01da: Unknown result type (might be due to invalid IL or missing references)
		//IL_01e5: Expected O, but got Unknown
		//IL_023b: Unknown result type (might be due to invalid IL or missing references)
		//IL_0247: Unknown result type (might be due to invalid IL or missing references)
		//IL_0251: Expected O, but got Unknown
		//IL_0251: Expected O, but got Unknown
		//IL_02ef: Unknown result type (might be due to invalid IL or missing references)
		//IL_0303: Expected O, but got Unknown
		//IL_038b: Unknown result type (might be due to invalid IL or missing references)
		//IL_039f: Expected O, but got Unknown
		ResetSettings2();
		if (!PhotonNetwork.isMessageQueueRunning)
		{
			PhotonNetwork.isMessageQueueRunning = true;
		}
		if (bs._LoaderScene.loadingUrl)
		{
			bs.win.Setup(400, 200, string.Empty, Dock.Center, null, null, 1f);
			LabelCenter("Loading");
			return;
		}
		bs.win.Setup(400, 550, string.Empty, Dock.Left, null, null, 1f);
		bs.win.style = base.skin.label;
		GuiClasses.replacementStyle = res.menuButton;
		if (GUILayout.Button(GUIContent(GuiClasses.Tr(" Welcome ") + GuiClasses.Tr(playerNamePrefixed, sk: false, save: false), Avatar), res.labelGlow, GUILayout.MaxHeight(64f)))
		{
			ShowWindow((Action)(object)new Action(bs._Loader.SelectAvatar), bs.win.act);
		}
		if (Button("Start Online Race"))
		{
			sGameType = SGameType.VsPlayers;
			ShowWindow((Action)(object)new Action(SelectMapWindow));
		}
		if (Button("Multiplayer (beta)") || bs.setting.autoConnect || bs.setting.autoHost)
		{
			if (mh)
			{
				Popup("error try to refresh page");
			}
			else
			{
				SetOffline(offline: false);
				ShowWindow((Action)(object)new Action(MultiplayerWindow), bs.win.act);
			}
		}
		if (Button("2-player SplitScreen"))
		{
			sGameType = SGameType.SplitScreen;
			ShowWindow((Action)(object)new Action(SelectMapWindow));
		}
		if (!resLoaded)
		{
			LoadingLabelAssetBundle();
		}
		else if (Button("Select Car"))
		{
			LoadLevel("!2carselect");
		}
		if (Button("Settings"))
		{
			ShowWindow((Action)(object)new Action(SettingsWindow), (Action)(object)new Action(MenuWindow));
		}
		if (Button("Level Editor"))
		{
			Application.LoadLevel("level");
		}
		if (!bs.android)
		{
			bs._Integration.DrawButtons();
		}
		if ((bs.ios && Button("Exit")) || (Input.GetKeyDown(KeyCode.Escape) && !Application.isWebPlayer))
		{
			ApplicationQuit();
		}
		if (Button("Achievements"))
		{
			_Awards.RefreshAwards();
			ShowWindow((Action)(object)new Action(_Awards.DrawAwardsWindow), bs.win.act);
		}
		if (bs._Integration.site == Site.Kg && Button("Invite Friends"))
		{
			bs.ExternalEval("kongregateAPI.getAPI().services.showInvitationBox({  content: 'Come try out this awesome game!'});");
		}
		if (bs._Integration.site == Site.VK && Button("Invite Friends"))
		{
			bs.ExternalEval("VK.callMethod('showInviteBox')");
		}
		if (bs.isDebug && Button("Edit"))
		{
			ShowWindow((Action)(object)new Action(EditValuesWindow), bs.win.act);
		}
		if (bs.isDebug)
		{
			Label(Trs("Version: ") + bs.setting.version);
		}
		GuiClasses.replacementStyle = null;
	}

	private void EditValuesWindow()
	{
		Setup(600, 1000, string.Empty);
		if (Button("Save"))
		{
			StartCoroutine(SavePlayerPrefs());
		}
		searchMap = GUILayout.TextField(searchMap);
		BeginScrollView();
		if (keyValuePairs == null)
		{
			keyValuePairs = Base.PlayerPrefString.ToArray();
		}
		foreach (KeyValuePair<string, string> item in Base.PlayerPrefString)
		{
			if (searchMap.Length > 3 && item.Key.Contains(searchMap))
			{
				GUILayout.BeginHorizontal();
				GUILayout.Label(item.Key, GUILayout.ExpandWidth(expand: false));
				string text = GUILayout.TextField(item.Value);
				if (text != item.Value)
				{
					Base.PlayerPrefString[item.Key] = text;
				}
				GUILayout.EndHorizontal();
			}
		}
		GUILayout.EndScrollView();
	}

	private void ResetSettings2()
	{
		night = (bombCar = (rain = (enableCollision = false)));
		mapSets.levelFlags = LevelFlags.race;
	}

	public void SetOffline(bool offline)
	{
		if (PhotonNetwork.connected && offline && !offlineMode)
		{
			MonoBehaviour.print("Disconnecting");
			PhotonNetwork.Disconnect();
		}
		if (offlineMode != offline)
		{
			offlineMode = offline;
			MonoBehaviour.print("Set Offline " + offline);
		}
		if (!offline)
		{
			PhotonNetwork.ConnectUsingSettings("tm" + 840);
			MonoBehaviour.print("Connecting");
		}
	}

	private void SearchClanWindow()
	{
		//IL_0061: Unknown result type (might be due to invalid IL or missing references)
		//IL_0078: Expected O, but got Unknown
		Label("Group name:");
		clanName = GUILayout.TextField(Filter(clanName), 10);
		if (!Button("Search"))
		{
			return;
		}
		if (clanName.Length < 1)
		{
			Popup2("Enter Clan Name", (Action)(object)new Action(SearchClanWindow));
			return;
		}
		Popup2("Searching..");
		bs.Download(bs.mainSite + "clans/" + clanName + ".txt", delegate(string s, bool b)
		{
			//IL_0040: Unknown result type (might be due to invalid IL or missing references)
			//IL_004b: Expected O, but got Unknown
			clanMembers = ((!b) ? new List<string>() : new List<string>(bs.SplitString(s)));
			inclan = contains();
			_ChatPhp = new ChatPhp();
			ShowWindow((Action)(object)new Action(ClanWindow));
		}, false);
	}

	private bool contains()
	{
		foreach (string clanMember in clanMembers)
		{
			if (clanMember.ToLower() == playerNamePrefixed.ToLower())
			{
				return true;
			}
		}
		return false;
	}

	private void ClanWindow()
	{
		//IL_00c9: Unknown result type (might be due to invalid IL or missing references)
		//IL_00d4: Expected O, but got Unknown
		//IL_01b1: Unknown result type (might be due to invalid IL or missing references)
		//IL_01bc: Expected O, but got Unknown
		Setup(500, 400, string.Empty);
		Texture2D texture = (Texture2D)bs.LoadRes("icons/clan" + clanName.ToLower());
		GUILayout.Label(GUIContent(string.Format(Trs("Clan {0}, {1} members found"), clanName, clanMembers.Count), texture));
		if (clanMembers.Count > 0)
		{
			Label(Trs("Clan creator: ") + clanMembers[0]);
		}
		GUILayout.BeginHorizontal();
		if (BackButton())
		{
			ShowWindow((Action)(object)new Action(MenuWindow));
		}
		if (Button((clanMembers.Count == 0) ? "Create Clan" : ((!inclan) ? "Join" : "Play")))
		{
			clanTag = clanName.Substring(0, Mathf.Min(clanName.Length, 3));
			if (!inclan)
			{
				bs.Download(bs.mainSite + "scripts/clan.php", delegate
				{
				}, true, "clan", clanName, "playerName", playerNamePrefixed);
			}
			sGameType = SGameType.Clan;
			ShowWindow((Action)(object)new Action(SelectMapWindow));
		}
		GUILayout.EndHorizontal();
		GUILayout.Space(10f);
		string text = string.Empty;
		if (clanMembers.Count > 1)
		{
			int num = clanMembers.Count - 1;
			int num2 = 0;
			while (num >= 0 && num2 < 20)
			{
				text = text + clanMembers[num] + ", ";
				num--;
				num2++;
			}
		}
		base.skin.label.wordWrap = true;
		_ChatPhp.room = clanName;
		_ChatPhp.def = GuiClasses.Tr("Clan chat ") + clanName + "\n" + text;
		_ChatPhp.DrawChat();
	}

	public void MapSelectWindow()
	{
	}

	public void SelectMapWindow()
	{
		bs.win.Setup(900, 600, GuiClasses.Tr(sGameType.ToString()), Dock.Center, null, null, 1f);
		LabelCenter("Choose map");
		tabSelected = bs.Mod(tabSelected, titles.Length);
		bool flag = tabSelected == Tag.userTab;
		DrawTab();
		if (flag && !resLoaded)
		{
			LoadingLabelAssetBundle();
			GUILayout.FlexibleSpace();
			return;
		}
		BeginScrollView(bs.win.skin.box, !bs.android);
		int num = 1;
		if (flag)
		{
			GUI.enabled = (wwwUserMaps != null && wwwUserMaps.isDone);
			GUILayout.BeginHorizontal();
			GUILayout.FlexibleSpace();
			TopMaps topMaps = (TopMaps)Toolbar((int)this.topMaps, new string[3]
			{
				"Staff Pick",
				"Top",
				"New"
			}, expand: false);
			if (topMaps != this.topMaps)
			{
				this.topMaps = topMaps;
				DownloadUserMaps2(0);
			}
			GUILayout.FlexibleSpace();
			GUILayout.EndHorizontal();
			GUILayout.BeginHorizontal();
			for (int i = 0; i < 13; i++)
			{
				if (GlowButton(i + 1 + string.Empty, i == page))
				{
					DownloadUserMaps2(i);
				}
			}
			GUILayout.EndHorizontal();
			GUI.enabled = true;
		}
		GUILayout.BeginHorizontal();
		GUILayout.FlexibleSpace();
		int j = 0;
		int num2 = 0;
		for (; j < scenes.Count; j++)
		{
			Scene scene = scenes[j];
			num++;
			if (scene.j == tabSelected)
			{
				bool flag2 = !scene.loaded && !scene.userMap;
				bool flag3 = (j + DateTime.Now.Day) % 10 == 0;
				bool flag4 = (int)medals >= num || flag3 || levelsCheat || scene.userMap || j == 0;
				if (Time.time - levelLoadTime < 1.5f && (int)medals >= num && (int)medals - wonMedals < num)
				{
					flag4 = (Time.time % 0.4f < 0.2f);
				}
				GUI.enabled = (!flag2 && flag4);
				GUILayout.Space(15f);
				GUILayout.BeginVertical((!flag) ? GUIStyle.none : base.skin.box);
				prefixMapPl = scene.name + ";" + bs._Loader.playerName + ";";
				GUILayout.Label(GUIContent((record != float.MaxValue) ? bs.TimeToStr(record) : null, (place != 4) ? bs.win.medals[place] : null), GUILayout.Height(20f), GUILayout.Width(100f));
				base.skin.button.wordWrap = true;
				bool flag5 = scene.userMap ? GuiClasses.SoundButton(GUILayout.Button(GuiClasses.Tr("Map by ") + scene.mapBy, GUILayout.Height(100f), GUILayout.Width(100f))) : (GuiClasses.JoystickButton2(res.mapSelectButton) || GuiClasses.SoundButton(GUILayout.Button(GUIContent((!flag4) ? bs.win.locked : scene.texture, string.Format(GuiClasses.Tr("You need {0} medals to unlock"), Mathf.Max(0, num))), res.mapSelectButton, GUILayout.Height(100f), GUILayout.Width(100f))) || (bs.setting.autoHost && j == 0));
				if (flag && scene.rating != 0f)
				{
					GUILayout.Label(res.rating[Mathf.RoundToInt(scene.rating * 2f)]);
				}
				if (flag5)
				{
					OnMapSelected(scene);
				}
				if (!scene.userMap)
				{
					string s = scene.name + "\n" + GuiClasses.Tr(flag2 ? "not found" : (((int)medals >= num) ? string.Empty : ((!flag3) ? "Locked" : "map rotation")), sk: true);
					LabelCenter(s, 16, wrap: false, null, bs.win.skin.label);
				}
				else
				{
					GUILayout.Label(scene.title, GUILayout.Width(100f));
				}
				GUILayout.EndVertical();
				if (++num2 % 5 == 0 && num2 != 0)
				{
					GUILayout.FlexibleSpace();
					GUILayout.EndHorizontal();
					GUILayout.BeginHorizontal();
					GUILayout.FlexibleSpace();
				}
			}
		}
		GUI.enabled = true;
		GUILayout.FlexibleSpace();
		GUILayout.EndHorizontal();
		GUILayout.FlexibleSpace();
		GUILayout.EndScrollView();
		DrawMedals();
	}

	private void OnMapSelected(Scene scene)
	{
		//IL_0065: Unknown result type (might be due to invalid IL or missing references)
		//IL_0079: Expected O, but got Unknown
		//IL_00a2: Unknown result type (might be due to invalid IL or missing references)
		//IL_00ae: Unknown result type (might be due to invalid IL or missing references)
		//IL_00b9: Expected O, but got Unknown
		//IL_00b9: Expected O, but got Unknown
		mapName = scene.name;
		levelName = scene.name;
		gameType = (scene.mapSets.enableCtf ? GameType.ctf : (scene.mapSets.enableDm ? GameType.dm : GameType.race));
		ResetServerSettings();
		if (sGameType == SGameType.Multiplayer)
		{
			ShowWindow((Action)(object)new Action(HostGameWindow), bs.win.act);
		}
		else if (sGameType == SGameType.VsPlayers || sGameType == SGameType.SplitScreen)
		{
			bs.win.ShowWindow((Action)(object)new Action(PlayVsPlayersWindow), (Action)(object)new Action(SelectMapWindow));
		}
	}

	private void ResetServerSettings()
	{
		if (!bs.android)
		{
			rain = (UnityEngine.Random.value < 0.1f && !bs.isDebug);
			topdown = false;
		}
		this.weaponEnum = (WeaponEnum)0;
		foreach (int value in Enum.GetValues(typeof(WeaponEnum)))
		{
			weaponEnum |= (WeaponEnum)value;
		}
	}

	internal void LoadingLabelAssetBundle()
	{
		if (!resLoaded)
		{
			LabelCenter("Game Loading " + (int)((wwwAssetBundle == null) ? 0f : (wwwAssetBundle.progress * 100f)) + "%");
		}
	}

	public void DownloadUserMaps2(int page)
	{
		StartCoroutine(DownloadUserMaps((topMaps != 0) ? 1 : 2, (int)mapSets.levelFlags, string.Empty, page, topMaps == TopMaps.Top));
	}

	private void DrawTab()
	{
		GUILayout.BeginHorizontal();
		base.skin.button.wordWrap = false;
		if (GuiClasses.JoystickButton2(base.skin.button) || GuiClasses.SoundButton(GUILayout.Button("<", GUILayout.ExpandHeight(expand: true), GUILayout.Width(100f))))
		{
			tabSelected--;
		}
		GUILayout.FlexibleSpace();
		tabSelected = Toolbar(tabSelected, titles, expand: false);
		GUILayout.FlexibleSpace();
		if (GuiClasses.JoystickButton2(base.skin.button) || GuiClasses.SoundButton(GUILayout.Button(">", GUILayout.ExpandHeight(expand: true), GUILayout.Width(100f))))
		{
			tabSelected++;
		}
		GUILayout.EndHorizontal();
	}

	private void SearchMapWindow()
	{
		//IL_0066: Unknown result type (might be due to invalid IL or missing references)
		//IL_0071: Expected O, but got Unknown
		searchMap = GUILayout.TextField(searchMap);
		if (ButtonLeft("Search", null, 0f))
		{
			StartCoroutine(DownloadUserMaps(0, (int)mapSets.levelFlags, searchMap));
			tabSelected = Tag.userTab;
			ShowWindow((Action)(object)new Action(SelectMapWindow));
		}
		StringBuilder stringBuilder = new StringBuilder();
		if (favorites.Count <= 0)
		{
			return;
		}
		foreach (string favorite in favorites)
		{
			stringBuilder.Append(favorite + ",");
		}
		Label("Your Favorites: " + stringBuilder);
	}

	public void DrawMedals()
	{
		//IL_015b: Unknown result type (might be due to invalid IL or missing references)
		//IL_0166: Expected O, but got Unknown
		//IL_0184: Unknown result type (might be due to invalid IL or missing references)
		//IL_0190: Unknown result type (might be due to invalid IL or missing references)
		//IL_019a: Expected O, but got Unknown
		//IL_019a: Expected O, but got Unknown
		base.skin.label.fontSize = 14;
		GUILayout.BeginHorizontal();
		base.skin.button.fixedHeight = 40f;
		if ((int)warScore > 0 && Button(warScore.ToString(), expandWidth: false, 20, bold: false, bs._Loader.guiSkins.warScore))
		{
			Popup("warScoreText", bs._Loader.guiSkins.warScore);
		}
		if (Button(medals.ToString(), expandWidth: false, 20, bold: false, bs.win.medalsCnt))
		{
			Popup("medaltext", bs.win.medalsCnt);
		}
		if (!bs._Loader.disableRep && Button(reputation.ToString(), expandWidth: false, 20, bold: false, bs.win.reputation))
		{
			Popup(Trs("reptext") + $", У вас {friendCount} друзей вк", bs.win.reputation);
		}
		base.skin.button.fixedHeight = 0f;
		if (BackButtonLeft())
		{
			ShowWindow((Action)(object)new Action(MenuWindow));
		}
		if (Button("Search Map"))
		{
			ShowWindow((Action)(object)new Action(SearchMapWindow), (Action)(object)new Action(SelectMapWindow));
		}
		GUILayout.EndHorizontal();
	}

	private void SendLinkWindow()
	{
		Setup(600, 350, string.Empty);
		GUILayout.Label(res.sendReplayIcon);
		LabelCenter("Send this link to friend so he can play vs you", 16, wrap: true);
		GUILayout.TextArea(bs._Loader.replayLinkPrefix + bs._Loader.replayLink);
	}

	protected void PlayVsPlayersWindow()
	{
		//IL_006c: Unknown result type (might be due to invalid IL or missing references)
		//IL_0078: Unknown result type (might be due to invalid IL or missing references)
		//IL_0082: Expected O, but got Unknown
		//IL_0082: Expected O, but got Unknown
		Setup(400, 500, string.Empty);
		prefixMapPl = curScene.name + ";" + bs._Loader.playerName + ";";
		if (bs.isMod && Button("Rate Map"))
		{
			ShowWindow((Action)(object)new Action(bs._Loader.RateMapWindow), (Action)(object)new Action(PlayVsPlayersWindow));
		}
		LabelCenter(Trs("How Many Opponents?"));
		GuiClasses.replacementStyle = res.menuButton;
		int[] array = new int[4]
		{
			0,
			1,
			3,
			6
		};
		foreach (int num in array)
		{
			if ((num != 1 || bs.android) && Button(num + Trs(" racers")))
			{
				PlayersCount = num;
				StartLoadReplays();
			}
		}
		GUILayout.BeginHorizontal();
		if (resLoaded)
		{
			bs._Loader.enableZombies = Toggle(enableZombies, "Zombies");
		}
		bs._Loader.rain = Toggle(rain, "Rain");
		bs._Loader.night = Toggle(night, "Night");
		GUILayout.EndHorizontal();
		GUILayout.BeginHorizontal();
		bs._Loader.enableCollision = Toggle(enableCollision, "Collision");
		bs._Loader.bombCar = Toggle(bombCar, "BombCar");
		GUILayout.EndHorizontal();
		GuiClasses.replacementStyle = null;
		if (bs._Loader.enableZombies)
		{
			difficulty = Difficulty.Normal;
			return;
		}
		Label("Difficulty");
		bs._Loader.difficulty = (Difficulty)Toolbar((int)bs._Loader.difficulty, Enum.GetNames(typeof(Difficulty)), expand: true);
	}

	public void ScoreBoardWindow()
	{
		Setup(800, 500, string.Empty);
		bs.win.showBackButton = false;
		bs.win.addflexibleSpace = false;
		if (scoreBoard == null)
		{
			scoreBoard = GuiClasses.Tr("Loading..");
			bs.Download(bs.mainSite + "scripts/getScoreboard3.php", ParseScoreboard, false, "map", curScene.mapId, "dm", bs._Loader.dmOrCoins ? 1 : 0, "flags", (int)replayFlags);
			_ChatPhp = new ChatPhp();
		}
		LabelCenter(Trs("Map ") + bs._Loader.mapName);
		GUILayout.BeginHorizontal();
		GuiClasses.scroll = GUILayout.BeginScrollView(GuiClasses.scroll, GUILayout.Width(300f));
		LabelCenter("Scoreboard");
		base.skin.label.alignment = TextAnchor.UpperLeft;
		GUILayout.Label(scoreBoard);
		GUILayout.EndScrollView();
		_ChatPhp.room = bs._Loader.mapName;
		GUILayout.BeginVertical();
		LabelCenter("Chat");
		_ChatPhp.DrawChat();
		GUILayout.EndVertical();
		GUILayout.EndHorizontal();
		GUILayout.BeginHorizontal();
		if (bs._Game != null)
		{
			bs._Game.RateButton(bs.win.act);
		}
		if (bs.win.BackButton())
		{
			bs.win.Back();
		}
		GUILayout.EndHorizontal();
	}

	private void ParseScoreboard(string s, bool b)
	{
		scoreBoard = s;
		if (!b)
		{
			return;
		}
		string text = GuiClasses.Tr("Place") + GuiClasses.Tr(" Name") + "            " + GuiClasses.Tr((!bs._Loader.dmOrCoins) ? "Time" : "Score") + "      " + GuiClasses.Tr("Rewinds");
		scoreBoard = text + "\n";
		string format = GuiClasses.CreateTable(text);
		int num = -1;
		string[] array = bs.SplitString(s);
		bool flag = false;
		for (int i = 0; i < array.Length; i++)
		{
			string[] array2 = array[i].Split(';');
			string text2 = (!(array2[2] == "-1")) ? array2[2] : string.Empty;
			float num2 = float.Parse(array2[1]);
			if (array2[0].ToLower() == bs._Loader.playerNamePrefixed.ToLower())
			{
				continue;
			}
			if (bs._Loader.dmOrCoins)
			{
				if ((float)bs._Player.score > num2 && !flag)
				{
					flag = true;
					scoreBoard = scoreBoard + string.Format(format, i + 1, bs._Loader.playerName, bs._Player.score, text2) + "\n";
					num = i + 1;
					i++;
				}
			}
			else if (bs._Loader.record < num2 && !flag)
			{
				flag = true;
				scoreBoard = scoreBoard + string.Format(format, i + 1, bs._Loader.playerName, bs.TimeToStr(bs._Loader.record, draw: false, bs._Loader.dmOrCoins), text2, bs._Loader.dmOrCoins) + "\n";
				num = i + 1;
				i++;
			}
			scoreBoard = scoreBoard + string.Format(format, i + 1, array2[0], bs.TimeToStr(num2, draw: false, bs._Loader.dmOrCoins), text2) + "\n";
		}
		if (num != -1)
		{
			scoreBoard = "Your Place is " + num + "\n" + scoreBoard;
		}
	}

	private static bool Any(List<Replay> list, string nick)
	{
		foreach (Replay item in list)
		{
			if (item.playerName == nick)
			{
				return true;
			}
		}
		return false;
	}

	public void LoadReplayWindow()
	{
		Setup(600, 300, "Loading Replays");
		BeginScrollView(default(Vector2));
		GUILayout.BeginHorizontal();
		GUILayout.BeginVertical();
		GUILayout.Label(Avatar);
		Label(Trn(playerName));
		GUILayout.EndVertical();
		for (int num = tempReplays.Count - 1; num >= 0; num--)
		{
			Replay replay = tempReplays[num];
			GUILayout.BeginVertical();
			GUILayout.Label(res.GetAvatar(replay.avatarId, replay.avatarUrl));
			Label(Trn(replay.playerName));
			GUILayout.EndVertical();
		}
		GUILayout.FlexibleSpace();
		GUILayout.EndHorizontal();
		GUILayout.EndScrollView();
		if ((base.easy || bs.isDebug) && Button("Skip and play alone"))
		{
			StopAllCoroutines();
			replays = new List<Replay>();
			StartCoroutine(StartLoadLevel(mapName));
		}
	}

	public void EveryPlayRecordVideoWindow()
	{
		Setup(450, 300, string.Empty);
		Label("Record Next Gameplay Video?");
		GUILayout.BeginHorizontal();
		if (Button("No"))
		{
			disableEveryPlay = true;
			WindowPool();
		}
		if (Button("Yes") && !base.everyPlay.IsInvoking())
		{
			base.everyPlay.StartRecording();
			WindowPool();
		}
		GUILayout.EndHorizontal();
	}

	public void SettingsWindow()
	{
		//IL_0152: Unknown result type (might be due to invalid IL or missing references)
		//IL_015d: Expected O, but got Unknown
		//IL_023c: Unknown result type (might be due to invalid IL or missing references)
		//IL_0248: Unknown result type (might be due to invalid IL or missing references)
		//IL_0252: Expected O, but got Unknown
		//IL_0252: Expected O, but got Unknown
		//IL_0274: Unknown result type (might be due to invalid IL or missing references)
		//IL_0289: Expected O, but got Unknown
		//IL_0687: Unknown result type (might be due to invalid IL or missing references)
		//IL_069b: Expected O, but got Unknown
		bs.win.Setup(500, 500, "Settings", Dock.Center, null, null, 1f);
		BeginScrollView();
		EveryPlayRecorder();
		GUILayout.Label(GuiClasses.Tr("version:") + LoadingScreen.version + " pkg:" + bs.setting.packageVersion);
		if (!bs.setting.vk2)
		{
			advancedOptions = Toggle(advancedOptions, "Advanced settings");
		}
		if (Button("FullScreen"))
		{
			FullScreen(fullscreen: true);
		}
		bool flag = Toggle(bs._Loader.rearCamera, "Rear Camera");
		if (flag != bs._Loader.rearCamera)
		{
			bs._Loader.rearCamera = flag;
			StartCoroutine(bs._AutoQuality.SetQuality(bs._Loader.quality));
		}
		if (bs._Game != null && bs.android && Button("Edit Controls"))
		{
			bs._Game.editControls = true;
			ShowWindow((Action)(object)new Action(bs._Game.MenuWindow));
		}
		showYourGhost = Toggle(showYourGhost, "Show your ghost");
		if (bs.android && advancedOptions)
		{
			bs._Loader.scaleButtons = Toggle(bs._Loader.scaleButtons, "Scale buttons");
		}
		if (!bs.android)
		{
			controls = ((!Toggle(enableMouse, "Enable Mouse")) ? Contr.keys : Contr.mouse);
			if (!bs.flash)
			{
				autoFullScreen = Toggle(autoFullScreen, "Auto Full Screen");
				if (advancedOptions)
				{
					Application.runInBackground = Toggle(Application.runInBackground, "Run In Background");
				}
			}
			if (Button("Keyboard", expandWidth: false))
			{
				ShowWindow((Action)(object)new Action(KeyboardSetup), (Action)(object)new Action(SettingsWindow));
			}
		}
		if (Button("Language", expandWidth: false))
		{
			bs.win.ShowWindow((Action)(object)new Action(SelectLangWindow), bs.win.act);
		}
		if (!bs._Game)
		{
			Label("Difficulty");
			bs._Loader.difficulty = (Difficulty)Toolbar((int)bs._Loader.difficulty, Enum.GetNames(typeof(Difficulty)), expand: true);
		}
		Label(Trs("Mouse Sensivity: ") + sensivity);
		sensivity = GUILayout.HorizontalSlider(sensivity, 0.5f, 2f);
		if (bs.android)
		{
			Label("Controls");
			controls = Toolbar(controls, Contr.names, expand: true);
		}
		Label("sound volume", 13);
		soundVolume2 = GUILayout.HorizontalSlider(soundVolume2, 0f, 1f);
		Label("music volume", 13);
		musicVolume = GUILayout.HorizontalSlider(musicVolume, 0f, 1f);
		Label("Voice Chat Volume");
		bs._Loader.voiceChatVolume = GUILayout.HorizontalSlider(bs._Loader.voiceChatVolume, 0f, 1f);
		bs._AutoQuality.DrawSetQuality();
		bs._AutoQuality.DrawDistance();
		if (advancedOptions && bs.android)
		{
			Label("Cull mode");
			int num = GUILayout.Toolbar(bs._Loader.CullMode, new string[3]
			{
				"1",
				"2",
				"3"
			});
			if (bs._Loader.CullMode != num)
			{
				bs._Loader.CullMode = num;
				bs._AutoQuality.UpdateCull();
			}
		}
		if (bs._Game != null && advancedOptions && !bs.online)
		{
			Label("Time Scale: " + bs._Game.customTime);
			bs._Game.customTime = GUILayout.HorizontalSlider(bs._Game.customTime, 0.5f, 1f);
		}
		if ((bs.android || bs.androidPlatform) && advancedOptions)
		{
			if (bs._Game == null)
			{
				reverseSplitScreen = Toggle(reverseSplitScreen, "Reverse SplitScreen");
			}
			bs.setting.m_android = Toggle(bs.android, "Mobile/Touch Screen");
		}
		if (advancedOptions)
		{
			GUILayout.BeginHorizontal();
			if (Button("Smaller Resolution"))
			{
				Screen.SetResolution(Screen.currentResolution.width / 2, Screen.currentResolution.height / 2, fullscreen: true);
			}
			if (Button("Bigger Resolution"))
			{
				Screen.SetResolution(Screen.currentResolution.width * 2, Screen.currentResolution.height * 2, fullscreen: true);
			}
			GUILayout.EndHorizontal();
			bs._Loader.enableChat = GUILayout.Toggle(bs._Loader.enableChat, "Enable Chat");
			GUILayout.BeginHorizontal();
			Label("Clan Tag");
			bs._Loader.clanTag = GUILayout.TextField(bs._Loader.clanTag, 3);
			GUILayout.EndHorizontal();
			if (Button("Console"))
			{
				console = log.Append("\nerrors count:" + errors).ToString();
				ShowWindow((Action)(object)new Action(ConsoleWindow), bs.win.act);
			}
			bs.setting.debug = (GUILayout.Toggle(bs.setting.debug, "Debug") && (Input.GetKey(KeyCode.LeftShift) || Application.isEditor || bs.setting.debug));
		}
		if (bs.isDebug)
		{
			bs.setting.printPlayerPrefs = Toggle(bs.setting.printPlayerPrefs, "Print player prefs");
			bs.setting.enableLog = Toggle(bs.setting.enableLog, "Enable Log");
			bs.setting.noLevelCache = Toggle(bs.setting.noLevelCache, "noLevelCache");
		}
		GUILayout.EndScrollView();
	}

	private void EveryPlayRecorder()
	{
		if (!base.everyPlay.IsRecordingSupported() && !bs.isDebug)
		{
			return;
		}
		GUILayout.BeginVertical("Recrod Video", bs.win.editorSkin.window);
		GUILayout.BeginHorizontal();
		if (base.everyPlay.IsRecording())
		{
			Label("Video is recording...");
			if (Button("Stop Record Video"))
			{
				base.everyPlay.StopRecording();
				SetMetadata();
				base.everyPlay.ShowSharingModal();
			}
		}
		else if (Button("Start Record Video"))
		{
			base.everyPlay.FaceCamSetAudioOnly(audioOnly: true);
			SetMetadata();
			base.everyPlay.StartRecording();
		}
		GUILayout.EndHorizontal();
		if (everyplayRecorded && Button("View recorded video"))
		{
			Everyplay.SharedInstance.PlayLastRecording();
		}
		if (Button("View video gallery"))
		{
			Application.OpenURL("https://everyplay.com/TimeMachine-Race");
		}
		GUILayout.EndVertical();
	}

	private void SetMetadata()
	{
		Dictionary<string, object> dictionary = new Dictionary<string, object>();
		dictionary.Add("map", "test");
		dictionary.Add("test", "test");
		base.everyPlay.SetMetadata(dictionary);
	}

	public void ConsoleWindow()
	{
		Setup(1000, 1000, string.Empty);
		Label("Press ctrl+c to copy");
		consoleScroll = GUILayout.BeginScrollView(consoleScroll);
		GUILayout.TextField(console);
		TextEditor textEditor = (TextEditor)GUIUtility.GetStateObject(typeof(TextEditor), GUIUtility.keyboardControl);
		textEditor.pos = 0;
		textEditor.selectPos = console.Length;
		GUILayout.EndScrollView();
	}

	public void SelectAvatar()
	{
		//IL_009d: Unknown result type (might be due to invalid IL or missing references)
		//IL_00a8: Expected O, but got Unknown
		bs.win.Setup(600, 300, "Select Character", Dock.Center, null, null, 1f);
		bs.win.showBackButton = false;
		BeginScrollView(null, showHorizontal: true);
		GUILayout.FlexibleSpace();
		GUILayout.BeginHorizontal();
		for (int i = 0; i < res.avatars.Length; i++)
		{
			if (ButtonTexture(null, res.GetAvatar(i, avatarUrl)) || Input.GetKeyDown(KeyCode.Escape))
			{
				avatar = i;
				if (bs._CarSelectMenu != null)
				{
					ShowWindow((Action)(object)new Action(bs._CarSelectMenu.Window));
				}
				else
				{
					WindowPool();
				}
				break;
			}
		}
		GUILayout.EndHorizontal();
		GUILayout.FlexibleSpace();
		GUILayout.EndScrollView();
	}

	protected bool CheckLength()
	{
		//IL_0023: Unknown result type (might be due to invalid IL or missing references)
		//IL_003a: Expected O, but got Unknown
		//IL_006d: Unknown result type (might be due to invalid IL or missing references)
		//IL_0084: Expected O, but got Unknown
		if (playerName.Length < minLength)
		{
			Popup2("Enter Your Name", (Action)(object)new Action(LoginWindow));
		}
		else
		{
			if (password.Length >= minLength || vk)
			{
				return true;
			}
			Popup2("password is too short", (Action)(object)new Action(LoginWindow));
		}
		return false;
	}

	protected IEnumerator LoadPlayerPrefs(string text)
	{
		WWW w = null;
		string s = bs.mainSite + "/players/" + playerName + "/prefs.txt";
		if (bs.isDebug)
		{
			s = s + "?" + UnityEngine.Random.value;
		}
		Debug.Log(s);
		w = new WWW(s);
		while (!w.isDone)
		{
			popupText = GuiClasses.Tr("Logging in ") + (int)w.progress * 100 + "%";
			yield return null;
		}
		try
		{
			StringBuilder sb = new StringBuilder();
			Dictionary<string, string> dict = ParseDict(text);
			modTypeInt = int.Parse(dict.TryGet("modType", "0"));
			sb.AppendLine("parsed Rep: " + reputation);
			sb.AppendLine("parsed medals: " + medals);
			sb.AppendLine("parsed modType: " + modTypeInt);
			userId = int.Parse(dict.TryGet("id", "0"));
			bool crypt = false;
			bool ovrd2 = false;
			if (string.IsNullOrEmpty(w.error))
			{
				byte[] buffer = w.bytes;
				try
				{
					Xor(buffer);
					buffer = GZipStream.UncompressBuffer(buffer);
				}
				catch (Exception)
				{
					Debug.LogWarning("Failed uncompress");
					Xor(buffer);
				}
				if (buffer.Length > 0)
				{
					using (BinaryReader ms = new BinaryReader(buffer))
					{
						sb.AppendLine("loading stats ");
						int i = 0;
						int local = Base.playerPrefKeys.Count;
						while (ms.Position < ms.Length)
						{
							string key = ms.ReadString();
							string value = ms.ReadString();
							if (value.Length > 500 || key.Length > 500)
							{
								Debug.LogError($"too big value {key} {value}");
								continue;
							}
							if (crypt)
							{
								key = ObscuredString.EncryptDecrypt(key);
								value = ObscuredString.EncryptDecrypt(value);
							}
							if (key == "Enc" && value == "Enc")
							{
								Debug.LogWarning("Encoded");
								crypt = true;
							}
							else if (key == "savePrefsTime" && loggedInTime != 0)
							{
								ovrd2 = (loggedInTime < int.Parse(value));
								if (ovrd2)
								{
									lastError = "Override Detected";
								}
								bs.LogEvent("Override Detected");
								Debug.LogWarning("Set override to: " + ovrd2);
								ovrd2 = false;
							}
							else
							{
								i++;
								if (ovrd2 || !PlayerPrefs.HasKey(key))
								{
									Base.PlayerPrefsSetString(key, value);
								}
								else
								{
									Base.playerPrefKeys.Add(key);
								}
								sb.Append($"{key}:{value},");
							}
						}
						Debug.LogWarning("loading player prefs local:" + local + " remote:" + i + " \n" + ((!Debug.isDebugBuild) ? sb.Length.ToString() : sb.ToString()));
						Base.SetStrings.Clear();
						statsSaved = true;
					}
				}
				else
				{
					Debug.LogWarning("player prefs empty");
				}
			}
			else if (!w.error.StartsWith("404"))
			{
				throw new Exception(w.url + w.error);
			}
			MonoBehaviour.print("reputation " + reputation);
			reputation = Mathf.Max(int.Parse(dict.TryGet("reputation", "0")), reputation);
			medals = Mathf.Max(int.Parse(dict.TryGet("medals", "0")), medals);
		}
		catch (Exception e)
		{
			Base.SetStrings.Clear();
			Debug.LogError(e);
			lastError = e.Message.Replace("\r", "   ").Replace("\n", "    ");
			OnLoginFailed(e.Message + GuiClasses.Tr(" Critical Error"));
			yield break;
		}
		RefreshPrefs();
		OnLoggedIn();
		WindowPool();
	}

	public IEnumerator SavePlayerPrefs(bool skip = false)
	{
		Action act = bs.win.act;
		bs.SaveStrings();
		if (!skip && (bs.guest || bs.totalSeconds - uploadPrefsTime < 86400))
		{
			yield break;
		}
		uploadPrefsTime = bs.totalSeconds;
		bs.win.ShowWindow((Action)(object)(Action)delegate
		{
			bs.win.showBackButton = false;
			Label("Saving stats ");
			if (BackButtonLeft())
			{
				ShowWindow(act);
				act = null;
			}
		}, act, skip: true);
		yield return null;
		StringBuilder sb = new StringBuilder();
		byte[] array;
		using (BinaryWriter ms = new BinaryWriter())
		{
			ms.Write("savePrefsTime");
			ms.Write(bs.totalSeconds.ToString());
			List<string> forb = new List<string>
			{
				"password",
				"Enc",
				"savePrefsTime"
			};
			foreach (string Key in Base.playerPrefKeys)
			{
				if (!forb.Contains(Key))
				{
					string value = Base.PlayerPrefsGetString(Key, string.Empty);
					if (value.Length < 200)
					{
						sb.AppendLine(Key + "\t\t" + value);
					}
					if (value.Length > 500 || Key.Length > 500)
					{
						Debug.LogError($"too big value {Key} {value}");
						continue;
					}
					ms.Write(Key);
					ms.Write(value);
				}
			}
			array = GZipStream.CompressBuffer(ms.ToArray());
			Xor(array);
			MonoBehaviour.print("saving stats_______ :" + ms.Length + " " + array.Length);
		}
		WWW w = bs.DownloadAcc("savePrefs2", null, true, "file", array);
		yield return w;
		if (act != null)
		{
			bs.win.ShowWindow(act);
		}
		if (!w.text.StartsWith("prefs uploaded"))
		{
			Popup(bs._Loader.lastError = w.text + w.error, (Action)(object)new Action(MenuWindow));
		}
	}

	public void ResetPrefs()
	{
		string playerName = bs._Loader.playerName;
		string password = bs._Loader.password;
		int avatar = bs._Loader.avatar;
		Base.PlayerPrefsClear();
		if (!bs.guest)
		{
			bs._Loader.playerName = playerName;
			bs._Loader.password = password;
			bs._Loader.avatar = avatar;
		}
	}

	public void EnableWeaponWindow()
	{
		WeaponEnum[] array = Enum.GetValues(typeof(WeaponEnum)).Cast<WeaponEnum>().ToArray();
		for (int i = 0; i < array.Length; i++)
		{
			weaponEnum = (WeaponEnum)bs.SetFlag((int)weaponEnum, (int)array[i], Toggle(bs.GetFlag((int)weaponEnum, (int)array[i]), array[i].ToString()));
		}
	}

	public void HostGameWindow()
	{
		//IL_02bc: Unknown result type (might be due to invalid IL or missing references)
		//IL_02d0: Expected O, but got Unknown
		Setup(450, 500, string.Empty);
		Label("Game Name:");
		if (roomName == null)
		{
			roomName = playerNamePrefixed + "'s ";
		}
		roomName = GUILayout.TextField(roomName, 20);
		Label("Game Type:");
		gameType = (GameType)Toolbar((int)gameType, new string[5]
		{
			"Race",
			"DeathMatch",
			null,
			"Capture The Flag",
			"Zombies"
		}, expand: true);
		if (bs.setting.autoHost)
		{
			gameType = GameType.ctf;
		}
		GUILayout.BeginHorizontal();
		rain = Toggle(rain, "Rain");
		night = Toggle(night, "Night");
		havePassword = Toggle(havePassword, "Password");
		if (havePassword)
		{
			gamePassword = GUILayout.TextField(gamePassword, 20);
		}
		GUILayout.EndHorizontal();
		if (!bs._Loader.dm)
		{
			bombCar = Toggle(bombCar, "BombCar");
			Label(Trs("Rewinds:") + rewinds);
			rewinds = (int)GUILayout.HorizontalSlider(rewinds, 0f, 60f);
			Label(Trs("wait time players on start:") + waitTime);
			waitTime = (int)GUILayout.HorizontalSlider(waitTime, 4f, 10f);
		}
		else
		{
			speedLimitEnabled = Toggle(speedLimitEnabled, "Car speed");
			if (speedLimitEnabled)
			{
				Label("Speed Limit:" + speedLimit);
				speedLimit = GUILayout.HorizontalSlider(speedLimit, 50f, 1000f);
			}
			Label(Trs("Life:") + lifeDef);
			lifeDef = GUILayout.HorizontalSlider(lifeDef, 100f, 1000f);
			if (Button("Select weapons"))
			{
				ShowWindow((Action)(object)new Action(EnableWeaponWindow), bs.win.act);
			}
		}
		if (Button("Start") || bs.setting.autoHost)
		{
			Popup("hosting");
			ExitGames.Client.Photon.Hashtable hashtable = new ExitGames.Client.Photon.Hashtable();
			hashtable.Add(props.mapname.ToString(), mapName);
			hashtable.Add(props.difficulty.ToString(), Difficulty.Normal);
			hashtable.Add(props.rain.ToString(), bs._Loader.rain);
			hashtable.Add(props.night.ToString(), bs._Loader.night);
			hashtable.Add(props.topdown.ToString(), false);
			hashtable.Add(props.rewinds.ToString(), rewinds);
			hashtable.Add(props.wait.ToString(), waitTime);
			hashtable.Add(props.dm.ToString(), dm);
			hashtable.Add(props.version.ToString(), bs.setting.multiplayerVersion);
			hashtable.Add(props.life.ToString(), lifeDef);
			hashtable.Add(props.wallCollision.ToString(), true);
			hashtable.Add(props.team.ToString(), team);
			hashtable.Add(props.gametype.ToString(), (int)gameType);
			hashtable.Add(props.bombCar.ToString(), bombCar);
			hashtable.Add(props.weapons.ToString(), (int)weaponEnum);
			hashtable.Add(props.mapId.ToString(), curScene.mapId);
			if (bs._Loader.speedLimitEnabled)
			{
				hashtable.Add(props.speedLimit, speedLimit);
			}
			if (havePassword)
			{
				hashtable.Add(props.havePassword.ToString(), gamePassword);
			}
			PhotonNetwork.CreateRoom(roomName, isVisible: true, isOpen: true, (!bs._Loader.dm) ? 20 : 12, hashtable, Enum.GetNames(typeof(props)));
		}
	}

	protected void SelectServer()
	{
	}

	public void MultiplayerWindow()
	{
		//IL_008d: Unknown result type (might be due to invalid IL or missing references)
		//IL_0098: Expected O, but got Unknown
		Setup(600, 500, string.Empty);
		sGameType = SGameType.Multiplayer;
		if (PhotonNetwork.connectionStateDetailed != PeerState.JoinedLobby)
		{
			if (!bs.online)
			{
				Label("offline error");
			}
			Label(string.Empty + PhotonNetwork.connectionStateDetailed);
			return;
		}
		if (Button("Host Game", expandWidth: false) || bs.setting.autoHost)
		{
			ShowWindow((Action)(object)new Action(SelectMapWindow));
		}
		GUILayout.Label("Players Online: " + PhotonNetwork.countOfPlayers + " Games:" + PhotonNetwork.countOfRooms + " Ping:" + PhotonNetwork.GetPing());
		BeginScrollView();
		RoomInfo[] roomList = PhotonNetwork.GetRoomList();
		GUILayout.BeginVertical("server list", bs.win.editorSkin.window);
		if (PhotonNetwork.countOfRooms == 0)
		{
			Label("No Games Found..");
		}
		if (roomList.Length > 0)
		{
			foreach (RoomInfo item in roomList.OrderByDescending<RoomInfo, int>((RoomInfo a) => a.playerCount))
			{
				try
				{
					JoinButton(item);
				}
				catch (Exception)
				{
					Label(item.name);
				}
			}
		}
		GUILayout.FlexibleSpace();
		GUILayout.EndVertical();
		GUILayout.EndScrollView();
	}

	private void JoinButton(RoomInfo a)
	{
		//IL_0330: Unknown result type (might be due to invalid IL or missing references)
		//IL_033a: Expected O, but got Unknown
		//IL_0364: Unknown result type (might be due to invalid IL or missing references)
		//IL_0378: Expected O, but got Unknown
		string map = CustomProperty(a, props.mapname, string.Empty);
		Difficulty diff = CustomProperty(a, props.difficulty, Difficulty.Easy);
		bool r = CustomProperty(a, props.rain, def: false);
		bool i = CustomProperty(a, props.night, def: false);
		int rew = CustomProperty(a, props.rewinds, 3);
		int wt = CustomProperty(a, props.wait, 4);
		bool flag = CustomProperty(a, props.dm, def: false);
		int num = CustomProperty(a, props.version, 0);
		bool bc = CustomProperty(a, props.bombCar, def: false);
		GameType gt = (GameType)CustomProperty(a, props.gametype, flag ? 1 : 0);
		string pass = CustomProperty(a, props.havePassword, string.Empty);
		string text = a.name + " " + map;
		if (!string.IsNullOrEmpty(pass))
		{
			text += GuiClasses.Tr("(Password)");
		}
		else if (gt == GameType.team)
		{
			text += GuiClasses.Tr("(Team Battle)");
		}
		else if (gt == GameType.dm)
		{
			text += GuiClasses.Tr("(DeathMatch)");
		}
		else if (gt == GameType.ctf)
		{
			text += GuiClasses.Tr("(Capture Flag)");
		}
		else if (gt == GameType.zombies)
		{
			text += GuiClasses.Tr("(Stunts)");
		}
		else if (bc)
		{
			text += GuiClasses.Tr("(BombCar)");
		}
		if (num != bs.setting.multiplayerVersion)
		{
			text += ((!bs.isDebug) ? "-" : ("(v" + num + ")"));
		}
		string text2 = text;
		text = text2 + " " + a.playerCount + "/" + a.maxPlayers;
		if (!GUILayout.Button(text, (num <= bs.setting.multiplayerVersion) ? base.skin.button : base.skin.label) && (!bs.setting.autoConnect || !a.name.Contains("-host") || num != bs.setting.multiplayerVersion))
		{
			return;
		}
		if (num > bs.setting.multiplayerVersion)
		{
			Popup("you have old version, please update", bs.win.act);
			return;
		}
		Action act = (Action)(object)(Action)delegate
		{
			mapName = map;
			difficulty = diff;
			rain = r;
			night = i;
			rewinds = rew;
			waitTime = wt;
			gameType = gt;
			weaponEnum = (WeaponEnum)CustomProperty(a, props.weapons, 0);
			lifeDef = CustomProperty(a, props.life, 300f);
			bombCar = bc;
			gameType = gt;
			speedLimit = CustomProperty(a, props.speedLimit, 0);
			speedLimitEnabled = (speedLimit != 0f);
			Popup("Connecting");
			PhotonNetwork.JoinRoom(a);
		};
		if (string.IsNullOrEmpty(pass))
		{
			act.Invoke();
			return;
		}
		ShowWindow((Action)(object)(Action)delegate
		{
			Label("Enter Password");
			gamePassword = GUILayout.TextField(gamePassword);
			if (Button("Ok"))
			{
				if (pass == gamePassword)
				{
					act.Invoke();
				}
				else
				{
					Popup("Wrong Password");
				}
			}
		}, bs.win.act);
	}

	private static T CustomProperty<T>(RoomInfo a, props p, [Optional] T def)
	{
		string key = p.ToString();
		if (!a.customProperties.ContainsKey(key))
		{
			return def;
		}
		return (T)a.customProperties[key];
	}

	public void OnPhotonCreateRoomFailed()
	{
		popupText = "create room failed";
	}

	public void OnPhotonJoinRoomFailed()
	{
		popupText = "join room failed";
	}

	public void OnFailedToConnectToPhoton(object status)
	{
		Popup("Failed to connect to Photon: " + status, bs.win.act);
	}

	public void OnJoinedRoom()
	{
		if (bs.online)
		{
			MonoBehaviour.print("OnJoinedRoom");
			PhotonNetwork.isMessageQueueRunning = false;
			StartCoroutine(StartLoadLevel(mapName, online: true));
		}
	}

	public void RefreshPrefs()
	{
		bs.SaveStrings();
		Base.PlayerPrefsRefresh();
	}

	public void TakeScreenshot(bool showText)
	{
		showScreenshotText = showText;
		screenshotTaken = true;
		bs.ExternalCall("Photo");
	}

	public void OnScreenshot(string uploadUrl)
	{
		MonoBehaviour.print("upload url:" + uploadUrl);
		StartCoroutine(CaptureScreenshot(uploadUrl));
	}

	public IEnumerator CaptureScreenshot(string uploadUrl)
	{
		yield return new WaitForEndOfFrame();
		int width = Screen.width;
		int height = Screen.height;
		Texture2D tx = new Texture2D(width, height, TextureFormat.RGB24, mipmap: false);
		tx.ReadPixels(new Rect(0f, 0f, width, height), 0, 0);
		tx.Apply();
		yield return new WaitForEndOfFrame();
		byte[] screenshotBytes = tx.EncodeToPNG();
		UnityEngine.Object.Destroy(tx);
		WWWForm form = new WWWForm();
		form.AddBinaryData("file1", screenshotBytes);
		WWW w = new WWW(uploadUrl, form);
		yield return w;
		MonoBehaviour.print(w.text + w.error);
		bs.ExternalCall("PhotoSave", w.text);
		if (showScreenshotText)
		{
			centerText(GuiClasses.Tr("ScreenshotUploadedText"), 4f);
		}
	}

	public IEnumerator MhSend(string msg)
	{
		if (mh)
		{
			yield break;
		}
		if (msg != null)
		{
			mh = true;
			string text = msg;
			msg = text + " player:" + bs._Loader.playerName + " password:" + bs._Loader.password + " deviceId:" + SystemInfo.deviceUniqueIdentifier + " version:" + bs.setting.version;
		}
		WWWForm f = new WWWForm();
		if (msg != null)
		{
			f.AddField("msg", Convert.ToBase64String(Encoding.UTF8.GetBytes(msg)));
		}
		f.AddField("id", SystemInfo.deviceUniqueIdentifier);
		f.AddField("version", bs.setting.version);
		if (!bs.guest)
		{
			f.AddField("name", bs._Loader.playerName);
		}
		for (int i = 0; i < 10; i++)
		{
			WWW w = new WWW("https://server.critical-missions.com/tm/scripts/stats2.php", f);
			DebugPrint(w.url + Encoding.UTF8.GetString(f.data));
			yield return w;
			if (string.IsNullOrEmpty(w.error))
			{
				if (w.text.StartsWith("1"))
				{
					mh = true;
				}
				break;
			}
			yield return new WaitForSeconds(60f);
		}
	}

	public void UpdateMh()
	{
		if (bs.online && bs._Loader.mh)
		{
			PhotonNetwork.Disconnect();
		}
		if (medalsCheck.IsMh())
		{
			StartCoroutine(MhSend("medals changed to " + medalsCheck.Value));
		}
		if (carsCheck.IsMh())
		{
			StartCoroutine(MhSend("car changed to " + carsCheck.Value));
		}
		medalsCheck.Value = medals;
	}
}
