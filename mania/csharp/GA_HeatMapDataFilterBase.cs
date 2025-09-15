using System.Collections.Generic;
using UnityEngine;

public abstract class GA_HeatMapDataFilterBase : MonoBehaviour
{
	public delegate void DataDownloadDelegate(GA_HeatMapDataFilterBase sender);

	public event DataDownloadDelegate OnDataUpdate;

	public abstract List<GA_DataPoint> GetData();

	public void OnDataUpdated()
	{
		if (this.OnDataUpdate != null)
		{
			this.OnDataUpdate(this);
		}
	}
}
