using UnityEngine;

public class MyProperty
{
	public enum ET
	{
		None,
		boola,
		color,
		floata,
		inta,
		AnimCurve
	}

	[HideInInspector]
	public AnimationCurve animationCurveValue;

	[HideInInspector]
	public bool boolValue;

	[HideInInspector]
	public Color colorValue;

	[HideInInspector]
	public float floatValue;

	[HideInInspector]
	public int intValue;

	[HideInInspector]
	public string stringValue;

	[HideInInspector]
	public Vector2 vector2Value;

	[HideInInspector]
	public Vector3 vector3Value;

	public ET et;

	public string monoName;

	public string fieldName;

	public MyProperty SetValue(string s, object v, string index)
	{
		fieldName = s;
		monoName = index;
		if (v is bool)
		{
			boolValue = (bool)v;
			et = ET.boola;
		}
		else if (v is Color)
		{
			colorValue = (Color)v;
			et = ET.color;
		}
		else if (v is int)
		{
			intValue = (int)v;
			et = ET.inta;
		}
		else if (v is float)
		{
			floatValue = (float)v;
			et = ET.floata;
		}
		else
		{
			if (!(v is AnimationCurve))
			{
				return null;
			}
			animationCurveValue = (AnimationCurve)v;
			et = ET.AnimCurve;
		}
		return this;
	}

	public object getValue()
	{
		if (et == ET.AnimCurve)
		{
			return animationCurveValue;
		}
		if (et == ET.boola)
		{
			return boolValue;
		}
		if (et == ET.color)
		{
			return colorValue;
		}
		if (et == ET.floata)
		{
			return floatValue;
		}
		if (et == ET.inta)
		{
			return intValue;
		}
		Debug.Log("Value not found " + et);
		return null;
	}
}
