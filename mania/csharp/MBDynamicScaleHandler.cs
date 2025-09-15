using System.Collections;
using UnityEngine;

public class MBDynamicScaleHandler : MonoBehaviour
{
	public SplinePathMeshBuilder SplineMesh;

	private SplineWalker Walker;

	private float meshMinTF;

	private float meshMaxTF;

	private float sphereRadius = 1.5f;

	private bool insideMesh;

	private IEnumerator Start()
	{
		Walker = GetComponent<SplineWalker>();
		while (!Walker.Spline.IsInitialized)
		{
			yield return null;
		}
		SplineMesh.OnGetScale += SplineMesh_OnGetScale;
		meshMinTF = SplineMesh.Spline.DistanceToTF(SplineMesh.Spline.TFToDistance(SplineMesh.FromTF) - sphereRadius);
		meshMaxTF = SplineMesh.Spline.DistanceToTF(SplineMesh.Spline.TFToDistance(SplineMesh.ToTF) + sphereRadius);
	}

	private void OnDestroy()
	{
		SplineMesh.OnGetScale -= SplineMesh_OnGetScale;
	}

	private void Update()
	{
		if ((bool)SplineMesh)
		{
			bool flag = insideMesh;
			insideMesh = (Walker.TF >= meshMinTF && Walker.TF <= meshMaxTF);
			if (insideMesh || flag != insideMesh)
			{
				SplineMesh.Refresh();
			}
			flag = insideMesh;
		}
	}

	private Vector3 SplineMesh_OnGetScale(SplinePathMeshBuilder sender, float tf)
	{
		if (insideMesh)
		{
			float num = Walker.Spline.TFToDistance(Walker.TF);
			float num2 = Walker.Spline.TFToDistance(tf);
			float f = num - num2;
			float num3 = Mathf.Abs(f);
			float d = 0f;
			if (num3 <= sphereRadius)
			{
				d = Mathf.Cos(num3 / sphereRadius);
			}
			return Vector3.one + Vector3.one * d;
		}
		return Vector3.one;
	}
}
