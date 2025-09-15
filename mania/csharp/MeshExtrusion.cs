using System;
using System.Collections;
using UnityEngine;

public class MeshExtrusion
{
	[Serializable]
	public class Edge
	{
		public int[] vertexIndex = new int[2];

		public int[] faceIndex = new int[2];
	}

	public static Mesh CreateMesh(Vector2[] poly)
	{
		Triangulator triangulator = new Triangulator(poly);
		int[] array = triangulator.Triangulate();
		Mesh mesh = new Mesh();
		Vector3[] array2 = new Vector3[poly.Length * 2];
		for (int i = 0; i < poly.Length; i++)
		{
			array2[i].x = poly[i].x;
			array2[i].y = poly[i].y;
			array2[i].z = -10f;
			array2[i + poly.Length].x = poly[i].x;
			array2[i + poly.Length].y = poly[i].y;
			array2[i + poly.Length].z = 10f;
		}
		int[] array3 = new int[array.Length * 2 + poly.Length * 6];
		int num = 0;
		for (int j = 0; j < array.Length; j += 3)
		{
			array3[j] = array[j];
			array3[j + 1] = array[j + 1];
			array3[j + 2] = array[j + 2];
		}
		num += array.Length;
		for (int k = 0; k < array.Length; k += 3)
		{
			array3[num + k] = array[k + 2] + poly.Length;
			array3[num + k + 1] = array[k + 1] + poly.Length;
			array3[num + k + 2] = array[k] + poly.Length;
		}
		num += array.Length;
		for (int l = 0; l < poly.Length; l++)
		{
			int num2 = (l + 1) % poly.Length;
			array3[num] = l;
			array3[num + 1] = l + poly.Length;
			array3[num + 2] = num2;
			array3[num + 3] = num2;
			array3[num + 4] = num2 + poly.Length;
			array3[num + 5] = l + poly.Length;
			num += 6;
		}
		mesh.vertices = array2;
		mesh.triangles = array3;
		mesh.RecalculateNormals();
		mesh.RecalculateBounds();
		mesh.Optimize();
		return mesh;
	}

	public static void ExtrudeMesh(Mesh srcMesh, Mesh extrudedMesh, Matrix4x4[] extrusion, bool invertFaces)
	{
		Edge[] edges = BuildManifoldEdges(srcMesh);
		ExtrudeMesh(srcMesh, extrudedMesh, extrusion, edges, invertFaces);
	}

	public static void ExtrudeMesh(Mesh srcMesh, Mesh extrudedMesh, Matrix4x4[] extrusion, Edge[] edges, bool invertFaces)
	{
		int num = edges.Length * 2 * extrusion.Length;
		int num2 = edges.Length * 6;
		int num3 = num2 * (extrusion.Length - 1);
		Vector3[] vertices = srcMesh.vertices;
		Vector2[] uv = srcMesh.uv;
		int[] triangles = srcMesh.triangles;
		Vector3[] array = new Vector3[num + srcMesh.vertexCount * 2];
		Vector2[] array2 = new Vector2[array.Length];
		int[] array3 = new int[num3 + triangles.Length * 2];
		int num4 = 0;
		for (int i = 0; i < extrusion.Length; i++)
		{
			Matrix4x4 matrix4x = extrusion[i];
			float y = (float)i / (float)(extrusion.Length - 1);
			foreach (Edge edge in edges)
			{
				ref Vector3 reference = ref array[num4];
				reference = matrix4x.MultiplyPoint(vertices[edge.vertexIndex[0]]);
				ref Vector3 reference2 = ref array[num4 + 1];
				reference2 = matrix4x.MultiplyPoint(vertices[edge.vertexIndex[1]]);
				ref Vector2 reference3 = ref array2[num4];
				reference3 = new Vector2(uv[edge.vertexIndex[0]].x, y);
				ref Vector2 reference4 = ref array2[num4 + 1];
				reference4 = new Vector2(uv[edge.vertexIndex[1]].x, y);
				num4 += 2;
			}
		}
		for (int k = 0; k < 2; k++)
		{
			Matrix4x4 matrix4x2 = extrusion[(k != 0) ? (extrusion.Length - 1) : 0];
			int num5 = (k != 0) ? (num + vertices.Length) : num;
			for (int l = 0; l < vertices.Length; l++)
			{
				ref Vector3 reference5 = ref array[num5 + l];
				reference5 = matrix4x2.MultiplyPoint(vertices[l]);
				ref Vector2 reference6 = ref array2[num5 + l];
				reference6 = uv[l];
			}
		}
		for (int m = 0; m < extrusion.Length - 1; m++)
		{
			int num6 = edges.Length * 2 * m;
			int num7 = edges.Length * 2 * (m + 1);
			for (int n = 0; n < edges.Length; n++)
			{
				int num8 = m * num2 + n * 6;
				array3[num8] = num6 + n * 2;
				array3[num8 + 1] = num7 + n * 2;
				array3[num8 + 2] = num6 + n * 2 + 1;
				array3[num8 + 3] = num7 + n * 2;
				array3[num8 + 4] = num7 + n * 2 + 1;
				array3[num8 + 5] = num6 + n * 2 + 1;
			}
		}
		int num9 = triangles.Length / 3;
		int num10 = num;
		int num11 = num3;
		for (int num12 = 0; num12 < num9; num12++)
		{
			array3[num12 * 3 + num11] = triangles[num12 * 3 + 1] + num10;
			array3[num12 * 3 + num11 + 1] = triangles[num12 * 3 + 2] + num10;
			array3[num12 * 3 + num11 + 2] = triangles[num12 * 3] + num10;
		}
		int num13 = num + vertices.Length;
		int num14 = num3 + triangles.Length;
		for (int num15 = 0; num15 < num9; num15++)
		{
			array3[num15 * 3 + num14] = triangles[num15 * 3] + num13;
			array3[num15 * 3 + num14 + 1] = triangles[num15 * 3 + 2] + num13;
			array3[num15 * 3 + num14 + 2] = triangles[num15 * 3 + 1] + num13;
		}
		if (invertFaces)
		{
			for (int num16 = 0; num16 < array3.Length / 3; num16++)
			{
				int num17 = array3[num16 * 3];
				array3[num16 * 3] = array3[num16 * 3 + 1];
				array3[num16 * 3 + 1] = num17;
			}
		}
		extrudedMesh.Clear();
		extrudedMesh.name = "extruded";
		extrudedMesh.vertices = array;
		extrudedMesh.uv = array2;
		extrudedMesh.triangles = array3;
		extrudedMesh.RecalculateNormals();
	}

