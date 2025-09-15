using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
[RequireComponent(typeof(GA_HeatMapRenderer))]
public class GA_HeatMapDataFilter : GA_HeatMapDataFilterBase
{
	public enum CombineHeatmapType
	{
		None,
		Add,
		Subtract,
		SubtractZero
	}

	[HideInInspector]
	public List<string> AvailableTypes;

	[HideInInspector]
	public int CurrentTypeIndex;

	[HideInInspector]
	public List<string> AvailableBuilds;

	[HideInInspector]
	public int CurrentBuildIndex;

	public bool RedownloadDataOnPlay;

	public bool IgnoreDates = true;

	public bool IgnoreBuilds = true;

	public bool DownloadingData;

	public bool BuildingHeatmap;

	public float BuildHeatmapPercentage;

	[SerializeField]
	private string _startDateTime;

	[SerializeField]
	private string _endDateTime;

	[HideInInspector]
	public List<string> AvailableAreas;

	[HideInInspector]
	public int CurrentAreaIndex;

	[HideInInspector]
	public List<string> AvailableEvents;

	[HideInInspector]
	public List<bool> CurrentEventFlag;

	[HideInInspector]
	public GA_HeatmapData DataContainer;

	[SerializeField]
	private bool didInit;

	private CombineHeatmapType _combineType;

	public float LoadProgress;

	public bool Loading;

	public DateTime? StartDateTime
	{
		get
		{
			DateTime result = DateTime.Now;
			DateTime.TryParse(_startDateTime, out result);
			return result;
		}
		set
		{
			_startDateTime = ((!value.HasValue) ? string.Empty : value.Value.ToString());
		}
	}

	public DateTime? EndDateTime
	{
		get
		{
			DateTime result = DateTime.Now;
			DateTime.TryParse(_endDateTime, out result);
			return result;
		}
		set
		{
			_endDateTime = ((!value.HasValue) ? string.Empty : value.Value.ToString());
		}
	}

	public void OnEnable()
	{
		if (!didInit)
		{
			Debug.Break();
			AvailableBuilds = new List<string>();
			AvailableAreas = new List<string>();
			AvailableTypes = new List<string>();
			AvailableEvents = new List<string>();
			AvailableTypes.Add("Design");
			AvailableTypes.Add("Quality");
			AvailableTypes.Add("Business");
			StartDateTime = DateTime.Now;
			EndDateTime = DateTime.Now;
			didInit = true;
		}
	}

	private void Awake()
	{
		if (Application.isPlaying && !Application.isEditor && RedownloadDataOnPlay)
		{
			DownloadData();
		}
	}

	public override List<GA_DataPoint> GetData()
	{
		return (!(DataContainer != null)) ? null : DataContainer.Data;
	}

	private IEnumerator FollowProgress(WWW progress)
	{
		Loading = true;
		while (!progress.isDone)
		{
			LoadProgress = progress.progress;
			yield return null;
		}
		Loading = false;
		LoadProgress = 0f;
	}

	private void NormalizeDataPoints(List<GA_DataPoint> Data)
	{
		float num = 0f;
		float num2 = 0f;
		foreach (GA_DataPoint Datum in Data)
		{
			if (num < Datum.count)
			{
				num = Datum.count;
			}
			if (num2 > Datum.count)
			{
				num2 = Datum.count;
			}
		}
		foreach (GA_DataPoint Datum2 in Data)
		{
			Datum2.density = (Datum2.count - num2) / (num - num2);
		}
	}

