using System.Collections;
using UnityEngine;

public class GA_Business
{
	public void NewEvent(string eventName, string currency, int amount, Vector3 trackPosition)
	{
		CreateNewEvent(eventName, currency, amount, trackPosition.x, trackPosition.y, trackPosition.z);
	}

	public void NewEvent(string eventName, string currency, int amount, float x, float y, float z)
	{
		CreateNewEvent(eventName, currency, amount, x, y, z);
	}

	public void NewEvent(string eventName, string currency, int amount)
	{
		CreateNewEvent(eventName, currency, amount, null, null, null);
	}

	private void CreateNewEvent(string eventName, string currency, int amount, float? x, float? y, float? z)
	{
		Hashtable hashtable = new Hashtable();
		hashtable.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.EventID], eventName);
		hashtable.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Currency], currency);
		hashtable.Add(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Amount], amount.ToString());
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
		GA_Queue.AddItem(hashtable2, GA_Submit.CategoryType.GA_Purchase, stack: false);
	}
}
