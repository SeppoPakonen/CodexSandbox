using System.Collections.Generic;
using UnityEngine;

public class effectSpawn : MonoBehaviour
{
	public GameObject[] ListaEffetti;

	public float[] Offsets;

	public string[] Descrizioni;

	private GameObject effect;

	private Ray ray;

	private RaycastHit hit;

	private int listaLength;

	private int id;

	private GUIStyle customText;

	public Color textColor;

	public int fontSize;

	public int textPositionX;

	public int textPositionY;

	private GameObject instance;

	public bool DontDestroyOldObjectOnNew;

	private List<GameObject> oldObject;

	private void Awake()
	{
		textColor = Color.black;
		customText = new GUIStyle();
		customText.normal.textColor = textColor;
		customText.fontSize = fontSize;
		customText.padding.left = textPositionX;
		customText.padding.top = textPositionY;
		oldObject = new List<GameObject>();
	}

	private void Start()
	{
		id = 0;
		effect = ListaEffetti[id];
		listaLength = ListaEffetti.Length;
	}

	private void Update()
	{
		getImputMouse();
		getInputKey();
	}

	private void getImputMouse()
	{
		effect = ListaEffetti[id];
		if (!Input.GetButtonDown("Fire1"))
		{
			return;
		}
		Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
		if (Physics.Raycast(ray, out hit, 1000f))
		{
			Debug.Log(hit.collider);
			if (!DontDestroyOldObjectOnNew && oldObject.Count != 0)
			{
				destroyOldObject();
			}
			Vector3 point = hit.point;
			float x = point.x;
			float y = Offsets[id];
			Vector3 point2 = hit.point;
			Vector3 position = new Vector3(x, y, point2.z);
			instance = (Object.Instantiate((Object)effect, position, Quaternion.Euler(0f, 0f, 0f)) as GameObject);
			oldObject.Add(instance);
		}
	}

	private void getInputKey()
	{
		if (Input.GetKeyDown(KeyCode.Space))
		{
			if (!DontDestroyOldObjectOnNew && oldObject.Count != 0)
			{
				destroyOldObject();
			}
			id++;
			if (id >= listaLength)
			{
				id = 0;
			}
		}
	}

	private void destroyOldObject()
	{
		foreach (GameObject item in oldObject)
		{
			Object.Destroy(item);
		}
	}

	private void OnGUI()
	{
		customText.normal.textColor = textColor;
		customText.fontSize = fontSize;
		customText.padding.left = textPositionX;
		customText.padding.top = textPositionY;
		GUI.Label(new Rect(1f, 1f, Screen.width, Screen.height), Descrizioni[id], customText);
	}
}
