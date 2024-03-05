#include "GsCameraRotationBackView.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "GameObject/Abnormality/CrowdControl/GsCrowdControlBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Camera/Mode/GsCameraModeBackView.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Target/GsTargetHandlerBase.h"

#include "ControllerEx/GsPlayerController.h"

#include "Management/ScopeGame/GsCameraModeManager.h"

#include "Camera/Define/GsCameraGameDefine.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataSchema/Camera/GsSchemaCameraModeData.h"

#include "UTIL/GsMath.h"

#include "Engine/Classes/Curves/CurveFloat.h"
#include "Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Camera/PlayerCameraManager.h"

// 회전속도 구하기
float FGsCameraRotationBackView::GetRotSpeed()
{
	float rotSpeed = 0.0f;

	if (_isAfterDialog == true)
	{
		rotSpeed =
			GData()->GetGlobalData()->CamDialogRotLerpSpeed;
	}
	else if (_isModeRotChanging == true)
	{
		rotSpeed =
			GData()->GetGlobalData()->CamModeChangeRotLerpSpeed;
	}
	else
	{	
		rotSpeed =
			(_followMode == EGsCameraInitType::None)? 
			GData()->GetGlobalData()->CamFollowRotLerpSpeed :
			GData()->GetGlobalData()->CamShoulderTargetRotLerpSpeed;
	}
	return rotSpeed;
}

// 속도 만들기
void FGsCameraRotationBackView::MakeSpeed()
{
	if (_isAfterDialog == true)
	{
		// 회전과 이동 같은 속도로 처리하게 하기위해 외부에서 밀어넣음
	}
	else
	{
		Super::MakeSpeed();
	}
}

// 자동회전 갱신
void FGsCameraRotationBackView::UpdateAutoRot(float In_deltaTime)
{
	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();

	// 타겟 캐릭터 체크
	if (nullptr == localChar)
	{
		return;
	}

	// 타겟 오브젝트 체크
	if (nullptr == _local ||
		nullptr == _targetObj)
	{
		return;
	}

	// 타겟 obj의 스킬(실제 카메라 attach 된 대상)
	FGsSkillHandlerBase* skill = _targetObj->GetSkillHandler();
	// 스킬 체크
	if (nullptr == skill)
	{
		return;
	}
	FGsSkillRunnerBase* skillRunner = skill->GetSkillRunner();

	// 스킬 러너 체크
	if (nullptr == skillRunner)
	{
		return;
	}	

	float inputDegrees =
		FGsMath::CalcDegreesFromCoordinate(_currentXInput, _currentYInput);
#ifdef TEST_LOG		
	GSLOG(Warning,
		TEXT("inputDegrees: %f"),
		inputDegrees
	);
#endif
	//_currentXInput = 0.0f;
	//_currentYInput = 0.0f;

	float calcDegrees = 0.0f;
	// 조이스틱 기준 오른쪽 체크(제한 안에서)
	if (inputDegrees >= _rightMin &&
		inputDegrees <= _rightMax)
	{
		calcDegrees = inputDegrees;
	}

	// 조이스틱 기준 왼쪽 체크(제한 안에서)
	if (inputDegrees >= _leftMin &&
		inputDegrees <= _leftMax)
	{
		calcDegrees = inputDegrees;
	}
	float camAddRotVal = GData()->GetGlobalData()->CamAddRotVal;


#ifdef TEST_LOG
	GSLOG(Warning, TEXT("calcDegrees: %f"), calcDegrees);
#endif

	// 값이 0으로 나오면 처리할 필요 없음
	// 스킬 사용중이면 회전 하지말기...
	// 리액션 중이어도 하지말기...
	// 카메라 회전 더하기 값이 0이면 처리안한다
	if (//calcDegrees == 0.0f ||
		FMath::IsNearlyZero(calcDegrees) ||
		skillRunner->IsValid() == true ||
		_targetObj->IsCrowdControlLock() == true ||
		//camAddRotVal == 0.0f)
		FMath::IsNearlyZero(camAddRotVal))
	{
		// 자동 회전 최근값 초기화
		_currentAutoRotInput = 0;
	}
	else
	{

		// 양수, 음수 구분해서 처리
		float  degreeSymbol = (calcDegrees > 0) ? 1.0f: -1.0f;
		// 고정 값으로 하자 시대가 바뀌었다(린2m)
		_targetAutoRotInput = camAddRotVal * degreeSymbol;


		// 자동 회전 최근값 보간처리
		_currentAutoRotInput =
			FMath::FInterpTo(
				_currentAutoRotInput,
				_targetAutoRotInput, In_deltaTime,
				GData()->GetGlobalData()->CamAutoRotIntpSpeed);

#ifdef TEST_LOG
		GSLOG(Warning,
			TEXT("_targetAutoRotInput: %f, _currentAutoRotInput: %f"),
			_targetAutoRotInput,
			_currentAutoRotInput);
#endif

		UCurveFloat* curve = GData()->GetCameraAutoRotCurve();

		// 커브 처리
		float progressRate =
			_currentAutoRotInput / _targetAutoRotInput;

		float curveVal = 0.0f;
		if (curve == nullptr)
		{
			GSLOG(Error, TEXT("curve is nullptr"));
		}
		else
		{
			curveVal = curve->GetFloatValue(progressRate);
		}

		// 커브값을 곱함
		float curveMulVal = _currentAutoRotInput * curveVal;

		localChar->AddControllerYawInput(curveMulVal);

	}
}

