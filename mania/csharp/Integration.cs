using LitJson;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Text.RegularExpressions;
using UnityEngine;

public class Integration : GuiClasses
{
	internal bool fbLoggedIn;

	internal string userId;

	public int FbFriendsInGame;

	internal int pendingFriends;

	internal string userName = string.Empty;

	private string facebookLink = "http://apps.facebook.com/TrackRacing";

	public bool alternativeOpen;

	internal bool feedPosted;

	internal Site site;

	public bool vkLoggedIn;

	public List<Posts> posts = new List<Posts>();

	public bool loggedIn => fbLoggedIn || vkLoggedIn;

	public bool facebookLinkPressed
	{
		get
		{
			return Base.PlayerPrefsGetBool(bs._Loader.playerName + "facebookLink");
		}
		set
		{
			Base.PlayerPrefsSetBool(bs._Loader.playerName + "facebookLink", value);
		}
	}

	private void Facebook_onLogin(bool resultado, JsonData respuesta)
	{
		if (resultado)
		{
			fbLoggedIn = true;
			MonoBehaviour.print("Facebook_onLogin");
			StartCoroutine(getInfoFriends());
			Facebook.onAppRequest += Facebook_onAppRequest;
			Facebook.onGraphRequest += Facebook_onGraphRequest;
			FacebookGetInfo();
		}
		else
		{
			MonoBehaviour.print("facebook login failed " + respuesta);
		}
	}

	private static void FacebookGetInfo()
	{
		Facebook.graphRequest("/me/?fields=id,name,installed");
	}

	private void Facebook_onGraphRequest(bool result, JsonData reply)
	{
		MonoBehaviour.print("Facebook_onGraphRequest " + reply.ToJson());
		if (string.IsNullOrEmpty(userId) && bs._Loader.loggedIn)
		{
			string value = reply["name"].ToString();
			GuiClasses.dict[bs._Loader.playerName.ToLower()] = value;
			userName = value;
			site = Site.FB;
			userId = reply["id"].ToString();
			bs.Download(bs.mainSite + "scripts/dict.php", delegate
			{
			}, true, "key", "fb" + userId, "value", bs._Loader.playerNamePrefixed);
			MonoBehaviour.print("App Installed " + reply["installed"]);
		}
	}

	private void Facebook_onAppRequest(bool resultado, JsonData respuesta)
	{
		MonoBehaviour.print("Facebook_onAppRequest");
		if (resultado && respuesta["to"] != null)
		{
			pendingFriends += respuesta["to"].Count;
		}
		else
		{
			MonoBehaviour.print("AppRequest failed " + respuesta);
		}
	}

	public IEnumerator getInfoFriends()
	{
		string texto = Facebook.getInfo("/me/friends?fields=id,name,installed");
		WWW www = new WWW(texto);
		yield return www;
		MonoBehaviour.print(www);
		JsonData friendsinfo = JsonMapper.ToObject(www.text);
		bs._Loader.friendCount = friendsinfo[0].Count;
		for (int i = 0; i < (int)bs._Loader.friendCount; i++)
		{
			FriendParse(friendsinfo[0][i]);
		}
	}

	private void FriendParse(JsonData jsonData)
	{
		if (!bool.Parse(jsonData["installed"].ToString()))
		{
			return;
		}
		MonoBehaviour.print("friend found ");
		bs.Download(bs.mainSite + "/dict/fb" + userId + ".txt", delegate(string s, bool b)
		{
			if (b)
			{
				MonoBehaviour.print("friend added " + s);
				AddFriend(s);
				GuiClasses.dict[s] = jsonData["name"].ToString();
			}
		}, false);
		bs._Loader.friends = bs._Loader.friends;
		FbFriendsInGame++;
	}

	public override void Awake()
	{
		if (!Application.isEditor)
		{
			vkLoggedIn = false;
		}
		base.Awake();
	}

