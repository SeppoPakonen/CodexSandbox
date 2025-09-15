using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading;
using UnityEngine;

public class Game : bsNetwork
{
	public Player testPlayer;

	public Player testBot;

	public GUIText scoreText;

	public ParticleEmitter smoke;

	public ParticleSystem smoke2;

	public float[] lightninigs = new float[0];

	public ParticleSystem[] dirt;

	public float customTime = 1f;

	public new Res res;

	public bool started;

	public GameObject ctf;

	internal List<Zombie> zombies = new List<Zombie>();

	internal bool finnish;

	public GUITexture iosMenu;

	internal Transform m_StartPos;

	internal Vector3 startPosOffset;

	[Obsolete]
	private Transform SpawnPos;

	private float lastRecord;

	private int place;

	internal Player finnishedPlayer;

	internal bool backTime;

	private bool wonRecord;

	private bool wonMedal;

	private WWW www;

	internal Player m_Player2;

	internal Player m_Player;

	public List<Player> listOfPlayers = new List<Player>();

	public Dictionary<int, Player> photonPlayers = new Dictionary<int, Player>();

	internal List<bs> finnishedPlayers = new List<bs>();

	public List<Collider> triggers = new List<Collider>();

	public GUITexture textureCenter;

	public GUIText centerText2;

	internal int FrameCount;

	private bool replaysLoaded;

	internal float timeElapsedLevel;

	internal bool editControls;

	internal bool enableKeys = true;

	public ParticleEmitter rainfall;

	public ParticleEmitter[] splash;

	public ParticleEmitter[] sparks;

	public ParticleEmitter[] bulletHit;

	public Light sunLight;

	private List<Material> animMaterials = new List<Material>();

	private static bool sendedWmp;

	private bool debugEndGame;

	public double StartTime = double.MinValue;

	private int startTimeCnt;

	public bool isCustomLevel;

	public int blueTeamCount;

	public int redTeamCount;

	private bool gotDmAward;

	internal float topDownTime;

	public int rewindsUsed;

	internal GameObject[] coins = new GameObject[0];

	private static Vector3[] vct = new Vector3[3]
	{
		new Vector3(-1.5f, -2f),
		new Vector3(1.5f, -2f),
		new Vector3(0f, 2f)
	};

	public Light lightning;

	public AudioSource rainSound;

	public GUIText leftText;

	public HashSet<int> allies = new HashSet<int>();

	public HashSet<int> allyVisible = new HashSet<int>();

	private bool teamSElected;

	internal Player rewindingPlayer;

	public List<Replay> ghosts = new List<Replay>();

	internal List<Renderer> LevelRenderers = new List<Renderer>();

	private string olds;

	private List<Replay> endgamereplays;

	public float startTime;

	public Transform cursor;

	public GUITexture cursorTexture2;

	public GUITexture cursorTexture;

	public List<Flag> flags = new List<Flag>();

	internal Transform StartPos
	{
		get
		{
			startPosOffset = Vector3.zero;
			if (bs._Loader.team)
			{
				return (!bs.setting.autoConnect && !bs.setting.autoHost && (bool)bs._Player && !bs._Player.replay.red) ? blueFlagPos : redFlagPos;
			}
			if (bs._Loader.dm)
			{
				GameObject[] array = GameObject.FindGameObjectsWithTag("Spawn");
				if (array.Length > 0)
				{
					return array[UnityEngine.Random.Range(0, array.Length)].transform;
				}
				if (UnityEngine.Random.value < 1f / (float)(bs.checkPoints.Length + 2) || bs.isDebug)
				{
					return m_StartPos;
				}
				startPosOffset = Vector3.up * 2f;
				return bs.checkPoints[UnityEngine.Random.Range(0, bs.checkPoints.Length)].transform;
			}
			return m_StartPos;
		}
		set
		{
			m_StartPos = value;
		}
	}

	public Transform blueFlagPos
	{
		get
		{
			GameObject gameObject = GameObject.FindGameObjectWithTag("BlueSpawn");
			if ((bool)gameObject)
			{
				return gameObject.transform;
			}
			Transform transform = bs.checkPoints.OrderByDescending<GameObject, float>((GameObject a) => (m_StartPos.position - a.transform.position).magnitude).First().transform;
			startPosOffset = Vector3.up * 4f;
			return transform;
		}
	}

	public Transform redFlagPos
	{
		get
		{
			GameObject gameObject = GameObject.FindGameObjectWithTag("RedSpawn");
			if ((bool)gameObject)
			{
				return gameObject.transform;
			}
			return m_StartPos;
		}
	}

	internal bool backTime2 => Time.timeScale <= 0f || !bs._Game.started;

	public IEnumerable<Player> BlueTeam => ((IEnumerable<Player>)listOfPlayers).Where((Func<Player, bool>)((Player a) => a.replay.blue));

	public IEnumerable<Player> RedTeam => ((IEnumerable<Player>)listOfPlayers).Where((Func<Player, bool>)((Player a) => a.replay.red));

	internal int frameInterval => (int)(0.02f / Time.fixedDeltaTime);

	internal float timeElapsed2
	{
		get
		{
			return timeElapsed;
		}
		set
		{
		}
	}

	internal float timeElapsed => timeElapsedLevel - startTime;

	public void Emit(Vector3 point, Vector3 normal, ParticleEmitter[] ParticleEmitters)
	{
		for (int i = 0; i < ParticleEmitters.Length; i++)
		{
			ParticleEmitter particleEmitter = ParticleEmitters[i];
			if (i == 0)
			{
				particleEmitter.transform.position = point;
				particleEmitter.transform.up = normal + UnityEngine.Random.insideUnitSphere * 0.1f;
			}
			particleEmitter.Emit();
		}
	}

	public void OnEnable()
	{
		MonoBehaviour.print("Game OnEnable");
		bs._Game = this;
	}

	public override void Awake()
	{
		Debug.LogWarning("Game Awake");
		m_StartPos = new GameObject("StartPos").transform;
		bs._Game = this;
		bs._Loader.game = this;
		if (!sendedWmp && !string.IsNullOrEmpty(Application.loadedLevelName))
		{
			SendWmp("open", bs._Loader.mapName);
		}
		sendedWmp = true;
		if (!string.IsNullOrEmpty(Application.loadedLevelName) && string.IsNullOrEmpty(bs._Loader.mapName))
		{
			bs._Loader.mapName = Application.loadedLevelName;
		}
		MonoBehaviour.print(bs._Loader.mapName);
		bs._Loader.prefixMapPl = bs._Loader.mapName + ";" + bs._Loader.playerName + ";";
		bs._Player = ((!testPlayer) ? ((Player)UnityEngine.Object.FindObjectOfType(typeof(Player))) : testPlayer);
		if (bs._Player != null)
		{
			bs._Player.gameObject.SetActive(value: false);
		}
		if (!bs.isDebug)
		{
			customTime = 1f;
		}
		Shader shader = Shader.Find("Specular");
		if (!shader)
		{
			MonoBehaviour.print("Warning specular is null");
		}
		UnityEngine.Object[] array = Resources.FindObjectsOfTypeAll(typeof(Material));
		for (int i = 0; i < array.Length; i++)
		{
			Material material = (Material)array[i];
			if (res.animatedTextures.Contains(material.name))
			{
				animMaterials.Add(material);
			}
			if ((bool)shader && res.specularTextures.Contains(material.name))
			{
				material.shader = shader;
				material.SetFloat("_Shininess", 0.4f);
			}
		}
		Physics.IgnoreLayerCollision(Layer.car, Layer.car, bs.splitScreen);
	}

