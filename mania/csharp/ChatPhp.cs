using UnityEngine;

public class ChatPhp
{
	private Vector2 chatScroll = new Vector2(0f, 1000f);

	public string mapChat;

	private string mapChatInput = string.Empty;

	public string def = "What you think about this map?";

	public string room = "none";

	private float sendTime = float.MinValue;

	public void DrawChat()
	{
		GUISkin skin = bs._Loader.skin;
		Loader loader = bs._Loader;
		if (mapChat == null)
		{
			mapChat = GuiClasses.Tr(def) + "\n";
			bs.Download(bs.mainSite + "chat/" + room + ".txt", delegate(string s, bool b)
			{
				if (b)
				{
					mapChat += s;
				}
				chatScroll = new Vector2(0f, 10000f);
			}, false);
		}
		skin.label.alignment = TextAnchor.UpperLeft;
		chatScroll = GUILayout.BeginScrollView(chatScroll, bs.win.editorSkin.box);
		skin.textField.wordWrap = true;
		skin.label.wordWrap = true;
		GUILayout.Label(mapChat, GUILayout.ExpandHeight(expand: true));
		GUILayout.EndScrollView();
		GUILayout.BeginHorizontal();
		if (Event.current.keyCode == KeyCode.Return && Event.current.isKey)
		{
			Event.current.Use();
		}
		mapChatInput = GUILayout.TextField(mapChatInput, 200);
		if (loader.Button("Send", expandWidth: false) && Time.realtimeSinceStartup - sendTime > 5f && !string.IsNullOrEmpty(mapChatInput.Trim()))
		{
			string text = loader.playerNamePrefixed + ": " + mapChatInput.Trim().Replace(":", "-");
			bs.Download(bs.mainSite + "scripts/chatSend.php", null, true, "map", room, "send", text);
			mapChat = mapChat + "\n" + text;
			sendTime = Time.realtimeSinceStartup;
			mapChatInput = string.Empty;
			chatScroll = new Vector2(0f, 10000f);
		}
		GUILayout.EndHorizontal();
	}
}
