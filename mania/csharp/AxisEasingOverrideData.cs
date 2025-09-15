using System;

[Serializable]
public class AxisEasingOverrideData
{
	public bool m_override_default;

	public EasingEquation m_x_ease;

	public EasingEquation m_y_ease;

	public EasingEquation m_z_ease;

	public AxisEasingOverrideData Clone()
	{
		AxisEasingOverrideData axisEasingOverrideData = new AxisEasingOverrideData();
		axisEasingOverrideData.m_override_default = m_override_default;
		axisEasingOverrideData.m_x_ease = m_x_ease;
		axisEasingOverrideData.m_y_ease = m_y_ease;
		axisEasingOverrideData.m_z_ease = m_z_ease;
		return axisEasingOverrideData;
	}
}