	public void Start()
	{
		//IL_015b: Unknown result type (might be due to invalid IL or missing references)
		//IL_0165: Expected O, but got Unknown
		//IL_02e5: Unknown result type (might be due to invalid IL or missing references)
		//IL_02f0: Expected O, but got Unknown
		//IL_03f6: Unknown result type (might be due to invalid IL or missing references)
		//IL_0400: Expected O, but got Unknown
		bs._Loader.playedTimes++;
		cursor.parent.gameObject.SetActive(bs._Loader.dm);
		isCustomLevel = (bs._MapLoader != null);
		PhotonNetwork.isMessageQueueRunning = true;
		MonoBehaviour.print("!Game Start server version " + bs._Loader.serverVersion);
		Reset();
		bs._Loader.wonMedals = 0;
		MonoBehaviour.print("Game Loaded");
		if (bs._Loader.levelEditor == null)
		{
			bs._Loader.gamePlayed = true;
		}
		if (bs._Loader.autoFullScreen && !Screen.fullScreen)
		{
			bs._Loader.FullScreen(fullscreen: true);
		}
		SetStartPoint();
		bs.win.CloseWindow();
		if (bs._Loader.sGameType != SGameType.Replay)
		{
			bs._Player = InstanciatePlayer();
			bs._Player.InitNetwork();
			MonoBehaviour.print("set player name " + bs._Loader.playerName);
		}
		if (bs._Loader.sGameType == SGameType.SplitScreen)
		{
			bs._Player2 = InstanciatePlayer();
		}
		InitLevel();
		LoadReplays();
		StartCoroutine(OnStartCorontinue());
		StartCoroutine(Base.AddMethod(10f, (Action)(object)(Action)delegate
		{
			if (bs._AutoQuality.fps <= 100f)
			{
				bs.LogEvent(EventGroup.Fps, bs._Loader.quality.ToString(), (int)(Mathf.Min(70f, bs._AutoQuality.fps) / 10f) + "0fps");
			}
		}));
		bs.LogEvent(string.Concat(bs._Loader.sGameType, string.Empty));
		bs.LogEvent("Load Map Finnish");
		if (bs.splitScreen)
		{
			bs._GameGui.enabled = false;
		}
		if (bs._Loader.rain)
		{
			rainSound.Play();
		}
		if (bs._Loader.night)
		{
			Material material = new Material(RenderSettings.skybox);
			material.SetColor("_Tint", Color.black);
			RenderSettings.skybox = material;
			RenderSettings.ambientLight = Color.black;
		}
		sunLight.enabled = !bs._Loader.night;
		GameObject[] array = GameObject.FindGameObjectsWithTag("node");
		foreach (GameObject gameObject in array)
		{
			Collider collider = gameObject.get_collider();
			bool enabled = false;
			gameObject.get_renderer().enabled = enabled;
			collider.enabled = enabled;
		}
		StartCoroutine(bs._AutoQuality.OnLevelWasLoaded2(0));
		MonoBehaviour.print("Set gravity " + Physics.gravity);
		Resources.UnloadUnusedAssets();
		GC.Collect();
		if (bs._Loader.dm)
		{
			started = true;
		}
		if ((bool)bs._music)
		{
			bs._music.PlayRandom();
		}
		bs._Loader.scoreBoard = null;
		if (bs._Loader.team)
		{
			ShowWindow((Action)(object)new Action(SelectTeamWindow));
		}
		ctf.SetActive(bs._Loader.ctf);
		if (bs._Loader.bombCar)
		{
			centerText(GuiClasses.Tr("You have BombCar, if you collide you will explode!"), 4f);
		}
		if (bs._Loader.stunts)
		{
			centerText(GuiClasses.Tr($"In Stunt Mode, collect score in {bs._GameSettings.levelTime} seconds"), 4f);
		}
		if ((bool)bs._Loader.levelEditor)
		{
			centerText(GuiClasses.Tr("Press T to add coins"), 4f);
		}
		if (bs._Loader.dm)
		{
			centerText(string.Format(GuiClasses.Tr("Press 1-3 to select weapon\n Press {0} to jump"), "g"), 4f);
		}
		if (!bs._Loader.screenshotTaken && UnityEngine.Random.value < 0.3f)
		{
			StartCoroutine(Base.AddMethod(5f, (Action)(object)(Action)delegate
			{
				bs._Loader.TakeScreenshot(showText: false);
			}));
		}
		if (bs._Loader.enableZombies && bs._Loader.resLoaded2)
		{
			SpawnZombies();
		}
		if (!bs._Loader.levelEditor)
		{
			foreach (GameObject item in ((IEnumerable<string>)new string[4]
			{
				"RedSpawn",
				"BlueSpawn",
				"Spawn",
				"zombieSpawn"
			}).SelectMany<string, GameObject>((Func<string, IEnumerable<GameObject>>)((string a) => GameObject.FindGameObjectsWithTag(a))))
			{
				Renderer[] componentsInChildren = item.GetComponentsInChildren<Renderer>();
				foreach (Renderer renderer in componentsInChildren)
				{
					renderer.enabled = false;
				}
			}
		}
		coins = GameObject.FindGameObjectsWithTag("coin");
	}

