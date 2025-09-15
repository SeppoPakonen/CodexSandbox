using System;
using UnityEngine;

[Serializable]
public class LetterAction
{
	private bool m_editor_folded;

	public bool m_offset_from_last;

	[HideInInspector]
	public bool m_use_gradient;

	public ACTION_TYPE m_action_type;

	public bool m_use_gradient_start;

	public ActionColorProgression m_start_colour = new ActionColorProgression(Color.white);

	public ActionVertexColorProgression m_start_vertex_colour = new ActionVertexColorProgression(new VertexColour(Color.white));

	public bool m_use_gradient_end;

	public ActionColorProgression m_end_colour = new ActionColorProgression(Color.white);

	public ActionVertexColorProgression m_end_vertex_colour = new ActionVertexColorProgression(new VertexColour(Color.white));

	public AxisEasingOverrideData m_position_axis_ease_data = new AxisEasingOverrideData();

	public ActionPositionVector3Progression m_start_pos = new ActionPositionVector3Progression(Vector3.zero);

	public ActionPositionVector3Progression m_end_pos = new ActionPositionVector3Progression(Vector3.zero);

	public AxisEasingOverrideData m_rotation_axis_ease_data = new AxisEasingOverrideData();

	public ActionVector3Progression m_start_euler_rotation = new ActionVector3Progression(Vector3.zero);

	public ActionVector3Progression m_end_euler_rotation = new ActionVector3Progression(Vector3.zero);

	public AxisEasingOverrideData m_scale_axis_ease_data = new AxisEasingOverrideData();

	public ActionVector3Progression m_start_scale = new ActionVector3Progression(Vector3.one);

	public ActionVector3Progression m_end_scale = new ActionVector3Progression(Vector3.one);

	public bool m_force_same_start_time;

	public ActionFloatProgression m_delay_progression = new ActionFloatProgression(0f);

	public ActionFloatProgression m_duration_progression = new ActionFloatProgression(1f);

	public EasingEquation m_ease_type;

	public TextAnchor m_letter_anchor = TextAnchor.MiddleCenter;

	public AudioClip m_audio_on_start;

	public bool m_audio_on_start_display;

	public ActionFloatProgression m_audio_on_start_delay = new ActionFloatProgression(0f);

	public ActionFloatProgression m_audio_on_start_offset = new ActionFloatProgression(0f);

	public ActionFloatProgression m_audio_on_start_volume = new ActionFloatProgression(1f);

	public ActionFloatProgression m_audio_on_start_pitch = new ActionFloatProgression(1f);

	public AudioClip m_audio_on_finish;

	public bool m_audio_on_finish_display;

	public ActionFloatProgression m_audio_on_finish_delay = new ActionFloatProgression(0f);

	public ActionFloatProgression m_audio_on_finish_offset = new ActionFloatProgression(0f);

	public ActionFloatProgression m_audio_on_finish_volume = new ActionFloatProgression(1f);

	public ActionFloatProgression m_audio_on_finish_pitch = new ActionFloatProgression(1f);

	public ParticleEmitter m_emitter_on_start;

	public bool m_emitter_on_start_display;

	public bool m_emitter_on_start_per_letter = true;

	public ActionFloatProgression m_emitter_on_start_delay = new ActionFloatProgression(0f);

	public ActionFloatProgression m_emitter_on_start_duration = new ActionFloatProgression(0f);

	public bool m_emitter_on_start_follow_mesh;

	public ActionVector3Progression m_emitter_on_start_offset = new ActionVector3Progression(Vector3.zero);

	public ParticleEmitter m_emitter_on_finish;

	public bool m_emitter_on_finish_display;

	public bool m_emitter_on_finish_per_letter = true;

	public ActionFloatProgression m_emitter_on_finish_delay = new ActionFloatProgression(0f);

	public ActionFloatProgression m_emitter_on_finish_duration = new ActionFloatProgression(0f);

	public bool m_emitter_on_finish_follow_mesh;

