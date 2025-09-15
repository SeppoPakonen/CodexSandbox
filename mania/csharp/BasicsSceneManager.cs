using UnityEngine;

public class BasicsSceneManager : MonoBehaviour
{
	public BasicEffectData[] m_effects;

	private string[] m_effect_names;

	private Vector3 m_local_position = new Vector3(0f, 0f, -2f);

	private bool m_sync_toggle = true;

	private int m_effect_index;

	private EffectManager m_current_active_effect;

	private void Start()
	{
		m_effect_names = new string[m_effects.Length];
		int num = 0;
		BasicEffectData[] effects = m_effects;
		foreach (BasicEffectData basicEffectData in effects)
		{
			m_effect_names[num] = basicEffectData.m_name;
			num++;
		}
		m_current_active_effect = m_effects[0].m_effect_sync;
		m_current_active_effect.ResetAnimation();
		m_current_active_effect.transform.localPosition = m_local_position;
		m_current_active_effect.PlayAnimation(0.5f);
	}

	private void OnGUI()
	{
		m_effect_index = GUI.SelectionGrid(new Rect((float)Screen.width / 2f - (float)Screen.width / 4f, 2f * ((float)Screen.height / 3f), (float)Screen.width / 2f, 7f * ((float)Screen.height / 24f)), m_effect_index, m_effect_names, 3);
		m_sync_toggle = GUI.Toggle(new Rect(5f * ((float)Screen.width / 6f), 11f * ((float)Screen.height / 12f), (float)Screen.width / 7f, (float)Screen.height / 13f), m_sync_toggle, "Sync?");
		if (GUI.changed)
		{
			m_current_active_effect.gameObject.SetActive(value: false);
			m_current_active_effect = ((!m_sync_toggle) ? m_effects[m_effect_index].m_effect_random : m_effects[m_effect_index].m_effect_sync);
			m_current_active_effect.gameObject.SetActive(value: true);
			m_current_active_effect.transform.localPosition = m_local_position;
			m_current_active_effect.PlayAnimation(0f);
		}
		if (GUI.Button(new Rect((float)Screen.width / 28f, 10.5f * ((float)Screen.height / 12f), (float)Screen.width / 7f, (float)Screen.height / 13f), "Back"))
		{
			PlayerPrefs.SetInt("TextFx_Skip_Intro_Anim", 1);
			Application.LoadLevel("TitleScene");
		}
	}
}
