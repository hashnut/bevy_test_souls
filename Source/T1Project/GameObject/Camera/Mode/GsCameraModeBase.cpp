#include "GsCameraModeBase.h"

#include "GameFrameWork/SpringArmComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "GameFrameWork/PlayerController.h"

#include "Input/GsInputBindingLocalPlayer.h"

#include "EngineMinimal.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"

#include "UTIL/GsText.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataSchema/Camera/GsSchemaCameraModeData.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Camera/Work/Zoom/GsCameraZoomGame.h"
#include "GameObject/Camera/Work/Location/GsCameraLocationGame.h"
#include "GameObject/Camera/Work/Rotation/GsCameraRotationBase.h"
#include "GameObject/Camera/Work/FOV/GsCameraFOVGame.h"
#include "GameObject/Camera/Work/DOF/GsCameraDOFGame.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/Camera/Interface/GsCameraWork.h"

#include "Camera/Define/GsCameraGameDefine.h"

#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"

#include "UTIL/GsClassUtil.h"

// 처음 한번만 하는 초기화


// FGsCameraModeDialog는 해당 Initialize 호출을 의도적으로 주석처리 해놨음
// 공통 기능을 여기에서 초기화 할때는 FGsCameraModeDialog::Initialize 확인요망
void FGsCameraModeBase::Initialize()
{
	IGsCameraWork* dofWork = new FGsCameraDOFGame();
	dofWork->Initialize();
	_mapCamWorks.Add(EGsCameraWorkType::DOFWork, dofWork);
}

// 마지막 정리할때
void FGsCameraModeBase::Close()
{
	if (_mapCamWorks.Num() > 0)
	{
		for (auto& iter : _mapCamWorks)
		{
			if (iter.Value == nullptr)
			{
				continue;
			}

			iter.Value->Finalize();
			delete iter.Value;
			iter.Value = nullptr;
		}
		_mapCamWorks.Empty();
	}

	RemoveCharacter();
}
// 상태 전환(인자로 캐릭터)
void FGsCameraModeBase::Enter()
{
	if (_localPlayer == nullptr ||
		_handler == nullptr)
	{
		return;
	}
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr)
	{
		GSLOG(Error, TEXT("localChar == nullptr"));
		return;
	}

	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);
	InitMode(localChar, camModeData);

	bool isGreetingView = false;
	
	if (UWorld* world = GLevel()->GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			if (gameMode->GetGameModeType() == EGsGameModeType::World||
				gameMode->GetGameModeType() == EGsGameModeType::TestClient)
			{
				if (FGsCameraModeManager* camModeManager = GSCameraMode())
				{
					isGreetingView = camModeManager->GetIsGreetingView();
				}
			}
		}
	}


	// 그리팅 뷰인지
	if (true == isGreetingView)
	{ 		
	}
	else
	{
		EGsCameraInitModeType initModeType = _handler->GetInitModeType();
		switch (initModeType)
		{
		case EGsCameraInitModeType::SameMapSpawn:
			InitSameMapSpawn(camModeData);
			break;
		case EGsCameraInitModeType::OtherMapSpawn:
			// 스폰 스테이트에서 할려고 했는데 무비 플레이시 freeze로 갈수도 있어서
			// 여기서 한다
			InitOtherMapSpawn(camModeData);
			break;
		case EGsCameraInitModeType::ModeChange:
			ChangeMode(camModeData);
			break;
		}
		
	}
}

// 모드 초기화(데이터들)

void FGsCameraModeBase::InitMode(
	AGsCharacterLocalPlayer* In_localChar, const FGsSchemaCameraModeData* In_modeData)
{
	if (nullptr == In_modeData ||
		nullptr == In_localChar)
	{
		return;
	}

	// 회전
	if (In_localChar->Controller &&
		In_localChar->Controller->IsLocalPlayerController())
	{
		APlayerController* const PC =
			CastChecked<APlayerController>(In_localChar->Controller);


		InitPitchMinMax(PC, In_modeData);
	}


	if (FGsCameraRotationBase* rotationWork = GetCameraRotationBase())
	{
		rotationWork->InitMode();
	}

	// 플래그 초기화
	_locationLerpFinished = false;
	_rotaionLerpFinished = false;
}

