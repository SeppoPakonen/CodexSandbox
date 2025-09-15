using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MeshHelper
{
	private static Edge getNextEdge(int curVI, ref List<Edge> pool, out int nextVI)
	{
		foreach (Edge item in pool)
		{
			if (item.vertexIndex[0] == curVI)
			{
				nextVI = item.vertexIndex[1];
				return item;
			}
			if (item.vertexIndex[1] == curVI)
			{
				nextVI = item.vertexIndex[0];
				return item;
			}
		}
		nextVI = -1;
		Debug.LogError("Curvy Mesh Builder: Open Edge Loop detected! Please check your StartMesh!");
		return null;
	}

	internal static EdgeLoop[] BuildEdgeLoops(Edge[] manifoldEdges)
	{
		List<EdgeLoop> list = new List<EdgeLoop>();
		if (manifoldEdges.Length == 0)
		{
			return list.ToArray();
		}
		List<Edge> pool = new List<Edge>(manifoldEdges);
		List<int> list2 = new List<int>();
		list2.Add(pool[0].vertexIndex[0]);
		list2.Add(pool[0].vertexIndex[1]);
		int num = pool[0].vertexIndex[1];
		pool.RemoveAt(0);
		while (pool.Count > 0)
		{
			int nextVI;
			Edge nextEdge = getNextEdge(num, ref pool, out nextVI);
			if (nextEdge == null)
			{
				return new EdgeLoop[0];
			}
			list2.Add(nextVI);
			num = nextVI;
			pool.Remove(nextEdge);
			if (num == list2[0])
			{
				list.Add(new EdgeLoop(list2));
				list2.Clear();
				if (pool.Count > 0)
				{
					list2.Add(pool[0].vertexIndex[0]);
					list2.Add(pool[0].vertexIndex[1]);
					num = pool[0].vertexIndex[1];
					pool.RemoveAt(0);
				}
			}
		}
		if (list2.Count > 0)
		{
			list.Add(new EdgeLoop(list2));
		}
		return list.ToArray();
	}

	internal static Edge[] BuildManifoldEdges(Mesh mesh)
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

	internal static Edge[] BuildEdges(int vertexCount, int[] triangleArray)
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

	public static Mesh CreateLineMesh(float width)
	{
		if (width <= 0f)
		{
			return null;
		}
		Mesh mesh = new Mesh();
		mesh.vertices = new Vector3[2]
		{
			new Vector3((0f - width) / 2f, 0f, 0f),
			new Vector3(width / 2f, 0f, 0f)
		};
		mesh.uv = new Vector2[2]
		{
			new Vector2(0f, 0f),
			new Vector2(1f, 0f)
		};
		mesh.RecalculateBounds();
		return mesh;
	}

	public static Mesh CreateNgonMesh(int n, float radius, float hollowPercent)
	{
		if (n < 3)
		{
			return null;
		}
		Mesh mesh = new Mesh();
		mesh.name = "Ngon";
		float num = (float)Math.PI * 2f / (float)n;
		Vector3[] array;
		Vector2[] array2;
		int[] array3;
		if (hollowPercent == 0f)
		{
			array = new Vector3[n + 1];
			array2 = new Vector2[n + 1];
			array3 = new int[n * 3];
			ref Vector3 reference = ref array[0];
			reference = new Vector3(0f, 0f, 0f);
			ref Vector2 reference2 = ref array2[0];
			reference2 = new Vector2(0.5f, 0.5f);
			for (int i = 0; i < n; i++)
			{
				ref Vector3 reference3 = ref array[i + 1];
				reference3 = new Vector3(Mathf.Sin((float)i * num) * radius, Mathf.Cos((float)i * num) * radius, 0f);
				ref Vector2 reference4 = ref array2[i + 1];
				reference4 = new Vector2((1f + Mathf.Sin((float)i * num)) * 0.5f, (1f + Mathf.Cos((float)i * num)) * 0.5f);
			}
			for (int j = 0; j < n; j++)
			{
				array3[j * 3] = 0;
				array3[j * 3 + 1] = j + 1;
				array3[j * 3 + 2] = j + 2;
			}
			array3[n * 3 - 1] = 1;
		}
		else
		{
			array = new Vector3[n * 2];
			array2 = new Vector2[n * 2];
			array3 = new int[n * 6];
			for (int k = 0; k < n; k++)
			{
				ref Vector3 reference5 = ref array[k];
				reference5 = new Vector3(Mathf.Sin((float)k * num) * radius, Mathf.Cos((float)k * num) * radius, 0f);
				ref Vector3 reference6 = ref array[k + n];
				reference6 = new Vector3(Mathf.Sin((float)k * num) * radius * hollowPercent, Mathf.Cos((float)k * num) * radius * hollowPercent, 0f);
				ref Vector2 reference7 = ref array2[k];
				reference7 = new Vector2((1f + Mathf.Sin((float)k * num)) * 0.5f, (1f + Mathf.Cos((float)k * num)) * 0.5f);
				ref Vector2 reference8 = ref array2[k + n];
				reference8 = new Vector2((1f + Mathf.Sin((float)k * num) * hollowPercent) * 0.5f, (1f + Mathf.Cos((float)k * num) * hollowPercent) * 0.5f);
			}
			int num2 = 0;
			for (int l = 0; l < n - 1; l++)
			{
				array3[num2] = l;
				array3[num2 + 1] = l + 1;
				array3[num2 + 2] = l + n;
				array3[num2 + 3] = l + n;
				array3[num2 + 4] = l + 1;
				array3[num2 + 5] = l + n + 1;
				num2 += 6;
			}
			array3[num2] = n - 1;
			array3[num2 + 1] = 0;
			array3[num2 + 2] = n - 1 + n;
			array3[num2 + 3] = n - 1 + n;
			array3[num2 + 4] = 0;
			array3[num2 + 5] = n;
		}
		mesh.vertices = array;
		mesh.triangles = array3;
		mesh.uv = array2;
		return mesh;
	}

	public static Mesh CreateRectangleMesh(float width, float height, float hollowPercent)
	{
		if (width <= 0f || height < 0f)
		{
			return null;
		}
		Mesh mesh = new Mesh();
		float num = width / 2f;
		float num2 = height / 2f;
		if (hollowPercent <= 0f)
		{
			mesh.vertices = new Vector3[4]
			{
				new Vector3(0f - num, num2, 0f),
				new Vector3(num, num2, 0f),
				new Vector3(num, 0f - num2, 0f),
				new Vector3(0f - num, 0f - num2, 0f)
			};
			mesh.uv = new Vector2[4]
			{
				new Vector2(0f, 1f),
				new Vector2(1f, 1f),
				new Vector2(1f, 0f),
				new Vector2(0f, 0f)
			};
			mesh.triangles = new int[6]
			{
				0,
				1,
				2,
				2,
				3,
				0
			};
		}
		else
		{
			float num3 = num * hollowPercent;
			float num4 = num2 * hollowPercent;
			float num5 = hollowPercent * 0.5f;
			mesh.vertices = new Vector3[8]
			{
				new Vector3(0f - num, num2, 0f),
				new Vector3(num, num2, 0f),
				new Vector3(num, 0f - num2, 0f),
				new Vector3(0f - num, 0f - num2, 0f),
				new Vector3(0f - num3, num4, 0f),
				new Vector3(num3, num4, 0f),
				new Vector3(num3, 0f - num4, 0f),
				new Vector3(0f - num3, 0f - num4, 0f)
			};
			mesh.uv = new Vector2[8]
			{
				new Vector2(0f, 1f),
				new Vector2(1f, 1f),
				new Vector2(1f, 0f),
				new Vector2(0f, 0f),
				new Vector2(0.5f - num5, 0.5f + num5),
				new Vector2(0.5f + num5, 0.5f + num5),
				new Vector2(0.5f + num5, 0.5f - num5),
				new Vector2(0.5f - num5, 0.5f - num5)
			};
			mesh.triangles = new int[24]
			{
				0,
				1,
				5,
				5,
				4,
				0,
				5,
				1,
				6,
				1,
				2,
				6,
				2,
				7,
				6,
				7,
				2,
				3,
				3,
				4,
				7,
				3,
				0,
				4
			};
		}
		return mesh;
	}

	public static Mesh CreateSplineMesh(CurvySpline spline, int ignoreAxis, bool close, float angleDiff)
	{
		float tf = 0f;
		int direction = 1;
		List<Vector3> list = new List<Vector3>();
		list.Add(spline.Transform.worldToLocalMatrix.MultiplyPoint3x4(spline.Interpolate(0f)));
		while (tf < 1f)
		{
			list.Add(spline.Transform.worldToLocalMatrix.MultiplyPoint3x4(spline.MoveByAngle(ref tf, ref direction, angleDiff, CurvyClamping.Clamp, 0.005f)));
		}
		return buildSplineMesh(list.ToArray(), ignoreAxis, !close);
	}

	public static Mesh CreateSplineMesh(CurvySpline spline, int ignoreAxis, bool close, Mesh msh = null)
	{
		Vector3[] userValues = spline.GetApproximation(local: true);
		if (spline.Closed)
		{
			CurvySplineSegment.Resize(ref userValues, userValues.Length - 1);
		}
		return buildSplineMesh(userValues, ignoreAxis, !close, msh);
	}

	private static Mesh buildSplineMesh(Vector3[] vertices, int ignoreAxis, bool noTrisAtAll, Mesh msh = null)
	{
		int[] triangles = (!noTrisAtAll) ? new Triangulator(vertices, ignoreAxis).Triangulate() : new int[0];
		if (msh == null)
		{
			msh = new Mesh();
		}
		msh.vertices = vertices;
		msh.triangles = triangles;
		msh.RecalculateBounds();
		Vector2[] array = new Vector2[vertices.Length];
		Bounds bounds = msh.bounds;
		switch (ignoreAxis)
		{
		case 0:
		{
			Vector3 min3 = bounds.min;
			float x = min3.y;
			Vector3 size3 = bounds.size;
			float x2 = size3.y;
			Vector3 min4 = bounds.min;
			float y = min4.z;
			Vector3 size4 = bounds.size;
			float y2 = size4.z;
			for (int j = 0; j < vertices.Length; j++)
			{
				ref Vector2 reference2 = ref array[j];
				reference2 = new Vector2((vertices[j].y - x) / x2, (vertices[j].z - y) / y2);
			}
			break;
		}
		case 1:
		{
			Vector3 min5 = bounds.min;
			float x = min5.x;
			Vector3 size5 = bounds.size;
			float x2 = size5.x;
			Vector3 min6 = bounds.min;
			float y = min6.z;
			Vector3 size6 = bounds.size;
			float y2 = size6.z;
			for (int k = 0; k < vertices.Length; k++)
			{
				ref Vector2 reference3 = ref array[k];
				reference3 = new Vector2((vertices[k].x - x) / x2, (vertices[k].z - y) / y2);
			}
			break;
		}
		default:
		{
			Vector3 min = bounds.min;
			float x = min.x;
			Vector3 size = bounds.size;
			float x2 = size.x;
			Vector3 min2 = bounds.min;
			float y = min2.y;
			Vector3 size2 = bounds.size;
			float y2 = size2.y;
			for (int i = 0; i < vertices.Length; i++)
			{
				ref Vector2 reference = ref array[i];
				reference = new Vector2((vertices[i].x - x) / x2, (vertices[i].y - y) / y2);
			}
			break;
		}
		}
		msh.uv = array;
		msh.RecalculateNormals();
		return msh;
	}
}
