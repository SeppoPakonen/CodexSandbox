using UnityEngine;

public class MenuGUI : MonoBehaviour
{
	public Texture2D logo;

	private void Start()
	{
	}

	private void Update()
	{
		Screen.lockCursor = false;
	}

	private void OnGUI()
	{
		GUI.skin.button.fontSize = 20;
		GUI.DrawTexture(new Rect(Screen.width / 2 - logo.width / 2, 10f, logo.width, logo.height), logo);
		if (GUI.Button(new Rect(Screen.width / 2 - 400, 300f, 300f, 50f), "Demo 1"))
		{
			Application.LoadLevel("Demo1");
		}
		if (GUI.Button(new Rect(Screen.width / 2 - 400, 360f, 300f, 50f), "Demo 2"))
		{
			Application.LoadLevel("Demo2");
		}
		if (GUI.Button(new Rect(Screen.width / 2 - 400, 420f, 300f, 50f), "Demo 3"))
		{
			Application.LoadLevel("Demo3");
		}
		if (GUI.Button(new Rect(Screen.width / 2 - 400, 480f, 300f, 50f), "Get this project"))
		{
			Application.OpenURL("https://www.assetstore.unity3d.com/#/content/7676");
		}
		GUI.skin.label.fontSize = 14;
		GUI.Label(new Rect(20f, Screen.height - 60, 300f, 50f), "Weapon System 3.0 By Rachan Neamprasert www.hardworkerstudio.com");
	}
}
