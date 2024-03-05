#include "GsCameraModeBackView.h"

#include "T1Project.h"
#include "EngineMinimal.h"
#include "Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Management/ScopeGame/GsCameraModeManager.h"

#include "ActorEx/GsCharacterLocalPlayer.h"
#include "Input/GsInputBindingLocalPlayer.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "GameObject/Abnormality/CrowdControl/GsCrowdControlBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Camera/Work/Rotation/GsCameraRotationBackView.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/Target/GsTargetHandlerBase.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataSchema/Camera/GsSchemaCameraModeData.h"
#include "ActorComponentEx/GsCameraPostProcessComponent.h"
#include "UTIL/GsClassUtil.h"

#include "Core/Public/Misc/DateTime.h"


//#define TEST_LOG


FGsCameraModeBackView::FGsCameraModeBackView() : FGsCameraModeControlBase(EGsCameraMode::BackView)
{
}

void FGsCameraModeBackView::Initialize()
{
	// 대사창 업데이트시 회전갱신후 그값으로 이동처리해야하므로 
	// rot을 먼저 등록!!!
	IGsCameraWork* rotationWork = new FGsCameraRotationBackView();
	rotationWork->Initialize();
	rotationWork->SetMode(this);
	_mapCamWorks.Add(EGsCameraWorkType::RotationWork, rotationWork);

	Super::Initialize();
}


void FGsCameraModeBackView::Enter()
{
	Super::Enter();
	UGsCameraPostProcessComponent::SetMotionBlurMode(true);
}

void FGsCameraModeBackView::SetAxisValue(const FVector& inAxisValue)
{
	if (FGsCameraRotationBackView* rotationWork = GetCameraRotationBackView())
	{
		rotationWork->SetAxisValue(inAxisValue);
	}
}

// 모드 초기화(데이터들)
void FGsCameraModeBackView::InitMode(
	AGsCharacterLocalPlayer* In_localChar,const FGsSchemaCameraModeData* In_modeData)
{
	Super::InitMode(In_localChar, In_modeData);

}


// 인풋에 의한 모드 변환
void FGsCameraModeBackView::ChangeModeNormal(const FGsSchemaCameraModeData* In_modeData)
{
	Super::ChangeModeNormal(In_modeData);

	// 모드 회전 변환 시작
	if (FGsCameraRotationBase* rotationWork = GetCameraRotationBase())
	{
		rotationWork->SetIsModeRotChanging(true);
	}
		
	// 처음에 바로 세팅
	StartFollow(EGsCameraInitType::ModeChange);

	PostChangeModeNormal();
}

// 대화 끝 변환
void FGsCameraModeBackView::ChangeModeDialogEnd()
{
	Super::ChangeModeDialogEnd();
	if (FGsCameraRotationBackView* rotationWork = GetCameraRotationBackView())
	{
		rotationWork->SetFollowMode(EGsCameraInitType::ModeChange);
	}
}
// 그리팅 끝 호출
void FGsCameraModeBackView::OnGreetingOff(bool In_isDirect)
{
	Super::OnGreetingOff(In_isDirect);
	// 처음에 바로 세팅
	// 백뷰 강제 세팅 말고
	StartFollow(EGsCameraInitType::GreetingEnd, In_isDirect);
}

// 상태 종료
void FGsCameraModeBackView::Exit()
{
	UGsCameraPostProcessComponent::SetMotionBlurMode(false);
	Super::Exit();	
}

// 터치 시작
void FGsCameraModeBackView::TouchPress()
{
	Super::TouchPress();

	if (FGsCameraRotationBackView* rotationWork = GetCameraRotationBackView())
	{
		rotationWork->OnTouchPress();
	}
}



// 다음 스텝 진행
void FGsCameraModeBackView::NextStep(FGsCameraModeHandler* In_handler)
{
	Super::NextStep(In_handler);
	if (In_handler == nullptr)
	{
		return;
	}

	// 다음은 쿼터다
	In_handler->ChangeState(EGsCameraMode::Quarter);	
}