	public ActionVector3Progression m_emitter_on_finish_offset = new ActionVector3Progression(Vector3.zero);

	public bool m_starting_in_sync = true;

	public bool m_ending_in_sync = true;

	private bool m_static_colour;

	private bool m_static_position;

	private bool m_static_rotation;

	private bool m_static_scale;

	public bool FoldedInEditor
	{
		get
		{
			return m_editor_folded;
		}
		set
		{
			m_editor_folded = value;
		}
	}

	public bool StaticColour => m_static_colour;

	public bool StaticPosition => m_static_position;

	public bool StaticRotation => m_static_rotation;

	public bool StaticScale => m_static_scale;

	public void SoftReset(LetterAction prev_action, AnimationProgressionVariables progression_variables, AnimatePerOptions animate_per)
	{
		if (m_use_gradient || m_use_gradient_start)
		{
			if (!m_offset_from_last && m_start_vertex_colour.UniqueRandom)
			{
				m_start_vertex_colour.CalculateUniqueRandom(progression_variables, animate_per, prev_action?.m_end_vertex_colour.m_values);
			}
		}
		else if (!m_offset_from_last && m_start_colour.UniqueRandom)
		{
			m_start_colour.CalculateUniqueRandom(progression_variables, animate_per, prev_action?.m_end_colour.m_values);
		}
		if (!m_offset_from_last)
		{
			if (m_start_pos.UniqueRandom)
			{
				m_start_pos.CalculateUniqueRandom(progression_variables, animate_per, prev_action?.m_end_pos.m_values);
			}
			if (m_start_euler_rotation.UniqueRandom)
			{
				m_start_euler_rotation.CalculateUniqueRandom(progression_variables, animate_per, prev_action?.m_end_euler_rotation.m_values);
			}
			if (m_start_scale.UniqueRandom)
			{
				m_start_scale.CalculateUniqueRandom(progression_variables, animate_per, prev_action?.m_end_scale.m_values);
			}
		}
		if (m_delay_progression.UniqueRandom)
		{
			m_delay_progression.CalculateUniqueRandom(progression_variables, animate_per);
		}
		if (m_duration_progression.UniqueRandom)
		{
			m_duration_progression.CalculateUniqueRandom(progression_variables, animate_per);
		}
		if (m_audio_on_start != null)
		{
			if (m_audio_on_start_volume.UniqueRandom)
			{
				m_audio_on_start_volume.CalculateUniqueRandom(progression_variables, animate_per);
			}
			if (m_audio_on_start_delay.UniqueRandom)
			{
				m_audio_on_start_delay.CalculateUniqueRandom(progression_variables, animate_per);
			}
			if (m_audio_on_start_offset.UniqueRandom)
			{
				m_audio_on_start_offset.CalculateUniqueRandom(progression_variables, animate_per);
			}
			if (m_audio_on_start_pitch.UniqueRandom)
			{
				m_audio_on_start_pitch.CalculateUniqueRandom(progression_variables, animate_per);
			}
		}
		if (m_audio_on_finish != null)
		{
			if (m_audio_on_finish_volume.UniqueRandom)
			{
				m_audio_on_finish_volume.CalculateUniqueRandom(progression_variables, animate_per);
			}
			if (m_audio_on_finish_delay.UniqueRandom)
			{
				m_audio_on_finish_delay.CalculateUniqueRandom(progression_variables, animate_per);
			}
			if (m_audio_on_finish_offset.UniqueRandom)
			{
				m_audio_on_finish_offset.CalculateUniqueRandom(progression_variables, animate_per);
			}
			if (m_audio_on_finish_pitch.UniqueRandom)
			{
				m_audio_on_finish_pitch.CalculateUniqueRandom(progression_variables, animate_per);
			}
		}
		if (m_emitter_on_start != null)
		{
			if (m_emitter_on_start_offset.UniqueRandom)
			{
				m_emitter_on_start_offset.CalculateUniqueRandom(progression_variables, animate_per, null);
			}
			if (m_emitter_on_start_delay.UniqueRandom)
			{
				m_emitter_on_start_delay.CalculateUniqueRandom(progression_variables, animate_per);
			}
			if (m_emitter_on_start_duration.UniqueRandom)
			{
				m_emitter_on_start_duration.CalculateUniqueRandom(progression_variables, animate_per);
			}
		}
		if (m_emitter_on_finish != null)
		{
			if (m_emitter_on_finish_offset.UniqueRandom)
			{
				m_emitter_on_finish_offset.CalculateUniqueRandom(progression_variables, animate_per, null);
			}
			if (m_emitter_on_finish_delay.UniqueRandom)
			{
				m_emitter_on_finish_delay.CalculateUniqueRandom(progression_variables, animate_per);
			}
			if (m_emitter_on_finish_duration.UniqueRandom)
			{
				m_emitter_on_finish_duration.CalculateUniqueRandom(progression_variables, animate_per);
			}
		}
	}

