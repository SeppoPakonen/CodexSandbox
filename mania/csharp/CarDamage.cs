using System;
using UnityEngine;

public class CarDamage : bs
{
	[Serializable]
	public class permaVertsColl
	{
		public Vector3[] permaVerts;
	}

	internal MeshFilter[] meshFilters;

	private MeshFilter[] m_meshFilters;

	public float deformNoise = 0.03f;

	public float deformRadius = 0.5f;

	private float bounceBackSleepCap = 0.002f;

	public float bounceBackSpeed = 2f;

	private permaVertsColl[] originalMeshData;

	private bool sleep = true;

	public float maxDeform = 0.5f;

	public float multiplier = 0.1f;

	public float YforceDamp = 1f;

	[HideInInspector]
	public bool repair;

	private Vector3 vec;

	private Transform myTransform;

	public void Start()
	{
		myTransform = base.transform;
		originalMeshData = new permaVertsColl[meshFilters.Length];
		for (int i = 0; i < meshFilters.Length; i++)
		{
			originalMeshData[i] = new permaVertsColl();
			originalMeshData[i].permaVerts = meshFilters[i].mesh.vertices;
		}
	}

	public void Update()
	{
		if (sleep || !repair || !(bounceBackSpeed > 0f))
		{
			return;
		}
		sleep = true;
		for (int i = 0; i < meshFilters.Length; i++)
		{
			Vector3[] vertices = meshFilters[i].mesh.vertices;
			for (int j = 0; j < vertices.Length; j++)
			{
				vertices[j] += (originalMeshData[i].permaVerts[j] - vertices[j]) * (Time.deltaTime * bounceBackSpeed);
				if ((originalMeshData[i].permaVerts[j] - vertices[j]).magnitude >= bounceBackSleepCap)
				{
					sleep = false;
				}
			}
			meshFilters[i].mesh.vertices = vertices;
			meshFilters[i].mesh.RecalculateNormals();
			meshFilters[i].mesh.RecalculateBounds();
		}
		if (sleep)
		{
			repair = false;
		}
	}

	public void OnHit(Collision collision, Vector3 colRelVel)
	{
		if (collision.contacts.Length > 0)
		{
			colRelVel.y *= YforceDamp;
			sleep = false;
			vec = myTransform.InverseTransformDirection(colRelVel) * multiplier * 0.1f;
			for (int i = 0; i < meshFilters.Length; i++)
			{
				DeformMesh(meshFilters[i].mesh, originalMeshData[i].permaVerts, collision, 1f, meshFilters[i].transform);
			}
		}
	}

	public void DeformMesh(Mesh mesh, Vector3[] originalMesh, Collision collision, float cos, Transform meshTransform)
	{
		Vector3[] vertices = mesh.vertices;
		ContactPoint[] contacts = collision.contacts;
		foreach (ContactPoint contactPoint in contacts)
		{
			Vector3 a = meshTransform.InverseTransformPoint(contactPoint.point);
			for (int j = 0; j < vertices.Length; j++)
			{
				if ((a - vertices[j]).magnitude < deformRadius)
				{
					vertices[j] += vec * (deformRadius - (a - vertices[j]).magnitude) / deformRadius * cos + UnityEngine.Random.onUnitSphere * deformNoise;
					if (maxDeform > 0f && (vertices[j] - originalMesh[j]).magnitude > maxDeform)
					{
						ref Vector3 reference = ref vertices[j];
						reference = originalMesh[j] + (vertices[j] - originalMesh[j]).normalized * maxDeform;
					}
				}
			}
		}
		mesh.vertices = vertices;
		mesh.RecalculateNormals();
		mesh.RecalculateBounds();
	}
}