// 따라가기 시작
void FGsCameraModeBackView::StartFollow(EGsCameraInitType In_initType, bool In_isDirect)
{	
	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);

	if (camModeData == nullptr)
	{
		GSLOG(Error, TEXT("now mode: %s data is null"),
			*GetEnumToString(EGsCameraMode, nowMode));
		return;
	}



	// 캐릭터 방향에 맞춘다
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	AGsCharacterBase* targetChar = _targetPlayer->GetCharacter();
	if (localChar == nullptr ||
		localChar->Controller == nullptr ||
		localChar->Controller->IsLocalPlayerController() == false ||
		targetChar == nullptr)
	{
		return;
	}
	// backup pitch 구하기
	// 1. backup 값
	// 2. 없으면 기본 모드값
	// 3. min, max 검증 까지 함
	float checkedPitch = GSCameraMode()->GetModeBackupCheckedPitch(nowMode);

	FRotator rotTo = FRotator::ZeroRotator;
	// 그리팅은 백업 데이터 쓸일이 없다
	// 컨틀로러 회전에(현재 회전 그대로) pitch 모드 데이터 넣는다
	// 그리팅 종료에도 백업 데이터(ini 파일) 사용한다
	if (In_initType == EGsCameraInitType::GreetingEnd)
	{
		// 유저 세팅에 저장되었는지
		if (GSCameraMode()->GetIsBackupUserSetting() == true)
		{
			rotTo = GSCameraMode()->GetBackupQuat().Rotator();
		}
		else
		{
			rotTo = localChar->Controller->GetControlRotation();
			rotTo.Pitch = checkedPitch;
		}
	}
	// 모드 변경은 백업 데이터 있으면 쓴다
	// 아니면 액터 회전에 pitch 모드 데이터 사용
	else if(In_initType == EGsCameraInitType::ModeChange)
	{
		if (GSCameraMode()->GetIsBackUpTransform() == true)
		{
			rotTo = GSCameraMode()->GetBackupQuat().Rotator();
		}
		else
		{
			rotTo = targetChar->GetActorRotation();
			rotTo.Pitch = checkedPitch;
		}
	}
	
	if (FGsCameraRotationBackView* rotationWork = GetCameraRotationBackView())
	{
		rotationWork->StartFollow(In_initType, rotTo, In_isDirect);
	}
}


// 캐릭터가 이동 될때 여기가 호출된다(자동이동, 수동 이동)
void FGsCameraModeBackView::UpdateMove(bool In_isAuto)
{
	// 상위꺼 호출
	Super::UpdateMove(In_isAuto);

	// 회전은 자동 일때만 추가
	if (In_isAuto == false)
	{
		return;
	}
}

// 자동 공격시 회전 갱신
void FGsCameraModeBackView::UpdateAttack(bool In_isAuto)
{
	Super::UpdateAttack(In_isAuto);
}

// 카메라 회전 백뷰 처리 접근
FGsCameraRotationBackView* FGsCameraModeBackView::GetCameraRotationBackView()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::RotationWork);
	if (work == nullptr)
	{
		return nullptr;
	}

	FGsCameraRotationBackView* rotationWork = FGsClassUtil::FCheckStaticCast<FGsCameraRotationBackView>(work);
	return rotationWork;
}

// 모드 회전 구하기
FRotator FGsCameraModeBackView::GetModeRot(const FGsSchemaCameraModeData* In_modeData)
{
	FRotator controlRotTo = FRotator::ZeroRotator;

	// 캐릭터 방향에 맞춘다
	AGsCharacterBase* targetChar = _targetPlayer->GetCharacter();
	if (targetChar == nullptr)
	{
		return controlRotTo;
	}

	EGsCameraMode nowMode = GetType();
	float checkedPitch = GSCameraMode()->GetModeBackupCheckedPitch(nowMode);
		
	controlRotTo = targetChar->GetActorRotation();
	controlRotTo.Pitch = checkedPitch;
		
	return controlRotTo;
}

// 업데이트
void FGsCameraModeBackView::Update(float In_deltaTime)
{
	Super::Update(In_deltaTime);
	UpdateFollowCoolTime(In_deltaTime);
	UpdateFollow(In_deltaTime);
}

