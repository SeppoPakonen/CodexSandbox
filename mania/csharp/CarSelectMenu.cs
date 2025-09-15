using CodeStage.AntiCheat.ObscuredTypes;
using System;
using UnityEngine;

public class CarSelectMenu : GuiClasses
{
	public class Profile
	{
		public string prefs = string.Empty;
	}

	public ObscuredInt car = -1;

	public GUITexture leftButton;

	public GUITexture rightButton;

	public Texture2D lockedIcon;

	public Texture2D selectIcon;

	public GUITexture selectButton;

	public GUITexture cancelButton;

	public GUITexture selectFlagButton;

	public Transform CarPlaceHolder;

	public GUIText carName;

	public GUIText lockedTxt;

	public GUITexture textbacground;

	public GUITexture lockedButton;

	private Texture2D pickTexture;

	private Texture2D hueTexture;

	private Color hueColor;

	private Renderer[] renderers;

	private GameObject carModel;

	public Transform cam;

	private Vector3 camRot;

	private float sPos;

	private Vector2 prevCord;

	private bool locked;

	private CarSkin carSkin;

	private float old;

	public float Scale = 30f;

	private int CarId => bs.Mod(car, bs._Loader.CarSkins.Count);

	public void Start()
	{
		//IL_009e: Unknown result type (might be due to invalid IL or missing references)
		//IL_00aa: Expected O, but got Unknown
		UnityEngine.Object[] array = UnityEngine.Object.FindObjectsOfType(typeof(GUITexture));
		for (int i = 0; i < array.Length; i++)
		{
			GUITexture gUITexture = (GUITexture)array[i];
			if (gUITexture != bs._Loader.fullScreen)
			{
				Rect pixelInset = gUITexture.pixelInset;
				pixelInset.width = (float)((int)pixelInset.width * Screen.width) / 800f;
				pixelInset.height = (float)((int)pixelInset.height * Screen.width) / 800f;
				gUITexture.pixelInset = pixelInset;
			}
		}
		bs.LogEvent("!CarSelect");
		bs.win.ShowWindow((Action)(object)new Action(Window), null, skip: true);
		car = ((bs._Loader.carskin != -1) ? bs._Loader.carskin : 0);
		sPos = (int)car;
		LoadSkin();
		camRot = cam.eulerAngles;
		StartCoroutine(bs._AutoQuality.OnLevelWasLoaded2(0));
	}

	public Color huefrag(Vector2 o, Color te)
	{
		float num = Mathf.Floor(o.x * 6f);
		float num2 = o.x * 6f - num;
		return (num == 0f) ? new Color(1f, num2, 0f, 1f) : ((num == 1f) ? new Color(1f - num2, 1f, 0f, 1f) : ((num == 2f) ? new Color(0f, 1f, num2, 1f) : ((num == 3f) ? new Color(0f, 1f - num2, 1f, 1f) : ((num == 4f) ? new Color(num2, 0f, 1f, 1f) : ((num != 5f) ? new Color(1f, 0f, 0f, 1f) : new Color(1f, 0f, 1f - num2, 1f))))));
	}

	public Color frag(Vector2 o, Color _Color)
	{
		Color result = o.y * Color.white + (_Color - Color.white) * o.x * Color.white;
		result.a = 1f;
		return result;
	}

	private void MaterialPick()
	{
		Label("Pick Color");
		bs.win.dock = Dock.Right;
		bool mouseButtonDown = Input.GetMouseButtonDown(0);
		if (hueTexture == null)
		{
			hueTexture = Create(new Texture2D(250, 20), huefrag, Color.white);
		}
		GUILayout.Label(hueTexture, GUIStyle.none, GUILayout.Width(250f));
		Rect lastRect = GUILayoutUtility.GetLastRect();
		Vector2 mousePosition = Event.current.mousePosition;
		if (lastRect.Contains(mousePosition) && mouseButtonDown)
		{
			hueColor = hueTexture.GetPixel((int)((mousePosition.x - lastRect.x) / lastRect.width * (float)hueTexture.width), 1);
			pickTexture = null;
		}
		if (pickTexture == null)
		{
			pickTexture = Create(new Texture2D(100, 100), frag, hueColor);
		}
		GUILayout.Label(pickTexture, GUIStyle.none, GUILayout.ExpandWidth(expand: false));
		lastRect = GUILayoutUtility.GetLastRect();
		if (lastRect.Contains(mousePosition) && mouseButtonDown)
		{
			Color pixel = pickTexture.GetPixel((int)Mathf.Clamp(mousePosition.x - lastRect.x, 0f, lastRect.width), (int)Mathf.Clamp(lastRect.height - (mousePosition.y - lastRect.y), 0f, lastRect.height - 1f));
			carSkin.color = pixel;
			carSkin.SetColor(renderers);
		}
	}

