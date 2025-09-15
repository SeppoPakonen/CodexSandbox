using System.Collections.Generic;
using UnityEngine;

public class ModelLibrary : bs
{
	public ModelItem RootItem;

	public List<ModelFile> models = new List<ModelFile>();

	private Dictionary<string, ModelFile> m_dict;

	public Dictionary<string, ModelFile> dict
	{
		get
		{
			if (m_dict == null)
			{
				m_dict = new Dictionary<string, ModelFile>();
				foreach (ModelFile model in models)
				{
					if (!m_dict.ContainsKey(model.name))
					{
						m_dict.Add(model.name, model);
					}
					else
					{
						Debug.Log("Duplicate: " + model.name, model.gameObj);
					}
				}
			}
			return m_dict;
		}
	}
}
