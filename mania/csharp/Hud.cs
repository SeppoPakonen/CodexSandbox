using UnityEngine;

public class Hud : GuiClasses
{
	public GameObject root;

	public TextMesh time;

	public TextMesh distance;

	public TextMesh speed;

	public TextMesh checkPoint;

	public TextMesh checkPointRed;

	public TextMesh backup;

	public Renderer backupIcon;

	public Player pl;

	public Transform damage;

	public Animation damageAnim;

	public TextMesh zombieScore;

	public override void Awake()
	{
		base.Awake();
	}

	public void Start()
	{
		TextMesh textMesh = checkPointRed;
		string empty = string.Empty;
		checkPoint.text = empty;
		textMesh.text = empty;
		if (bs._Loader.dm)
		{
			root.SetActive(value: false);
		}
	}

	public void Update()
	{
		Renderer renderer = ((Component)backupIcon).get_renderer();
		bool enabled = !bs.lowestQuality;
		((Component)backup).get_renderer().enabled = enabled;
		renderer.enabled = enabled;
		if (!(pl == null) && !pl.finnished)
		{
			if (!bs._Loader.dm)
			{
				time.text = bs.TimeToStr((!bs._Game.started) ? 0f : bs._Game.timeElapsed2);
			}
			speed.text = ((int)(pl.rigidbody.velocity.magnitude * 3.6f)).ToString();
			distance.text = (int)pl.totalMeters + "m";
		}
	}

	public void PlayScore(string s, Color c)
	{
		zombieScore.text = s;
		((Component)zombieScore).get_renderer().material.color = c;
		((Component)zombieScore).get_animation().Rewind();
		((Component)zombieScore).get_animation().Play();
	}
}