	public void Start()
	{
		MonoBehaviour.print("Start Integration");
		string str = (!Application.absoluteURL.Contains("kongregate")) ? "u.getUnity()" : "kongregateUnitySupport.getUnityObject()";
		bs.ExternalEval(str + ".SendMessage('!Loader', 'Url',document.location.toString());");
		bs.ExternalEval(str + ".SendMessage('Integration', 'UrlOdnoklasniki',document.location.toString());");
		if (bs.setting.testVK)
		{
			bs._Loader.Url("http://server.critical-missions.com/tm/web2/kong.html?DO_NOT_SHARE_THIS_LINK=1&kongregate_username=SoulKey&kongregate_user_id=12399663&kongregate_game_auth_token=e27a256515c68fdd165b6520a8f3d3b1d837a1618337fe2fde9638d914ac43a4&kongregate_game_id=182469&kongregate_host=http%3A%2F%2Fwww.kongregate.com&kongregate_game_url=http%3A%2F%2Fwww.kongregate.com%2Fgames%2FSoulKey%2Ftr-online&kongregate_api_host=http%3A%2F%2Fapi.kongregate.com&kongregate_channel_id=1390039898416&kongregate_api_path=http%3A%2F%2Fchat.kongregate.com%2Fflash%2FAPI_AS3_efeff9e3f4f6350255b95aa31ba0ef32.swf&kongregate_ansible_path=chat.kongregate.com%2Fflash%2Fansible_5cae4aad0aa719bffe641383dd1d3178.swf&kongregate_preview=true&kongregate_language=en&preview=true&kongregate_split_treatments=none&kongregate=true&KEEP_THIS_DATA_PRIVATE=1");
		}
		Facebook.onLogin += Facebook_onLogin;
		FacebookLogin();
		StartVk();
		StartCoroutine(StartLoadNews());
	}

	private IEnumerator StartLoadNews()
	{
		if (Application.isWebPlayer)
		{
			yield return new WaitForSeconds(1f);
		}
		WWW w = new WWW("https://graph.facebook.com/trackracingonline/feed?access_token=261172830687941|d5mN7DtR-LQoNlBW9JFBL7gM_h4");
		yield return w;
		if (posts.Count > 0 && !bs.isDebug)
		{
			yield break;
		}
		posts.Clear();
		MonoBehaviour.print(w.url);
		IEnumerable d = JsonMapper.ToObject(w.text)["data"];
		foreach (JsonData a in d)
		{
			string type = a["type"].ToString();
			if (!(a["from"]["name"].ToString() == "Trackracing Online"))
			{
				continue;
			}
			switch (type)
			{
			case "video":
			case "status":
			case "link":
			{
				JsonData jsonData = a["desctiption"] ?? a["message"] ?? a["name"];
				if (jsonData != null)
				{
					Posts post = new Posts();
					posts.Add(post);
					post.id = a["id"].ToString().Split('_')[1];
					post.imageUrl = Uri.UnescapeDataString(string.Concat(a["picture"], string.Empty));
					post.date = DateTime.Parse(a["created_time"].ToString());
					post.msg = string.Concat(jsonData, string.Empty);
					if (a["comments"] != null)
					{
						post.comments = a["comments"]["data"].Count;
					}
					StartCoroutine(post.Load());
				}
				break;
			}
			}
		}
	}

	public void OnLoggedIn()
	{
		if (bs._Loader.isOdnoklasniki)
		{
			PlayerPrefs.SetString(bs._Loader.vkPassword, bs._Loader.playerName);
		}
		bs.ExternalCall("VkFriends");
		if (fbLoggedIn)
		{
			FacebookGetInfo();
		}
	}