void FGsCameraRotationBackView::Update(float In_deltaTime)
{	
	bool isTouchOn = GetTouchOn();

	// 입력이 있다면
	// 터치는 안되어있어야함
	if (
		//(_currentXInput != 0.0f || _currentYInput != 0.0f) 
		// 매우 작은수 들어올수 있으므로
		// 1e-8 whose meaning is 10^-8 i.e., 0.00000001 .
		//https://www.quora.com/What-does-the-C++-statement-int-I-1e-8-mean
		(!FMath::IsNearlyZero(_currentXInput) || !FMath::IsNearlyZero(_currentYInput))
		&& (isTouchOn == false))
	{
		UpdateAutoRot(In_deltaTime);
	}
	// 없으면
	else
	{
		// 자동 회전 최근값 초기화
		_currentAutoRotInput = 0;
	}
	Super::Update(In_deltaTime);
}
void FGsCameraRotationBackView::SetAxisValue(const FVector& In_axisValue)
{
	_currentYInput = In_axisValue.Y;
	_currentXInput = In_axisValue.X;
}

// 따라가기 시작
void FGsCameraRotationBackView::StartFollow(
	EGsCameraInitType In_initType,
	FRotator In_rot, 
	bool In_isDirect)
{
	_followMode = In_initType;

	if (nullptr == _local)
	{
		return;
	}

	// 캐릭터 방향에 맞춘다
	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
	AGsCharacterBase* targetChar = _targetObj->GetCharacter();
	if (localChar == nullptr||
		localChar->Controller == nullptr ||
		localChar->Controller->IsLocalPlayerController() == false ||
		targetChar == nullptr)
	{
		return;
	}
	// 시작값 빽업
	FRotator followRotFrom = localChar->Controller->GetControlRotation();

	FRotator followRotTo = FRotator::ZeroRotator;

	if (EGsCameraInitType::GreetingEnd == In_initType ||
		EGsCameraInitType::ModeChange == In_initType)
	{
		followRotTo = In_rot;
	}
	// 액터에 diff값을 더해서 컨트럴 회전값을 구함
	else
	{
		// 린2m 숄더뷰 기준은 pitch는 놔두고 yaw만 보정함

		// 기존꺼에서
		followRotTo = followRotFrom;

		FRotator nowRot = GetDiffRotTo();
		// 목표치의
		FRotator totalFollowRotTo = nowRot + targetChar->GetActorRotation();

		// yaw만 보정한다
		followRotTo.Yaw = totalFollowRotTo.Yaw;		

		// 카메라 값 백업해놓기(유효한 상황에서만 백업/gameusersetting에 저장될 값)
		//카메라 회전
		//카메라 줌
		//모드 변경
		//back view 갱신시

		// 자동으로 돌아갈때 회전값 저장
		GSCameraMode()->SetBackupQuat(followRotTo.Quaternion());
	}

#ifdef TEST_LOG
	GSLOG(Log, TEXT("startFollow _followRotFrom: %s _followRotTo: %s"),
		*_followRotFrom.ToString(),
		*_followRotTo.ToString());
#endif

	// 이미 회전 중이고 
	// 목표치가 같으면 하지 말자
	if (GetIsRotationFinish() == false &&
		_rotTo.Equals(followRotTo) == true)
	{
		return;
	}

	// 시작과 끝이 같아도 하지말자
	if (followRotTo.Equals(followRotFrom)==true)
	{
		return;
	}


	// 회전 타입은 컨트롤 회전
	_rotationType = EGsCameraRotationType::ControlRotaion;

	if (In_isDirect == true)
	{
		SetRotation(followRotTo);
	}
	else
	{
		// 회전값 세팅
		SetTargetRot(
			followRotTo,
			followRotFrom);
	}
}

