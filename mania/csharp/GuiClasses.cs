using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text.RegularExpressions;
using UnityEngine;

public class GuiClasses : bs
{
	protected static int cnt;

	private bool[] toggledFlags2 = new bool[50];

	private string[] toggledFlagsstr = new string[50];

	public static bool skipNext;

	public static Dictionary<string, string> trcache = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);

	public static GUIStyle replacementStyle;

	internal string popupText;

	private static Dictionary<string, string> m_dict;

	private static Dictionary<string, string> tooltips = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);

	internal static Vector2 scroll;

	public static TextAsset assetDictionary => bs.setting.assetDictionaries[Mathf.Max(0, curDict)];

	protected static int curDict
	{
		get
		{
			return PlayerPrefs.GetInt("Dict", bs._Loader.vkSite ? 1 : 0);
		}
		set
		{
			PlayerPrefs.SetInt("Dict", value);
			dict = null;
		}
	}

	internal static Dictionary<string, string> dict
	{
		get
		{
			if (m_dict == null)
			{
				LoadTranslate();
			}
			return m_dict;
		}
		set
		{
			m_dict = value;
		}
	}

	public GUISkin skin
	{
		get
		{
			if (bs.win.skin == null)
			{
				bs.win.skin = GUI.skin;
			}
			return bs.win.skin;
		}
	}

	public static string CreateTable(string source)
	{
		string text = string.Empty;
		MatchCollection matchCollection = Regex.Matches(source, "\\w*\\s*");
		for (int i = 0; i < matchCollection.Count - 1; i++)
		{
			string text2 = text;
			text = text2 + "{" + i + ",-" + matchCollection[i].Length + "}";
		}
		return text;
	}

	public void ToggleTab(string name, bool b = true)
	{
		for (int i = 0; i < 50; i++)
		{
			if (toggledFlagsstr[i] == name)
			{
				toggledFlags2[i] = b;
			}
		}
	}

	public bool BeginVertical(string name)
	{
		cnt++;
		toggledFlags2[cnt] = GUILayout.Toggle(toggledFlags2[cnt], Tr(name), skin.GetStyle("ToolbarDropDown"));
		toggledFlagsstr[cnt] = name;
		if (toggledFlags2[cnt])
		{
			GUILayout.BeginVertical(skin.GetStyle("CN Box"));
		}
		return toggledFlags2[cnt];
	}

	public bool BackButton(string s = "Back")
	{
		return Button(s, expandWidth: false) || Input.GetKeyDown(KeyCode.Escape);
	}

	public bool BackButtonLeft()
	{
		return ButtonLeft("Back", null, 0f) || Input.GetKeyDown(KeyCode.Escape);
	}

	public static void LoadTranslate()
	{
		dict = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
		tooltips = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
		if (!bs.setting.disableTranslate)
		{
			Load(bs.setting.assetDictionaries[0]);
			Load(assetDictionary);
		}
	}

	private static void Load(TextAsset AssetDictionary)
	{
		string[] array = AssetDictionary.text.Split(new string[4]
		{
			";\n",
			"; \n",
			";\r\n",
			"; \r\n"
		}, StringSplitOptions.RemoveEmptyEntries);
		MonoBehaviour.print("LoadTranslate " + AssetDictionary.name + array.Length);
		string[] array2 = array;
		foreach (string text in array2)
		{
			string[] array3 = text.Split(';');
			string text2 = array3[0];
			if (array3.Length == 1)
			{
				dict[text2] = text2;
			}
			if (array3.Length >= 2)
			{
				dict[text2] = array3[1];
			}
			if (array3.Length >= 3)
			{
				tooltips[text2] = array3[2];
			}
		}
	}

	public bool GlowButton(string s, bool glow)
	{
		Texture2D background = skin.button.normal.background;
		skin.button.normal.background = ((!glow) ? skin.button.normal.background : skin.button.active.background);
		bool result = SoundButton(GUILayout.Button(Tr(s), GUILayout.MinWidth(50f)));
		skin.button.normal.background = background;
		return result;
	}

	public bool GlowButton(GUIContent s, bool glow)
	{
		Texture2D background = skin.button.normal.background;
		skin.button.normal.background = ((!glow) ? skin.button.normal.background : skin.button.active.background);
		bool result = SoundButton(GUILayout.Button(s, GUILayout.MinWidth(50f)));
		skin.button.normal.background = background;
		return result;
	}

	public int Toolbar(int id, IList<string> getNames, bool expand, bool center = false, int limit = 99, int hor = -1, bool useSkin = true)
	{
		if (getNames[bs.Mod(id, getNames.Count)] == null)
		{
			for (int i = 0; i < getNames.Count && i < limit && getNames[i] == null; i++)
			{
				id = i;
			}
		}
		if (useSkin)
		{
			GUILayout.BeginVertical(skin.box);
		}
		GUILayout.BeginHorizontal();
		if (center)
		{
			GUILayout.FlexibleSpace();
		}
		int j = 0;
		int num = 0;
		for (; j < getNames.Count && j < limit; j++)
		{
			if (hor != -1 && num % hor == 0 && num != 0)
			{
				GUILayout.EndHorizontal();
				GUILayout.BeginHorizontal();
			}
			if (getNames[j] != null)
			{
				num++;
				Texture2D background = skin.button.normal.background;
				skin.button.normal.background = ((id != j) ? skin.button.normal.background : skin.button.active.background);
				if (JoystickButton2(skin.button, id == j) || SoundButton(GUILayout.Button(GUIContent(Tr(getNames[j]), null, Tp(getNames[j])), GUILayout.ExpandWidth(expand), GUILayout.MinWidth(50f))))
				{
					id = j;
				}
				skin.button.normal.background = background;
			}
		}
		if (center)
		{
			GUILayout.FlexibleSpace();
		}
		GUILayout.EndHorizontal();
		if (useSkin)
		{
			GUILayout.EndVertical();
		}
		return id;
	}

	private static void SaveTr(string s)
	{
	}

	private static void WriteAllText(string assetPath, string s)
	{
	}

	public string Tp(string s)
	{
		if (string.IsNullOrEmpty(s) || s.Length > 300)
		{
			return string.Empty;
		}
		if (bs.setting.disableTranslate)
		{
			return s;
		}
		tooltips.TryGetValue(s, out string value);
		return value;
	}

	public string Trs(string s)
	{
		return Tr(s, sk: true);
	}

	public string Trn(string s)
	{
		return Tr(s, sk: false, save: false);
	}

	public static string Tr(string s, bool sk = false, bool save = true)
	{
		if (bs.setting.disableTranslate || string.IsNullOrEmpty(s))
		{
			return s;
		}
		if (skipNext)
		{
			skipNext = sk;
			return s;
		}
		skipNext = sk;
		if (string.IsNullOrEmpty(s) || s.Length > 300)
		{
			return s;
		}
		if (trcache.TryGetValue(s, out string value))
		{
			return value;
		}
		string s2 = s.Replace("\r", "\\r").Replace("\n", "\\n");
		s2 = Trim(s2, out string start, out string end);
		if (string.IsNullOrEmpty(s2))
		{
			return s;
		}
		if (Application.isEditor && bs.resEditor.saveTr && save)
		{
			SaveTr(s2);
		}
		dict.TryGetValue(s2, out string value2);
		string text = (!string.IsNullOrEmpty(value2)) ? (start + bs.unescape(value2) + end) : s;
		trcache[s] = text;
		return text;
	}

	private static string Trim(string s, out string start, out string end)
	{
		char[] trimChars = new char[8]
		{
			':',
			' ',
			'\t',
			'.',
			',',
			'>',
			'<',
			'\n'
		};
		start = s.TrimStart(trimChars);
		start = s.Substring(0, s.Length - start.Length);
		end = s.TrimEnd(trimChars);
		end = s.Substring(end.Length);
		s = s.Substring(start.Length, Mathf.Max(0, s.Length - end.Length - start.Length));
		return s;
	}

	public Vector2 BeginScrollView(Vector3 v)
	{
		return GUILayout.BeginScrollView(v);
	}

	public void Label(string s, int fontSize = 16, bool wrap = true, bool expand = false)
	{
		skin.label.imagePosition = ImagePosition.ImageLeft;
		skin.label.alignment = TextAnchor.UpperLeft;
		skin.label.wordWrap = wrap;
		skin.label.fontSize = fontSize;
		GUILayout.Label(Tr(s), GUILayout.ExpandWidth(expand));
	}

	public bool Toggle(bool toggle, string text)
	{
		bool flag = GUILayout.Toggle(toggle, GUIContent(Tr(text), null, Tp(text)), GUILayout.ExpandWidth(expand: false));
		SoundButton(toggle != flag);
		return flag;
	}

	public void LabelCenter(string s, int fontSize = 16, bool wrap = false, Texture2D txt = null, GUIStyle style = null)
	{
		if (style == null)
		{
			style = skin.label;
		}
		style.wordWrap = wrap;
		style.fontSize = fontSize;
		style.alignment = TextAnchor.UpperCenter;
		GUILayout.Label(GUIContent(Tr(s), txt), style);
		skin.label.alignment = TextAnchor.UpperLeft;
	}

	public bool ButtonTexture(string s, Texture2D texture, int height)
	{
		return GUILayout.Button(new GUIContent(s, texture), GUILayout.Height(height));
	}

	public bool ButtonTexture(string s, Texture2D texture, bool expandWidth = true, int font = 14, bool bold = false)
	{
		return Button(s, expandWidth, font, bold, texture);
	}

	public bool ButtonLeft(string s, Texture2D txt = null, float height = 0f, GUIStyle style = null, int fontsize = 12)
	{
		GUILayout.BeginHorizontal();
		GUILayout.FlexibleSpace();
		skin.button.fontSize = fontsize;
		GUI.SetNextControlName(s);
		if (JoystickButton2(skin.button))
		{
			return true;
		}
		bool flag = GUILayout.Button(GUIContent(Tr(s), txt), style ?? skin.button, GUILayout.ExpandWidth(expand: false), (height != 0f) ? GUILayout.Height(height) : GUILayout.MinWidth(100f));
		SoundButton(flag);
		GUILayout.EndHorizontal();
		return flag;
	}

	public bool Button(string s, bool expandWidth = true, int font = 14, bool bold = false, Texture2D texture = null, bool wrap = false)
	{
		GUIStyle gUIStyle = (replacementStyle != null) ? replacementStyle : skin.button;
		gUIStyle.fontSize = font;
		gUIStyle.wordWrap = wrap;
		gUIStyle.fontStyle = (bold ? FontStyle.Bold : FontStyle.Normal);
		GUI.SetNextControlName(s);
		if (JoystickButton2(gUIStyle))
		{
			return true;
		}
		bool button = GUILayout.Button(GUIContent(Tr(s), texture, Tp(s)), gUIStyle, GUILayout.ExpandWidth(expand: false), GUILayout.MinWidth(100f), GUILayout.ExpandWidth(expandWidth));
		return SoundButton(button);
	}

	public static bool JoystickButton2(GUIStyle style, bool sel = false)
	{
		if (!bs.setting.useKeysForGui)
		{
			return false;
		}
		if (CustomWindow.buttonId == -1)
		{
			return false;
		}
		bool flag = CustomWindow.buttonId++ == CustomWindow.curButton;
		style.normal = ((!flag && !sel) ? style.onNormal : style.hover);
		if (flag && CustomWindow.backSpaceDown)
		{
			CustomWindow.backSpaceDown = false;
			return SoundButton(button: true);
		}
		return false;
	}

	public void ShowWindow(Action func, Action back = null)
	{
		bs.win.ShowWindow(func, back);
	}

	public void Setup(int x = 400, int y = 300, string tittle = "", Dock dock = Dock.Center)
	{
		bs.win.Setup(x, y, tittle, dock, null, null, 1f);
	}

	public static bool SoundButton(bool button)
	{
		if (Input.GetMouseButtonUp(0) && button)
		{
			bs.PlayOneShotGui(bs._Loader.pushButton);
		}
		return button;
	}

	public void Popup(string msg, Action back = null, string button = null, bool skip = true)
	{
		Popup2(msg, back ?? bs.win.act, button, skip);
	}

	public void Popup(string msg, Action back, int width, int height)
	{
		Popup2(msg, back, null, skip: false, width, height);
	}

	public void Popup(string msg, Texture2D txt)
	{
		Popup2(msg, bs.win.act, null, skip: false, 500, 250, txt);
	}

	[DebuggerStepThrough]
	public void Popup2(string msg, Action back = null, string button = null, bool skip = true, int width = 500, int height = 250, Texture2D txt = null)
	{
		//IL_006a: Unknown result type (might be due to invalid IL or missing references)
		//IL_007b: Expected O, but got Unknown
		MonoBehaviour.print(msg);
		popupText = msg;
		if (button == null)
		{
			button = "back";
		}
		bs.win.ShowWindow((Action)(object)(Action)delegate
		{
			bs.win.Setup(width, height, string.Empty, Dock.Center, txt, null, 1f);
			BeginScrollView();
			LabelCenter(popupText, 20, wrap: true);
			GUILayout.EndScrollView();
			if (back != null && ButtonLeft(button, null, 0f))
			{
				bs.win.ShowWindow(back, null, skip);
			}
		}, null, skip);
	}

	public void BeginScrollView(GUIStyle style = null, bool showHorizontal = false, bool showVertical = true)
	{
		if (style == null)
		{
			style = skin.scrollView;
		}
		skin.scrollView.fixedWidth = 0f;
		Vector2 lhs = GUILayout.BeginScrollView(scroll, false, false, (!showHorizontal) ? GUIStyle.none : skin.horizontalScrollbar, (!showVertical) ? GUIStyle.none : skin.verticalScrollbar, style);
		if (lhs == scroll)
		{
			Vector2 mouseDelta = getMouseDelta(returnMouse: false);
			lhs += new Vector2(0f - mouseDelta.x, mouseDelta.y) * 3f;
		}
		scroll = lhs;
	}
}
