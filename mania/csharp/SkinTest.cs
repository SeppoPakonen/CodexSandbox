using UnityEngine;

public class SkinTest : MonoBehaviour
{
	public struct snNodeArray
	{
		public string itemType;

		public string itemName;

		public snNodeArray(string itemType, string itemName)
		{
			this.itemType = itemType;
			this.itemName = itemName;
		}
	}

	public GUISkin thisMetalGUISkin;

	public GUISkin thisOrangeGUISkin;

	public GUISkin thisAmigaGUISkin;

	private Rect rctWindow1;

	private Rect rctWindow2;

	private Rect rctWindow3;

	private Rect rctWindow4;

	private bool blnToggleState;

	private float fltSliderValue = 0.5f;

	private float fltScrollerValue = 0.5f;

	private Vector2 scrollPosition = Vector2.zero;

	private snNodeArray[] testArray = new snNodeArray[20];

	private void Awake()
	{
		rctWindow1 = new Rect(20f, 20f, 320f, 400f);
		rctWindow2 = new Rect(260f, 30f, 320f, 420f);
		rctWindow3 = new Rect(260f, 30f, 320f, 200f);
		rctWindow4 = new Rect(360f, 20f, 320f, 400f);
		for (int i = 0; i < 19; i++)
		{
			testArray[i].itemType = "node";
			testArray[i].itemName = "Hello" + i;
		}
	}

	private void OnGUI()
	{
		GUI.skin = thisOrangeGUISkin;
		rctWindow1 = GUI.Window(0, rctWindow1, DoMyWindow, "Orange Unity", GUI.skin.GetStyle("window"));
		GUI.skin = thisMetalGUISkin;
		rctWindow2 = GUI.Window(1, rctWindow2, DoMyWindow2, "Metal Vista", GUI.skin.GetStyle("window"));
		rctWindow3 = GUI.Window(2, rctWindow3, DoMyWindow4, "Compound Control - Toggle Listbox", GUI.skin.GetStyle("window"));
		GUI.skin = thisAmigaGUISkin;
		rctWindow4 = GUI.Window(3, rctWindow4, DoMyWindow, "Amiga500", GUI.skin.GetStyle("window"));
	}

	private void gcListItem(string strItemName)
	{
		GUILayout.BeginHorizontal();
		GUILayout.Label(strItemName);
		blnToggleState = GUILayout.Toggle(blnToggleState, string.Empty);
		GUILayout.EndHorizontal();
	}

	private void gcListBox()
	{
		GUILayout.BeginVertical(GUI.skin.GetStyle("box"));
		scrollPosition = GUILayout.BeginScrollView(scrollPosition, GUILayout.Width(160f), GUILayout.Height(130f));
		for (int i = 0; i < 20; i++)
		{
			gcListItem("I'm listItem number " + i);
		}
		GUILayout.EndScrollView();
		GUILayout.EndVertical();
	}

	private void DoMyWindow4(int windowID)
	{
		gcListBox();
		GUI.DragWindow();
	}

	private void DoMyWindow3(int windowID)
	{
		scrollPosition = GUI.BeginScrollView(new Rect(10f, 100f, 200f, 200f), scrollPosition, new Rect(0f, 0f, 220f, 200f));
		GUI.Button(new Rect(0f, 0f, 100f, 20f), "Top-left");
		GUI.Button(new Rect(120f, 0f, 100f, 20f), "Top-right");
		GUI.Button(new Rect(0f, 180f, 100f, 20f), "Bottom-left");
		GUI.Button(new Rect(120f, 180f, 100f, 20f), "Bottom-right");
		GUI.EndScrollView();
		GUI.DragWindow();
	}

	private void DoMyWindow(int windowID)
	{
		GUILayout.BeginVertical();
		GUILayout.Label("Im a Label");
		GUILayout.Space(8f);
		GUILayout.Button("Im a Button");
		GUILayout.TextField("Im a textfield");
		GUILayout.TextArea("Im a textfield\nIm the second line\nIm the third line\nIm the fourth line");
		blnToggleState = GUILayout.Toggle(blnToggleState, "Im a Toggle button");
		GUILayout.EndVertical();
		GUILayout.BeginVertical();
		GUILayout.BeginHorizontal();
		fltSliderValue = GUILayout.HorizontalSlider(fltSliderValue, 0f, 1.1f, GUILayout.Width(128f));
		fltSliderValue = GUILayout.VerticalSlider(fltSliderValue, 0f, 1.1f, GUILayout.Height(50f));
		GUILayout.EndHorizontal();
		GUILayout.BeginHorizontal();
		fltScrollerValue = GUILayout.HorizontalScrollbar(fltScrollerValue, 0.1f, 0f, 1.1f, GUILayout.Width(128f));
		fltScrollerValue = GUILayout.VerticalScrollbar(fltScrollerValue, 0.1f, 0f, 1.1f, GUILayout.Height(90f));
		GUILayout.Box("Im\na\ntest\nBox");
		GUILayout.EndHorizontal();
		GUILayout.EndVertical();
		GUI.DragWindow();
	}

	private void DoMyWindow2(int windowID)
	{
		GUILayout.Label("3D Graphics Settings");
		GUILayout.BeginVertical();
		GUILayout.BeginHorizontal();
		blnToggleState = GUILayout.Toggle(blnToggleState, "Soft Shadows");
		blnToggleState = GUILayout.Toggle(blnToggleState, "Particle Effects");
		GUILayout.EndHorizontal();
		GUILayout.BeginHorizontal();
		blnToggleState = GUILayout.Toggle(blnToggleState, "Enemy Shadows");
		blnToggleState = GUILayout.Toggle(blnToggleState, "Object Glow");
		GUILayout.EndHorizontal();
		GUILayout.EndVertical();
		GUILayout.BeginVertical();
		GUILayout.Button("Im a Button");
		GUILayout.TextField("Im a textfield");
		GUILayout.TextArea("Im a textfield\nIm the second line\nIm the third line\nIm the fourth line");
		blnToggleState = GUILayout.Toggle(blnToggleState, "Im a Toggle button");
		GUILayout.EndVertical();
		GUILayout.BeginVertical();
		GUILayout.BeginHorizontal();
		fltSliderValue = GUILayout.HorizontalSlider(fltSliderValue, 0f, 1.1f, GUILayout.Width(128f));
		fltSliderValue = GUILayout.VerticalSlider(fltSliderValue, 0f, 1.1f, GUILayout.Height(50f));
		GUILayout.EndHorizontal();
		GUILayout.BeginHorizontal();
		fltScrollerValue = GUILayout.HorizontalScrollbar(fltScrollerValue, 0.1f, 0f, 1.1f, GUILayout.Width(128f));
		fltScrollerValue = GUILayout.VerticalScrollbar(fltScrollerValue, 0.1f, 0f, 1.1f, GUILayout.Height(90f));
		GUILayout.Box("Im\na\ntest\nBox");
		GUILayout.EndHorizontal();
		GUILayout.EndVertical();
		GUI.DragWindow();
	}
}