	public void OnSuccessDownload(GA_Request.RequestType requestType, Hashtable jsonList, GA_Request.SubmitErrorHandler errorEvent)
	{
		DownloadingData = false;
		BuildingHeatmap = true;
		if (DataContainer == null)
		{
			GameObject gameObject = new GameObject("GA_Data");
			gameObject.transform.parent = base.transform;
			DataContainer = gameObject.AddComponent<GA_HeatmapData>();
			DataContainer.Data = new List<GA_DataPoint>();
			GA.Log(DataContainer);
		}
		else if (_combineType == CombineHeatmapType.None)
		{
			DataContainer.Data.Clear();
		}
		List<GA_DataPoint> list = new List<GA_DataPoint>();
		ArrayList arrayList = (ArrayList)jsonList["x"];
		ArrayList arrayList2 = (ArrayList)jsonList["y"];
		ArrayList arrayList3 = (ArrayList)jsonList["z"];
		ArrayList arrayList4 = (ArrayList)jsonList["value"];
		for (int i = 0; i < arrayList.Count; i++)
		{
			try
			{
				bool flag = false;
				if (_combineType == CombineHeatmapType.Add)
				{
					Vector3 rhs = new Vector3(float.Parse(arrayList[i].ToString()) / GA.SettingsGA.HeatmapGridSize.x, float.Parse(arrayList2[i].ToString()) / GA.SettingsGA.HeatmapGridSize.y, float.Parse(arrayList3[i].ToString()) / GA.SettingsGA.HeatmapGridSize.z);
					int num = int.Parse(arrayList4[i].ToString());
					for (int j = 0; j < DataContainer.Data.Count; j++)
					{
						if (DataContainer.Data[j].position == rhs)
						{
							DataContainer.Data[j].count += num;
							flag = true;
							j = DataContainer.Data.Count;
						}
					}
				}
				else if (_combineType == CombineHeatmapType.Subtract)
				{
					Vector3 rhs2 = new Vector3(float.Parse(arrayList[i].ToString()) / GA.SettingsGA.HeatmapGridSize.x, float.Parse(arrayList2[i].ToString()) / GA.SettingsGA.HeatmapGridSize.y, float.Parse(arrayList3[i].ToString()) / GA.SettingsGA.HeatmapGridSize.z);
					int num2 = int.Parse(arrayList4[i].ToString());
					for (int k = 0; k < DataContainer.Data.Count; k++)
					{
						if (DataContainer.Data[k].position == rhs2)
						{
							DataContainer.Data[k].count = DataContainer.Data[k].count - (float)num2;
							k = DataContainer.Data.Count;
							flag = true;
						}
					}
				}
				else if (_combineType == CombineHeatmapType.SubtractZero)
				{
					flag = true;
					Vector3 rhs3 = new Vector3(float.Parse(arrayList[i].ToString()) / GA.SettingsGA.HeatmapGridSize.x, float.Parse(arrayList2[i].ToString()) / GA.SettingsGA.HeatmapGridSize.y, float.Parse(arrayList3[i].ToString()) / GA.SettingsGA.HeatmapGridSize.z);
					int num3 = int.Parse(arrayList4[i].ToString());
					for (int l = 0; l < DataContainer.Data.Count; l++)
					{
						if (DataContainer.Data[l].position == rhs3)
						{
							DataContainer.Data[l].count = Mathf.Max(DataContainer.Data[l].count - (float)num3, 0f);
							if (DataContainer.Data[l].count == 0f)
							{
								list.Add(DataContainer.Data[l]);
							}
							l = DataContainer.Data.Count;
						}
					}
				}
				if (_combineType == CombineHeatmapType.Subtract && !flag)
				{
					GA_DataPoint gA_DataPoint = new GA_DataPoint();
					gA_DataPoint.position = new Vector3(float.Parse(arrayList[i].ToString()) / GA.SettingsGA.HeatmapGridSize.x, float.Parse(arrayList2[i].ToString()) / GA.SettingsGA.HeatmapGridSize.y, float.Parse(arrayList3[i].ToString()) / GA.SettingsGA.HeatmapGridSize.z);
					gA_DataPoint.count = -int.Parse(arrayList4[i].ToString());
					DataContainer.Data.Add(gA_DataPoint);
				}
				else if (_combineType != CombineHeatmapType.Subtract && (_combineType == CombineHeatmapType.None || !flag))
				{
					GA_DataPoint gA_DataPoint2 = new GA_DataPoint();
					gA_DataPoint2.position = new Vector3(float.Parse(arrayList[i].ToString()) / GA.SettingsGA.HeatmapGridSize.x, float.Parse(arrayList2[i].ToString()) / GA.SettingsGA.HeatmapGridSize.y, float.Parse(arrayList3[i].ToString()) / GA.SettingsGA.HeatmapGridSize.z);
					gA_DataPoint2.count = int.Parse(arrayList4[i].ToString());
					DataContainer.Data.Add(gA_DataPoint2);
				}
			}
			catch (Exception ex)
			{
				GA.LogError("GameAnalytics: Error in parsing JSON data from server - " + ex.Message);
			}
			BuildHeatmapPercentage = i * 100 / arrayList.Count;
		}
		foreach (GA_DataPoint item in list)
		{
			DataContainer.Data.Remove(item);
		}
		BuildingHeatmap = false;
		BuildHeatmapPercentage = 0f;
		NormalizeDataPoints(DataContainer.Data);
		GetComponent<GA_HeatMapRenderer>().OnDataUpdate();
		Loading = false;
	}

