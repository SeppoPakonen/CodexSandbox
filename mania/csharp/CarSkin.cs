using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

[Serializable]
public class CarSkin
{
	public string prefabName;

	public AudioClip[] horn = new AudioClip[0];

	private static AnimationCurve speedCurve = new AnimationCurve(new Keyframe(0f, 1f), new Keyframe(4f, 1.3f));

	private static AnimationCurve antiFlyCurve = new AnimationCurve(new Keyframe(0f, 0f), new Keyframe(4f, 1f));

	private static AnimationCurve distanceToGround = new AnimationCurve(new Keyframe(0f, 2f), new Keyframe(4f, 4f));

	private static AnimationCurve rotCurve = new AnimationCurve(new Keyframe(0f, 1.3f), new Keyframe(4f, 2.5f));

	private static AnimationCurve rotCurveMouse = new AnimationCurve(new Keyframe(0f, 0.5f), new Keyframe(1f, 4f));

	private static AnimationCurve driftCurve = new AnimationCurve(new Keyframe(0f, 1f), new Keyframe(4f, 0.5f));

	private static AnimationCurve driftBrakeCurve = new AnimationCurve(new Keyframe(0f, 1f), new Keyframe(4f, 0f));

	public int TopSpeed = 4;

	public int Speed = 2;

	public int Rotation = 2;

	public int Drift;

	public int mass;

	public bool flyRotation;

	public int friendsNeeded;

	public int medalsNeeded;

	public int repNeeded;

	public bool allMedals;

	public AudioClip engineSound;

	public bool hidden;

	public bool canPickColor;

	public bool unlocked => repUnlocked && medalsUnlocked;

	public bool medalsUnlocked => medalsNeeded <= (int)bs._Loader.medals;

	public bool repUnlocked
	{
		get
		{
			return repNeeded == 0 || bs._Loader.disableRep || Base.PlayerPrefsGetBool(prefix + "repUnlocked");
		}
		set
		{
			Base.PlayerPrefsSetBool(prefix + "repUnlocked", value);
		}
	}

	public bool paintUnlocked
	{
		get
		{
			return Base.PlayerPrefsGetBool(prefix + "paintUnlocked");
		}
		set
		{
			Base.PlayerPrefsSetBool(prefix + "paintUnlocked", value);
		}
	}

	public GameObject model
	{
		get
		{
			UnityEngine.Object @object = bs.LoadRes("Cars/" + prefabName);
			if (@object == null)
			{
				@object = bs.LoadRes("Cars/carF1");
			}
			return (GameObject)@object;
		}
	}

	public Color color
	{
		get
		{
			return new Color(Base.PlayerPrefsGetFloat(prefix + "r", 0f), Base.PlayerPrefsGetFloat(prefix + "g", 0f), Base.PlayerPrefsGetFloat(prefix + "b", 0f));
		}
		set
		{
			Base.PlayerPrefsSetFloat(prefix + "r", value.r);
			Base.PlayerPrefsSetFloat(prefix + "g", value.g);
			Base.PlayerPrefsSetFloat(prefix + "b", value.b);
			Base.PlayerPrefsSetBool(prefix + "color", value: true);
		}
	}

	public bool haveColor => Base.PlayerPrefsGetBool(prefix + "color");

	private string prefix => bs._Loader.playerName + prefabName;

	public float getDistanceToGround()
	{
		return distanceToGround.Evaluate(mass);
	}

	public float getAntiFly()
	{
		return antiFlyCurve.Evaluate(mass);
	}

	public float getMass()
	{
		return 0f;
	}

	public float getSpeed()
	{
		return speedCurve.Evaluate(Speed);
	}

	public float getBrakeDrift()
	{
		return driftBrakeCurve.Evaluate(Drift);
	}

	public float getDrift()
	{
		return driftCurve.Evaluate(Drift);
	}

	public float getRotationMouse()
	{
		return rotCurveMouse.Evaluate(Rotation);
	}

	public float getRotation()
	{
		return rotCurve.Evaluate(Rotation);
	}

	public void SetColor(Renderer[] renderers, Color? c = null)
	{
		if (!canPickColor)
		{
			return;
		}
		foreach (Material item in ((IEnumerable<Renderer>)renderers).SelectMany<Renderer, Material>((Func<Renderer, IEnumerable<Material>>)((Renderer a) => a.materials)))
		{
			if (item.shader.name == "Car/CarPain2 Bump" || item.shader.name == "Car/CarPain2")
			{
				item.SetFloat("_CandyScale", 0.7f);
				Color value = (!c.HasValue) ? color : c.Value;
				item.SetColor("_AmbientColor2", value);
			}
		}
	}
}
