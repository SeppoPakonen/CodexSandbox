using System;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
public class LetterSetup
{
	public string m_character;

	public bool m_flipped;

	public float m_offset_width;

	public float m_width;

	public float m_height;

	public Vector3[] m_base_vertices;

	public Vector3 m_base_offset;

	public Mesh m_mesh;

	public float m_x_offset;

	public float m_y_offset;

	public bool m_base_offsets_setup;

	private bool m_waiting_to_sync;

	private bool m_started_action;

	private float m_break_delay;

	private float m_action_progress;

	private int finished_action = -1;

	private float m_action_delay;

	private float m_action_duration;

	private float m_action_timer;

	private float m_linear_progress;

	private List<ActionLoopCycle> m_active_loop_cycles;

	private VertexColour start_colour;

	private VertexColour end_colour;

	private int m_action_index;

	private bool m_reverse;

	private int m_action_index_progress;

	private bool m_active;

	private LetterAction current_letter_action;

	private float m_timer_offset;

	private int m_prev_action_index = -1;

	public AnimationProgressionVariables m_progression_variables;

	public bool WaitingToSync => m_waiting_to_sync;

	public List<ActionLoopCycle> ActiveLoopCycles => m_active_loop_cycles;

	public int ActionIndex => m_action_index;

	public bool InReverse => m_reverse;

	public int ActionProgress => m_action_index_progress;

	public bool Active
	{
		get
		{
			return m_active;
		}
		set
		{
			m_active = value;
		}
	}

	public LetterSetup(string character, int letter_idx, Mesh mesh, Vector3 base_offset, CustomCharacterInfo char_info, int line_num, int word_idx)
	{
		m_character = character;
		m_mesh = mesh;
		m_base_offset = base_offset;
		m_progression_variables = new AnimationProgressionVariables(letter_idx, word_idx, line_num);
		SetupLetterMesh(char_info);
		if (m_flipped)
		{
			m_mesh.uv = new Vector2[4]
			{
				mesh.uv[3],
				mesh.uv[2],
				mesh.uv[1],
				mesh.uv[0]
			};
		}
	}

	public void SetupLetterMesh(CustomCharacterInfo char_info)
	{
		m_offset_width = char_info.width;
		m_width = char_info.vert.width;
		m_height = char_info.vert.height;
		m_flipped = char_info.flipped;
		m_x_offset = char_info.vert.x;
		m_y_offset = char_info.vert.y;
		if (!m_flipped)
		{
			m_base_vertices = new Vector3[4]
			{
				new Vector3(m_width, 0f, 0f),
				new Vector3(0f, 0f, 0f),
				new Vector3(0f, m_height, 0f),
				new Vector3(m_width, m_height, 0f)
			};
		}
		else
		{
			m_base_vertices = new Vector3[4]
			{
				new Vector3(0f, 0f, 0f),
				new Vector3(0f, m_height, 0f),
				new Vector3(m_width, m_height, 0f),
				new Vector3(m_width, 0f, 0f)
			};
		}
	}

	public void Reset(LetterAnimation animation)
	{
		m_timer_offset = 0f;
		m_waiting_to_sync = false;
		m_break_delay = 0f;
		m_action_index = 0;
		m_action_index_progress = 0;
		m_active_loop_cycles = new List<ActionLoopCycle>();
		m_reverse = false;
		m_prev_action_index = -1;
		if (animation.m_loop_cycles.Count > 0)
		{
			UpdateLoopList(animation);
		}
	}

	public void SetBaseOffset(TextAnchor anchor, TextDisplayAxis display_axis, TextAlignment alignment, List<TextSizeData> text_datas)
	{
		SetupBaseOffsets(anchor, display_axis, alignment, text_datas[m_progression_variables.m_line_value]);
		m_base_offsets_setup = true;
	}

