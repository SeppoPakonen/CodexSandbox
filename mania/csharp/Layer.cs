using UnityEngine;

public static class Layer
{
	public static int fx = LayerMask.NameToLayer("TransparentFX");

	public static int car = LayerMask.NameToLayer("Car");

	public static int cull = LayerMask.NameToLayer("Cull");

	public static int model = LayerMask.NameToLayer("model");

	public static int border = LayerMask.NameToLayer("border");

	public static int nGui = LayerMask.NameToLayer("NGUI");

	public static int level = LayerMask.NameToLayer("Level");

	public static int def = LayerMask.NameToLayer("Default");

	public static int node = LayerMask.NameToLayer("node");

	public static int stadium = LayerMask.NameToLayer("Stadium");

	public static int water = LayerMask.NameToLayer("Water");

	public static int terrain = LayerMask.NameToLayer("terrain");

	public static int block = LayerMask.NameToLayer("block");

	public static int hitBox = LayerMask.NameToLayer("hitBox");

	public static int CheckPoint = LayerMask.NameToLayer("CheckPoint");

	public static int nodeLayer = (1 << node) | (1 << block);

	public static int particles = LayerMask.NameToLayer("particles");

	public static int levelMask = (1 << level) | (1 << def) | (1 << stadium) | (1 << border) | (1 << cull) | (1 << water) | (1 << terrain) | (1 << block);

	public static int allmask = levelMask | (1 << car) | (1 << hitBox);
}
