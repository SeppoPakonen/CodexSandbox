using UnityEngine;

public class SampleGUI : MonoBehaviour
{
	public GUISkin skinBlackGloss;

	private Rect blackWinRect = new Rect(10f, 10f, 760f, 600f);

	private string blackTextField = "Sample Text Field";

	private string blackTextArea = "Sample Text Area\nSample Text Area\nSample Text Area";

	private bool blackButtonToggle;

	private bool blackButtonToggleLeft;

	private bool blackButtonToggleMid;

	private bool blackButtonToggleRight;

	private bool blackToggleRadio;

	private bool blackToggleChkBox;

	private float blackHorizontalSlide = 0.5f;

	private float blackVerticalSlide = 0.5f;

	private Vector2 blackScroll = Vector2.zero;

	private void Start()
	{
	}

	private void Update()
	{
	}

	private void OnGUI()
	{
		GUI.skin = skinBlackGloss;
		blackWinRect = GUI.Window(100000, blackWinRect, winBlack, GUIContent.none, "Window");
	}

	private void winBlack(int id)
	{
		GUI.DragWindow(new Rect(0f, 0f, 760f, 40f));
		GUILayout.BeginArea(new Rect(20f, 20f, 720f, 560f));
		GUILayout.BeginHorizontal();
		GUILayout.BeginVertical();
		GUILayout.Label("Sample Window", GUILayout.Width(180f), GUILayout.Height(24f));
		GUILayout.Label("Sample Label First Line\nSample Label Second Line\nSample Label Third Line", GUILayout.Width(180f));
		GUILayout.Space(8f);
		blackTextField = GUILayout.TextField(blackTextField, GUILayout.Width(240f), GUILayout.Height(32f));
		GUILayout.Space(8f);
		blackTextArea = GUILayout.TextArea(blackTextArea, GUILayout.Width(350f), GUILayout.Height(160f));
		GUILayout.Space(8f);
		GUILayout.Label("Sample Button", GUILayout.Width(180f));
		GUILayout.Space(8f);
		GUILayout.BeginHorizontal();
		GUILayout.Button("Single", GUILayout.Width(100f), GUILayout.Height(32f));
		GUILayout.Space(8f);
		GUILayout.Button("Left", "ButtonLeft", GUILayout.Width(80f), GUILayout.Height(32f));
		GUILayout.Button("Middle", "ButtonMid", GUILayout.Width(80f), GUILayout.Height(32f));
		GUILayout.Button("Right", "ButtonRight", GUILayout.Width(80f), GUILayout.Height(32f));
		GUILayout.EndHorizontal();
		GUILayout.Space(8f);
		GUILayout.Label("Sample Toggle Button", GUILayout.Width(180f));
		GUILayout.Space(8f);
		GUILayout.BeginHorizontal();
		blackButtonToggle = GUILayout.Toggle(blackButtonToggle, "Single", "ButtonToggle", GUILayout.Width(100f), GUILayout.Height(32f));
		GUILayout.Space(8f);
		blackButtonToggleLeft = GUILayout.Toggle(blackButtonToggleLeft, "Left", "ButtonToggleLeft", GUILayout.Width(80f), GUILayout.Height(32f));
		blackButtonToggleMid = GUILayout.Toggle(blackButtonToggleMid, "Middle", "ButtonToggleMid", GUILayout.Width(80f), GUILayout.Height(32f));
		blackButtonToggleRight = GUILayout.Toggle(blackButtonToggleRight, "Right", "ButtonToggleRight", GUILayout.Width(80f), GUILayout.Height(32f));
		GUILayout.EndHorizontal();
		GUILayout.Space(8f);
		blackToggleChkBox = GUILayout.Toggle(blackToggleChkBox, "Sample Toggle Checkbox Style", GUILayout.Width(240f), GUILayout.Height(28f));
		blackToggleRadio = GUILayout.Toggle(blackToggleRadio, "Sample Toggle Radio Style", "ToggleRadio", GUILayout.Width(240f), GUILayout.Height(28f));
		GUILayout.EndVertical();
		GUILayout.BeginVertical();
		GUILayout.Space(20f);
		GUILayout.EndVertical();
		GUILayout.BeginVertical();
		GUILayout.Space(30f);
		GUILayout.Box("Sample Box Area\nSample Box Area\nSample Box Area", GUILayout.Width(350f), GUILayout.Height(120f));
		GUILayout.Space(8f);
		GUILayout.Label("Sample Horizontal Slider", GUILayout.Width(180f));
		GUILayout.Space(8f);
		blackHorizontalSlide = GUILayout.HorizontalSlider(blackHorizontalSlide, 0f, 1f, GUILayout.Width(350f), GUILayout.Height(10f));
		GUILayout.Space(8f);
		GUILayout.Label("Sample Vertical Slider", GUILayout.Width(180f));
		GUILayout.Space(8f);
		GUILayout.BeginHorizontal();
		GUILayout.BeginVertical(GUILayout.Width(10f));
		GUILayout.Space(10f);
		GUILayout.EndVertical();
		GUILayout.BeginVertical(GUILayout.Width(300f));
		blackVerticalSlide = GUILayout.VerticalSlider(blackVerticalSlide, 0f, 1f, GUILayout.Width(10f), GUILayout.Height(120f));
		GUILayout.EndVertical();
		GUILayout.EndHorizontal();
		GUILayout.Space(24f);
		blackScroll = GUILayout.BeginScrollView(blackScroll, true, true, GUILayout.Width(320f), GUILayout.Height(180f));
		GUILayout.Label("Sample Scroll View Area...", GUILayout.Width(420f));
		GUILayout.Label("Sample Scroll View Area...");
		GUILayout.Label("Sample Scroll View Area...");
		GUILayout.Label("Sample Scroll View Area...");
		GUILayout.Label("Sample Scroll View Area...");
		GUILayout.Label("Sample Scroll View Area...");
		GUILayout.Label("Sample Scroll View Area...");
		GUILayout.Label("Sample Scroll View Area...");
		GUILayout.Label("Sample Scroll View Area...");
		GUILayout.Label("Sample Scroll View Area...");
		GUILayout.Label("Sample Scroll View Area...");
		GUILayout.Label("Sample Scroll View Area...");
		GUILayout.Label("Sample Scroll View Area...");
		GUILayout.Label("Sample Scroll View Area...");
		GUILayout.Label("Sample Scroll View Area...");
		GUILayout.Label("Sample Scroll View Area...");
		GUILayout.EndScrollView();
		GUILayout.EndVertical();
		GUILayout.EndHorizontal();
		GUILayout.EndArea();
	}
}
