using System;
using System.IO;
using System.Text;
using UnityEngine;

public class BinaryWriter : MemoryStream
{
	public void Write(string a)
	{
		byte[] bytes = Encoding.UTF8.GetBytes(a);
		Write(BitConverter.GetBytes(bytes.Length));
		Write(bytes);
	}

	public void Write(int b)
	{
		Write(BitConverter.GetBytes(b));
	}

	public void Write(bool b)
	{
		Write((byte)(b ? 1u : 0u));
	}

	public void Write(byte b)
	{
		Write(new byte[1]
		{
			b
		});
	}

	public void Write(Vector3 vector3)
	{
		Write(vector3.x);
		Write(vector3.y);
		Write(vector3.z);
	}

	public void Write(Color vector3)
	{
		Write(vector3.r);
		Write(vector3.b);
		Write(vector3.g);
		Write(vector3.a);
	}

	public void Write(float value)
	{
		Write(BitConverter.GetBytes(value));
	}

	public void Write(byte[] bts)
	{
		Write(bts, 0, bts.Length);
	}
}