// pitch min max 초기화
void FGsCameraModeBase::InitPitchMinMax(APlayerController* In_playerController,
	const FGsSchemaCameraModeData* In_modeData)
{
	if (nullptr == In_playerController)
	{
		return;
	}

	// pitch min max
	if (nullptr != In_playerController->PlayerCameraManager)
	{
		In_playerController->PlayerCameraManager->ViewPitchMin =
			In_modeData->ViewPitchMin;
		In_playerController->PlayerCameraManager->ViewPitchMax =
			In_modeData->ViewPitchMax;
	}	
}
// 인풋에 의한 모드 변환
void FGsCameraModeBase::ChangeMode(const FGsSchemaCameraModeData* In_modeData)
{
	bool isUseDialogMode = GSCameraMode()->GetIsCameraDialogPlay();
	if (isUseDialogMode == false)
	{
		ChangeModeNormal(In_modeData);
	}
	else
	{
		ChangeModeDialogEnd();
	}

	// fov도 override 해서 처리
	StartModeFOV();
}

// 일반적인 모드 전환
void FGsCameraModeBase::ChangeModeNormal(const FGsSchemaCameraModeData* In_modeData)
{
	if (FGsCameraLocationGame* locationGame = GetCameraLocationGame())
	{
		locationGame->SetIsAfterDialog(false);
		locationGame->SetIsUseFinishCallback(false);
	}	

	if (FGsCameraFOVGame* fovGame = GetCameraFOVGame())
	{
		fovGame->SetIsAfterDialog(false);
	}
}


// 업데이터(인자로 캐릭터)
void FGsCameraModeBase::Update(float In_deltaTime)
{
	if (_mapCamWorks.Num() > 0)
	{
		for (const auto& iter : _mapCamWorks)
		{
			if (iter.Value == nullptr)
			{
				continue;
			}

			iter.Value->Update(In_deltaTime);
		}
	}
}

// 캐릭터가 이동 될때 여기가 호출된다(자동이동, 수동 이동)
void FGsCameraModeBase::UpdateMove(bool In_isAuto)
{
	// 이전에 그리팅이었나
	if (false == _isBeforeGreeting)
	{
		return;
	}
	
	_isBeforeGreeting = false;

	GreetingOff();

}

// 자동 공격시 회전 갱신
void FGsCameraModeBase::UpdateAttack(bool In_isAuto)
{
	// 이전에 그리팅이었나
	if (false == _isBeforeGreeting)
	{
		return;
	}

	_isBeforeGreeting = false;

	GreetingOff();
}
// 그리팅 끝 호출
void FGsCameraModeBase::OnGreetingOff(bool In_isDirect)
{
	// pitch min max 복구(모드값)
	if (_localPlayer == nullptr)
	{
		return;
	}
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr)
	{
		GSLOG(Error, TEXT("localChar == nullptr"));
		return;
	}

	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);

	if (camModeData == nullptr)
	{
		return;
	}
	// 회전
	if (localChar->Controller &&
		localChar->Controller->IsLocalPlayerController())
	{
		APlayerController* const PC =
			CastChecked<APlayerController>(localChar->Controller);


		InitPitchMinMax(PC, camModeData);
	}

	// 그리팅 끝 메시지
	GMessage()->GetCamera().SendMessage(MessageContentCamera::GREETING_OFF);

	if (In_isDirect == false)
	{
		if (FGsCameraRotationBase* rotationWork = GetCameraRotationBase())
		{
			rotationWork->SetIsModeRotChanging(true);
		}
	}
}

// 그리팅 호출 후처리
void FGsCameraModeBase::OnGreetingOffAfter()
{
	GSCameraMode()->SetIsBackupUserSetting(false);
}