	public void DrawButtons()
	{
		//IL_0059: Unknown result type (might be due to invalid IL or missing references)
		//IL_0069: Unknown result type (might be due to invalid IL or missing references)
		//IL_0074: Expected O, but got Unknown
		//IL_0074: Expected O, but got Unknown
		//IL_00fa: Unknown result type (might be due to invalid IL or missing references)
		//IL_0106: Expected O, but got Unknown
		if (bs.platformPrefix != "flash" && bs._Integration.fbLoggedIn && Button("Invite Friends (unlock new car)", bs.res.faceBook))
		{
			Screen.fullScreen = false;
			bs.win.ShowWindow((Action)(object)(Action)delegate
			{
				if (ButtonLeft("Refresh", null, 0f))
				{
					StartCoroutine(bs._Integration.getInfoFriends());
				}
				Label(string.Format("Invitations sent :{1}\r\nFriends Added    :{0}\n\n Add more friends to unlock new cars!", bs._Integration.FbFriendsInGame, bs._Integration.pendingFriends));
			}, (Action)(object)new Action(bs._Loader.MenuWindow));
			bs.LogEvent("FacebookInviteFriends");
			Facebook.uiAppRequest("TrackMania Online", "Play Popular 3D Racing multiplayer game!");
		}
		if (bs.platformPrefix == "flash" && Button("Play Full Version on Facebook" + ((!facebookLinkPressed) ? "\n and unlock new car!" : string.Empty), bs.res.faceBook))
		{
			bs.LogEvent("Facebook");
			bs.win.ShowWindow((Action)(object)new Action(GoToFbLink));
			if (!alternativeOpen)
			{
			}
		}
	}

	private void GoToFbLink()
	{
		Setup(700, 400, string.Empty);
		LabelCenter("Latest version found at", 16, wrap: true);
		if (Button(Trs(facebookLink)))
		{
			Application.OpenURL(facebookLink);
		}
		GUILayout.FlexibleSpace();
		if (BackButtonLeft())
		{
			if (!facebookLinkPressed)
			{
				facebookLinkPressed = true;
				bs._Loader.wonMedals = 10;
				Loader loader = bs._Loader;
				loader.medals = (int)loader.medals + bs._Loader.wonMedals;
			}
			bs._Loader.WindowPool();
		}
	}

	public void FacebookLogin()
	{
		Facebook.login("user_about_me,publish_actions,publish_stream");
	}

	public void wallPost(float FinnishTime)
	{
		MonoBehaviour.print("<<<<<<<<<<<<<<<<<<<<<postFeed>>>>>>>>>>>>>>>");
		string message = string.Format("Click to play vs {2}'s replay!\r\nTrack:{0} Time:{1}", bs._Loader.mapName, bs.TimeToStr(FinnishTime), bs._Integration.userName);
		Screen.fullScreen = false;
		feedPosted = true;
		if (fbLoggedIn)
		{
			Facebook.postFeed(message, "http://206.190.128.180/tm/docs/150.jpg", bs._Loader.replayLinkPrefix + bs._Loader.replayLink);
			return;
		}
		bs.ExternalCall("PostWall2", bs._Loader.replayLink, bs._Loader.mapName, bs.TimeToStr(FinnishTime));
	}

	public IEnumerator KongParse(string s)
	{
		NameValueCollection q = ParseQueryString(s);
		string name = q.Get("kongregate_username");
		string id = q.Get("kongregate_user_id");
		if (!string.IsNullOrEmpty(name) && !string.IsNullOrEmpty(id) && id != "0")
		{
			userId = id;
			bs._Loader.vkPassword = id;
			userName = name;
			Debug.LogWarning(userId);
			Debug.LogWarning(userName);
			if (string.IsNullOrEmpty(bs._Loader.playerName))
			{
				bs._Loader.playerName = name;
			}
			bs._Loader.vk = true;
			site = Site.Kg;
			bs.ExternalEval("kongregateAPI.loadAPI();");
			vkLoggedIn = true;
			string url = "http://www.kongregate.com/api/user_info.json?user_id=" + userId;
			Debug.LogWarning(url);
			WWW w = new WWW(url);
			yield return w;
			if (!string.IsNullOrEmpty(w.error))
			{
				Debug.LogWarning(w.url + w.error);
				yield break;
			}
			JsonData data = JsonMapper.ToObject(w.text);
			bs._Loader.avatarUrl = data["avatar_url"].ToString();
			Debug.LogWarning("avatar " + bs._Loader.avatarUrl);
		}
	}