	public LetterAction ContinueActionFromThis()
	{
		LetterAction letterAction = new LetterAction();
		letterAction.m_offset_from_last = true;
		letterAction.m_editor_folded = true;
		letterAction.m_use_gradient = m_use_gradient;
		letterAction.m_use_gradient_start = m_use_gradient_start;
		letterAction.m_use_gradient_end = m_use_gradient_end;
		letterAction.m_position_axis_ease_data = m_position_axis_ease_data.Clone();
		letterAction.m_rotation_axis_ease_data = m_rotation_axis_ease_data.Clone();
		letterAction.m_scale_axis_ease_data = m_scale_axis_ease_data.Clone();
		letterAction.m_start_colour = m_end_colour.Clone();
		letterAction.m_end_colour = m_end_colour.Clone();
		letterAction.m_start_vertex_colour = m_end_vertex_colour.Clone();
		letterAction.m_end_vertex_colour = m_end_vertex_colour.Clone();
		letterAction.m_start_pos = m_end_pos.CloneThis();
		letterAction.m_end_pos = m_end_pos.CloneThis();
		letterAction.m_start_euler_rotation = m_end_euler_rotation.Clone();
		letterAction.m_end_euler_rotation = m_end_euler_rotation.Clone();
		letterAction.m_start_scale = m_end_scale.Clone();
		letterAction.m_end_scale = m_end_scale.Clone();
		letterAction.m_delay_progression = new ActionFloatProgression(0f);
		letterAction.m_duration_progression = new ActionFloatProgression(1f);
		letterAction.m_letter_anchor = m_letter_anchor;
		letterAction.m_ease_type = m_ease_type;
		return letterAction;
	}

	private int GetProgressionTotal(int num_letters, int num_words, int num_lines, AnimatePerOptions animate_per_default, AnimatePerOptions animate_per_override, bool overriden)
	{
		switch ((!overriden) ? animate_per_default : animate_per_override)
		{
		case AnimatePerOptions.LETTER:
			return num_letters;
		case AnimatePerOptions.WORD:
			return num_words;
		case AnimatePerOptions.LINE:
			return num_lines;
		default:
			return num_letters;
		}
	}