// 그리팅 off 처리
void FGsCameraModeBase::GreetingOff(bool In_isDirect)
{
	OnGreetingOff(In_isDirect);
	OnGreetingOffAfter();
}

// 캐릭터 삭제 처리
void FGsCameraModeBase::RemoveCharacter() 
{
	_targetPlayer = nullptr;
	_localPlayer = nullptr;
}

void FGsCameraModeBase::SetModeTarget(UGsGameObjectBase* In_target)
{
	_targetPlayer = In_target;

	if (_mapCamWorks.Num() > 0)
	{
		for (const auto& iter : _mapCamWorks)
		{
			if (iter.Value == nullptr)
			{
				continue;
			}

			iter.Value->SetWorkTarget(In_target);
		}
	}
}
// 로컬 플레이어 세팅
void FGsCameraModeBase::SetModeLocalObject(UGsGameObjectLocalPlayer* In_local)
{
	_localPlayer = In_local;

	if (_mapCamWorks.Num() > 0)
	{
		for (const auto& iter : _mapCamWorks)
		{
			if (iter.Value == nullptr)
			{
				continue;
			}

			iter.Value->SetWorkLocalPlayer(In_local);
		}
	}
}
// 모드 회전 변환중인지
bool FGsCameraModeBase::GetIsModeRotChanging()
{
	FGsCameraRotationBase* rotationWork = GetCameraRotationBase();
	if (rotationWork == nullptr)
	{
		return false;
	}
	// 모드 체인지 이면서 회전끝이 아니면 막는다
	bool resVal = (rotationWork->GetIsModeRotChanging() == true &&
		rotationWork->GetIsRotationFinish() == false);
	return resVal;
}
// 보간 끝
void FGsCameraModeBase::OnFinishLerpType(EGsDialogCameraFinishType In_type)
{
	if (In_type == EGsDialogCameraFinishType::Location)
	{
		_locationLerpFinished = true;
	}
	else
	{
		_rotaionLerpFinished = true;
	}

	if (_locationLerpFinished == true &&
		_rotaionLerpFinished == true)
	{
		// 다시 들어오는거 막기용
		// 대사복구 다음 backview follow 회전시
		_locationLerpFinished = false;
		_rotaionLerpFinished = false;

		OnFinishLerpMode();
	}
}
// 카메라 기능 접근
IGsCameraWork* FGsCameraModeBase::GetCameraWork(EGsCameraWorkType In_type)
{
	IGsCameraWork* work = _mapCamWorks.FindRef(In_type);
	return work;
}
// 카메라 위치 처리 접근
FGsCameraLocationGame* FGsCameraModeBase::GetCameraLocationGame()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::LocationWork);
	if (work == nullptr)
	{
		return nullptr;
	}

	FGsCameraLocationGame* locationWork = FGsClassUtil::FCheckStaticCast<FGsCameraLocationGame>(work);
	return locationWork;
}

// 카메라 회전 처리 접근
FGsCameraRotationBase* FGsCameraModeBase::GetCameraRotationBase()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::RotationWork);
	if (work == nullptr)
	{
		return nullptr;
	}

	FGsCameraRotationBase* rotationWork = FGsClassUtil::FCheckStaticCast<FGsCameraRotationBase>(work);
	return rotationWork;
}

// 카메라 fov 처리 접근
FGsCameraFOVGame* FGsCameraModeBase::GetCameraFOVGame()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::FOVWork);
	if (work == nullptr)
	{
		return nullptr;
	}

	FGsCameraFOVGame* fovWork = FGsClassUtil::FCheckStaticCast<FGsCameraFOVGame>(work);
	return fovWork;
}

// 카메라 dof 처리 접근
FGsCameraDOFGame* FGsCameraModeBase::GetCameraDOFGame()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::DOFWork);
	if (work == nullptr)
	{
		return nullptr;
	}

	FGsCameraDOFGame* dofWork = FGsClassUtil::FCheckStaticCast<FGsCameraDOFGame>(work);
	return dofWork;
}