	public static NameValueCollection ParseQueryString(string s)
	{
		NameValueCollection nameValueCollection = new NameValueCollection();
		if (s.Contains("?"))
		{
			s = s.Substring(s.IndexOf('?') + 1);
		}
		string[] array = Regex.Split(s, "&");
		foreach (string input in array)
		{
			string[] array2 = Regex.Split(input, "=");
			if (array2.Length == 2)
			{
				nameValueCollection.Add(array2[0], array2[1]);
			}
			else
			{
				nameValueCollection.Add(array2[0], string.Empty);
			}
		}
		return nameValueCollection;
	}

	public void UrlOdnoklasniki(string s)
	{
		//IL_0141: Unknown result type (might be due to invalid IL or missing references)
		//IL_014c: Expected O, but got Unknown
		MonoBehaviour.print("UrlOdnoklasniki " + s);
		if (!(bs._LoaderScene != null) || !bs._Loader.vk)
		{
			return;
		}
		if (s.Contains("?") && bs._Loader.isOdnoklasniki)
		{
			Dictionary<string, string> dictionary = new Dictionary<string, string>();
			string[] array = s.Split('?')[1].Split('&');
			string[] array2 = array;
			foreach (string text in array2)
			{
				string[] array3 = text.Split('=');
				if (array3.Length > 0)
				{
					string key = array3[0].Trim('?', ' ');
					string value = array3[1].Trim();
					dictionary.Add(key, value);
				}
			}
			bs._Loader.vkPassword = dictionary["logged_user_id"];
			MonoBehaviour.print("odno id " + bs._Loader.password);
			bs._Loader.playerName = PlayerPrefs.GetString(bs._Loader.vkPassword, string.Empty);
			ShowWindow((Action)(object)new Action(bs._Loader.LoginWindow));
		}
		else
		{
			bs._Loader.WindowPool();
		}
		UnityEngine.Object.Destroy(bs._LoaderScene.support);
	}

