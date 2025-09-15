using UnityEngine;

public class GLCurvyRenderer : MonoBehaviour
{
	public CurvySpline[] Splines;

	public Color[] Colors;

	private Vector3[] Points;

	private Material lineMaterial;

	private void CreateLineMaterial()
	{
		if (!lineMaterial)
		{
			lineMaterial = new Material("Shader \"Lines/Colored Blended\" {SubShader { Pass {     Blend SrcAlpha OneMinusSrcAlpha     ZWrite Off Cull Off Fog { Mode Off }     BindChannels {      Bind \"vertex\", vertex Bind \"color\", color }} } }");
			lineMaterial.hideFlags = (HideFlags.HideInHierarchy | HideFlags.DontSaveInEditor | HideFlags.NotEditable);
			lineMaterial.shader.hideFlags = (HideFlags.HideInHierarchy | HideFlags.DontSaveInEditor | HideFlags.NotEditable);
		}
	}

	private void OnPostRender()
	{
		if (Splines.Length == 0)
		{
			return;
		}
		for (int i = 0; i < Splines.Length; i++)
		{
			CurvySpline curvySpline = Splines[i];
			Color c = (i >= Colors.Length) ? Color.green : Colors[i];
			if ((bool)curvySpline && curvySpline.IsInitialized)
			{
				Points = curvySpline.GetApproximation();
				CreateLineMaterial();
				lineMaterial.SetPass(0);
				GL.Begin(1);
				GL.Color(c);
				for (int j = 1; j < Points.Length; j++)
				{
					GL.Vertex(Points[j - 1]);
					GL.Vertex(Points[j]);
				}
				GL.End();
			}
		}
	}
}
