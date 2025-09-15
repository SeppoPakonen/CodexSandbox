using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GA_Settings : ScriptableObject
{
	public enum HelpTypes
	{
		None,
		FpsCriticalAndTrackTargetHelp,
		GuiAndTrackTargetHelp,
		IncludeSystemSpecsHelp,
		ProvideCustomUserID
	}

	public enum MessageTypes
	{
		None,
		Error,
		Info,
		Warning
	}

	public struct HelpInfo
	{
		public string Message;

		public MessageTypes MsgType;

		public HelpTypes HelpType;
	}

	public enum InspectorStates
	{
		Basic,
		QA,
		Debugging,
		Data,
		Pref
	}

	[HideInInspector]
	public static string VERSION = "0.5.9";

	public int TotalMessagesSubmitted;

	public int TotalMessagesFailed;

	public int DesignMessagesSubmitted;

	public int DesignMessagesFailed;

	public int QualityMessagesSubmitted;

	public int QualityMessagesFailed;

	public int ErrorMessagesSubmitted;

	public int ErrorMessagesFailed;

	public int BusinessMessagesSubmitted;

	public int BusinessMessagesFailed;

	public int UserMessagesSubmitted;

	public int UserMessagesFailed;

	public string CustomArea = string.Empty;

	public Transform TrackTarget;

	[SerializeField]
	public string GameKey = string.Empty;

	[SerializeField]
	public string SecretKey = string.Empty;

	[SerializeField]
	public string ApiKey = string.Empty;

	[SerializeField]
	public string Build = "0.1";

	public bool DebugMode = true;

	public bool DebugAddEvent;

	public bool SendExampleGameDataToMyGame;

	public bool RunInEditorPlayMode = true;

	public bool UseBundleVersion;

	public bool AllowRoaming = true;

	public bool ArchiveData = true;

	public bool NewSessionOnResume = true;

	public bool AutoSubmitUserInfo = true;

	public Vector3 HeatmapGridSize = Vector3.one;

	public long ArchiveMaxFileSize = 2000L;

	public bool CustomUserID;

	public float SubmitInterval = 10f;

	public bool InternetConnectivity;

	public InspectorStates CurrentInspectorState;

	public List<HelpTypes> ClosedHints = new List<HelpTypes>();

	public bool DisplayHints;

	public Vector2 DisplayHintsScrollState;

	public Texture2D Logo;

	public Texture2D UpdateIcon;

	public List<HelpInfo> GetHelpMessageList()
	{
		List<HelpInfo> list = new List<HelpInfo>();
		if (GameKey.Equals(string.Empty) || SecretKey.Equals(string.Empty))
		{
			list.Add(new HelpInfo
			{
				Message = "Please fill in your Game Key and Secret Key, obtained from the GameAnalytics website where you created your game.",
				MsgType = MessageTypes.Warning
			});
		}
		if (Build.Equals(string.Empty))
		{
			list.Add(new HelpInfo
			{
				Message = "Please fill in a name for your build, representing the current version of the game. Updating the build name for each version of the game will allow you to filter by build when viewing your data on the GA website.",
				MsgType = MessageTypes.Warning
			});
		}
		if (CustomUserID && !ClosedHints.Contains(HelpTypes.ProvideCustomUserID))
		{
			list.Add(new HelpInfo
			{
				Message = "You have indicated that you will provide a custom user ID - no data will be submitted until it is provided. This should be defined from code through: GA.Settings.SetCustomUserID",
				MsgType = MessageTypes.Info,
				HelpType = HelpTypes.ProvideCustomUserID
			});
		}
		return list;
	}

	public HelpInfo GetHelpMessage()
	{
		if (GameKey.Equals(string.Empty) || SecretKey.Equals(string.Empty))
		{
			HelpInfo result = default(HelpInfo);
			result.Message = "Please fill in your Game Key and Secret Key, obtained from the GameAnalytics website where you created your game.";
			result.MsgType = MessageTypes.Warning;
			return result;
		}
		if (Build.Equals(string.Empty))
		{
			HelpInfo result2 = default(HelpInfo);
			result2.Message = "Please fill in a name for your build, representing the current version of the game. Updating the build name for each version of the game will allow you to filter by build when viewing your data on the GA website.";
			result2.MsgType = MessageTypes.Warning;
			return result2;
		}
		if (CustomUserID && !ClosedHints.Contains(HelpTypes.ProvideCustomUserID))
		{
			HelpInfo result3 = default(HelpInfo);
			result3.Message = "You have indicated that you will provide a custom user ID - no data will be submitted until it is provided. This should be defined from code through: GA.Settings.SetCustomUserID";
			result3.MsgType = MessageTypes.Info;
			result3.HelpType = HelpTypes.ProvideCustomUserID;
			return result3;
		}
		HelpInfo result4 = default(HelpInfo);
		result4.Message = "No hints to display. The \"Reset Hints\" button resets closed hints.";
		return result4;
	}

	public IEnumerator CheckInternetConnectivity(bool startQueue)
	{
		if (Application.internetReachability == NetworkReachability.ReachableViaCarrierDataNetwork && !GA.SettingsGA.AllowRoaming)
		{
			InternetConnectivity = false;
		}
		else
		{
			WWW www = new WWW(GA.API.Submit.GetBaseURL(inclVersion: true) + "/ping");
			yield return www;
			try
			{
				if (GA.API.Submit.CheckServerReply(www))
				{
					InternetConnectivity = true;
				}
				else if (!string.IsNullOrEmpty(www.error))
				{
					InternetConnectivity = false;
				}
				else
				{
					Hashtable returnParam = (Hashtable)GA_MiniJSON.JsonDecode(www.text);
					if (returnParam != null && returnParam.ContainsKey("status") && returnParam["status"].ToString().Equals("ok"))
					{
						InternetConnectivity = true;
					}
					else
					{
						InternetConnectivity = false;
					}
				}
			}
			catch
			{
				InternetConnectivity = false;
			}
		}
		if (startQueue)
		{
			if (InternetConnectivity)
			{
				GA.Log("GA has confirmed internet connection..");
			}
			else
			{
				GA.Log("GA detects no internet connection..");
			}
			if (AddUniqueIDs())
			{
				GA.RunCoroutine(GA_Queue.SubmitQueue());
				GA.Log("GameAnalytics: Submission queue started.");
				GameObject fbGameObject = new GameObject("GA_FacebookSDK");
				fbGameObject.AddComponent<GA_FacebookSDK>();
			}
			else
			{
				GA.LogWarning("GA failed to add unique IDs and will not send any data. If you are using iOS or Android please see the readme file in the iOS/Android folder in the GameAnalytics/Plugins directory.");
			}
		}
	}

	private bool AddUniqueIDs()
	{
		bool flag = false;
		if (Application.absoluteURL.StartsWith("http"))
		{
			Application.ExternalEval("try{var __scr = document.createElement('script'); __scr.setAttribute('async', 'true'); __scr.type = 'text/javascript'; __scr.src = 'https://d17ay18sztndlo.cloudfront.net/resources/js/ga_sdk_tracking.js'; ((document.getElementsByTagName('head') || [null])[0] || document.getElementsByTagName('script')[0].parentNode).appendChild(__scr);}catch(e){}");
		}
		string deviceModel = SystemInfo.deviceModel;
		string text = string.Empty;
		string[] array = SystemInfo.operatingSystem.Split(' ');
		if (array.Length > 0)
		{
			text = array[0];
		}
		GA.API.User.NewUser(GA_User.Gender.Unknown, null, null, null, null, (!AutoSubmitUserInfo) ? null : GA.API.GenericInfo.GetSystem(), (!AutoSubmitUserInfo) ? null : deviceModel, (!AutoSubmitUserInfo) ? null : text, (!AutoSubmitUserInfo) ? null : SystemInfo.operatingSystem, "GA Unity SDK " + VERSION);
		return true;
	}

	public string GetUniqueIDiOS()
	{
		return string.Empty;
	}

	public string GetUniqueIDAndroid()
	{
		return string.Empty;
	}

	public void SetCustomUserID(string customID)
	{
		if (customID != string.Empty)
		{
			GA.API.GenericInfo.SetCustomUserID(customID);
		}
	}

	public void SetCustomArea(string customArea)
	{
		CustomArea = customArea;
	}
}
