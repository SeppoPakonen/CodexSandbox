using ExitGames.Client.Photon;

public class RoomInfo
{
	private Hashtable customPropertiesField = new Hashtable();

	protected byte maxPlayersField;

	protected bool openField = true;

	protected bool visibleField = true;

	protected bool autoCleanUpField;

	protected string nameField;

	public bool removedFromList
	{
		get;
		internal set;
	}

	public Hashtable customProperties => customPropertiesField;

	public string name => nameField;

	public int playerCount
	{
		get;
		private set;
	}

	public bool isLocalClientInside
	{
		get;
		set;
	}

	public byte maxPlayers => maxPlayersField;

	public bool open => openField;

	public bool visible => visibleField;

	protected internal RoomInfo(string roomName, Hashtable properties)
	{
		CacheProperties(properties);
		nameField = roomName;
	}

	public override bool Equals(object p)
	{
		RoomInfo roomInfo = p as RoomInfo;
		return roomInfo != null && nameField.Equals(roomInfo.nameField);
	}

	public override int GetHashCode()
	{
		return nameField.GetHashCode();
	}

	public override string ToString()
	{
		return $"Room: '{nameField}' visible: {visibleField} open: {openField} max: {maxPlayersField} count: {playerCount}\ncustomProps: {customPropertiesField.ToStringFull()}";
	}

	protected internal void CacheProperties(Hashtable propertiesToCache)
	{
		if (propertiesToCache == null || propertiesToCache.Count == 0 || customPropertiesField.Equals(propertiesToCache))
		{
			return;
		}
		if (propertiesToCache.ContainsKey((byte)251))
		{
			removedFromList = (bool)propertiesToCache[(byte)251];
			if (removedFromList)
			{
				return;
			}
		}
		if (propertiesToCache.ContainsKey(byte.MaxValue))
		{
			maxPlayersField = (byte)propertiesToCache[byte.MaxValue];
		}
		if (propertiesToCache.ContainsKey((byte)253))
		{
			openField = (bool)propertiesToCache[(byte)253];
		}
		if (propertiesToCache.ContainsKey((byte)254))
		{
			visibleField = (bool)propertiesToCache[(byte)254];
		}
		if (propertiesToCache.ContainsKey((byte)252))
		{
			playerCount = (byte)propertiesToCache[(byte)252];
		}
		if (propertiesToCache.ContainsKey((byte)249))
		{
			autoCleanUpField = (bool)propertiesToCache[(byte)249];
		}
		customPropertiesField.MergeStringKeys(propertiesToCache);
	}
}
