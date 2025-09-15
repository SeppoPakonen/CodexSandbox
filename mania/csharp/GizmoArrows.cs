using UnityEngine;

public class GizmoArrows : bs
{
	public Vector3 direction;

	public void Start()
	{
		base.get_renderer().material.color = ((direction.x > 0f) ? Color.red : ((!(direction.y > 0f)) ? Color.blue : Color.green));
	}

	public void OnMouseDrag()
	{
	}
}
