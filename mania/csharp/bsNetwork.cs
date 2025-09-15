using System;
using System.Linq;
using System.Reflection;
using UnityEngine;

public class bsNetwork : GuiClasses
{
	private PhotonPlayer ToPhotonPlayer;

	public void CallRPC(Action<PhotonMessageInfo> n)
	{
		CallRPC(n.Method.Name);
	}

	public void CallRPC<T, T2>(Action<T, T2, PhotonMessageInfo> n, T p, T2 p2)
	{
		CallRPC(((Delegate)(object)n).Method.Name, new object[2]
		{
			p,
			p2
		});
	}

	public void CallRPC(Action n)
	{
		CallRPC(((Delegate)(object)n).Method.Name);
	}

	public void CallRPC<T>(Action<T> n, T p)
	{
		CallRPC(n.Method.Name, p);
	}

	public void CallRPC<T, T2>(Action<T, T2> n, T p, T2 p2)
	{
		CallRPC(((Delegate)(object)n).Method.Name, new object[2]
		{
			p,
			p2
		});
	}

	public void CallRPC<T, T2, T3>(Action<T, T2, T3> n, T p, T2 p2, T3 p3)
	{
		CallRPC(((Delegate)(object)n).Method.Name, new object[3]
		{
			p,
			p2,
			p3
		});
	}

	public void CallRPC<T, T2, T3, T4>(Action<T, T2, T3, T4> n, T p, T2 p2, T3 p3, T4 p4)
	{
		CallRPC(((Delegate)(object)n).Method.Name, new object[4]
		{
			p,
			p2,
			p3,
			p4
		});
	}

	public void CallRPC<T, T2, T3, T4, T5>(Action<T, T2, T3, T4, T5> n, T p, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		CallRPC(n.Method.Name, p, p2, p3, p4, p5);
	}

	public void CallRPC<T, T2, T3, T4, T5, T6>(Action<T, T2, T3, T4, T5, T6> n, T p, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		CallRPC(n.Method.Name, p, p2, p3, p4, p5, p6);
	}

	public void CallRPCTo(Action n)
	{
		CallRPCTo(((Delegate)(object)n).Method.Name);
	}

	public void CallRPCTo<T>(Action<T> n, T p)
	{
		CallRPCTo(n.Method.Name, p);
	}

	public void CallRPCTo<T, T2>(Action<T, T2> n, T p, T2 p2)
	{
		CallRPCTo(((Delegate)(object)n).Method.Name, new object[2]
		{
			p,
			p2
		});
	}

	public void CallRPCTo<T, T2, T3>(Action<T, T2, T3> n, T p, T2 p2, T3 p3)
	{
		CallRPCTo(((Delegate)(object)n).Method.Name, new object[3]
		{
			p,
			p2,
			p3
		});
	}

	public void CallRPCTo<T, T2, T3, T4>(Action<T, T2, T3, T4> n, T p, T2 p2, T3 p3, T4 p4)
	{
		CallRPCTo(((Delegate)(object)n).Method.Name, new object[4]
		{
			p,
			p2,
			p3,
			p4
		});
	}

	public void CallRPCTo<T, T2, T3, T4, T5>(Action<T, T2, T3, T4, T5> n, T p, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		CallRPCTo(n.Method.Name, p, p2, p3, p4, p5);
	}

	public void CallRPCTo<T, T2, T3, T4, T5, T6>(Action<T, T2, T3, T4, T5, T6> n, T p, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		CallRPCTo(n.Method.Name, p, p2, p3, p4, p5, p6);
	}

	private void CallRPC(string mn, params object[] p)
	{
		try
		{
			if (!bs.online)
			{
				MethodInfo method = GetType().GetMethod(mn, BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
				if (p.Length < method.GetParameters().Length)
				{
					p = p.Concat(new PhotonMessageInfo[1]
					{
						new PhotonMessageInfo()
					}).ToArray();
				}
				method.Invoke(this, p);
			}
			else if (ToPhotonPlayer != null)
			{
				MonoBehaviour.print(mn + " Sending to " + ToPhotonPlayer.ID);
				base.photonView.RPC(mn, ToPhotonPlayer, p);
			}
			else
			{
				base.photonView.RPC(mn, PhotonTargets.All, p);
			}
		}
		catch (TargetParameterCountException)
		{
		}
		catch (Exception message)
		{
			Debug.LogError(message);
		}
	}

	public void CallRPCTo(string mn, params object[] p)
	{
		base.photonView.RPC(mn, ToPhotonPlayer, p);
	}

	public void OnPhotonPlayerConnected(PhotonPlayer player)
	{
		ToPhotonPlayer = player;
		if (PhotonNetwork.isMasterClient)
		{
			try
			{
				OnPlConnected();
			}
			catch (Exception message)
			{
				Debug.LogError(message);
			}
		}
		ToPhotonPlayer = null;
	}

	public virtual void OnPlConnected()
	{
	}
}
