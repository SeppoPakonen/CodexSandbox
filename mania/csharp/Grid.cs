using UnityEngine;

public class Grid : bs
{
	private Transform t;

	internal bool renderAwalys;

	internal float cellSmall = 1f;

	internal float cellBig = 10f;

	internal Color cellSmallColor = Color.white * 0.3f;

	internal Color cellBigColor = Color.white * 0.5f;

	public void Start()
	{
		t = base.transform;
		if (bs.android)
		{
			base.gameObject.SetActive(value: false);
		}
	}

	public void OnRenderObject()
	{
		if (renderAwalys || (!(bs._Loader.levelEditor == null) && bs._Loader.levelEditor.shapeEditor))
		{
			bs.res.lineMaterialYellow.SetPass(0);
			GL.Begin(1);
			GL.Color(cellSmallColor);
			Draw(cellSmall, 0f);
			if (cellBig != 0f)
			{
				GL.Color(cellBigColor);
				Draw(cellBig, 0.3f);
			}
			GL.End();
		}
	}

	private void Draw(float sz, float h = 0f)
	{
		float num = 300f / sz;
		float num2 = sz * num;
		for (int i = (int)(0f - num); (float)i < num; i++)
		{
			Vertex3((float)i * sz, h, num2);
			Vertex3((float)i * sz, h, 0f - num2);
		}
		for (int j = (int)(0f - num); (float)j < num; j++)
		{
			Vertex3(0f - num2, h, (float)j * sz);
			Vertex3(num2, h, (float)j * sz);
		}
	}

	private void Vertex3(float x, float y, float z)
	{
		Vector3 position = t.position;
		GL.Vertex3(x + position.x, y + position.y, z + position.z);
	}
}