// follow 갱신
void FGsCameraModeBackView::UpdateFollow(float In_deltaTime)
{
	// 그리팅이 아니면 계속 yaw 회전을 보정한다
	if (_isBeforeGreeting == true)
	{
		return;
	}

	// 수동 이동일때는 막아야 함
	bool isPressJoystick = false;
	if (FGsCameraModeHandler* cameraModehandler = _localPlayer->GetCameraModeHandler())
	{
		isPressJoystick =
			cameraModehandler->GetPressJoystick();
	}

	bool isTouchMove = false;
	// 터치 이동일때도 막음
	if (UGsInputBindingLocalPlayer* localInput = _localPlayer->GetInputBinder())
	{
		isTouchMove = localInput->IsReserveTouchMoveData();
	}

	// 터치 쿨타임이면 막음(터치 땐후 특정 시간)
	// 터치 중이면 막음
	bool isTouchOn = GetTouchOn();

	// 대화연출 복구 중에는 처리 안함
	bool isUseDialogMode = GSCameraMode()->GetIsCameraDialogPlayAll();


	// 모드 전환 회전중에는 follow를 안한다
	bool isModeRotChanging = false;
	bool isRotFinish = true;
	if (FGsCameraRotationBackView* rotationWork = GetCameraRotationBackView())
	{
		isModeRotChanging = rotationWork->GetIsModeRotChanging();
		isRotFinish = rotationWork->GetIsRotationFinish();
	}


	// 조이스틱 프레스 되었음 or 터치 이동이다 or 
	// follow 쿨타임이다 or 터치 회전 중이다 or
	// 대화 연출 복구 중이다 or 모드 변환 rot 
	if (isPressJoystick == true ||
		isTouchMove == true ||
		_isFollowCoolTime == true ||
		isTouchOn == true ||
		isUseDialogMode == true ||
		(isModeRotChanging == true&& isRotFinish == false))
	{
		return;
	}

	// 이동에 의한 회전으로
	// 모드 변환 막는거 해제
	if (FGsCameraRotationBase* rotationWork = GetCameraRotationBase())
	{
		rotationWork->SetIsModeRotChanging(false);
	}

	EGsCameraInitType initMode = EGsCameraInitType::None;
	// is target exist
	if (FGsTargetHandlerBase* targetHandler = _targetPlayer->GetTargetHandler())
	{
		if (UGsGameObjectBase* targetHanderTarget = targetHandler->GetTarget())
		{
			initMode = EGsCameraInitType::TargetFollow;
		}
	}

	StartFollow(initMode);
	
}

// 터치 끝
void FGsCameraModeBackView::TouchRelease(bool In_isSimpleTouch)
{
	Super::TouchRelease(In_isSimpleTouch);

	// use cooltime when release drag
	if (false == In_isSimpleTouch)
	{
		// touch 쿨 시작
		_isFollowCoolTime = true;
		_touchReleaseStartTime = FDateTime::UtcNow().GetTicks();
	}
}

// follow 쿨 갱신
void FGsCameraModeBackView::UpdateFollowCoolTime(float In_deltaTime)
{
	if (_isFollowCoolTime == false)
	{
		return;
	}

	int64 nowTick = FDateTime::UtcNow().GetTicks();
	int64 diffTick = nowTick - _touchReleaseStartTime;

	float coolTimeTick = GSCameraMode()->GetCameraShoulderFollowCoolTimeTick();

	if (diffTick >= coolTimeTick)
	{
		_isFollowCoolTime = false;
	}
}

// 현재 줌min 체크 하는가
bool FGsCameraModeBackView::GetIsNowCheckZoomMin()
{
	// 그리팅이 아니면 함
	return (_isBeforeGreeting == true) ? false : true;
}

void FGsCameraModeBackView::PressJoystick()
{
	if (FGsCameraRotationBackView* rotationWork = GetCameraRotationBackView())
	{
		rotationWork->OnMoveDirectionInput();
	}
}