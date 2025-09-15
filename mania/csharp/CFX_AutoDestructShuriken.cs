using System.Collections;
using UnityEngine;

[RequireComponent(typeof(ParticleSystem))]
public class CFX_AutoDestructShuriken : MonoBehaviour
{
	public bool OnlyDeactivate;

	public ParticleSystem particleSystem => GetComponent<ParticleSystem>();

	private void OnEnable()
	{
		StartCoroutine("CheckIfAlive");
	}

	private IEnumerator CheckIfAlive()
	{
		do
		{
			yield return new WaitForSeconds(0.5f);
		}
		while (particleSystem.IsAlive(withChildren: true));
		if (!OnlyDeactivate)
		{
			Object.Destroy(base.gameObject);
		}
	}
}