void FGsCameraRotationBackView::SetRotation(const FRotator& In_rot)
{
	if (_local == nullptr)
	{
		return;
	}
	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();

	// sentry nullptr 체크
	// https://com2us-raon.sentry.io/issues/4279926795/?alert_rule_id=12817173&alert_timestamp=1687938172341&alert_type=email&environment=Release&project=4504115468566528&referrer=alert_email
	if (localChar == nullptr)
	{
		return;
	}

	if (_rotationType == EGsCameraRotationType::WorldRotation)
	{
		if (UCameraComponent* camComponent = localChar->GetFollowCamera())
		{
			camComponent->SetWorldRotation(In_rot);
		}
	}
	else if (_rotationType == EGsCameraRotationType::ControlRotaion)
	{
		if (localChar->Controller == nullptr ||
			localChar->Controller->IsLocalPlayerController() == false)
		{
			return;
		}
		localChar->Controller->SetControlRotation(In_rot);		
	}
	
}
void FGsCameraRotationBackView::OnFinishLerp()
{
	Super::OnFinishLerp();
	if (_isUseFinishCallback == false || _owner == nullptr)
	{
		return;
	}

	_owner->OnFinishLerpType(EGsDialogCameraFinishType::Rotation);	
}

// 회전 가능한가
bool FGsCameraRotationBackView::IsRotationPossible()
{
	bool isTouchOn = GetTouchOn();
	// 아닌 케이스
	// 1. rotation finished
	// 2. on touch
	return ((_isFinish == true) || ( true == isTouchOn))
		? false : true;
}
bool FGsCameraRotationBackView::GetTouchOn()
{
	bool isTouchOn = false;
	if (_owner != nullptr)
	{
		if (FGsCameraModeBackView* backviewMode = static_cast<FGsCameraModeBackView*>(_owner))
		{
			isTouchOn = backviewMode->GetTouchOn();
		}
	}
	return isTouchOn;
}

// 현재 사용 커브 접근
UCurveFloat* FGsCameraRotationBackView::GetCurve()
{
	UCurveFloat* curve = nullptr;
	// 1. follow
	// 2. target follow(target exist)
	// 3. etc

	switch (_followMode)
	{
	case EGsCameraInitType::None:
		curve = GData()->GetCamShoulderFollowCurve();
		break;
	case EGsCameraInitType::TargetFollow:
		curve = GData()->GetCamShoulderTargetCurve();
		break;
	default:
		curve = GData()->GetCamFollowCurve();
		break;
	}
	

	return curve;
}
// 전투 상태랑 그외랑 분리(숄더뷰 전투시 회전 틀어서 타겟 중심으로 보이게)
FRotator FGsCameraRotationBackView::GetDiffRotTo()
{
	if (nullptr == _targetObj)
	{
		return FRotator::ZeroRotator;
	}

	FRotator nowRot = FRotator::ZeroRotator;

	// 타겟이 있으면
	if (FGsTargetHandlerBase* targetHandler = _targetObj->GetTargetHandler())
	{
		UGsGameObjectBase* targetHanderTarget = targetHandler->GetTarget();
		if (nullptr != targetHanderTarget)
		{
			AGsCharacterBase* cameraParentChar = _targetObj->GetCharacter();
			if (cameraParentChar == nullptr)
			{
				return nowRot;
			}

			// cam - target look
			FVector currCamLocation = FVector::ZeroVector;
			AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
			if (localChar == nullptr)
			{
				return nowRot;
			}
			if (AGsPlayerController* controller = localChar->GetPlayerController())
			{
				APlayerCameraManager* camManager = controller->PlayerCameraManager;
				if (camManager != nullptr)
				{
					currCamLocation = camManager->GetCameraLocation();					
				}
			}
			FVector lookVec = targetHanderTarget->GetLocation() - currCamLocation;
			nowRot.Yaw = lookVec.Rotation().Yaw + _addShoulderTargetYaw;

			nowRot -= cameraParentChar->GetActorRotation();
		}
	}


	return nowRot;
}

void FGsCameraRotationBackView::Initialize()
{
	Super::Initialize();

	_addShoulderTargetYaw = 
		GData()->GetGlobalData()->_cameraShoulderTargetAddYaw;
}

void FGsCameraRotationBackView::OnTouchPress()
{
	// rot finish
	_isFinish = true;
}

// joystick or keyboard dir input
void FGsCameraRotationBackView::OnMoveDirectionInput()
{
	// rot finish
	_isFinish = true;
}