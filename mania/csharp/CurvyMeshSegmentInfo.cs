using UnityEngine;

internal struct CurvyMeshSegmentInfo
{
	public CurvySpline Spline;

	public float TF;

	private float mDistance;

	public Matrix4x4 Matrix;

	public float Distance => (mDistance != -1f) ? mDistance : Spline.TFToDistance(TF);

	public CurvyMeshSegmentInfo(SplinePathMeshBuilder mb, float tf, Vector3 scale)
	{
		Spline = mb.Spline;
		TF = tf;
		mDistance = -1f;
		Vector3 position = (!mb.FastInterpolation) ? Spline.Interpolate(TF) : Spline.InterpolateFast(TF);
		if (mb.UseWorldPosition)
		{
			Matrix = Matrix4x4.TRS(mb.Transform.InverseTransformPoint(position), Spline.GetOrientationFast(TF), scale);
		}
		else
		{
			Matrix = Matrix4x4.TRS(Spline.Transform.InverseTransformPoint(position), Spline.GetOrientationFast(TF), scale);
		}
	}

	public CurvyMeshSegmentInfo(SplinePathMeshBuilder mb, float tf, float distance, Vector3 scale)
	{
		Spline = mb.Spline;
		TF = tf;
		mDistance = distance;
		Vector3 position = (!mb.FastInterpolation) ? Spline.Interpolate(TF) : Spline.InterpolateFast(TF);
		if (mb.UseWorldPosition)
		{
			Matrix = Matrix4x4.TRS(mb.Transform.InverseTransformPoint(position), Spline.GetOrientationFast(TF), scale);
		}
		else
		{
			Matrix = Matrix4x4.TRS(Spline.Transform.InverseTransformPoint(position), Spline.GetOrientationFast(TF), scale);
		}
	}
}
