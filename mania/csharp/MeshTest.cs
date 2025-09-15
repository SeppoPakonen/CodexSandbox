using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using UnityEngine;

public class MeshTest : bs
{
	public class Tr
	{
		public int[] ind;

		public int[] pointers;

		public int group;

		public bool went;
	}

	public MeshFilter mf;

	private Player pl;

	private Dictionary<int, List<Tr>> lab;

	internal Vector3[] vertices;

	internal Vector3[] oldVertices;

	internal Vector3[] normals;

	internal Vector2[] uvs;

	internal Vector4[] tangents;

	private List<Tr> flat = new List<Tr>();

	public List<Element> elements = new List<Element>();

	private bool changed;

	public void Start()
	{
		if (mf == null)
		{
			mf = GetComponent<MeshFilter>();
		}
		pl = base.transform.root.GetComponent<Player>();
		elements.Clear();
		lab = new Dictionary<int, List<Tr>>();
		vertices = mf.mesh.vertices;
		oldVertices = (Vector3[])vertices.Clone();
		normals = mf.mesh.normals;
		tangents = mf.mesh.tangents;
		uvs = mf.mesh.uv;
		for (int i = 0; i < mf.mesh.subMeshCount; i++)
		{
			int[] indices = mf.mesh.GetIndices(i);
			for (int j = 0; j < indices.Length; j += 3)
			{
				Tr tr = new Tr();
				tr.ind = new int[3]
				{
					indices[j],
					indices[j + 1],
					indices[j + 2]
				};
				tr.pointers = new int[3]
				{
					j,
					j + 1,
					j + 2
				};
				tr.group = i;
				Tr item = tr;
				flat.Add(item);
				for (int k = 0; k < 3; k++)
				{
					if (!lab.TryGetValue(indices[j + k], out List<Tr> value))
					{
						value = (lab[indices[j + k]] = new List<Tr>());
					}
					value.Add(item);
				}
			}
		}
		foreach (Tr item2 in flat)
		{
			if (!item2.went)
			{
				Element element = new Element();
				element.MeshTest = this;
				elements.Add(element);
				Color white = Color.white;
				Go(item2, white, element);
			}
		}
	}

	public void Reset()
	{
		if (!changed)
		{
			return;
		}
		changed = false;
		if (oldVertices == null)
		{
			return;
		}
		Vector3[] array = (Vector3[])oldVertices.Clone();
		mf.mesh.vertices = array;
		vertices = array;
		foreach (Element element in elements)
		{
			element.detached = false;
		}
	}

	private void Go(Tr tr, Color32 color, Element element)
	{
		if (tr.went)
		{
			return;
		}
		tr.went = true;
		element.materialGroup = tr.group;
		element.pointers.AddRange(tr.pointers);
		element.list.AddRange(tr.ind);
		int[] ind = tr.ind;
		foreach (int num in ind)
		{
			element.b.Encapsulate(vertices[num]);
		}
		int[] ind2 = tr.ind;
		foreach (int key in ind2)
		{
			if (!lab.TryGetValue(key, out List<Tr> value))
			{
				continue;
			}
			foreach (Tr item in value)
			{
				Go(item, color, element);
			}
		}
	}

	public void Damage(Vector3 nwPoint, Vector3 direction)
	{
		changed = true;
		nwPoint = mf.transform.InverseTransformPoint(nwPoint);
		for (int i = 0; i < 3; i++)
		{
			Vector3 b = nwPoint + Random.insideUnitSphere;
			Vector3 onUnitSphere = Random.onUnitSphere;
			float num = float.MaxValue;
			Vector3 b2 = nwPoint;
			for (int j = 0; j < vertices.Length; j++)
			{
				if ((vertices[j] - b).magnitude < num)
				{
					nwPoint = vertices[j];
					num = (vertices[j] - b).magnitude;
				}
				float num2 = 0.7f - (vertices[j] - b2).magnitude;
				if (num2 > 0f && oldVertices[j] == vertices[j])
				{
					vertices[j] += num2 * onUnitSphere * 0.3f;
				}
			}
		}
		mf.mesh.vertices = vertices;
	}

	public void Hit(Vector3 point, [Optional] Vector3 vel, float cnt = 10f, int max = 20)
	{
		changed = true;
		Vector3 po = mf.transform.InverseTransformPoint(point);
		IEnumerable<Element> enumerable = elements.OrderBy<Element, float>((Element a) => (a.b.center - po).magnitude).Take(max);
		foreach (Element item in enumerable)
		{
			if (item.detached)
			{
				continue;
			}
			if (item.list.Count > 10)
			{
				item.GenerateVertex();
				Mesh mesh = new Mesh();
				mesh.vertices = item.vertex.ToArray();
				mesh.triangles = item.nwlist.ToArray();
				mesh.uv = item.uvs.ToArray();
				mesh.tangents = item.tangents.ToArray();
				mesh.normals = item.normals.ToArray();
				mesh.RecalculateBounds();
				float magnitude = mesh.bounds.size.magnitude;
				cnt -= magnitude;
				if (cnt < 0f)
				{
					break;
				}
				GameObject gameObject = new GameObject();
				gameObject.transform.position = mf.transform.position;
				gameObject.transform.rotation = mf.transform.rotation;
				MeshRenderer meshRenderer = gameObject.AddComponent<MeshRenderer>();
				meshRenderer.sharedMaterial = ((Component)mf).get_renderer().sharedMaterials[item.materialGroup];
				gameObject.AddComponent<MeshFilter>().mesh = mesh;
				GameObject gameObject2 = new GameObject(string.Empty);
				gameObject2.transform.position = meshRenderer.bounds.center;
				gameObject.transform.parent = gameObject2.transform;
				gameObject2.AddComponent<Oskolok2>();
				gameObject.AddComponent<BoxCollider>();
				gameObject.get_collider().material = new PhysicMaterial();
				gameObject.gameObject.layer = Layer.particles;
				gameObject2.AddComponent<Rigidbody>().useGravity = false;
				gameObject2.AddComponent<ConstantForce>().force = Vector3.down * 15f;
				gameObject2.get_rigidbody().velocity = (bs.ZeroY(point - base.pos, 0f).normalized + Vector3.up + Random.insideUnitSphere) * 5f + vel;
				gameObject2.get_rigidbody().angularVelocity = Random.insideUnitSphere * 3f;
				gameObject2.hideFlags = HideFlags.HideInHierarchy;
				if ((bool)pl)
				{
					pl.fx.Add(gameObject2);
				}
			}
			item.detached = true;
			foreach (int item2 in item.list)
			{
				ref Vector3 reference = ref vertices[item2];
				reference = Vector3.zero;
			}
		}
		mf.mesh.vertices = vertices;
	}
}
