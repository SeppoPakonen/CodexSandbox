using System;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class GA_HeatMapRenderer : MonoBehaviour
{
	public enum RenderModel
	{
		Transparent,
		TransparentOverlay
	}

	public RenderModel CurrentRenderModel = RenderModel.TransparentOverlay;

	public Color MinColor = Color.yellow;

	public Color MaxColor = Color.red;

	public float RangeMin;

	public float RangeMax = 1f;

	public float MaxRadius = 1f;

	public bool ShowValueLabels;

	public GA_Histogram Histogram;

	public GA_Histogram.HistogramScale HistogramScale;

	public GA_HeatMapDataFilterBase datafilter;

	public GameObject BillBoard;

	private List<GameObject> DestroyList;

	private void OnEnable()
	{
		datafilter = GetComponent<GA_HeatMapDataFilterBase>();
	}

	public void OnDataUpdate()
	{
		if (DestroyList == null)
		{
			DestroyList = new List<GameObject>();
		}
		for (int i = 0; i < base.transform.childCount; i++)
		{
			GameObject gameObject = base.transform.GetChild(i).gameObject;
			if (!DestroyList.Contains(gameObject) && gameObject.GetComponent<GA_HeatmapData>() == null && gameObject.name.Contains("GA_"))
			{
				DestroyList.Add(gameObject);
			}
		}
		RenderModelChanged();
		if (datafilter == null)
		{
			datafilter = GetComponent<GA_HeatMapDataFilterBase>();
		}
		if (datafilter != null)
		{
			List<GA_DataPoint> data = datafilter.GetData();
			RecalculateHistogram();
			if (data == null || data.Count == 0)
			{
				GA.Log("GameAnalytics: No data to create heatmap. Returning.");
			}
			else
			{
				createBillboards(data);
			}
		}
		else
		{
			GA.Log("GameAnalytics: GA_HeatMapDataFilterBase component missing.");
		}
	}

	private void createBillboards(List<GA_DataPoint> data)
	{
		GA.Log("GameAnalytics: Creating mesh billboards for heapmap");
		if (data == null)
		{
			return;
		}
		int num = data.Count;
		int num2 = 0;
		while (num > 0)
		{
			int num3 = num;
			int num4 = (int)Mathf.Floor(21666.666f);
			if (num3 > num4)
			{
				num3 = num4;
			}
			BillBoard = new GameObject("GA_Billboards-" + num2 + "-" + (num2 + num3), typeof(MeshFilter), typeof(MeshRenderer));
			BillBoard.transform.parent = base.transform;
			BillBoard.transform.localPosition = Vector3.zero;
			Vector3[] array = new Vector3[num3 * 3];
			Vector3[] array2 = new Vector3[num3 * 3];
			Vector2[] array3 = new Vector2[num3 * 3];
			Color[] array4 = new Color[num3 * 3];
			int[] array5 = new int[num3 * 3];
			for (int i = 0; i < num3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					ref Vector3 reference = ref array[3 * i + j];
					reference = data[num2].position;
					ref Color reference2 = ref array4[3 * i + j];
					reference2 = new Color(data[num2].density, 0f, 0f, 1f);
					float f = (float)j * (float)Math.PI * 2f / 3f;
					ref Vector2 reference3 = ref array3[3 * i + j];
					reference3 = new Vector2(Mathf.Cos(f), Mathf.Sin(f));
					ref Vector3 reference4 = ref array2[3 * i + j];
					reference4 = Vector3.zero;
				}
				array5[i * 3] = i * 3;
				array5[i * 3 + 1] = i * 3 + 2;
				array5[i * 3 + 2] = i * 3 + 1;
				num2++;
			}
			MeshFilter component = BillBoard.GetComponent<MeshFilter>();
			Mesh mesh = new Mesh();
			mesh.vertices = array;
			mesh.normals = array2;
			mesh.uv = array3;
			mesh.colors = array4;
			mesh.triangles = array5;
			component.sharedMesh = mesh;
			component.sharedMesh.RecalculateBounds();
			num -= num3;
			SetMaterialVariables();
		}
	}

	public void SetMaterialVariables()
	{
		MeshRenderer[] componentsInChildren = base.gameObject.GetComponentsInChildren<MeshRenderer>();
		foreach (MeshRenderer meshRenderer in componentsInChildren)
		{
			if (meshRenderer.gameObject.name.Contains("GA_Billboards"))
			{
				if (meshRenderer.sharedMaterial == null)
				{
					meshRenderer.sharedMaterial = new Material(Shader.Find("Custom/GA_HeatMapSolid"));
				}
				RenderModel currentRenderModel = CurrentRenderModel;
				if (currentRenderModel == RenderModel.TransparentOverlay)
				{
					meshRenderer.sharedMaterial.shader = Shader.Find("Custom/GA_HeatMapTransparentOverlay");
				}
				else
				{
					meshRenderer.sharedMaterial.shader = Shader.Find("Custom/GA_HeatMapTransparent");
				}
				meshRenderer.sharedMaterial.SetColor("_Cold", MinColor);
				meshRenderer.sharedMaterial.SetColor("_Warm", MaxColor);
				meshRenderer.sharedMaterial.SetFloat("_RangeMin", RangeMin);
				meshRenderer.sharedMaterial.SetFloat("_RangeWidth", RangeMax - RangeMin);
				meshRenderer.sharedMaterial.SetFloat("_MaxRadius", MaxRadius);
				float value = (0f - Histogram.RealDataMin) / (Histogram.RealDataMax - Histogram.RealDataMin);
				meshRenderer.sharedMaterial.SetFloat("_RangeZero", value);
				meshRenderer.sharedMaterial.SetColor("_ColZero", Color.white);
			}
		}
	}

	public void RecalculateHistogram()
	{
		if (datafilter != null)
		{
			List<GA_DataPoint> data = datafilter.GetData();
			if (data != null)
			{
				Histogram.Recalculate(data, 40, HistogramScale);
			}
		}
	}

	public void RenderModelChanged()
	{
		SetMaterialVariables();
	}

	public void OnScaleChanged()
	{
		RecalculateHistogram();
	}

	private void Update()
	{
		if (DestroyList == null)
		{
			return;
		}
		foreach (GameObject destroy in DestroyList)
		{
			if (Application.isPlaying)
			{
				UnityEngine.Object.Destroy(destroy);
			}
		}
	}
}
