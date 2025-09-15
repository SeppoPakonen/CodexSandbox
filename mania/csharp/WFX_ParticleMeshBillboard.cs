using UnityEngine;

[RequireComponent(typeof(ParticleSystemRenderer))]
public class WFX_ParticleMeshBillboard : MonoBehaviour
{
	private Mesh mesh;

	private Vector3[] vertices;

	private Vector3[] rvertices;

	private void Awake()
	{
		mesh = (Mesh)Object.Instantiate((Object)GetComponent<ParticleSystemRenderer>().mesh);
		GetComponent<ParticleSystemRenderer>().mesh = mesh;
		vertices = new Vector3[mesh.vertices.Length];
		for (int i = 0; i < vertices.Length; i++)
		{
			ref Vector3 reference = ref vertices[i];
			reference = mesh.vertices[i];
		}
		rvertices = new Vector3[vertices.Length];
	}

	private void OnWillRenderObject()
	{
		if (!(mesh == null) && !(Camera.current == null))
		{
			Quaternion rotation = Quaternion.LookRotation(Camera.current.transform.forward, Camera.current.transform.up);
			Quaternion rotation2 = Quaternion.Inverse(base.transform.rotation);
			for (int i = 0; i < rvertices.Length; i++)
			{
				ref Vector3 reference = ref rvertices[i];
				reference = rotation * vertices[i];
				ref Vector3 reference2 = ref rvertices[i];
				reference2 = rotation2 * rvertices[i];
			}
			mesh.vertices = rvertices;
		}
	}
}
