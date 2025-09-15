using System.Collections.Generic;
using UnityEngine;

public class VoiceChatNetworkProxy : MonoBehaviour
{
	private static int networkIdCounter;

	private int assignedNetworkId = -1;

	private VoiceChatPlayer player;

	private Queue<VoiceChatPacket> packets = new Queue<VoiceChatPacket>(16);

	private void Start()
	{
		//IL_0054: Unknown result type (might be due to invalid IL or missing references)
		if (base.get_networkView().get_isMine())
		{
			VoiceChatRecorder.Instance.NewSample += OnNewSample;
		}
		if (Network.get_isServer())
		{
			assignedNetworkId = ++networkIdCounter;
			base.get_networkView().RPC("SetNetworkId", base.get_networkView().get_owner(), new object[1]
			{
				assignedNetworkId
			});
		}
		if (Network.get_isClient() && !base.get_networkView().get_isMine())
		{
			base.gameObject.AddComponent<AudioSource>();
			player = base.gameObject.AddComponent<VoiceChatPlayer>();
		}
	}

	private void OnNewSample(VoiceChatPacket packet)
	{
		packets.Enqueue(packet);
	}

	[RPC]
	private void SetNetworkId(int networkId)
	{
		VoiceChatRecorder.Instance.NetworkId = networkId;
	}

	private void OnSerializeNetworkView(BitStream stream, NetworkMessageInfo info)
	{
		int count = packets.Count;
		if (stream.get_isWriting())
		{
			stream.Serialize(ref count);
			while (packets.Count > 0)
			{
				VoiceChatPacket packet = packets.Dequeue();
				stream.WritePacket(packet);
				if (packet.Data.Length == VoiceChatSettings.Instance.SampleSize)
				{
					VoiceChatBytePool.Instance.Return(packet.Data);
				}
			}
			return;
		}
		if (Network.get_isServer())
		{
			stream.Serialize(ref count);
			for (int i = 0; i < count; i++)
			{
				packets.Enqueue(stream.ReadPacket());
				if (Network.get_connections().Length < 2)
				{
					packets.Dequeue();
				}
			}
			return;
		}
		stream.Serialize(ref count);
		for (int j = 0; j < count; j++)
		{
			VoiceChatPacket packet2 = stream.ReadPacket();
			if (player != null)
			{
				player.OnNewSample(packet2);
			}
		}
	}
}
