using CodeStage.AntiCheat;
using CodeStage.AntiCheat.ObscuredTypes;
using Ionic.Zlib;
using Photon;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class Base : Photon.MonoBehaviour
{
	public class PlayerPrefs
	{
		public static bool HasKey(string key)
		{
			if (bs.setting.playerPrefSecurity)
			{
				return PlayerPrefsObscured.HasKey(key);
			}
			return UnityEngine.PlayerPrefs.HasKey(key);
		}

		public static void SetString(string key, string value)
		{
			try
			{
				if (bs.setting.debugPlayerPrefs)
				{
					throw new Exception();
				}
				if (bs.setting.playerPrefSecurity)
				{
					PlayerPrefsObscured.SetString(key, value);
				}
				else
				{
					UnityEngine.PlayerPrefs.SetString(key, value);
				}
			}
			catch (Exception)
			{
				Loader.errors++;
			}
		}

		public static void SetString2(string key, string value)
		{
			try
			{
				if (bs.setting.debugPlayerPrefs)
				{
					throw new Exception();
				}
				UnityEngine.PlayerPrefs.SetString(key, value);
			}
			catch (Exception)
			{
				Loader.errors++;
			}
		}

		public static string GetString(string key, string defaultValue = "")
		{
			if (bs.setting.playerPrefSecurity)
			{
				return PlayerPrefsObscured.GetString(key, defaultValue);
			}
			return UnityEngine.PlayerPrefs.GetString(key, defaultValue);
		}

		internal static float GetFloat(string p1, float p2 = 0f)
		{
			if (bs.setting.playerPrefSecurity)
			{
				return PlayerPrefsObscured.GetFloat(p1, p2);
			}
			return UnityEngine.PlayerPrefs.GetFloat(p1, p2);
		}

		internal static void SetFloat(string p, float totalSeconds)
		{
			try
			{
				if (bs.setting.debugPlayerPrefs)
				{
					throw new Exception();
				}
				if (bs.setting.playerPrefSecurity)
				{
					PlayerPrefsObscured.SetFloat(p, totalSeconds);
				}
				else
				{
					UnityEngine.PlayerPrefs.SetFloat(p, totalSeconds);
				}
			}
			catch (Exception ex)
			{
				Debug.LogError(ex.Message);
			}
		}

		internal static int GetInt(string p, int p2 = 0)
		{
			if (bs.setting.playerPrefSecurity)
			{
				return PlayerPrefsObscured.GetInt(p, p2);
			}
			return UnityEngine.PlayerPrefs.GetInt(p, p2);
		}

		internal static void SetInt(string p, int totalSeconds)
		{
			try
			{
				if (bs.setting.debugPlayerPrefs)
				{
					throw new Exception();
				}
				if (bs.setting.playerPrefSecurity)
				{
					PlayerPrefsObscured.SetInt(p, totalSeconds);
				}
				else
				{
					UnityEngine.PlayerPrefs.SetInt(p, totalSeconds);
				}
			}
			catch (Exception ex)
			{
				Debug.LogError(ex.Message);
			}
		}

		internal static void DeleteAll()
		{
			UnityEngine.PlayerPrefs.DeleteAll();
			PlayerPrefsObscured.DeleteAll();
		}

		internal static void DeleteKey(string p)
		{
			if (bs.setting.playerPrefSecurity)
			{
				PlayerPrefsObscured.DeleteKey(p);
			}
			else
			{
				UnityEngine.PlayerPrefs.DeleteKey(p);
			}
		}

		internal static void Save()
		{
			try
			{
				if (bs.setting.playerPrefSecurity)
				{
					PlayerPrefsObscured.Save();
				}
				else
				{
					UnityEngine.PlayerPrefs.Save();
				}
			}
			catch (Exception ex)
			{
				Debug.LogError(ex.Message);
			}
		}
	}

	private static Dictionary<string, ObscuredInt> PlayerPrefInt = new Dictionary<string, ObscuredInt>(StringComparer.OrdinalIgnoreCase);

	private static Dictionary<string, ObscuredFloat> PlayerPrefFloat = new Dictionary<string, ObscuredFloat>(StringComparer.OrdinalIgnoreCase);

	public static Dictionary<string, string> PlayerPrefString = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);

	private static Dictionary<string, List<string>> PlayerPrefStrings = new Dictionary<string, List<string>>(StringComparer.OrdinalIgnoreCase);

	private static Dictionary<string, bool> PlayerPrefBool = new Dictionary<string, bool>(StringComparer.OrdinalIgnoreCase);

	public static Dictionary<string, string> SetStrings = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);

	public static HashSet<string> m_playerPrefKeys;

	public static HashSet<string> playerPrefKeys
	{
		get
		{
			if (m_playerPrefKeys == null)
			{
				LoadKeys();
			}
			return m_playerPrefKeys;
		}
		set
		{
			m_playerPrefKeys = value;
		}
	}

	public virtual void Init()
	{
	}

	public virtual void OnEditorGui()
	{
	}

	public static void ClearLog()
	{
	}

	public void SetDirty(UnityEngine.MonoBehaviour g = null)
	{
	}

	public static IEnumerator AddMethod(float seconds, Action act)
	{
		yield return new WaitForSeconds(seconds);
		act.Invoke();
	}

	public IEnumerator AddMethodCor(float seconds, Func<IEnumerator> act)
	{
		yield return new WaitForSeconds(seconds);
		yield return StartCoroutine(act.Invoke());
	}

	public static IEnumerator AddMethod(Action act)
	{
		yield return null;
		act.Invoke();
	}

	public static IEnumerator AddMethod(YieldInstruction y, Action act)
	{
		yield return y;
		act.Invoke();
	}

	public static IEnumerator AddMethod(Func<bool> y, Action act)
	{
		while (!y.Invoke())
		{
			yield return null;
		}
		act.Invoke();
	}

	public static IEnumerator AddUpdate(Func<bool> y)
	{
		yield return null;
		while (y.Invoke())
		{
			yield return null;
		}
	}

	public static void PlayerPrefsClear()
	{
		PlayerPrefsRefresh();
		PlayerPrefs.DeleteAll();
	}

	public static void PlayerPrefsRefresh()
	{
		PlayerPrefStrings.Clear();
		PlayerPrefInt.Clear();
		PlayerPrefFloat.Clear();
		PlayerPrefBool.Clear();
	}

	public static bool PlayerPrefsGetBool(string s2, bool defValue = false)
	{
		if (PlayerPrefBool.TryGetValue(s2, out bool value))
		{
			return value;
		}
		bool flag = boolParse(PlayerPrefsGetString(s2, string.Empty), defValue);
		PlayerPrefBool[s2] = flag;
		return flag;
	}

	public static void PlayerPrefsSetBool(string s, bool value)
	{
		if (!PlayerPrefBool.TryGetValue(s, out bool value2) || value2 != value)
		{
			PlayerPrefBool[s] = value;
			PlayerPrefsSetString(s, value.ToString());
		}
	}

	public static int PlayerPrefsGetInt(string s2, int defValue = 0)
	{
		if (PlayerPrefInt.TryGetValue(s2, out ObscuredInt value))
		{
			return value;
		}
		ObscuredInt value2 = intParse(PlayerPrefsGetString(s2, string.Empty), defValue);
		PlayerPrefInt[s2] = value2;
		return value2;
	}

	public static void PlayerPrefsSetInt(string s, int value)
	{
		if (!PlayerPrefInt.TryGetValue(s, out ObscuredInt value2) || (int)value2 != value)
		{
			PlayerPrefInt[s] = value;
			PlayerPrefsSetString(s, value.ToString());
		}
	}

	public static float PlayerPrefsGetFloat(string s2, float defValue = 0f)
	{
		if (PlayerPrefFloat.TryGetValue(s2, out ObscuredFloat value))
		{
			return value;
		}
		ObscuredFloat value2 = floatParse(PlayerPrefsGetString(s2, string.Empty), defValue);
		PlayerPrefFloat[s2] = value2;
		return value2;
	}

	public static void PlayerPrefsSetFloat(string s, float value)
	{
		if (!PlayerPrefFloat.TryGetValue(s, out ObscuredFloat value2) || (float)value2 != value)
		{
			PlayerPrefFloat[s] = value;
			PlayerPrefsSetString(s, value.ToString());
		}
	}

	public static string PlayerPrefsGetString(string s2, string defValue = "")
	{
		if (PlayerPrefString.TryGetValue(s2, out string value))
		{
			return value;
		}
		string @string = PlayerPrefs.GetString(s2.ToLower(), defValue);
		PlayerPrefString[s2] = @string;
		return @string;
	}

	private static bool boolParse(string getString, bool def)
	{
		if (bool.TryParse(getString, out bool result))
		{
			return result;
		}
		return def;
	}

	private static int intParse(string getString, int def)
	{
		if (int.TryParse(getString, out int result))
		{
			return result;
		}
		return def;
	}

	private static float floatParse(string s1, float def)
	{
		if (float.TryParse(s1, out float result))
		{
			return result;
		}
		return def;
	}

	public static void PlayerPrefsSetString(string s, string value)
	{
		if (!PlayerPrefString.TryGetValue(s, out string value2) || !(value2 == value))
		{
			PlayerPrefString[s] = value;
			SetStrings[s] = value;
			playerPrefKeys.Add(s);
		}
	}

	public static void PlayerPrefsSetStringList(string s, List<string> value)
	{
		PlayerPrefStrings[s] = value;
		PlayerPrefsSetString(s, string.Join("\n", value.ToArray()));
	}

	public static List<string> PlayerPrefsGetStrings(string s2, string def = "")
	{
		if (PlayerPrefStrings.TryGetValue(s2, out List<string> value))
		{
			return value;
		}
		List<string> list = new List<string>(PlayerPrefsGetString(s2, def).Split(new char[1]
		{
			'\n'
		}, StringSplitOptions.RemoveEmptyEntries));
		PlayerPrefStrings[s2] = list;
		return list;
	}

	public static void LoadKeys()
	{
		m_playerPrefKeys = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
		if (bs.setting.disablePlayerPrefs)
		{
			return;
		}
		string @string = UnityEngine.PlayerPrefs.GetString("keysnew3");
		if (!string.IsNullOrEmpty(@string))
		{
			@string = GZipStream.UncompressString(Convert.FromBase64String(@string));
			m_playerPrefKeys = new HashSet<string>(@string.Split(new char[1]
			{
				','
			}, StringSplitOptions.RemoveEmptyEntries), StringComparer.OrdinalIgnoreCase);
			Debug.Log(m_playerPrefKeys.Count + " Keys Loaded3");
			if (m_playerPrefKeys.Count > 10)
			{
				UnityEngine.PlayerPrefs.DeleteKey("keysnew2");
			}
			return;
		}
		if (!string.IsNullOrEmpty(@string = UnityEngine.PlayerPrefs.GetString("keysnew2")))
		{
			m_playerPrefKeys = new HashSet<string>(@string.Split(new char[1]
			{
				','
			}, StringSplitOptions.RemoveEmptyEntries), StringComparer.OrdinalIgnoreCase);
			UnityEngine.MonoBehaviour.print(m_playerPrefKeys.Count + " Keys Loaded");
			return;
		}
		bool flag = false;
		int num = 0;
		int num2 = 0;
		while (num2 < 10 || flag)
		{
			flag = PlayerPrefs.HasKey("keys" + num);
			if (flag)
			{
				m_playerPrefKeys.Add(PlayerPrefs.GetString("keys" + num, string.Empty));
			}
			else
			{
				num2++;
			}
			num++;
		}
		UnityEngine.MonoBehaviour.print("Old " + m_playerPrefKeys.Count + " Keys Loaded");
	}

	public T SetSecure<T>(T value, string s)
	{
		PlayerPrefsSetBool(GetHash(value, s), value: true);
		return value;
	}

	private static string GetHash<T>(T value, string s)
	{
		return (bs._Loader.playerName.GetHashCode() ^ s.GetHashCode() ^ value.GetHashCode()).ToString();
	}

	public T GetSecureOff<T>(T o, string s, [Optional] T def)
	{
		return o;
	}

	public T GetSecure<T>(T o, string s, [Optional] T def)
	{
		if (PlayerPrefsGetBool(GetHash(o, s)))
		{
			return o;
		}
		return def;
	}
}
