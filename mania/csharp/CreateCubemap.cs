using UnityEngine;

[ExecuteInEditMode]
public class CreateCubemap : MonoBehaviour
{
	private bool oneFacePerFrame;

	public Cubemap cubemap;

	private void Awake()
	{
	}

	public void OnEnable()
	{
		UpdateCubemap();
	}

	private void Update()
	{
		if (oneFacePerFrame)
		{
			int num = Time.frameCount % 6;
			int faceMask = 1 << num;
			UpdateCubemap(faceMask);
		}
		else
		{
			UpdateCubemap();
		}
	}

	public void UpdateCubemap(int faceMask = 63)
	{
		if (!cubemap)
		{
			cubemap = new Cubemap(512, TextureFormat.RGB24, mipmap: false);
		}
		base.get_camera().RenderToCubemap(cubemap);
	}

	private void OnDisable()
	{
	}
}
