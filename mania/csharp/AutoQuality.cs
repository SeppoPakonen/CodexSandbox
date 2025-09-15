using System;
using System.Collections;
using System.Linq;
using UnityEngine;

public class AutoQuality : GuiClasses
{
	internal float fps;

	internal bool stop;

	internal float fixedDeltaTime = 0.005f;

	public bool autoQuality => bs._Loader.autoQuality;

	public new bool enabled
	{
		get
		{
			return base.enabled;
		}
		set
		{
			base.enabled = value;
		}
	}

	private static bool drawDistanceSet
	{
		get
		{
			return Base.PlayerPrefsGetBool("drawDistanceSet");
		}
		set
		{
			Base.PlayerPrefsSetBool("drawDistanceSet", value);
		}
	}

	public override void Awake()
	{
		base.Awake();
	}

	public IEnumerator Start()
	{
		return null;
	}

	public IEnumerator OnLevelWasLoaded2(int level)
	{
		MonoBehaviour.print("public IEnumerator OnLevelWasLoaded2(int level)");
		yield return new WaitForSeconds(0.1f);
		StartCoroutine(SetQuality(autoQuality ? Quality2.Low : bs._Loader.quality));
		stop = !autoQuality;
		enabled = true;
		fps = 0f;
		yield return null;
	}

	public void Update()
	{
		if (bs.setting.fps10)
		{
			Application.targetFrameRate = 10 + UnityEngine.Random.Range(0, 30);
		}
		if (bs._Loader.deltaTime != 0f)
		{
			fps = Mathf.Lerp(fps, 1f / bs._Loader.deltaTime, (!(bs._Loader.deltaTime * (1f / bs._Loader.deltaTime) > fps)) ? 0.01f : 1f);
		}
		bs.Log((!bs.isDebug) ? Mathf.Round(1f / bs._Loader.deltaTime) : fps, important: true);
		if (!stop && fps > 40f && autoQuality && bs._Loader.quality < Quality2.High)
		{
			StartCoroutine(SetQuality(bs._Loader.quality + 1));
		}
		if (KeyDebug(KeyCode.E, "Change Quality"))
		{
			StartCoroutine(SetQuality(bs._Loader.quality + 1));
		}
		if (KeyDebug(KeyCode.Q, "Change Quality") || (fps < 35f && bs._Loader.quality > Quality2.Low && autoQuality))
		{
			StartCoroutine(SetQuality(bs._Loader.quality - 1));
			MonoBehaviour.print("Auto quality stop");
			stop = true;
		}
	}

