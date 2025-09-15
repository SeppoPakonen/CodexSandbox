public class MapSets
{
	public LevelFlags levelFlags;

	public bool usedAdvancedTools
	{
		get
		{
			return GetFlag(LevelFlags.advanced);
		}
		set
		{
			SetFlag(LevelFlags.advanced, value);
		}
	}

	public bool tested
	{
		get
		{
			return GetFlag(LevelFlags.tested);
		}
		set
		{
			SetFlag(LevelFlags.tested, value);
		}
	}

	public bool enableCtf
	{
		get
		{
			return GetFlag(LevelFlags.Ctf);
		}
		set
		{
			SetFlag(LevelFlags.Ctf, value);
		}
	}

	public bool enableDm
	{
		get
		{
			return GetFlag(LevelFlags.dm);
		}
		set
		{
			SetFlag(LevelFlags.dm, value);
		}
	}

	public bool race
	{
		get
		{
			return GetFlag(LevelFlags.race) || levelFlags == LevelFlags.none;
		}
		private set
		{
			SetFlag(LevelFlags.race, value);
		}
	}

	private void SetFlag(LevelFlags flag, bool value)
	{
		if (value)
		{
			levelFlags |= flag;
		}
		else
		{
			levelFlags &= ~flag;
		}
	}

	private bool GetFlag(LevelFlags flag)
	{
		return (levelFlags & flag) != 0;
	}
}
