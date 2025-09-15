using UnityEngine;

public class Coin : bs
{
	private bool gameLoaded;

	public Animation componentInChildren;

	public void Update()
	{
		if (gameLoaded || (!bs._Loader.levelEditor && !(bs._Game == null) && (bool)bs._Player))
		{
			gameLoaded = true;
			if ((bs._Player.pos - base.pos).magnitude < 6f)
			{
				Player player = bs._Player;
				player.score = (float)player.score + 100f;
				bs._Awards.coinsCollected.Add();
				base.gameObject.SetActive(value: false);
				((Component)bs._Player).get_audio().PlayOneShot(bs.res.coinSound);
			}
		}
	}

	public void Start()
	{
		if (!(componentInChildren != null))
		{
			return;
		}
		foreach (AnimationState componentInChild in componentInChildren)
		{
			componentInChild.normalizedTime = Random.value;
		}
	}

	public void Reset()
	{
		base.gameObject.SetActive(value: true);
	}
}
