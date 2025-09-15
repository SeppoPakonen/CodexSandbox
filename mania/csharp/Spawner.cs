using UnityEngine;

public class Spawner : MonoBehaviour
{
	public GameObject ObjectSpawn;

	private float timeSpawnTemp;

	public float TimeSpawn = 20f;

	public float ObjectCount;

	public int Radiun;

	private void Start()
	{
		if ((bool)base.get_renderer())
		{
			base.get_renderer().enabled = false;
		}
	}

	private void Update()
	{
		if ((bool)ObjectSpawn)
		{
			GameObject[] array = GameObject.FindGameObjectsWithTag("Enemy");
			if ((float)array.Length < ObjectCount && Time.time >= timeSpawnTemp + TimeSpawn)
			{
				GameObject gameObject = (GameObject)Object.Instantiate((Object)ObjectSpawn, base.transform.position + new Vector3(Random.Range(-Radiun, Radiun), 20f, Random.Range(-Radiun, Radiun)), Quaternion.identity);
				float num = Random.Range(5, 20);
				gameObject.transform.localScale = new Vector3(num, num, num);
				timeSpawnTemp = Time.time;
			}
		}
	}
}
