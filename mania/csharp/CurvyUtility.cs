using UnityEngine;

public class CurvyUtility
{
	public static bool isPlanar(CurvySpline spline, out int ignoreAxis)
	{
		bool xplanar;
		bool yplanar;
		bool zplanar;
		bool result = isPlanar(spline, out xplanar, out yplanar, out zplanar);
		if (xplanar)
		{
			ignoreAxis = 0;
		}
		else if (yplanar)
		{
			ignoreAxis = 1;
		}
		else
		{
			ignoreAxis = 2;
		}
		return result;
	}

	public static bool isPlanar(CurvySpline spline, out bool xplanar, out bool yplanar, out bool zplanar)
	{
		xplanar = true;
		yplanar = true;
		zplanar = true;
		if (spline.ControlPointCount == 0)
		{
			return false;
		}
		Vector3 position = spline.ControlPoints[0].Position;
		for (int i = 1; i < spline.ControlPointCount; i++)
		{
			Vector3 position2 = spline.ControlPoints[i].Position;
			if (!Mathf.Approximately(position2.x, position.x))
			{
				xplanar = false;
			}
			Vector3 position3 = spline.ControlPoints[i].Position;
			if (!Mathf.Approximately(position3.y, position.y))
			{
				yplanar = false;
			}
			Vector3 position4 = spline.ControlPoints[i].Position;
			if (!Mathf.Approximately(position4.z, position.z))
			{
				zplanar = false;
			}
			if (!xplanar && !yplanar && !zplanar)
			{
				return false;
			}
		}
		return true;
	}

	public static void makePlanar(CurvySpline spline, int axis)
	{
		Vector3 position = spline.ControlPoints[0].Position;
		for (int i = 1; i < spline.ControlPointCount; i++)
		{
			Vector3 position2 = spline.ControlPoints[i].Position;
			switch (axis)
			{
			case 0:
				position2.x = position.x;
				break;
			case 1:
				position2.y = position.y;
				break;
			case 2:
				position2.z = position.z;
				break;
			}
			spline.ControlPoints[i].Position = position2;
		}
		spline.Refresh();
	}

	public static void centerPivot(CurvySpline spline)
	{
		Bounds bounds = spline.GetBounds(local: false);
		Vector3 vector = spline.Transform.position - bounds.center;
		foreach (CurvySplineSegment controlPoint in spline.ControlPoints)
		{
			controlPoint.Transform.position += vector;
		}
		spline.transform.position -= vector;
		spline.Refresh();
	}

	public static void setFirstCP(CurvySplineSegment newStartCP)
	{
		CurvySpline spline = newStartCP.Spline;
		if (newStartCP.ControlPointIndex != 0)
		{
			CurvySplineSegment[] array = new CurvySplineSegment[newStartCP.ControlPointIndex];
			for (int i = 0; i < newStartCP.ControlPointIndex; i++)
			{
				array[i] = spline.ControlPoints[i];
			}
			CurvySplineSegment[] array2 = array;
			foreach (CurvySplineSegment item in array2)
			{
				spline.ControlPoints.Remove(item);
				spline.ControlPoints.Add(item);
			}
			spline._nameControlPoints();
			spline.Refresh();
		}
	}
}