	public void StartVk()
	{
		if (bs.isDebug)
		{
			VkInfo2("212511867,Raimo Nieminen,http://cs303603.vk.me/v303603867/4b03/zh7qvX_z6ls.jpg");
			VkNews("Все качайте игру на телефон,мы сделали видео обзор.;;;0;;;1388879381;;;4692;;;http://cs617131.vk.me/u189810914/video/l_1d2ec6c0.jpg###Набор в тестеры снова открыт http://vk.com/topic-59755500_29093610?offset=20, только помни надо быть активным!;;;0;;;1388851186;;;4674;;;###Дорогие гонщики,мы вам всем дали по джипам,у модераторов теперь новая машина.;;;12;;;1388759908;;;4620;;;###Обновление в силе!;;;3;;;1388753097;;;4601;;;http://cs310228.vk.me/v310228694/780e/P-TEAv8s54c.jpg###Скоро будут тесты новой версии, кто хочет сыграть первым записываемся в тестера!;;;32;;;1388601988;;;4547;;;###Как вы думаете когда 500.000 гонщиков будет ?;;;0;;;1388578161;;;4534;;;###Дорогие Гонщики!Поздравляем вас с Новым годом!Желаем вам прежде всего здоровья,счастья и чтобы все мечты сбылись,а так же больше медалей и репутации!С новым,2014 годом!;;;7;;;1388521406;;;4520;;;###Всех с наступающим!<br>Новогоднее обновление будет когда наберете 100 репостов <br>В обновлении будет:<br>-Зомби режим<br>-Новая тачка <br>-Режим трюков (на очки)<br>-Можно будет редактировать любые официальные карты.;;;22;;;1388476828;;;4490;;;http://cs312529.vk.me/u212511867/video/l_f01c4e27.jpg###А давайте все запомним эту дату! 31.12.2013 - в этот день, в нашу любимую игру теперь играет 100.000 замечательных гонщиков и гонщиц ! Радуемся вместе : И ещё раз всех-все-всех с НАСТУПАЮЩИМ НОВЫМ ГОДОМ!;;;2;;;1388461084;;;4483;;;###Дорогие участники группы!Администрация вас убедительно просит писать сообщения в нужные темы.Не надо писать в тему Вопросы по игре\" ваши ошибки игры.А так же просим не писать вопросы под скриншотами.Для ваших вопросов существуют соответствующие темы.Администрации просто неудобно отвечать на ваши сообщения;;;0;;;1388417150;;;4475;;;###Обновление<br>• Исправлен баг с новыми моделями,теперь сквозь них не проваливаешься.<br>• Добавлена строка,показывающая какие модели были взяты ранее,что довольно удобно при работе с малым кол-вом моделей.;;;1;;;1388389462;;;4453;;;http://cs605224.vk.me/v605224694/51e/a8ZdafYh5lg.jpg###◘ Хотите узнавать новости своей любимой игры первыми?<br>◘ Хотите смотреть обзоры лучших карт?<br>◘ Хотите участвовать в конкурсах на ценные призы?<br><br>Да?<br>_____________________<br>Тогда наше сообщество для вас :<br>http://vk.com/news_trackracing;;;0;;;1388388796;;;4452;;;http://cs413831.vk.me/v413831694/9f47/Dk_xLPLCRuQ.jpg###Вступаем в первый по созданию клан в игре —&gt; http://vk.com/racer_online_fresh;;;0;;;1388381242;;;4451;;;http://cs413829.vk.me/v413829608/4f8d/eq9F6fwENC4.jpg###Пользуйтесь :;;;1;;;1388321197;;;4425;;;http://cs605223.vk.me/v605223694/745/p-K6D5yozy0.jpg###Уважаемые игроки, появилась тема: Жалобы на игроков\". Там вы можете оставить жалобу на игроков (Скрины обязательно)   —-&gt; http://vk.com/topic-59755500_29138173;;;1;;;1388225402;;;4404;;;###;;;2;;;1388165797;;;4386;;;http://cs413829.vk.me/v413829928/5b83/SMD90a8bpyM.jpg###Придумал режим для игры, по трассе разбросаны монеты(очки),  кто их больше всех соберет за 5 минут тот выиграл!;;;7;;;1388161165;;;4379;;;###Конкурс будет окончен через 30 мин (Причину могу не называть);;;0;;;1388149305;;;4366;;;###Друзья! Гонщики! Вся администрация игры поздравляет вас с наступающим 2014 годом! Знаком года будет лошадь,которая такая-же быстрая как и мы :<br>_________<br>Скоро в честь нового года в игре будет добавлена машина,да не простая а с новогодней тематикой!<br><br>Жмите лайки если хотите машину!;;;13;;;1388068748;;;4341;;;http://cs605222.vk.me/v605222694/134/GqZyPTcugzg.jpg###Наверное многие ждали этого :<br><br>Скоро (возможно сегодня) в игре можно будет строить карты как стандартные.;;;0;;;1388018892;;;4310;;;http://cs605221.vk.me/v605221694/a9/odclQqQzvIo.jpg###");
		}
		bs.ExternalCall("VkInfo");
		bs.ExternalCall("VkNews");
	}