	private Texture2D Create(Texture2D hue, Func<Vector2, Color, Color> act, Color c)
	{
		for (int i = 0; i < hue.width; i++)
		{
			for (int j = 0; j < hue.height; j++)
			{
				hue.SetPixel(i, j, act.Invoke(new Vector2((float)i / (float)hue.width, (float)j / (float)hue.height), c));
			}
		}
		hue.Apply(updateMipmaps: true);
		return hue;
	}

	private void LoadSkin()
	{
		foreach (Transform item in CarPlaceHolder)
		{
			UnityEngine.Object.Destroy(item.gameObject);
		}
		CarSkin carSkin = bs._Loader.GetCarSkin(CarId, mine: true);
		GameObject model = carSkin.model;
		Vector3 position = CarPlaceHolder.position;
		Vector3 position2 = model.transform.position;
		carModel = (GameObject)UnityEngine.Object.Instantiate((UnityEngine.Object)model, position + position2.y * Vector3.up, Quaternion.identity);
		renderers = carModel.GetComponentsInChildren<Renderer>();
		if (carSkin.haveColor)
		{
			carSkin.SetColor(renderers);
		}
		Player.SetFlag(carModel, bs._Loader.Country);
		carModel.transform.parent = CarPlaceHolder;
	}

	public void ProfileWindow()
	{
		GUILayout.Label(bs._Loader.Avatar);
		Label("Medals:" + bs._Loader.medals);
		Label("Reputation:" + bs._Loader.reputation);
		Label("Friends:" + bs._Loader.friendCount);
		Label("Clan:" + bs._Loader.clanName);
		foreach (string friend in bs._Loader.friends)
		{
			Label(friend);
		}
	}

