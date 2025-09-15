using System;
using System.Collections;
using System.Collections.Generic;
using System.Security.Cryptography;
using System.Text;
using UnityEngine;

public class GA_Submit
{
	public enum CategoryType
	{
		GA_User,
		GA_Event,
		GA_Log,
		GA_Purchase,
		GA_Error
	}

	public struct Item
	{
		public CategoryType Type;

		public Hashtable Parameters;

		public float AddTime;

		public int Count;
	}

	public delegate void SubmitSuccessHandler(List<Item> items, bool success);

	public delegate void SubmitErrorHandler(List<Item> items);

	public Dictionary<CategoryType, string> Categories;

	private string _publicKey;

	private string _privateKey;

	private string _baseURL = "://api.gameanalytics.com";

	private string _version = "1";

	public void SetupKeys(string publicKey, string privateKey)
	{
		_publicKey = publicKey;
		_privateKey = privateKey;
		Categories = new Dictionary<CategoryType, string>
		{
			{
				CategoryType.GA_User,
				"user"
			},
			{
				CategoryType.GA_Event,
				"design"
			},
			{
				CategoryType.GA_Log,
				"quality"
			},
			{
				CategoryType.GA_Purchase,
				"business"
			},
			{
				CategoryType.GA_Error,
				"error"
			}
		};
	}