	public void VkNews(string s)
	{
		this.posts.Clear();
		string[] array = s.Replace("<br>", string.Empty).Split(new string[1]
		{
			"###"
		}, StringSplitOptions.RemoveEmptyEntries);
		string[] array2 = array;
		foreach (string text in array2)
		{
			try
			{
				string[] array3 = text.Split(new string[1]
				{
					";;;"
				}, StringSplitOptions.None);
				DateTime date = new DateTime(1970, 1, 1, 0, 0, 0, 0).AddSeconds(long.Parse(array3[2]));
				Posts posts = new Posts();
				posts.msg = array3[0].Trim();
				posts.comments = int.Parse(array3[1]);
				posts.date = date;
				posts.id = array3[3];
				posts.imageUrl = array3[4];
				Posts posts2 = posts;
				StartCoroutine(posts2.Load());
				this.posts.Add(posts2);
			}
			catch (Exception message)
			{
				Debug.LogError(message);
			}
		}
		Debug.Log("vknews " + array.Length + ":" + this.posts.Count);
	}

	public void VkFriends(string fr)
	{
		//IL_00f5: Unknown result type (might be due to invalid IL or missing references)
		//IL_0115: Expected O, but got Unknown
		MonoBehaviour.print("VkFriends " + fr);
		string[] array = fr.Split(new char[1]
		{
			','
		}, StringSplitOptions.RemoveEmptyEntries);
		if ((int)bs._Loader.friendCount != array.Length)
		{
			int num = ((int)bs._Loader.friendCount != -1) ? (array.Length - (int)bs._Loader.friendCount) : 0;
			bs._Loader.friendCount = Mathf.Max(array.Length, bs._Loader.friendCount);
			MonoBehaviour.print("<<<<<<<<<< friend added " + num);
			if (num > 0)
			{
				int num2 = num * 10;
				Loader loader = bs._Loader;
				loader.reputation = (int)loader.reputation + num2;
				Popup2(string.Format(GuiClasses.Tr("+{0} reputation points, you added {1} friends"), num2, num), (Action)(object)new Action(bs._Loader.MenuWindow), null, skip: false, 500, 250, bs.res.sendReplayIcon);
			}
		}
		string[] array2 = array;
		foreach (string text in array2)
		{
			bs.Download(bs.mainSite + "/dict/vk" + text.Trim() + ".txt", delegate(string s, bool b)
			{
				if (b)
				{
					string[] array3 = s.Split(':');
					string text2 = array3[0].Trim().ToLower();
					AddFriend(text2);
					GuiClasses.dict[text2] = array3[1];
				}
			}, false);
		}
		bs._Loader.friends = bs._Loader.friends;
	}

	private static void AddFriend(string trim)
	{
		trim = trim.ToLower();
		if (!bs._Loader.friends.Contains(trim))
		{
			MonoBehaviour.print("friend added " + trim);
			bs._Loader.friends.Add(trim);
		}
	}

	public void OnPostedWall()
	{
		if (bs.totalSeconds - PlayerPrefs.GetInt("posted") > 180)
		{
			PlayerPrefs.SetInt("posted", bs.totalSeconds);
			AddReputation(3);
		}
	}

	public void AddReputation(int v, string s = "+{0} reputation points")
	{
		if (v > 0)
		{
			Loader loader = bs._Loader;
			loader.reputation = (int)loader.reputation + v;
			Popup(string.Format(GuiClasses.Tr(s), v), bs.res.sendReplayIcon);
		}
	}

	public void AddMedals(int v, string s = "+{0} medals")
	{
		if (v > 0)
		{
			Loader loader = bs._Loader;
			loader.medals = (int)loader.medals + v;
			Popup(string.Format(GuiClasses.Tr(s), v), bs.res.sendReplayIcon);
		}
	}

	public void VkInfo2(string s)
	{
		vkLoggedIn = true;
		site = Site.VK;
		MonoBehaviour.print("Info " + s);
		string[] array = s.Split(',');
		userId = array[0];
		userName = array[1];
		bs._Loader.avatarUrl = array[2];
		bs._Loader.vkPassword = userId;
		bs.Download(bs.mainSite + "scripts/dict.php", delegate
		{
		}, true, "key", "vk" + userId, "value", bs._Loader.playerNamePrefixed + ":" + userName);
	}
}
