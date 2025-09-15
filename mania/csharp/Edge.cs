using System;

[Serializable]
internal class Edge
{
	public int[] vertexIndex = new int[2];

	public int[] faceIndex = new int[2];

	public override string ToString()
	{
		return vertexIndex[0] + "-" + vertexIndex[1];
	}
}
