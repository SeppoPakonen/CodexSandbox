public class VoiceChatShortPool : VoiceChatPool<short[]>
{
	public static readonly VoiceChatShortPool Instance = new VoiceChatShortPool();

	private VoiceChatShortPool()
	{
	}

	protected override short[] Create()
	{
		return new short[VoiceChatSettings.Instance.SampleSize];
	}
}