	private void SpawnZombies()
	{
		if (bs._Loader.stunts)
		{
			GameObject[] array = GameObject.FindGameObjectsWithTag("zombieSpawn");
			MonoBehaviour.print("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
			MonoBehaviour.print(array.Length);
			if (array.Length > 0)
			{
				GameObject[] array2 = array;
				foreach (GameObject gameObject in array2)
				{
					GameObject gameObject2 = (GameObject)UnityEngine.Object.Instantiate(bs.LoadRes("zombie"));
					gameObject2.transform.position = gameObject.transform.position + Vector3.up;
					gameObject2.transform.forward = bs.ZeroY(UnityEngine.Random.insideUnitSphere, 0f);
				}
			}
			else
			{
				if (!(bs._Loader.mapLoader == null))
				{
					return;
				}
				MonoBehaviour.print("Spawn Zombies");
				string[] source = new string[4]
				{
					"ec324e91",
					"0d119bcc",
					"9eadb28a",
					"42494742"
				};
				Bounds bounds = default(Bounds);
				MeshCollider[] array3 = UnityEngine.Object.FindObjectsOfType<MeshCollider>();
				foreach (MeshCollider meshCollider in array3)
				{
					bounds.Encapsulate(meshCollider.bounds);
				}
				UnityEngine.Random.seed = 0;
				int num = 0;
				int num2 = 0;
				while (num < 50 && num2 < 1000)
				{
					Vector3 min = bounds.min;
					Vector3 size = bounds.size;
					float x = UnityEngine.Random.Range(0f, size.x);
					Vector3 size2 = bounds.size;
					float y = size2.y;
					Vector3 size3 = bounds.size;
					Vector3 vector = min + new Vector3(x, y, UnityEngine.Random.Range(0f, size3.z));
					if (Physics.Raycast(vector, Vector3.down, out RaycastHit hitInfo, 1000f, Layer.levelMask) && !source.Contains(((Component)hitInfo.transform).get_renderer().sharedMaterial.name))
					{
						num++;
						Debug.DrawRay(vector, Vector3.down * 1000f, Color.green, 10f);
						GameObject gameObject3 = (GameObject)UnityEngine.Object.Instantiate(bs.LoadRes("zombie"));
						gameObject3.transform.position = hitInfo.point + Vector3.up;
						gameObject3.transform.forward = bs.ZeroY(UnityEngine.Random.insideUnitSphere, 0f);
					}
					num2++;
				}
			}
		}
		else
		{
			if (bs._Loader.replays.Count <= 0)
			{
				return;
			}
			for (int k = 0; k < 30; k++)
			{
				List<PosVel> posVels = bs._Loader.replays[0].posVels;
				if (posVels.Count >= 100)
				{
					PosVel posVel = posVels[UnityEngine.Random.Range(100, posVels.Count)];
					if (Physics.Raycast(posVel.pos + Vector3.up, Vector3.down, out RaycastHit hitInfo2, 10f, Layer.levelMask))
					{
						GameObject gameObject4 = (GameObject)UnityEngine.Object.Instantiate(bs.LoadRes("zombie"));
						gameObject4.transform.position = hitInfo2.point + Vector3.up;
						gameObject4.transform.forward = bs.ZeroY(UnityEngine.Random.insideUnitSphere, 0f);
					}
				}
			}
		}
	}

	public void centerText(string s, float seconds = 4f, bool noAndroid = true)
	{
		if (!noAndroid || !bs.android)
		{
			bs._Loader.centerText(s, Mathf.Max(0.3f, seconds));
		}
	}

	private void VoiceChat(VoiceChatPacket obj)
	{
		bs._Player.replay.voiceChatTime = Time.realtimeSinceStartup;
		if (!bs._Loader.banned)
		{
			bs._Player.photonView.RPC("SendAudio", PhotonTargets.Others, obj.Data, obj.Length, PhotonNetwork.player.ID);
		}
	}

	public void Reset()
	{
		GameObject[] array = coins;
		foreach (GameObject gameObject in array)
		{
			gameObject.gameObject.SetActive(value: true);
		}
		textureCenter.enabled = false;
	}

	public void SetStartPoint()
	{
		if (bs._MapLoader != null)
		{
			bs._MapLoader.UpdateMinimap();
			GameSettings gameSettings = bs._GameSettings;
			Vector3 position = bs._MapLoader.water.transform.position;
			gameSettings.miny = position.y - 4f;
		}
		triggers = new List<Collider>();
		TagMe("CheckPoint");
		TagMe("speed");
		TagMe("engineOff");
		TagMe("noSkid");
		MonoBehaviour.print("SetStartPoint");
		GameObject gameObject = GameObject.FindGameObjectWithTag("Start");
		if (gameObject == null)
		{
			gameObject = GameObject.Find("Start");
		}
		if (gameObject != null)
		{
			gameObject.SetActive(value: false);
			StartPos = gameObject.transform;
		}
		else
		{
			Debug.LogWarning("Start not found");
		}
		if (bs._MapLoader != null && bs._MapLoader.terrain != null)
		{
			((Component)bs._MapLoader.terrain).get_collider().Raycast(new Ray(StartPos.position + Vector3.up * 500f, Vector3.down), out RaycastHit hitInfo, 1000f);
			Vector3 position2 = StartPos.position;
			Vector3 point = hitInfo.point;
			float a = point.y + 3f;
			Vector3 position3 = StartPos.position;
			position2.y = Mathf.Max(a, position3.y);
			StartPos.position = position2;
		}
	}

	public void SendWmp(string c, string p)
	{
	}

	private IEnumerator OnStartCorontinue()
	{
		if (bs.online)
		{
			MonoBehaviour.print(bs._Loader.waitTime);
			Begin();
			yield break;
		}
		base.get_audio().volume = 0.2f;
		MonoBehaviour.print("setting.DontWait:" + bs.setting.DontWait + " inited:" + bs.setting.inited);
		if (bs.setting.DontWait)
		{
			OnStart();
			timeElapsedLevel = 4f;
			startTime = 4f;
		}
		else
		{
			StartCoroutine(CountTo3());
		}
	}

	private IEnumerator CountTo3()
	{
		yield return new WaitForSeconds(1f);
		bs.PlayOneShotGui(bs.res.bip);
		SetCenterTexture(bs.res.go123[3]);
		yield return new WaitForSeconds(1f);
		bs.PlayOneShotGui(bs.res.bip);
		SetCenterTexture(bs.res.go123[2]);
		yield return new WaitForSeconds(1f);
		bs.PlayOneShotGui(bs.res.bip);
		SetCenterTexture(bs.res.go123[1]);
		yield return new WaitForSeconds(1f);
		OnStart();
	}

	private void OnStart()
	{
		//IL_0069: Unknown result type (might be due to invalid IL or missing references)
		//IL_0073: Expected O, but got Unknown
		MonoBehaviour.print("Start!!");
		startTime = timeElapsedLevel;
		centerText2.enabled = false;
		bs._Game.SendWmp("goto", bs._GameSettings.sendWmpOffset + string.Empty);
		SetCenterTexture(bs.res.go123[0]);
		StartCoroutine(Base.AddMethod(1f, (Action)(object)(Action)delegate
		{
			textureCenter.enabled = false;
		}));
		bs.PlayOneShotGui(bs.res.start);
		started = true;
	}

	public void Begin()
	{
		started = false;
		if (PhotonNetwork.time - StartTime + 1.0 > 0.0)
		{
			CallRPC(SetStartTime, PhotonNetwork.time + (double)bs._Loader.waitTime);
		}
	}

	[RPC]
	public void SetStartTime(double time)
	{
		StartTime = time;
	}

	private void UpdateOnStart()
	{
		bool flag = StartTime - PhotonNetwork.time < 1.0;
		if (flag != started && flag)
		{
			OnStart();
		}
		started = flag;
		if (!started)
		{
			int num = (int)(StartTime - PhotonNetwork.time);
			centerText2.enabled = true;
			centerText2.text = num + string.Empty;
			if (startTimeCnt != num)
			{
				bs.PlayOneShotGui(bs.res.bip);
			}
			startTimeCnt = num;
		}
	}

	public void Update()
	{
		//IL_04d1: Unknown result type (might be due to invalid IL or missing references)
		//IL_04dd: Expected O, but got Unknown
		//IL_0527: Unknown result type (might be due to invalid IL or missing references)
		//IL_0533: Expected O, but got Unknown
		if (Loader.errors > 100 && !Debug.isDebugBuild)
		{
			if (bs.online)
			{
				PhotonNetwork.LeaveRoom();
			}
			Exit();
		}
		if (KeyDebug(KeyCode.W))
		{
			CallRPC(bs._Game.FlagCaptured, bs._Player.playerId);
		}
		if (bs.setting.timeLapse)
		{
			FrameCount = (int)(timeElapsed / Time.fixedDeltaTime);
		}
		if (bs.splitScreen)
		{
			Physics.IgnoreLayerCollision(Layer.car, Layer.car, timeElapsed < 3f);
		}
		if (Input.GetKeyDown(KeyCode.LeftControl) && bs._Loader.levelEditor == null)
		{
			Debug.Break();
		}
		if ((bool)bs._Loader.levelEditor && Input.GetKeyDown(KeyCode.T))
		{
			GameObject gameObject = (GameObject)UnityEngine.Object.Instantiate((UnityEngine.Object)res.coin, bs._Player.pos + Vector3.up, Quaternion.identity);
			gameObject.name = res.coin.name;
			gameObject.AddComponent<ModelObject>();
		}
		if (bs._Loader.team)
		{
			blueTeamCount = BlueTeam.Count();
			redTeamCount = RedTeam.Count();
		}
		if (bs.online)
		{
			listOfPlayers.Remove(null);
		}
		if (Input.GetMouseButtonDown(0) && !bs.win.enabled && bs._Loader.dm)
		{
			Screen.lockCursor = true;
		}
		if (bs.setting.lag)
		{
			Thread.Sleep(UnityEngine.Random.Range(0, 100));
		}
		timeElapsedLevel += (((!bs.online || !backTime) && !bs.setting.timeLapse) ? Time.deltaTime : bs._Loader.deltaTime);
		if (started)
		{
			timeElapsed2 += bs._Loader.deltaTime;
		}
		if (!started && bs.online && !bs._Loader.dm)
		{
			UpdateOnStart();
		}
		if (KeyDebug(KeyCode.F1, "Begin"))
		{
			Begin();
		}
		if (KeyDebug(KeyCode.B, "Enable Keys"))
		{
			InputManager.enableKeys = !InputManager.enableKeys;
		}
		if (!bs.win.enabled)
		{
			editControls = false;
		}
		if (!bs._Loader.dm)
		{
			Time.timeScale = ((!bs.win.enabled && !bs._GameGui.chatEnabled && !bs._Game.backTime && !finnish) ? customTime : 0f);
		}
		if (Time.timeScale != 0f && Time.timeScale != 1f)
		{
			Time.fixedDeltaTime = bs._AutoQuality.fixedDeltaTime * Time.timeScale;
		}
		if (bs.Hotkey())
		{
			wonRecord = true;
			debugEndGame = true;
			bs._Player.EndGame();
		}
		if (bs.input.GetKeyDown(KeyCode.R))
		{
			if (bs._Loader.dm && bs.online)
			{
				if (Time.time - bs._Player.upsideDown < 0.1f && Physics.Raycast(bs._Player.pos, Vector3.down, out RaycastHit hitInfo, 3f, Layer.levelMask))
				{
					bs._Player.transform.up = hitInfo.normal;
				}
				if (bs._Loader.dm)
				{
					bs._Player.SetLife((float)bs._Player.life - 50f, base.myId);
				}
			}
			else if (started)
			{
				RestartLevel();
			}
		}
		iosMenu.enabled = ((bs.ios && !bs.win.enabled) || editControls);
		if ((bs._Player != null && iosMenu.enabled && iosMenu.HitTest(Input.mousePosition, ((Component)bs._Player.hud).get_camera()) && Input.GetMouseButtonDown(0)) || bs.input.GetKeyDown(KeyCode.Escape))
		{
			if (!bs.win.enabled)
			{
				bs.win.ShowWindow((Action)(object)new Action(MenuWindow), null, skip: true);
			}
			else
			{
				bs.win.CloseWindow();
			}
		}
		if (bs.input.GetKeyDown(KeyCode.B) && !bs._Loader.team && bs._Loader.dm)
		{
			bs.win.ShowWindow((Action)(object)new Action(AllyWindow));
		}
		if (bs.input.GetKeyDown(KeyCode.O))
		{
			bs._Loader.fieldOfView += 5f;
		}
		if (bs.input.GetKeyDown(KeyCode.P))
		{
			bs._Loader.fieldOfView -= 5f;
		}
		if (TimeElapsed(1f, 0f))
		{
			bs._Game.listOfPlayers.Sort(bs._Player);
		}
		bool flag = (bs._Loader.quality < Quality2.High && bs.android) || bs.lowQuality;
		leftText.enabled = (flag && !bs.splitScreen && !bs.online);
		bs._GameGui.enabled = ((!flag && !bs.splitScreen) || bs.online);
		if (flag && FramesElapsed(10))
		{
			string text = string.Empty;
			bool flag2 = bs._Loader.PlayersCount < 1 && !bs.online;
			for (int i = 0; i < ((!flag2) ? bs._Game.listOfPlayers.Count : bs._Loader.replays.Count); i++)
			{
				Replay replay = (!flag2) ? bs._Game.listOfPlayers[i].replay : bs._Loader.replays[i];
				text = text + replay.getText(i + 1) + "\n";
			}
			leftText.text = text;
		}
		foreach (Material animMaterial in animMaterials)
		{
			if (animMaterial.HasProperty("_MainTex"))
			{
				Vector2 mainTextureOffset = animMaterial.mainTextureOffset;
				mainTextureOffset.x += Time.deltaTime;
				animMaterial.mainTextureOffset = mainTextureOffset;
			}
		}
		UpdateRain();
		if (Input.GetKeyDown(KeyCode.Y))
		{
			MonoBehaviour.print("Voice Chat: " + Application.HasUserAuthorization(UserAuthorization.Microphone));
			if (!VoiceChatRecorder.Instance.enabled)
			{
				StartCoroutine(InitMicrophone());
			}
		}
		UpdateAwards();
	}

	private void UpdateAwards()
	{
		if (bs._Player.topdown)
		{
			topDownTime += Time.deltaTime;
		}
		if (bs.online && !gotDmAward && bs._Loader.PlayersCount > 2 && listOfPlayers.IndexOf(bs._Player) == 0 && ((float)bs._Player.score > 0f || bs._Player.finnishTime > 0f))
		{
			gotDmAward = true;
			if (bs._Loader.dm)
			{
				bs._Awards.DeathMatchOrCtf.Add();
			}
			else if (bs._Loader.race)
			{
				bs._Awards.WinInMultiplayerRace.Add();
			}
			else if (bs._Loader.stunts)
			{
				bs._Awards.zombieMode.Add();
			}
		}
	}

	private IEnumerator InitMicrophone()
	{
		if (bs._Loader.dm && !Application.HasUserAuthorization(UserAuthorization.Microphone))
		{
			ShowWindow((Action)(object)new Action(MenuWindow));
		}
		yield return Application.RequestUserAuthorization(UserAuthorization.Microphone);
		MonoBehaviour.print("Microphones " + Microphone.devices.Length);
		if (Microphone.devices.Length > 0)
		{
			VoiceChatRecorder v = VoiceChatRecorder.Instance;
			v.enabled = true;
			v.NetworkId = PhotonNetwork.player.ID;
			v.Device = v.AvailableDevices[0];
			v.StartRecording();
			v.NewSample -= VoiceChat;
			v.NewSample += VoiceChat;
		}
		yield return null;
	}

	private void UpdateRain()
	{
		RaycastHit hitInfo;
		bool flag = Physics.Raycast(bs._Player.pos + Vector3.up, Vector3.up, out hitInfo, 300f, Layer.levelMask);
		AudioReverbFilter reverbZone = bs._Player.reverbZone;
		if (reverbZone.enabled)
		{
			AudioReverbPreset audioReverbPreset = (!flag) ? AudioReverbPreset.Generic : ((!(hitInfo.distance > 50f)) ? AudioReverbPreset.User : AudioReverbPreset.Hangar);
			if (audioReverbPreset != reverbZone.reverbPreset)
			{
				if (audioReverbPreset == AudioReverbPreset.User)
				{
					reverbZone.reverbPreset = AudioReverbPreset.Drugged;
					reverbZone.decayHFRatio = 0.35f;
				}
				reverbZone.reverbPreset = audioReverbPreset;
			}
		}
		rainfall.enabled = bs._Loader.rain;
		if (!bs._Loader.rain)
		{
			return;
		}
		if (bs._Player != null && bs._Loader.rain)
		{
			((Component)rainfall).get_renderer().material.shader = ((!flag) ? res.rainShader : res.rainShader2);
		}
		float[] array = lightninigs;
		foreach (float num in array)
		{
			float num2 = rainSound.time % rainSound.clip.length;
			if (num >= num2 - Time.deltaTime && num < num2)
			{
				StartCoroutine(Lightning());
			}
		}
		rainfall.transform.position = bs._Player.rainfall.position;
	}

	public void OnRenderObject()
	{
		if (!isCustomLevel || bs._Player == null || bs._Player.camera != Camera.current)
		{
			return;
		}
		GL.PushMatrix();
		res.lineMaterialYellow.SetPass(0);
		GL.LoadOrtho();
		if (bs._Game.isCustomLevel)
		{
			GL.Color((!bs.android) ? Color.yellow : Color.black);
			GL.Begin(1);
			Vector3? vector = null;
			for (int i = 0; i < bs._MapLoader.minimap.Count; i++)
			{
				Vector2 vector2 = bs._MapLoader.minimap[i];
				if (vector.HasValue && vector2 != Vector2.zero && (vector.GetValueOrDefault() != Vector3.zero || !vector.HasValue))
				{
					GL.Vertex(vector.Value);
					GL.Vertex(vector2);
				}
				vector = vector2;
			}
			GL.End();
		}
		GL.Begin(4);
		float d = Mathf.Min(300f, bs._GameSettings.levelBounds.size.magnitude);
		float num = (float)Screen.width / (float)Screen.height;
		foreach (Player listOfPlayer in listOfPlayers)
		{
			GL.Color(GameGui.colors[listOfPlayer.replay.textColor]);
			Vector2 v = MapLoader.ResizeToMinimap(listOfPlayer.pos);
			for (int j = 0; j < vct.Length; j++)
			{
				Vector3 vector3 = listOfPlayer.transform.TransformDirection(new Vector3(vct[j].x, 0f, vct[j].y));
				vector3 = new Vector3(vector3.x, vector3.z * num);
				GL.Vertex((Vector3)v + vector3 / d);
			}
		}
		GL.End();
		GL.PopMatrix();
	}

	public IEnumerator Lightning()
	{
		MonoBehaviour.print("lightbning");
		int turn = (!(UnityEngine.Random.value > 0.6f)) ? 1 : 2;
		lightning.transform.forward = Vector3.down + UnityEngine.Random.insideUnitSphere;
		for (int i = 0; i < turn; i++)
		{
			lightning.enabled = true;
			yield return new WaitForSeconds(0.2f);
			lightning.enabled = false;
			yield return new WaitForSeconds(0.2f);
		}
	}

	public void FixedUpdate()
	{
		if (bs.setting.lag)
		{
			Thread.Sleep(UnityEngine.Random.Range(0, 10));
		}
		if (started && !bs.setting.timeLapse)
		{
			FrameCount++;
		}
	}

	public void OnLeftRoom()
	{
		if (bs._Loader.dm)
		{
			bs._Loader.warScore = Mathf.Max((int)bs._Loader.warScore + (int)(float)bs._Player.score, 0);
			bs._Awards.xp.Add((int)((float)bs._Player.score / 3f));
			SendReplay(null, bs._Player.score);
		}
		Exit();
	}

	public void AllyWindow()
	{
		Setup(400, 700, string.Empty);
		Label("If you ally with player, player will see where you are");
		foreach (Player item in ((IEnumerable<Player>)listOfPlayers).Where((Func<Player, bool>)((Player a) => a != bs._Player)))
		{
			bool flag = allies.Contains(item.playerId);
			bool flag2 = GUILayout.Toggle(flag, GuiClasses.Tr("ally with ") + item.playerNameClan);
			if (flag2 != flag && (!flag2 || allies.Count < 4))
			{
				CallRPC(SetAlly, base.myId, item.playerId, flag2);
				if (!flag2 && allyVisible.Contains(item.playerId))
				{
					CallRPC(SetAlly, item.playerId, base.myId, p3: false);
				}
			}
		}
		if (BackButton())
		{
			bs.win.CloseWindow();
		}
	}

	[RPC]
	public void SetAlly(int from, int to, bool b)
	{
		if (from == base.myId)
		{
			if (b)
			{
				allies.Add(to);
			}
			else
			{
				allies.Remove(to);
			}
		}
		Player player = bs._Game.photonPlayers[from];
		if (to == base.myId)
		{
			if (b)
			{
				allyVisible.Add(from);
				bs._Game.centerText(string.Format(GuiClasses.Tr("To ally with {0} press b"), player.playerNameClan), 4f);
				player.replay.ally = true;
				player.RefreshText();
			}
			else
			{
				allyVisible.Remove(from);
				player.replay.ally = false;
				player.RefreshText();
			}
		}
		bs._GameGui.Chat(player.replay.playerName + GuiClasses.Tr((!b) ? " UnAllied with " : " Allied with ") + bs._Game.photonPlayers[to].replay.playerName);
	}

	public void SelectTeamWindow()
	{
		Setup(500, 300, string.Empty);
		LabelCenter("Select your team");
		GUILayout.BeginHorizontal();
		if (GUILayout.Button(GuiClasses.Tr("Blue Team (") + BlueTeam.Count((Player a) => teamSElected || a != bs._Player) + ")", GUILayout.Height(100f)) || bs.setting.autoConnect)
		{
			SelectTeam(Team.Blue);
		}
		if (GUILayout.Button(GuiClasses.Tr("Red Team (") + RedTeam.Count((Player a) => teamSElected || a != bs._Player) + ")", GUILayout.Height(100f)) || bs.setting.autoHost)
		{
			SelectTeam(Team.Red);
		}
		GUILayout.EndHorizontal();
	}

	public void SelectTeam(Team team)
	{
		teamSElected = true;
		bs.win.CloseWindow();
		if (bs._Player.replay.team != team)
		{
			bs._Player.replay.team = team;
			bs._Player.CallRPC(bs._Player.SetTeam, (int)team);
			StartCoroutine(bs._Player.Reset());
		}
	}

	public void MenuWindow()
	{
		//IL_007d: Unknown result type (might be due to invalid IL or missing references)
		//IL_0088: Expected O, but got Unknown
		//IL_010f: Unknown result type (might be due to invalid IL or missing references)
		//IL_011a: Expected O, but got Unknown
		//IL_0140: Unknown result type (might be due to invalid IL or missing references)
		//IL_014c: Unknown result type (might be due to invalid IL or missing references)
		//IL_0157: Expected O, but got Unknown
		//IL_0157: Expected O, but got Unknown
		//IL_01a8: Unknown result type (might be due to invalid IL or missing references)
		//IL_01bc: Expected O, but got Unknown
		//IL_01f8: Unknown result type (might be due to invalid IL or missing references)
		//IL_020c: Expected O, but got Unknown
		//IL_0214: Unknown result type (might be due to invalid IL or missing references)
		//IL_021e: Expected O, but got Unknown
		Setup(300, 500, Trs("Menu") + " " + bs._Loader.mapName);
		if (Button("Resume"))
		{
			bs.win.CloseWindow();
		}
		if (bs._Loader.team && Button("Choose Team"))
		{
			ShowWindow((Action)(object)new Action(SelectTeamWindow));
		}
		if (Button("FullScreen"))
		{
			bs._Loader.FullScreen(fullscreen: true);
		}
		if (editControls)
		{
			bs.win.style = GUIStyle.none;
			Label("Now Drag and Scale Icons, press back button when done");
			return;
		}
		if (bs._Loader.dm && !bs._Loader.team && Button("Ally"))
		{
			ShowWindow((Action)(object)new Action(AllyWindow));
		}
		if (Button("Settings"))
		{
			bs.win.ShowWindow((Action)(object)new Action(bs._Loader.SettingsWindow), (Action)(object)new Action(MenuWindow));
		}
		if (!bs._Loader.dm && Button("Restart"))
		{
			bs._Game.RestartLevel();
		}
		if (Button("Scoreboard"))
		{
			ShowWindow((Action)(object)new Action(bs._Loader.ScoreBoardWindow), bs.win.act);
		}
		if (bs.isMod && (bs.online || bs.isDebug) && Button("Ban"))
		{
			ShowWindow((Action)(object)new Action(BanWindow), bs.win.act);
		}
		RateButton((Action)(object)new Action(MenuWindow));
		if (bs._Loader.levelEditor != null && Button("Back To Editor"))
		{
			StartCoroutine(bs._Loader.levelEditor.Resume());
		}
		if (bs._Loader.levelEditor == null && Button("Quit"))
		{
			if (!bs._Loader.menuLoaded)
			{
				ApplicationQuit();
			}
			else if (bs.online)
			{
				PhotonNetwork.LeaveRoom();
			}
			else
			{
				Exit();
			}
		}
		if (bs.isDebug && Button("Replay"))
		{
			EndGameReplay();
		}
		if (bs.android && Button("Change Camera") && !bs._Loader.topdown)
		{
			bs._Player.ChangeCamera(bs._Player.curCam + 1);
		}
		bs._AutoQuality.DrawDistance();
	}

	public void BanWindow()
	{
		Setup(600, 600, string.Empty);
		BeginScrollView();
		foreach (Player listOfPlayer in listOfPlayers)
		{
			GUILayout.BeginHorizontal();
			GUILayout.Button(listOfPlayer.replay.playerName);
			if (GUILayout.Button("unban"))
			{
				listOfPlayer.CallRPC(listOfPlayer.Mute, 0);
			}
			if (GUILayout.Button("10 min"))
			{
				listOfPlayer.CallRPC(listOfPlayer.Mute, 10);
			}
			if (GUILayout.Button("12 h"))
			{
				listOfPlayer.CallRPC(listOfPlayer.Mute, 720);
			}
			GUILayout.EndHorizontal();
		}
		GUILayout.EndScrollView();
	}

	public void OnDisconnectedFromPhoton()
	{
		if (bs.online)
		{
			Exit();
		}
	}

	private void Exit()
	{
		LoadLevel("!1");
	}

	private Player InstanciatePlayer()
	{
		if (bs._Player != null && !bs._Loader.menuLoaded)
		{
			bs._Player.gameObject.SetActive(value: true);
			StartPos.position = bs._Player.pos;
			return bs._Player;
		}
		return (!bs.online) ? ((GameObject)UnityEngine.Object.Instantiate(Resources.Load("Player"), StartPos.position + startPosOffset, StartPos.rotation)).GetComponent<Player>() : PhotonNetwork.Instantiate("Player", StartPos.position + startPosOffset, StartPos.rotation, 0).GetComponent<Player>();
	}

	private void LoadReplays()
	{
		if (replaysLoaded)
		{
			return;
		}
		MonoBehaviour.print("Ghosts " + ghosts.Count);
		if (bs._Loader.replays.Count > 0)
		{
			bs._Loader.replays.Sort(bs._Loader.replays[0]);
			int num = bs._Loader.replays.Count - 1;
			int num2 = 1;
			while (num >= 0)
			{
				Replay replay = bs._Loader.replays[num];
				if (num2 > bs._Loader.PlayersCount && bs.vsPlayersOrSplitscreen)
				{
					break;
				}
				AddGhost(replay);
				num--;
				num2++;
			}
		}
		if (!bs._Loader.showYourGhost)
		{
			return;
		}
		foreach (Replay ghost in ghosts)
		{
			AddGhost(ghost);
		}
	}

	private void AddGhost(Replay replay)
	{
		if (!Any(replay))
		{
			replaysLoaded = true;
			Player player = InstanciatePlayer();
			player.playerNameTxt2.text = Trn(replay.playerName);
			player.ghost = true;
			player.replay = replay;
		}
	}

	private bool Any(Replay replay)
	{
		foreach (Player listOfPlayer in listOfPlayers)
		{
			if (listOfPlayer.replay == replay)
			{
				return true;
			}
		}
		return false;
	}

	public override void OnEditorGui()
	{
		if (GUILayout.Button("Clear Player Prefs"))
		{
			PlayerPrefs.DeleteAll();
		}
		base.OnEditorGui();
	}

	private void InitLevel()
	{
		if (!isCustomLevel)
		{
			UnityEngine.Object[] array = UnityEngine.Object.FindObjectsOfType(typeof(MeshCollider));
			for (int i = 0; i < array.Length; i++)
			{
				MeshCollider meshCollider = (MeshCollider)array[i];
				meshCollider.smoothSphereCollisions = true;
				bs._GameSettings.levelBounds.Encapsulate(meshCollider.bounds);
			}
		}
		Initbackground();
		try
		{
			InitSpecular();
		}
		catch (Exception ex)
		{
			Debug.LogError(ex.Message);
		}
	}

	private void Initbackground()
	{
		if (!bs._Loader.autoQuality && !bs.mediumAndroidHigh)
		{
			return;
		}
		GameObject gameObject = (GameObject)bs.LoadRes("TerrainBig");
		if (gameObject != null)
		{
			Vector3 position;
			if (bs._Loader.mapLoader != null)
			{
				position = Vector3.up * 108.1349f;
			}
			else
			{
				RaycastHit hitInfo;
				bool flag = Physics.SphereCast(StartPos.position + Vector3.down * 1000f, 100f, Vector3.up, out hitInfo, 2000f, Layer.levelMask);
				Vector3 b = new Vector3(-1000f, -300f, -1000f);
				position = ((!flag) ? StartPos.position : (hitInfo.point + Vector3.up)) + b;
			}
			UnityEngine.Object.Instantiate((UnityEngine.Object)gameObject, position, Quaternion.identity);
		}
	}

	private void InitSpecular()
	{
		MonoBehaviour.print("Init specular");
		UnityEngine.Object[] array = UnityEngine.Object.FindObjectsOfType(typeof(MeshRenderer));
		for (int i = 0; i < array.Length; i++)
		{
			Renderer renderer = (Renderer)array[i];
			int layer = renderer.gameObject.layer;
			if (layer == Layer.level || layer == Layer.cull)
			{
				LevelRenderers.Add(renderer);
			}
		}
		bs._AutoQuality.UpdateMaterials();
	}

	private void SetCenterTexture(Texture2D texture2D)
	{
		textureCenter.texture = texture2D;
		int width = textureCenter.texture.width;
		int height = textureCenter.texture.height;
		textureCenter.pixelInset = new Rect((float)(-width) / 2f, (float)(-height) / 2f, width, height);
		textureCenter.enabled = true;
	}

	public void RestartLevel()
	{
		Debug.LogWarning("RestartLevel");
		if (bs.splitScreen)
		{
			Physics.IgnoreLayerCollision(Layer.car, Layer.car);
		}
		if (!started)
		{
			return;
		}
		wonRecord = false;
		Time.timeScale = 1f;
		wonMedal = false;
		Base.ClearLog();
		finnishedPlayers.Clear();
		finnish = false;
		foreach (Player listOfPlayer in listOfPlayers)
		{
			if (!bs.online || listOfPlayer == bs._Player)
			{
				listOfPlayer.StartCoroutine(listOfPlayer.Reset());
				listOfPlayer.finnished = false;
				listOfPlayer.oldpos = base.pos;
				listOfPlayer.checkPointsPass.Clear();
				backTime = false;
			}
		}
		bs._Player.replay = bs._Player.replay.CloneAndClear();
		replaysLoaded = false;
		if (bs._Loader.enableCollision && bs._Loader.showYourGhost && bs._Loader.levelEditor == null && !bs.online)
		{
			LoadReplays();
		}
		backTime = false;
		started = false;
		timeElapsed2 = (timeElapsedLevel = 0f);
		StartCoroutine(OnStartCorontinue());
		FrameCount = 0;
		bs.win.CloseWindow();
		Reset();
		GA.API.Quality.NewEvent("Restart " + bs._Loader.mapName, bs._Player.pos);
		if (bs.online)
		{
			return;
		}
		if (bs._Loader.enableZombies)
		{
			Zombie[] array = UnityEngine.Object.FindObjectsOfType<Zombie>();
			foreach (Zombie zombie in array)
			{
				zombie.Reset();
			}
		}
		Zombie.zombieKills = 0;
	}

	public void EndGame(Player fp)
	{
		if (!bs.online)
		{
			bs._Game.ghosts.Add(bs._Player.replay);
		}
		if (bs.online)
		{
			bs._Player.CallRPC((Action<float>)bs._Player.SetScore2, (float)bs._Player.score);
		}
		else
		{
			foreach (Player item in ((IEnumerable<Player>)listOfPlayers).Where((Func<Player, bool>)((Player a) => a.ghost)))
			{
				item.score = item.replay.posVels[item.replay.posVels.Count - 1].score;
			}
		}
		bs._Game.listOfPlayers.Sort(bs._Player);
		finnishedPlayer = fp;
		finnish = true;
		lastRecord = bs._Loader.record;
		endgamereplays = ((!bs.online) ? bs._Loader.replays : ((IEnumerable<Player>)bs._Game.listOfPlayers).Select<Player, Replay>((Func<Player, Replay>)((Player a) => a.replay)).ToList());
		if (bs._Loader.race)
		{
			if (bs._MapLoader == null)
			{
				bs.LogEvent(EventGroup.Maps, "Finnished:" + bs._Loader.mapName);
			}
			if (!debugEndGame && (finnishedPlayer.finnishTime < bs._Loader.record || bs._Loader.levelEditor != null || bs.isDebug))
			{
				if (bs.online)
				{
					SendReplay(null, bs._Player.replay.finnishTime);
				}
				else if (!bs.splitScreen && !bs.guest && !bs._Loader.enableCollision)
				{
					WriteSaveReplay();
				}
				bs._Loader.record = timeElapsed2;
				wonRecord = true;
			}
			if (listOfPlayers.Count > 1 && !bs.online)
			{
				place = finnishedPlayers.Count;
			}
			else
			{
				place = endgamereplays.Count((Replay a) => a.finnishTime != 0f && a.finnishTime < bs._Player.finnishTime && a != bs._Player.replay) + 1;
			}
			int num = place;
			if (num > 3)
			{
				num = 3;
			}
			if (endgamereplays.Count == 0)
			{
				num = 3;
			}
			if (num < 4)
			{
				bs.PlayOneShotGui(bs.res.winSound);
				if (num < bs._Loader.place || bs._Loader.place == 4 || bs.isDebug || bs.online)
				{
					wonMedal = true;
					int num2 = Mathf.Abs(bs._Loader.place - num);
					MonoBehaviour.print("won " + num2);
					bs._Loader.wonMedals = num2;
					Loader loader = bs._Loader;
					loader.medals = (int)loader.medals + num2;
					bs._Loader.place = num;
				}
			}
		}
		else if (bs._Loader.stunts)
		{
			if ((!bs.splitScreen && !bs.online && !bs.guest) || bs.isDebug)
			{
				WriteSaveReplay();
			}
			place = listOfPlayers.Count((Player a) => (float)a.score > (float)bs._Player.score && a != bs._Player) + 1;
			if (place < 4)
			{
				bs.PlayOneShotGui(bs.res.winSound);
				if (place < bs._Loader.place)
				{
					wonMedal = true;
					int num3 = Mathf.Abs(bs._Loader.place - place);
					MonoBehaviour.print("won " + num3);
					bs._Loader.wonMedals = num3;
					Loader loader2 = bs._Loader;
					loader2.medals = (int)loader2.medals + num3;
					bs._Loader.place = place;
				}
			}
		}
		bs._Game.listOfPlayers.Sort(bs._Player);
		MonoBehaviour.print("wonRecord" + wonRecord);
		StartCoroutine(EndGame2());
	}

	public void OnDisable()
	{
	}

	public IEnumerator EndGame2()
	{
		MonoBehaviour.print("_Loader.levelEditor " + bs._Loader.levelEditor);
		MonoBehaviour.print(!Base.PlayerPrefsGetBool("voted" + bs._Loader.mapName) || bs.isDebug);
		bs._Awards.OnEndGame();
		if (isCustomLevel && bs._Loader.levelEditor == null && (!Base.PlayerPrefsGetBool("voted" + bs._Loader.mapName) || bs.isDebug))
		{
			yield return StartCoroutine(bs.win.ShowWindow2((Action)(object)new Action(bs._Loader.RateMapWindow)));
		}
		if (bs._Loader.levelEditor != null)
		{
			StartCoroutine(bs._Loader.levelEditor.Resume());
		}
		else if (wonMedal || bs._Awards.awards.Any((Award a) => a.local > 0))
		{
			bs.win.ShowWindow((Action)(object)new Action(WonMedalWindow));
		}
		else
		{
			bs.win.ShowWindow((Action)(object)new Action(EndGameWindow));
		}
		yield return null;
	}

	private void EndGameReplay()
	{
		started = false;
		bs.win.CloseWindow();
		finnish = false;
		finnishedPlayers.Clear();
		foreach (Player listOfPlayer in listOfPlayers)
		{
			listOfPlayer.StartCoroutine(listOfPlayer.Reset());
		}
		FrameCount = 0;
		timeElapsedLevel = 0f;
		StartCoroutine(OnStartCorontinue());
	}

	private void EndGameWindow()
	{
		//IL_0391: Unknown result type (might be due to invalid IL or missing references)
		//IL_03a5: Expected O, but got Unknown
		bs.win.Setup(550, 450, string.Format(GuiClasses.Tr("Track {0} Complete!"), bs._Loader.mapName), Dock.Center, null, null, 1f);
		if (!bs.android)
		{
			GUILayout.Space(50f);
		}
		if (www != null && !bs.setting.vk2 && string.IsNullOrEmpty(www.error))
		{
			if (www.isDone)
			{
				Label(Trn("Done: ") + www.text, 10);
			}
			else
			{
				Label(Trn("uploading: ") + (int)(www.uploadProgress * 100f), 10);
			}
		}
		base.skin.label.fontSize = 20;
		GUILayout.Label(GUIContent(finnishedPlayer.playerNameClan, finnishedPlayer.avatar));
		if (endgamereplays.Count > 0)
		{
			bs.win.LabelAnim(GuiClasses.Tr(bs.Ordinal(place)) + " " + Trs("place") + " /" + (endgamereplays.Count + 1), 18, center: true);
		}
		Label(Trs("Retries: ") + finnishedPlayer.replay.backupRetries);
		if (bs._Loader.race)
		{
			if (wonRecord)
			{
				bs.win.LabelAnim("You Have new personal record", 14, center: true);
			}
			bs.win.LabelAnim(Trs("Current Time:").PadRight(20) + bs.TimeToStr(finnishedPlayer.finnishTime));
			if (lastRecord != float.MaxValue)
			{
				bs.win.LabelAnim(Trs("Last Time:").PadRight(20) + bs.TimeToStr(lastRecord));
			}
		}
		if ((float)bs._Player.score > 0f)
		{
			bs.win.LabelAnim(Trs("Score:").PadRight(20) + (int)(float)bs._Player.score);
		}
		if (bs._Loader.enableZombies)
		{
			bs.win.LabelAnim(Trs("Zombies Killed:").PadRight(20) + Zombie.zombieKills + "/" + bs._Game.zombies.Count);
		}
		GUILayout.FlexibleSpace();
		GUILayout.BeginHorizontal();
		if (!bs._Loader.dm && Button("Restart"))
		{
			bs._Game.RestartLevel();
		}
		if (!bs.setting.vk2 && Button("Scoreboard"))
		{
			ShowWindow((Action)(object)new Action(bs._Loader.ScoreBoardWindow), bs.win.act);
		}
		if (!bs.online && Button("Next Track"))
		{
			LoadLevel("!1");
		}
		GUILayout.EndHorizontal();
	}

	[RPC]
	public override void OnPlConnected()
	{
		CallRPC(SetStartTime, StartTime);
	}

	public void RateButton(Action a)
	{
		//IL_0076: Unknown result type (might be due to invalid IL or missing references)
		//IL_0081: Expected O, but got Unknown
		if (bs._Loader.levelEditor == null && (!Base.PlayerPrefsGetBool("voted" + bs._Loader.mapName) || (bs.isDebug && !Button("AlreadyVoted"))) && Button("Rate this map"))
		{
			ShowWindow((Action)(object)new Action(bs._Loader.RateMapWindow), a);
		}
	}

	private void WonMedalWindow()
	{
		//IL_0087: Unknown result type (might be due to invalid IL or missing references)
		//IL_0093: Expected O, but got Unknown
		Setup(600, 450, string.Empty);
		DrawAwards();
		GUILayout.FlexibleSpace();
		if (!ButtonLeft("Continue", null, 0f))
		{
			return;
		}
		foreach (Award award in bs._Awards.awards)
		{
			award.local = 0;
		}
		bs.win.ShowWindow((Action)(object)new Action(EndGameWindow));
	}

	private void DrawAwards()
	{
		bs._Awards.DrawReward(bs._Awards.xp);
		LabelCenter("Achievements");
		GUILayout.BeginHorizontal();
		if (wonMedal)
		{
			for (int i = (!bs.isDebug) ? bs._Loader.place : 0; i <= 3; i++)
			{
				GUILayout.Label(bs.res.medals[i]);
			}
		}
		foreach (Award award in bs._Awards.awards)
		{
			if (award.local > 0)
			{
				base.skin.label.imagePosition = ImagePosition.ImageAbove;
				GUILayout.Label(new GUIContent(award.local + string.Empty, award.texture, GuiClasses.Tr(award.title)));
			}
		}
		GUILayout.FlexibleSpace();
		GUILayout.EndHorizontal();
		GUILayout.Label(GUI.tooltip);
	}

	private void WriteSaveReplay()
	{
		if (bs._Player.posVels.Count < 100)
		{
			MonoBehaviour.print("cannot save Empty replay");
		}
		BinaryWriter binaryWriter = null;
		if (!bs._Loader.dontUploadReplay)
		{
			using (binaryWriter = new BinaryWriter())
			{
				foreach (PosVel posVel in bs._Player.posVels)
				{
					binaryWriter.Write((byte)1);
					binaryWriter.Write(posVel.pos);
					binaryWriter.Write(posVel.rot.eulerAngles);
					binaryWriter.Write(posVel.vel);
					binaryWriter.Write((byte)5);
					binaryWriter.Write(posVel.mouserot);
					binaryWriter.Write((byte)8);
					binaryWriter.Write(posVel.skid);
					binaryWriter.Write((byte)12);
					binaryWriter.Write(posVel.score);
				}
				foreach (KeyDown keyDown in bs._Player.replay.keyDowns)
				{
					binaryWriter.Write((byte)2);
					binaryWriter.Write((int)keyDown.keyCode);
					binaryWriter.Write(keyDown.time);
					binaryWriter.Write(keyDown.down);
				}
				binaryWriter.Write((byte)3);
				binaryWriter.Write(bs._Player.replay.avatarId);
				MonoBehaviour.print(binaryWriter.Length);
				binaryWriter.Write((byte)4);
				binaryWriter.Write(bs._Player.replay.carSkin);
				binaryWriter.Write((byte)6);
				binaryWriter.Write(bs._Player.replay.finnishTime);
				binaryWriter.Write((byte)7);
				binaryWriter.Write((byte)bs._Player.replay.contry);
				binaryWriter.Write((byte)9);
				binaryWriter.Write(bs.setting.version);
				if (!string.IsNullOrEmpty(bs._Loader.avatarUrl))
				{
					binaryWriter.Write((byte)11);
					binaryWriter.Write(bs._Loader.avatarUrl);
				}
				if (bs._Player.carSkin.haveColor)
				{
					binaryWriter.Write((byte)10);
					Color color = bs._Player.carSkin.color;
					binaryWriter.Write(color.r);
					binaryWriter.Write(color.g);
					binaryWriter.Write(color.b);
				}
				binaryWriter.Write((byte)13);
				binaryWriter.Write(bs._Player.replay.playerName);
				binaryWriter.Write((byte)14);
				binaryWriter.Write(bs._Player.replay.clanTag);
			}
		}
		SendReplay(binaryWriter, (!bs._Loader.dmOrCoins) ? bs._Player.replay.finnishTime : ((float)bs._Player.score));
	}

	private void SendReplay(MemoryStream ms, float score)
	{
		if (!bs._Loader.mh && !bs.guest)
		{
			if (bs._Loader.userId == 0)
			{
				Debug.LogError("User id is zero");
				return;
			}
			www = bs.Download(bs.mainSite + "scripts/sendReplay2.php", null, true, "map", bs._Loader.curScene.mapId, "user", bs._Loader.userId, "flags", (int)bs._Loader.replayFlags, "playerName", bs._Loader.playerNamePrefixed, "mapName", bs._Loader.mapNamePrefixed, "file", (ms != null) ? ((object)ms.ToArray()) : ((object)string.Empty), "time", score, "version", bs.setting.replayVersion, "fps", (int)bs._AutoQuality.fps, "retries", bs._Player.replay.backupRetries, "scoreOnly", bs._Loader.dmOrCoins ? 1 : 0);
		}
	}

	private void TagMe(string tag)
	{
		GameObject[] array = GameObject.FindGameObjectsWithTag(tag);
		foreach (GameObject gameObject in array)
		{
			if (tag != "CheckPoint")
			{
				gameObject.GetComponentInChildren<Renderer>().enabled = false;
			}
			triggers.Add(gameObject.get_collider());
		}
	}

	[RPC]
	internal void FlagCaptured(int plid)
	{
		//IL_00c2: Unknown result type (might be due to invalid IL or missing references)
		//IL_00cd: Expected O, but got Unknown
		foreach (Flag flag in flags)
		{
			flag.ResetPos(play: false);
		}
		foreach (Player listOfPlayer in bs._Game.listOfPlayers)
		{
			listOfPlayer.resetTime = Time.time;
		}
		MonoBehaviour.print("FlagCaptured" + plid);
		Player pl = photonPlayers[plid];
		ShowWindow((Action)(object)(Action)delegate
		{
			Setup(500, 300, string.Empty);
			string text = string.Format(GuiClasses.Tr("{0} brought enemy flag home, {1} wins!"), pl.playerNameClan, GuiClasses.Tr((!pl.sameTeam) ? "enemy team" : "our team"));
			base.skin.label.wordWrap = true;
			GUILayout.Label(new GUIContent(text, pl.avatar));
			if (GUILayout.Button(GuiClasses.Tr("Continue"), GUILayout.ExpandHeight(expand: true)))
			{
				bs.win.CloseWindow();
			}
		});
		StartCoroutine(bs._Player.Reset());
		bs.PlayOneShotGui((!pl.sameTeam) ? res.youLoose : res.youWin);
		if (pl.sameTeam)
		{
			bs.PlayOneShotGui(res.youWin2);
		}
		bs.PlayOneShotGui(res.endGame);
		started = false;
		StartCoroutine(CountTo3());
	}

	[RPC]
	public void SetSpeedLimit(float limit)
	{
		bs._Loader.speedLimitEnabled = true;
		bs._Loader.speedLimit = limit;
	}
}