	public void Window()
	{
		//IL_0066: Unknown result type (might be due to invalid IL or missing references)
		//IL_0072: Unknown result type (might be due to invalid IL or missing references)
		//IL_007c: Expected O, but got Unknown
		//IL_007c: Expected O, but got Unknown
		//IL_00af: Unknown result type (might be due to invalid IL or missing references)
		//IL_00ba: Expected O, but got Unknown
		//IL_026a: Unknown result type (might be due to invalid IL or missing references)
		//IL_028c: Expected O, but got Unknown
		//IL_03b0: Unknown result type (might be due to invalid IL or missing references)
		//IL_03e5: Expected O, but got Unknown
		bs.win.Setup(350, 600, " ", Dock.Right, null, null, 1f);
		BeginScrollView();
		GUILayout.BeginHorizontal();
		if (GUILayout.Button(bs._Loader.Avatar, base.skin.label))
		{
			ShowWindow((Action)(object)new Action(bs._Loader.SelectAvatar), (Action)(object)new Action(Window));
		}
		GUILayout.BeginVertical();
		if (bs.isDebug && Button("Profile"))
		{
			ShowWindow((Action)(object)new Action(ProfileWindow));
		}
		GUILayout.EndVertical();
		GUILayout.EndHorizontal();
		GUILayout.Label(bs._Loader.playerName.ToUpper());
		GUILayout.BeginHorizontal();
		bool flag = Button("<Prev");
		if (flag || Button("Next>"))
		{
			int num = (!flag) ? 1 : (-1);
			do
			{
				car = (int)car + num;
			}
			while (bs._Loader.GetCarSkin(CarId, mine: true).hidden && bs._Loader.modType < ModType.tester);
		}
		GUILayout.EndHorizontal();
		if (!locked)
		{
			if (!carSkin.repUnlocked && !bs._Loader.disableRep)
			{
				base.skin.label.wordWrap = true;
				if ((int)bs._Loader.reputation < carSkin.repNeeded)
				{
					GUILayout.Label(new GUIContent(string.Format(GuiClasses.Tr("You need {0} reputation points to unlock this car"), carSkin.repNeeded), bs.win.reputation));
				}
				else
				{
					GUILayout.Label(new GUIContent(string.Format(GuiClasses.Tr("Price: {0} reputation points"), carSkin.repNeeded), bs.win.reputation));
					if (Button("Buy this car"))
					{
						BuyWindow((Action)(object)(Action)delegate
						{
							carSkin.repUnlocked = true;
						}, carSkin.repNeeded, GuiClasses.Tr("car"));
					}
				}
			}
			else
			{
				if (ButtonTexture(GuiClasses.Tr((!locked) ? "Select" : "Locked"), (!locked) ? selectIcon : lockedIcon, 60) && !locked)
				{
					bs._Loader.carskin = CarId;
					LoadLevel("!1");
				}
				if ((int)car == 0 && Button("Change Skin"))
				{
					bs._Loader.Country++;
					bs._Loader.Country = (CountryCodes)((int)bs._Loader.Country % Enum.GetValues(typeof(CountryCodes)).Length);
					Player.SetFlag(carModel, bs._Loader.Country);
				}
				if (carSkin.canPickColor && bs.medium && Button("Pick Color"))
				{
					BuyWindow((Action)(object)(Action)delegate
					{
						//IL_0014: Unknown result type (might be due to invalid IL or missing references)
						//IL_0020: Unknown result type (might be due to invalid IL or missing references)
						//IL_002a: Expected O, but got Unknown
						//IL_002a: Expected O, but got Unknown
						carSkin.paintUnlocked = true;
						ShowWindow((Action)(object)new Action(MaterialPick), (Action)(object)new Action(Window));
					}, 15, GuiClasses.Tr("Pick Color"), carSkin.paintUnlocked || bs._Loader.disableRep);
				}
			}
		}
		else
		{
			base.skin.label.wordWrap = true;
			GUILayout.Label(new GUIContent(lockedTxt.text, lockedIcon));
		}
		if (!locked && carSkin != null)
		{
			base.skin.label.alignment = TextAnchor.UpperLeft;
			GUILayout.Label(new GUIContent(GuiClasses.Tr("Top Speed"), bs.res.rating[(carSkin.TopSpeed + 1) * 2]));
			GUILayout.Label(new GUIContent(GuiClasses.Tr("Acceleration"), bs.res.rating[(carSkin.Speed + 1) * 2]));
			GUILayout.Label(new GUIContent(GuiClasses.Tr("Handling"), bs.res.rating[(carSkin.Rotation + 1) * 2]));
			GUILayout.Label(new GUIContent(GuiClasses.Tr("Drift"), bs.res.rating[(carSkin.Drift + 1) * 2]));
		}
		if (bs.input.GetKeyDown(KeyCode.Escape) || Button("Exit"))
		{
			Cancel();
		}
		if (Button(string.Concat(bs._Loader.medals, Trs(" medals")), expandWidth: false, 20, bold: false, bs.win.medalsCnt))
		{
			Popup("medaltext", bs.win.medalsCnt);
		}
		if (!bs._Loader.disableRep && Button(string.Concat(bs._Loader.reputation, Trs(" Reputation")), expandWidth: false, 20, bold: false, bs.win.reputation))
		{
			Popup("reptext", bs.win.reputation);
		}
		GUILayout.EndScrollView();
	}

