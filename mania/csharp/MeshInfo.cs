using System;
using UnityEngine;

[Serializable]
public class MeshInfo
{
	internal EdgeLoop[] EdgeLoops = new EdgeLoop[0];

	public Vector3[] EdgeVertices = new Vector3[0];

	public Vector2[] EdgeUV = new Vector2[0];

	public Vector3[] Vertices;

	public Vector2[] UVs;

	public int[] Triangles;

	public int VertexCount => Vertices.Length;

	public int LoopVertexCount => EdgeVertices.Length;

	public int LoopTriIndexCount
	{
		get
		{
			int num = 0;
			EdgeLoop[] edgeLoops = EdgeLoops;
			foreach (EdgeLoop edgeLoop in edgeLoops)
			{
				num += edgeLoop.TriIndexCount;
			}
			return num;
		}
	}

	public MeshInfo(Mesh mesh, bool calculateLoops, bool mirror)
	{
		if (mirror)
		{
			mesh = MirrorMesh(mesh);
		}
		Vector3[] vertices = mesh.vertices;
		Vector2[] uv = mesh.uv;
		int[] triangles = mesh.triangles;
		Vertices = new Vector3[vertices.Length];
		UVs = new Vector2[uv.Length];
		Triangles = new int[triangles.Length];
		vertices.CopyTo(Vertices, 0);
		uv.CopyTo(UVs, 0);
		triangles.CopyTo(Triangles, 0);
		if (!calculateLoops)
		{
			return;
		}
		EdgeLoops = MeshHelper.BuildEdgeLoops(MeshHelper.BuildManifoldEdges(mesh));
		EdgeVertices = new Vector3[vertices.Length];
		EdgeUV = new Vector2[uv.Length];
		int num = int.MaxValue;
		int num2 = 0;
		EdgeLoop[] edgeLoops = EdgeLoops;
		foreach (EdgeLoop edgeLoop in edgeLoops)
		{
			for (int j = 0; j < edgeLoop.vertexCount; j++)
			{
				ref Vector3 reference = ref EdgeVertices[num2 + j];
				reference = vertices[edgeLoop.vertexIndex[j]];
				ref Vector2 reference2 = ref EdgeUV[num2 + j];
				reference2 = uv[edgeLoop.vertexIndex[j]];
				num = Mathf.Min(num, edgeLoop.vertexIndex[j]);
			}
			num2 += edgeLoop.vertexCount;
		}
		EdgeLoop[] edgeLoops2 = EdgeLoops;
		foreach (EdgeLoop edgeLoop2 in edgeLoops2)
		{
			edgeLoop2.ShiftIndices(-num);
		}
		CurvySplineSegment.Resize(ref EdgeVertices, num2);
		CurvySplineSegment.Resize(ref EdgeUV, num2);
	}

	public Vector3[] TRSVertices(Matrix4x4 matrix)
	{
		Vector3[] array = new Vector3[Vertices.Length];
		for (int i = 0; i < array.Length; i++)
		{
			ref Vector3 reference = ref array[i];
			reference = matrix.MultiplyPoint3x4(Vertices[i]);
		}
		return array;
	}

	private Mesh MirrorMesh(Mesh mesh)
	{
		Vector3[] vertices = mesh.vertices;
		Vector2[] uv = mesh.uv;
		Vector3[] normals = mesh.normals;
		int[] triangles = mesh.triangles;
		Mesh mesh2 = new Mesh();
		for (int i = 0; i < vertices.Length; i++)
		{
			vertices[i].z *= -1f;
		}
		for (int j = 0; j < normals.Length; j++)
		{
			normals[j] *= -1f;
		}
		for (int k = 0; k < triangles.Length; k += 3)
		{
			int num = triangles[k];
			triangles[k] = triangles[k + 1];
			triangles[k + 1] = num;
		}
		mesh2.vertices = vertices;
		mesh2.uv = uv;
		mesh2.normals = normals;
		mesh2.triangles = triangles;
		return mesh2;
	}
}
