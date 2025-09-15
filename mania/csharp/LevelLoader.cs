using System.Collections;
using UnityEngine;

public class LevelLoader : MapLoader
{
	public new IEnumerator Start()
	{
		base.Start();
		yield return StartCoroutine(LoadMap(bs._Loader.curScene.url));
		if (userMapSucces)
		{
			yield return null;
			yield return StartCoroutine(ActiveEditor(editor: false));
			Object[] splines = Object.FindObjectsOfType(typeof(SplinePathMeshBuilder));
			Object[] array = splines;
			for (int i = 0; i < array.Length; i++)
			{
				SplinePathMeshBuilder a = (SplinePathMeshBuilder)array[i];
				a.enabled = false;
			}
			Object[] array2 = Object.FindObjectsOfType(typeof(CurvySpline2));
			for (int j = 0; j < array2.Length; j++)
			{
				CurvySpline2 a2 = (CurvySpline2)array2[j];
				a2.AutoRefresh = false;
			}
			GameObject start = GameObject.FindGameObjectWithTag("Start");
			GameObject checkpoint = GameObject.FindGameObjectWithTag("CheckPoint");
			MonoBehaviour.print("Splines " + splines.Length);
			MonoBehaviour.print(checkpoint);
			MonoBehaviour.print(start);
			Camera.main.gameObject.SetActive(value: false);
			LoadLevelAdditive("!2game");
		}
		else
		{
			LoadLevel("!1");
		}
	}
}
