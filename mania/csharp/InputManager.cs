using System;
using System.Collections.Generic;
using UnityEngine;

public class InputManager : bs
{
	public enum KeyAction
	{
		KeyDown,
		KeyUp,
		Key
	}

	public struct DR
	{
		public KeyCode key;

		public Func<KeyCode, bool> func;

		public DR(KeyCode a, Func<KeyCode, bool> b)
		{
			key = a;
			func = b;
		}
	}

	public class Axis
	{
		public string s;

		public float min;
	}

	internal List<KeyValue> keys = new List<KeyValue>();

	public Player pl;

	private KeyValue[] m_alternatives;

	public Dictionary<KeyCode, KeyCode> KeyBack = new Dictionary<KeyCode, KeyCode>();

	public Dictionary<KeyCode, AndroidHud.KeyHudBool> dict;

	public static bool enableKeys = true;

	private static Axis[] m_buttons;

	public bool secondPlayer => pl != null && pl.secondPlayer;

	internal KeyValue[] alternatives => (m_alternatives != null) ? m_alternatives : InitDict();

	public static Axis[] Buttons
	{
		get
		{
			if (m_buttons == null)
			{
				m_buttons = new Axis[459];
				m_buttons[97] = new Axis
				{
					s = "Horizontal",
					min = -0.1f
				};
				m_buttons[100] = new Axis
				{
					s = "Horizontal",
					min = 0.1f
				};
				m_buttons[276] = new Axis
				{
					s = "Horizontal2",
					min = -0.1f
				};
				m_buttons[275] = new Axis
				{
					s = "Horizontal2",
					min = 0.1f
				};
			}
			return m_buttons;
		}
	}

	public void Start()
	{
		InitDict();
		if (pl != null && pl.androidHud != null && (bool)pl.androidHud)
		{
			dict = pl.androidHud.dict;
		}
	}

	public KeyValue[] InitDict()
	{
		m_alternatives = new KeyValue[400];
		KeyCode keyCode = (!bs.splitScreen) ? KeyCode.JoystickButton2 : KeyCode.Joystick1Button2;
		KeyCode keyCode2 = (!bs.splitScreen) ? KeyCode.JoystickButton0 : KeyCode.Joystick1Button0;
		KeyCode keyCode3 = (!bs.splitScreen) ? KeyCode.JoystickButton1 : KeyCode.Joystick1Button1;
		KeyCode keyCode4 = (!bs.splitScreen) ? KeyCode.JoystickButton3 : KeyCode.Joystick1Button3;
		Add(KeyCode.Backspace, "Revert Time", KeyCode.Mouse2, keyCode, KeyCode.Joystick2Button2, KeyCode.E, KeyCode.Insert);
		Add(KeyCode.Escape, "Menu", KeyCode.JoystickButton4, KeyCode.M, KeyCode.F1, KeyCode.F10);
		Add(KeyCode.R, "Restart Level", KeyCode.Delete, KeyCode.JoystickButton7);
		Add(KeyCode.F, "Horn", KeyCode.JoystickButton6);
		Add(KeyCode.LeftShift, "Nitro", KeyCode.RightShift, KeyCode.Mouse4, KeyCode.JoystickButton5);
		Add(KeyCode.RightControl, "Drift", KeyCode.LeftControl, KeyCode.Mouse3);
		Add(KeyCode.S, "Down/Brake", KeyCode.DownArrow, keyCode2, KeyCode.Joystick2Button0, KeyCode.Space);
		Add(KeyCode.Space, "Brake", KeyCode.End, keyCode3, KeyCode.Joystick2Button1, KeyCode.Mouse1);
		Add(KeyCode.W, "Up", KeyCode.UpArrow, keyCode4, KeyCode.Joystick2Button3, (!bs._Loader.dm) ? KeyCode.Mouse0 : KeyCode.JoystickButton9);
		Add(KeyCode.A, "Left", KeyCode.LeftArrow);
		Add(KeyCode.D, "Right", KeyCode.RightArrow);
		Add(KeyCode.Q, "Look Back");
		Add(KeyCode.Return, "Chat");
		Add(KeyCode.Y, "Voice Chat");
		Add(KeyCode.U, "Headlights");
		Add(KeyCode.X, "Jump", KeyCode.G);
		Add(KeyCode.F1, "Screenshot");
		Add(KeyCode.F12, "FullScreen");
		return m_alternatives;
	}

	public void Add(KeyCode key, string descr, params KeyCode[] alt)
	{
		List<KeyCode> list = new List<KeyCode>(alt);
		list.Insert(0, key);
		foreach (KeyCode item in list)
		{
			KeyBack[item] = key;
		}
		KeyValue keyValue = new KeyValue();
		keyValue.descr = descr;
		keyValue.keyCodeAlt = list.ToArray();
		KeyValue keyValue2 = keyValue;
		keys.Add(keyValue2);
		m_alternatives[(int)key] = keyValue2;
	}

	public void Update()
	{
	}

	public bool GetKey(KeyCode key)
	{
		return GetKey(key, KeyAction.Key);
	}

	public bool GetKeyDown(KeyCode key)
	{
		return GetKey(key, KeyAction.KeyDown);
	}

	public bool GetKeyUp(KeyCode key)
	{
		return GetKey(key, KeyAction.KeyUp);
	}

	public bool GetKey(KeyCode key, KeyAction GetKey)
	{
		KeyValue keyValue = alternatives[(int)key];
		if (keyValue == null)
		{
			return ParseAction(GetKey, key);
		}
		KeyCode[] keyCodeAlt = keyValue.keyCodeAlt;
		for (int i = 0; i < keyCodeAlt.Length; i++)
		{
			if (ParseAction(GetKey, keyCodeAlt[i]) && (!bs.splitScreen || (i % 2 == 0 && !secondPlayer) || (i % 2 == 1 && secondPlayer)))
			{
				return true;
			}
		}
		return false;
	}

	private bool ParseAction(KeyAction a, KeyCode key)
	{
		if (bs.android)
		{
			if (dict != null && dict.TryGetValue(key, out AndroidHud.KeyHudBool value))
			{
				AndroidHud.KeyHudBool keyHudBool = value;
				if (keyHudBool.down && a == KeyAction.KeyDown)
				{
					return true;
				}
				if (keyHudBool.up && a == KeyAction.KeyUp)
				{
					return true;
				}
				if (keyHudBool.hold && a == KeyAction.Key)
				{
					return true;
				}
			}
			if (key >= KeyCode.Mouse0 && key <= KeyCode.Mouse6)
			{
				return false;
			}
		}
		bool result;
		switch (a)
		{
		case KeyAction.Key:
			result = InputGetKey(key);
			break;
		case KeyAction.KeyDown:
			result = Input.GetKeyDown(key);
			break;
		default:
			result = Input.GetKeyUp(key);
			break;
		}
		return result;
	}

	private static bool InputGetKey(KeyCode key)
	{
		if (!enableKeys)
		{
			return false;
		}
		Axis axis = Buttons[(int)key];
		if (axis != null)
		{
			float axis2 = Input.GetAxis(axis.s);
			if ((axis2 < axis.min && axis.min < 0f) || (axis2 > axis.min && axis.min > 0f))
			{
				return true;
			}
		}
		return Input.GetKey(key);
	}
}
