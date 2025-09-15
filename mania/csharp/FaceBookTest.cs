using LitJson;
using System.Collections;
using UnityEngine;

[ExecuteInEditMode]
public class FaceBookTest : MonoBehaviour
{
	public enum tipoGui
	{
		step1,
		step2,
		step3,
		step4
	}

	public GUIStyle foto;

	public GameObject obj;

	public GUIText text1;

	public GUIText text2;

	public GUIText text3;

	public GUIText text4;

	public GUIText text5;

	public GUIText text6;

	public tipoGui tipo;

	private JsonData friendsinfo;

	private float space = 8f;

	private void Start()
	{
		Facebook.onLogin += delegate(bool result, JsonData reply)
		{
			if (result)
			{
				tipo = tipoGui.step3;
				string info = Facebook.getInfo("/me/picture?type=normal");
				StartCoroutine(getImage(info));
			}
		};
		Facebook.onGraphRequest += delegate(bool result, JsonData reply)
		{
			if (result)
			{
				text1.text = reply["name"].ToString();
				text2.text = reply["username"].ToString();
				text3.text = reply["id"].ToString();
				text4.text = reply["religion"].ToString();
			}
		};
		Facebook.onGetLoginStatusdos += delegate(bool result, string status)
		{
			if (result)
			{
				text6.text = "Status+: " + status;
			}
			else
			{
				text6.text = "Status-: ";
			}
		};
	}

	public IEnumerator getImage(string _imageURL)
	{
		WWW www = new WWW(_imageURL);
		yield return www;
		foto.normal.background = www.texture;
	}

	public IEnumerator getInfoFriends(string texto)
	{
		WWW www = new WWW(texto);
		yield return www;
		friendsinfo = JsonMapper.ToObject(www.text);
		buildfriends();
	}

	private void buildfriends()
	{
		int num = -14;
		int count = friendsinfo[0].Count;
		text4.text = count + " friends found";
		text5.text = "Keyup / Key down to move camera";
		int num2 = 0;
		bool flag = true;
		for (num2 = 0; num2 <= count; num2++)
		{
			if (num2 > count / 2 && flag)
			{
				num = -3;
				space = 8f;
				flag = false;
			}
			GameObject gameObject = (GameObject)Object.Instantiate((Object)this.obj, new Vector3(num, space, 0f), this.obj.transform.rotation);
			gameObject.transform.parent = base.transform;
			gameObject.transform.name = friendsinfo[0][num2]["id"].ToString();
			gameObject.GetComponent<MiObjeto>().txtnombre = friendsinfo[0][num2]["name"].ToString();
			gameObject.GetComponent<MiObjeto>().txtimagen = friendsinfo[0][num2]["picture"][0][0].ToString();
			try
			{
				if (friendsinfo[0][num2]["gender"].ToString() == "male")
				{
					gameObject.GetComponent<MiObjeto>().txtgenero = "m";
				}
				else
				{
					gameObject.GetComponent<MiObjeto>().txtgenero = "f";
				}
			}
			catch
			{
			}
			space -= 2f;
		}
	}

	private void OnGUI()
	{
		switch (tipo)
		{
		case tipoGui.step2:
			break;
		case tipoGui.step1:
			if (GUI.Button(new Rect(10f, 10f, Screen.width - 20, Screen.height - 20), "Enter"))
			{
				Facebook.login("email,publish_actions,publish_stream");
				tipo = tipoGui.step2;
			}
			break;
		case tipoGui.step3:
			GUI.Label(new Rect((float)Screen.width * 0.82f, (float)Screen.height * 0.12f, 125f, 125f), string.Empty, foto);
			if (GUI.Button(new Rect(Screen.width - 190, Screen.height / 2 - 100, 170f, 30f), "User Information"))
			{
				Facebook.graphRequest("/me");
			}
			if (GUI.Button(new Rect(Screen.width - 190, Screen.height / 2 - 60, 170f, 30f), "Get Status"))
			{
				Facebook.getLoginStatusdos();
			}
			if (GUI.Button(new Rect(770f, 385f, 170f, 30f), "Send invitations"))
			{
				Facebook.uiAppRequest("Title Request", "I invite you to try this application");
			}
			if (GUI.Button(new Rect(770f, 420f, 170f, 30f), "Post with Pop Up"))
			{
				Facebook.uiFeedRequest("http://www.google.com", "http://www.google.com/logos/2012/tsiolkovsky12-hp.jpg", "Resaltado - ..........", "Facebook desde la Web", "Descripcion..... ... ..... .... ... ....... .");
			}
			if (GUI.Button(new Rect(770f, 455f, 170f, 30f), "Post without Pop Up"))
			{
				Facebook.postear("me", "Post Message", "Name...", "Des", "http://www.google.com/logos/2012/tsiolkovsky12-hp.jpg", "Facebook Web Cap", "http://www.google.com");
			}
			if (GUI.Button(new Rect(770f, 490f, 170f, 30f), "Post Video"))
			{
				Facebook.postearv("me", "Post Message", "Name...", "Des", "http://img.youtube.com/vi/9bZkp7q19f0/0.jpg", "Facebook Web Cap", "http://www.youtube.com/e/9bZkp7q19f0", "http://www.youtube.com/watch?v=9bZkp7q19f0");
			}
			if (GUI.Button(new Rect(770f, 525f, 170f, 30f), "Friends"))
			{
				StartCoroutine(getInfoFriends(Facebook.getInfo("/me/friends?fields=id,name,gender,picture")));
			}
			if (GUI.Button(new Rect(770f, 559f, 170f, 30f), "Post Photo"))
			{
				Facebook.photo("Message...", "http://a.lyecorp.com/marcador.jpg");
			}
			break;
		}
	}
}