	private void SetupBaseOffsets(TextAnchor anchor, TextDisplayAxis display_axis, TextAlignment alignment, TextSizeData text_data)
	{
		if (display_axis == TextDisplayAxis.HORIZONTAL)
		{
			m_base_offset += new Vector3(m_x_offset, m_y_offset - text_data.m_line_height_offset, 0f);
		}
		else
		{
			m_base_offset += new Vector3(text_data.m_line_height_offset, 0f, 0f);
		}
		m_base_offset.y -= text_data.m_y_max;
		switch (anchor)
		{
		case TextAnchor.MiddleLeft:
		case TextAnchor.MiddleCenter:
		case TextAnchor.MiddleRight:
			m_base_offset.y += text_data.m_total_text_height / 2f;
			break;
		case TextAnchor.LowerLeft:
		case TextAnchor.LowerCenter:
		case TextAnchor.LowerRight:
			m_base_offset.y += text_data.m_total_text_height;
			break;
		}
		float num = 0f;
		if (display_axis == TextDisplayAxis.HORIZONTAL)
		{
			switch (alignment)
			{
			case TextAlignment.Center:
				num = (text_data.m_total_text_width - text_data.m_text_line_width) / 2f;
				break;
			case TextAlignment.Right:
				num = text_data.m_total_text_width - text_data.m_text_line_width;
				break;
			}
		}
		else
		{
			switch (alignment)
			{
			case TextAlignment.Center:
				m_base_offset.y -= (text_data.m_total_text_height - text_data.m_text_line_height) / 2f;
				break;
			case TextAlignment.Right:
				m_base_offset.y -= text_data.m_total_text_height - text_data.m_text_line_height;
				break;
			}
		}
		switch (anchor)
		{
		case TextAnchor.UpperRight:
		case TextAnchor.MiddleRight:
		case TextAnchor.LowerRight:
			m_base_offset.x -= text_data.m_total_text_width - num;
			break;
		case TextAnchor.UpperCenter:
		case TextAnchor.MiddleCenter:
		case TextAnchor.LowerCenter:
			m_base_offset.x -= text_data.m_total_text_width / 2f - num;
			break;
		default:
			m_base_offset.x += num;
			break;
		}
	}

	public void SetMeshState(int action_idx, float action_progress, LetterAnimation animation, AnimatePerOptions animate_per)
	{
		if (action_idx >= 0 && action_idx < animation.m_letter_actions.Count)
		{
			SetupMesh(animation.m_letter_actions[action_idx], Mathf.Clamp(action_progress, 0f, 1f), first_action_call: true, m_progression_variables, animate_per, Mathf.Clamp(action_progress, 0f, 1f));
			return;
		}
		Vector3[] array = new Vector3[4];
		for (int i = 0; i < 4; i++)
		{
			ref Vector3 reference = ref array[i];
			reference = m_base_vertices[i] + m_base_offset;
		}
		m_mesh.vertices = array;
		m_mesh.colors = new Color[4]
		{
			Color.white,
			Color.white,
			Color.white,
			Color.white
		};
	}

	private void SetNextActionIndex(LetterAnimation animation)
	{
		m_action_index_progress++;
		int num = 0;
		while (num < m_active_loop_cycles.Count)
		{
			ActionLoopCycle actionLoopCycle = m_active_loop_cycles[num];
			if ((actionLoopCycle.m_loop_type != 0 || m_action_index != actionLoopCycle.m_end_action_idx) && (actionLoopCycle.m_loop_type != LOOP_TYPE.LOOP_REVERSE || ((!m_reverse || m_action_index != actionLoopCycle.m_start_action_idx) && (m_reverse || m_action_index != actionLoopCycle.m_end_action_idx))))
			{
				break;
			}
			bool flag = actionLoopCycle.m_loop_type == LOOP_TYPE.LOOP || m_reverse;
			if (flag)
			{
				actionLoopCycle.m_number_of_loops--;
			}
			if (actionLoopCycle.m_loop_type == LOOP_TYPE.LOOP_REVERSE)
			{
				m_reverse = !m_reverse;
			}
			if (flag && actionLoopCycle.m_number_of_loops == 0)
			{
				m_active_loop_cycles.RemoveAt(num);
				num--;
				if (actionLoopCycle.m_loop_type == LOOP_TYPE.LOOP_REVERSE)
				{
					m_action_index = actionLoopCycle.m_end_action_idx;
				}
				num++;
				continue;
			}
			if (actionLoopCycle.m_number_of_loops < 0)
			{
				actionLoopCycle.m_number_of_loops = -1;
			}
			if (actionLoopCycle.m_loop_type == LOOP_TYPE.LOOP)
			{
				m_action_index = actionLoopCycle.m_start_action_idx;
			}
			return;
		}
		m_action_index += ((!m_reverse) ? 1 : (-1));
		if (m_action_index >= animation.m_letter_actions.Count)
		{
			m_active = false;
			m_action_index = animation.m_letter_actions.Count - 1;
		}
	}

