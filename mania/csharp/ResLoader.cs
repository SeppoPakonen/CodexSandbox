using System.Collections.Generic;
using UnityEngine;

public class ResLoader : bs
{
	public bool dontLoadAssets;

	public bool disablePlayerPrefs;

	public bool disableTranslate;

	public bool wwwCache;

	public bool lag;

	public bool lagNetw;

	public bool autoConnect;

	public bool autoHost;

	public int packageVersion;

	public int packageVersion2;

	public bool enableGuiEdit;

	public bool m_android;

	public bool m_ios;

	public int version = 20;

	public int replayVersion = 1205;

	public int levelEditorVersion = 20;

	public int physicVersion = 6;

	public bool printPlayerPrefs = true;

	public bool debug = true;

	public bool debugPlayerPrefs;

	public bool enableLog = true;

	public bool localhost = true;

	public bool noLevelCache;

	public bool skipLogin;

	internal bool inited;

	public bool delayLoading;

	internal bool vk2;

	public bool testVK;

	public bool offline;

	public bool fps10;

	internal bool unitTest;

	public List<Map> maps = new List<Map>();

	public TextAsset[] assetDictionaries;

	public TextAsset carSkinsTxt;

	public bool useKeysForGui;

	public bool sendWmp = true;

	public bool hideCull;

	public int MapVersion = 2;

	public int androidMapVersion = 3;

	internal string hostTHisGame = "You are allowed to share, link, download, publish, or embed this game on your website or other platforms, provided that the games are republished in exactly the same manner as provided on this web page. Games may not be modified in any way whatsoever, and all branding and hyperlinks included therein must be maintained.";

	public float hitTestForce = 0.99f;

	public int multiplayerVersion;

	internal bool playerPrefSecurity = true;

	public bool ForceLogin;

	public bool AngleTest;

	public bool enableDrag = true;

	public bool zanos;

	public bool timeLapse;

	public bool speedTweak = true;

	public bool DontWait => skipLogin;

	public void Disable()
	{
	}

	public void InitSettings()
	{
		inited = true;
		if (!Debug.isDebugBuild)
		{
			ForceLogin = (disablePlayerPrefs = (debugPlayerPrefs = (AngleTest = (disableTranslate = (testVK = (dontLoadAssets = (debug = (lagNetw = (lag = (autoHost = (autoConnect = (unitTest = (wwwCache = (enableGuiEdit = (fps10 = (offline = (enableLog = (m_ios = (m_android = (noLevelCache = (hideCull = (skipLogin = (delayLoading = (localhost = false))))))))))))))))))))))));
		}
		if (Application.platform == RuntimePlatform.IPhonePlayer)
		{
			m_ios = true;
		}
		if (Application.platform == RuntimePlatform.Android || Application.platform == RuntimePlatform.IPhonePlayer || Application.platform == RuntimePlatform.WP8Player)
		{
			m_android = true;
		}
	}
}
