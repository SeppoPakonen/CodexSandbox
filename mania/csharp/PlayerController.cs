using System.Collections;
using UnityEngine;

public class PlayerController : MonoBehaviour
{
	public CurvySpline Spline;

	public float Speed = 20f;

	public float JumpSpeed = 20f;

	public float JumpDuration = 0.5f;

	public float Gravity = 15f;

	public float TF;

	private CharacterController mController;

	private Transform mTransform;

	private float mLastCurveY;

	private float mJumpDurationLeft;

	private bool mStopMoving;

	private IEnumerator Start()
	{
		mController = GetComponent<CharacterController>();
		mController.stepOffset = 0f;
		mTransform = base.transform;
		CapsuleCollider headCollider = GetComponentInChildren<CapsuleCollider>();
		Physics.IgnoreCollision(headCollider, ((Component)mController).get_collider());
		if ((bool)Spline)
		{
			while (!Spline.IsInitialized)
			{
				yield return new WaitForEndOfFrame();
			}
			Init();
		}
	}

	private void Update()
	{
		Vector3 zero = Vector3.zero;
		Vector3 position = mTransform.position;
		float tF = TF;
		float y = mLastCurveY;
		float axis = Input.GetAxis("Horizontal");
		bool button = Input.GetButton("Jump");
		if (axis != 0f)
		{
			int num = (!(axis > 0f)) ? 1 : (-1);
			int direction = num;
			Vector3 vector = Spline.MoveBy(ref TF, ref direction, Mathf.Abs(axis) * Speed * Time.smoothDeltaTime, CurvyClamping.PingPong);
			if (direction != num)
			{
				Spline = Spline.GetComponent<NextRailSpline>().NextSpline;
				TF = Mathf.Abs(1f - TF);
				direction = num;
				vector = Spline.MoveBy(ref TF, ref direction, Mathf.Abs(axis) * Speed * Time.smoothDeltaTime, CurvyClamping.PingPong);
			}
			zero.x = vector.x - position.x;
			zero.z = vector.z - position.z;
			y = vector.y;
		}
		if (button && mJumpDurationLeft > 0f)
		{
			zero += new Vector3(0f, JumpSpeed * Time.smoothDeltaTime, 0f);
			mJumpDurationLeft -= Time.deltaTime;
		}
		else
		{
			zero += new Vector3(0f, (0f - Gravity) * Time.smoothDeltaTime, 0f);
		}
		if (position.y + zero.y < y)
		{
			zero.y = y - position.y;
			mJumpDurationLeft = JumpDuration;
		}
		if (zero != Vector3.zero)
		{
			if (mController.Move(zero) != 0)
			{
				if (mStopMoving)
				{
					mTransform.position = position;
					TF = tF;
					y = mLastCurveY;
				}
			}
			else
			{
				mStopMoving = false;
			}
			mTransform.rotation = Spline.GetOrientationFast(TF);
		}
		mLastCurveY = y;
	}

	private void OnControllerColliderHit(ControllerColliderHit hit)
	{
		Vector3 moveDirection = hit.moveDirection;
		mStopMoving = (moveDirection.x != 0f);
	}

	private void Init()
	{
		if ((bool)Spline)
		{
			mTransform.position = Spline.Interpolate(TF);
			mTransform.rotation = Spline.GetOrientationFast(TF);
			Vector3 position = mTransform.position;
			mLastCurveY = position.y;
		}
	}
}
