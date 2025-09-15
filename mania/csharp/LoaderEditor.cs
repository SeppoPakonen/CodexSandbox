using UnityEngine;

[ExecuteInEditMode]
public class LoaderEditor : bs
{
	public void OnEnable()
	{
		Loader[] componentsInChildren = GetComponentsInChildren<Loader>();
		foreach (Loader loader in componentsInChildren)
		{
			loader.OnInit();
		}
	}
}
