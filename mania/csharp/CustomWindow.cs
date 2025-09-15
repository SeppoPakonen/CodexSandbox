using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CustomWindow : GuiClasses
{
	public Texture2D locked;

	internal float _width;

	internal float _height;

	public GUISkin skinDefault;

	public GUISkin editorSkin;

	public new GUISkin skin;

	internal GUIStyle style;

	public int sizeX;

	public int sizeY;

	private string Window;

	private MonoBehaviour target;

	public string windowTitle;

	public Texture2D windowTexture;

	public Dock dock = Dock.Center;

	internal Action act;

	private Action act2;

	private Dictionary<Action, Action> backs = new Dictionary<Action, Action>();

	public new Texture2D splitScreen;

	public Texture2D[] medals;

	public Texture2D medalsCnt;

	public Texture2D reputation;

	public Texture2D score;

	private float offsetAnim = -1600f;

	public float speed = 0.1f;

	private float[] anim = new float[100];

	private int cur;

	private int curAnim;

	public static bool backSpaceDown;

	public static int buttonId;

	public static int buttonCount;

	public static int curButton = -1;

	private float WindowScale = 1f;

	internal Vector2 offset2;

	internal Vector2 scale;

	internal bool addflexibleSpace = true;

	internal bool showBackButton = true;

	public string tooltip;

	private Rect windowRect;

	private static float originalWidth = 950f;

	private static float originalHeight = 450f;

	public static Vector3 guiscale;

	internal float mouseDrag;

	public bool WindowHit
	{
		get
		{
			if (!base.enabled)
			{
				return false;
			}
			Vector3 mousePosition = Input.mousePosition;
			return windowRect.Contains(new Vector3(mousePosition.x / guiscale.x, mousePosition.y / guiscale.y));
		}
	}

	public override void Awake()
	{
		base.enabled = false;
		base.Awake();
	}

	public override void OnEditorGui()
	{
	}

	public void OnGUI()
	{
		if (Event.current.isMouse && Input.GetMouseButtonUp(0) && mouseDrag > 0.0048828125f)
		{
			Event.current.Use();
		}
		buttonCount = buttonId;
		GuiClasses.cnt = (buttonId = 0);
		GUI.depth = 0;
		scale = GUIMatrix(WindowScale);
		cur = 0;
		float num = _width = (float)Screen.width / scale.x;
		float num2 = _height = (float)Screen.height / scale.y;
		if (bs._Loader.mapName == "level" || Application.loadedLevelName == "level")
		{
			GUI.skin = editorSkin;
		}
		else
		{
			GUI.skin = skinDefault;
		}
		skin = GUI.skin;
		if (Window == null && act2 == null)
		{
			base.enabled = false;
		}
		else
		{
			Vector3 zero = Vector3.zero;
			Vector3 b = new Vector3(Mathf.Min(num, sizeX), Mathf.Min(num2, sizeY)) / 2f;
			zero = ((dock == Dock.Right) ? new Vector3(num - b.x, num2 / 2f) : ((dock == Dock.Left) ? new Vector3(b.x, num2 / 2f) : ((dock != Dock.Down) ? (new Vector3(num, num2) / 2f) : new Vector3(num / 2f, num2 - b.y))));
			Vector3 vector = zero - b;
			Vector3 vector2 = zero + b;
			skin.window.fontSize = 15;
			windowRect = Rect.MinMaxRect(vector.x + offsetAnim * num + offset2.x, vector.y + offset2.y, vector2.x + offsetAnim * num, vector2.y);
			GUILayout.BeginArea(windowRect, GUIContent(windowTitle, windowTexture), style);
			if (act2 != null)
			{
				act2.Invoke();
				if (act2 == null)
				{
					return;
				}
				if (backs.ContainsKey(act2) && backs[act2] != null && showBackButton)
				{
					if (addflexibleSpace)
					{
						GUILayout.FlexibleSpace();
					}
					if (BackButtonLeft())
					{
						Back();
					}
				}
			}
			else
			{
				target.SendMessage(Window);
			}
			GUILayout.EndArea();
		}
		if (bs._Loader.levelEditor == null)
		{
			DrawToolTip();
		}
		if (Event.current.type == EventType.Repaint)
		{
			tooltip = GUI.tooltip;
		}
	}

	public void Back()
	{
		Action key = act2;
		bs.win.CloseWindow();
		if (backs.ContainsKey(key))
		{
			bs.win.ShowWindow(backs[key]);
		}
	}

	public void DrawToolTip()
	{
		GUI.Label(new Rect(0f, bs.win._height - 15f, bs.win._width, 50f), tooltip);
	}

	public static Vector3 GUIMatrix(float sc = 1f)
	{
		float num = (float)Screen.height / (float)Screen.width / (originalHeight / originalWidth);
		guiscale = ((!bs._Loader.scaleButtons || Screen.orientation == ScreenOrientation.AutoRotation) ? Vector3.one : (new Vector3((float)Screen.width / originalWidth * num, (float)Screen.height / originalHeight, 1f) * sc));
		GUI.matrix = Matrix4x4.TRS(Vector3.zero, Quaternion.identity, guiscale);
		return guiscale;
	}

	public void Update()
	{
		if (Input.GetMouseButtonDown(0) || Input.GetMouseButtonDown(1) || Input.GetMouseButtonDown(2))
		{
			mouseDrag = 0f;
		}
		else if (Input.GetMouseButton(0) || Input.GetMouseButton(1) || Input.GetMouseButton(2))
		{
			mouseDrag += new Vector2(Input.GetAxis("Mouse X") / (float)Screen.width, Input.GetAxis("Mouse Y") / (float)Screen.height).magnitude;
		}
		KeysForGui();
		if (curAnim < 100)
		{
			if (anim[curAnim] > 0f)
			{
				anim[curAnim] -= bs._Loader.deltaTime * 1500f;
				return;
			}
			anim[curAnim] = 0f;
			curAnim++;
		}
	}

	private void KeysForGui()
	{
		if (!bs.setting.useKeysForGui)
		{
			return;
		}
		backSpaceDown = bs.input.GetKeyDown(KeyCode.Backspace);
		if (!Input.GetMouseButtonDown(0))
		{
			if (bs.input.GetKeyDown(KeyCode.S))
			{
				curButton = bs.Mod(curButton + 1, buttonCount);
			}
			if (bs.input.GetKeyDown(KeyCode.W))
			{
				curButton = bs.Mod(curButton - 1, buttonCount);
			}
		}
	}

	public void CloseWindow()
	{
		Window = null;
		act = (act2 = null);
	}

	public void LabelAnim(string s, int fontSize = 16, bool center = false)
	{
		skin.label.wordWrap = false;
		skin.label.fontSize = fontSize;
		skin.label.alignment = (center ? TextAnchor.UpperCenter : TextAnchor.UpperLeft);
		GUILayout.BeginHorizontal();
		GUILayout.Space(anim[cur]);
		GUILayout.Label(GUIContent(GuiClasses.Tr(s)));
		GUILayout.EndHorizontal();
		cur++;
	}

	public void Setup(int x = 400, int y = 300, string tittle = "", Dock dock = Dock.Center, Texture2D txt = null, GUIStyle st = null, float windowscale = 1f)
	{
		showBackButton = true;
		addflexibleSpace = true;
		offset2 = Vector2.zero;
		WindowScale = windowscale;
		skin.window.contentOffset = new Vector2(0f, -34f);
		style = ((st != null) ? st : skin.window);
		sizeX = x;
		sizeY = y;
		windowTitle = GuiClasses.Tr(tittle);
		windowTexture = txt;
		this.dock = dock;
	}

	public IEnumerator ShowWindow2(Action func)
	{
		ShowWindow(func);
		while (act != null)
		{
			yield return null;
		}
	}

	public void ShowWindow(Action func, Action back = null, bool skip = false)
	{
		if (!((MulticastDelegate)(object)act == (MulticastDelegate)(object)func))
		{
			act = func;
			MonoBehaviour.print("Show Window: " + ((Delegate)(object)func).Method.Name);
			bs._Loader.OnWindowShow(func);
			curButton = -1;
			Screen.lockCursor = false;
			GuiClasses.scroll = Vector2.zero;
			for (int i = 0; i < anim.Length; i++)
			{
				anim[i] = 500f;
			}
			curAnim = 0;
			StopAllCoroutines();
			if (skip || bs.setting.debug || (bool)bs._Loader.levelEditor)
			{
				offsetAnim = 0f;
				SetWindow(func, back);
			}
			else
			{
				StartCoroutine(cor(func, back));
			}
		}
	}

	public IEnumerator cor(Action func, Action back)
	{
		while (offsetAnim > -2f)
		{
			offsetAnim -= bs._Loader.deltaTime * speed;
			yield return null;
		}
		SetWindow(func, back);
		offsetAnim = 2f;
		while (offsetAnim > 0f)
		{
			offsetAnim -= bs._Loader.deltaTime * speed;
			yield return null;
		}
		offsetAnim = 0f;
	}

	private void SetWindow(Action func, Action back)
	{
		Setup(400, 300, string.Empty, Dock.Center, null, null, 1f);
		Screen.lockCursor = false;
		base.enabled = true;
		act = (act2 = func);
		if (back != null)
		{
			backs[func] = back;
		}
	}
}
