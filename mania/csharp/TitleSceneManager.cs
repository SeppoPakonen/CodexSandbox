using UnityEngine;

public class TitleSceneManager : MonoBehaviour
{
	public EffectManager m_title_effect;

	private string[] m_demo_scenes = new string[4]
	{
		"Basics",
		"Large Texts",
		"Audio & Particles",
		"Other Bits"
	};

	private int m_scene_select_idx = -1;

	private Rect m_version_display_rect = new Rect(Screen.width - 30, Screen.height - 20, 35f, 20f);

	private bool m_display_buttons;

	private void Start()
	{
		if (PlayerPrefs.HasKey("TextFx_Skip_Intro_Anim"))
		{
			PlayerPrefs.DeleteKey("TextFx_Skip_Intro_Anim");
			m_title_effect.SetAnimationState(0, 1f);
			m_display_buttons = true;
		}
		else
		{
			m_title_effect.PlayAnimation(0.5f, TitleEffectFinished);
		}
	}

	private void TitleEffectFinished()
	{
		m_display_buttons = true;
	}

	private void Update()
	{
		if (m_display_buttons && Input.GetMouseButtonDown(0))
		{
			Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
			RaycastHit hitInfo = default(RaycastHit);
			if (base.get_collider().Raycast(ray, out hitInfo, 10000f))
			{
				m_title_effect.ResetAnimation();
				m_title_effect.PlayAnimation(0.5f);
			}
		}
	}

	private void OnGUI()
	{
		if (m_display_buttons)
		{
			m_scene_select_idx = GUI.SelectionGrid(new Rect((float)Screen.width / 2f - (float)Screen.width / 4f, 5f * ((float)Screen.height / 8f), Screen.width / 2, Screen.height / 6), m_scene_select_idx, m_demo_scenes, 2);
			if (GUI.changed)
			{
				Application.LoadLevel(m_demo_scenes[m_scene_select_idx]);
			}
		}
		GUI.Label(m_version_display_rect, EffectManager.m_version);
	}
}
