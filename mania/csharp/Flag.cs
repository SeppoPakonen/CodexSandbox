using UnityEngine;

public class Flag : bsNetwork
{
	internal Vector3 StartPos;

	public Team team;

	public Player pl;

	public Flag otherFlag;

	public float flagUsedTime;

	private bool flagHome;

	public GUITexture flagIcon;

	private bool told;

	private bool captured => pl != null;

	public void Start()
	{
		base.transform.position = ((team != 0) ? bs._Game.blueFlagPos : bs._Game.redFlagPos).position + bs._Game.startPosOffset;
		flagIcon.transform.parent = null;
		StartPos = base.transform.position;
		bs._Game.flags.Add(this);
	}

	public void Update()
	{
		if (PhotonNetwork.isMasterClient)
		{
			int num = 10;
			flagHome = ((StartPos - base.pos).magnitude < (float)num);
			if (captured || flagHome)
			{
				flagUsedTime = Time.time;
			}
			if (Time.time - flagUsedTime > 15f)
			{
				flagUsedTime = 99999f;
				CallRPC(ResetPos, p: true);
			}
			if (pl != null && pl.dead)
			{
				CallRPC(DropFlag, pl.FlagPlaceHolder.position);
			}
			if (pl != null && pl.team == team)
			{
				CallRPC(ResetPos, p: true);
			}
			foreach (Player listOfPlayer in bs._Game.listOfPlayers)
			{
				if (((listOfPlayer.pos - base.pos).magnitude < (float)num || KeyDebug(KeyCode.X)) && !captured && !listOfPlayer.dead && Time.time - listOfPlayer.resetTime > 3f)
				{
					if (listOfPlayer.team == team && !flagHome)
					{
						CallRPC(ResetPos, p: true);
					}
					if (listOfPlayer.team != team)
					{
						CallRPC(SetOwner, listOfPlayer.playerId);
					}
				}
			}
			if ((base.pos - otherFlag.StartPos).magnitude < (float)num && !otherFlag.captured && otherFlag.flagHome && pl != null && !pl.dead && Time.time - pl.resetTime > 3f)
			{
				MonoBehaviour.print("score");
				Player player = pl;
				pl = null;
				int count = bs._Game.listOfPlayers.Count;
				player.CallRPC(player.SetScore2, (float)player.score + (float)((count > 7) ? 60 : ((count <= 3) ? 10 : 30)));
				bs._Game.CallRPC(bs._Game.FlagCaptured, player.playerId);
			}
		}
		if (pl != null)
		{
			base.transform.position = pl.FlagPlaceHolder.position;
			base.transform.rotation = pl.FlagPlaceHolder.rotation;
		}
		flagIcon.transform.position = bs._Player.camera.WorldToViewportPoint(base.pos + Vector3.up) + Vector3.up * 0.05f - Vector3.forward * 10f;
		GUITexture gUITexture = flagIcon;
		Vector3 position = flagIcon.transform.position;
		gUITexture.enabled = (position.z > 0f);
		flagIcon.color = ((team != Team.Blue) ? Color.red : Color.blue) - new Color(0f, 0f, 0f, Mathf.Min(0.9f, (bs._Player.pos - base.pos).magnitude / 400f));
	}

	[RPC]
	public void ResetPos(bool play)
	{
		told = false;
		MonoBehaviour.print(string.Concat("ResetPos", base.pos, "  ", StartPos));
		if (play)
		{
			bs.PlayOneShotGui((team != Team.Blue) ? bs.res.redFlagReturn : bs.res.blueFlagReturn);
		}
		pl = null;
		base.pos = StartPos;
		rot = Quaternion.identity;
	}

	[RPC]
	public void DropFlag(Vector3 pos)
	{
		MonoBehaviour.print("Drop Flag");
		base.transform.position = pos;
		pl = null;
	}

	public override void OnPlConnected()
	{
		if (pl != null)
		{
			CallRPC(SetOwner, pl.playerId);
		}
		base.OnPlConnected();
	}

	[RPC]
	public void SetOwner(int id)
	{
		MonoBehaviour.print("Set FlagOwner " + id + " ph" + bs._Game.photonPlayers.Count);
		pl = bs._Game.photonPlayers[id];
		if (!told)
		{
			bs.PlayOneShotGui((pl == bs._Player) ? bs.res.youHaveFlag : ((!pl.sameTeam) ? bs.res.enemyHaveYourFlag : bs.res.yourTeamHaveFlag));
		}
		told = true;
	}
}
