using UnityEngine;

public class ParticleAutoDestruction : MonoBehaviour
{
	private ParticleSystem[] particleSystems;

	private void Start()
	{
		particleSystems = GetComponentsInChildren<ParticleSystem>();
	}

	private void Update()
	{
		bool flag = true;
		ParticleSystem[] array = particleSystems;
		foreach (ParticleSystem particleSystem in array)
		{
			if (!particleSystem.isStopped)
			{
				flag = false;
			}
		}
		if (flag)
		{
			Object.Destroy(base.gameObject);
		}
	}
}
