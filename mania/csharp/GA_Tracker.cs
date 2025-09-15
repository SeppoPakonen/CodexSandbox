using System;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class GA_Tracker : MonoBehaviour
{
	public enum GAEventType
	{
		BreadCrumb,
		Start,
		OnDestroy,
		OnMouseDown,
		OnLevelWasLoaded,
		OnTriggerEnter,
		OnCollisionEnter,
		OnControllerColliderHit
	}

	public static Dictionary<GAEventType, string> EventTooltips = new Dictionary<GAEventType, string>
	{
		{
			GAEventType.BreadCrumb,
			"Send an event every time interval. Good for generating heatmaps of player position/movement in your levels."
		},
		{
			GAEventType.Start,
			"Send an event when the Start method is run. Use this for tracking spawning of the object"
		},
		{
			GAEventType.OnDestroy,
			"Send an event when the OnDestroy method is run. Use this for tracking \"death\" of the object."
		},
		{
			GAEventType.OnMouseDown,
			"Send an event when the OnMouseDown method is run. Use this for tracking when the player performs a click/touch on the object."
		},
		{
			GAEventType.OnLevelWasLoaded,
			"Send an event when the OnLevelWasLoaded method is run. Use this for tracking when a new level is loaded."
		},
		{
			GAEventType.OnTriggerEnter,
			"Send an event when the OnTriggerEnter method is run. Use this for tracking when something (f.x. the player) enters a trigger area."
		},
		{
			GAEventType.OnCollisionEnter,
			"Send an event when the OnCollisionEnter method is run. Use this for tracking when objects collide."
		},
		{
			GAEventType.OnControllerColliderHit,
			"Send an event when the OnControllerColliderHit method is run. Use this for tracking when a controller hits a collider while performing a Move."
		}
	};

	[SerializeField]
	public List<GAEventType> TrackedEvents = new List<GAEventType>();

	public bool TrackedEventsFoldOut;

	public bool TrackTarget;

	public bool ShowGizmo = true;

	public float BreadCrumbTrackInterval = 1f;

	private static bool _trackTargetAlreadySet;

	private float _lastBreadCrumbTrackTime;

	private void Start()
	{
		if (!Application.isPlaying)
		{
			return;
		}
		if (TrackedEvents.Contains(GAEventType.Start))
		{
			GA.API.Design.NewEvent("Start:" + base.gameObject.name, base.transform.position);
		}
		if (TrackTarget)
		{
			GA.SettingsGA.TrackTarget = base.transform;
			if (_trackTargetAlreadySet)
			{
				GA.LogWarning("You should only set the Track Target of GA_Tracker once per scene");
			}
			_trackTargetAlreadySet = true;
		}
	}

	private void Update()
	{
		if (Application.isPlaying && TrackedEvents.Contains(GAEventType.BreadCrumb) && Time.time > _lastBreadCrumbTrackTime + BreadCrumbTrackInterval)
		{
			_lastBreadCrumbTrackTime = Time.time;
			GA.API.Design.NewEvent("BreadCrumb:" + base.gameObject.name, base.transform.position);
		}
	}

	private void OnDestroy()
	{
		if (Application.isPlaying && TrackedEvents.Contains(GAEventType.OnDestroy))
		{
			GA.API.Design.NewEvent("OnDestroy:" + base.gameObject.name, base.transform.position);
		}
	}

	private void OnMouseDown()
	{
		if (Application.isPlaying && TrackedEvents.Contains(GAEventType.OnMouseDown))
		{
			GA.API.Design.NewEvent("OnMouseDown:" + base.gameObject.name, base.transform.position);
		}
	}

	public void OnLevelWasLoaded()
	{
		if (Application.isPlaying && TrackedEvents.Contains(GAEventType.OnLevelWasLoaded))
		{
			GA.API.Design.NewEvent("OnLevelWasLoaded:" + base.gameObject.name, base.transform.position);
		}
	}

	public void OnTriggerEnter()
	{
		if (Application.isPlaying && TrackedEvents.Contains(GAEventType.OnTriggerEnter))
		{
			GA.API.Design.NewEvent("OnTriggerEnter:" + base.gameObject.name, base.transform.position);
		}
	}

	public void OnCollisionEnter()
	{
		if (Application.isPlaying && TrackedEvents.Contains(GAEventType.OnCollisionEnter))
		{
			GA.API.Design.NewEvent("OnCollisionEnter:" + base.gameObject.name, base.transform.position);
		}
	}

	public void OnControllerColliderHit()
	{
		if (Application.isPlaying && TrackedEvents.Contains(GAEventType.OnControllerColliderHit))
		{
			GA.API.Design.NewEvent("OnControllerColliderHit:" + base.gameObject.name, base.transform.position);
		}
	}

	public Array GetEventValues()
	{
		return Enum.GetValues(typeof(GAEventType));
	}

	private void OnDrawGizmos()
	{
		if (ShowGizmo)
		{
			Gizmos.DrawIcon(base.transform.position, "gaLogo", allowScaling: true);
		}
	}
}
