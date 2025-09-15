using UnityEngine;

public class VoiceChatSettings : MonoBehaviour
{
	private static VoiceChatSettings instance;

	[SerializeField]
	private int frequency = 16000;

	[SerializeField]
	private int sampleSize = 640;

	[SerializeField]
	private VoiceChatCompression compression = VoiceChatCompression.Speex;

	[SerializeField]
	private VoiceChatPreset preset = VoiceChatPreset.Speex_16K;

	[SerializeField]
	private bool localDebug;

	public static VoiceChatSettings Instance
	{
		get
		{
			if (instance == null)
			{
				instance = (Object.FindObjectOfType(typeof(VoiceChatSettings)) as VoiceChatSettings);
			}
			return instance;
		}
	}

	public int Frequency
	{
		get
		{
			return frequency;
		}
		private set
		{
			frequency = value;
		}
	}

	public bool LocalDebug
	{
		get
		{
			return localDebug;
		}
		set
		{
			localDebug = value;
		}
	}

	public VoiceChatPreset Preset
	{
		get
		{
			return preset;
		}
		set
		{
			preset = value;
			switch (preset)
			{
			case VoiceChatPreset.Speex_8K:
				Frequency = 8000;
				SampleSize = 320;
				Compression = VoiceChatCompression.Speex;
				break;
			case VoiceChatPreset.Speex_16K:
				Frequency = 16000;
				SampleSize = 640;
				Compression = VoiceChatCompression.Speex;
				break;
			case VoiceChatPreset.Alaw_4k:
				Frequency = 4096;
				SampleSize = 128;
				Compression = VoiceChatCompression.Alaw;
				break;
			case VoiceChatPreset.Alaw_8k:
				Frequency = 8192;
				SampleSize = 256;
				Compression = VoiceChatCompression.Alaw;
				break;
			case VoiceChatPreset.Alaw_16k:
				Frequency = 16384;
				SampleSize = 512;
				Compression = VoiceChatCompression.Alaw;
				break;
			case VoiceChatPreset.Alaw_Zlib_4k:
				Frequency = 4096;
				SampleSize = 128;
				Compression = VoiceChatCompression.AlawZlib;
				break;
			case VoiceChatPreset.Alaw_Zlib_8k:
				Frequency = 8192;
				SampleSize = 256;
				Compression = VoiceChatCompression.AlawZlib;
				break;
			case VoiceChatPreset.Alaw_Zlib_16k:
				Frequency = 16384;
				SampleSize = 512;
				Compression = VoiceChatCompression.AlawZlib;
				break;
			}
		}
	}

	public VoiceChatCompression Compression
	{
		get
		{
			return compression;
		}
		private set
		{
			compression = value;
		}
	}

	public int SampleSize
	{
		get
		{
			return sampleSize;
		}
		private set
		{
			sampleSize = value;
		}
	}

	public double SampleTime => (double)SampleSize / (double)Frequency;
}
