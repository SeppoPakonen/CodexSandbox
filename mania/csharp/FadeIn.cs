using System.Collections;
using UnityEngine;

public class FadeIn : MonoBehaviour
{
	public Transform GO;

	private bool startAnimation;

	private int levelIndex;

	private IEnumerator Start()
	{
		Object.DontDestroyOnLoad(base.gameObject);
		((Component)GO).get_renderer().material.color = new Color(1f, 1f, 1f, 0f);
		yield return StartCoroutine(waitAndStart());
	}

	private void Update()
	{
		if (startAnimation)
		{
			((Component)GO).get_renderer().material.color = Color.Lerp(((Component)GO).get_renderer().material.color, new Color(1f, 1f, 1f, 1f), 0.5f * Time.deltaTime);
		}
	}

	private IEnumerator waitAndStart()
	{
		yield return new WaitForSeconds(8.8f);
		startAnimation = true;
	}

	private void OnGUI()
	{
		if (!startAnimation)
		{
			return;
		}
		if (levelIndex == 0)
		{
			if (GUI.Button(new Rect(Screen.width / 2 - 100, (float)Screen.height / 1.2f, 200f, 30f), "START"))
			{
				startDemo();
			}
		}
		else if (GUI.Button(new Rect(Screen.width / 2 - 100, (float)Screen.height / 1.2f, 200f, 30f), "NEXT DEMO"))
		{
			startDemo();
		}
	}

	private void startDemo()
	{
		if (levelIndex >= 9)
		{
			levelIndex = 0;
			Application.LoadLevel(levelIndex);
			Object.Destroy(base.gameObject);
		}
		else
		{
			base.transform.position = new Vector3(0f, -1000f, 0f);
			levelIndex++;
			Application.LoadLevel(levelIndex);
		}
	}
}
