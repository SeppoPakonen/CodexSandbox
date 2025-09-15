using System;

[Serializable]
public class VarCheck
{
	private int check;

	public int m_Value;

	public int Value
	{
		get
		{
			return m_Value;
		}
		set
		{
			check = (value ^ 0x978);
			m_Value = value;
		}
	}

	public bool IsMh()
	{
		return check != 0 && m_Value != (check ^ 0x978);
	}
}