	public IEnumerator SetQuality(Quality2 q)
	{
		Debug.LogWarning("SetQuality:" + q);
		bs._Loader.quality = q;
		if (!drawDistanceSet)
		{
			if (bs.android)
			{
				bs._Loader.drawDistance = ((q == Quality2.Lowest) ? 150 : ((q <= Quality2.Low) ? 200 : ((q != Quality2.Medium) ? 500 : 300)));
			}
			else
			{
				Loader loader = bs._Loader;
				int drawDistance;
				switch (q)
				{
				case Quality2.Lowest:
					drawDistance = 200;
					break;
				case Quality2.Low:
					drawDistance = 1200;
					break;
				default:
					drawDistance = 10000;
					break;
				}
				loader.drawDistance = drawDistance;
			}
		}
		if (q < Quality2.Lowest)
		{
			q = Quality2.Lowest;
		}
		if (q > Quality2.Ultra)
		{
			q = Quality2.Ultra;
		}
		if (!bs.android)
		{
			int qlevel = (q >= Quality2.Medium) ? 5 : ((q >= Quality2.Low && !bs.android) ? 3 : 0);
			if (QualitySettings.GetQualityLevel() != qlevel)
			{
				QualitySettings.SetQualityLevel(qlevel, !autoQuality);
			}
		}
		RenderSettings.fog = (q > Quality2.Low);
		float num2 = fixedDeltaTime = (Time.fixedDeltaTime = ((q >= Quality2.Low) ? 0.005f : 0.01f));
		Shader.globalMaximumLOD = ((q <= Quality2.Low) ? 100 : 600);
		bs._Loader.shadows = (q >= Quality2.High && (!bs.flash || !bs.splitScreen));
		UpdateShadows();
		GameObject findGameObjectWithTag = GameObject.FindGameObjectWithTag("RearCamera");
		if (findGameObjectWithTag != null)
		{
			findGameObjectWithTag.get_camera().enabled = (q > Quality2.Low && bs._Loader.rearCamera);
		}
		Camera[] allCameras = Camera.allCameras;
		foreach (Camera a in allCameras)
		{
			if (a.tag == "MainCamera" || a.tag == "RearCamera")
			{
				if (q < Quality2.Low)
				{
					RenderSettings.skybox = null;
				}
				a.renderingPath = ((q > Quality2.Low || (!bs.android && !(bs._MapLoader == null))) ? ((q <= Quality2.Medium && !bs.android) ? RenderingPath.Forward : RenderingPath.DeferredLighting) : RenderingPath.VertexLit);
				MonoBehaviour.print(a.renderingPath);
				if (bs.android && bs.lowQuality)
				{
					a.cullingMask &= ~(1 << Layer.stadium);
				}
				else
				{
					a.cullingMask |= 1 << Layer.stadium;
				}
				if (bs.android && bs.lowQuality)
				{
					a.cullingMask &= ~(1 << Layer.water);
				}
				else
				{
					a.cullingMask |= 1 << Layer.water;
				}
				a.backgroundColor = RenderSettings.fogColor;
				a.farClipPlane = ((!bs.lowQuality || !bs.android) ? 10000 : 1000);
				a.hdr = bs._Loader.enableBloom;
				a.nearClipPlane = ((!bs.android) ? 0.1f : 0.3f);
			}
		}
		if (bs._MapLoader != null && bs._MapLoader.terrain != null)
		{
			bs._MapLoader.terrain.gameObject.SetActive(!bs.lowQuality || bs._Loader.dm);
			if (bs.android)
			{
				bs._MapLoader.terrain.heightmapMaximumLOD = 1;
			}
			bs._MapLoader.terrain.editorRenderFlags = ((!bs.highOrNotAndroid) ? ((TerrainRenderFlags)5) : TerrainRenderFlags.all);
		}
		UnityEngine.Object[] array = UnityEngine.Object.FindObjectsOfType(typeof(AudioReverbFilter));
		for (int j = 0; j < array.Length; j++)
		{
			AudioReverbFilter a2 = (AudioReverbFilter)array[j];
			a2.enabled = bs.medium;
		}
		UnityEngine.Object[] array2 = UnityEngine.Object.FindObjectsOfType(typeof(PostEffectsBase));
		for (int k = 0; k < array2.Length; k++)
		{
			MonoBehaviour a3 = (MonoBehaviour)array2[k];
			EnablePostEffect(a3);
		}
		UnityEngine.Object[] array3 = UnityEngine.Object.FindObjectsOfType(typeof(SSAOEffect));
		for (int l = 0; l < array3.Length; l++)
		{
			MonoBehaviour a4 = (MonoBehaviour)array3[l];
			EnablePostEffect(a4);
		}
		UnityEngine.Object[] array4 = UnityEngine.Object.FindObjectsOfType(typeof(AmplifyMotionEffect));
		for (int m = 0; m < array4.Length; m++)
		{
			MonoBehaviour a5 = (MonoBehaviour)array4[m];
			a5.enabled = (bs._Loader.enableMotionBlur && bs.highQuality);
		}
		UnityEngine.Object[] array5 = UnityEngine.Object.FindObjectsOfType(typeof(SunShafts));
		for (int n = 0; n < array5.Length; n++)
		{
			SunShafts efect = (SunShafts)array5[n];
			foreach (Light lt in UnityEngine.Object.FindObjectsOfType(typeof(Light)).Cast<Light>().Where((Func<Light, bool>)((Light b) => b.type == LightType.Directional)))
			{
				if (lt.flare != null)
				{
					lt.enabled = !bs.UltraQuality;
				}
				else
				{
					efect.sunTransform = lt.transform;
				}
			}
		}
		if (bs._Loader.levelEditor == null && bs._Game != null)
		{
			UpdateMaterials();
		}
		UpdateCull();
		if (!bs.flash)
		{
			Application.targetFrameRate = -1;
			int vSyncCount = QualitySettings.vSyncCount = ((!bs.setting.fps10) ? ((bs.isDebug || !bs.lowQuality) ? 1 : 0) : 0);
			MonoBehaviour.print("Set Vsync " + vSyncCount);
		}
		yield return null;
		MonoBehaviour.print("SetQuality: " + q);
	}

	private static void EnablePostEffect(Behaviour efect)
	{
		efect.enabled = (bs.UltraQuality && !bs.android && ((!(efect is Bloom) && !(efect is ColorCorrectionCurves) && !(efect is Tonemapping)) || bs._Loader.enableBloom));
	}