	private void BuyWindow(Action a, int rep, string s, bool unlocked = false)
	{
		//IL_0050: Unknown result type (might be due to invalid IL or missing references)
		//IL_005b: Expected O, but got Unknown
		MonoBehaviour.print(bs.win.act);
		Action oldwin = bs.win.act;
		if (!unlocked)
		{
			ShowWindow((Action)(object)(Action)delegate
			{
				base.skin.label.wordWrap = true;
				if ((int)bs._Loader.reputation < rep)
				{
					GUILayout.Label(new GUIContent(string.Format(GuiClasses.Tr("You must have at least {0} reputation points"), rep), bs.win.reputation));
					if (Button("Back"))
					{
						ShowWindow(oldwin);
					}
				}
				else
				{
					GUILayout.Label(new GUIContent(string.Format(GuiClasses.Tr("Unlock '{0}' for {1} reputation points?"), s, rep), bs.win.reputation));
					GUILayout.BeginHorizontal();
					if (Button("Yes"))
					{
						ShowWindow(oldwin);
						Loader loader = bs._Loader;
						loader.reputation = (int)loader.reputation - rep;
						a.Invoke();
					}
					if (Button("No"))
					{
						ShowWindow(oldwin);
					}
					GUILayout.EndHorizontal();
				}
			});
		}
		else
		{
			a.Invoke();
		}
	}

	public void Update()
	{
		if (Input.GetKeyDown(KeyCode.Escape))
		{
			Cancel();
		}
		sPos = Mathf.Lerp(sPos, (int)car, Time.deltaTime * 2f);
		sPos = Mathf.MoveTowards(sPos, (int)car, Time.deltaTime * 0.3f);
		carName.text = CarId + 1 + "/" + bs._Loader.CarSkins.Count;
		if (Input.GetMouseButton(0))
		{
			Vector2 mouseDelta = getMouseDelta();
			camRot.y += mouseDelta.x * 3f;
		}
		else
		{
			camRot.y += 10f * Time.deltaTime;
		}
		camRot.x = Mathf.Clamp(camRot.x, 0f, 45f);
		cam.eulerAngles = camRot;
		GUITexture[] array = new GUITexture[5]
		{
			leftButton,
			rightButton,
			selectButton,
			cancelButton,
			selectFlagButton
		};
		carSkin = bs._Loader.GetCarSkin(CarId, mine: true);
		locked = false;
		if (!carSkin.medalsUnlocked && !bs._Loader.carsCheat)
		{
			locked = true;
			lockedTxt.text = string.Format(GuiClasses.Tr("You need {0} medals to unlock this car, you have {1}"), carSkin.medalsNeeded, bs._Loader.medals);
		}
		else if (carSkin.friendsNeeded > bs._Integration.FbFriendsInGame && !bs._Loader.tankCheat)
		{
			locked = true;
			lockedTxt.text = string.Format(GuiClasses.Tr("You need {0} invited friends on facebook to unlock this tank, you have {1}"), carSkin.friendsNeeded, bs._Integration.FbFriendsInGame);
		}
		lockedButton.enabled = locked;
		selectButton.texture = ((!locked) ? selectIcon : lockedIcon);
		GUIText gUIText = lockedTxt;
		bool enabled = locked;
		textbacground.enabled = enabled;
		gUIText.enabled = enabled;
		selectFlagButton.enabled = ((int)car == 0);
		float num = bs.Mod(sPos + 0.5f, 1f) - 0.5f;
		GUITexture[] array2 = array;
		foreach (GUITexture gUITexture in array2)
		{
			bool flag = gUITexture.HitTest(Input.mousePosition) && (gUITexture != selectButton || !locked);
			gUITexture.color = ((!flag) ? new Color(0.5f, 0.5f, 0.5f, 1f) : new Color(1f, 1f, 1f, 1f));
			if (!flag || Input.GetMouseButtonDown(0))
			{
			}
		}
		if (Math.Abs(old - num) > 0.9f)
		{
			LoadSkin();
		}
		old = num;
		CarPlaceHolder.parent.position = -Camera.main.transform.right * num * Scale;
	}

	private void Cancel()
	{
		if (bs._Loader.carskin == -1)
		{
			bs._Loader.carskin = 0;
		}
		LoadLevel("!1");
	}
}
