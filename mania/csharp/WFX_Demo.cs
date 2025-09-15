using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WFX_Demo : MonoBehaviour
{
	public float cameraSpeed = 10f;

	public bool orderedSpawns = true;

	public float step = 1f;

	public float range = 5f;

	private float order = -5f;

	public GameObject walls;

	public GameObject bulletholes;

	public GameObject[] ParticleExamples;

	private int exampleIndex;

	private string randomSpawnsDelay = "0.5";

	private bool randomSpawns;

	private bool slowMo;

	private bool rotateCam = true;

	public Material wood;

	public Material concrete;

	public Material metal;

	public Material checker;

	public Material woodWall;

	public Material concreteWall;

	public Material metalWall;

	public Material checkerWall;

	private string groundTextureStr = "Checker";

	private List<string> groundTextures = new List<string>(new string[4]
	{
		"Concrete",
		"Wood",
		"Metal",
		"Checker"
	});

	public GameObject m4;

	public GameObject m4fps;

	private bool rotate_m4 = true;

	private void OnMouseDown()
	{
		RaycastHit hitInfo = default(RaycastHit);
		if (base.get_collider().Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), out hitInfo, 9999f))
		{
			GameObject gameObject = spawnParticle();
			if (!gameObject.name.StartsWith("WFX_MF"))
			{
				gameObject.transform.position = hitInfo.point + gameObject.transform.position;
			}
		}
	}

	public GameObject spawnParticle()
	{
		GameObject gameObject = (GameObject)Object.Instantiate((Object)ParticleExamples[exampleIndex]);
		if (gameObject.name.StartsWith("WFX_MF"))
		{
			gameObject.transform.parent = ParticleExamples[exampleIndex].transform.parent;
			gameObject.transform.localPosition = ParticleExamples[exampleIndex].transform.localPosition;
			gameObject.transform.localRotation = ParticleExamples[exampleIndex].transform.localRotation;
		}
		else if (gameObject.name.Contains("Hole"))
		{
			gameObject.transform.parent = bulletholes.transform;
		}
		SetActiveCrossVersions(gameObject, active: true);
		return gameObject;
	}

	private void SetActiveCrossVersions(GameObject obj, bool active)
	{
		obj.SetActive(active);
		for (int i = 0; i < obj.transform.childCount; i++)
		{
			obj.transform.GetChild(i).gameObject.SetActive(active);
		}
	}

	private void OnGUI()
	{
		GUILayout.BeginArea(new Rect(5f, 20f, Screen.width - 10, 60f));
		GUILayout.BeginHorizontal();
		GUILayout.Label("Effect: " + ParticleExamples[exampleIndex].name, GUILayout.Width(280f));
		if (GUILayout.Button("<", GUILayout.Width(30f)))
		{
			prevParticle();
		}
		if (GUILayout.Button(">", GUILayout.Width(30f)))
		{
			nextParticle();
		}
		GUILayout.FlexibleSpace();
		GUILayout.Label("Click on the ground to spawn the selected effect");
		GUILayout.FlexibleSpace();
		if (GUILayout.Button((!rotateCam) ? "Rotate Camera" : "Pause Camera", GUILayout.Width(110f)))
		{
			rotateCam = !rotateCam;
		}
		if (GUILayout.Button((!base.get_renderer().enabled) ? "Show Ground" : "Hide Ground", GUILayout.Width(90f)))
		{
			base.get_renderer().enabled = !base.get_renderer().enabled;
		}
		if (GUILayout.Button((!slowMo) ? "Slow Motion" : "Normal Speed", GUILayout.Width(100f)))
		{
			slowMo = !slowMo;
			if (slowMo)
			{
				Time.timeScale = 0.33f;
			}
			else
			{
				Time.timeScale = 1f;
			}
		}
		GUILayout.EndHorizontal();
		GUILayout.BeginHorizontal();
		GUILayout.Label("Ground texture: " + groundTextureStr, GUILayout.Width(160f));
		if (GUILayout.Button("<", GUILayout.Width(30f)))
		{
			prevTexture();
		}
		if (GUILayout.Button(">", GUILayout.Width(30f)))
		{
			nextTexture();
		}
		GUILayout.EndHorizontal();
		GUILayout.EndArea();
		if (!m4.get_renderer().enabled)
		{
			return;
		}
		GUILayout.BeginArea(new Rect(5f, Screen.height - 100, Screen.width - 10, 90f));
		rotate_m4 = GUILayout.Toggle(rotate_m4, "AutoRotate Weapon", GUILayout.Width(250f));
		GUI.enabled = !rotate_m4;
		Vector3 localEulerAngles = m4.transform.localEulerAngles;
		float x = localEulerAngles.x;
		x = ((!(x > 90f)) ? x : (x - 180f));
		Vector3 localEulerAngles2 = m4.transform.localEulerAngles;
		float y = localEulerAngles2.y;
		Vector3 localEulerAngles3 = m4.transform.localEulerAngles;
		float z = localEulerAngles3.z;
		x = GUILayout.HorizontalSlider(x, 0f, 179f, GUILayout.Width(256f));
		y = GUILayout.HorizontalSlider(y, 0f, 359f, GUILayout.Width(256f));
		z = GUILayout.HorizontalSlider(z, 0f, 359f, GUILayout.Width(256f));
		if (GUI.changed)
		{
			if (x > 90f)
			{
				x += 180f;
			}
			m4.transform.localEulerAngles = new Vector3(x, y, z);
			Debug.Log(x);
		}
		GUILayout.EndArea();
	}

	private IEnumerator RandomSpawnsCoroutine()
	{
		while (true)
		{
			GameObject particles = spawnParticle();
			if (orderedSpawns)
			{
				Transform transform = particles.transform;
				Vector3 position = base.transform.position;
				float x = order;
				Vector3 position2 = particles.transform.position;
				transform.position = position + new Vector3(x, position2.y, 0f);
				order -= step;
				if (order < 0f - range)
				{
					order = range;
				}
			}
			else
			{
				Transform transform2 = particles.transform;
				Vector3 a = base.transform.position + new Vector3(Random.Range(0f - range, range), 0f, Random.Range(0f - range, range));
				Vector3 position3 = particles.transform.position;
				transform2.position = a + new Vector3(0f, position3.y, 0f);
			}
			yield return new WaitForSeconds(float.Parse(randomSpawnsDelay));
		}
	}

	private void Update()
	{
		if (Input.GetKeyDown(KeyCode.LeftArrow))
		{
			prevParticle();
		}
		else if (Input.GetKeyDown(KeyCode.RightArrow))
		{
			nextParticle();
		}
		if (rotateCam)
		{
			Camera.main.transform.RotateAround(Vector3.zero, Vector3.up, cameraSpeed * Time.deltaTime);
		}
		if (rotate_m4)
		{
			m4.transform.Rotate(new Vector3(0f, 40f, 0f) * Time.deltaTime, Space.World);
		}
	}

	private void prevTexture()
	{
		int num = groundTextures.IndexOf(groundTextureStr);
		num--;
		if (num < 0)
		{
			num = groundTextures.Count - 1;
		}
		groundTextureStr = groundTextures[num];
		selectMaterial();
	}

	private void nextTexture()
	{
		int num = groundTextures.IndexOf(groundTextureStr);
		num++;
		if (num >= groundTextures.Count)
		{
			num = 0;
		}
		groundTextureStr = groundTextures[num];
		selectMaterial();
	}

	private void selectMaterial()
	{
		switch (groundTextureStr)
		{
		case "Concrete":
			base.get_renderer().material = concrete;
			((Component)walls.transform.GetChild(0)).get_renderer().material = concreteWall;
			((Component)walls.transform.GetChild(1)).get_renderer().material = concreteWall;
			break;
		case "Wood":
			base.get_renderer().material = wood;
			((Component)walls.transform.GetChild(0)).get_renderer().material = woodWall;
			((Component)walls.transform.GetChild(1)).get_renderer().material = woodWall;
			break;
		case "Metal":
			base.get_renderer().material = metal;
			((Component)walls.transform.GetChild(0)).get_renderer().material = metalWall;
			((Component)walls.transform.GetChild(1)).get_renderer().material = metalWall;
			break;
		case "Checker":
			base.get_renderer().material = checker;
			((Component)walls.transform.GetChild(0)).get_renderer().material = checkerWall;
			((Component)walls.transform.GetChild(1)).get_renderer().material = checkerWall;
			break;
		}
	}

	private void prevParticle()
	{
		exampleIndex--;
		if (exampleIndex < 0)
		{
			exampleIndex = ParticleExamples.Length - 1;
		}
		showHideStuff();
	}

	private void nextParticle()
	{
		exampleIndex++;
		if (exampleIndex >= ParticleExamples.Length)
		{
			exampleIndex = 0;
		}
		showHideStuff();
	}

	private void showHideStuff()
	{
		if (ParticleExamples[exampleIndex].name.StartsWith("WFX_MF Spr"))
		{
			m4.get_renderer().enabled = true;
		}
		else
		{
			m4.get_renderer().enabled = false;
		}
		if (ParticleExamples[exampleIndex].name.StartsWith("WFX_MF FPS"))
		{
			m4fps.get_renderer().enabled = true;
		}
		else
		{
			m4fps.get_renderer().enabled = false;
		}
		if (ParticleExamples[exampleIndex].name.StartsWith("WFX_BImpact"))
		{
			SetActiveCrossVersions(walls, active: true);
			Renderer[] componentsInChildren = bulletholes.GetComponentsInChildren<Renderer>();
			Renderer[] array = componentsInChildren;
			foreach (Renderer renderer in array)
			{
				renderer.enabled = true;
			}
		}
		else
		{
			SetActiveCrossVersions(walls, active: false);
			Renderer[] componentsInChildren2 = bulletholes.GetComponentsInChildren<Renderer>();
			Renderer[] array2 = componentsInChildren2;
			foreach (Renderer renderer2 in array2)
			{
				renderer2.enabled = false;
			}
		}
		if (ParticleExamples[exampleIndex].name.Contains("Wood"))
		{
			groundTextureStr = "Wood";
			selectMaterial();
		}
		else if (ParticleExamples[exampleIndex].name.Contains("Concrete"))
		{
			groundTextureStr = "Concrete";
			selectMaterial();
		}
		else if (ParticleExamples[exampleIndex].name.Contains("Metal"))
		{
			groundTextureStr = "Metal";
			selectMaterial();
		}
		else if (ParticleExamples[exampleIndex].name.Contains("Dirt") || ParticleExamples[exampleIndex].name.Contains("Sand") || ParticleExamples[exampleIndex].name.Contains("SoftBody"))
		{
			groundTextureStr = "Checker";
			selectMaterial();
		}
		else if (ParticleExamples[exampleIndex].name == "WFX_Explosion")
		{
			groundTextureStr = "Checker";
			selectMaterial();
		}
	}
}
