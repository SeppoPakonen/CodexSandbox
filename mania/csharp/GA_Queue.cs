using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class GA_Queue
{
	public delegate void EventSuccess();

	public static int MAXQUEUESIZE = 800;

	public static bool QUITONSUBMIT = false;

	private static List<GA_Submit.Item> _queue = new List<GA_Submit.Item>();

	private static List<GA_Submit.Item> _tempQueue = new List<GA_Submit.Item>();

	private static List<GA_Submit.Item> _errorQueue = new List<GA_Submit.Item>();

	private static bool _submittingData = false;

	private static int _submitCount = 0;

	private static bool _endsubmit = false;

	public static event EventSuccess OnSuccess;

	public static void AddItem(Hashtable parameters, GA_Submit.CategoryType type, bool stack)
	{
		if (_endsubmit || (Application.isEditor && !GA.SettingsGA.RunInEditorPlayMode))
		{
			return;
		}
		GA_Submit.Item item = default(GA_Submit.Item);
		item.Type = type;
		item.Parameters = parameters;
		item.AddTime = Time.time;
		GA_Submit.Item item2 = item;
		if (_submittingData)
		{
			if (stack && type == GA_Submit.CategoryType.GA_Log)
			{
				StackQueue(_tempQueue, item2);
			}
			else
			{
				_tempQueue.Add(item2);
			}
		}
		else if (stack && type == GA_Submit.CategoryType.GA_Log)
		{
			StackQueue(_queue, item2);
		}
		else
		{
			_queue.Add(item2);
		}
	}

	public static IEnumerator SubmitQueue()
	{
		while (!_endsubmit)
		{
			while (GA.SettingsGA.CustomUserID && GA.API.GenericInfo.UserID == string.Empty)
			{
				GA.LogWarning("GameAnalytics: User ID not set. No data will be sent until Custom User ID is set.");
				yield return new WaitForSeconds(10f);
			}
			while (_submittingData)
			{
				yield return new WaitForSeconds(0.5f);
			}
			if (GA.SettingsGA.ArchiveData && GA.SettingsGA.InternetConnectivity)
			{
				List<GA_Submit.Item> archivedItems = GA.API.Archive.GetArchivedData();
				if (archivedItems != null && archivedItems.Count > 0)
				{
					foreach (GA_Submit.Item item in archivedItems)
					{
						AddItem(item.Parameters, item.Type, stack: false);
					}
					if (GA.SettingsGA.DebugMode)
					{
						GA.Log("GA: Network connection detected. Adding archived data to next submit queue.");
					}
				}
			}
			ForceSubmit();
			yield return new WaitForSeconds(GA.SettingsGA.SubmitInterval);
		}
	}

	public static void ForceSubmit()
	{
		GA_SpecialEvents.SubmitAverageFPS();
		if (_queue.Count > 0 && !_submittingData && !_endsubmit)
		{
			_submittingData = true;
			GA.Log("GameAnalytics: Queue submit started");
			GA.API.Submit.SubmitQueue(_queue, Submitted, SubmitError, gaTracking: false, string.Empty, string.Empty);
		}
	}

	public static void EndSubmit()
	{
		GA.Log("GA: Ending all data submission after this timer interval");
		_endsubmit = true;
	}

	private static void StackQueue(List<GA_Submit.Item> queue, GA_Submit.Item item)
	{
		bool flag = false;
		for (int i = 0; i < queue.Count; i++)
		{
			if (flag)
			{
				continue;
			}
			GA_Submit.Item item2 = queue[i];
			if (item2.Type != GA_Submit.CategoryType.GA_Log || item.Type != GA_Submit.CategoryType.GA_Log)
			{
				continue;
			}
			GA_Submit.Item item3 = queue[i];
			if (!item3.Parameters.ContainsKey(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.EventID]) || !item.Parameters.ContainsKey(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.EventID]))
			{
				continue;
			}
			GA_Submit.Item item4 = queue[i];
			if (!item4.Parameters[GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.EventID]].Equals(item.Parameters[GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.EventID]]))
			{
				continue;
			}
			GA_Submit.Item item5 = queue[i];
			if (item5.Parameters.ContainsKey(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Message]) && item.Parameters.ContainsKey(GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Message]))
			{
				GA_Submit.Item item6 = queue[i];
				if (item6.Parameters[GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Message]].Equals(item.Parameters[GA_ServerFieldTypes.Fields[GA_ServerFieldTypes.FieldType.Message]]))
				{
					flag = true;
					int index = i;
					GA_Submit.Item value = default(GA_Submit.Item);
					GA_Submit.Item item7 = queue[i];
					value.AddTime = item7.AddTime;
					GA_Submit.Item item8 = queue[i];
					value.Parameters = item8.Parameters;
					GA_Submit.Item item9 = queue[i];
					value.Type = item9.Type;
					GA_Submit.Item item10 = queue[i];
					value.Count = Mathf.Max(2, item10.Count + 1);
					queue[index] = value;
				}
			}
		}
		if (!flag)
		{
			queue.Add(item);
		}
	}

	private static void Submitted(List<GA_Submit.Item> items, bool success)
	{
		_submitCount += items.Count;
		if (success)
		{
			if (GA_Queue.OnSuccess != null)
			{
				GA_Queue.OnSuccess();
			}
			GA.SettingsGA.TotalMessagesSubmitted += items.Count;
			foreach (GA_Submit.Item item in items)
			{
				switch (item.Type)
				{
				case GA_Submit.CategoryType.GA_Event:
					GA.SettingsGA.DesignMessagesSubmitted++;
					break;
				case GA_Submit.CategoryType.GA_Log:
					GA.SettingsGA.QualityMessagesSubmitted++;
					break;
				case GA_Submit.CategoryType.GA_Error:
					GA.SettingsGA.ErrorMessagesSubmitted++;
					break;
				case GA_Submit.CategoryType.GA_Purchase:
					GA.SettingsGA.BusinessMessagesSubmitted++;
					break;
				case GA_Submit.CategoryType.GA_User:
					GA.SettingsGA.UserMessagesSubmitted++;
					break;
				}
			}
		}
		if (_submitCount >= _queue.Count)
		{
			if (GA.SettingsGA.DebugMode)
			{
				GA.Log("GA: Queue submit over");
			}
			if (QUITONSUBMIT)
			{
				Application.Quit();
			}
			_queue = _tempQueue;
			_tempQueue = new List<GA_Submit.Item>();
			if (success)
			{
				_queue.AddRange(_errorQueue);
				_errorQueue = new List<GA_Submit.Item>();
			}
			_submitCount = 0;
			_submittingData = false;
		}
	}

	private static void SubmitError(List<GA_Submit.Item> items)
	{
		if (items == null)
		{
			GA.Log("GA: Ending all data submission after this timer interval");
			_endsubmit = true;
			return;
		}
		GA.SettingsGA.TotalMessagesFailed += items.Count;
		foreach (GA_Submit.Item item in items)
		{
			switch (item.Type)
			{
			case GA_Submit.CategoryType.GA_Event:
				GA.SettingsGA.DesignMessagesFailed++;
				break;
			case GA_Submit.CategoryType.GA_Log:
				GA.SettingsGA.QualityMessagesFailed++;
				break;
			case GA_Submit.CategoryType.GA_Error:
				GA.SettingsGA.ErrorMessagesFailed++;
				break;
			case GA_Submit.CategoryType.GA_Purchase:
				GA.SettingsGA.BusinessMessagesFailed++;
				break;
			case GA_Submit.CategoryType.GA_User:
				GA.SettingsGA.UserMessagesFailed++;
				break;
			}
		}
		GA.RunCoroutine(GA.SettingsGA.CheckInternetConnectivity(startQueue: false));
		_errorQueue.AddRange(items);
		if (_errorQueue.Count > MAXQUEUESIZE)
		{
			_errorQueue.Sort(new ItemComparer());
			_errorQueue.RemoveRange(MAXQUEUESIZE, _errorQueue.Count - MAXQUEUESIZE);
		}
		Submitted(items, success: false);
	}
}
