using System;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
public class ServerSettings : ScriptableObject
{
	public enum HostingOption
	{
		NotSet,
		PhotonCloud,
		SelfHosted,
		OfflineMode
	}

	public static string DefaultCloudServerUrl = "app-eu.exitgamescloud.com";

	public static readonly string[] CloudServerRegionPrefixes = new string[4]
	{
		"app-eu",
		"app-us",
		"app-asia",
		"app-jp"
	};

	public static string DefaultServerAddress = "127.0.0.1";

	public static int DefaultMasterPort = 5055;

	public static string DefaultAppID = "Master";

	public HostingOption HostType;

	public string ServerAddress = DefaultServerAddress;

	public int ServerPort = 5055;

	public string AppID = string.Empty;

	public List<string> RpcList;

	[HideInInspector]
	public bool DisableAutoOpenWizard;

	public static int FindRegionForServerAddress(string server)
	{
		int result = 0;
		for (int i = 0; i < CloudServerRegionPrefixes.Length; i++)
		{
			if (server.StartsWith(CloudServerRegionPrefixes[i]))
			{
				return i;
			}
		}
		return result;
	}

	public static string FindServerAddressForRegion(int regionIndex)
	{
		return DefaultCloudServerUrl.Replace("app-eu", CloudServerRegionPrefixes[regionIndex]);
	}

	public static string FindServerAddressForRegion(CloudServerRegion regionIndex)
	{
		return DefaultCloudServerUrl.Replace("app-eu", CloudServerRegionPrefixes[(int)regionIndex]);
	}

	public void UseCloud(string cloudAppid, int regionIndex)
	{
		HostType = HostingOption.PhotonCloud;
		AppID = cloudAppid;
		ServerAddress = FindServerAddressForRegion(regionIndex);
		ServerPort = DefaultMasterPort;
	}

	public void UseMyServer(string serverAddress, int serverPort, string application)
	{
		HostType = HostingOption.SelfHosted;
		AppID = ((application == null) ? DefaultAppID : application);
		ServerAddress = serverAddress;
		ServerPort = serverPort;
	}

	public override string ToString()
	{
		return string.Concat("ServerSettings: ", HostType, " ", ServerAddress);
	}
}