// 다음 스텝 진행(매니저 넘김)
void FGsCameraModeBase::NextStep(FGsCameraModeHandler* In_handler)
{
	// 이전에 그리팅이었으면
	// 파일 백업 클리어
	if (true == _isBeforeGreeting)	
	{
		GSCameraMode()->SetIsBackupUserSetting(false);
	}
}

// 줌 최소 도달 콜백
void FGsCameraModeBase::OnReachZoomMin(bool In_isReached)
{
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}

	UCameraComponent* camComponent = localChar->GetFollowCamera();
	if (camComponent == nullptr)
	{
		return;
	}

	// dof 사용 모드 인가
	EGsCameraMode nowMode = GetType();
	const FGsSchemaCameraModeData* camModeData = FGsCameraModeManager::GetCamModeData(nowMode);
	if (camModeData == nullptr)
	{
		return;
	}
	// dof 사용하면서 실제 값이 true 일때 on
	bool isDofOn = (camModeData->UseZoomMinDOF && In_isReached) ? true : false;
	// dof 사용하면서 실제 값이 true 일때 on
	bool isFovOn = (camModeData->UseZoomMinFOV && In_isReached) ? true : false;

	if (FGsCameraDOFGame* dofGame = GetCameraDOFGame())
	{
		// 현재 값
		float scaleFrom =
			camComponent->PostProcessSettings.DepthOfFieldScale;
		// min 에도달하면 scale max로 
		// min 아니면 0으로
		float scaleTo = (isDofOn == true) ?
			GData()->GetGlobalData()->_cameraDepthOfFieldScaleMax :
			0.0f;

		dofGame->StartDOF(scaleFrom, scaleTo, isDofOn);
	}

	// fov 변경
	OnReachZoomFOV(isFovOn);
}
// make common speed(for sync)
void FGsCameraModeBase::MakeDialogSpeed(FGsCameraLocationGame* In_locationWork, FGsCameraRotationBase* In_rotationWork)
{
	if (In_locationWork == nullptr ||
		In_rotationWork == nullptr)
	{
		return;
	}

	float locationSpeed = In_locationWork->MakeDistSpeed();
	float rotationSpeed = In_rotationWork->MakeDistSpeed();

	// 둘중 더 느린걸로 하자(싱크 맞추가)
	float slowSpeed = FMath::Min(rotationSpeed, locationSpeed);

	// prevent too slow
	float minVSpeed = GData()->GetGlobalData()->CamDialogMinSpeed;
	slowSpeed = FMath::Max(slowSpeed, minVSpeed);

	float mulVal = GData()->GetGlobalData()->CamDialogLerpSpeedMul;

	slowSpeed *= mulVal;

	In_rotationWork->SetNowSpeed(slowSpeed);
	In_locationWork->SetNowSpeed(slowSpeed);

	_currentWorkSpeed = slowSpeed;
}

void FGsCameraModeBase::Exit()
{
	if (_mapCamWorks.Num() > 0)
	{
		for (const auto& iter : _mapCamWorks)
		{
			if (iter.Value == nullptr)
			{
				continue;
			}

			iter.Value->ClearFinishValue();
		}
	}
}

void FGsCameraModeBase::GreetingOffForeced()
{
	if (false == _isBeforeGreeting)
	{
		return;
	}

	_isBeforeGreeting = false;

	GreetingOff(true);

}

void FGsCameraModeBase::CameraRotationChanged()
{
	if (FGsCameraDOFGame* dofGame = GetCameraDOFGame())
	{
		dofGame->UpdateFocalDistance();
	}
}

void FGsCameraModeBase::OnJoysticChanged()
{
	if (FGsCameraDOFGame* dofGame = GetCameraDOFGame())
	{
		dofGame->UpdateFocalDistance();
	}
}