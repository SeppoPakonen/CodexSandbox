using CodeStage.AntiCheat.ObscuredTypes;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class Player : bsNetwork, IComparer<Player>
{
	internal struct State
	{
		internal double timestamp;

		internal Vector3 pos;

		internal Quaternion rot;
	}

	private const int minVel = 25;

	public Light muzzleFlashLight;

	public Vector3 distanceToCursor = Vector3.one * 99999f;

	public Vector3 distanceToCursor2;

	public int TargetPlayerId = -1;

	public Transform FlagPlaceHolder;

	public Hud hud;

	public AndroidHud androidHud;

	public Skidmarks SkidMarks;

	public Transform rainfall;

	public Transform cam;

	public bool brake;

	private float groundedTime;

	private bool movingBack;

	private bool movingBack2;

	internal Transform leftWhell;

	internal Transform rightWhell;

	internal Transform upLeftWhell;

	internal Transform upRightWhell;

	private float tireRot;

	private float tireRot2;

	public Replay replay = new Replay();

	private bool grounded = true;

	internal bool grounded2 = true;

	public float Friq = 0.06f;

	private float rotVel;

	internal int rewinds;

	public new InputManager input;

	private AudioSource idleAudio;

	private AudioSource stopAudio;

	private AudioSource motorAudio;

	private AudioSource windAudio;

	private AudioSource backTimeAudio;

	private AudioSource hornAudio;

	private AudioSource nitroAudio;

	private int[] skidMarksArray = new int[4];

	internal Vector3 oldpos;

	public float totalMeters;

	public float checkPointMeters;

	internal bool up;

	internal bool down;

	internal bool left;

	internal bool right;

	private bool groundHit = true;

	public Transform[] camPoses;

	public Camera camera;

	public Transform model;

	public bool ghost;

	private Vector3 deltaPos;

	private Vector3 ghostVel;

	public ParticleEmitter[] emiters;

	internal Rigidbody rigidbody;

	internal new Transform transform;

	public Transform flashLight;

	private float hitForce;

	public int randomHash;

	public Collider carBox;

	public MeshTest meshTest;

	public GameObject dmTr;

	private List<PosVel> oldPosVels;

	public Renderer shield;

	public ParticleEmitter[] capsule;

	private bool carIdSet;

	public CarSkin carSkin;

	private CarDamage carDamage;

	public Renderer[] renderers;

	public List<Material[]> materials = new List<Material[]>();

	private float oldTime;

	private float deltaTime;

	private Vector3 splashTime;

	private Transform[] wheels;

	private Vector3[] wheelPos = new Vector3[4];

	internal bool isKinematic;

	public bool enableGravity = true;

	private Vector3 springAng;

	private Transform corpus2;

	private bool animateSpring;

	private Vector3 sprintOldVel;

	private Vector3 springAngVel;

	private float velSmooth;

	private float lastJump;

	private float fvBrake;

	private Vector3 oldVel2;

	private Vector3 oldVel;

	private Vector3 oldAng;

	internal float upAngle;

	internal float upsideDown = float.MinValue;

	private float groundedTime2;

	public bool m_dead;

	public float suspendTime;

	private float meshHitTime;

	private float deathTime;

	private float hitTime;

	private float crashTime;

	private int crCount;

	private int lastCheckFrame;

	private float sparkTime;

	public float lastBorderHit;

	private bool nitroDown;

	internal int curCam;

	internal ObscuredFloat nitro = 0f;

	internal List<GameObject> fx = new List<GameObject>();

	private float flashMouse;

	public float angularVel;

	private float avrgVelSlow = 50f;

	internal float avrVel;

	private bool[] triggerColOld = new bool[100];

	private float speed;

	public bool engineOff;

	private AudioClip oldStopAudio;

	private float skid;

	private float oldSpeed;

	public static bool dirtMaterial;

	private RaycastHit camHit;

	private bool camHited;

	internal Vector3 turretDirection = Vector3.forward;

	internal Vector3 shootDirection = Vector3.forward;

	public AudioReverbFilter reverbZone;

	private Vector3 camrot;

	public GUIText playerNameTxt;

	private VoiceChatPlayer voiceChatPlayer;

	private bool voiceChatFirst;

	internal int lap = 1;

	public TextMesh teamGuiText;

	private Renderer teamGuiTextRenderer;

	private float showTextTime;

	private State[] m_BufferedState = new State[20];

	private int m_TimestampCount;

	public float interpolationBackTime;

	private float oldTimeElapsed;

	internal bool[] keys = new bool[330];

	public bool finnished;

	internal float m_finnishTime;

	internal ObscuredFloat m_finnishTimeSec;

	internal int m_IsMine = -1;

	public bool? m_IsMaster;

	internal List<Transform> checkPointsPass = new List<Transform>();

	private static int minVelForSkid = 5;

	public bool firstPlayer;

	public bool secondPlayer;

	internal float velm;

	private int step;

	private float LastTimeSend;

	public WeaponBase[] weapons = new WeaponBase[0];

	internal float spawnTime;

	public ObscuredFloat score;

	private Vector3? camDefPos;

	public GameObject fire;

	public float resetTime;

	internal int curWeaponId;

	public GameObject ice;

	public ObscuredFloat life = 100f;

	private Player killedBy;

	public float lastHitTime;

	internal float freezeTime = float.MinValue;

	internal float freeze;

	private float fireTime = float.MinValue;

	private Vector3 oldRot;

	private float tempScore;

	private float oldTempScore;

	public bool testPlayer => this == bs._Game.testPlayer;

	public bool testBot => this == bs._Game.testBot;

	public bool test => testPlayer || testBot;

	public Texture2D avatar => bs.res.GetAvatar(replay.avatarId, replay.avatarUrl);

	internal List<PosVel> posVels => replay.posVels;

	public bool haveFlag => bs._Loader.ctf && bs._Game.flags.Any((Flag a) => a.pl == this);

	public bool sameTeam => bs._Loader.team && team == bs._Player.team;

	internal int playerId => base.photonView.ownerId;

	public static bool angleTest => bs.setting.AngleTest;

	internal bool dead
	{
		get
		{
			return m_dead || !base.enabled;
		}
		set
		{
			speed = 0f;
			m_dead = value;
			if (value)
			{
				deathTime = bs._Game.timeElapsed;
			}
			if (!ghost)
			{
				if (value)
				{
					rigidbody.angularDrag = 5f;
				}
				rigidbody.useGravity = true;
				rigidbody.drag = (value ? 1 : 0);
				return;
			}
			if (dead && !rigidbody)
			{
				rigidbody = base.gameObject.AddComponent<Rigidbody>();
				rigidbody.velocity = ghostVel;
				rigidbody.mass = 1f;
			}
			if (!dead)
			{
				UnityEngine.Object.Destroy(rigidbody);
			}
		}
	}

	private bool fps => curCam == 2;

	public bool topdown => curCam == 4;

	private float skid2 => (1f - Mathf.Pow(Mathf.Abs(Vector3.Dot(transform.forward, vel.normalized)), velm / 3f)) * 2f;

	private bool UpsideDown => Time.time - upsideDown < 0.1f && upAngle > 140f;

	internal string playerNameClan => replay.playerNameClan;

	public GUIText playerNameTxt2 => playerNameTxt;

	private Vector3 syncPos => base.pos;

	public bool visible
	{
		get
		{
			if (!ghost || !bs.lowOrAndorid || (bs._Player.pos - syncPos).magnitude < 30f)
			{
				return true;
			}
			return visible2;
		}
	}

	private bool visible2
	{
		get
		{
			Vector3 point = bs._Player.camera.WorldToViewportPoint(syncPos);
			return new Rect(0f, 0f, 1f, 1f).Contains(point) && point.z > 0f;
		}
	}

	public Team team => replay.team;

	internal float finnishTime
	{
		get
		{
			return (!bs.online || !IsMine) ? m_finnishTime : ((float)m_finnishTimeSec);
		}
		set
		{
			if (bs.online && IsMine)
			{
				m_finnishTimeSec = value;
			}
			else
			{
				m_finnishTime = value;
			}
		}
	}

	public bool IsMine
	{
		get
		{
			if (m_IsMine == -1)
			{
				m_IsMine = (base.photonView.isMine ? 1 : 0);
			}
			return m_IsMine == 1;
		}
	}

	public bool isMaster
	{
		get
		{
			if (!m_IsMaster.HasValue)
			{
				m_IsMaster = PhotonNetwork.isMasterClient;
			}
			return m_IsMaster.Value;
		}
	}

	public Vector3 vel
	{
		get
		{
			return (!isKinematic) ? rigidbody.velocity : ghostVel;
		}
		set
		{
			if (!isKinematic)
			{
				rigidbody.velocity = value;
			}
			else
			{
				ghostVel = value;
			}
		}
	}

	public float fx2 => Time.fixedDeltaTime / 0.02f;

	public bool froozen => Time.time - freezeTime < 3f;

	private float lifeDef
	{
		get
		{
			if (bs._Loader.team && bs._Game.BlueTeam.Any() && bs._Game.RedTeam.Any())
			{
				float num = (float)bs._Game.redTeamCount / (float)bs._Game.blueTeamCount;
				float num2 = Mathf.Lerp(1f, (!replay.red) ? num : (1f / num), 0.4f);
				return bs._Loader.lifeDef * num2;
			}
			return bs._Loader.lifeDef;
		}
	}

	public WeaponBase curWeapon => weapons[curWeaponId];

	public override void Awake()
	{
		resetTime = Time.time;
		voiceChatPlayer = GetComponent<VoiceChatPlayer>();
		rigidbody = base.get_rigidbody();
		transform = base.transform;
		SkidMarks.transform.parent = null;
		SkidMarks.transform.position = Vector3.zero;
		SkidMarks.transform.rotation = Quaternion.identity;
		if ((bs._Player == null && !testBot) || testPlayer)
		{
			bs._Player = this;
		}
		bs._Game.listOfPlayers.Add(this);
		camera = ((Component)cam.GetComponentInChildren<Camera>()).get_camera();
		cam.gameObject.SetActive(value: false);
		replay.carSkin = bs._Loader.carskin;
		replay.contry = bs._Loader.Country;
		replay.avatarId = bs._Loader.avatar;
		if (!bs.android && !bs.ios)
		{
			UnityEngine.Object.Destroy(androidHud.gameObject);
		}
		randomHash = GetHashCode();
		if (testPlayer)
		{
			PhotonNetwork.player.actorID = 0;
			bs._Game.photonPlayers.Add(base.photonView.ownerId = 0, this);
		}
		else if (testBot)
		{
			m_IsMine = 0;
			ghost = true;
			bs._Game.photonPlayers.Add(base.photonView.ownerId = 1, this);
		}
		else if (bs.online)
		{
			base.enabled = false;
			if (!bs._Game.photonPlayers.ContainsKey(base.photonView.ownerId))
			{
				bs._Game.photonPlayers.Add(base.photonView.ownerId, this);
			}
			else
			{
				Debug.LogError(base.photonView.ownerId + " ID Already exists");
			}
			MonoBehaviour.print(bs.online + "Add " + base.photonView.ownerId);
		}
		carBox.isTrigger = true;
		if (bs._Loader.dm)
		{
			WeaponBase weaponBase = weapons.FirstOrDefault((WeaponBase a) => bs.GetFlag((int)bs._Loader.weaponEnum, (int)a.weaponEnum));
			if (weaponBase != null)
			{
				SelectWeapon(weaponBase.id);
			}
			else
			{
				SelectWeapon(0);
			}
		}
		else
		{
			UnityEngine.Object.Destroy(dmTr);
		}
		base.Awake();
	}

	private void DeactiveWeps()
	{
		for (int i = 0; i < weapons.Length; i++)
		{
			weapons[i].id = i;
			weapons[i].pl = this;
			weapons[i].gameObject.SetActive(value: false);
		}
	}

	public void Start()
	{
		carBox.isTrigger = false;
		if (bs.online)
		{
			ghost = !base.photonView.isMine;
		}
		base.name = "Player " + replay.playerName;
		replay.pl = this;
		firstPlayer = (this == bs._Player);
		secondPlayer = (this == bs._Player2);
		LoadSkin();
		MeshCollider componentInChildren = model.GetComponentInChildren<MeshCollider>();
		if ((bool)componentInChildren && componentInChildren.convex)
		{
			UnityEngine.Object.Destroy(carBox.gameObject);
			carBox = componentInChildren;
			carBox.transform.parent = this.transform;
			MonoBehaviour.print("mass center Offset " + (base.pos - carBox.bounds.center).magnitude);
		}
		Transform[] componentsInChildren = model.GetComponentsInChildren<Transform>();
		Transform[] array = componentsInChildren;
		foreach (Transform transform in array)
		{
			if (transform.name == "ult" || transform.name == "ul")
			{
				upLeftWhell = transform;
			}
			else if (transform.name == "urt" || transform.name == "ur")
			{
				upRightWhell = transform;
			}
			else if (transform.name == "lt" || transform.name == "dl")
			{
				leftWhell = transform;
			}
			else if (transform.name == "rt" || transform.name == "dr")
			{
				rightWhell = transform;
			}
			else if (transform.name == "corpus")
			{
				corpus2 = transform;
			}
		}
		animateSpring = (corpus2 != null && !ghost);
		if (this == bs._Player)
		{
			bs._Loader.prefixMapPl = bs._Loader.mapName + ";" + bs._Loader.playerName + ";";
		}
		StartCoroutine(Reset());
		playerNameTxt.enabled = false;
		InitInput();
		InitSounds();
		bool flag = (bs.highQuality && !bs.android) || !ghost;
		flashLight.gameObject.SetActive(bs._Loader.night && flag);
		flashLight.GetChild(0).gameObject.SetActive(!ghost);
		Collider[] componentsInChildren2 = GetComponentsInChildren<Collider>();
		if (!bs._Loader.enableCollision || bs._Loader.dm)
		{
			if (ghost)
			{
				Collider[] array2 = componentsInChildren2;
				foreach (Collider collider in array2)
				{
					if (collider.tag != "HitBox" || collider == carBox)
					{
						UnityEngine.Object.DestroyImmediate(collider);
					}
				}
			}
		}
		else
		{
			Collider[] array3 = componentsInChildren2;
			foreach (Collider collider2 in array3)
			{
				collider2.enabled = !ghost;
			}
		}
		if (bs._Loader.topdown)
		{
			ChangeCamera(4);
		}
		if (!bs._Loader.dm)
		{
			ChangeCamera(1);
		}
		if (this == bs._Player)
		{
			Physics.gravity = bs.res.gravitation * Vector3.down;
		}
		if (bs._Loader.enableCollision && ghost)
		{
			oldPosVels = new List<PosVel>(posVels);
		}
		if (playerNameTxt is GUIText)
		{
			playerNameTxt.transform.parent = null;
		}
	}

	public override void OnPlConnected()
	{
		Debug.LogWarning("OnPlConnected");
		InitNetwork();
	}

	public void InitNetwork()
	{
		//IL_0203: Unknown result type (might be due to invalid IL or missing references)
		//IL_020d: Expected O, but got Unknown
		CallRPC(SetFinnishTime, finnishTime);
		CallRPC(SetNick2, (!IsMine) ? replay.playerName : bs._Loader.playerNamePrefixed);
		CallRPC(SetAvatarUrl, (!IsMine) ? replay.avatarUrl : bs._Loader.avatarUrl);
		string text = (bs._Loader.clanTag.Length <= 0) ? string.Empty : ("[" + bs._Loader.clanTag + "]");
		CallRPC(SetClanTag, (!IsMine) ? replay.clanTag : text);
		CallRPC(SetAvatar, replay.avatarId);
		CallRPC(SetCarId, replay.carSkin);
		if (bs._Loader.dm)
		{
			CallRPC((Action<float>)SetLife2, (float)life);
		}
		if (bs._Loader.dmOrCoins)
		{
			CallRPC((Action<float>)SetScore2, (float)score);
		}
		Color? color = replay.color;
		if (color.HasValue)
		{
			SetColorRPC(replay.color.Value);
		}
		else if (IsMine && carSkin.haveColor)
		{
			SetColorRPC(carSkin.color);
		}
		CallRPC(SetTeam, (int)replay.team);
		CallRPC((Action)(object)new Action(SetEnabled));
		if (bs._Loader.dm)
		{
			CallRPC(SelectWeapon, curWeaponId);
		}
	}

	public static void SetFlag(GameObject car, CountryCodes cc)
	{
		if (!car.name.StartsWith("carF1"))
		{
			return;
		}
		MonoBehaviour.print("Setting Flag for car " + cc);
		Renderer[] componentsInChildren = car.GetComponentsInChildren<Renderer>();
		foreach (Renderer renderer in componentsInChildren)
		{
			if (renderer.name == "DrawCall_0013")
			{
				Material material = bs.LoadRes("Flags/" + cc.ToString().ToLower() + "Flag") as Material;
				if (material != null)
				{
					renderer.sharedMaterial = material;
				}
				else
				{
					MonoBehaviour.print("Flag not found");
				}
			}
		}
	}

	[RPC]
	private void SetClanTag(string s)
	{
		replay.clanTag = s;
	}

	[RPC]
	private void SetColor(float r, float g, float b)
	{
		Color color = new Color(r, g, b);
		MonoBehaviour.print("Set Color " + color);
		replay.color = color;
	}

	private void SetColorRPC(Color c)
	{
		CallRPC(SetColor, c.r, c.g, c.b);
	}

	[RPC]
	private void SetCarId(int obj)
	{
		carIdSet = true;
		replay.carSkin = obj;
		carSkin = bs._Loader.GetCarSkin(replay.carSkin, IsMine);
	}

	[RPC]
	private void SetAvatar(int Obj)
	{
		replay.avatarId = Obj;
	}

	[RPC]
	public void SetEnabled()
	{
		base.enabled = true;
	}

	[RPC]
	public void SetAvatarUrl(string s)
	{
		MonoBehaviour.print(s);
		replay.avatarUrl = s;
	}

	[RPC]
	public void SetNick2(string Obj)
	{
		playerNameTxt2.text = Trn(Obj);
		replay.playerName = Obj;
	}

	[RPC]
	private void SetFinnishTime(float fin)
	{
		float num3 = replay.finnishTime = (finnishTime = fin);
	}

	private void LoadSkin()
	{
		if (!carIdSet)
		{
			Debug.LogWarning("LoadCarSkin Before CarSet! " + playerNameClan);
		}
		replay.carSkin = Mathf.Max(replay.carSkin, 0);
		carSkin = bs._Loader.GetCarSkin((!bs._Loader.dm) ? replay.carSkin : 0, firstPlayer);
		UnityEngine.Object.Destroy(model.gameObject);
		GameObject gameObject = (GameObject)UnityEngine.Object.Instantiate((UnityEngine.Object)carSkin.model);
		if (replay.contry != 0)
		{
			SetFlag(gameObject, replay.contry);
		}
		gameObject.transform.parent = transform;
		gameObject.transform.localPosition = gameObject.transform.position + Vector3.down * 0.37f;
		gameObject.transform.localRotation = Quaternion.identity;
		if (bs._Loader.autoQuality || !bs.lowQualityAndAndroid)
		{
			meshTest = gameObject.GetComponentInChildren<MeshTest>();
		}
		model = gameObject.transform;
		renderers = model.GetComponentsInChildren<Renderer>();
		Color? color = replay.color;
		if (color.HasValue)
		{
			carSkin.SetColor(renderers, replay.color);
		}
		Renderer[] array = renderers;
		foreach (Renderer renderer in array)
		{
			materials.Add(renderer.sharedMaterials);
		}
		carDamage = GetComponent<CarDamage>();
		if (carDamage != null)
		{
			carDamage.meshFilters = ((IEnumerable<Renderer>)renderers).Where((Func<Renderer, bool>)((Renderer a) => a.tag == "damage")).Select<Renderer, MeshFilter>((Func<Renderer, MeshFilter>)((Renderer a) => a.GetComponent<MeshFilter>())).ToArray();
			if (carDamage.meshFilters.Length == 0)
			{
				carDamage.enabled = false;
			}
		}
	}

	private void InitInput()
	{
		if (!ghost || bs._Player == this)
		{
			cam.gameObject.SetActive(value: true);
		}
		if (bs.splitScreen)
		{
			Camera obj = ((Component)hud).get_camera();
			Rect rect = (!(bs._Player2 == this)) ? new Rect(0f, 0f, 1f, 0.5f) : new Rect(0f, 0.5f, 1f, 1f);
			camera.rect = rect;
			obj.rect = rect;
			if (secondPlayer)
			{
				GUILayer component = camera.GetComponent<GUILayer>();
				bool enabled = false;
				camera.GetComponent<AudioListener>().enabled = enabled;
				component.enabled = enabled;
			}
		}
		if (ghost)
		{
			UnityEngine.Object.Destroy(hud.gameObject);
		}
		else
		{
			hud.transform.parent = null;
			hud.transform.position = ((!secondPlayer) ? Vector3.zero : (Vector3.left * 50f));
			hud.transform.rotation = Quaternion.identity;
		}
		cam.transform.parent = null;
		if (testBot)
		{
			cam.gameObject.SetActive(value: false);
		}
		if (!ghost)
		{
			input = (InputManager)UnityEngine.Object.Instantiate((UnityEngine.Object)bs._Loader.inputManger);
			input.pl = this;
		}
		if (secondPlayer)
		{
			replay.playerName = GuiClasses.Tr("Second Player");
		}
		if (secondPlayer && bs._Loader.reverseSplitScreen)
		{
			List<Transform> trs = GetTrs(hud.transform);
			foreach (Transform item in trs)
			{
				item.parent = null;
			}
			Transform obj2 = hud.transform;
			Vector3 localEulerAngles = new Vector3(0f, 0f, 180f);
			camera.transform.localEulerAngles = localEulerAngles;
			obj2.localEulerAngles = localEulerAngles;
			foreach (Transform item2 in trs)
			{
				item2.parent = hud.transform;
			}
		}
		if (ghost)
		{
			if (!bs._Loader.enableZombies)
			{
				UnityEngine.Object.Destroy(rigidbody);
			}
			isKinematic = true;
		}
		if (bs.splitScreen && bs._Loader.reverseSplitScreen)
		{
			Screen.orientation = ScreenOrientation.AutoRotation;
		}
	}

	private void InitSounds()
	{
		dopler(base.get_audio());
		idleAudio = InitSound(bs.res.idle);
		stopAudio = InitSound(bs.res.brake);
		AudioClip engineSound = bs.res.CarSkins[Mathf.Max(0, replay.carSkin)].engineSound;
		motorAudio = InitSound(engineSound);
		backTimeAudio = InitSound(bs.res.backTime, play: false);
		if (firstPlayer)
		{
			windAudio = InitSound(bs.res.wind);
		}
		hornAudio = InitSound(bs.res.horn, play: false);
		hornAudio.loop = false;
		nitroAudio = InitSound(bs.res.nitro, play: false);
		if (this == bs._Player)
		{
			base.get_audio().priority = 0;
		}
	}

	public void Update()
	{
		Update2();
		UpdateText();
	}

	private void DamageText(string Text)
	{
		DamageText damageText = (DamageText)UnityEngine.Object.Instantiate((UnityEngine.Object)bs.res.damageText, base.pos, Quaternion.identity);
		damageText.tmMesh.text = Text;
	}

	public void Update2()
	{
		if (KeyDebug(KeyCode.Tab))
		{
			Freeze();
		}
		UpdateConstrains();
		if (oldTime != 0f)
		{
			deltaTime = Time.realtimeSinceStartup - oldTime;
		}
		oldTime = Time.realtimeSinceStartup;
		if (Time.deltaTime > 1f)
		{
			return;
		}
		if (oldpos != Vector3.zero)
		{
			deltaPos = base.pos - oldpos;
		}
		UpdateSounds();
		if (!bs._Game.started && ghost && !bs.online)
		{
			base.pos = bs._Player.pos;
			rot = bs._Player.rot;
		}
		if (!bs._Game.started)
		{
			groundHit = (grounded = (grounded2 = true));
		}
		if (!bs.android || !ghost || !bs.lowQuality)
		{
			ParticleEmitter[] array = emiters;
			foreach (ParticleEmitter particleEmitter in array)
			{
				particleEmitter.emit = (!bs._Game.backTime2 && speed > 0f && GetKey(KeyCode.W));
			}
		}
		UpdateHelpText();
		if ((!bs._Game.started || bs._Game.editControls || finnished) && !bs.online)
		{
			return;
		}
		UpdateTimeRevert();
		if (bs._Loader.dm)
		{
			UpdatePlayerDm();
		}
		if (dead && !ghost)
		{
			return;
		}
		FallCheck();
		UpdateInput();
		UpdateRecordBackupReplay2();
		UpdateGhost();
		UpdateTriggers();
		UpdateFixed();
		UpdateWhell();
		UpdateSkidMarks();
		if (bs._Loader.stunts)
		{
			UpdateStunts();
		}
		if (!ghost)
		{
			totalMeters += deltaPos.magnitude * (float)((!bs._Game.backTime) ? 1 : (-1));
		}
		if (!bs._Loader.dm && ((!bs._Loader.topdown && GetKeyDown(KeyCode.C)) || (fps && bs._Game.finnish)))
		{
			ChangeCamera(curCam + 1);
		}
		if (GetKey(KeyCode.F) && !hornAudio.isPlaying)
		{
			hornAudio.volume = 1f;
			if (carSkin.horn.Length > 0)
			{
				hornAudio.clip = carSkin.horn[UnityEngine.Random.Range(0, carSkin.horn.Length)];
			}
			hornAudio.Play();
		}
		if (!ghost && bs._Loader.rain)
		{
			bs._Game.rainfall.localVelocity = bs.ZeroY(rigidbody.velocity / 2f, 0f) + Vector3.down * 40f;
		}
		if (speed > 0f && !nitroAudio.isPlaying)
		{
			nitroAudio.Play();
		}
		if (speed <= 0f && nitroAudio.time > 0.3f && nitroAudio.isPlaying)
		{
			nitroAudio.Stop();
		}
		if (hud != null)
		{
			StringBuilder stringBuilder = new StringBuilder();
			if (!bs._Loader.dm)
			{
				stringBuilder.Append(Mathf.Max(0, rewinds).ToString());
			}
			if (bs._GameSettings.laps > 1 && !bs._Loader.dm)
			{
				stringBuilder.Append(GuiClasses.Tr("\nLap: ") + Mathf.Min(lap, bs._GameSettings.laps) + "/" + bs._GameSettings.laps);
			}
			if (bs._Loader.dm)
			{
				stringBuilder.Append(GuiClasses.Tr("\nLife:")).Append((int)(float)life);
			}
			if ((float)nitro > 0f)
			{
				stringBuilder.Append(GuiClasses.Tr("\nNitro:") + (int)((float)nitro * 100f));
			}
			hud.backup.text = stringBuilder.ToString();
		}
		oldpos = base.pos;
	}

	private void UpdateHelpText()
	{
		if (!bs._Game.backTime && bs._Game.started)
		{
			if ((avrgVelSlow < 7f && bs._Game.timeElapsed > 3f) || dead)
			{
				ShowCenterTextReset();
			}
			if (avrgVelSlow < 20f)
			{
				bs._GameGui.ShowHelpScreen(0.1f);
			}
		}
	}

	private void UpdateConstrains()
	{
		if (!ghost)
		{
			rigidbody.constraints = ((!bs._Game.started) ? ((RigidbodyConstraints)10) : RigidbodyConstraints.None);
		}
	}

	private void UpdateSkidMarks()
	{
		if (!bs.medium || bs._Game.backTime2 || rightWhell == null)
		{
			return;
		}
		if (wheels == null)
		{
			wheels = new Transform[4]
			{
				leftWhell,
				rightWhell,
				upLeftWhell,
				upRightWhell
			};
			for (int i = 0; i < wheels.Length; i++)
			{
				ref Vector3 reference = ref wheelPos[i];
				reference = wheels[i].localPosition;
			}
		}
		if (bs._Game.isCustomLevel)
		{
			UpdateWater();
		}
		for (int j = 0; j < wheels.Length; j++)
		{
			if (animateSpring)
			{
				wheels[j].localPosition = Vector3.Lerp(wheels[j].localPosition, wheelPos[j], Time.deltaTime * 30f);
			}
			else if ((j > 1 || stopAudio.volume < 0.8f) && !animateSpring)
			{
				skidMarksArray[j] = -1;
				continue;
			}
			if (Physics.Raycast(wheels[j].position, -transform.up, out RaycastHit hitInfo, 0.7f, Layer.levelMask))
			{
				if (animateSpring)
				{
					wheels[j].position = Vector3.Lerp(wheels[j].position, wheels[j].position - transform.up * (Mathf.Min(0.4f, hitInfo.distance) - 0.35f), Time.deltaTime * 30f);
				}
				if (j > 1)
				{
					continue;
				}
				if (stopAudio.volume > 0.8f)
				{
					if (!bs.flash && !dirtMaterial)
					{
						Vector3 pos = hitInfo.point + transform.up * 0.05f;
						skidMarksArray[j] = SkidMarks.AddSkidMark(pos, hitInfo.normal, skid / 2f, skidMarksArray[j]);
					}
					SkidSmoke(j);
				}
				else
				{
					skidMarksArray[j] = -1;
				}
			}
			else
			{
				stopAudio.volume = 0f;
			}
		}
	}

	private void SkidSmoke(int i)
	{
		Vector3 vector = oldpos;
		for (int j = 0; j < 5; j++)
		{
			vector = Vector3.MoveTowards(vector, base.pos, (!dirtMaterial) ? 0.5f : 1f);
			if (dirtMaterial)
			{
				ParticleSystem[] dirt = bs._Game.dirt;
				foreach (ParticleSystem particleSystem in dirt)
				{
					particleSystem.transform.position = wheels[i].position + (oldpos - vector);
					particleSystem.Emit(1);
				}
			}
			else
			{
				bs._Game.smoke.transform.position = wheels[i].position + (oldpos - vector);
				bs._Game.smoke.Emit(1);
			}
			if (vector == base.pos)
			{
				break;
			}
		}
	}

	private void UpdateWater()
	{
		Vector3 pos = base.pos;
		Ray ray = new Ray(pos + Vector3.up, Vector3.down);
		Vector3 position = bs._MapLoader.water.position;
		float y = position.y;
		Vector3 pos2 = base.pos;
		if (pos2.y - 0.3f < y && new Plane(Vector3.down, y).Raycast(ray, out float enter) && velm > 1f)
		{
			Vector3 point = ray.GetPoint(enter);
			Vector3 vector = splashTime;
			vector.y = point.y;
			for (int i = 0; i < 5; i++)
			{
				vector = Vector3.MoveTowards(vector, point, 1f);
				if (vector == point)
				{
					break;
				}
				if (i == 0 && !base.get_audio().isPlaying)
				{
					if (base.get_audio().clip != bs.res.waterSound)
					{
						AudioSource.PlayClipAtPoint(bs.res.waterSound2, base.pos, bs._Loader.soundVolume);
					}
					base.get_audio().clip = bs.res.waterSound;
					base.get_audio().Play();
				}
				splashTime = pos;
				bs._Game.splash[0].transform.position = vector;
				ParticleEmitter[] splash = bs._Game.splash;
				foreach (ParticleEmitter particleEmitter in splash)
				{
					if (particleEmitter.particleCount < 50)
					{
						particleEmitter.Emit();
					}
				}
			}
		}
		else
		{
			if (base.get_audio().clip == bs.res.waterSound)
			{
				base.get_audio().clip = null;
			}
			splashTime = base.pos;
		}
	}

	private void UpdateFixed()
	{
		if (isKinematic)
		{
			return;
		}
		if (topdown)
		{
			camHited = Physics.Linecast(camera.transform.position, base.pos + transform.up * 2f, out camHit, Layer.levelMask);
		}
		else
		{
			camHited = false;
		}
		if (!FramesElapsedA(3, randomHash))
		{
			return;
		}
		string item = string.Empty;
		groundHit = false;
		dirtMaterial = false;
		if (speed == 3.01f)
		{
			speed = 0f;
		}
		if (!Physics.Raycast(base.pos, -transform.up * 2f, out RaycastHit hitInfo, 2f, Layer.levelMask))
		{
			return;
		}
		if (!groundHit)
		{
			groundHit = true;
			if (ghost)
			{
				grounded2 = true;
				groundedTime = Time.time;
			}
			if (hitInfo.collider != null && ((Component)hitInfo.collider).get_renderer() != null && ((Component)hitInfo.collider).get_renderer().sharedMaterial != null)
			{
				item = ((Component)hitInfo.collider).get_renderer().sharedMaterial.name;
			}
			if (hitInfo.collider.name == "Terrain(Clone)" || hitInfo.collider.name == "Terrain")
			{
				item = "ground";
			}
			if (bs.res.dirtMaterials.Contains(item))
			{
				dirtMaterial = true;
			}
		}
		if (hitInfo.collider.tag == "Dirt")
		{
			dirtMaterial = true;
		}
		if (hitInfo.collider.tag == RoadType.Speed.ToString() && input.GetKey(KeyCode.W))
		{
			speed = 3.01f;
		}
	}

	private void UpdateRotation()
	{
		if (isKinematic)
		{
			return;
		}
		if (!ghost)
		{
			float num;
			if (bs._Loader.enableMouse)
			{
				if (bs.android)
				{
					num = androidHud.mouse.x;
				}
				else if (firstPlayer)
				{
					Vector3 mousePosition = Input.mousePosition;
					num = (mousePosition.x / (float)Screen.width - 0.5f) * 2f;
				}
				else
				{
					num = 0f;
				}
			}
			else
			{
				num = 0f;
			}
			flashMouse = num;
			if (bs._Loader.enableMouse)
			{
				flashMouse += Input.GetAxis((!firstPlayer) ? "Horizontal2" : "Horizontal");
			}
			flashMouse = Mathf.Clamp(flashMouse, -1f, 1f) * carSkin.getRotationMouse();
		}
		float num2 = left ? (-1f) : ((!right) ? (flashMouse * bs._Loader.sensivity * 2f) : 1f);
		bool flag = groundHit && Time.time - groundedTime < 0.5f;
		if (flag)
		{
			angularVel = Mathf.MoveTowards(angularVel, rotVel, Time.deltaTime * 5f);
			if ((angularVel > 0f && !right) || (angularVel < 0f && !left))
			{
				angularVel *= 0.95f;
			}
		}
		else if ((rotVel < 0f && angularVel > 0f) || (rotVel > 0f && angularVel < 0f) || carSkin.flyRotation)
		{
			angularVel = Mathf.Lerp(angularVel, rotVel, Time.deltaTime * 0.6f);
		}
		if (bs._Loader.enableMouse)
		{
			rotVel = num2 * 3f * bs._GameSettings.Rotation;
		}
		else
		{
			int num3 = ((!left || !(rotVel > 0f)) && (!right || !(rotVel < 0f)) && (right || left)) ? 3 : 5;
			float num4 = Time.deltaTime * carSkin.getRotation() * (float)num3 * Mathf.Min(1f, 0.5f + velm / 100f);
			if (bs.setting.zanos)
			{
				num4 /= Mathf.Clamp(skid2 * 3f, 1f, 2.3f);
			}
			float b = (1f + skid / 2f) * num2 * bs.res.RotCurve.Evaluate(velm * 4f) * 2.5f * bs._GameSettings.Rotation;
			rotVel = Mathf.Lerp(rotVel, b, num4);
		}
		float num5 = Mathf.Min(1f, velm / bs.res.rotationStart);
		if (bs._Game.started && !isKinematic)
		{
			float value = (float)((!movingBack) ? 1 : (-1)) * num5 * ((!flag) ? angularVel : rotVel);
			rigidbody.MoveRotation(rigidbody.rotation * Quaternion.Euler(0f, Mathf.Clamp(value, -7f, 7f) * Time.deltaTime * 17f, 0f));
		}
	}

	private void UpdateWhell()
	{
		if (rightWhell != null)
		{
			if (!brake)
			{
				float num = velm * (float)((!movingBack2) ? 1 : (-1)) * 3.14f * 20f * Time.deltaTime;
				tireRot += num;
				tireRot2 += num * (float)((!up || !(velm < (float)minVelForSkid)) ? 1 : 5);
			}
			Transform obj = leftWhell;
			Vector3 localEulerAngles = new Vector3(tireRot2, 0f, 0f);
			rightWhell.localEulerAngles = localEulerAngles;
			obj.localEulerAngles = localEulerAngles;
			Transform obj2 = upRightWhell;
			localEulerAngles = new Vector3(tireRot, Mathf.Clamp(rotVel * 10f, -45f, 45f), 0f);
			upLeftWhell.localEulerAngles = localEulerAngles;
			obj2.localEulerAngles = localEulerAngles;
		}
	}

	public void FixedUpdate()
	{
		if (bs._Loader.dm && bs._Loader.speedLimitEnabled)
		{
			this.vel = Vector3.ClampMagnitude(this.vel, bs._Loader.speedLimit / 3.6f);
		}
		velm = this.vel.magnitude;
		if (ghost)
		{
			FixedUpdateGhost();
		}
		UpdateDeadCam();
		if (isKinematic || dead || ghost)
		{
			return;
		}
		if (animateSpring)
		{
			Vector3 a = Quaternion.Inverse(rot) * this.vel;
			Vector3 vector = (a - sprintOldVel) * 5f;
			vector = new Vector3(vector.z * 1f, vector.x * 0.2f, vector.x * 0.5f);
			springAngVel = Vector3.Lerp(springAngVel, vector - springAng, Time.deltaTime);
			springAng += springAngVel;
			springAng = Vector3.Lerp(springAng, vector, 10f * Time.deltaTime);
			sprintOldVel = a;
			corpus2.localEulerAngles = Vector3.ClampMagnitude(-springAng * 2f, 5f);
		}
		upAngle = Vector3.Angle(transform.up, Vector3.up);
		UpdateCamPos();
		if (froozen)
		{
			return;
		}
		if (angleTest)
		{
			rigidbody.angularDrag = 5f;
		}
		else
		{
			rigidbody.angularDrag = ((!(Time.time - upsideDown < 0.1f)) ? 20 : 5);
		}
		avrgVelSlow = Mathf.Lerp(avrgVelSlow, velm, (!(avrgVelSlow < velm)) ? (Time.deltaTime * 0.5f) : 100f);
		avrVel = Mathf.Lerp(avrVel, velm, Time.deltaTime);
		if (enableGravity)
		{
			if (!ghost)
			{
				if ((!(Time.time - groundedTime < 0.1f) || !groundHit) && Physics.Raycast(base.pos, Vector3.down, out RaycastHit hitInfo, (!bs._Game.isCustomLevel) ? 2 : 4, Layer.levelMask))
				{
					rigidbody.useGravity = false;
					Vector3 vector2 = bs.ZeroY(hitInfo.normal, 0f);
					rigidbody.AddForceAtPosition(bs.res.gravitation * Vector3.down, (!(Vector3.Dot(transform.forward, vector2) < 0f)) ? (rigidbody.worldCenterOfMass + vector2 * ((!angleTest) ? 1f : 0.2f)) : rigidbody.worldCenterOfMass);
					if (bs._GameSettings.gravitationAntiFly > 1f)
					{
						rigidbody.AddForce(bs.res.gravitation * Vector3.down * (bs._GameSettings.gravitationAntiFly - 1f));
					}
				}
				else
				{
					rigidbody.useGravity = true;
				}
			}
		}
		else
		{
			rigidbody.useGravity = false;
		}
		AirResistenseAndMass();
		if (camera != null)
		{
			float num = (float)(((curCam != 2 && curCam != 1) || bs.splitScreen) ? 65 : 80) + bs._Loader.fieldOfView;
			camera.fieldOfView = Mathf.Min(num * 1.5f, Mathf.Lerp(camera.fieldOfView, num + Mathf.Max(0f, velm - avrVel) * 0.5f + speed * 3f, Time.deltaTime));
		}
		if (!ghost)
		{
			UpdateRecordBackupReplay();
		}
		if (finnished)
		{
			return;
		}
		grounded = (Time.time - groundedTime < 0.2f && groundHit);
		if (rigidbody.IsSleeping())
		{
			rigidbody.WakeUp();
		}
		if (grounded)
		{
			if (brake)
			{
				this.vel = Vector3.Lerp(this.vel, Vector3.zero, fvBrake * Time.deltaTime * 0.3f * bs._GameSettings.Brake);
				this.vel = Vector3.MoveTowards(this.vel, Vector3.zero, fvBrake * Time.deltaTime * 30f);
			}
			else if (up || down)
			{
				float num2 = bs.res.ForceCurve.Evaluate(velm * 4f) + skid * 5f;
				float num3 = num2 * (float)(up ? 1 : (-1)) * 0.65f * bs._GameSettings.speed * carSkin.getSpeed();
				if (bs.setting.speedTweak)
				{
					num3 *= 0.7f;
				}
				rigidbody.AddForce(ZeroY2(transform.forward) * num3);
			}
			else if (!engineOff)
			{
				this.vel = Vector3.Lerp(this.vel, Vector3.zero, Time.deltaTime * 0.1f);
			}
			movingBack2 = (Vector3.Dot(transform.forward, this.vel) < 0f);
			if (down && movingBack2)
			{
				movingBack = true;
			}
			if (up && !movingBack2)
			{
				movingBack = false;
			}
		}
		if (speed > 0f && GetKey(KeyCode.W))
		{
			rigidbody.AddForce(transform.forward * ((bs._Game.isCustomLevel || nitroDown) ? Mathf.Max(0f, speed * 15f - velm / 3f) : (speed * 15f)));
		}
		fvBrake = Mathf.Lerp(fvBrake, brake ? 1 : 0, Time.deltaTime * 3f);
		if (Time.time - groundedTime < 0.05f)
		{
			Vector3 b = Vector3.Project(this.vel, transform.forward);
			float num4 = (!(upAngle > 70f)) ? 0f : (upAngle * ((float)Math.PI / 180f));
			float num5 = ((!dirtMaterial) ? (((!brake && !input.GetKey(KeyCode.RightControl)) ? 1f : carSkin.getBrakeDrift()) * carSkin.getDrift()) : 0.5f) + b.magnitude / 450f + num4 * 6f;
			if (velm > 1f || upAngle > 10f)
			{
				b = Vector3.Slerp(this.vel, b, num5 * Friq * (bs._Loader.snow ? 0.2f : ((!bs._Loader.rain) ? 1f : 0.5f)) * fx2 / Mathf.Min(skid / 2f + 0.5f, 2f));
			}
			float x = b.x;
			float y;
			if (velm < 25f)
			{
				Vector3 vel = this.vel;
				y = vel.y;
			}
			else
			{
				y = b.y;
			}
			this.vel = new Vector3(x, y, b.z);
		}
		UpdateRotation();
		if (!grounded)
		{
			Stabilize();
		}
		if (!grounded && base.easy && !bs._GameSettings.disableFlyDir)
		{
			Vector3 dir = Vector3.Project(this.vel, transform.forward);
			Debug.DrawRay(base.pos, dir);
			Vector3 vel2 = this.vel;
			float x2 = dir.x;
			Vector3 vel3 = this.vel;
			this.vel = Vector3.Lerp(vel2, new Vector3(x2, vel3.y, dir.z), Time.deltaTime).normalized * velm;
		}
		grounded2 = false;
		hitForce = Mathf.Lerp(hitForce, 0f, Time.deltaTime * 10f);
		if (angleTest)
		{
			Vector3 angularVelocity = rigidbody.angularVelocity;
			angularVelocity.y = Mathf.Lerp(angularVelocity.y, 0f, Time.deltaTime * 10f);
			angularVelocity.z = Mathf.Lerp(angularVelocity.z, 0f, Time.deltaTime * 10f);
			angularVelocity.x = Mathf.Lerp(angularVelocity.x, 0f, Time.deltaTime * 10f);
			rigidbody.angularVelocity = angularVelocity;
		}
		oldVel2 = oldVel;
		oldVel = this.vel;
		oldAng = rigidbody.angularVelocity;
	}

	private void UpdateDeadCam()
	{
		if (dead && !ghost && killedBy != null && !killedBy.dead)
		{
			cam.LookAt(killedBy.transform);
			if (Vector3.Distance(cam.position, killedBy.pos) > 10f)
			{
				cam.position = Vector3.Lerp(cam.position, killedBy.pos, Time.deltaTime * 5f);
			}
		}
	}

	private void Stabilize()
	{
		float num = 0.5f;
		Vector3 forward = transform.forward;
		if (forward.y > 0f - num)
		{
			Vector3 forward2 = transform.forward;
			float d = forward2.y + num;
			rigidbody.AddForceAtPosition(Vector3.down * ((!angleTest) ? 9f : 3f) * d, rigidbody.worldCenterOfMass + transform.forward);
			rigidbody.AddForceAtPosition(Vector3.up * ((!angleTest) ? 9f : 3f) * d, rigidbody.worldCenterOfMass - transform.forward);
		}
	}

	private void AirResistenseAndMass()
	{
		if (Time.time - groundedTime > 0.5f)
		{
			if (bs._GameSettings.gravitationFactor > 1f)
			{
				rigidbody.AddForce(Vector3.down * (bs._GameSettings.gravitationFactor - 1f) * 10f, ForceMode.Acceleration);
			}
			Vector3 vel = this.vel;
			if (vel.y > 15f)
			{
				this.vel *= 0.999f;
			}
		}
	}

	public void OnCollisionEnter(Collision collisionInfo)
	{
		if (bs.online || (!bs._Loader.enableCollision && !bs.splitScreen) || dead)
		{
			return;
		}
		Player component = collisionInfo.transform.root.GetComponent<Player>();
		if (component != null && !component.dead && !component.finnished)
		{
			HitMesh(collisionInfo, 10, 10);
			component.HitMesh(collisionInfo, 20, 20);
			component.GhostDie(collisionInfo);
			if (!base.get_audio().isPlaying)
			{
				PlayHitSound(b: true);
			}
		}
		vel = oldVel;
		rigidbody.angularVelocity = oldAng;
	}

	private void OnTriggerEnter(Collider other)
	{
		if (bs._Loader.enableZombies)
		{
			Zombie component = other.transform.root.GetComponent<Zombie>();
			if ((bool)component && !component.dead)
			{
				component.Hit();
			}
		}
	}

	public void OnCollisionStay(Collision collisionInfo)
	{
		if (isKinematic || bs._Game.backTime || dead || ghost || froozen || collisionInfo.collider.tag == "zombie")
		{
			return;
		}
		groundedTime2 = Time.time;
		OnCollisionEnter2(collisionInfo);
		OnBorderCheck(collisionInfo);
		if (groundHit)
		{
			grounded2 = true;
			groundedTime = Time.time;
		}
		else if (Time.time - groundedTime > 0.2f)
		{
			float num = 0.99f;
			upsideDown = Time.time;
			if (UpsideDown)
			{
				Vector3 vel = this.vel;
				float x = vel.x * num;
				Vector3 vel2 = this.vel;
				float y = vel2.y;
				Vector3 vel3 = this.vel;
				this.vel = new Vector3(x, y, vel3.z * num);
			}
		}
		if (!angleTest && bs.setting.enableDrag)
		{
			float magnitude = (this.vel - oldVel2).magnitude;
			rigidbody.angularDrag = Mathf.Min(rigidbody.angularDrag, Mathf.Max(0f, 20f - magnitude));
		}
	}

	public void HitMesh(Collision col, int cnt, int max)
	{
		if (!bs.lowQualityAndAndroid)
		{
			ContactPoint[] contacts = col.contacts;
			if ((bool)meshTest && cnt > 0 && Time.time - meshHitTime > 0.5f)
			{
				MonoBehaviour.print("hitForce;" + cnt);
				meshHitTime = Time.time;
				ContactPoint contactPoint = contacts[0];
				meshTest.Hit(contacts[0].point, vel, cnt, max);
				meshTest.Damage(contactPoint.point, contactPoint.normal);
			}
		}
	}

	public void OnCollisionEnter2(Collision collisionInfo)
	{
		if (isKinematic || bs._Game.backTime || dead)
		{
			return;
		}
		float magnitude = (vel - oldVel2).magnitude;
		float magnitude2 = (collisionInfo.relativeVelocity + rigidbody.velocity).magnitude;
		hitForce += magnitude;
		if (!bs._Loader.dm)
		{
			Crash(magnitude, collisionInfo);
		}
		magnitude = Mathf.Max(magnitude2, magnitude);
		if (magnitude > 15f && Time.time - hitTime > 0.1f)
		{
			hitTime = Time.time;
			ContactPoint[] contacts = collisionInfo.contacts;
			for (int i = 0; i < contacts.Length; i++)
			{
				ContactPoint contactPoint = contacts[i];
				Sparks(contactPoint.point, contactPoint.normal);
			}
			if (!base.get_audio().isPlaying || !bs.res.hitSoundBig.Contains(base.get_audio().clip))
			{
				PlayHitSound(b: true);
			}
		}
	}

	private void Crash(float magnitude, Collision collisionInfo)
	{
		if (magnitude > 20f && !dead)
		{
			if (crCount < 3)
			{
				crCount++;
				MonoBehaviour.print("Crash detected " + magnitude + " " + crCount);
				Rewind(crCount + 1, skip: true);
				crashTime = Time.time;
			}
			else if (magnitude > 40f * (bs._GameSettings.gravitationAntiFly + bs._GameSettings.gravitationFactor) / 2f)
			{
				MonoBehaviour.print(magnitude);
				meshHitTime = -1f;
				HitMesh(collisionInfo, 20, 100);
				Die();
			}
		}
		if (Time.time - crashTime > 0.1f)
		{
			crCount = 0;
		}
	}

	private void Die(bool water = false, int killer = -1)
	{
		if (!dead)
		{
			if (bs._Loader.dm)
			{
				CallRPC(Die2, (killer != -1) ? killer : playerId);
			}
			vel = oldVel2;
			if (water)
			{
				bs.PlayOneShotGui(bs.res.waterSplash);
			}
			else
			{
				CallRPC(Explode, base.pos);
			}
			dead = true;
			if (bs._Loader.dm)
			{
				StartCoroutine(AddMethodCor(5f, Reset));
			}
			MonoBehaviour.print("Bum");
		}
	}

	private void BlackReset()
	{
		for (int i = 0; i < renderers.Length; i++)
		{
			renderers[i].sharedMaterials = materials[i];
		}
	}

	private void BlackCar()
	{
		Renderer[] array = renderers;
		foreach (Renderer renderer in array)
		{
			Material[] array2 = new Material[renderer.materials.Length];
			for (int j = 0; j < renderer.materials.Length; j++)
			{
				Material material = array2[j] = new Material(renderer.materials[j]);
				material.shader = bs.res.diffuse;
				material.color = Color.black;
			}
			renderer.materials = array2;
		}
	}

	private void OnBorderCheck(Collision collisionInfo)
	{
		if (ghost || collisionInfo.collider.tag == "HitBox" || lastCheckFrame == Time.frameCount)
		{
			return;
		}
		lastCheckFrame = Time.frameCount;
		for (int i = 0; i < collisionInfo.contacts.Length; i++)
		{
			ContactPoint contactPoint = collisionInfo.contacts[i];
			float num = Vector3.Angle(transform.up, contactPoint.normal);
			if (Application.isEditor && num > 30f)
			{
				Debug.DrawRay(contactPoint.point, contactPoint.normal, Color.red, 10f);
			}
			bool flag = collisionInfo.collider is TerrainCollider;
			if (!(num > 40f * ((!flag) ? 1f : 0.3f)))
			{
				continue;
			}
			float magnitude = oldVel2.magnitude;
			rigidbody.angularDrag = 0f;
			if ((bool)meshTest)
			{
				float magnitude2 = collisionInfo.relativeVelocity.magnitude;
				if (magnitude2 > 15f)
				{
					HitMesh(collisionInfo, Mathf.Min(5, (int)((magnitude2 - 20f) / 5f)), 20);
				}
			}
			if (bs.setting.enableDrag)
			{
				vel = Vector3.MoveTowards(vel, Vector3.zero, 0.05f);
			}
			if (tempScore > 100f)
			{
				MonoBehaviour.print("HIT>>>>>>>>>>>>>>>>>>" + num);
			}
			tempScore = 0f;
			if (magnitude > 30f && (bs._GameSettings.enableBorderHit || bs._Loader.bombCar) && Time.time - lastBorderHit > 0.1f)
			{
				if (!flag)
				{
					Sparks(contactPoint.point, contactPoint.normal);
				}
				if (Application.isEditor)
				{
					MonoBehaviour.print("angle " + num + " vel " + magnitude * 4f);
				}
				if (!base.get_audio().isPlaying)
				{
					PlayHitSound(b: false);
				}
				lastBorderHit = Time.time;
				rigidbody.AddForceAtPosition(contactPoint.normal * Mathf.Min(velm, 30f) * ((!angleTest) ? 40f : (40f / rigidbody.angularVelocity.magnitude)), rigidbody.worldCenterOfMass + transform.forward * 1f);
				vel *= 0.8f;
				if (bs._Loader.bombCar && !bs._Loader.dm)
				{
					Die();
				}
				break;
			}
		}
	}

	public void Sparks(Vector3 point, Vector3 normal)
	{
		if (Time.time - sparkTime > 0.1f)
		{
			bs._Game.Emit(point, normal, bs._Game.sparks);
			sparkTime = Time.time;
		}
	}

	private void UpdateInput()
	{
		//IL_024a: Unknown result type (might be due to invalid IL or missing references)
		//IL_0254: Expected O, but got Unknown
		if (!visible)
		{
			return;
		}
		down = (brake = (up = false));
		bool key = GetKey(KeyCode.S);
		bool flag = GetKey(KeyCode.W);
		if (bs.android)
		{
			flag = (flag || GetKey(KeyCode.LeftShift));
		}
		if (key)
		{
			flag = false;
		}
		if (!bs._Game.started || GetKey(KeyCode.Space))
		{
			brake = true;
		}
		else if (key || flag)
		{
			if (key && (velm < 5f || movingBack))
			{
				down = !engineOff;
			}
			else if (flag && (velm < 5f || !movingBack))
			{
				up = !engineOff;
			}
			else
			{
				brake = true;
			}
		}
		if (flashMouse == 0f)
		{
			left = GetKey(KeyCode.A);
			right = GetKey(KeyCode.D);
		}
		if (GetKeyDown(KeyCode.U))
		{
			flashLight.gameObject.SetActive(!flashLight.gameObject.activeSelf);
		}
		nitroDown = (((float)nitro > 0f || ghost) && GetKey(KeyCode.LeftShift));
		if (nitroDown)
		{
			speed = 3.02f;
			nitro = (float)nitro - Time.deltaTime;
		}
		else if (speed == 3.02f)
		{
			speed = 0f;
		}
		if (!ghost && GetKeyDown(KeyCode.X) && grounded && Time.time - lastJump > 1f && (bs._Loader.dm || bs.isDebug))
		{
			lastJump = Time.time;
			CallRPC((Action)(object)new Action(Jump));
			rigidbody.AddForceAtPosition(Vector3.up * 1500f, rigidbody.worldCenterOfMass + transform.forward * 3f);
		}
	}

	[RPC]
	private void Jump()
	{
		PlayOneShot(bs.res.jump, 1f);
	}

	private void PlayHitSound(bool b)
	{
		AudioClip[] array = (!b) ? bs.res.hitSound : bs.res.hitSoundBig;
		base.get_audio().clip = array[UnityEngine.Random.Range(0, array.Length)];
		base.get_audio().Play();
	}

	internal void ChangeCamera(int cam)
	{
		curCam = cam;
		curCam %= camPoses.Length;
		model.gameObject.SetActive(!fps);
		camera.transform.parent = camPoses[curCam];
		camera.transform.localPosition = Vector3.zero;
		camera.transform.localRotation = Quaternion.identity;
	}

	private void UpdateTimeRevert()
	{
		if (ghost || bs._Loader.dm || !bs._Game.started || finnished)
		{
			return;
		}
		if ((input.GetKeyDown(KeyCode.W) || input.GetKeyDown(KeyCode.S) || input.GetKeyDown(KeyCode.A) || input.GetKeyDown(KeyCode.D)) && bs._Game.backTime)
		{
			Base.ClearLog();
			bs._Game.SendWmp("goto", (bs._Game.timeElapsed + bs._GameSettings.sendWmpOffset).ToString());
			bs._Game.backTime = false;
			if (rewinds >= 0)
			{
				replay.backupRetries++;
			}
			if (bs._Game.rewindingPlayer != null)
			{
				bs._Game.rewindsUsed++;
				bs._Game.rewindingPlayer.rewinds--;
			}
			KeyDown[] array = replay.keyDowns.ToArray();
			foreach (KeyDown keyDown in array)
			{
				if (keyDown.time >= bs._Game.timeElapsedLevel)
				{
					replay.keyDowns.Remove(keyDown);
				}
			}
		}
		int num = Mathf.Max(1, (int)(deltaTime * 100f));
		if (!bs._Loader.dm && GetKey(KeyCode.Backspace) && posVels.Count > num && rewinds > 0)
		{
			if (backTimeAudio != null && !backTimeAudio.isPlaying)
			{
				backTimeAudio.Play();
			}
			if (!bs._Game.backTime)
			{
				bs.PlayOneShotGui(bs._Loader.click);
				if ((bool)meshTest)
				{
					meshTest.Reset();
				}
			}
			bs._Game.backTime = true;
			if (bs._Loader.rain)
			{
				bs._Game.rainfall.Simulate(deltaTime);
			}
			bs._Player.Rewind(num);
			if (bs.splitScreen)
			{
				bs._Player2.Rewind(num);
			}
			bs._Game.rewindingPlayer = this;
		}
		else if (backTimeAudio != null && backTimeAudio.isPlaying)
		{
			backTimeAudio.Stop();
		}
		if (bs._Game.backTime)
		{
			bs._Game.centerText(string.Format(GuiClasses.Tr("Hold {0} button to rewind time, Press {1} to resume"), GuiClasses.Tr((!bs.android) ? "e key" : "this"), GuiClasses.Tr((!bs.android) ? "any key" : "Acelerate")), 0.1f, noAndroid: false);
		}
	}

	private void Rewind(int cut, bool skip = false)
	{
		if (bs._Loader.quality != 0 && posVels.Count >= 1)
		{
			if (!skip)
			{
				posVels.RemoveRange(posVels.Count - cut, cut);
			}
			PosVel posVel = posVels[posVels.Count - 1];
			if (!skip)
			{
				base.pos = posVel.pos;
				cam.position = posVel.camPos;
				cam.rotation = posVel.camRot;
			}
			rot = posVel.rot;
			lap = posVel.lap;
			dead = false;
			rotVel = posVel.mouserot;
			groundedTime = posVel.groundTime;
			nitro = posVel.nitro;
			Vector3 vector = this.vel = posVel.vel;
			oldVel = (oldVel2 = vector);
			engineOff = posVel.engineOff;
			totalMeters = posVel.meters;
			checkPointsPass = posVel.checkPointsPass;
			rigidbody.angularVelocity = posVel.angVel;
			velSmooth = posVel.velSmooth;
			if (!bs.online && !bs.setting.timeLapse)
			{
				bs._Game.timeElapsedLevel = posVel.time;
			}
			bs._Game.FrameCount = posVel.frameCount;
			upAngle = Vector3.Angle(transform.up, Vector3.up);
			hitForce = 0f;
		}
	}

	private static void ShowCenterTextReset()
	{
		if (!bs._Loader.dm)
		{
			bs._Game.centerText(GuiClasses.Tr((!bs.android) ? "Press R Key to Restart, Hold E key to rewind time" : "Go to Menu->Restart"), 0.1f, noAndroid: false);
		}
	}

	private void UpdateTriggers()
	{
		for (int i = 0; i < bs._Game.triggers.Count; i++)
		{
			Collider collider = bs._Game.triggers[i];
			bool flag = ((Component)collider).get_collider().bounds.Contains(base.pos);
			if (!triggerColOld[i] && flag && collider.tag == "CheckPoint")
			{
				flag = (Vector3.Dot((base.pos - collider.transform.position).normalized, transform.forward) > 0f);
			}
			if (triggerColOld[i] != flag)
			{
				if (flag)
				{
					OnTriggerEnter2(collider);
				}
				else
				{
					OnTriggerExit2(collider);
				}
			}
			triggerColOld[i] = flag;
		}
	}

	private void FallCheck()
	{
		Vector3 pos = base.pos;
		if (pos.y < bs._GameSettings.miny && !ghost && !dead)
		{
			Die(water: true);
			if (!bs._Loader.stunts)
			{
				CallRPC(SetScore2, (float)score - 5f);
			}
		}
	}

	public Vector3 ZeroY2(Vector3 v)
	{
		if (!(v.y < 0f) && velm < 25f)
		{
			v.y = 0f;
		}
		return v;
	}

	public bool GetKeyUp(KeyCode key)
	{
		if (ghost)
		{
			return replay.OnKeyUp[(int)key];
		}
		return input.GetKeyUp(key);
	}

	private bool GetKey(KeyCode key)
	{
		if (ghost)
		{
			return replay.OnKey[(int)key];
		}
		return input.GetKey(key);
	}

	private bool GetKeyDown(KeyCode key)
	{
		if (ghost)
		{
			return replay.OnKeyDown[(int)key];
		}
		return input.GetKeyDown(key);
	}

	public void OnTriggerExit2(Collider other)
	{
		if (other.tag == "speed")
		{
			speed = 0f;
		}
	}

	public void OnTriggerEnter2(Collider other)
	{
		if (!bs._Loader.dm && other.tag == "engineOff")
		{
			engineOff = true;
		}
		if (other.tag == "speed")
		{
			base.get_audio().clip = bs.res.speedUp;
			base.get_audio().Play();
			Item component = other.GetComponent<Item>();
			speed = ((!(component == null)) ? component.speed : 10f);
		}
		if (other.tag == "CheckPoint")
		{
			engineOff = false;
		}
		if (!bs._Game.backTime && !bs._Loader.dm && other.tag == "CheckPoint" && !checkPointsPass.Contains(other.transform) && (bs._Game.timeElapsed > 20f || other.name != "Finnish" || checkPointsPass.Count == bs.checkPoints.Length - 1))
		{
			OnFinnishedCheckpoint(other);
		}
	}

	private void UpdateSounds()
	{
		if (!visible)
		{
			return;
		}
		AudioClip y = (!dirtMaterial) ? bs.res.brake : bs.res.mudBrake;
		if (oldStopAudio != y)
		{
			stopAudio.clip = (oldStopAudio = y);
			stopAudio.Play();
		}
		float num = velm * 4f % 70f;
		if (!ghost && oldSpeed - num > 10f && velm > oldVel.magnitude)
		{
			PlayOneShot(bs.res.gearChange[UnityEngine.Random.Range(0, bs.res.gearChange.Length)], 0.5f);
			suspendTime = Time.time;
		}
		oldSpeed = num;
		bool flag = up || down;
		bool flag2 = Time.time - suspendTime < 1f / velm && velm > 1f;
		motorAudio.volume = Mathf.Lerp(motorAudio.volume, (up && !bs._Game.started) ? 1f : ((!flag && !down) ? 0.5f : Mathf.Min(1f, velm / 5f)), Time.deltaTime * 10f);
		idleAudio.volume = ((!ghost) ? (1f - motorAudio.volume) : 0f);
		float num2 = bs.res.gears.Evaluate(velm * 4f / 70f / 10f);
		if (grounded && bs._Game.started)
		{
			motorAudio.pitch = num2 + num / (float)((!flag) ? 140 : 70) * (2f - num2 * 2f) + velm * 0.005f;
		}
		else
		{
			motorAudio.pitch = Mathf.Lerp(motorAudio.pitch, (!up && !down) ? 0.1f : (1.5f + velm * 0.005f), Time.deltaTime);
		}
		if (windAudio != null)
		{
			windAudio.volume = ((!finnished) ? (Mathf.Max(0f, velm - 70f) / 70f) : 0f);
		}
		if (bs._Game.started && !ghost)
		{
			skid = skid2;
			float volume = (grounded && !(velm < 2f)) ? Mathf.Max(0f, skid) : 0f;
			stopAudio.volume = volume;
			skid = volume;
			if (velm < (float)minVelForSkid && !brake && up && !ghost && bs._Game.started)
			{
				volume = 1f;
				stopAudio.volume = volume;
				skid = volume;
			}
			if (brake && velm > (float)minVelForSkid && grounded)
			{
				volume = 1f;
				stopAudio.volume = volume;
				skid = volume;
			}
		}
		if (!bs._Game.started || bs._Game.editControls || finnished || dead || bs._Game.backTime2)
		{
			AudioSource audioSource = hornAudio;
			float volume = 0f;
			stopAudio.volume = volume;
			volume = volume;
			idleAudio.volume = volume;
			volume = volume;
			motorAudio.volume = volume;
			audioSource.volume = volume;
			nitroAudio.Stop();
			if (windAudio != null)
			{
				windAudio.volume = 0f;
			}
		}
		if (engineOff || flag2)
		{
			AudioSource audioSource2 = idleAudio;
			float volume = 0f;
			motorAudio.volume = volume;
			audioSource2.volume = volume;
		}
	}

	public void PlayOneShot(AudioClip AudioClip, float volume = 1f)
	{
		if (checkVis(base.pos))
		{
			base.get_audio().PlayOneShot(AudioClip, volume * bs._Loader.soundVolume);
		}
	}

	private void UpdateCamPos()
	{
		if (finnished || dead || (!firstPlayer && !secondPlayer && bs._Loader.sGameType != SGameType.Replay))
		{
			return;
		}
		if (topdown)
		{
			cam.position = Vector3.Lerp(cam.position, base.pos, Time.deltaTime * 10f);
			Transform transform = Camera.main.transform;
			cam.transform.localRotation = Quaternion.identity;
			Vector3 forward = this.transform.forward;
			transform.localPosition = Vector3.Lerp(transform.localPosition, new Vector3(forward.x, forward.z, -1.5f) * Mathf.Clamp(velm * 1f, 30f, 100f), Time.deltaTime * 3f);
			if (camHited)
			{
				Renderer renderer = ((Component)camHit.collider).get_renderer();
				if (!bs.Nancl && renderer != null && !bs.res.levelMaterialsTxt.Contains(renderer.sharedMaterial.name))
				{
					renderer.material.shader = bs.res.transparment;
					Color color = renderer.material.color;
					color.a = 0.5f;
					renderer.material.color = color;
				}
			}
		}
		if (!topdown)
		{
			Vector3 position = Vector3.Lerp(cam.position, base.pos, Time.deltaTime * 60f);
			if (bs._Game.isCustomLevel)
			{
				Vector3 position2 = bs._MapLoader.water.transform.position;
				position.y = Mathf.Max(position2.y, position.y);
			}
			cam.position = position;
			bool key = input.GetKey(KeyCode.Q);
			if (key)
			{
				cam.Rotate(Vector3.up, 180f);
			}
			if (UpsideDown)
			{
				cam.Rotate(Vector3.forward, 180f);
			}
			cam.rotation = Quaternion.Lerp(cam.rotation, rot, Time.deltaTime * (3f + Quaternion.Angle(cam.rotation, rot) / 10f));
			UpdateCamDm();
			if (UpsideDown)
			{
				cam.Rotate(Vector3.forward, 180f);
			}
			if (key)
			{
				cam.Rotate(Vector3.up, 180f);
			}
			if (!ghost && camHited)
			{
				cam.position = camHit.point - (camera.transform.position - cam.position) + camHit.normal * 0.01f;
			}
		}
	}

	public int Compare(Player a, Player b)
	{
		if (bs._Loader.dmOrCoins)
		{
			if ((float)b.score > (float)a.score)
			{
				return 1;
			}
			if ((float)b.score < (float)a.score)
			{
				return -1;
			}
		}
		float num = (!bs.online) ? a.finnishTime : a.replay.finnishTime;
		float num2 = (!bs.online) ? b.finnishTime : b.replay.finnishTime;
		float num3 = (num != 0f) ? num : float.MaxValue;
		float num4 = (num2 != 0f) ? num2 : float.MaxValue;
		if (num3 > num4)
		{
			return 1;
		}
		if (num3 < num4)
		{
			return -1;
		}
		if (a.lap < b.lap)
		{
			return 1;
		}
		if (a.lap > b.lap)
		{
			return -1;
		}
		int count = a.checkPointsPass.Count;
		int count2 = b.checkPointsPass.Count;
		if (count > count2)
		{
			return -1;
		}
		if (count < count2)
		{
			return 1;
		}
		float num5 = a.totalMeters - a.checkPointMeters;
		float num6 = b.totalMeters - b.checkPointMeters;
		if (num5 > num6)
		{
			return -1;
		}
		if (num5 < num6)
		{
			return 1;
		}
		if (a.playerNameClan != b.playerNameClan)
		{
			return a.playerNameClan.CompareTo(b.playerNameClan);
		}
		return a.GetHashCode().CompareTo(b.GetHashCode());
	}

	protected void VoiceChatSend()
	{
	}

	[RPC]
	public void SendAudio(byte[] data, int length, int id)
	{
		replay.voiceChatTime = Time.realtimeSinceStartup;
		if (!((double)bs._Loader.voiceChatVolume < 0.01))
		{
			voiceChatPlayer.audio.volume = bs._Loader.voiceChatVolume;
			if (!voiceChatFirst)
			{
				voiceChatFirst = true;
				bs._Game.centerText(GuiClasses.Tr("To use voice chat, press Y"), 2f);
			}
			voiceChatPlayer.OnNewSample(new VoiceChatPacket
			{
				Compression = VoiceChatCompression.Speex,
				Data = data,
				Length = length,
				NetworkId = id
			});
		}
	}

	public IEnumerator Reset()
	{
		if (bs._Loader.enableCollision && ghost)
		{
			UnityEngine.Object.Destroy(rigidbody);
			carBox.enabled = false;
			if (oldPosVels != null)
			{
				replay.posVels = new List<PosVel>(oldPosVels);
			}
		}
		if (IsMine && bs._Loader.dm)
		{
			CallRPC((Action<PhotonMessageInfo>)Reset2);
		}
		tempScore = (oldTempScore = 0f);
		Vector3 vector2;
		if (!test)
		{
			vector2 = (oldpos = (base.pos = bs._Game.StartPos.position + bs._Game.startPosOffset));
			rot = bs._Game.StartPos.rotation;
		}
		dead = false;
		if (!bs.online)
		{
			score = 0f;
		}
		hitForce = 0f;
		nitro = ((!bs._Game.isCustomLevel) ? ((float)((bs._Loader.curScene != null) ? bs._Loader.curScene.nitro : 0)) : bs._MapLoader.nitro);
		if (bs.isDebug)
		{
			nitro = 999f;
		}
		if ((float)nitro > 0f && !ghost)
		{
			bs._Game.centerText(string.Format(GuiClasses.Tr("You have {0} seconds long Nitro, Press Shift to Use"), nitro), 3f);
		}
		speed = 0f;
		keys = new bool[330];
		replay.OnKey = new bool[500];
		up = (down = (left = (right = false)));
		flashMouse = 0f;
		rewinds = ((!bs._Loader.stunts || bs.isDebug) ? (bs.online ? bs._Loader.rewinds : ((bs._Loader.levelEditor != null || bs.isDebug) ? 999 : ((!base.easy || bs.splitScreen) ? 3 : 60))) : 0);
		engineOff = false;
		grounded = true;
		totalMeters = 0f;
		if (!bs.online)
		{
			finnishTime = 0f;
		}
		finnished = false;
		checkPointsPass.Clear();
		checkPointMeters = 0f;
		if (hud != null)
		{
			TextMesh checkPointRed = hud.checkPointRed;
			string empty = string.Empty;
			hud.checkPoint.text = empty;
			checkPointRed.text = empty;
		}
		grounded = (grounded2 = (groundHit = true));
		lap = 1;
		if (SkidMarks != null)
		{
			SkidMarks.Reset();
		}
		vector2 = (vel = Vector3.zero);
		oldVel2 = (oldVel = vector2);
		if (rigidbody != null)
		{
			Rigidbody obj = rigidbody;
			vector2 = Vector3.zero;
			rigidbody.velocity = vector2;
			obj.angularVelocity = vector2;
		}
		foreach (GameObject a in fx)
		{
			UnityEngine.Object.Destroy(a);
		}
		fx.Clear();
		if ((bool)meshTest)
		{
			meshTest.Reset();
		}
		oldRot = transform.eulerAngles;
		fireTime = float.MinValue;
		yield return null;
	}

	public void UpdateText()
	{
		if (!ghost)
		{
			return;
		}
		bool flag = !bs.lowQuality || bs._Loader.dm;
		float magnitude = (bs._Player.pos - base.pos).magnitude;
		int num = 200;
		if ((playerNameTxt.enabled || FramesElapsed(10)) && flag)
		{
			playerNameTxt.transform.position = bs._Player.camera.WorldToViewportPoint(base.pos + Vector3.up) + Vector3.up * 0.05f;
			Vector3 position = playerNameTxt.transform.position;
			if (position.z < 0f)
			{
				Vector3 position2 = playerNameTxt.transform.position;
				position2.y = 0.1f;
				position2.x = Mathf.Clamp(1f - position2.x, 0.2f, 0.7f);
				playerNameTxt.transform.position = position2;
			}
			playerNameTxt.fontSize = (int)Mathf.Lerp(16f, 8f, magnitude / (float)num);
		}
		playerNameTxt.enabled = flag;
		if ((!bs._Loader.dm || !bs._Game.allyVisible.Contains(playerId)) && !sameTeam && !haveFlag && !(Time.time - bs._Player.spawnTime < 7f))
		{
			playerNameTxt.enabled = (playerNameTxt.enabled && (magnitude < (float)num || Time.time - showTextTime < 3f) && (!bs._Loader.dm || !Physics.Linecast(bs._Player.camera.transform.position, base.pos, Layer.levelMask)));
		}
		if (bs._Loader.dm)
		{
			playerNameTxt2.color = ((team != 0) ? Color.blue : Color.red);
		}
	}

	private void FixedUpdateGhost()
	{
		if (dead && !bs._Game.backTime2 && bs._Loader.enableCollision && bs._Game.FrameCount % bs._Game.frameInterval == 0 && bs._Game.started && base.quality != 0)
		{
			PosVel item = CrPosVel();
			posVels.Insert(bs._Game.FrameCount / bs._Game.frameInterval, item);
		}
	}

	private void GhostDie(Collision collisionInfo)
	{
		if (!rigidbody)
		{
			dead = true;
		}
	}

	private void UpdateGhost()
	{
		if (!ghost || test)
		{
			return;
		}
		if (bs._Loader.enableCollision)
		{
			if (dead && !bs._Game.backTime2)
			{
				if (!bs.lowQuality && Mathf.Abs(Vector3.Dot(transform.forward, rigidbody.velocity.normalized)) < 0.5f)
				{
					SkidSmoke(0);
					SkidSmoke(1);
				}
				if (!visible2 && bs._Game.timeElapsed - deathTime > 2f)
				{
					dead = false;
				}
				return;
			}
			carBox.enabled = (bs._Game.timeElapsed > 2f && ((base.pos - bs._Player.pos).magnitude > 8f || carBox.enabled) && !finnished);
		}
		rotVel = Mathf.Lerp(rotVel, left ? (-3f) : ((!right) ? 0f : 3f), deltaTime * 10f);
		if (bs.online)
		{
			double time = PhotonNetwork.time;
			double num = time - (double)interpolationBackTime;
			if (m_BufferedState[0].timestamp > num)
			{
				for (int i = 0; i < m_TimestampCount; i++)
				{
					if (m_BufferedState[i].timestamp <= num || i == m_TimestampCount - 1)
					{
						State state = m_BufferedState[Mathf.Max(i - 1, 0)];
						State state2 = m_BufferedState[i];
						double num2 = state.timestamp - state2.timestamp;
						float t = 0f;
						if (num2 > 0.0001)
						{
							t = (float)((num - state2.timestamp) / num2);
						}
						transform.localPosition = ((!(Vector3.Distance(state2.pos, state.pos) > 50f)) ? Vector3.Lerp(state2.pos, state.pos, t) : state.pos);
						transform.localRotation = Quaternion.Slerp(state2.rot, state.rot, t);
						return;
					}
				}
			}
			else
			{
				State state3 = m_BufferedState[0];
				transform.localPosition = Vector3.Lerp(transform.localPosition, state3.pos, deltaTime * 20f);
				transform.localRotation = state3.rot;
			}
			foreach (KeyCode recordKey in bs.recordKeys)
			{
				int num3 = (int)recordKey;
				replay.OnKeyDown[num3] = false;
				replay.OnKeyUp[num3] = false;
			}
			return;
		}
		float num4 = bs._Game.FrameCount;
		int num5 = (int)(num4 / (float)bs._Game.frameInterval);
		float t2 = num4 / (float)bs._Game.frameInterval - (float)num5;
		num5 = Mathf.Min(posVels.Count - 1, num5);
		if (num5 >= posVels.Count - 1 && !finnished)
		{
			EndGame();
		}
		if (num5 < posVels.Count && num5 != (int)((num4 - 1f) / (float)bs._Game.frameInterval))
		{
			vel = posVels[num5].vel;
			if (!bs._Game.backTime2)
			{
				stopAudio.volume = posVels[num5].skid;
			}
			totalMeters = posVels[num5].meters;
			score = posVels[num5].score;
		}
		if (num5 < posVels.Count - 1 && num5 != 0)
		{
			base.pos = Vector3.Lerp(posVels[num5 - 1].pos, posVels[num5].pos, t2);
			rot = Quaternion.Lerp(posVels[num5 - 1].rot, posVels[num5].rot, t2);
		}
		foreach (KeyCode recordKey2 in bs.recordKeys)
		{
			replay.OnKeyDown[(int)recordKey2] = false;
			replay.OnKeyUp[(int)recordKey2] = false;
		}
		foreach (KeyDown keyDown in replay.keyDowns)
		{
			if (keyDown.time >= oldTimeElapsed && keyDown.time < bs._Game.timeElapsedLevel)
			{
				if (bs._Loader.inputManger.KeyBack.ContainsKey(keyDown.keyCode))
				{
					int num6 = (int)bs._Loader.inputManger.KeyBack[keyDown.keyCode];
					replay.OnKeyDown[num6] = keyDown.down;
					replay.OnKeyUp[num6] = !keyDown.down;
					replay.OnKey[num6] = keyDown.down;
				}
				else
				{
					MonoBehaviour.print("keynotfound " + keyDown.keyCode);
				}
			}
		}
		if (ghost && Physics.Linecast(base.pos + transform.up * 2f, base.pos, out RaycastHit hitInfo, Layer.levelMask))
		{
			base.pos = hitInfo.point;
		}
		oldTimeElapsed = bs._Game.timeElapsedLevel;
	}

	private void UpdateRecordBackupReplay()
	{
		if (bs._Game.FrameCount % bs._Game.frameInterval == 0 && bs._Game.started && base.quality != 0)
		{
			posVels.Add(CrPosVel());
		}
	}

	private PosVel CrPosVel()
	{
		PosVel posVel = new PosVel();
		posVel.camRot = cam.rotation;
		posVel.camPos = cam.position;
		posVel.pos = transform.position;
		posVel.rot = transform.rotation;
		posVel.vel = vel;
		posVel.angVel = rigidbody.angularVelocity;
		posVel.time = bs._Game.timeElapsedLevel;
		posVel.frameCount = bs._Game.FrameCount;
		posVel.meters = totalMeters;
		posVel.mouserot = rotVel;
		posVel.checkPointsPass = new List<Transform>(checkPointsPass);
		posVel.lap = lap;
		posVel.groundTime = groundedTime;
		posVel.engineOff = engineOff;
		posVel.skid = stopAudio.volume;
		posVel.nitro = nitro;
		posVel.velSmooth = velSmooth;
		posVel.score = (int)(float)score;
		return posVel;
	}

	private void UpdateRecordBackupReplay2()
	{
		if (ghost)
		{
			return;
		}
		for (int i = 0; i < bs.recordKeys.Count; i++)
		{
			KeyCode keyCode = bs.recordKeys[i];
			bool flag = (input.GetKey(keyCode) && (keyCode != KeyCode.LeftShift || (float)nitro > 0f)) || (keyCode == KeyCode.LeftShift && speed > 0f);
			if (keys[(int)keyCode] != flag)
			{
				keys[(int)keyCode] = flag;
				CallRPC((Action<int, bool>)SetKey, (int)keyCode, flag);
				replay.keyDowns.Add(new KeyDown
				{
					time = bs._Game.timeElapsedLevel,
					down = keys[(int)keyCode],
					keyCode = keyCode
				});
			}
		}
	}

	[RPC]
	public void SetKey(int keycode, bool down)
	{
		if (down)
		{
			replay.OnKeyDown[keycode] = true;
			replay.OnKey[keycode] = true;
		}
		if (!down)
		{
			replay.OnKeyUp[keycode] = true;
			replay.OnKey[keycode] = false;
		}
	}

	private void OnFinnishedCheckpoint(Collider other)
	{
		//IL_0122: Unknown result type (might be due to invalid IL or missing references)
		//IL_012c: Expected O, but got Unknown
		if (!ghost)
		{
			string text = bs._Loader.prefixMapPl + lap + ":" + checkPointsPass.Count;
			float num = Base.PlayerPrefsGetFloat(text, float.MaxValue);
			if (bs._Game.timeElapsed2 < num && !ghost)
			{
				Base.PlayerPrefsSetFloat(text, bs._Game.timeElapsed2);
			}
			hud.checkPoint.text = hud.time.text;
			((Component)hud.checkPointRed).get_renderer().material.color = ((!(bs._Game.timeElapsed2 > num)) ? Color.blue : Color.red);
			hud.checkPointRed.text = ((num != float.MaxValue) ? bs.TimeToStr(bs._Game.timeElapsed2 - num, draw: true) : string.Empty);
			StartCoroutine(Base.AddMethod(2f, (Action)(object)(Action)delegate
			{
				TextMesh checkPointRed = hud.checkPointRed;
				string empty = string.Empty;
				hud.checkPoint.text = empty;
				checkPointRed.text = empty;
			}));
			bs.PlayOneShotGui(bs.res.checkPoint);
		}
		checkPointMeters = totalMeters;
		checkPointsPass.Add(other.transform);
		if (checkPointsPass.Count == bs.checkPoints.Length && bs.checkPoints.Length > 0)
		{
			if (lap >= bs._GameSettings.laps && !ghost)
			{
				EndGame();
			}
			else
			{
				checkPointsPass.Clear();
			}
			lap++;
		}
	}

	public void EndGame()
	{
		if (!bs._Loader.dm)
		{
			finnished = true;
		}
		if (bs._Loader.enableCollision && ghost)
		{
			carBox.enabled = false;
		}
		finnishTime = bs._Game.timeElapsed2;
		if (replay.finnishTime > finnishTime || replay.finnishTime == 0f)
		{
			replay.finnishTime = finnishTime;
			if (bs.online)
			{
				CallRPC(SetFinnishTime, finnishTime);
			}
		}
		bs._Game.finnishedPlayers.Add(this);
		if (!ghost)
		{
			bs._Game.EndGame(this);
		}
	}

	private AudioSource InitSound(AudioClip audioClip, bool play = true)
	{
		AudioSource audioSource = base.gameObject.AddComponent<AudioSource>();
		audioSource.clip = audioClip;
		audioSource.loop = true;
		dopler(audioSource);
		if (this == bs._Player)
		{
			audioSource.priority = 0;
		}
		if (play)
		{
			audioSource.Play();
			audioSource.volume = 0f;
		}
		return audioSource;
	}

	private void dopler(AudioSource au)
	{
		au.dopplerLevel = (ghost ? 1 : 0);
	}

	private static List<Transform> GetTrs(Transform tr)
	{
		List<Transform> list = new List<Transform>();
		foreach (Transform item in tr)
		{
			list.Add(item);
		}
		return list;
	}

	public void OnDestroy()
	{
		if (!Loader.loadingLevelQuit)
		{
			if (hud != null)
			{
				UnityEngine.Object.Destroy(hud.gameObject);
			}
			if (androidHud != null)
			{
				UnityEngine.Object.Destroy(androidHud.gameObject);
			}
			if (input != null && input != bs._Loader.inputManger)
			{
				UnityEngine.Object.Destroy(input.gameObject);
			}
			if (cam != null)
			{
				UnityEngine.Object.Destroy(cam.gameObject);
			}
			if (SkidMarks != null)
			{
				UnityEngine.Object.Destroy(SkidMarks.gameObject);
			}
			if (playerNameTxt != null)
			{
				UnityEngine.Object.Destroy(playerNameTxt.gameObject);
			}
			if (bs._Game != null)
			{
				bs._Game.listOfPlayers.Remove(this);
			}
		}
	}

	[RPC]
	public void Kick()
	{
		if (this == bs._Player)
		{
			MonoBehaviour.print("<<<<<<<speedhack>>>>>>>>>");
			PhotonNetwork.LeaveRoom();
		}
	}

	protected void OnPhotonSerializeView(PhotonStream stream, PhotonMessageInfo info)
	{
		//IL_0193: Unknown result type (might be due to invalid IL or missing references)
		//IL_019d: Expected O, but got Unknown
		Vector3 obj = vel;
		float obj2 = skid;
		stream.Serialize(ref obj);
		stream.Serialize(ref obj2);
		if (bs._Loader.dm)
		{
			if (bs._Loader.serverVersion < 1012)
			{
				Vector3 obj3 = Quaternion.LookRotation(turretDirection).eulerAngles;
				stream.Serialize(ref obj3);
				turretDirection = Quaternion.Euler(obj3) * Vector3.forward;
			}
			else
			{
				stream.Serialize(ref distanceToCursor2);
				stream.Serialize(ref TargetPlayerId);
			}
		}
		if (stream.isWriting)
		{
			Vector3 obj4 = transform.localPosition;
			Quaternion obj5 = transform.localRotation;
			stream.Serialize(ref obj4);
			stream.Serialize(ref obj5);
			return;
		}
		if (bs._Loader.dm && isMaster && Time.timeSinceLevelLoad > 10f)
		{
			step++;
			if (step >= 100)
			{
				if (LastTimeSend != 0f)
				{
					float num = (Time.realtimeSinceStartup - LastTimeSend) / 100f;
					if (num < 0.095f && num > 0f)
					{
						bs._GameGui.CallRPC(bs._GameGui.Chat, playerNameClan + " Kicked " + (Time.realtimeSinceStartup - LastTimeSend) / 100f);
						CallRPC((Action)(object)new Action(Kick));
					}
				}
				LastTimeSend = Time.realtimeSinceStartup;
				step = 0;
			}
		}
		if (stopAudio != null)
		{
			stopAudio.volume = obj2;
		}
		vel = obj;
		Vector3 obj6 = Vector3.zero;
		Quaternion obj7 = Quaternion.identity;
		stream.Serialize(ref obj6);
		stream.Serialize(ref obj7);
		for (int num2 = m_BufferedState.Length - 1; num2 >= 1; num2--)
		{
			ref State reference = ref m_BufferedState[num2];
			reference = m_BufferedState[num2 - 1];
		}
		float num3 = Mathf.Min(2f, (float)(PhotonNetwork.time - m_BufferedState[0].timestamp) + 0.1f);
		interpolationBackTime = ((!bs.setting.lagNetw) ? Mathf.Lerp(interpolationBackTime, num3, (!(interpolationBackTime > num3)) ? 0.5f : 0.01f) : 3f);
		State state = default(State);
		state.timestamp = info.timestamp;
		state.pos = obj6;
		state.rot = obj7;
		m_BufferedState[0] = state;
		m_TimestampCount = Mathf.Min(m_TimestampCount + 1, m_BufferedState.Length);
		for (int i = 0; i < m_TimestampCount - 1; i++)
		{
			if (m_BufferedState[i].timestamp < m_BufferedState[i + 1].timestamp)
			{
				Debug.Log("State inconsistent");
			}
		}
	}

	[RPC]
	internal void Mute(int time)
	{
		if (bs._Player == this)
		{
			MonoBehaviour.print("Set bantime " + time);
			bs._Loader.banTime = bs.totalSeconds + time * 60;
			bs._Loader.carskin = 0;
		}
		if (time < 1)
		{
			bs._GameGui.Chat(replay.playerName + GuiClasses.Tr(" unbanned from chat"));
		}
		else
		{
			bs._GameGui.Chat(replay.playerName + GuiClasses.Tr(" banned from chat"));
		}
	}

	public void UpdatePlayerDm()
	{
		bool flag = Time.time - fireTime < 7f;
		fire.SetActive(flag);
		if (flag)
		{
			life = (float)life - 15f * Time.deltaTime;
		}
		freeze = Mathf.Lerp(freeze, 0f, Time.deltaTime);
		ice.SetActive(froozen);
		if (dead)
		{
			return;
		}
		if (TimeElapsed(0.3f, 0f))
		{
			SetLife2(Mathf.Min((float)life + 1f, lifeDef));
		}
		if (!froozen)
		{
			WeaponBase[] array = weapons;
			foreach (WeaponBase weaponBase in array)
			{
				weaponBase.UpdateAlways();
			}
			if (IsMine)
			{
				for (int j = 0; j < 5; j++)
				{
					if (Input.GetKeyDown((KeyCode)(49 + j)) && j < weapons.Length && weapons[j].show && curWeaponId != j)
					{
						CallRPC(SelectWeapon, j);
					}
				}
			}
		}
		if (IsMine)
		{
			Player player = (from a in ((IEnumerable<Player>)bs._Game.listOfPlayers).Where((Func<Player, bool>)((Player a) => a != bs._Player && !a.dead && a.enabled && a.distanceToCursor.magnitude < 50f))
				orderby a.distanceToCursor.magnitude
				select a).FirstOrDefault();
			TargetPlayerId = ((!(player != null)) ? (-1) : player.playerId);
			distanceToCursor2 = ((!(player != null)) ? curWeapon.turretCannon.forward : player.distanceToCursor);
			return;
		}
		Vector3 pos = base.pos;
		Plane plane = new Plane((bs._Player.curWeapon.turretCannon.position - pos).normalized, pos);
		bs.tempTr.position = pos;
		bs.tempTr.forward = bs._Player.curWeapon.turretCannon.position - pos;
		Ray ray = new Ray(bs._Player.curWeapon.turretCannon.position, bs._Player.curWeapon.turretCannon.forward);
		if (!plane.Raycast(ray, out float enter))
		{
			distanceToCursor = bs._Player.curWeapon.turretCannon.forward * 99999f;
		}
		else
		{
			Vector3 point = ray.GetPoint(enter);
			Debug.DrawLine(point, pos);
			distanceToCursor = bs.tempTr.InverseTransformPoint(point);
		}
		UpdateTurretEuler();
	}

	public void UpdateTurretEuler(bool shooting = false)
	{
		Transform turretCannon = curWeapon.turretCannon;
		if (distanceToCursor2 != Vector3.zero)
		{
			if (!bs._Game.photonPlayers.ContainsKey(TargetPlayerId) || bs._Game.photonPlayers[TargetPlayerId] == null)
			{
				turretDirection = (shootDirection = distanceToCursor2);
				return;
			}
			bs.tempTr.position = bs._Game.photonPlayers[TargetPlayerId].pos;
			bs.tempTr.forward = turretCannon.position - bs.tempTr.position;
			turretDirection = (shootDirection = bs.tempTr.TransformPoint(distanceToCursor2) - turretCannon.position);
		}
	}

	[RPC]
	public void Shoot(int plId, Vector3 dist, PhotonMessageInfo info)
	{
		curWeapon.Shoot(plId, dist, info);
	}

	[RPC]
	public void SetTeam(int team)
	{
		replay.team = (Team)team;
	}

	[RPC]
	public void Die2(int k)
	{
		dead = true;
		Player player = bs._Game.photonPlayers[k];
		if (IsMine)
		{
			CallRPC(SetScore2, (float)score - 5f);
			bs._Game.centerText(string.Format(GuiClasses.Tr("{0} killed You {2} Points"), player.playerNameClan, string.Empty, -5), 3f);
		}
		else if (k == base.myId)
		{
			player.CallRPC(SetScore2, (float)player.score + 10f);
			bs._Game.centerText(string.Format(GuiClasses.Tr("You killed {1} {2} Points"), string.Empty, playerNameClan, 10), 3f);
		}
	}

	[RPC]
	public void Explode(Vector3 pos)
	{
		//IL_0050: Unknown result type (might be due to invalid IL or missing references)
		//IL_005a: Expected O, but got Unknown
		if (bs._Loader.dm)
		{
			BlackCar();
		}
		StartCoroutine(Base.AddMethod((!bs._Loader.dm) ? interpolationBackTime : 0f, (Action)(object)(Action)delegate
		{
			GameObject gameObject = (GameObject)UnityEngine.Object.Instantiate((UnityEngine.Object)bs.res.explosion);
			UnityEngine.Object.Destroy(gameObject, 5f);
			gameObject.transform.position = pos;
			if (IsMine)
			{
				gameObject.get_audio().priority = 0;
			}
		}));
	}

	[RPC]
	public void Reset2(PhotonMessageInfo info)
	{
		killedBy = null;
		resetTime = Time.time;
		dead = false;
		SetLife2(lifeDef);
		BlackReset();
		spawnTime = Time.time + (float)(PhotonNetwork.time - info.timestamp);
	}

	private void UpdateCamDm()
	{
		if (!bs._Loader.dm)
		{
			return;
		}
		if (!curWeapon.shootForward)
		{
			Vector3 vector = getMouseDelta() * (bs._Loader.sensivity * bs._Loader.sensivity) * 0.3f;
			if (!Screen.lockCursor)
			{
				vector = Vector3.zero;
			}
			camrot += new Vector3(0f - vector.y, vector.x * 2f);
			camrot.x = Mathf.Clamp(camrot.x, -70f, 85f);
			cam.localEulerAngles = camrot;
		}
		RaycastHit hitInfo = default(RaycastHit);
		Vector3? vector2 = camDefPos;
		if (!vector2.HasValue)
		{
			camDefPos = camera.transform.localPosition;
		}
		camera.transform.localPosition = camDefPos.Value;
		if (Physics.Linecast(camera.transform.position + camera.transform.forward * 4f, camera.transform.position, out hitInfo, Layer.levelMask))
		{
			camera.transform.position = hitInfo.point;
		}
		Ray ray = camera.ViewportPointToRay(Vector3.one / 2f);
		Vector3 position = curWeapon.turretCannon.position;
		if (!Physics.Raycast(ray, out hitInfo, 1000f, Layer.levelMask))
		{
			hitInfo.point = position + ray.direction * 1000f;
		}
		turretDirection = hitInfo.point - position;
		if (!Physics.Raycast(position, curWeapon.turretCannon.forward, out hitInfo, 1000f, Layer.levelMask))
		{
			hitInfo.point = position + curWeapon.turretCannon.forward * 1000f;
		}
		Vector3 b = camera.WorldToViewportPoint(hitInfo.point);
		bs._Game.cursorTexture.enabled = (b.z > 0f);
		bs._Game.cursorTexture.color = ((!(curWeapon.shootTm > curWeapon.shootInterval) && !(curWeapon.shootInterval < 0.2f)) ? new Color(0.1f, 0.1f, 0.1f, 0.5f) : new Color(0.5f, 0.5f, 0.5f, 0.5f));
		bs._Game.cursor.position = Vector3.Lerp(bs._Game.cursor.position, b, Time.deltaTime * 20f);
		((Component)bs._Game.cursor.parent).get_guiText().enabled = !curWeapon.shootForward;
		bs._Game.cursorTexture2.enabled = false;
	}

	[RPC]
	public void SelectWeapon(int id)
	{
		curWeaponId = id;
		DeactiveWeps();
		curWeapon.gameObject.SetActive(value: true);
		curWeapon.OnSelect();
		if (IsMine)
		{
			bs._Game.cursorTexture.texture = ((!(curWeapon.cursor != null)) ? bs.res.defCursor : curWeapon.cursor);
		}
	}

	private string ColorToHex(Color32 color)
	{
		return "#" + color.r.ToString("X2") + color.g.ToString("X2") + color.b.ToString("X2") + color.a.ToString("X2");
	}

	[RPC]
	public void SetScore2(float score)
	{
		this.score = score;
	}

	[Obsolete]
	[RPC]
	public void SetScore(int score)
	{
		this.score = score;
	}

	[RPC]
	internal void SetLife(float nlife, int killer)
	{
		if (Time.time - spawnTime < 5f)
		{
			return;
		}
		lastHitTime = Time.time;
		float num = (float)life - nlife;
		SetLife2(nlife);
		if (killer == base.myId)
		{
			showTextTime = Time.time;
		}
		if (IsMine && killer != -1)
		{
			hud.damageAnim.Rewind();
			hud.damageAnim.Play();
			killedBy = bs._Game.photonPlayers[killer];
			Vector3 eulerAngles = Quaternion.LookRotation(killedBy.pos - base.pos).eulerAngles;
			float y = eulerAngles.y;
			float current = y;
			Vector3 eulerAngles2 = cam.eulerAngles;
			y = Mathf.DeltaAngle(current, eulerAngles2.y);
			hud.damage.eulerAngles = new Vector3(0f, 0f, y);
		}
		if (killer == base.myId)
		{
			if (num > 0f)
			{
				DamageText((-(int)num).ToString());
			}
			bs.PlayOneShotGui(bs.res.hitFeedback);
		}
		if (IsMine && (float)life < 0f && !dead)
		{
			Die(water: false, killer);
		}
	}

	[RPC]
	internal void Freeze()
	{
		freezeTime = Time.time;
	}

	[RPC]
	internal void SetOnFire()
	{
		fireTime = Time.time;
	}

	[RPC]
	internal void SetLife2(float obj)
	{
		life = obj;
		RefreshText();
	}

	public void RefreshText()
	{
		string text = playerNameClan + "\n" + new string('▄', Mathf.Max(0, Mathf.CeilToInt((float)life / bs._Loader.lifeDef * 10f)));
		if (bs._Loader.team && team != bs._Player.team)
		{
			text = "<" + bs._Loader.Trn("Enemy") + ">\n" + text;
		}
		playerNameTxt2.text = text;
	}

	[RPC]
	public void SetShoot(bool b)
	{
		curWeapon.SetShoot(b);
	}

	private void UpdateStunts()
	{
		if (!ghost)
		{
			if (!bs.online)
			{
				score = Mathf.Lerp(score, 0f, Time.deltaTime * 0.01f);
			}
			Vector3 eulerAngles = transform.eulerAngles;
			bool flag = Time.time - groundedTime2 < 0.1f;
			if (!flag)
			{
				tempScore += Time.deltaTime * 20f;
				tempScore += Mathf.Abs(Mathf.DeltaAngle(oldRot.x, eulerAngles.x)) + Math.Abs(Mathf.DeltaAngle(oldRot.y, eulerAngles.y)) + Mathf.Abs(Mathf.DeltaAngle(oldRot.z, eulerAngles.z)) / 30f;
			}
			bool flag2 = Math.Abs(oldTempScore - tempScore) > 0.01f;
			if (flag2 && tempScore > 100f)
			{
				string s = ((int)tempScore).ToString();
				bs._Player.hud.PlayScore(s, Color.white);
			}
			if (flag && tempScore > 100f && !flag2)
			{
				AddScore(tempScore, Color.blue);
			}
			if (!flag2 && tempScore > 0f)
			{
				tempScore = 0f;
			}
			oldRot = eulerAngles;
			oldTempScore = tempScore;
		}
	}

	public void AddScore(float i, Color c)
	{
		if (bs._Loader.stunts)
		{
			bs._Player.hud.PlayScore(((int)i).ToString(), c);
			Player player = bs._Player;
			player.score = (float)player.score + i;
			bs._Player.CallRPC((Action<float>)SetScore2, (float)bs._Player.score);
		}
	}
}
