using System;
using UnityEngine;

[Serializable]
public class ActionFloatProgression : ActionVariableProgression
{
	public float[] m_values;

	public float m_from;

	public float m_to;

	public float m_to_to;

	public int NumEditorLines
	{
		get
		{
			if (m_progression == ValueProgression.Constant)
			{
				return 2;
			}
			if (m_progression == ValueProgression.Random || (m_progression == ValueProgression.Eased && !m_to_to_bool))
			{
				return 4;
			}
			return 5;
		}
	}

	public ActionFloatProgression(float start_val)
	{
		m_from = start_val;
		m_to = start_val;
		m_to_to = start_val;
	}

	public float GetValue(AnimationProgressionVariables progression_variables, AnimatePerOptions animate_per_default)
	{
		return GetValue(GetProgressionIndex(progression_variables, animate_per_default));
	}

	public float GetValue(int progression_idx)
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
		return 0f;
	}

	public void CalculateUniqueRandom(AnimationProgressionVariables progression_variables, AnimatePerOptions animate_per)
	{
		m_values[GetProgressionIndex(progression_variables, animate_per)] = m_from + (m_to - m_from) * UnityEngine.Random.value;
	}

	public void CalculateProgressions(int num_progressions)
	{
		m_values = new float[(m_progression != ValueProgression.Eased && m_progression != ValueProgression.Random) ? 1 : num_progressions];
		if (m_progression == ValueProgression.Random)
		{
			for (int i = 0; i < num_progressions; i++)
			{
				m_values[i] = m_from + (m_to - m_from) * UnityEngine.Random.value;
			}
		}
		else if (m_progression == ValueProgression.Eased)
		{
			for (int j = 0; j < num_progressions; j++)
			{
				float num = (num_progressions != 1) ? ((float)j / ((float)num_progressions - 1f)) : 0f;
				if (m_to_to_bool)
				{
					if (num <= 0.5f)
					{
						m_values[j] = m_from + (m_to - m_from) * EasingManager.GetEaseProgress(m_ease_type, num / 0.5f);
						continue;
					}
					num -= 0.5f;
					m_values[j] = m_to + (m_to_to - m_to) * EasingManager.GetEaseProgress(EasingManager.GetEaseTypeOpposite(m_ease_type), num / 0.5f);
				}
				else
				{
					m_values[j] = m_from + (m_to - m_from) * EasingManager.GetEaseProgress(m_ease_type, num);
				}
			}
		}
		else if (m_progression == ValueProgression.Constant)
		{
			m_values[0] = m_from;
		}
	}

	public ActionFloatProgression Clone()
	{
		ActionFloatProgression actionFloatProgression = new ActionFloatProgression(0f);
		actionFloatProgression.m_progression = m_progression;
		actionFloatProgression.m_ease_type = m_ease_type;
		actionFloatProgression.m_from = m_from;
		actionFloatProgression.m_to = m_to;
		actionFloatProgression.m_to_to = m_to_to;
		actionFloatProgression.m_to_to_bool = m_to_to_bool;
		actionFloatProgression.m_unique_randoms = m_unique_randoms;
		actionFloatProgression.m_override_animate_per_option = m_override_animate_per_option;
		actionFloatProgression.m_animate_per = m_animate_per;
		return actionFloatProgression;
	}
}