	public void UpdateMaterials()
	{
		MonoBehaviour.print("Update Materials " + bs._Game.LevelRenderers.Count);
		if (Application.isEditor)
		{
			return;
		}
		foreach (Renderer levelRenderer in bs._Game.LevelRenderers)
		{
			if (!(levelRenderer != null))
			{
				continue;
			}
			string name = levelRenderer.sharedMaterial.name;
			Material material = (!Application.isEditor) ? levelRenderer.sharedMaterial : levelRenderer.material;
			if (Application.isEditor)
			{
				material.name = name;
			}
			if (material.shader.name == bs.res.reflect.name || material.shader.name == bs.res.diffuse.name || material.shader.name == "Diffuse")
			{
				if (bs.highQuality && !bs.res.dirtMaterials.Contains(material.name.Split(' ')[0]))
				{
					material.shader = bs.res.reflect;
					material.SetTexture("_SpecTex", bs.res.noise);
					material.SetFloat("_Shininess", 1f);
					material.SetColor("_ReflectColor", Color.white * 1f * (bs._Loader.rain ? 0.2f : ((!bs._Loader.night) ? 0.1f : 0.05f)));
					material.SetColor("_SpecColor", Color.white * 0.3f);
					material.SetTexture("_Cube", bs.res.cubeMap);
					material.SetTexture("_MainTex2", bs.res.noise);
					material.SetTextureScale("_MainTex2", new Vector2(10f, 50f));
				}
				else
				{
					material.shader = bs.res.diffuse;
				}
			}
		}
	}

	public void UpdateShadows()
	{
		QualitySettings.shadowDistance = (bs._Loader.shadows ? (bs.android ? 50 : ((!(bs._Loader.levelEditor != null)) ? 150 : 1000)) : 0);
	}

	public void UpdateCull()
	{
		if (bs._Loader.CullMode == 1)
		{
			Camera[] allCameras = Camera.allCameras;
			foreach (Camera camera in allCameras)
			{
				if (camera.tag == "MainCamera")
				{
					camera.useOcclusionCulling = false;
					camera.layerCullSpherical = false;
					camera.layerCullDistances = new float[32];
					camera.farClipPlane = bs._Loader.drawDistance;
				}
			}
		}
		else
		{
			if (bs._Loader.CullMode != 0 && bs._Loader.CullMode != 2)
			{
				return;
			}
			MonoBehaviour.print("UpdateCull()" + bs._Loader.drawDistance);
			int drawDistance = bs._Loader.drawDistance;
			Camera[] allCameras2 = Camera.allCameras;
			foreach (Camera camera2 in allCameras2)
			{
				if (camera2.tag == "MainCamera")
				{
					camera2.useOcclusionCulling = (bs._Loader.CullMode == 3);
					camera2.layerCullSpherical = true;
					float[] array = new float[32];
					array[Layer.cull] = drawDistance;
					array[Layer.level] = drawDistance;
					array[Layer.model] = drawDistance;
					array[Layer.def] = drawDistance;
					array[Layer.terrain] = ((!bs.highQuality) ? drawDistance : 0);
					array[Layer.fx] = 100f;
					array[Layer.block] = (float)drawDistance / 2f;
					MonoBehaviour.print(camera2.name + array[Layer.terrain]);
					camera2.layerCullDistances = array;
					camera2.farClipPlane = 10000f;
				}
			}
		}
	}

	public void DrawSetQuality()
	{
		Label("Graphics Quality");
		GUILayout.BeginHorizontal();
		if (!bs.androidPlatform)
		{
			bs._Loader.autoQuality = (GlowButton("Auto", bs._Loader.autoQuality) || bs._Loader.autoQuality);
		}
		Quality2 quality = (Quality2)Toolbar((int)((!autoQuality) ? base.quality : ((Quality2)(-1))), Enum.GetNames(typeof(Quality2)), expand: false, center: false, (!bs.android && !bs.flash) ? 99 : 4, -1, useSkin: false);
		GUILayout.EndHorizontal();
		if (base.quality != quality && quality != (Quality2)(-1))
		{
			bs._Loader.autoQuality = false;
			drawDistanceSet = false;
			StartCoroutine(SetQuality(quality));
		}
		if (Application.platform != RuntimePlatform.Android)
		{
			bool flag = Toggle(bs._Loader.shadows, "Enable shadows");
			if (flag != bs._Loader.shadows)
			{
				bs._Loader.shadows = flag;
				UpdateShadows();
			}
		}
		if (bs.highQuality)
		{
			bool flag2 = Toggle(bs._Loader.enableMotionBlur, "Motion blur");
			if (flag2 != bs._Loader.enableMotionBlur)
			{
				bs._Loader.enableMotionBlur = flag2;
				StartCoroutine(SetQuality(base.quality));
			}
		}
		if (bs.UltraQuality)
		{
			bool flag3 = Toggle(bs._Loader.enableBloom, "Enable Bloom");
			if (flag3 != bs._Loader.enableBloom)
			{
				bs._Loader.enableBloom = flag3;
				StartCoroutine(SetQuality(base.quality));
			}
		}
	}

	public void DrawDistance()
	{
		Label(Trs("Draw Distance: ") + bs._Loader.drawDistance);
		int num = (int)GUILayout.HorizontalSlider(bs._Loader.drawDistance, 1f, (!bs.android) ? 5000 : 1000);
		if (num != bs._Loader.drawDistance)
		{
			drawDistanceSet = true;
			bs._Loader.drawDistance = num;
			UpdateCull();
		}
	}
}
