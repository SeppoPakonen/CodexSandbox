using System;
using UnityEngine;

[Serializable]
public class ActionVertexColorProgression : ActionVariableProgression
{
	public VertexColour[] m_values;

	public VertexColour m_from = new VertexColour();

	public VertexColour m_to = new VertexColour();

	public VertexColour m_to_to = new VertexColour();

	public int NumEditorLines => (m_progression != 0) ? 4 : 3;

	public ActionVertexColorProgression(VertexColour start_colour)
	{
		m_from = start_colour.Clone();
		m_to = start_colour.Clone();
		m_to_to = start_colour.Clone();
	}

	public VertexColour GetValue(AnimationProgressionVariables progression_variables, AnimatePerOptions animate_per_default)
	{
		return GetValue(GetProgressionIndex(progression_variables, animate_per_default));
	}

	public VertexColour GetValue(int progression_idx)
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
		return new VertexColour(Color.white);
	}

	public void ConvertFromFlatColourProg(ActionColorProgression flat_colour_progression)
	{
		m_progression = flat_colour_progression.m_progression;
		m_ease_type = flat_colour_progression.m_ease_type;
		m_from = new VertexColour(flat_colour_progression.m_from);
		m_to = new VertexColour(flat_colour_progression.m_to);
		m_to_to = new VertexColour(flat_colour_progression.m_to_to);
		m_to_to_bool = flat_colour_progression.m_to_to_bool;
		m_is_offset_from_last = flat_colour_progression.m_is_offset_from_last;
		m_unique_randoms = flat_colour_progression.m_unique_randoms;
	}

	public void CalculateUniqueRandom(AnimationProgressionVariables progression_variables, AnimatePerOptions animate_per, VertexColour[] offset_colours)
	{
		int progressionIndex = GetProgressionIndex(progression_variables, animate_per);
		bool flag = offset_colours != null && offset_colours.Length == 1;
		m_values[progressionIndex] = ((!m_is_offset_from_last) ? new VertexColour(new Color(0f, 0f, 0f, 0f)) : offset_colours[(!flag) ? progressionIndex : 0].Clone());
		m_values[progressionIndex] = m_values[progressionIndex].Add(m_from.Add(m_to.Sub(m_from).Multiply(UnityEngine.Random.value)));
	}

	public void CalculateProgressions(int num_progressions, VertexColour[] offset_vert_colours, Color[] offset_colours)
	{
		if (m_progression == ValueProgression.Eased || m_progression == ValueProgression.Random || (m_is_offset_from_last && ((offset_colours != null && offset_colours.Length > 1) || (offset_vert_colours != null && offset_vert_colours.Length > 1))))
		{
			bool flag = (offset_colours != null && offset_colours.Length == 1) || (offset_vert_colours != null && offset_vert_colours.Length == 1);
			m_values = new VertexColour[num_progressions];
			for (int i = 0; i < num_progressions; i++)
			{
				m_values[i] = ((!m_is_offset_from_last) ? new VertexColour(new Color(0f, 0f, 0f, 0f)) : ((offset_colours == null) ? offset_vert_colours[(!flag) ? i : 0].Clone() : new VertexColour(offset_colours[(!flag) ? i : 0])));
			}
		}
		else
		{
			m_values = new VertexColour[1]
			{
				(!m_is_offset_from_last) ? new VertexColour(new Color(0f, 0f, 0f, 0f)) : ((offset_colours == null) ? offset_vert_colours[0].Clone() : new VertexColour(offset_colours[0]))
			};
		}
		if (m_progression == ValueProgression.Random)
		{
			for (int j = 0; j < num_progressions; j++)
			{
				m_values[j] = m_values[j].Add(m_from.Add(m_to.Sub(m_from).Multiply(UnityEngine.Random.value)));
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
						m_values[k] = m_values[k].Add(m_from.Add(m_to.Sub(m_from).Multiply(EasingManager.GetEaseProgress(m_ease_type, num / 0.5f))));
						continue;
					}
					num -= 0.5f;
					m_values[k] = m_values[k].Add(m_to.Add(m_to_to.Sub(m_to).Multiply(EasingManager.GetEaseProgress(m_ease_type, num / 0.5f))));
				}
				else
				{
					m_values[k] = m_values[k].Add(m_from.Add(m_to.Sub(m_from).Multiply(EasingManager.GetEaseProgress(m_ease_type, num))));
				}
			}
		}
		else if (m_progression == ValueProgression.Constant)
		{
			for (int l = 0; l < m_values.Length; l++)
			{
				m_values[l] = m_values[l].Add(m_from);
			}
		}
	}

	public ActionVertexColorProgression Clone()
	{
		ActionVertexColorProgression actionVertexColorProgression = new ActionVertexColorProgression(new VertexColour());
		actionVertexColorProgression.m_progression = m_progression;
		actionVertexColorProgression.m_ease_type = m_ease_type;
		actionVertexColorProgression.m_from = m_from.Clone();
		actionVertexColorProgression.m_to = m_to.Clone();
		actionVertexColorProgression.m_to_to = m_to_to.Clone();
		actionVertexColorProgression.m_to_to_bool = m_to_to_bool;
		actionVertexColorProgression.m_is_offset_from_last = m_is_offset_from_last;
		actionVertexColorProgression.m_unique_randoms = m_unique_randoms;
		actionVertexColorProgression.m_override_animate_per_option = m_override_animate_per_option;
		actionVertexColorProgression.m_animate_per = m_animate_per;
		return actionVertexColorProgression;
	}
}
