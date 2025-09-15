using System.Collections.Generic;
using UnityEngine;

public class ResEditor : bs
{
	public ResLoader resLoader;

	public bool editorGui;

	public TerrainData td;

	public Object[] Resources;

	public Object[] allScenes;

	public Object[] packageScenes;

	public Material[] skyboxes;

	public Transform plane;

	public List<SkinSet> skins = new List<SkinSet>();

	public bool enableLevelEdit;

	public bool includeMaps;

	public List<CustomRenderSettings> renderSettings = new List<CustomRenderSettings>();

	public bool saveTr;

	public bool autorun = true;

	public ModelLibrary ModelLibrary;
}
