using UnityEngine;

public class Skidmarks : MonoBehaviour
{
	public class markSection
	{
		public Vector3 pos;

		public Vector3 normal;

		public Vector4 tangent;

		public Vector3 posl;

		public Vector3 posr;

		public float intensity;

		public int lastIndex;
	}

	internal int maxMarks = 256;

	public float markWidth = 0.275f;

	public float groundOffset = 0.02f;

	public float minDistance = 0.1f;

	private int numMarks;

	private markSection[] skidmarks;

	private bool updated;

	public void Reset()
	{
		Awake();
		numMarks = 0;
	}

	public void Awake()
	{
		skidmarks = new markSection[maxMarks];
		for (int i = 0; i < maxMarks; i++)
		{
			skidmarks[i] = new markSection();
		}
		if (GetComponent<MeshFilter>().mesh == null)
		{
			GetComponent<MeshFilter>().mesh = new Mesh();
		}
	}

	public int AddSkidMark(Vector3 pos, Vector3 normal, float intensity, int lastIndex)
	{
		if (intensity > 1f)
		{
			intensity = 1f;
		}
		if (intensity < 0f)
		{
			return -1;
		}
		if (skidmarks == null)
		{
			return -1;
		}
		markSection markSection = skidmarks[numMarks % maxMarks];
		markSection.pos = pos + normal * groundOffset;
		markSection.normal = normal;
		markSection.intensity = intensity;
		markSection.lastIndex = lastIndex;
		if (lastIndex != -1)
		{
			markSection markSection2 = skidmarks[lastIndex % maxMarks];
			Vector3 lhs = markSection.pos - markSection2.pos;
			Vector3 normalized = Vector3.Cross(lhs, normal).normalized;
			markSection.posl = markSection.pos + normalized * markWidth * 0.5f;
			markSection.posr = markSection.pos - normalized * markWidth * 0.5f;
			markSection.tangent = new Vector4(normalized.x, normalized.y, normalized.z, 1f);
			if (markSection2.lastIndex == -1)
			{
				markSection2.tangent = markSection.tangent;
				markSection2.posl = markSection.pos + normalized * markWidth * 0.5f;
				markSection2.posr = markSection.pos - normalized * markWidth * 0.5f;
			}
		}
		numMarks++;
		updated = true;
		return numMarks - 1;
	}

	public void LateUpdate()
	{
		if (!updated)
		{
			return;
		}
		updated = false;
		Mesh mesh = GetComponent<MeshFilter>().mesh;
		mesh.Clear();
		int num = 0;
		for (int i = 0; i < numMarks && i < maxMarks; i++)
		{
			if (skidmarks[i].lastIndex != -1 && skidmarks[i].lastIndex > numMarks - maxMarks)
			{
				num++;
			}
		}
		Vector3[] array = new Vector3[num * 4];
		Vector3[] array2 = new Vector3[num * 4];
		Vector4[] array3 = new Vector4[num * 4];
		Color[] array4 = new Color[num * 4];
		Vector2[] array5 = new Vector2[num * 4];
		int[] array6 = new int[num * 6];
		num = 0;
		for (int j = 0; j < numMarks && j < maxMarks; j++)
		{
			if (skidmarks[j].lastIndex != -1 && skidmarks[j].lastIndex > numMarks - maxMarks)
			{
				markSection markSection = skidmarks[j];
				markSection markSection2 = skidmarks[markSection.lastIndex % maxMarks];
				ref Vector3 reference = ref array[num * 4];
				reference = markSection2.posl;
				ref Vector3 reference2 = ref array[num * 4 + 1];
				reference2 = markSection2.posr;
				ref Vector3 reference3 = ref array[num * 4 + 2];
				reference3 = markSection.posl;
				ref Vector3 reference4 = ref array[num * 4 + 3];
				reference4 = markSection.posr;
				ref Vector3 reference5 = ref array2[num * 4];
				reference5 = markSection2.normal;
				ref Vector3 reference6 = ref array2[num * 4 + 1];
				reference6 = markSection2.normal;
				ref Vector3 reference7 = ref array2[num * 4 + 2];
				reference7 = markSection.normal;
				ref Vector3 reference8 = ref array2[num * 4 + 3];
				reference8 = markSection.normal;
				ref Vector4 reference9 = ref array3[num * 4];
				reference9 = markSection2.tangent;
				ref Vector4 reference10 = ref array3[num * 4 + 1];
				reference10 = markSection2.tangent;
				ref Vector4 reference11 = ref array3[num * 4 + 2];
				reference11 = markSection.tangent;
				ref Vector4 reference12 = ref array3[num * 4 + 3];
				reference12 = markSection.tangent;
				ref Color reference13 = ref array4[num * 4];
				reference13 = new Color(0f, 0f, 0f, markSection2.intensity);
				ref Color reference14 = ref array4[num * 4 + 1];
				reference14 = new Color(0f, 0f, 0f, markSection2.intensity);
				ref Color reference15 = ref array4[num * 4 + 2];
				reference15 = new Color(0f, 0f, 0f, markSection.intensity);
				ref Color reference16 = ref array4[num * 4 + 3];
				reference16 = new Color(0f, 0f, 0f, markSection.intensity);
				ref Vector2 reference17 = ref array5[num * 4];
				reference17 = new Vector2(0f, 0f);
				ref Vector2 reference18 = ref array5[num * 4 + 1];
				reference18 = new Vector2(1f, 0f);
				ref Vector2 reference19 = ref array5[num * 4 + 2];
				reference19 = new Vector2(0f, 1f);
				ref Vector2 reference20 = ref array5[num * 4 + 3];
				reference20 = new Vector2(1f, 1f);
				array6[num * 6] = num * 4;
				array6[num * 6 + 2] = num * 4 + 1;
				array6[num * 6 + 1] = num * 4 + 2;
				array6[num * 6 + 3] = num * 4 + 2;
				array6[num * 6 + 5] = num * 4 + 1;
				array6[num * 6 + 4] = num * 4 + 3;
				num++;
			}
		}
		mesh.vertices = array;
		mesh.normals = array2;
		mesh.tangents = array3;
		mesh.triangles = array6;
		mesh.colors = array4;
		mesh.uv = array5;
	}
}
