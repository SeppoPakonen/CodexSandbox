using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GA_GenericInfo
{
	private string _userID = string.Empty;

	private string _sessionID;

	private bool _settingUserID;

	public string UserID
	{
		get
		{
			if ((_userID == null || _userID == string.Empty) && !GA.SettingsGA.CustomUserID)
			{
				_userID = GetUserUUID();
			}
			return _userID;
		}
	}

	public string SessionID
	{
		get
		{
			if (_sessionID == null)
			{
				_sessionID = GetSessionUUID();
			}
			return _sessionID;
		}
	}

	public List<Hashtable> GetGenericInfo(string message)
	{
		List<Hashtable> list = new List<Hashtable>();
		list.Add(AddSystemSpecs("unity_wrapper", GA_Settings.VERSION, message));
		list.Add(AddSystemSpecs("os", SystemInfo.operatingSystem, message));
		list.Add(AddSystemSpecs("processor_type", SystemInfo.processorType, message));
		list.Add(AddSystemSpecs("gfx_name", SystemInfo.graphicsDeviceName, message));
		list.Add(AddSystemSpecs("gfx_version", SystemInfo.graphicsDeviceVersion, message));
		return list;
	}

	public string GetUserUUID()
	{
		return SystemInfo.deviceUniqueIdentifier;
	}

	public string GetSessionUUID()
	{
		return Guid.NewGuid().ToString();
	}

	public void SetSessionUUID()
	{
		_sessionID = GetSessionUUID();
	}

	public void SetCustomUserID(string customID)
	{
		_userID = customID;
	}

	private Hashtable AddSystemSpecs(string key, string type, string message)
	{
		string str = string.Empty;
		if (message != string.Empty)
		{
			str = ": " + message;
		}
		Hashtable hashtable = new Hashtable();
		hashtable.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.EventID], "system:" + key);
		hashtable.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Message], type + str);
		hashtable.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Level], (!GA.SettingsGA.CustomArea.Equals(string.Empty)) ? GA.SettingsGA.CustomArea : Application.loadedLevelName);
		return hashtable;
	}

	public string GetSystem()
	{
		return "WEBPLAYER";
	}
}
