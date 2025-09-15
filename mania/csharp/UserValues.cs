using UnityEngine;

public class UserValues : MonoBehaviour
{
	private SplineWalker walkerScript;

	private void Start()
	{
		walkerScript = GetComponent<SplineWalker>();
	}

	private void Update()
	{
		if ((bool)walkerScript)
		{
			base.transform.localScale = walkerScript.Spline.InterpolateUserValue(walkerScript.TF, 0);
		}
	}
}
