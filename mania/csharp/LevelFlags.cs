using System;

[Flags]
public enum LevelFlags
{
	none = 0x0,
	race = 0x1,
	advanced = 0x2,
	tested = 0x4,
	Ctf = 0x10,
	dm = 0x20
}
