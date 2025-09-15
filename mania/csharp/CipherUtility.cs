using System;
using System.IO;
using System.Security.Cryptography;
using System.Text;

public class CipherUtility
{
	private static string Decode(string s)
	{
		char[] array = s.ToCharArray();
		string text = "fptasdjklfjekljfkajsdfdjfkldsjfklsdfjdskljfklesa";
		for (int i = 0; i < array.Length; i++)
		{
			array[i] ^= text[i];
		}
		return Encoding.UTF8.GetString(Convert.FromBase64CharArray(array, 0, array.Length));
	}

	public static string Encrypt<T>(byte[] value, string password, string salt) where T : SymmetricAlgorithm, new()
	{
		//Discarded unreachable code: IL_008b
		DeriveBytes deriveBytes = new Rfc2898DeriveBytes(password, Encoding.Unicode.GetBytes(salt));
		SymmetricAlgorithm symmetricAlgorithm = new T();
		byte[] bytes = deriveBytes.GetBytes(symmetricAlgorithm.KeySize >> 3);
		byte[] bytes2 = deriveBytes.GetBytes(symmetricAlgorithm.BlockSize >> 3);
		ICryptoTransform transform = symmetricAlgorithm.CreateEncryptor(bytes, bytes2);
		using (MemoryStream memoryStream = new MemoryStream())
		{
			using (CryptoStream cryptoStream = new CryptoStream(memoryStream, transform, CryptoStreamMode.Write))
			{
				cryptoStream.Write(value, 0, value.Length);
			}
			return Convert.ToBase64String(memoryStream.ToArray());
		}
	}

	public static Stream Decrypt<T>(string text, string password, string salt) where T : SymmetricAlgorithm, new()
	{
		//Discarded unreachable code: IL_0067, IL_007b
		DeriveBytes deriveBytes = new Rfc2898DeriveBytes(password, Encoding.Unicode.GetBytes(salt));
		SymmetricAlgorithm symmetricAlgorithm = new T();
		byte[] bytes = deriveBytes.GetBytes(symmetricAlgorithm.KeySize >> 3);
		byte[] bytes2 = deriveBytes.GetBytes(symmetricAlgorithm.BlockSize >> 3);
		ICryptoTransform transform = symmetricAlgorithm.CreateDecryptor(bytes, bytes2);
		using (MemoryStream stream = new MemoryStream(Convert.FromBase64String(text)))
		{
			using (CryptoStream result = new CryptoStream(stream, transform, CryptoStreamMode.Read))
			{
				return result;
			}
		}
	}
}
