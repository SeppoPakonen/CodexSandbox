using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class AndroidHud : bs
{
	public class KeyHudBool
	{
		public Touch? hitTest;

		public bool hold;

		public Archor guitext;

		public KeyCode key;

		public bool down;

		public bool up;

		public bool secondPlayer;

		public Color hudColor
		{
			get
			{
				return guitext.color;
			}
			set
			{
				guitext.color = value;
			}
		}

		public string prefix => bs._Loader.playerName + guitext.name + secondPlayer;

		public float scale
		{
			get
			{
				return Base.PlayerPrefsGetFloat(prefix + "scale2", 1f);
			}
			set
			{
				Base.PlayerPrefsSetFloat(prefix + "scale2", value);
			}
		}

		public float posx
		{
			get
			{
				return Base.PlayerPrefsGetFloat(prefix + "posx2", 0f);
			}
			set
			{
				Base.PlayerPrefsSetFloat(prefix + "posx2", value);
			}
		}

		public float posy
		{
			get
			{
				return Base.PlayerPrefsGetFloat(prefix + "posy2", 0f);
			}
			set
			{
				Base.PlayerPrefsSetFloat(prefix + "posy2", value);
			}
		}

		public KeyHudBool LoadPos()
		{
			if (posx != 0f && posy != 0f)
			{
				guitext.pos = new Vector3(posx, posy, 0f);
			}
			if (scale != 1f)
			{
				UpdateScale();
			}
			return this;
		}

		public void UpdateScale()
		{
			guitext.scale = scale;
		}
	}

	public Player pl;

	public Archor pad;

	public Archor flashBack;

	public Archor brake;

	public Archor forward;

	public Archor padTouch;

	public Archor left;

	public Archor right;

	public Archor nitro;

	private Dictionary<KeyCode, KeyHudBool> m_dict;

	internal Vector2 mouse;

	private KeyHudBool last;

	public Dictionary<KeyCode, KeyHudBool> dict
	{
		get
		{
			if (m_dict == null)
			{
				InitDict();
			}
			return m_dict;
		}
	}

	public static Touch[] touches => Input.touches;

	private void keyhudAdd(KeyHudBool keyHudBool)
	{
		keyHudBool.LoadPos();
		dict.Add(keyHudBool.key, keyHudBool);
	}

	public void InitDict()
	{
		m_dict = new Dictionary<KeyCode, KeyHudBool>();
		keyhudAdd(new KeyHudBool
		{
			key = KeyCode.W,
			guitext = forward
		});
		keyhudAdd(new KeyHudBool
		{
			key = KeyCode.S,
			guitext = brake
		});
		keyhudAdd(new KeyHudBool
		{
			key = KeyCode.A,
			guitext = left
		});
		keyhudAdd(new KeyHudBool
		{
			key = KeyCode.D,
			guitext = right
		});
		keyhudAdd(new KeyHudBool
		{
			key = KeyCode.Backspace,
			guitext = flashBack
		});
		keyhudAdd(new KeyHudBool
		{
			key = KeyCode.Clear,
			guitext = pad
		});
		keyhudAdd(new KeyHudBool
		{
			key = KeyCode.LeftShift,
			guitext = nitro
		});
		keyhudAdd(new KeyHudBool
		{
			key = KeyCode.UpArrow,
			guitext = forward,
			secondPlayer = true
		});
		keyhudAdd(new KeyHudBool
		{
			key = KeyCode.DownArrow,
			guitext = brake,
			secondPlayer = true
		});
		keyhudAdd(new KeyHudBool
		{
			key = KeyCode.LeftArrow,
			guitext = left,
			secondPlayer = true
		});
		keyhudAdd(new KeyHudBool
		{
			key = KeyCode.RightArrow,
			guitext = right,
			secondPlayer = true
		});
		keyhudAdd(new KeyHudBool
		{
			key = KeyCode.Insert,
			guitext = flashBack,
			secondPlayer = true
		});
		keyhudAdd(new KeyHudBool
		{
			key = KeyCode.Caret,
			guitext = pad,
			secondPlayer = true
		});
		keyhudAdd(new KeyHudBool
		{
			key = KeyCode.RightShift,
			guitext = nitro,
			secondPlayer = true
		});
	}

	public void Update()
	{
		Archor archor = padTouch;
		bool enabled = bs._Loader.controls == Contr.mouse;
		pad.enabled = enabled;
		archor.enabled = enabled;
		bool flag = bs._Loader.controls == Contr.keys;
		Archor archor2 = left;
		enabled = flag;
		right.enabled = enabled;
		archor2.enabled = enabled;
		nitro.enabled = ((float)pl.nitro > 0f);
		mouse = Vector3.Lerp(mouse, Vector3.zero, Time.deltaTime * 10f);
		bool flag2 = false;
		if (bs._Loader.accelometer)
		{
			Vector3 acceleration = Input.acceleration;
			mouse = new Vector2(acceleration.x, 0f) * 2f;
		}
		else if (bs._Loader.enableMouse)
		{
			Touch[] touches = Input.touches;
			foreach (Touch touch in touches)
			{
				Vector3 vector = touch.position;
				Vector3 inversePos = pad.inversePos;
				inversePos.x *= Screen.width;
				inversePos.y *= Screen.height;
				if ((vector.x < (float)Screen.width / 2f && inversePos.x < (float)Screen.width / 2f) || (vector.x > (float)Screen.width / 2f && inversePos.x > (float)Screen.width / 2f))
				{
					Vector3 a2 = inversePos - vector;
					Vector3 size = pad.size;
					Vector3 v = a2 / (size.x / 2f) * (pl.secondPlayer ? 1 : (-1));
					v.x = Mathf.Clamp(v.x, -1f, 1f);
					v.y = Mathf.Clamp(v.y, -1f, 1f);
					if (!bs.splitScreen || (vector.y > (float)Screen.height / 2f && pl.secondPlayer) || (vector.y < (float)Screen.height / 2f && !pl.secondPlayer))
					{
						mouse = v;
						padTouch.screenPos = vector;
						flag2 = true;
					}
				}
			}
			if (!flag2)
			{
				padTouch.transform.position = pad.transform.position;
			}
		}
		if (bs._Game.editControls)
		{
			if (Input.touchCount == 1)
			{
				foreach (KeyHudBool value2 in dict.Values)
				{
					Touch? touch2 = HitTest(value2);
					if (touch2.HasValue)
					{
						last = value2;
						Vector2 position = touch2.Value.position;
						position = ((Component)pl.hud).get_camera().ScreenToViewportPoint(position);
						value2.guitext.inversePos = position;
						value2.posx = position.x;
						value2.posy = position.y;
					}
				}
			}
			else if (last != null)
			{
				last.scale += GetDoubleTouch() * 0.01f;
				last.scale = Mathf.Max(0.7f, last.scale);
				last.UpdateScale();
			}
		}
		if (bs._Game.editControls)
		{
			return;
		}
		if (flag && !bs.splitScreen)
		{
			foreach (KeyHudBool value3 in dict.Values)
			{
				value3.hitTest = null;
			}
			Touch[] touches2 = AndroidHud.touches;
			for (int j = 0; j < touches2.Length; j++)
			{
				Touch value = touches2[j];
				Vector2 pos = value.position;
				pos.x /= Screen.width;
				pos.y /= Screen.height;
				KeyHudBool keyHudBool = (from a in ((IEnumerable<KeyHudBool>)dict.Values).Where((Func<KeyHudBool, bool>)((KeyHudBool a) => a.guitext.enabled))
					orderby Vector2.Distance(pos, a.guitext.pos)
					select a).FirstOrDefault();
				keyHudBool.hitTest = value;
			}
		}
		else
		{
			foreach (KeyHudBool value4 in dict.Values)
			{
				value4.hitTest = HitTest(value4);
			}
		}
		foreach (KeyHudBool value5 in dict.Values)
		{
			TouchPhase touchPhase = (!value5.hitTest.HasValue) ? ((TouchPhase)221) : value5.hitTest.Value.phase;
			value5.hold = (touchPhase == TouchPhase.Stationary || touchPhase == TouchPhase.Moved);
			value5.up = (touchPhase == TouchPhase.Canceled || touchPhase == TouchPhase.Ended);
			value5.down = (touchPhase == TouchPhase.Began);
			if (!bs.splitScreen)
			{
				value5.hudColor = (((!flag2 || !(value5.guitext == pad)) && !value5.hold) ? new Color(0f, 0f, 0f, 0f) : new Color(0.5f, 0.5f, 0.5f, 0.5f));
			}
		}
	}

	private Touch? HitTest(KeyHudBool hud)
	{
		Touch[] touches = Input.touches;
		for (int i = 0; i < touches.Length; i++)
		{
			Touch value = touches[i];
			Vector2 position = value.position;
			if (hud.guitext.HitTest(position))
			{
				return value;
			}
		}
		return null;
	}

	public static float GetDoubleTouch()
	{
		if (touches.Length > 1)
		{
			Vector2 vector = touches[0].position - touches[1].position;
			Vector2 vector2 = touches[0].position - touches[0].deltaPosition - (touches[1].position - touches[1].deltaPosition);
			return vector.magnitude - vector2.magnitude;
		}
		return 0f;
	}
}
