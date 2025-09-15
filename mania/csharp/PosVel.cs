using System;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
public class PosVel
{
	public float time;

	public int frameCount;

	public float meters;

	public float velSmooth;

	public bool movingBack;

	public float nitro;

	public Vector3 pos;

	public Quaternion rot;

	public bool left;

	public bool up;

	public float groundTime;

	public Vector3 vel;

	public Vector3 angVel;

	public bool engineOff;

	internal Vector3 camPos;

	internal Quaternion camRot;

	public bool right;

	public float mouserot;

	public float skid;

	public List<Transform> checkPointsPass;

	public int lap;

	public int score;
}
