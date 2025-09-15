using System;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
public class ModelItem
{
	public string Name;

	public ModelItem parent;

	public List<ModelItem> dirs = new List<ModelItem>();

	public List<ModelFile> files = new List<ModelFile>();

	internal Vector2 scroll;

	public Texture FolderTexture;
}