	public void PrepareData(int num_letters, int num_words, int num_lines, LetterAction prev_action, AnimatePerOptions animate_per, bool prev_action_end_state = true)
	{
		m_static_position = false;
		m_static_rotation = false;
		m_static_scale = false;
		m_static_colour = false;
		m_duration_progression.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_duration_progression.m_animate_per, m_duration_progression.m_override_animate_per_option));
		if (m_audio_on_start != null)
		{
			m_audio_on_start_volume.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_audio_on_start_volume.m_animate_per, m_audio_on_start_volume.m_override_animate_per_option));
			m_audio_on_start_offset.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_audio_on_start_offset.m_animate_per, m_audio_on_start_offset.m_override_animate_per_option));
			m_audio_on_start_delay.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_audio_on_start_delay.m_animate_per, m_audio_on_start_delay.m_override_animate_per_option));
			m_audio_on_start_pitch.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_audio_on_start_pitch.m_animate_per, m_audio_on_start_pitch.m_override_animate_per_option));
		}
		if (m_audio_on_finish != null)
		{
			m_audio_on_finish_volume.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_audio_on_finish_volume.m_animate_per, m_audio_on_finish_volume.m_override_animate_per_option));
			m_audio_on_finish_offset.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_audio_on_finish_offset.m_animate_per, m_audio_on_finish_offset.m_override_animate_per_option));
			m_audio_on_finish_delay.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_audio_on_finish_delay.m_animate_per, m_audio_on_finish_delay.m_override_animate_per_option));
			m_audio_on_finish_pitch.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_audio_on_finish_pitch.m_animate_per, m_audio_on_finish_pitch.m_override_animate_per_option));
		}
		if (m_emitter_on_start != null)
		{
			m_emitter_on_start_offset.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_emitter_on_start_offset.m_animate_per, m_emitter_on_start_offset.m_override_animate_per_option), null);
			m_emitter_on_start_delay.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_emitter_on_start_delay.m_animate_per, m_emitter_on_start_delay.m_override_animate_per_option));
			m_emitter_on_start_duration.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_emitter_on_start_duration.m_animate_per, m_emitter_on_start_duration.m_override_animate_per_option));
		}
		if (m_emitter_on_finish != null)
		{
			m_emitter_on_finish_offset.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_emitter_on_finish_offset.m_animate_per, m_emitter_on_finish_offset.m_override_animate_per_option), null);
			m_emitter_on_finish_delay.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_emitter_on_finish_delay.m_animate_per, m_emitter_on_finish_delay.m_override_animate_per_option));
			m_emitter_on_finish_duration.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_emitter_on_finish_duration.m_animate_per, m_emitter_on_finish_duration.m_override_animate_per_option));
		}
		if (m_action_type == ACTION_TYPE.BREAK)
		{
			return;
		}
		m_delay_progression.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_delay_progression.m_animate_per, m_delay_progression.m_override_animate_per_option));
		if (m_offset_from_last && prev_action != null)
		{
			m_use_gradient_start = (prev_action.m_use_gradient_end || prev_action.m_use_gradient);
			if (prev_action_end_state)
			{
				if (m_use_gradient_start)
				{
					m_start_vertex_colour.m_values = prev_action.m_end_vertex_colour.m_values;
				}
				else
				{
					m_start_colour.m_values = prev_action.m_end_colour.m_values;
				}
			}
			else if (m_use_gradient_start)
			{
				m_start_vertex_colour.m_values = prev_action.m_start_vertex_colour.m_values;
			}
			else
			{
				m_start_colour.m_values = prev_action.m_start_colour.m_values;
			}
		}
		else if (m_use_gradient_start || m_use_gradient || (prev_action != null && (prev_action.m_use_gradient_end || prev_action.m_use_gradient)))
		{
			if (!m_use_gradient_start && !m_use_gradient)
			{
				m_use_gradient_start = true;
				m_use_gradient = false;
				m_start_vertex_colour.ConvertFromFlatColourProg(m_start_colour);
			}
			m_start_vertex_colour.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_start_vertex_colour.m_animate_per, m_start_vertex_colour.m_override_animate_per_option), (prev_action == null || (!prev_action.m_use_gradient_end && !prev_action.m_use_gradient)) ? null : prev_action.m_end_vertex_colour.m_values, (prev_action == null || prev_action.m_use_gradient_end || prev_action.m_use_gradient) ? null : prev_action.m_end_colour.m_values);
		}
		else
		{
			m_start_colour.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_start_colour.m_animate_per, m_start_colour.m_override_animate_per_option), prev_action?.m_end_colour.m_values);
		}
		if (m_use_gradient_end || m_use_gradient || m_use_gradient_start || m_use_gradient)
		{
			if (!m_use_gradient_end && !m_use_gradient)
			{
				m_use_gradient_end = true;
				m_use_gradient = false;
				m_end_vertex_colour.ConvertFromFlatColourProg(m_end_colour);
			}
			m_end_vertex_colour.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_end_vertex_colour.m_animate_per, m_end_vertex_colour.m_override_animate_per_option), (!m_use_gradient_start && !m_use_gradient) ? null : m_start_vertex_colour.m_values, (m_use_gradient_start || m_use_gradient) ? null : m_start_colour.m_values);
		}
		else
		{
			m_end_colour.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_end_colour.m_animate_per, m_end_colour.m_override_animate_per_option), m_start_colour.m_values);
		}
		if (m_use_gradient_start && m_use_gradient_end && m_start_vertex_colour.m_values.Length == 1 && m_end_vertex_colour.m_values.Length == 1 && m_start_vertex_colour.GetValue(0).Equals(m_end_vertex_colour.GetValue(0)))
		{
			m_static_colour = true;
		}
		else if (!m_use_gradient_start && !m_use_gradient_end && m_start_colour.m_values.Length == 1 && m_end_colour.m_values.Length == 1 && m_start_colour.GetValue(0).Equals(m_end_colour.GetValue(0)))
		{
			m_static_colour = true;
		}
		if (m_offset_from_last && prev_action != null)
		{
			m_start_pos.m_values = ((!prev_action_end_state) ? prev_action.m_start_pos.m_values : prev_action.m_end_pos.m_values);
			m_start_euler_rotation.m_values = ((!prev_action_end_state) ? prev_action.m_start_euler_rotation.m_values : prev_action.m_end_euler_rotation.m_values);
			m_start_scale.m_values = ((!prev_action_end_state) ? prev_action.m_start_scale.m_values : prev_action.m_end_scale.m_values);
		}
		else
		{
			m_start_pos.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_start_pos.m_animate_per, m_start_pos.m_override_animate_per_option), (prev_action != null) ? prev_action.m_end_pos.m_values : new Vector3[1]
			{
				Vector3.zero
			});
			m_start_euler_rotation.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_start_euler_rotation.m_animate_per, m_start_euler_rotation.m_override_animate_per_option), (prev_action != null) ? prev_action.m_end_euler_rotation.m_values : new Vector3[1]
			{
				Vector3.zero
			});
			m_start_scale.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_start_scale.m_animate_per, m_start_scale.m_override_animate_per_option), (prev_action != null) ? prev_action.m_end_scale.m_values : new Vector3[1]
			{
				Vector3.zero
			});
		}
		m_end_pos.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_end_pos.m_animate_per, m_end_pos.m_override_animate_per_option), m_start_pos.m_values);
		m_end_euler_rotation.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_end_euler_rotation.m_animate_per, m_end_euler_rotation.m_override_animate_per_option), m_start_euler_rotation.m_values);
		m_end_scale.CalculateProgressions(GetProgressionTotal(num_letters, num_words, num_lines, animate_per, m_end_scale.m_animate_per, m_end_scale.m_override_animate_per_option), m_start_scale.m_values);
		if (m_start_pos.m_values.Length == 1 && m_end_pos.m_values.Length == 1 && m_start_pos.m_values[0].Equals(m_end_pos.m_values[0]) && m_start_pos.m_force_position_override == m_end_pos.m_force_position_override)
		{
			m_static_position = true;
		}
		if (m_start_euler_rotation.m_values.Length == 1 && m_end_euler_rotation.m_values.Length == 1 && m_start_euler_rotation.m_values[0].Equals(m_end_euler_rotation.m_values[0]))
		{
			m_static_rotation = true;
		}
		if (m_start_scale.m_values.Length == 1 && m_end_scale.m_values.Length == 1 && m_start_scale.m_values[0].Equals(m_end_scale.m_values[0]))
		{
			m_static_scale = true;
		}
	}
}
