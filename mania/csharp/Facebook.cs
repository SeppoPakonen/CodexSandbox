using LitJson;
using UnityEngine;

public class Facebook : MonoBehaviour
{
	public delegate void FacebookEventDelegate(bool resultado, JsonData respuesta);

	public delegate void FacebookEventDelegatedos(bool resultado, string status);

	public TextAsset FBjavascriptfile;

	public TextAsset jsJson;

	public string appID;

	public static string Token;

	public static event FacebookEventDelegate onLogin;

	public static event FacebookEventDelegate onLogout;

	public static event FacebookEventDelegate onGetLoginStatus;

	public static event FacebookEventDelegate onPostFeed;

	public static event FacebookEventDelegate onAppRequest;

	public static event FacebookEventDelegate onFeedRequest;

	public static event FacebookEventDelegate onGraphRequest;

	public static event FacebookEventDelegatedos onGetLoginStatusdos;

	public static void login(string permisos)
	{
		bs.ExternalCall("login", permisos);
	}

	public static void logout()
	{
		bs.ExternalCall("logout");
	}

	public static void getLoginStatus()
	{
		bs.ExternalCall("getLoginStatus");
	}

	public static void getLoginStatusdos()
	{
		bs.ExternalCall("getLoginStatusdos");
	}

	public static string getInfo(string text)
	{
		return "https://graph.facebook.com" + text + "&access_token=" + Token;
	}

	public static void postear(string to, string message, string name, string description, string picture, string caption, string link)
	{
		string text = "/" + to + "/feed";
		bs.ExternalCall("postear", text, message, name, description, picture, caption, link);
	}

	public static void postearv(string to, string message, string name, string description, string picture, string caption, string source, string link)
	{
		string text = "/" + to + "/feed";
		bs.ExternalCall("postearv", text, message, name, description, picture, caption, source, link);
	}

	public static void photo(string message, string link)
	{
		bs.ExternalCall("photo", message, link);
	}

	public static void userGraphRequest(string _user, string _request)
	{
		graphRequest(_user + _request);
	}

	public static void graphRequest(string _request)
	{
		bs.ExternalCall("graphRequest", _request);
	}

	public static void uiAppRequest(string title, string message)
	{
		bs.ExternalCall("uiAppRequest", title, message);
	}

	public static void uiFeedRequest(string link, string picture, string name, string caption, string description)
	{
		bs.ExternalCall("uiFeedRequest", link, picture, name, caption, description);
	}

	public static void postFeed(string message, string picture, string link)
	{
		bs.ExternalCall("postFeed", message, picture, link);
	}

	private void defineToken(string token)
	{
		Token = token;
	}

	private void onLoginExitoso(string message)
	{
		if (Facebook.onLogin != null)
		{
			Facebook.onLogin(resultado: true, JsonMapper.ToObject(message));
		}
	}

	private void onLoginFallido(string message)
	{
		if (Facebook.onLogin != null)
		{
			Facebook.onLogin(resultado: false, JsonMapper.ToObject(message));
		}
	}

	private void onLogoutExitoso(string message)
	{
		if (Facebook.onLogout != null)
		{
			Facebook.onLogout(resultado: true, JsonMapper.ToObject(message));
		}
	}

	private void onLogoutFallido(string message)
	{
		if (Facebook.onLogout != null)
		{
			Facebook.onLogout(resultado: false, JsonMapper.ToObject(message));
		}
	}

	private void onGetLoginStatusExitoso(string message)
	{
		if (Facebook.onGetLoginStatus != null)
		{
			Facebook.onGetLoginStatus(resultado: true, JsonMapper.ToObject(message));
		}
	}

	private void onGetLoginStatusFallido(string message)
	{
		if (Facebook.onGetLoginStatus != null)
		{
			Facebook.onGetLoginStatus(resultado: false, JsonMapper.ToObject(message));
		}
	}

	private void onGetLoginStatusconnected(string message)
	{
		if (Facebook.onGetLoginStatusdos != null)
		{
			Facebook.onGetLoginStatusdos(resultado: true, "connected");
		}
	}

	private void onGetLoginStatusnot_authorized(string message)
	{
		if (Facebook.onGetLoginStatusdos != null)
		{
			Facebook.onGetLoginStatusdos(resultado: true, "not_authorized");
		}
	}

	private void onGetLoginStatusnotlogged(string message)
	{
		if (Facebook.onGetLoginStatusdos != null)
		{
			Facebook.onGetLoginStatusdos(resultado: true, "notlogged");
		}
	}

	private void onPostFeedExitoso(string message)
	{
		if (Facebook.onPostFeed != null)
		{
			Facebook.onPostFeed(resultado: true, JsonMapper.ToObject(message));
		}
	}

	private void onPostFeedFallido(string message)
	{
		if (Facebook.onFeedRequest != null)
		{
			Facebook.onFeedRequest(resultado: false, JsonMapper.ToObject(message));
		}
	}

	private void onUiAppRequestExitoso(string message)
	{
		if (Facebook.onAppRequest != null)
		{
			Facebook.onAppRequest(resultado: true, JsonMapper.ToObject(message));
		}
	}

	private void onUiAppRequestFallido(string message)
	{
		if (Facebook.onAppRequest != null)
		{
			Facebook.onAppRequest(resultado: false, JsonMapper.ToObject(message));
		}
	}

	private void onUiFeedRequestExitoso(string message)
	{
		if (Facebook.onFeedRequest != null)
		{
			Facebook.onFeedRequest(resultado: true, JsonMapper.ToObject(message));
		}
	}

	private void onUiFeedRequestFallido(string message)
	{
		if (Facebook.onFeedRequest != null)
		{
			Facebook.onFeedRequest(resultado: false, JsonMapper.ToObject(message));
		}
	}

	private void onGraphRequestExitoso(string message)
	{
		if (Facebook.onGraphRequest != null)
		{
			Facebook.onGraphRequest(resultado: true, JsonMapper.ToObject(message));
		}
	}

	private void onGraphRequestFallido(string message)
	{
		if (Facebook.onGraphRequest != null)
		{
			Facebook.onGraphRequest(resultado: false, JsonMapper.ToObject(message));
		}
	}

	private void Start()
	{
		Object.DontDestroyOnLoad(base.gameObject);
	}
}