	public void SubmitQueue(List<Item> queue, SubmitSuccessHandler successEvent, SubmitErrorHandler errorEvent, bool gaTracking, string pubKey, string priKey)
	{
		if ((_publicKey.Equals(string.Empty) || _privateKey.Equals(string.Empty)) && (pubKey.Equals(string.Empty) || priKey.Equals(string.Empty)))
		{
			if (!gaTracking)
			{
				GA.LogError("Game Key and/or Secret Key not set. Open GA_Settings to set keys.");
			}
			return;
		}
		Dictionary<CategoryType, List<Item>> dictionary = new Dictionary<CategoryType, List<Item>>();
		foreach (Item item in queue)
		{
			if (dictionary.ContainsKey(item.Type))
			{
				if (!item.Parameters.ContainsKey(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.UserID]))
				{
					item.Parameters.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.UserID], GA.API.GenericInfo.UserID);
				}
				if (!item.Parameters.ContainsKey(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.SessionID]))
				{
					item.Parameters.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.SessionID], GA.API.GenericInfo.SessionID);
				}
				if (!item.Parameters.ContainsKey(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Build]))
				{
					item.Parameters.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Build], GA.SettingsGA.Build);
				}
				dictionary[item.Type].Add(item);
				continue;
			}
			if (!item.Parameters.ContainsKey(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.UserID]))
			{
				item.Parameters.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.UserID], GA.API.GenericInfo.UserID);
			}
			if (!item.Parameters.ContainsKey(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.SessionID]))
			{
				item.Parameters.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.SessionID], GA.API.GenericInfo.SessionID);
			}
			if (!item.Parameters.ContainsKey(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Build]))
			{
				item.Parameters.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Build], GA.SettingsGA.Build);
			}
			dictionary.Add(item.Type, new List<Item>
			{
				item
			});
		}
		GA.RunCoroutine(Submit(dictionary, successEvent, errorEvent, gaTracking, pubKey, priKey));
	}

	public IEnumerator Submit(Dictionary<CategoryType, List<Item>> categories, SubmitSuccessHandler successEvent, SubmitErrorHandler errorEvent, bool gaTracking, string pubKey, string priKey)
	{
		if (pubKey.Equals(string.Empty))
		{
			pubKey = _publicKey;
		}
		if (priKey.Equals(string.Empty))
		{
			priKey = _privateKey;
		}
		foreach (KeyValuePair<CategoryType, List<Item>> category in categories)
		{
			List<Item> items = category.Value;
			if (items.Count == 0)
			{
				break;
			}
			Item item = items[0];
			CategoryType serviceType = item.Type;
			string url = GetURL(Categories[serviceType], pubKey);
			List<Hashtable> itemsParameters = new List<Hashtable>();
			for (int i = 0; i < items.Count; i++)
			{
				Item item2 = items[i];
				if (serviceType != item2.Type)
				{
					GA.LogWarning("GA Error: All messages in a submit must be of the same service/category type.");
					errorEvent?.Invoke(items);
					yield break;
				}
				Item item3 = items[i];
				if (!item3.Parameters.ContainsKey(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.UserID]))
				{
					Item item4 = items[i];
					item4.Parameters.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.UserID], GA.API.GenericInfo.UserID);
				}
				else
				{
					Item item5 = items[i];
					if (item5.Parameters[GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.UserID]] == null)
					{
						Item item6 = items[i];
						item6.Parameters[GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.UserID]] = GA.API.GenericInfo.UserID;
					}
				}
				Item item7 = items[i];
				Hashtable parameters;
				if (item7.Count > 1)
				{
					Item item8 = items[i];
					parameters = item8.Parameters;
				}
				else
				{
					Item item9 = items[i];
					parameters = item9.Parameters;
				}
				itemsParameters.Add(parameters);
			}
			string json = DictToJson(itemsParameters);
			if (GA.SettingsGA.ArchiveData && !gaTracking && !GA.SettingsGA.InternetConnectivity)
			{
				if (GA.SettingsGA.DebugMode)
				{
					GA.Log("GA: Archiving data (no network connection).");
				}
				GA.API.Archive.ArchiveData(json, serviceType);
				successEvent?.Invoke(items, success: true);
				break;
			}
			if (!GA.SettingsGA.InternetConnectivity)
			{
				if (!gaTracking)
				{
					GA.LogWarning("GA Error: No network connection.");
				}
				errorEvent?.Invoke(items);
				break;
			}
			byte[] data = Encoding.UTF8.GetBytes(json);
			WWW www2 = null;
			www2 = new WWW(url, data, new Hashtable
			{
				{
					"Authorization",
					CreateMD5Hash(json + priKey)
				}
			})
			{
				threadPriority = ThreadPriority.Low
			};
			yield return www2;
			if (GA.SettingsGA.DebugMode && !gaTracking)
			{
				GA.Log("GA URL: " + url);
				GA.Log("GA Submit: " + json);
				GA.Log("GA Hash: " + CreateMD5Hash(json + priKey));
			}
			try
			{
				if (!string.IsNullOrEmpty(www2.error) && !CheckServerReply(www2))
				{
					throw new Exception(www2.error);
				}
				Hashtable returnParam = (Hashtable)GA_MiniJSON.JsonDecode(www2.text);
				if ((returnParam != null && returnParam.ContainsKey("status") && returnParam["status"].ToString().Equals("ok")) || CheckServerReply(www2))
				{
					if (GA.SettingsGA.DebugMode && !gaTracking)
					{
						GA.Log("GA Result: " + www2.text);
					}
					successEvent?.Invoke(items, success: true);
				}
				else if (returnParam != null && returnParam.ContainsKey("message") && returnParam["message"].ToString().Equals("Game not found") && returnParam.ContainsKey("code") && returnParam["code"].ToString().Equals("400"))
				{
					if (!gaTracking)
					{
						GA.LogWarning("GA Error: " + www2.text + " (NOTE: make sure your Game Key and Secret Key match the keys you recieved from the Game Analytics website. It might take a few minutes before a newly added game will be able to recieve data.)");
					}
					errorEvent?.Invoke(null);
				}
				else
				{
					if (!gaTracking)
					{
						GA.LogWarning("GA Error: " + www2.text);
					}
					errorEvent?.Invoke(items);
				}
			}
			catch (Exception e)
			{
				if (!gaTracking)
				{
					GA.LogWarning("GA Error: " + e.Message);
				}
				if (e.Message.Contains("400 Bad Request"))
				{
					errorEvent?.Invoke(null);
				}
				else
				{
					errorEvent?.Invoke(items);
				}
			}
		}
	}

	public string GetBaseURL(bool inclVersion)
	{
		if (inclVersion)
		{
			return GetUrlStart() + _baseURL + "/" + _version;
		}
		return GetUrlStart() + _baseURL;
	}

	public string GetURL(string category, string pubKey)
	{
		return GetUrlStart() + _baseURL + "/" + _version + "/" + pubKey + "/" + category;
	}

	private string GetUrlStart()
	{
		if (Application.absoluteURL.StartsWith("https"))
		{
			return "https";
		}
		return "http";
	}

	public string CreateMD5Hash(string input)
	{
		MD5 mD = new MD5CryptoServiceProvider();
		byte[] bytes = Encoding.UTF8.GetBytes(input);
		byte[] array = mD.ComputeHash(bytes);
		string text = string.Empty;
		byte[] array2 = array;
		foreach (byte b in array2)
		{
			text += $"{b:x2}";
		}
		return text;
	}

	public string CreateSha1Hash(string input)
	{
		SHA1 sHA = new SHA1CryptoServiceProvider();
		byte[] bytes = Encoding.UTF8.GetBytes(input);
		byte[] inArray = sHA.ComputeHash(bytes);
		return Convert.ToBase64String(inArray);
	}

	public string GetPrivateKey()
	{
		return _privateKey;
	}

	public bool CheckServerReply(WWW www)
	{
		//Discarded unreachable code: IL_0104, IL_0112
		try
		{
			if (!string.IsNullOrEmpty(www.error))
			{
				string text = www.error.Substring(0, 3);
				if (text.Equals("201") || text.Equals("202") || text.Equals("203") || text.Equals("204") || text.Equals("205") || text.Equals("206"))
				{
					return true;
				}
			}
			if (!www.responseHeaders.ContainsKey("STATUS"))
			{
				return false;
			}
			string text2 = www.responseHeaders["STATUS"];
			string[] array = text2.Split(' ');
			if (array.Length > 1 && int.TryParse(array[1], out int result) && result >= 200 && result < 300)
			{
				return true;
			}
			return false;
		}
		catch
		{
			return false;
		}
	}

	public static string DictToJson(List<Hashtable> list)
	{
		string text = "[";
		int num = 0;
		int num2 = 0;
		foreach (Hashtable item in list)
		{
			text += '{';
			num2 = 0;
			foreach (object key in item.Keys)
			{
				num2++;
				string text2 = text;
				text = string.Concat(text2, "\"", key, "\":\"", item[key], "\"");
				if (num2 < item.Keys.Count)
				{
					text += ',';
				}
			}
			text += '}';
			num++;
			if (num < list.Count)
			{
				text += ',';
			}
		}
		return text + "]";
	}
}
