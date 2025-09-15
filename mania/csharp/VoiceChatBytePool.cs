public class VoiceChatBytePool : VoiceChatPool<byte[]>
{
	public static readonly VoiceChatBytePool Instance = new VoiceChatBytePool();

	private VoiceChatBytePool()
	{
	}

	protected override byte[] Create()
	{
		return new byte[VoiceChatSettings.Instance.SampleSize];
	}
}