	public static Edge[] BuildManifoldEdges(Mesh mesh)
	{
		Edge[] array = BuildEdges(mesh.vertexCount, mesh.triangles);
		ArrayList arrayList = new ArrayList();
		Edge[] array2 = array;
		foreach (Edge edge in array2)
		{
			if (edge.faceIndex[0] == edge.faceIndex[1])
			{
				arrayList.Add(edge);
			}
		}
		return arrayList.ToArray(typeof(Edge)) as Edge[];
	}

	public static Edge[] BuildEdges(int vertexCount, int[] triangleArray)
	{
		int num = triangleArray.Length;
		int[] array = new int[vertexCount + num];
		int num2 = triangleArray.Length / 3;
		for (int i = 0; i < vertexCount; i++)
		{
			array[i] = -1;
		}
		Edge[] array2 = new Edge[num];
		int num3 = 0;
		for (int j = 0; j < num2; j++)
		{
			int num4 = triangleArray[j * 3 + 2];
			for (int k = 0; k < 3; k++)
			{
				int num5 = triangleArray[j * 3 + k];
				if (num4 < num5)
				{
					Edge edge = new Edge();
					edge.vertexIndex[0] = num4;
					edge.vertexIndex[1] = num5;
					edge.faceIndex[0] = j;
					edge.faceIndex[1] = j;
					array2[num3] = edge;
					int num6 = array[num4];
					if (num6 == -1)
					{
						array[num4] = num3;
					}
					else
					{
						while (true)
						{
							int num7 = array[vertexCount + num6];
							if (num7 == -1)
							{
								break;
							}
							num6 = num7;
						}
						array[vertexCount + num6] = num3;
					}
					array[vertexCount + num3] = -1;
					num3++;
				}
				num4 = num5;
			}
		}
		for (int l = 0; l < num2; l++)
		{
			int num8 = triangleArray[l * 3 + 2];
			for (int m = 0; m < 3; m++)
			{
				int num9 = triangleArray[l * 3 + m];
				if (num8 > num9)
				{
					bool flag = false;
					for (int num10 = array[num9]; num10 != -1; num10 = array[vertexCount + num10])
					{
						Edge edge2 = array2[num10];
						if (edge2.vertexIndex[1] == num8 && edge2.faceIndex[0] == edge2.faceIndex[1])
						{
							array2[num10].faceIndex[1] = l;
							flag = true;
							break;
						}
					}
					if (!flag)
					{
						Edge edge3 = new Edge();
						edge3.vertexIndex[0] = num8;
						edge3.vertexIndex[1] = num9;
						edge3.faceIndex[0] = l;
						edge3.faceIndex[1] = l;
						array2[num3] = edge3;
						num3++;
					}
				}
				num8 = num9;
			}
		}
		Edge[] array3 = new Edge[num3];
		for (int n = 0; n < num3; n++)
		{
			array3[n] = array2[n];
		}
		return array3;
	}
}
