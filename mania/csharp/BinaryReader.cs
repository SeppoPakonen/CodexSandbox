using System;
using System.IO;
using System.Text;
using UnityEngine;

public class BinaryReader : MemoryStream
{
	public BinaryReader(byte[] buffer)
		: base(buffer)
	{
	}

	public string ReadString()
	{
		int num = BitConverter.ToInt32(ReadBytes(4), 0);
		return Encoding.UTF8.GetString(ReadBytes(num), 0, num);
	}

	public Vector3 ReadVector()
	{
		Vector3 result = default(Vector3);
		result.x = ReadFloat();
		result.y = ReadFloat();
		result.z = ReadFloat();
		return result;
	}

	public Color readColor()
	{
		Color result = default(Color);
		result.r = ReadFloat();
		result.b = ReadFloat();
		result.g = ReadFloat();
		result.a = ReadFloat();
		return result;
	}

	public int ReadInt()
	{
		return BitConverter.ToInt32(ReadBytes(4), 0);
	}

	public float ReadFloat()
	{
		return BitConverter.ToSingle(ReadBytes(4), 0);
	}

	public bool ReadBool()
	{
		return ReadByte2() == 1;
	}

	public byte ReadByte2()
	{
		int num = ReadByte();
		if (num == -1)
		{
			throw new Exception("stread end");
		}
		return (byte)num;
	}

	public byte[] ReadBytes(int len)
	{
		byte[] array = new byte[len];
		int num = Read(array, 0, len);
		if (num != len)
		{
			throw new Exception("failed read string");
		}
		return array;
	}
}
