using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Performance : MonoBehaviour
{
	public static string Prefix;

	public static List<Result> Results = new List<Result>();

	public static int Runs = 50000;

	private int splineSelection;

	private int granularity = 20;

	private bool includeDependent = true;

	private bool includeGetNearestPoint;

	private bool includeIndependent = true;

	private bool includeMisc = true;

	private bool running;

	private CurvyInterpolation splineType;

	private List<CurvySpline> Splines = new List<CurvySpline>();

	private Vector2 scroll;

	private void Start()
	{
	}

	private void OnGUI()
	{
		GUILayout.BeginArea(new Rect(10f, 10f, 950f, 600f));
		GUILayout.BeginHorizontal();
		GUILayout.Label("Spline Type: ");
		splineSelection = GUILayout.SelectionGrid(splineSelection, new string[3]
		{
			"Linear",
			"Catmul-Rom",
			"TCB"
		}, 3);
		GUILayout.EndHorizontal();
		switch (splineSelection)
		{
		case 0:
			splineType = CurvyInterpolation.Linear;
			Prefix = "Linear";
			break;
		case 1:
			splineType = CurvyInterpolation.CatmulRom;
			Prefix = "Catmul";
			break;
		case 2:
			splineType = CurvyInterpolation.TCB;
			Prefix = "TCB";
			break;
		}
		Prefix = Prefix + " G=" + granularity;
		GUILayout.BeginHorizontal();
		GUILayout.Label("Spline Granularity: " + granularity);
		int num = granularity;
		granularity = (int)GUILayout.HorizontalSlider(granularity, 1f, 100f);
		GUILayout.EndHorizontal();
		if (Splines.Count > 0 && (splineType != Splines[0].Interpolation || granularity != num))
		{
			Clear();
		}
		includeDependent = GUILayout.Toggle(includeDependent, "Run tests that vary by different Spline Types");
		includeIndependent = GUILayout.Toggle(includeIndependent, "Run tests that vary by different Granularity");
		includeMisc = GUILayout.Toggle(includeMisc, "Run tests for misc. methods (conversions, etc.)");
		includeGetNearestPoint = GUILayout.Toggle(includeGetNearestPoint, "Run CurvySpline.GetNearestPointTF() - takes some time");
		GUILayout.BeginHorizontal();
		if (GUILayout.Button("Clear Result"))
		{
			Results.Clear();
		}
		GUI.enabled = !running;
		if (GUILayout.Button((!running) ? "Run!" : "Please wait..."))
		{
			StartCoroutine(RunTests());
		}
		if (!Application.isWebPlayer)
		{
			GUI.enabled = (Results.Count > 0);
			if (GUILayout.Button("Save as CSV"))
			{
				SaveToCSV();
			}
		}
		GUI.enabled = true;
		GUILayout.EndHorizontal();
		GUILayout.Label("* 'Create & Precalculate' as an exception is a single run only, the other values are calculated by averaging 50k runs (including time needed to get random values)!");
		GUILayout.Label("* Times are in Milliseconds");
		GUILayout.BeginHorizontal();
		GUILayout.Label("Test", GUILayout.Width(400f));
		GUILayout.Label("2 Segments", GUILayout.Width(120f));
		GUILayout.Label("10 Segments", GUILayout.Width(120f));
		GUILayout.Label("50 Segments", GUILayout.Width(120f));
		GUILayout.Label("100 Segments", GUILayout.Width(120f));
		GUILayout.EndHorizontal();
		scroll = GUILayout.BeginScrollView(scroll);
		foreach (Result result in Results)
		{
			result.OnGUI();
		}
		GUILayout.EndScrollView();
		GUILayout.EndArea();
	}

	private void Clear()
	{
		foreach (CurvySpline spline in Splines)
		{
			UnityEngine.Object.Destroy(spline.gameObject);
		}
		Splines.Clear();
	}

	private IEnumerator RunTests()
	{
		running = true;
		yield return null;
		if (Splines.Count == 0)
		{
			using (Measure measure = new Measure("Create & Precalculate"))
			{
				measure.Start();
				Splines.Add(CreateSpline(splineType, 3, granularity));
				measure.StartNext();
				Splines.Add(CreateSpline(splineType, 11, granularity));
				measure.StartNext();
				Splines.Add(CreateSpline(splineType, 51, granularity));
				measure.StartNext();
				Splines.Add(CreateSpline(splineType, 101, granularity));
				measure.Stop();
			}
		}
		if (includeDependent)
		{
			using (Measure measure2 = new Measure("CurvySpline.Interpolate(TF)"))
			{
				measure2.Start();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[0].Interpolate(UnityEngine.Random.value);
				});
				measure2.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[1].Interpolate(UnityEngine.Random.value);
				});
				measure2.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[2].Interpolate(UnityEngine.Random.value);
				});
				measure2.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[3].Interpolate(UnityEngine.Random.value);
				});
				measure2.Stop();
			}
			yield return null;
		}
		if (includeIndependent)
		{
			using (Measure measure3 = new Measure("CurvySpline.InterpolateFast(TF)"))
			{
				measure3.Start();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[0].InterpolateFast(UnityEngine.Random.value);
				});
				measure3.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[1].InterpolateFast(UnityEngine.Random.value);
				});
				measure3.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[2].InterpolateFast(UnityEngine.Random.value);
				});
				measure3.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[3].InterpolateFast(UnityEngine.Random.value);
				});
				measure3.Stop();
			}
			yield return null;
		}
		if (includeDependent)
		{
			using (Measure measure4 = new Measure("CurvySpline.GetTangent(TF)"))
			{
				measure4.Start();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[0].GetTangent(UnityEngine.Random.value);
				});
				measure4.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[1].GetTangent(UnityEngine.Random.value);
				});
				measure4.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[2].GetTangent(UnityEngine.Random.value);
				});
				measure4.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[3].GetTangent(UnityEngine.Random.value);
				});
				measure4.Stop();
			}
			yield return null;
		}
		if (includeIndependent)
		{
			using (Measure measure5 = new Measure("CurvySpline.GetTangentFast(TF)"))
			{
				measure5.Start();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[0].GetTangentFast(UnityEngine.Random.value);
				});
				measure5.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[1].GetTangentFast(UnityEngine.Random.value);
				});
				measure5.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[2].GetTangentFast(UnityEngine.Random.value);
				});
				measure5.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[3].GetTangentFast(UnityEngine.Random.value);
				});
				measure5.Stop();
			}
			yield return null;
			using (Measure measure6 = new Measure("CurvySpline.GetOrientationFast(TF)"))
			{
				measure6.Start();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[0].GetOrientationFast(UnityEngine.Random.value);
				});
				measure6.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[1].GetOrientationFast(UnityEngine.Random.value);
				});
				measure6.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[2].GetOrientationFast(UnityEngine.Random.value);
				});
				measure6.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[3].GetOrientationFast(UnityEngine.Random.value);
				});
				measure6.Stop();
			}
			yield return null;
			using (Measure measure7 = new Measure("CurvySpline.GetOrientationUpFast(TF)"))
			{
				measure7.Start();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[0].GetOrientationUpFast(UnityEngine.Random.value);
				});
				measure7.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[1].GetOrientationUpFast(UnityEngine.Random.value);
				});
				measure7.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[2].GetOrientationUpFast(UnityEngine.Random.value);
				});
				measure7.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[3].GetOrientationUpFast(UnityEngine.Random.value);
				});
				measure7.Stop();
			}
			yield return null;
		}
		if (includeMisc)
		{
			using (Measure measure8 = new Measure("CurvySpline.DistanceToSegment"))
			{
				measure8.Start();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[0].DistanceToSegment(UnityEngine.Random.Range(0f, Splines[0].Length));
				});
				measure8.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[1].DistanceToSegment(UnityEngine.Random.Range(0f, Splines[1].Length));
				});
				measure8.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[2].DistanceToSegment(UnityEngine.Random.Range(0f, Splines[2].Length));
				});
				measure8.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[3].DistanceToSegment(UnityEngine.Random.Range(0f, Splines[3].Length));
				});
				measure8.Stop();
			}
			yield return null;
			using (Measure measure9 = new Measure("CurvySpline.DistanceToTF"))
			{
				measure9.Start();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[0].DistanceToTF(UnityEngine.Random.Range(0f, Splines[0].Length));
				});
				measure9.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[1].DistanceToTF(UnityEngine.Random.Range(0f, Splines[1].Length));
				});
				measure9.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[2].DistanceToTF(UnityEngine.Random.Range(0f, Splines[2].Length));
				});
				measure9.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[3].DistanceToTF(UnityEngine.Random.Range(0f, Splines[3].Length));
				});
				measure9.Stop();
			}
			yield return null;
			using (Measure measure10 = new Measure("CurvySpline.TFToSegment"))
			{
				measure10.Start();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[0].TFToSegment(UnityEngine.Random.value);
				});
				measure10.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[1].TFToSegment(UnityEngine.Random.value);
				});
				measure10.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[2].TFToSegment(UnityEngine.Random.value);
				});
				measure10.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[3].TFToSegment(UnityEngine.Random.value);
				});
				measure10.Stop();
			}
			yield return null;
			using (Measure measure11 = new Measure("CurvySpline.TFToDistance"))
			{
				measure11.Start();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[0].TFToDistance(UnityEngine.Random.value);
				});
				measure11.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[1].TFToDistance(UnityEngine.Random.value);
				});
				measure11.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[2].TFToDistance(UnityEngine.Random.value);
				});
				measure11.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[3].TFToDistance(UnityEngine.Random.value);
				});
				measure11.Stop();
			}
			yield return null;
		}
		if (includeDependent)
		{
			using (Measure measure12 = new Measure("CurvySplineSegment.Interpolate(F)"))
			{
				measure12.Start();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[0][0].Interpolate(UnityEngine.Random.value);
				});
				measure12.Stop();
				measure12.AddLast();
				measure12.AddLast();
				measure12.AddLast();
			}
			yield return null;
		}
		if (includeIndependent)
		{
			using (Measure measure13 = new Measure("CurvySplineSegment.InterpolateFast(F)"))
			{
				measure13.Start();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[0][0].InterpolateFast(UnityEngine.Random.value);
				});
				measure13.Stop();
				measure13.AddLast();
				measure13.AddLast();
				measure13.AddLast();
			}
			yield return null;
		}
		if (includeMisc)
		{
			using (Measure measure14 = new Measure("CurvySplineSegment.DistanceToLocalF"))
			{
				measure14.Start();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[0][0].DistanceToLocalF(UnityEngine.Random.Range(0f, Splines[0].Length));
				});
				measure14.Stop();
				measure14.AddLast();
				measure14.AddLast();
				measure14.AddLast();
			}
			yield return null;
			using (Measure measure15 = new Measure("CurvySplineSegment.LocalFToDistance"))
			{
				measure15.Start();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[0][0].LocalFToDistance(UnityEngine.Random.value);
				});
				measure15.Stop();
				measure15.AddLast();
				measure15.AddLast();
				measure15.AddLast();
			}
			yield return null;
			using (Measure measure16 = new Measure("CurvySplineSegment.LocalFToTF"))
			{
				measure16.Start();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[0][0].LocalFToTF(UnityEngine.Random.value);
				});
				measure16.Stop();
				measure16.AddLast();
				measure16.AddLast();
				measure16.AddLast();
			}
			yield return null;
		}
		if (includeGetNearestPoint)
		{
			using (Measure M = new Measure("CurvySpline.GetNearestPointTF()"))
			{
				M.Start();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[0].GetNearestPointTF(UnityEngine.Random.insideUnitCircle * 10f);
				});
				M.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[1].GetNearestPointTF(UnityEngine.Random.insideUnitCircle * 10f);
				});
				M.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[2].GetNearestPointTF(UnityEngine.Random.insideUnitCircle * 10f);
				});
				M.StartNext();
				RunMultiple((Action)(object)(Action)delegate
				{
					Splines[3].GetNearestPointTF(UnityEngine.Random.insideUnitCircle * 10f);
				});
				M.Stop();
			}
		}
		running = false;
	}

	private void RunMultiple(Action param)
	{
		for (int i = 0; i < Runs; i++)
		{
			param.Invoke();
		}
	}

	private CurvySpline CreateSpline(CurvyInterpolation type, int points, int granularity)
	{
		CurvySpline curvySpline = CurvySpline.Create();
		curvySpline.Interpolation = type;
		curvySpline.Granularity = granularity;
		curvySpline.Closed = false;
		curvySpline.ShowGizmos = false;
		curvySpline.AutoEndTangents = true;
		for (int i = 0; i < points; i++)
		{
			curvySpline.Add(null, refresh: false).Position = UnityEngine.Random.insideUnitCircle * 10f;
		}
		curvySpline.Refresh();
		return curvySpline;
	}

	private void SaveToCSV()
	{
	}
}