	public void OnErrorDownload(string msg)
	{
		GA.Log("GameAnalytics: Download failed: " + msg);
		DownloadingData = false;
		Loading = false;
	}

	public void SetCombineHeatmapType(CombineHeatmapType combineType)
	{
		_combineType = combineType;
	}

	public void DownloadData()
	{
		DownloadingData = true;
		List<string> list = new List<string>();
		for (int i = 0; i < AvailableEvents.Count; i++)
		{
			if (CurrentEventFlag[i])
			{
				list.Add(AvailableEvents[i]);
			}
		}
		bool flag = false;
		if (CurrentAreaIndex >= AvailableAreas.Count)
		{
			GA.LogWarning("GameAnalytics: Warning: Area selected is out of bounds. Not downloading data.");
			flag = true;
		}
		string build = string.Empty;
		if (!IgnoreBuilds)
		{
			if (CurrentBuildIndex >= AvailableBuilds.Count)
			{
				GA.LogWarning("GameAnalytics: Warning: Build selected is out of bounds. Not downloading data.");
				flag = true;
			}
			else
			{
				build = AvailableBuilds[CurrentBuildIndex];
			}
		}
		if (flag)
		{
			DownloadingData = false;
		}
		else
		{
			GA.API.Request.RequestHeatmapData(list, AvailableAreas[CurrentAreaIndex], build, (!IgnoreDates) ? StartDateTime : null, (!IgnoreDates) ? EndDateTime : null, OnSuccessDownload, OnErrorDownload);
		}
	}

	public void DownloadUpdate()
	{
		DownloadData();
	}

	public void OnSuccessDownloadInfo(GA_Request.RequestType requestType, Hashtable jsonList, GA_Request.SubmitErrorHandler errorEvent)
	{
		GA.Log("Succesful index downloaded");
		CurrentAreaIndex = 0;
		CurrentTypeIndex = 0;
		CurrentBuildIndex = 0;
		StartDateTime = DateTime.Now.AddDays(-14.0);
		EndDateTime = DateTime.Now;
		AvailableEvents = new List<string>();
		CurrentEventFlag = new List<bool>();
		AvailableAreas = new List<string>();
		AvailableBuilds = new List<string>();
		IgnoreDates = true;
		ArrayList arrayList = (ArrayList)jsonList["event_id"];
		for (int i = 0; i < arrayList.Count; i++)
		{
			try
			{
				string item = arrayList[i].ToString();
				AvailableEvents.Add(item);
				CurrentEventFlag.Add(item: false);
			}
			catch
			{
				GA.LogError("GameAnalytics: Error in parsing JSON data from server");
			}
		}
		ArrayList arrayList2 = (ArrayList)jsonList["area"];
		for (int j = 0; j < arrayList2.Count; j++)
		{
			try
			{
				string item2 = arrayList2[j].ToString();
				AvailableAreas.Add(item2);
			}
			catch
			{
				GA.LogError("GameAnalytics: Error in parsing JSON data from server");
			}
		}
		ArrayList arrayList3 = (ArrayList)jsonList["build"];
		for (int k = 0; k < arrayList3.Count; k++)
		{
			try
			{
				string item3 = arrayList3[k].ToString();
				AvailableBuilds.Add(item3);
			}
			catch
			{
				GA.LogError("GameAnalytics: Error in parsing JSON data from server");
			}
		}
	}

	public void OnErrorDownloadInfo(string msg)
	{
		GA.Log("GameAnalytics: Download failed: " + msg);
	}

	public void UpdateIndexData()
	{
		GA.Log("Downloading index from server");
		GA.API.Request.RequestGameInfo(OnSuccessDownloadInfo, OnErrorDownloadInfo);
	}
}
