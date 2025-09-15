using System.Collections;
using UnityEngine;

public class GA_Error
{
	public enum SeverityType
	{
		critical,
		error,
		warning,
		info,
		debug
	}

	public void NewEvent(SeverityType severity, string message, Vector3 trackPosition)
	{
		CreateNewEvent(severity, message, trackPosition.x, trackPosition.y, trackPosition.z, stack: false);
	}

	public void NewEvent(SeverityType severity, string message, float x, float y, float z)
	{
		CreateNewEvent(severity, message, x, y, z, stack: false);
	}

	public void NewEvent(SeverityType severity, string message)
	{
		CreateNewEvent(severity, message, null, null, null, stack: false);
	}

	public void NewErrorEvent(SeverityType severity, string message, float x, float y, float z)
	{
		CreateNewEvent(severity, message, x, y, z, stack: true);
	}

	private void CreateNewEvent(SeverityType severity, string message, float? x, float? y, float? z, bool stack)
	{
		Hashtable hashtable = new Hashtable();
		hashtable.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Severity], severity.ToString());
		hashtable.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Message], message);
		hashtable.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Level], (!GA.SettingsGA.CustomArea.Equals(string.Empty)) ? GA.SettingsGA.CustomArea : Application.loadedLevelName);
		Hashtable hashtable2 = hashtable;
		if (x.HasValue)
		{
			hashtable2.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.X], ((!x.HasValue) ? null : new float?(x.Value * GA.SettingsGA.HeatmapGridSize.x)).ToString());
		}
		if (y.HasValue)
		{
			hashtable2.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Y], ((!y.HasValue) ? null : new float?(y.Value * GA.SettingsGA.HeatmapGridSize.y)).ToString());
		}
		if (z.HasValue)
		{
			hashtable2.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Z], ((!z.HasValue) ? null : new float?(z.Value * GA.SettingsGA.HeatmapGridSize.z)).ToString());
		}
		GA_Queue.AddItem(hashtable2, GA_Submit.CategoryType.GA_Error, stack);
	}
}
