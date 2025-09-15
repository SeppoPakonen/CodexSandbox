using UnityEngine;

public class switchTarget : MonoBehaviour
{
	private hayate hyt;

	public GameObject target1;

	public GameObject target2;

	public GameObject target3;

	public Transform particleSystem;

	private float sliderValue;

	private void Start()
	{
		hyt = particleSystem.GetComponent<hayate>();
	}

	private void OnGUI()
	{
		hyt.particleSpeedToMesh = GUILayout.HorizontalSlider(hyt.particleSpeedToMesh, 0f, 20f);
		GUILayout.Label(hyt.particleSpeedToMesh.ToString());
		if (GUILayout.Button("Interpolation mode"))
		{
			hyt.byDistance = !particleSystem.GetComponent<hayate>().byDistance;
		}
		if (GUILayout.Button("Cube"))
		{
			hyt.meshTarget = target1;
		}
		if (GUILayout.Button("Cylinder"))
		{
			hyt.meshTarget = target2;
		}
		if (GUILayout.Button("Torusknot"))
		{
			hyt.meshTarget = target3;
		}
	}
}
