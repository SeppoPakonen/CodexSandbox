using System.Collections;
using UnityEngine;

public class GA_Design
{
	public void NewEvent(string eventName, float? eventValue, Vector3 trackPosition)
	{
		CreateNewEvent(eventName, eventValue, trackPosition.x, trackPosition.y, trackPosition.z);
	}

	public void NewEvent(string eventName, Vector3 trackPosition)
	{
		CreateNewEvent(eventName, null, trackPosition.x, trackPosition.y, trackPosition.z);
	}

	public void NewEvent(string eventName, float? eventValue, float x, float y, float z)
	{
		CreateNewEvent(eventName, eventValue, x, y, z);
	}

	public void NewEvent(string eventName, float x, float y, float z)
	{
		CreateNewEvent(eventName, null, x, y, z);
	}

	public void NewEvent(string eventName, float eventValue)
	{
		CreateNewEvent(eventName, eventValue, null, null, null);
	}

	public void NewEvent(string eventName)
	{
		CreateNewEvent(eventName, null, null, null, null);
	}

	private void CreateNewEvent(string eventName, float? eventValue, float? x, float? y, float? z)
	{
		Hashtable hashtable = new Hashtable();
		hashtable.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.EventID], eventName);
		hashtable.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Level], (!GA.SettingsGA.CustomArea.Equals(string.Empty)) ? GA.SettingsGA.CustomArea : Application.loadedLevelName);
		Hashtable hashtable2 = hashtable;
		if (eventValue.HasValue)
		{
			hashtable2.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Value], eventValue.ToString());
		}
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
		GA_Queue.AddItem(hashtable2, GA_Submit.CategoryType.GA_Event, stack: false);
	}
}
