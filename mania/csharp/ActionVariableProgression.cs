using System;

[Serializable]
public class ActionVariableProgression
{
	public ValueProgression m_progression;

	public EasingEquation m_ease_type;

	public bool m_is_offset_from_last;

	public bool m_to_to_bool;

	public bool m_unique_randoms;

	public AnimatePerOptions m_animate_per;

	public bool m_override_animate_per_option;

	public bool UniqueRandom => m_progression == ValueProgression.Random && m_unique_randoms;

	public int GetProgressionIndex(AnimationProgressionVariables progression_variables, AnimatePerOptions animate_per_default)
	{
		return progression_variables.GetValue((!m_override_animate_per_option) ? animate_per_default : m_animate_per);
	}
}
