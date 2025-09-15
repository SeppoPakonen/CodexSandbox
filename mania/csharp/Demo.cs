using System.Collections.Generic;
using UnityEngine;

public class Demo : MonoBehaviour
{
	public GameObject[] effects;

	private int selected;

	private GameObject instance;

	private bool loopedEffect;

	private void Start()
	{
		OnSwitch();
	}

	private void OnGUI()
	{
		List<string> list = new List<string>();
		GameObject[] array = effects;
		foreach (GameObject gameObject in array)
		{
			list.Add(gameObject.name);
		}
		int num = selected;
		selected = GUILayout.Toolbar(selected, list.ToArray());
		if (selected != num)
		{
			OnSwitch();
		}
	}

	private void OnSwitch()
	{
		if (loopedEffect && instance != null)
		{
			Object.Destroy(instance);
		}
		instance = (GameObject)Object.Instantiate((Object)effects[selected], base.transform.position, Quaternion.identity);
		if (instance.GetComponent<ParticleAutoDestruction>() != null)
		{
			loopedEffect = false;
		}
		else
		{
			loopedEffect = true;
		}
	}
}
