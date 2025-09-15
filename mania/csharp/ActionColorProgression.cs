using System;
using UnityEngine;

[Serializable]
public class ActionColorProgression : ActionVariableProgression
{
	public Color[] m_values;

	public Color m_from = Color.white;

	public Color m_to = Color.white;

	public Color m_to_to = Color.white;

	public int NumEditorLines => (m_progression != 0) ? 3 : 2;

	public ActionColorProgression(Color start_colour)
	{
		m_from = start_colour;
		m_to = start_colour;
		m_to_to = start_colour;
	}

	public Color GetValue(AnimationProgressionVariables progression_variables, AnimatePerOptions animate_per_default)
	{
		return GetValue(GetProgressionIndex(progression_variables, animate_per_default));
	}

	public Color GetValue(int progression_idx)
	{
		int num = m_values.Length;
		if (num > 1 && progression_idx < num)
		{
			return m_values[progression_idx];
		}
		if (num == 1)
		{
			return m_values[0];
		}
		return Color.white;
	}

	public void CalculateUniqueRandom(AnimationProgressionVariables progression_variables, AnimatePerOptions animate_per, Color[] offset_cols)
	{
		int progressionIndex = GetProgressionIndex(progression_variables, animate_per);
		bool flag = offset_cols != null && offset_cols.Length == 1;
		ref Color reference = ref m_values[progressionIndex];
		reference = ((!m_is_offset_from_last) ? new Color(0f, 0f, 0f, 0f) : offset_cols[(!flag) ? progressionIndex : 0]);
		m_values[progressionIndex] += m_from + (m_to - m_from) * UnityEngine.Random.value;
	}

	public void CalculateProgressions(int num_progressions, Color[] offset_cols)
	{
		if (m_progression == ValueProgression.Eased || m_progression == ValueProgression.Random || (m_is_offset_from_last && offset_cols.Length > 1))
		{
			bool flag = offset_cols != null && offset_cols.Length == 1;
			m_values = new Color[num_progressions];
			for (int i = 0; i < num_progressions; i++)
			{
				ref Color reference = ref m_values[i];
				reference = ((!m_is_offset_from_last) ? new Color(0f, 0f, 0f, 0f) : offset_cols[(!flag) ? i : 0]);
			}
		}
		else
		{
			m_values = new Color[1]
			{
				(!m_is_offset_from_last) ? new Color(0f, 0f, 0f, 0f) : offset_cols[0]
			};
		}
		if (m_progression == ValueProgression.Random)
		{
			for (int j = 0; j < num_progressions; j++)
			{
				m_values[j] += m_from + (m_to - m_from) * UnityEngine.Random.value;
			}
		}
		else if (m_progression == ValueProgression.Eased)
		{
			for (int k = 0; k < num_progressions; k++)
			{
				float num = (num_progressions != 1) ? ((float)k / ((float)num_progressions - 1f)) : 0f;
				if (m_to_to_bool)
				{
					if (num <= 0.5f)
					{
						m_values[k] += m_from + (m_to - m_from) * EasingManager.GetEaseProgress(m_ease_type, num / 0.5f);
						continue;
					}
					num -= 0.5f;
					m_values[k] += m_to + (m_to_to - m_to) * EasingManager.GetEaseProgress(EasingManager.GetEaseTypeOpposite(m_ease_type), num / 0.5f);
				}
				else
				{
					m_values[k] += m_from + (m_to - m_from) * EasingManager.GetEaseProgress(m_ease_type, num);
				}
			}
		}
		else if (m_progression == ValueProgression.Constant)
		{
			for (int l = 0; l < m_values.Length; l++)
			{
				m_values[l] += m_from;
			}
		}
	}

	public ActionColorProgression Clone()
	{
		ActionColorProgression actionColorProgression = new ActionColorProgression(Color.white);
		actionColorProgression.m_progression = m_progression;
		actionColorProgression.m_ease_type = m_ease_type;
		actionColorProgression.m_from = m_from;
		actionColorProgression.m_to = m_to;
		actionColorProgression.m_to_to = m_to_to;
		actionColorProgression.m_to_to_bool = m_to_to_bool;
		actionColorProgression.m_is_offset_from_last = m_is_offset_from_last;
		actionColorProgression.m_unique_randoms = m_unique_randoms;
		actionColorProgression.m_override_animate_per_option = m_override_animate_per_option;
		actionColorProgression.m_animate_per = m_animate_per;
		return actionColorProgression;
	}
}