	private void UpdateLoopList(LetterAnimation animation)
	{
		foreach (ActionLoopCycle loop_cycle in animation.m_loop_cycles)
		{
			if (loop_cycle.m_start_action_idx != m_action_index)
			{
				continue;
			}
			int spanWidth = loop_cycle.SpanWidth;
			int num = 0;
			foreach (ActionLoopCycle active_loop_cycle in m_active_loop_cycles)
			{
				if (loop_cycle.m_start_action_idx == active_loop_cycle.m_start_action_idx && loop_cycle.m_end_action_idx == active_loop_cycle.m_end_action_idx)
				{
					num = -1;
					break;
				}
				if (spanWidth < active_loop_cycle.SpanWidth)
				{
					break;
				}
				num++;
			}
			if (num >= 0)
			{
				m_active_loop_cycles.Insert(num, loop_cycle.Clone());
			}
		}
	}

	public void ContinueAction(float animation_timer, LetterAnimation animation, AnimatePerOptions animate_per)
	{
		if (!m_waiting_to_sync)
		{
			return;
		}
		m_break_delay = 0f;
		m_waiting_to_sync = false;
		m_timer_offset = animation_timer;
		int action_index = m_action_index;
		SetNextActionIndex(animation);
		if (m_active)
		{
			if (!m_reverse && m_action_index_progress > m_action_index)
			{
				animation.m_letter_actions[m_action_index].SoftReset(animation.m_letter_actions[action_index], m_progression_variables, animate_per);
			}
			if (action_index != m_action_index)
			{
				UpdateLoopList(animation);
			}
		}
	}

