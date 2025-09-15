using System.Collections.Generic;

public static class Ext
{
	public static T2 TryGet<T, T2>(this Dictionary<T, T2> dict, T key, T2 def)
	{
		if (dict.TryGetValue(key, out T2 value))
		{
			return value;
		}
		return def;
	}
}
