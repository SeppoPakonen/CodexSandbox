using UnityEngine;

public class SceneEffectsManager : MonoBehaviour
{
	public bool m_force_effects_to_origin = true;

	public SceneEffectData[] m_effects;

	private string[] m_effect_names;

	private int m_effect_index;

	private EffectManager m_current_active_effect;

	private void Start()
	{
		m_effect_names = new string[m_effects.Length];
		int num = 0;
		SceneEffectData[] effects = m_effects;
		foreach (SceneEffectData sceneEffectData in effects)
		{
			m_effect_names[num] = sceneEffectData.m_name;
			num++;
		}
		PlayEffect(0, 0.5f);
	}

	private void PlayEffect(int effect_idx, float delay = 0f)
	{
		if (m_current_active_effect != null)
		{
			m_current_active_effect.gameObject.SetActive(value: false);
		}
		m_current_active_effect = m_effects[effect_idx].m_effect_sync;
		m_current_active_effect.gameObject.SetActive(value: true);
		if (m_force_effects_to_origin)
		{
			m_current_active_effect.transform.localPosition = m_effects[effect_idx].m_position_offset;
		}
		m_current_active_effect.PlayAnimation(delay);
	}

	private void OnGUI()
	{
		m_effect_index = GUI.SelectionGrid(new Rect((float)Screen.width / 2f - (float)Screen.width / 4f, 5f * ((float)Screen.height / 6f), (float)Screen.width / 2f, 1.5f * ((float)Screen.height / 13f)), m_effect_index, m_effect_names, 3);
		if (GUI.changed)
		{
			PlayEffect(m_effect_index, 0f);
		}
		if (GUI.Button(new Rect((float)Screen.width / 28f, 10.5f * ((float)Screen.height / 12f), (float)Screen.width / 7f, (float)Screen.height / 13f), "Back"))
		{
			PlayerPrefs.SetInt("TextFx_Skip_Intro_Anim", 1);
			Application.LoadLevel("TitleScene");
		}
	}
}
