using System.Collections.Generic;
using UnityEngine;

public class Res : MonoBehaviour
{
	public AnimationCurve ForceCurve;

	public AnimationCurve RotCurve;

	public float rotationStart = 3f;

	public float speedSubOnRotate = 0.05f;

	public Shader[] shaders;

	public Cubemap cubeMap;

	public AnimationCurve gears;

	public AudioClip[] hitSound;

	public AudioClip[] hitSoundBig;

	public Shader reflect;

	public Shader diffuse;

	public Shader specular;

	public Shader transparment;

	public Shader transparmentCutout;

	public Texture2D[] go123;

	public Texture2D[] medals;

	public AudioClip speedUp;

	public AudioClip bip;

	public AudioClip start;

	public AudioClip checkPoint;

	public AudioClip idle;

	public AudioClip wind;

	public GameObject speedUpPrefab;

	public GameObject checkPointPrefab;

	public Game _Game;

	public AudioClip brake;

	public AudioClip mudBrake;

	public AudioClip[] motor;

	public List<CarSkin> CarSkins;

	public List<Scene> scenes;

	public string[] titles;

	public Texture2D sendReplayIcon;

	public AudioClip winSound;

	public AudioClip backTime;

	public AudioClip horn;

	public List<string> levelMaterialsTxt;

	public List<string> dirtMaterials;

	public List<string> animatedTextures;

	public List<string> specularTextures;

	public GUIStyle mapSelectButton;

	public GUIStyle menuButton;

	public GUIStyle labelGlow;

	public Texture2D[] avatars;

	public Loader loaderPrefab;

	public Texture2D faceBook;

	public GUIStyle arrow;

	public Texture2D attention;

	public Texture2D android;

	public PhysicMaterial track;

	public PhysicMaterial border;

	private Dictionary<string, WWW> avatarWww = new Dictionary<string, WWW>();

	private Dictionary<string, Texture2D> avatarWT = new Dictionary<string, Texture2D>();

	public float minRotation2 = 0.5f;

	public float rotationAdd2 = 100f;

	public bool projectFly;

	public float gravitation = 25f;

	public GameSettings gameSettings;

	public float stabilize = 4f;

	public float stabilizeSpeed = 4f;

	public Texture2D noise;

	public Material roadMaterial;

	public Transform startPrefab;

	public Transform CheckPoint;

	public Transform dot;

	public Material[] levelTextures;

	public AudioClip nitro;

	public Material lineMaterialYellow;

	public int renderSettingsId;

	public AudioClip waterSplash;

	public AudioClip waterSound;

	public AudioClip waterSound2;

	public AudioClip carCrash;

	public GameObject explosion;

	public Texture2D[] rating;

	public Shader rainShader;

	public Shader rainShader2;

	public AudioClip[] gearChange;

	public Bounds terrainBounds;

	public AudioClip chat;

	public Material waterMaterial;

	public Shader waterMaterialDef;

	public AudioClip[] shoot;

	public Bullet bullet;

	public GameObject hole;

	public GameObject hole2;

	public AudioClip[] bulletHitSound;

	public GameObject turretExp;

	public AudioClip lifeHitSound;

	public AudioClip hitFeedback;

	public AudioClip youHaveFlag;

	public AudioClip yourTeamHaveFlag;

	public AudioClip enemyHaveYourFlag;

	public AudioClip redFlagReturn;

	public AudioClip blueFlagReturn;

	public AudioClip youLoose;

	public AudioClip youWin;

	public AudioClip youWin2;

	public AudioClip endGame;

	public AudioClip jump;

	public GameObject coin;

	public AudioClip coinSound;

	public DamageText damageText;

	public GameObject fire;

	public Texture2D defCursor;

	public Texture2D GetAvatar(int avatarId, string avatar)
	{
		Texture2D texture2D = avatars[Mathf.Clamp(avatarId, 0, avatars.Length)];
		if (avatarId == 0 && !string.IsNullOrEmpty(avatar))
		{
			if (!avatarWww.ContainsKey(avatar))
			{
				avatarWww[avatar] = new WWW(avatar);
			}
			if (avatarWT.TryGetValue(avatar, out Texture2D value))
			{
				return value;
			}
			if (avatarWww[avatar].isDone)
			{
				Texture2D texture;
				if (string.IsNullOrEmpty(avatarWww[avatar].error))
				{
					texture = avatarWww[avatar].texture;
					avatarWT[avatar] = texture;
					return texture;
				}
				Debug.LogWarning(avatarWww[avatar].error);
				texture = texture2D;
				avatarWT[avatar] = texture;
				return texture;
			}
		}
		return texture2D;
	}
}