	public LETTER_ANIMATION_STATE AnimateMesh(float timer, TextAnchor text_anchor, int lowest_action_progress, LetterAnimation animation, AnimatePerOptions animate_per, float delta_time, EffectManager effect_manager)
	{
		if (animation.m_letter_actions.Count > 0 && m_action_index < animation.m_letter_actions.Count)
		{
			if (!m_active)
			{
				return LETTER_ANIMATION_STATE.STOPPED;
			}
			bool first_action_call = false;
			if (m_action_index != m_prev_action_index)
			{
				current_letter_action = animation.m_letter_actions[m_action_index];
				first_action_call = true;
				m_started_action = false;
			}
			m_prev_action_index = m_action_index;
			if (m_waiting_to_sync)
			{
				if (current_letter_action.m_action_type == ACTION_TYPE.BREAK)
				{
					if (m_break_delay > 0f)
					{
						m_break_delay -= delta_time;
						if (m_break_delay <= 0f)
						{
							ContinueAction(timer, animation, animate_per);
							return LETTER_ANIMATION_STATE.PLAYING;
						}
					}
					return LETTER_ANIMATION_STATE.WAITING;
				}
				if (lowest_action_progress < m_action_index_progress)
				{
					return LETTER_ANIMATION_STATE.PLAYING;
				}
				m_waiting_to_sync = false;
				m_timer_offset = timer;
			}
			else if (current_letter_action.m_action_type == ACTION_TYPE.BREAK || (!m_reverse && current_letter_action.m_force_same_start_time && lowest_action_progress < m_action_index_progress))
			{
				m_waiting_to_sync = true;
				m_break_delay = Mathf.Max(current_letter_action.m_duration_progression.GetValue(m_progression_variables, animate_per), 0f);
				return LETTER_ANIMATION_STATE.PLAYING;
			}
			finished_action = -1;
			m_action_progress = 0f;
			m_linear_progress = 0f;
			m_action_delay = Mathf.Max(current_letter_action.m_delay_progression.GetValue(m_progression_variables, animate_per), 0f);
			m_action_duration = Mathf.Max(current_letter_action.m_duration_progression.GetValue(m_progression_variables, animate_per), 0f);
			m_action_timer = timer - m_timer_offset;
			if (m_reverse || m_action_timer > m_action_delay)
			{
				m_linear_progress = (m_action_timer - ((!m_reverse) ? m_action_delay : 0f)) / m_action_duration;
				if (m_reverse)
				{
					if (m_action_timer >= m_action_duration)
					{
						m_linear_progress = 0f;
					}
					else
					{
						m_linear_progress = 1f - m_linear_progress;
					}
				}
				if (!m_started_action)
				{
					if (current_letter_action.m_audio_on_start != null && (m_progression_variables.m_letter_value == 0 || !current_letter_action.m_starting_in_sync))
					{
						effect_manager.PlayAudioClip(current_letter_action.m_audio_on_start, current_letter_action.m_audio_on_start_delay.GetValue(m_progression_variables, animate_per), current_letter_action.m_audio_on_start_offset.GetValue(m_progression_variables, animate_per), current_letter_action.m_audio_on_start_volume.GetValue(m_progression_variables, animate_per), current_letter_action.m_audio_on_start_pitch.GetValue(m_progression_variables, animate_per));
					}
					if (current_letter_action.m_emitter_on_start != null && (current_letter_action.m_emitter_on_start_per_letter || m_progression_variables.m_letter_value == 0))
					{
						effect_manager.PlayParticleEffect(current_letter_action.m_emitter_on_start, current_letter_action.m_emitter_on_start_delay.GetValue(m_progression_variables, animate_per), current_letter_action.m_emitter_on_start_duration.GetValue(m_progression_variables, animate_per), m_mesh, current_letter_action.m_emitter_on_start_offset.GetValue(m_progression_variables, animate_per), current_letter_action.m_emitter_on_start_follow_mesh);
					}
					m_started_action = true;
				}
				m_action_progress = EasingManager.GetEaseProgress(current_letter_action.m_ease_type, m_linear_progress);
				if ((!m_reverse && m_linear_progress >= 1f) || (m_reverse && m_action_timer >= m_action_duration + m_action_delay))
				{
					m_action_progress = ((!m_reverse) ? 1 : 0);
					m_linear_progress = ((!m_reverse) ? 1 : 0);
					if (!m_reverse)
					{
						finished_action = m_action_index;
					}
					else
					{
						m_started_action = false;
					}
					int action_index = m_action_index;
					SetNextActionIndex(animation);
					if (m_active)
					{
						if (!m_reverse && m_action_index_progress > m_action_index)
						{
							animation.m_letter_actions[m_action_index].SoftReset(animation.m_letter_actions[action_index], m_progression_variables, animate_per);
						}
						m_timer_offset += m_action_delay + m_action_duration;
						if (action_index != m_action_index)
						{
							UpdateLoopList(animation);
						}
					}
				}
			}
			SetupMesh(current_letter_action, m_action_progress, first_action_call, m_progression_variables, animate_per, m_linear_progress);
			if (finished_action > -1)
			{
				if (current_letter_action.m_audio_on_finish != null && (m_progression_variables.m_letter_value == 0 || !current_letter_action.m_starting_in_sync))
				{
					effect_manager.PlayAudioClip(current_letter_action.m_audio_on_finish, current_letter_action.m_audio_on_finish_delay.GetValue(m_progression_variables, animate_per), current_letter_action.m_audio_on_finish_offset.GetValue(m_progression_variables, animate_per), current_letter_action.m_audio_on_finish_volume.GetValue(m_progression_variables, animate_per), current_letter_action.m_audio_on_finish_pitch.GetValue(m_progression_variables, animate_per));
				}
				if (current_letter_action.m_emitter_on_finish != null && (current_letter_action.m_emitter_on_finish_per_letter || m_progression_variables.m_letter_value == 0))
				{
					effect_manager.PlayParticleEffect(current_letter_action.m_emitter_on_finish, current_letter_action.m_emitter_on_finish_delay.GetValue(m_progression_variables, animate_per), current_letter_action.m_emitter_on_finish_duration.GetValue(m_progression_variables, animate_per), m_mesh, current_letter_action.m_emitter_on_finish_offset.GetValue(m_progression_variables, animate_per), current_letter_action.m_emitter_on_finish_follow_mesh);
				}
			}
		}
		else
		{
			Vector3[] array = new Vector3[4];
			for (int i = 0; i < 4; i++)
			{
				ref Vector3 reference = ref array[i];
				reference = m_base_vertices[i] + m_base_offset;
			}
			m_mesh.vertices = array;
		}
		return (!m_active) ? LETTER_ANIMATION_STATE.STOPPED : LETTER_ANIMATION_STATE.PLAYING;
	}

