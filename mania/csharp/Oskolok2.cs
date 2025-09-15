using UnityEngine;

public class Oskolok2 : bs
{
	private Vector3 old;

	public void Start()
	{
		old = base.pos;
		int num = 10;
		Object.Destroy(base.get_constantForce(), num);
		Object.Destroy(base.get_rigidbody(), num);
		Object.Destroy(this, num);
	}

	public void Update()
	{
		if (Physics.Linecast(old, base.pos, Layer.levelMask))
		{
			base.pos = old;
			Object.Destroy(base.get_constantForce());
			Object.Destroy(base.get_rigidbody());
		}
		old = base.pos;
	}
}
