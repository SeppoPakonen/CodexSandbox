using Exocortex.DSP;
using System;
using System.Linq;
using UnityEngine;

public class VoiceChatRecorder : VoiceChatBase
{
	private static VoiceChatRecorder instance;

	[SerializeField]
	private KeyCode toggleToTalkKey;

	[SerializeField]
	private KeyCode pushToTalkKey = KeyCode.Y;

	[SerializeField]
	private bool autoDetectSpeaking;

	[SerializeField]
	private int autoDetectIndex = 4;

	[SerializeField]
	private float forceTransmitTime = 2f;

	private int previousPosition;

	private int sampleIndex;

	private string device;

	private AudioClip clip;

	private bool transmitToggled;

	private bool recording;

	private float forceTransmit;

	private int recordFrequency;

	private int recordSampleSize;

	private int targetFrequency;

	private int targetSampleSize;

	private float[] fftBuffer;

	private float[] sampleBuffer;

	private VoiceChatCircularBuffer<float[]> previousSampleBuffer = new VoiceChatCircularBuffer<float[]>(5);

	private float time;

	public static VoiceChatRecorder Instance
	{
		get
		{
			if (instance == null)
			{
				instance = (UnityEngine.Object.FindObjectOfType(typeof(VoiceChatRecorder)) as VoiceChatRecorder);
			}
			return instance;
		}
	}

	public KeyCode PushToTalkKey
	{
		get
		{
			return pushToTalkKey;
		}
		set
		{
			pushToTalkKey = value;
		}
	}

	public KeyCode ToggleToTalkKey
	{
		get
		{
			return toggleToTalkKey;
		}
		set
		{
			toggleToTalkKey = value;
		}
	}

	public bool AutoDetectSpeech
	{
		get
		{
			return autoDetectSpeaking;
		}
		set
		{
			autoDetectSpeaking = value;
		}
	}

	public int NetworkId
	{
		get;
		set;
	}

	public string Device
	{
		get
		{
			return device;
		}
		set
		{
			if (value != null && !Microphone.devices.Contains(value))
			{
				Debug.LogError(value + " is not a valid microphone device");
			}
			else
			{
				device = value;
			}
		}
	}

	public bool HasDefaultDevice => device == null;

	public bool HasSpecificDevice => device != null;

	public bool IsTransmitting => transmitToggled || forceTransmit > 0f || Input.GetKey(pushToTalkKey);

	public bool IsRecording => recording;

	public string[] AvailableDevices => Microphone.devices;

	public event Action<VoiceChatPacket> NewSample;

	private void Start()
	{
		if (instance != null && instance != this)
		{
			UnityEngine.Object.Destroy(this);
			Debug.LogError("Only one instance of VoiceChatRecorder can exist");
		}
		else
		{
			Application.RequestUserAuthorization(UserAuthorization.Microphone);
			NetworkId = -1;
			instance = this;
		}
	}

	private void OnEnable()
	{
		if (instance != null && instance != this)
		{
			UnityEngine.Object.Destroy(this);
			Debug.LogError("Only one instance of VoiceChatRecorder can exist");
		}
		else
		{
			Application.RequestUserAuthorization(UserAuthorization.Microphone);
			instance = this;
		}
	}

	private void OnDisable()
	{
		instance = null;
	}

	private void OnDestroy()
	{
		instance = null;
	}

	private void Update()
	{
		if (!recording)
		{
			return;
		}
		forceTransmit -= bs._Loader.deltaTime;
		if (Input.GetKeyUp(toggleToTalkKey))
		{
			transmitToggled = !transmitToggled;
		}
		if (transmitToggled || Input.GetKey(pushToTalkKey))
		{
			time = Time.realtimeSinceStartup;
		}
		bool transmit = Time.realtimeSinceStartup - time < 1f;
		int position = Microphone.GetPosition(Device);
		if (position < previousPosition)
		{
			while (sampleIndex < recordFrequency)
			{
				ReadSample(transmit);
			}
			sampleIndex = 0;
		}
		previousPosition = position;
		while (sampleIndex + recordSampleSize <= position)
		{
			ReadSample(transmit);
		}
	}

	private void Resample(float[] src, float[] dst)
	{
		if (src.Length == dst.Length)
		{
			Array.Copy(src, 0, dst, 0, src.Length);
			return;
		}
		float num = 1f / (float)dst.Length;
		for (int i = 0; i < dst.Length; i++)
		{
			float num2 = num * (float)i * (float)src.Length;
			dst[i] = src[(int)num2];
		}
	}

	private void ReadSample(bool transmit)
	{
		clip.GetData(sampleBuffer, sampleIndex);
		float[] array = VoiceChatFloatPool.Instance.Get();
		Resample(sampleBuffer, array);
		sampleIndex += recordSampleSize;
		float num = float.MinValue;
		int num2 = -1;
		if (autoDetectSpeaking && !transmit)
		{
			for (int i = 0; i < fftBuffer.Length; i++)
			{
				fftBuffer[i] = 0f;
			}
			Array.Copy(array, 0, fftBuffer, 0, array.Length);
			Fourier.FFT(fftBuffer, fftBuffer.Length / 2, FourierDirection.Forward);
			for (int j = 0; j < fftBuffer.Length; j++)
			{
				if (fftBuffer[j] > num)
				{
					num = fftBuffer[j];
					num2 = j;
				}
			}
		}
		if (this.NewSample != null && (transmit || forceTransmit > 0f || num2 >= autoDetectIndex))
		{
			if (num2 >= autoDetectIndex)
			{
				if (forceTransmit <= 0f)
				{
					while (previousSampleBuffer.Count > 0)
					{
						TransmitBuffer(previousSampleBuffer.Remove());
					}
				}
				forceTransmit = forceTransmitTime;
			}
			TransmitBuffer(array);
		}
		else
		{
			if (previousSampleBuffer.Count == previousSampleBuffer.Capacity)
			{
				VoiceChatFloatPool.Instance.Return(previousSampleBuffer.Remove());
			}
			previousSampleBuffer.Add(array);
		}
	}

	private void TransmitBuffer(float[] buffer)
	{
		NormalizeSample(buffer);
		VoiceChatPacket obj = VoiceChatUtils.Compress(buffer);
		obj.NetworkId = NetworkId;
		this.NewSample(obj);
	}

	public bool StartRecording()
	{
		if (NetworkId == -1 && !VoiceChatSettings.Instance.LocalDebug)
		{
			Debug.LogError("NetworkId is -1");
			return false;
		}
		if (recording)
		{
			Debug.LogError("Already recording");
			return false;
		}
		targetFrequency = VoiceChatSettings.Instance.Frequency;
		targetSampleSize = VoiceChatSettings.Instance.SampleSize;
		Microphone.GetDeviceCaps(Device, out int minFreq, out int maxFreq);
		recordFrequency = ((minFreq != 0 || maxFreq != 0) ? maxFreq : 44100);
		recordSampleSize = recordFrequency / (targetFrequency / targetSampleSize);
		clip = Microphone.Start(Device, loop: true, 1, recordFrequency);
		sampleBuffer = new float[recordSampleSize];
		fftBuffer = new float[VoiceChatUtils.ClosestPowerOfTwo(targetSampleSize)];
		recording = true;
		return recording;
	}

	public void StopRecording()
	{
		clip = null;
		recording = false;
	}
}
