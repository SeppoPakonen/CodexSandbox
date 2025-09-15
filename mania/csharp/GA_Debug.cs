using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GA_Debug
{
	public bool SubmitErrors;

	public int MaxErrorCount;

	public bool SubmitErrorStackTrace;

	public bool SubmitErrorSystemInfo;

	private int _errorCount;

	private bool _showLogOnGUI;

	public List<string> Messages;

	public void HandleLog(string logString, string stackTrace, LogType type)
	{
		if (_showLogOnGUI)
		{
			if (Messages == null)
			{
				Messages = new List<string>();
			}
			Messages.Add(logString);
		}
		if (!SubmitErrors || _errorCount >= MaxErrorCount || type == LogType.Log)
		{
			return;
		}
		_errorCount++;
		string eventName = "Exception";
		if (SubmitErrorStackTrace)
		{
			SubmitError(eventName, logString.Replace('"', '\'').Replace('\n', ' ').Replace('\r', ' ') + " " + stackTrace.Replace('"', '\'').Replace('\n', ' ').Replace('\r', ' '), type);
		}
		else
		{
			SubmitError(eventName, null, type);
		}
		if (!SubmitErrorSystemInfo)
		{
			return;
		}
		List<Hashtable> genericInfo = GA.API.GenericInfo.GetGenericInfo(logString);
		foreach (Hashtable item in genericInfo)
		{
			GA_Queue.AddItem(item, GA_Submit.CategoryType.GA_Log, stack: false);
		}
	}

	public void SubmitError(string eventName, string message, LogType type)
	{
		Vector3 vector = Vector3.zero;
		if (GA.SettingsGA.TrackTarget != null)
		{
			vector = GA.SettingsGA.TrackTarget.position;
		}
		GA_Error.SeverityType severity = GA_Error.SeverityType.info;
		switch (type)
		{
		case LogType.Assert:
			severity = GA_Error.SeverityType.info;
			break;
		case LogType.Error:
			severity = GA_Error.SeverityType.error;
			break;
		case LogType.Exception:
			severity = GA_Error.SeverityType.critical;
			break;
		case LogType.Log:
			severity = GA_Error.SeverityType.debug;
			break;
		case LogType.Warning:
			severity = GA_Error.SeverityType.warning;
			break;
		}
		GA.API.Error.NewErrorEvent(severity, message, vector.x, vector.y, vector.z);
	}
}