	private void SetupMesh(LetterAction letter_action, float action_progress, bool first_action_call, AnimationProgressionVariables progression_variables, AnimatePerOptions animate_per, float linear_progress)
	{
		if (first_action_call || !letter_action.StaticPosition || !letter_action.StaticRotation || !letter_action.StaticScale)
		{
			Vector3[] array = new Vector3[4];
			for (int i = 0; i < 4; i++)
			{
				Vector3 zero = Vector3.zero;
				if (letter_action.m_letter_anchor == TextAnchor.UpperRight || letter_action.m_letter_anchor == TextAnchor.MiddleRight || letter_action.m_letter_anchor == TextAnchor.LowerRight)
				{
					zero += new Vector3(m_width, 0f, 0f);
				}
				else if (letter_action.m_letter_anchor == TextAnchor.UpperCenter || letter_action.m_letter_anchor == TextAnchor.MiddleCenter || letter_action.m_letter_anchor == TextAnchor.LowerCenter)
				{
					zero += new Vector3(m_width / 2f, 0f, 0f);
				}
				if (letter_action.m_letter_anchor == TextAnchor.MiddleLeft || letter_action.m_letter_anchor == TextAnchor.MiddleCenter || letter_action.m_letter_anchor == TextAnchor.MiddleRight)
				{
					zero += new Vector3(0f, m_height / 2f, 0f);
				}
				else if (letter_action.m_letter_anchor == TextAnchor.LowerLeft || letter_action.m_letter_anchor == TextAnchor.LowerCenter || letter_action.m_letter_anchor == TextAnchor.LowerRight)
				{
					zero += new Vector3(0f, m_height, 0f);
				}
				ref Vector3 reference = ref array[i];
				reference = m_base_vertices[i];
				array[i] -= zero;
				Vector3 value = letter_action.m_start_scale.GetValue(progression_variables, animate_per);
				Vector3 value2 = letter_action.m_end_scale.GetValue(progression_variables, animate_per);
				if (letter_action.m_scale_axis_ease_data.m_override_default)
				{
					ref Vector3 reference2 = ref array[i];
					reference2 = Vector3.Scale(array[i], new Vector3(EffectManager.FloatLerp(value.x, value2.x, EasingManager.GetEaseProgress(letter_action.m_scale_axis_ease_data.m_x_ease, linear_progress)), EffectManager.FloatLerp(value.y, value2.y, EasingManager.GetEaseProgress(letter_action.m_scale_axis_ease_data.m_y_ease, linear_progress)), EffectManager.FloatLerp(value.z, value2.z, EasingManager.GetEaseProgress(letter_action.m_scale_axis_ease_data.m_z_ease, linear_progress))));
				}
				else
				{
					ref Vector3 reference3 = ref array[i];
					reference3 = Vector3.Scale(array[i], EffectManager.Vector3Lerp(value, value2, action_progress));
				}
				Vector3 value3 = letter_action.m_start_euler_rotation.GetValue(progression_variables, animate_per);
				Vector3 value4 = letter_action.m_end_euler_rotation.GetValue(progression_variables, animate_per);
				if (letter_action.m_rotation_axis_ease_data.m_override_default)
				{
					ref Vector3 reference4 = ref array[i];
					reference4 = Quaternion.Euler(EffectManager.FloatLerp(value3.x, value4.x, EasingManager.GetEaseProgress(letter_action.m_rotation_axis_ease_data.m_x_ease, linear_progress)), EffectManager.FloatLerp(value3.y, value4.y, EasingManager.GetEaseProgress(letter_action.m_rotation_axis_ease_data.m_y_ease, linear_progress)), EffectManager.FloatLerp(value3.z, value4.z, EasingManager.GetEaseProgress(letter_action.m_rotation_axis_ease_data.m_z_ease, linear_progress))) * array[i];
				}
				else
				{
					ref Vector3 reference5 = ref array[i];
					reference5 = Quaternion.Euler(EffectManager.Vector3Lerp(value3, value4, action_progress)) * array[i];
				}
				array[i] += zero;
				Vector3 from_vec = (letter_action.m_start_pos.m_force_position_override ? Vector3.zero : m_base_offset) + letter_action.m_start_pos.GetValue(progression_variables, animate_per);
				Vector3 to_vec = (letter_action.m_end_pos.m_force_position_override ? Vector3.zero : m_base_offset) + letter_action.m_end_pos.GetValue(progression_variables, animate_per);
				if (letter_action.m_position_axis_ease_data.m_override_default)
				{
					array[i] += new Vector3(EffectManager.FloatLerp(from_vec.x, to_vec.x, EasingManager.GetEaseProgress(letter_action.m_position_axis_ease_data.m_x_ease, linear_progress)), EffectManager.FloatLerp(from_vec.y, to_vec.y, EasingManager.GetEaseProgress(letter_action.m_position_axis_ease_data.m_y_ease, linear_progress)), EffectManager.FloatLerp(from_vec.z, to_vec.z, EasingManager.GetEaseProgress(letter_action.m_position_axis_ease_data.m_z_ease, linear_progress)));
				}
				else
				{
					array[i] += EffectManager.Vector3Lerp(from_vec, to_vec, action_progress);
				}
			}
			m_mesh.vertices = array;
		}
		if (first_action_call || !letter_action.StaticColour)
		{
			if (letter_action.m_use_gradient_start || letter_action.m_use_gradient)
			{
				start_colour = letter_action.m_start_vertex_colour.GetValue(progression_variables, animate_per);
			}
			else
			{
				start_colour = new VertexColour(letter_action.m_start_colour.GetValue(progression_variables, animate_per));
			}
			if (letter_action.m_use_gradient_end || letter_action.m_use_gradient)
			{
				end_colour = letter_action.m_end_vertex_colour.GetValue(progression_variables, animate_per);
			}
			else
			{
				end_colour = new VertexColour(letter_action.m_end_colour.GetValue(progression_variables, animate_per));
			}
			if (!m_flipped)
			{
				m_mesh.colors = new Color[4]
				{
					Color.Lerp(start_colour.top_right, end_colour.top_right, action_progress),
					Color.Lerp(start_colour.top_left, end_colour.top_left, action_progress),
					Color.Lerp(start_colour.bottom_left, end_colour.bottom_left, action_progress),
					Color.Lerp(start_colour.bottom_right, end_colour.bottom_right, action_progress)
				};
			}
			else
			{
				m_mesh.colors = new Color[4]
				{
					Color.Lerp(start_colour.top_left, end_colour.top_left, action_progress),
					Color.Lerp(start_colour.bottom_left, end_colour.bottom_left, action_progress),
					Color.Lerp(start_colour.bottom_right, end_colour.bottom_right, action_progress),
					Color.Lerp(start_colour.top_right, end_colour.top_right, action_progress)
				};
			}
		}
	}
}
