using UnityEngine;

public class CameraRenderSpline : MonoBehaviour
{
	public CurvySpline Spline;

	private void OnPostRender()
	{
		if ((bool)Spline && Spline.IsInitialized)
		{
			Vector3[] approximation = Spline.GetApproximation();
			GL.Color(Color.white);
			GL.Begin(1);
			for (int i = 0; i < approximation.Length - 1; i++)
			{
				GL.Vertex(approximation[i]);
				GL.Vertex(approximation[i + 1]);
			}
			GL.End();
		}
	}
}
