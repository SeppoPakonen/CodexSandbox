using System;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
public class CustomRenderSettings
{
	public string name;

	public Color ambientLight;

	public float flareStrength;

	public bool fog;

	public Color fogColor;

	public float fogDensity;

	public float fogEndDistance;

	public FogMode fogMode;

	public float fogStartDistance;

	public float haloStrength;

	public Material skybox;

	public List<MyProperty> properties = new List<MyProperty>();

	public void Active()
	{
		RenderSettings.fog = fog;
		RenderSettings.ambientLight = ambientLight;
		RenderSettings.flareStrength = flareStrength;
		RenderSettings.fogColor = fogColor;
		RenderSettings.fogDensity = fogDensity;
		RenderSettings.fogEndDistance = fogEndDistance;
		RenderSettings.fogMode = fogMode;
		RenderSettings.fogStartDistance = fogStartDistance;
		RenderSettings.haloStrength = haloStrength;
		RenderSettings.skybox = skybox;
	}
}
