using UnityEngine;

public class SamsungAdListener : MonoBehaviour
{
	public bool debug = true;

	private static bool _instanceFound;

	private void Awake()
	{
		if (_instanceFound)
		{
			Object.Destroy(base.gameObject);
			return;
		}
		_instanceFound = true;
		Object.DontDestroyOnLoad(this);
		SamsungAd.Instance();
	}

	private void OnEnable()
	{
		SamsungAdAgent.OnReceiveAd += OnReceiveAd;
		SamsungAdAgent.OnFailedToReceiveAd += OnFailedToReceiveAd;
		SamsungAdAgent.OnPresentScreen += OnPresentScreen;
		SamsungAdAgent.OnDismissScreen += OnDismissScreen;
		SamsungAdAgent.OnLeaveApplication += OnLeaveApplication;
		SamsungAdAgent.OnReceiveAdInterstitial += OnReceiveAdInterstitial;
		SamsungAdAgent.OnFailedToReceiveAdInterstitial += OnFailedToReceiveAdInterstitial;
		SamsungAdAgent.OnPresentScreenInterstitial += OnPresentScreenInterstitial;
		SamsungAdAgent.OnDismissScreenInterstitial += OnDismissScreenInterstitial;
		SamsungAdAgent.OnLeaveApplicationInterstitial += OnLeaveApplicationInterstitial;
		SamsungAdAgent.OnAdShown += OnAdShown;
		SamsungAdAgent.OnAdHidden += OnAdHidden;
	}

	private void OnDisable()
	{
		SamsungAdAgent.OnReceiveAd -= OnReceiveAd;
		SamsungAdAgent.OnFailedToReceiveAd -= OnFailedToReceiveAd;
		SamsungAdAgent.OnPresentScreen -= OnPresentScreen;
		SamsungAdAgent.OnDismissScreen -= OnDismissScreen;
		SamsungAdAgent.OnLeaveApplication -= OnLeaveApplication;
		SamsungAdAgent.OnReceiveAdInterstitial -= OnReceiveAdInterstitial;
		SamsungAdAgent.OnFailedToReceiveAdInterstitial -= OnFailedToReceiveAdInterstitial;
		SamsungAdAgent.OnPresentScreenInterstitial -= OnPresentScreenInterstitial;
		SamsungAdAgent.OnDismissScreenInterstitial -= OnDismissScreenInterstitial;
		SamsungAdAgent.OnLeaveApplicationInterstitial -= OnLeaveApplicationInterstitial;
		SamsungAdAgent.OnAdShown -= OnAdShown;
		SamsungAdAgent.OnAdHidden -= OnAdHidden;
	}

	private void OnReceiveAd()
	{
		if (debug)
		{
			Debug.Log(GetType().ToString() + " - OnReceiveAd() Fired.");
		}
	}

	private void OnFailedToReceiveAd(string err)
	{
		if (debug)
		{
			Debug.Log(GetType().ToString() + " - OnFailedToReceiveAd() Fired. Error: " + err);
		}
	}

	private void OnPresentScreen()
	{
		if (debug)
		{
			Debug.Log(GetType().ToString() + " - OnPresentScreen() Fired.");
		}
	}

	private void OnDismissScreen()
	{
		if (debug)
		{
			Debug.Log(GetType().ToString() + " - OnDismissScreen() Fired.");
		}
	}

	private void OnLeaveApplication()
	{
		if (debug)
		{
			Debug.Log(GetType().ToString() + " - OnLeaveApplication() Fired.");
		}
	}

	private void OnReceiveAdInterstitial()
	{
		if (debug)
		{
			Debug.Log(GetType().ToString() + " - OnReceiveAdInterstitial() Fired.");
		}
	}

	private void OnFailedToReceiveAdInterstitial(string err)
	{
		if (debug)
		{
			Debug.Log(GetType().ToString() + " - OnFailedToReceiveAdInterstitial() Fired. Error: " + err);
		}
	}

	private void OnPresentScreenInterstitial()
	{
		if (debug)
		{
			Debug.Log(GetType().ToString() + " - OnPresentScreenInterstitial() Fired.");
		}
	}

	private void OnDismissScreenInterstitial()
	{
		if (debug)
		{
			Debug.Log(GetType().ToString() + " - OnDismissScreenInterstitial() Fired.");
		}
	}

	private void OnLeaveApplicationInterstitial()
	{
		if (debug)
		{
			Debug.Log(GetType().ToString() + " - OnLeaveApplicationInterstitial() Fired.");
		}
	}

	private void OnAdShown()
	{
		if (debug)
		{
			Debug.Log(GetType().ToString() + " - OnAdShown() Fired.");
		}
	}

	private void OnAdHidden()
	{
		if (debug)
		{
			Debug.Log(GetType().ToString() + " - OnAdHidden() Fired.");
		}
	}
}
