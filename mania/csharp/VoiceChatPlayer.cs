using NSpeex;
using System;
using UnityEngine;

[RequireComponent(typeof(AudioSource))]
public class VoiceChatPlayer : VoiceChatBase
{
	private float lastTime;

	private double played;

	private double received;

	private int index;

	private float[] data;

	private float playDelay;

	private bool shouldPlay;

	private float lastRecvTime;

	private SpeexDecoder speexDec = new SpeexDecoder(BandMode.Narrow);

	[SerializeField]
	private int playbackDelay = 2;

	public AudioSource audio;

	public float LastRecvTime => lastRecvTime;

	private void Start()
	{
		if (bs._Loader.offlineMode)
		{
			base.enabled = false;
			return;
		}
		int num = VoiceChatSettings.Instance.Frequency * 10;
		audio = base.gameObject.AddComponent<AudioSource>();
		audio.ignoreListenerVolume = true;
		audio.volume = 3f;
		AudioSource audioSource = audio;
		bool flag = true;
		audio.bypassListenerEffects = flag;
		flag = flag;
		audio.bypassReverbZones = flag;
		audioSource.bypassEffects = flag;
		audio.loop = true;
		audio.clip = AudioClip.Create("VoiceChat", num, 1, VoiceChatSettings.Instance.Frequency, _3D: false, stream: false);
		audio.priority = 0;
		data = new float[num];
		if (VoiceChatSettings.Instance.LocalDebug)
		{
			VoiceChatRecorder.Instance.NewSample += OnNewSample;
		}
	}

	private void Update()
	{
		if (audio.isPlaying)
		{
			if (lastTime > audio.time)
			{
				played += audio.clip.length;
			}
			lastTime = audio.time;
			if (played + (double)audio.time >= received)
			{
				Stop();
				shouldPlay = false;
			}
		}
		else if (shouldPlay)
		{
			playDelay -= bs._Loader.deltaTime;
			if (playDelay <= 0f)
			{
				audio.Play();
			}
		}
	}

	private void Stop()
	{
		audio.Stop();
		audio.time = 0f;
		index = 0;
		played = 0.0;
		received = 0.0;
		lastTime = 0f;
	}

	public void OnNewSample(VoiceChatPacket packet)
	{
		lastRecvTime = Time.realtimeSinceStartup;
		float[] array = null;
		int num = VoiceChatUtils.Decompress(speexDec, packet, out array);
		received += VoiceChatSettings.Instance.SampleTime;
		Array.Copy(array, 0, data, index, num);
		index += num;
		if (index >= audio.clip.samples)
		{
			index = 0;
		}
		audio.clip.SetData(data, 0);
		if (!audio.isPlaying)
		{
			shouldPlay = true;
			if (playDelay <= 0f)
			{
				playDelay = (float)VoiceChatSettings.Instance.SampleTime * (float)playbackDelay;
			}
		}
		VoiceChatFloatPool.Instance.Return(array);
	}
}
