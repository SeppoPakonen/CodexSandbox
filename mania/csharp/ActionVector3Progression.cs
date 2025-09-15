using System;
using UnityEngine;

[Serializable]
public class ActionVector3Progression : ActionVariableProgression
{
	public Vector3[] m_values;

	public Vector3 m_from = Vector3.zero;

	public Vector3 m_to = Vector3.zero;

	public Vector3 m_to_to = Vector3.zero;

	public EasingEquation m_x_ease;

	public EasingEquation m_y_ease;

	public EasingEquation m_z_ease;

	public bool m_override_per_axis_easing;

	public virtual int NumEditorLines
	{
		get
		{
			if (m_progression == ValueProgression.Constant)
			{
				return 3;
			}
			if (m_progression == ValueProgression.Random || (m_progression == ValueProgression.Eased && !m_to_to_bool))
			{
				return 6;
			}
			return 8;
		}
	}

	public ActionVector3Progression()
	{
	}

	public ActionVector3Progression(Vector3 start_vec)
	{
		m_from = start_vec;
		m_to = start_vec;
		m_to_to = start_vec;
	}

	public Vector3 GetValue(AnimationProgressionVariables progression_variables, AnimatePerOptions animate_per_default)
	{
		return GetValue(GetProgressionIndex(progression_variables, animate_per_default));
	}

	public Vector3 GetValue(int progression_idx)
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
		return Vector3.zero;
	}

	public void CalculateUniqueRandom(AnimationProgressionVariables progression_variables, AnimatePerOptions animate_per, Vector3[] offset_vec)
	{
		int progressionIndex = GetProgressionIndex(progression_variables, animate_per);
		bool flag = offset_vec != null && offset_vec.Length == 1;
		ref Vector3 reference = ref m_values[progressionIndex];
		reference = ((!m_is_offset_from_last) ? Vector3.zero : offset_vec[(!flag) ? progressionIndex : 0]);
		m_values[progressionIndex] += new Vector3(m_from.x + (m_to.x - m_from.x) * UnityEngine.Random.value, m_from.y + (m_to.y - m_from.y) * UnityEngine.Random.value, m_from.z + (m_to.z - m_from.z) * UnityEngine.Random.value);
	}

	public void CalculateProgressions(int num_progressions, Vector3[] offset_vecs)
	{
		if (m_progression == ValueProgression.Eased || m_progression == ValueProgression.Random || (m_is_offset_from_last && offset_vecs.Length > 1))
		{
			bool flag = offset_vecs != null && offset_vecs.Length == 1;
			m_values = new Vector3[num_progressions];
			for (int i = 0; i < num_progressions; i++)
			{
				ref Vector3 reference = ref m_values[i];
				reference = ((!m_is_offset_from_last) ? Vector3.zero : offset_vecs[(!flag) ? i : 0]);
			}
		}
		else
		{
			m_values = new Vector3[1]
			{
				(!m_is_offset_from_last || offset_vecs.Length < 1) ? Vector3.zero : offset_vecs[0]
			};
		}
		if (m_progression == ValueProgression.Random)
		{
			for (int j = 0; j < num_progressions; j++)
			{
				m_values[j] += new Vector3(m_from.x + (m_to.x - m_from.x) * UnityEngine.Random.value, m_from.y + (m_to.y - m_from.y) * UnityEngine.Random.value, m_from.z + (m_to.z - m_from.z) * UnityEngine.Random.value);
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

	public ActionVector3Progression Clone()
	{
		ActionVector3Progression actionVector3Progression = new ActionVector3Progression(Vector3.zero);
		actionVector3Progression.m_progression = m_progression;
		actionVector3Progression.m_ease_type = m_ease_type;
		actionVector3Progression.m_from = m_from;
		actionVector3Progression.m_to = m_to;
		actionVector3Progression.m_to_to = m_to_to;
		actionVector3Progression.m_to_to_bool = m_to_to_bool;
		actionVector3Progression.m_is_offset_from_last = m_is_offset_from_last;
		actionVector3Progression.m_unique_randoms = m_unique_randoms;
		actionVector3Progression.m_override_animate_per_option = m_override_animate_per_option;
		actionVector3Progression.m_animate_per = m_animate_per;
		return actionVector3Progression;
	}
}
