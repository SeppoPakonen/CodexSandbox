using System.Collections;
using UnityEngine;

public class CBExample : MonoBehaviour
{
	public GameObject[] CloneGroup1;

	public GameObject[] CloneGroup2;

	public GameObject[] CloneGroup3;

	public string[] CloneGroupNames;

	private SplinePathCloneBuilder mBuilder;

	private CurvySpline mSpline;

	private Transform mCPToMove;

	private int mCurGroup;

	private IEnumerator Start()
	{
		mBuilder = GetComponent<SplinePathCloneBuilder>();
		mSpline = mBuilder.Spline;
		while (!mSpline.IsInitialized)
		{
			yield return 0;
		}
		mCPToMove = mBuilder.Spline.ControlPoints[1].Transform;
		UpdateClones();
	}

	private void Update()
	{
		if ((bool)mCPToMove)
		{
			mCPToMove.Translate(Vector3.up * Mathf.Sin(Mathf.PingPong(Time.time, 2f) - 1f) * 5f * Time.deltaTime);
		}
	}

	private void OnGUI()
	{
		if ((bool)mSpline && mSpline.IsInitialized)
		{
			int num = mCurGroup;
			GUILayout.BeginHorizontal();
			mCurGroup = GUILayout.SelectionGrid(mCurGroup, CloneGroupNames, 4);
			GUILayout.Label("Gap");
			mBuilder.Gap = GUILayout.HorizontalSlider(mBuilder.Gap, 0f, 1f, GUILayout.Width(150f));
			GUILayout.Label("Swirl");
			mSpline.SwirlTurns = GUILayout.HorizontalSlider(mSpline.SwirlTurns, 0f, 1f, GUILayout.Width(150f));
			GUILayout.EndHorizontal();
			if (num != mCurGroup)
			{
				num = mCurGroup;
				UpdateClones();
			}
		}
	}

	private void UpdateClones()
	{
		mBuilder.Clear();
		switch (mCurGroup)
		{
		case 0:
			mBuilder.Source = CloneGroup1;
			break;
		case 1:
			mBuilder.Source = CloneGroup2;
			break;
		case 2:
			mBuilder.Source = CloneGroup3;
			break;
		}
		mBuilder.Refresh(force: true);
	}
}
