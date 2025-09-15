using System;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
public class GA_Histogram
{
	public enum HistogramScale
	{
		Linear,
		Logarithmic
	}

	public float min = float.PositiveInfinity;

	public float max;

	public float RealDataMin;

	public float RealDataMax;

	public float[] Data;

	public void Recalculate(List<GA_DataPoint> data, int numChunks, HistogramScale scale)
	{
		GA.Log("GameAnalytics: Recalculating Histogram");
		min = float.PositiveInfinity;
		max = 0f;
		List<int> list = new List<int>();
		foreach (GA_DataPoint datum in data)
		{
			if (datum.count < min)
			{
				min = datum.count;
			}
			if (datum.count > max)
			{
				max = datum.count;
			}
			if (!list.Contains((int)datum.count))
			{
				list.Add((int)datum.count);
			}
		}
		RealDataMin = min;
		RealDataMax = max;
		float num = max - min;
		numChunks = Mathf.Min(list.Count, numChunks);
		Data = new float[numChunks];
		for (int i = 0; i < numChunks; i++)
		{
			Data[i] = 0f;
		}
		foreach (GA_DataPoint datum2 in data)
		{
			int value = Mathf.FloorToInt((datum2.count - min) / num * (float)numChunks);
			value = Mathf.Clamp(value, 0, numChunks - 1);
			Data[value] += 1f;
		}
		float num2 = float.NegativeInfinity;
		float num3 = float.PositiveInfinity;
		float num4 = 0f;
		float[] data2 = Data;
		foreach (float num5 in data2)
		{
			num4 = num5;
			if (num4 < num3)
			{
				num3 = num4;
			}
			if (num4 > num2)
			{
				num2 = num4;
			}
		}
		float num6 = 0f;
		for (int k = 0; k < Data.Length; k++)
		{
			num6 = ((scale != 0) ? Mathf.Log(Data[k]) : Data[k]);
			Data[k] = (num6 - num3) / (num2 - num3);
		}
	}
}
