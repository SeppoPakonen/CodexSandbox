using System;
using UnityEngine;

[Serializable]
public class ActionPositionVector3Progression : ActionVector3Progression
{
	public bool m_force_position_override;

	public override int NumEditorLines
	{
		get
		{
			if (m_progression == ValueProgression.Constant)
			{
				return 4;
			}
			if (m_progression == ValueProgression.Random)
			{
				return 7;
			}
			return (!m_to_to_bool) ? 6 : 8;
		}
	}

	public ActionPositionVector3Progression(Vector3 start_vec)
	{
		m_from = start_vec;
		m_to = start_vec;
		m_to_to = start_vec;
	}

	public ActionPositionVector3Progression CloneThis()
	{
		ActionPositionVector3Progression actionPositionVector3Progression = new ActionPositionVector3Progression(Vector3.zero);
		actionPositionVector3Progression.m_progression = m_progression;
		actionPositionVector3Progression.m_ease_type = m_ease_type;
		actionPositionVector3Progression.m_from = m_from;
		actionPositionVector3Progression.m_to = m_to;
		actionPositionVector3Progression.m_to_to = m_to_to;
		actionPositionVector3Progression.m_to_to_bool = m_to_to_bool;
		actionPositionVector3Progression.m_is_offset_from_last = m_is_offset_from_last;
		actionPositionVector3Progression.m_unique_randoms = m_unique_randoms;
		actionPositionVector3Progression.m_force_position_override = m_force_position_override;
		actionPositionVector3Progression.m_override_animate_per_option = m_override_animate_per_option;
		actionPositionVector3Progression.m_animate_per = m_animate_per;
		return actionPositionVector3Progression;
	}
}
