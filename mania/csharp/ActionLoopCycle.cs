using System;

[Serializable]
public class ActionLoopCycle
{
	public int m_start_action_idx;

	public int m_end_action_idx;

	public int m_number_of_loops;

	public LOOP_TYPE m_loop_type;

	public int SpanWidth => m_end_action_idx - m_start_action_idx;

	public ActionLoopCycle(int start, int end)
	{
		m_start_action_idx = start;
		m_end_action_idx = end;
	}

	public ActionLoopCycle(int start, int end, int num_loops, LOOP_TYPE loop_type)
	{
		m_start_action_idx = start;
		m_end_action_idx = end;
		m_number_of_loops = num_loops;
		m_loop_type = loop_type;
	}

	public ActionLoopCycle Clone()
	{
		ActionLoopCycle actionLoopCycle = new ActionLoopCycle(m_start_action_idx, m_end_action_idx);
		actionLoopCycle.m_number_of_loops = m_number_of_loops;
		actionLoopCycle.m_loop_type = m_loop_type;
		return actionLoopCycle;
	}
}
