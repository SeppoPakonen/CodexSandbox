using System;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
public class LetterAnimation
{
	public List<LetterAction> m_letter_actions = new List<LetterAction>();

	public List<ActionLoopCycle> m_loop_cycles = new List<ActionLoopCycle>();

	public LETTERS_TO_ANIMATE m_letters_to_animate_option;

	public List<int> m_letters_to_animate;

	private LETTER_ANIMATION_STATE m_animation_state;

	public LETTER_ANIMATION_STATE CurrentAnimationState
	{
		get
		{
			return m_animation_state;
		}
		set
		{
			m_animation_state = value;
		}
	}

	public void AddLoop(int start_idx, int end_idx, bool change_type)
	{
		bool flag = true;
		int index = 0;
		if (end_idx >= start_idx && start_idx >= 0 && start_idx < m_letter_actions.Count && end_idx >= 0 && end_idx < m_letter_actions.Count)
		{
			int num = end_idx - start_idx;
			int num2 = 1;
			foreach (ActionLoopCycle loop_cycle in m_loop_cycles)
			{
				if ((start_idx < loop_cycle.m_start_action_idx && end_idx > loop_cycle.m_start_action_idx && end_idx < loop_cycle.m_end_action_idx) || (end_idx > loop_cycle.m_end_action_idx && start_idx > loop_cycle.m_start_action_idx && start_idx < loop_cycle.m_end_action_idx))
				{
					flag = false;
					Debug.LogWarning("Invalid Loop Added: Loops can not intersect other loops.");
					break;
				}
				if (start_idx == loop_cycle.m_start_action_idx && end_idx == loop_cycle.m_end_action_idx)
				{
					flag = false;
					if (change_type)
					{
						loop_cycle.m_loop_type = ((loop_cycle.m_loop_type == LOOP_TYPE.LOOP) ? LOOP_TYPE.LOOP_REVERSE : LOOP_TYPE.LOOP);
					}
					else
					{
						loop_cycle.m_number_of_loops++;
					}
					break;
				}
				if (num >= loop_cycle.SpanWidth)
				{
					index = num2;
				}
				num2++;
			}
		}
		else
		{
			flag = false;
			Debug.LogWarning("Invalid Loop Added: Check that start/end index are in bounds.");
		}
		if (flag)
		{
			m_loop_cycles.Insert(index, new ActionLoopCycle(start_idx, end_idx));
		}
	}

	public void PrepareData(LetterSetup[] letters, int num_words, int num_lines, AnimatePerOptions animate_per)
	{
		int num = letters.Length;
		if (m_letters_to_animate_option == LETTERS_TO_ANIMATE.ALL_LETTERS)
		{
			m_letters_to_animate = new List<int>();
			for (int i = 0; i < num; i++)
			{
				m_letters_to_animate.Add(i);
				letters[i].m_progression_variables.m_letter_value = i;
			}
		}
		else if (m_letters_to_animate_option == LETTERS_TO_ANIMATE.FIRST_LETTER || m_letters_to_animate_option == LETTERS_TO_ANIMATE.LAST_LETTER)
		{
			m_letters_to_animate = new List<int>();
			m_letters_to_animate.Add((m_letters_to_animate_option != LETTERS_TO_ANIMATE.FIRST_LETTER) ? (letters.Length - 1) : 0);
			letters[(m_letters_to_animate_option != LETTERS_TO_ANIMATE.FIRST_LETTER) ? (letters.Length - 1) : 0].m_progression_variables.m_letter_value = 0;
		}
		else if (m_letters_to_animate_option != LETTERS_TO_ANIMATE.CUSTOM)
		{
			m_letters_to_animate = new List<int>();
			int num2 = (m_letters_to_animate_option != LETTERS_TO_ANIMATE.LAST_LETTER_LINES) ? (-1) : 0;
			int num3 = (m_letters_to_animate_option != LETTERS_TO_ANIMATE.LAST_LETTER_WORDS) ? (-1) : 0;
			int num4 = 0;
			foreach (LetterSetup letterSetup in letters)
			{
				if (letterSetup.m_progression_variables.m_line_value > num2)
				{
					if (m_letters_to_animate_option == LETTERS_TO_ANIMATE.FIRST_LETTER_LINES)
					{
						letterSetup.m_progression_variables.m_letter_value = num4;
						m_letters_to_animate.Add(num4);
					}
					else if (m_letters_to_animate_option == LETTERS_TO_ANIMATE.LAST_LETTER_LINES)
					{
						letterSetup.m_progression_variables.m_letter_value = num4 - 1;
						m_letters_to_animate.Add(num4 - 1);
					}
					num2 = letterSetup.m_progression_variables.m_line_value;
				}
				if (letterSetup.m_progression_variables.m_word_value > num3)
				{
					if (m_letters_to_animate_option == LETTERS_TO_ANIMATE.FIRST_LETTER_WORDS)
					{
						letterSetup.m_progression_variables.m_letter_value = num4;
						m_letters_to_animate.Add(num4);
					}
					else if (m_letters_to_animate_option == LETTERS_TO_ANIMATE.LAST_LETTER_WORDS)
					{
						letterSetup.m_progression_variables.m_letter_value = num4 - 1;
						m_letters_to_animate.Add(num4 - 1);
					}
					num3 = letterSetup.m_progression_variables.m_word_value;
				}
				num4++;
			}
			if (m_letters_to_animate_option == LETTERS_TO_ANIMATE.LAST_LETTER_WORDS || m_letters_to_animate_option == LETTERS_TO_ANIMATE.LAST_LETTER_LINES)
			{
				letters[num - 1].m_progression_variables.m_letter_value = num4 - 1;
				m_letters_to_animate.Add(num4 - 1);
			}
		}
		else
		{
			int num5 = 0;
			for (int k = 0; k < num; k++)
			{
				if (m_letters_to_animate.Contains(k))
				{
					letters[k].m_progression_variables.m_letter_value = num5;
					num5++;
				}
			}
		}
		LetterAction prev_action = null;
		bool flag = true;
		bool prev_action_end_state = true;
		for (int l = 0; l < m_letter_actions.Count; l++)
		{
			LetterAction letterAction = m_letter_actions[l];
			letterAction.PrepareData(m_letters_to_animate.Count, num_words, num_lines, prev_action, animate_per, prev_action_end_state);
			if (letterAction.m_action_type == ACTION_TYPE.ANIM_SEQUENCE)
			{
				prev_action_end_state = true;
				prev_action = letterAction;
			}
			foreach (ActionLoopCycle loop_cycle in m_loop_cycles)
			{
				if (loop_cycle.m_end_action_idx == l && loop_cycle.m_loop_type == LOOP_TYPE.LOOP_REVERSE)
				{
					prev_action = m_letter_actions[loop_cycle.m_start_action_idx];
					prev_action_end_state = false;
				}
			}
			if (letterAction.m_force_same_start_time)
			{
				flag = true;
			}
			if (letterAction.m_delay_progression.m_progression != 0)
			{
				flag = false;
			}
			letterAction.m_starting_in_sync = flag;
			if (letterAction.m_duration_progression.m_progression != 0)
			{
				flag = false;
			}
			letterAction.m_ending_in_sync = flag;
		}
	}
}
