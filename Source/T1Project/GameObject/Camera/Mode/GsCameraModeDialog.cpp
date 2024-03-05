#include "GsCameraModeDialog.h"

#include "Camera/Define/EGsDialogCameraViewType.h"

#include "GameObject/Camera/Work/Rotation/GsCameraRotationDialog.h"
#include "GameObject/Camera/Work/Rotation/GsCameraRotationDialogCutLoop.h"
#include "GameObject/Camera/Work/Location/GsCameraLocationDialog.h"
#include "GameObject/Camera/Work/FOV/GsCameraFOVDialog.h"
#include "GameObject/Camera/Work/Zoom/GsCameraZoomDialog.h"
#include "GameObject/Camera/GsCameraModeHandler.h"

#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/GsMessageholder.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "DataSchema/Camera/GsSchemaCameraModeData.h"
#include "DataSchema/Camera/GsSchemaCameraDialogViewData.h"
#include "DataSchema/GsSchemaEnums.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

#include "Camera/Define/GsCameraGameDefine.h"

#include "T1Project.h"

#include "UTIL/GsClassUtil.h"

#include "Kismet/GameplayStatics.h"

#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Camera/PlayerCameraManager.h"
#include "Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Engine/GameInstance.h"
UCameraComponent* FGsCameraModeDialog::GetCameraComponent()
{
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return nullptr;
	}
	return localChar->GetFollowCamera();
}

// 타겟중 가장먼거리를 DepthOfFieldFocalDistance 에 세팅하고
// 설정값중 활성화 된 것을 대입한다.
void FGsCameraModeDialog::SetupCameraDOF()
{
	FGsCameraModeManager* cameraModeManager = GSCameraMode();
	if (cameraModeManager == nullptr)
	{
		return;
	}
	
	const FGsSchemaCameraDialogViewData* viewData = cameraModeManager->GetCurrentViewData();
	if (nullptr == viewData)
	{
		return;
	}

	UCameraComponent* camera = GetCameraComponent();
	if (nullptr == camera)
	{
		return;
	}

	float focusDistance = GetFocaDistance();
	if ( 0.0f != focusDistance)
	{
		float fCalcDepthOfFieldFocalDistance = focusDistance;

#if WITH_EDITOR
//		if (IsPCPlatform())
//		{
//			camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
//			camera->PostProcessSettings.bOverride_DepthOfFieldFstop = true;
//
//			// 계산된 기본값
//			camera->PostProcessSettings.DepthOfFieldFocalDistance = fCalcDepthOfFieldFocalDistance;
//
//			// 수동 입력값이 있다면 덮어 쓰기
//			if (viewData->bOverride_DepthOfFieldFstop)
//			{
//				camera->PostProcessSettings.DepthOfFieldFstop = viewData->DepthOfFieldFstop;
//			}
//			if (viewData->bOverride_DepthOfFieldFocalDistance)
//			{
//				camera->PostProcessSettings.DepthOfFieldFocalDistance = viewData->DepthOfFieldFocalDistance;
//			}
//		}
//		else
//		{
			camera->PostProcessSettings.bOverride_DepthOfFieldFocalRegion = true;
			camera->PostProcessSettings.bOverride_DepthOfFieldScale = true;
			camera->PostProcessSettings.DepthOfFieldScale = GData()->GetGlobalData()->_dialogDepthOfFieldScale;
			// 계산된 기본값
			camera->PostProcessSettings.DepthOfFieldFocalRegion = fCalcDepthOfFieldFocalDistance;

			// 수동 입력값이 있다면 덮어 쓰기
			if (viewData->bOverride_DepthOfFieldFocalRegion)
			{
				camera->PostProcessSettings.DepthOfFieldFocalRegion = viewData->DepthOfFieldFocalRegion;
			}
//		}
#endif
//#else
		//// 기능 활성화
		//camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
		//camera->PostProcessSettings.bOverride_DepthOfFieldFstop = true;

		//// 계산된 기본값
		//camera->PostProcessSettings.DepthOfFieldFocalDistance				= fCalcDepthOfFieldFocalDistance;

		//// 수동 입력값이 있다면 덮어 쓰기
		//if (viewData->bOverride_DepthOfFieldFstop)
		//{
		//	camera->PostProcessSettings.DepthOfFieldFstop					= viewData->DepthOfFieldFstop;
		//}
		//if (viewData->bOverride_DepthOfFieldFocalDistance)
		//{
		//	camera->PostProcessSettings.DepthOfFieldFocalDistance			= viewData->DepthOfFieldFocalDistance;
		//}
//#endif
#if PLATFORM_ANDROID || PLATFORM_IOS	
		// 기능 활성화
		camera->PostProcessSettings.bOverride_DepthOfFieldFocalRegion = true;
		camera->PostProcessSettings.bOverride_DepthOfFieldScale = true;
		camera->PostProcessSettings.DepthOfFieldScale						= GData()->GetGlobalData()->_dialogDepthOfFieldScale;
		// 계산된 기본값
		camera->PostProcessSettings.DepthOfFieldFocalRegion					= fCalcDepthOfFieldFocalDistance;

		// 수동 입력값이 있다면 덮어 쓰기
		if (viewData->bOverride_DepthOfFieldFocalRegion)
		{
			camera->PostProcessSettings.DepthOfFieldFocalRegion				= viewData->DepthOfFieldFocalRegion;
		}
#endif
	}
}

float FGsCameraModeDialog::GetFocaDistance()
{
	FGsCameraModeManager* cameraMode = GSCameraMode();
	if (cameraMode == nullptr)
	{
		return 0.0f;
	}
	EGsDialogCameraViewType viewType = cameraMode->GetDialogCameraViewType();
	UCameraComponent* camera = GetCameraComponent();

	float fDistance = 0.0f;
	switch (viewType)
	{
	case EGsDialogCameraViewType::None:						// 초기화 된 키값 그대로 쓰면 에러나서 넣음
		break;
	case EGsDialogCameraViewType::DialogBackLocalSingleNpc:	// 로컬 뒤로, npc 하나
	case EGsDialogCameraViewType::DialogBackNpc:			// npc 뒤로
	case EGsDialogCameraViewType::DialogSide:				// 옆으로
	case EGsDialogCameraViewType::DialogGreetingNpc:		// npc 그리팅
	case EGsDialogCameraViewType::DialogGreetingNpcFairy:		// npc fairy
	case EGsDialogCameraViewType::DialogBackLocalMultiNpc:	// 로컬 뒤로, npc 여럿
	case EGsDialogCameraViewType::DialogGreetingObject:		// 오브젝트 그리팅	
		{
			TArray<UGsGameObjectBase*> targetObjs = _handler->GetDialogTargets();
			float fSqrDistMax = (camera->GetComponentLocation() - _localPlayer->GetLocation()).SizeSquared();
			UGsGameObjectBase* maxDistTarget = _localPlayer;
			for (int i = 0; i < targetObjs.Num(); ++i)
			{
				float fSqrDist = (camera->GetComponentLocation() - targetObjs[i]->GetLocation()).SizeSquared();
				if (fSqrDistMax < fSqrDist)
				{
					fSqrDistMax = fSqrDist;
					maxDistTarget = targetObjs[i];
				}
			}

			if (nullptr != maxDistTarget)
			{
				float fAddValue = 100.0f;
				if (AActor* actor = maxDistTarget->GetActor())
				{
					if (UCapsuleComponent* capsule = Cast< UCapsuleComponent>(actor->GetComponentByClass(UCapsuleComponent::StaticClass())))
					{
						fAddValue = capsule->GetScaledCapsuleRadius();
					}
				}
				fDistance = (camera->GetComponentLocation() - maxDistTarget->GetLocation()).Size() + fAddValue;
			}
		}
		break;
	case EGsDialogCameraViewType::DialogGreetingLocalFairy:	// 로컬과 페어리 그리팅(중간점을 그리팅한다)
	case EGsDialogCameraViewType::DialogGreetingLocal:		// 로컬 그리팅		
		{
			float fAddValue = 100.0f;
			if (AGsCharacterPlayer* actor = _localPlayer->GetPlayerCharacter())
			{
				if (UCapsuleComponent* capsule = actor->GetCapsuleComponent())
				{
					fAddValue = capsule->GetScaledCapsuleRadius();
				}
			}
			fDistance = (camera->GetComponentLocation() - _localPlayer->GetLocation()).Size() + fAddValue;
		}
		break;

	case EGsDialogCameraViewType::DialogGreetingSpawnFairy:		// fairy greeting		
	{
		UGsGameObjectBase* fairyObj = cameraMode->FindDialogFairyObj();
		if (fairyObj != nullptr)
		{
			fDistance = (camera->GetComponentLocation() - fairyObj->GetLocation()).Size() + 100.0f;
		}
	}
	break;
	default:
		break;
	}
	return fDistance;
}

#if WITH_EDITOR
bool FGsCameraModeDialog::IsPCPlatform()
{
	auto instance = _localPlayer->GetWorld()->GetGameInstance();
	if (nullptr == instance)
	{
		return true;
	}
	return (instance->GetWorldContext()->PIEWorldFeatureLevel == ERHIFeatureLevel::SM5) ? true : false;
}
#endif

FGsCameraModeDialog::FGsCameraModeDialog() : FGsCameraModeBase(EGsCameraMode::Dialog)
{
	
}

void FGsCameraModeDialog::Initialize()
{
	// bak1210 
	// 상위에서 DOF 컨트롤러를 생성하는데..  이모드는 활용하지 않아서 주석
	// 	다만 상위에 공통로직이 추가되면 코드변경이 필요함
	//Super::Initialize();

	IGsCameraWork* fovWork = new FGsCameraFOVDialog();
	fovWork->Initialize();
	_mapCamWorks.Add(EGsCameraWorkType::FOVWork, fovWork);

	IGsCameraWork* rotationWork = new FGsCameraRotationDialog();
	rotationWork->Initialize();
	rotationWork->SetMode(this);
	_mapCamWorks.Add(EGsCameraWorkType::RotationWork, rotationWork);

	IGsCameraWork* locationWork = new FGsCameraLocationDialog();
	locationWork->Initialize();
	locationWork->SetMode(this);
	_mapCamWorks.Add(EGsCameraWorkType::LocationWork, locationWork);

	IGsCameraWork* zoomWork = new FGsCameraZoomDialog();
	zoomWork->Initialize();
	zoomWork->SetMode(this);
	_mapCamWorks.Add(EGsCameraWorkType::ZoomWork, zoomWork);

	IGsCameraWork* rotationDialogCutLoopWork = new FGsCameraRotationDialogCutLoop();
	rotationDialogCutLoopWork->Initialize();
	rotationDialogCutLoopWork->SetMode(this);
	_mapCamWorks.Add(EGsCameraWorkType::DialogCutLoopRotationWork, rotationDialogCutLoopWork);
}

void FGsCameraModeDialog::Enter()
{
	Super::Enter();

	
	if (UCameraComponent* camera = GetCameraComponent())
	{
		// 진입시 모드 활성화 Backup
#if PLATFORM_WINDOWS
//#if WITH_EDITOR
//		if (IsPCPlatform())
//		{
//			_backUpSetting.bOverride_DepthOfFieldFstop			= camera->PostProcessSettings.bOverride_DepthOfFieldFstop;
//			_backUpSetting.DepthOfFieldFstop					= camera->PostProcessSettings.DepthOfFieldFstop;
//			_backUpSetting.bOverride_DepthOfFieldFocalDistance	= camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance;
//			_backUpSetting.DepthOfFieldFocalDistance			= camera->PostProcessSettings.DepthOfFieldFocalDistance;
//		}
//		else
//		{
			_backUpSetting.bOverride_DepthOfFieldFocalRegion	= camera->PostProcessSettings.bOverride_DepthOfFieldFocalRegion;
			_backUpSetting.DepthOfFieldFocalRegion				= camera->PostProcessSettings.DepthOfFieldFocalRegion;
			_backUpSetting.DepthOfFieldScale					= camera->PostProcessSettings.DepthOfFieldScale;


			// https://jira.com2us.com/jira/browse/C2URWQ-3426
			// editor free cam mode dof -> dialog dof
			_backUpSetting.bOverride_DepthOfFieldFstop			= camera->PostProcessSettings.bOverride_DepthOfFieldFstop;
			_backUpSetting.bOverride_DepthOfFieldFocalDistance	= camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance;


			camera->PostProcessSettings.bOverride_DepthOfFieldFstop = false;
			camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = false;
//		}
//#else
//#else
//		_backUpSetting.bOverride_DepthOfFieldFstop				= camera->PostProcessSettings.bOverride_DepthOfFieldFstop;
//		_backUpSetting.DepthOfFieldFstop						= camera->PostProcessSettings.DepthOfFieldFstop;
//		_backUpSetting.bOverride_DepthOfFieldFocalDistance		= camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance;
//		_backUpSetting.DepthOfFieldFocalDistance				= camera->PostProcessSettings.DepthOfFieldFocalDistance;
#endif
#if PLATFORM_ANDROID || PLATFORM_IOS			
		//Mobile
		_backUpSetting.bOverride_DepthOfFieldFocalRegion		= camera->PostProcessSettings.bOverride_DepthOfFieldFocalRegion;
		_backUpSetting.DepthOfFieldFocalRegion					= camera->PostProcessSettings.DepthOfFieldFocalRegion;
		_backUpSetting.DepthOfFieldScale						= camera->PostProcessSettings.DepthOfFieldScale;
#endif
	}

}

void FGsCameraModeDialog::Exit()
{
	// 탈출시 이전 세팅 복원
	if (UCameraComponent* camera = GetCameraComponent())
	{
		// 진입시 모드 활성화 Backup
#if PLATFORM_WINDOWS
//#if WITH_EDITOR
//		if (IsPCPlatform())
//		{
//			camera->PostProcessSettings.bOverride_DepthOfFieldFstop			= _backUpSetting.bOverride_DepthOfFieldFstop;
//			camera->PostProcessSettings.DepthOfFieldFstop					= _backUpSetting.DepthOfFieldFstop;
//			camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = _backUpSetting.bOverride_DepthOfFieldFocalDistance;
//			camera->PostProcessSettings.DepthOfFieldFocalDistance			= _backUpSetting.DepthOfFieldFocalDistance;
//		}
//		else
//		{
			camera->PostProcessSettings.bOverride_DepthOfFieldFocalRegion	= _backUpSetting.bOverride_DepthOfFieldFocalRegion;
			camera->PostProcessSettings.DepthOfFieldFocalRegion				= _backUpSetting.DepthOfFieldFocalRegion;
			camera->PostProcessSettings.DepthOfFieldScale					= _backUpSetting.DepthOfFieldScale;

			// https://jira.com2us.com/jira/browse/C2URWQ-3426
			// editor free cam mode dof -> dialog dof
			camera->PostProcessSettings.bOverride_DepthOfFieldFstop = _backUpSetting.bOverride_DepthOfFieldFstop;
			camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = _backUpSetting.bOverride_DepthOfFieldFocalDistance;

//		}
//#else
//#endif
//		camera->PostProcessSettings.bOverride_DepthOfFieldFstop			= _backUpSetting.bOverride_DepthOfFieldFstop;
//		camera->PostProcessSettings.DepthOfFieldFstop					= _backUpSetting.DepthOfFieldFstop;
//		camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = _backUpSetting.bOverride_DepthOfFieldFocalDistance;
//		camera->PostProcessSettings.DepthOfFieldFocalDistance			= _backUpSetting.DepthOfFieldFocalDistance;
#endif
#if PLATFORM_ANDROID || PLATFORM_IOS	
		camera->PostProcessSettings.bOverride_DepthOfFieldFocalRegion	= _backUpSetting.bOverride_DepthOfFieldFocalRegion;
		camera->PostProcessSettings.DepthOfFieldFocalRegion				= _backUpSetting.DepthOfFieldFocalRegion;
		camera->PostProcessSettings.DepthOfFieldScale					= _backUpSetting.DepthOfFieldScale;
#endif
	}
	Super::Exit();
}

// 인풋에 의한 모드 변환
void FGsCameraModeDialog::ChangeModeNormal(const FGsSchemaCameraModeData* In_modeData)
{
	Super::ChangeModeNormal(In_modeData);
	if (FGsCameraRotationBase* rotationWork = GetCameraRotationBase())
	{
		rotationWork->SetRotationType(EGsCameraRotationType::WorldRotation);
	}
	SetModeRot(In_modeData);

	FGsCameraRotationDialogCutLoop* rotationDialogCutLoop = GetCameraDialogCutLoop();
	if (rotationDialogCutLoop == nullptr)
	{
		return;
	}
	rotationDialogCutLoop->StopRotation();
}

// 모드 회전 저장 하기
void FGsCameraModeDialog::SetModeRot(const FGsSchemaCameraModeData* In_modeData)
{
	if (In_modeData == nullptr ||
		nullptr ==_targetPlayer)
	{
		return;
	}


	_dirtyModeData = In_modeData;
	_isDirtyNextTickViewSet = true;
}
// 보간 끝(대사)
void FGsCameraModeDialog::OnFinishLerpMode()
{
	SetupCameraDOF();
	// 바로 요청하면 local object update에서 호출이라서
	// 스폰시 list update중간에 갯수 증가-> 에러
	//GMessage()->GetGameObject().SendMessage(MessageGameObject::CAMERA_DIALOG_START_LERP_END, nullptr);

	FGsCameraModeManager* cameraModeManager = GSCameraMode();
	if (cameraModeManager == nullptr)
	{
		return;
	}

	cameraModeManager->SetIsDialogLerpEnd(true);

	SetDialogCutLoop();

}


// 카메라 위치 처리 접근(대사)
FGsCameraLocationDialog* FGsCameraModeDialog::GetCameraLocationDialog()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::LocationWork);
	if (work == nullptr)
	{
		return nullptr;
	}

	FGsCameraLocationDialog* locationWork = FGsClassUtil::FCheckStaticCast<FGsCameraLocationDialog>(work);
	return locationWork;
}

// 카메라 회전 처리 접근(대사)
FGsCameraRotationDialog* FGsCameraModeDialog::GetCameraRotationDialog()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::RotationWork);
	if (work == nullptr)
	{
		return nullptr;
	}

	FGsCameraRotationDialog* rotationWork = FGsClassUtil::FCheckStaticCast<FGsCameraRotationDialog>(work);
	return rotationWork;
}

// camera zoom (dialog)
FGsCameraZoomDialog* FGsCameraModeDialog::GetCameraZoomDialog()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::ZoomWork);
	if (work == nullptr)
	{
		return nullptr;
	}

	FGsCameraZoomDialog* zoomWork = FGsClassUtil::FCheckStaticCast<FGsCameraZoomDialog>(work);
	return zoomWork;
}
// camera cut loop (dialog)
FGsCameraRotationDialogCutLoop* FGsCameraModeDialog::GetCameraDialogCutLoop()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::DialogCutLoopRotationWork);
	if (work == nullptr)
	{
		return nullptr;
	}

	FGsCameraRotationDialogCutLoop* rotationDialogCutLoopWork = FGsClassUtil::FCheckStaticCast<FGsCameraRotationDialogCutLoop>(work);
	return rotationDialogCutLoopWork;
}
// 모드 뷰 변경
void FGsCameraModeDialog::ChangeModeView()
{
	if (nullptr == _targetPlayer)
	{
		return;
	}

	// 대화 대상 위치
	FVector dialogTargetPos = FVector::ZeroVector;
	TArray<UGsGameObjectBase*> targetObjs = _handler->GetDialogTargets();

	FGsCameraModeManager* cameraModeManager = GSCameraMode();
	if (cameraModeManager == nullptr)
	{
		return;
	}

	if (cameraModeManager->GetDialogCameraViewType() == EGsDialogCameraViewType::DialogBackLocalMultiNpc)
	{
		if (1 < targetObjs.Num())
		{
			if (targetObjs[0] == nullptr ||
				targetObjs[1] == nullptr)
			{
				return;
			}

			dialogTargetPos =
				FMath::Lerp(targetObjs[0]->GetLocation(),
					targetObjs[1]->GetLocation(), 0.5f);
		}
	}
	else
	{
		if (0 < targetObjs.Num())
		{
			if (targetObjs[0] == nullptr)
			{
				return;
			}
			dialogTargetPos = targetObjs[0]->GetLocation();
		}
	}


	// 카메라 parent 위치
	FVector nowMyPos = _targetPlayer->GetLocation();
	
	FVector hitPos = FVector::ZeroVector;
	FRotator controlRotTo = FRotator::ZeroRotator;
	FVector lookTargetPos = FVector::ZeroVector;

	const FGsSchemaCameraModeData* currModeData =
		cameraModeManager->GetCurrCamModeData();


	float lastCamDist = 0.0f;
	bool isHit = false;
	cameraModeManager->GetDialogRotAndPos(
		_targetPlayer,
		targetObjs[0],
		currModeData,
		dialogTargetPos,
		nowMyPos,
		hitPos,
		controlRotTo,
		lookTargetPos,
		lastCamDist,
		isHit);


	cameraModeManager->SetBackupDialogLookPos(lookTargetPos);
	cameraModeManager->SetBackupDialogCamDist(lastCamDist);
	
	FGsCameraRotationBase* rotationWork = GetCameraRotationBase();
	FGsCameraLocationGame* locationWork = GetCameraLocationGame();

	if (rotationWork == nullptr ||
		locationWork == nullptr)
	{
		return;
	}

	locationWork->SetLocationSpace(EGsCameraLocationSpaceType::WorldLocation);

	rotationWork->SetRotation(controlRotTo);
	locationWork->SetDirectPos(hitPos);

	StartViewDataFOV(true);
	SetupCameraDOF();

	_isUseDirectRotToBackup = true;
	_directRotToBackup = controlRotTo;
	SetDialogCutLoop();
}
// fov 변환 시작(대사는 모드fov가 아니라 view 데이터 참조)
void FGsCameraModeDialog::StartModeFOV()
{
	if (_isDirtyNextTickViewSet == true)
	{
		return;
	}	

	FGsCameraModeManager* cameraModeManager = GSCameraMode();
	if (cameraModeManager == nullptr)
	{
		return;
	}

	// 컷 페이드 사용하면 lerp 사용안하고 바로 세팅
	bool isDirect = (cameraModeManager->GetIsUseCutFadeOut() == true) ? true : false;
	StartViewDataFOV(isDirect);
}

// 뷰 데이터 FOV로 변경
void FGsCameraModeDialog::StartViewDataFOV(bool In_isDirect)
{
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}	

	if (localChar->Controller == nullptr ||
		localChar->Controller->IsLocalPlayerController() == false)
	{
		return;
	}

	APlayerController* const PC =
		CastChecked<APlayerController>(localChar->Controller);

	if (nullptr == PC)
	{
		return;
	}

	if (nullptr == PC->PlayerCameraManager)
	{
		return;
	}

	FGsCameraModeManager* cameraModeManager = GSCameraMode();
	if (cameraModeManager == nullptr)
	{
		return;
	}

	const FGsSchemaCameraDialogViewData* viewData =
		cameraModeManager->GetCurrentViewData();

	if (viewData == nullptr)
	{
		return;
	}

	float newViewFOV = viewData->FOV;
	float oldFOV = PC->PlayerCameraManager->GetFOVAngle();

	// 같으면 할필요 없음
	if (newViewFOV == oldFOV)
	{
		return;
	}

	if (FGsCameraFOVGame* fovWork = GetCameraFOVGame())
	{
		if (In_isDirect == true)
		{
			fovWork->SetDirectFOV(newViewFOV);
		}
		else
		{			
			fovWork->StartModeFOV( oldFOV, newViewFOV);
			// 이전에 세팅한 속도로 세팅(이동, 회전공용값)
			fovWork->SetNowSpeed(_currentWorkSpeed);
		}
	}
}

// pitch min max 초기화
void FGsCameraModeDialog::InitPitchMinMax(APlayerController* In_playerController,
	const FGsSchemaCameraModeData* In_modeData)
{
	if (In_playerController == nullptr)
	{
		return;
	}

	// 대화 카메라에선 pitch 제한을 없애자(초기화시 넣는 최대치로)
	In_playerController->PlayerCameraManager->ViewPitchMin = -89.9f;
	In_playerController->PlayerCameraManager->ViewPitchMax = 89.9f;
}

void FGsCameraModeDialog::Update(float In_deltaTime)
{
	Super::Update(In_deltaTime);

	if (_isDirtyNextTickViewSet == true)
	{
		_isDirtyNextTickViewSet = false;
		NextTickViewSet();
	}
	
}

// 모드 초기화(데이터들)
void FGsCameraModeDialog::InitMode(
	AGsCharacterLocalPlayer* In_localChar, const FGsSchemaCameraModeData* In_modeData)
{
	Super::InitMode(In_localChar, In_modeData);

	// dof 사용 안하게 처리
	OnReachZoomMin(false);
}

// 다음틱 뷰 세팅
void FGsCameraModeDialog::NextTickViewSet()
{
	FGsCameraRotationBase* rotationWork = GetCameraRotationBase();
	FGsCameraLocationGame* locationWork = GetCameraLocationGame();

	if (rotationWork == nullptr ||
		locationWork == nullptr)
	{
		return;
	}
	locationWork->SetLocationSpace(EGsCameraLocationSpaceType::WorldLocation);

	if (_dirtyModeData == nullptr ||
		_targetPlayer == nullptr)
	{
		return;
	}

	// 캐릭터 방향에 맞춘다
	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr ||
		localChar->Controller == nullptr ||
		localChar->Controller->IsLocalPlayerController() == false ||
		nullptr == _handler)
	{
		return;
	}

	// 대화 대상 위치
	FVector dialogTargetPos = FVector::ZeroVector;
	TArray<UGsGameObjectBase*> targetObjs = _handler->GetDialogTargets();

	FGsCameraModeManager* cameraModeManager = GSCameraMode();
	if (cameraModeManager == nullptr)
	{
		return;
	}

	if (cameraModeManager->GetDialogCameraViewType() == EGsDialogCameraViewType::DialogBackLocalMultiNpc)
	{
		if (1 < targetObjs.Num())
		{
			dialogTargetPos =
				FMath::Lerp(targetObjs[0]->GetLocation(),
					targetObjs[1]->GetLocation(), 0.5f);
		}
	}
	else
	{
		if (0 < targetObjs.Num())
		{
			dialogTargetPos = targetObjs[0]->GetLocation();
		}
	}


	// 카메라 parent 위치
	FVector nowMyPos = _targetPlayer->GetLocation();
	UWorld* world = _targetPlayer->GetWorld();

	FVector camLoc = FVector::ZeroVector;
	FRotator camRot = FRotator::ZeroRotator;
	if (APlayerController* controller = UGameplayStatics::GetPlayerController(world, 0))
	{
		APlayerCameraManager* camManager = controller->PlayerCameraManager;
		camLoc = camManager->GetCameraLocation();
		camRot = camManager->GetCameraRotation();
	}
	FVector hitPos = FVector::ZeroVector;
	FRotator controlRotTo = FRotator::ZeroRotator;
	FVector lookTargetPos = FVector::ZeroVector;
	float lastCamDist = 0.0f;
	bool isHit = false;
	cameraModeManager->GetDialogRotAndPos(
		_targetPlayer,
		targetObjs[0],
		_dirtyModeData,
		dialogTargetPos,
		nowMyPos,
		hitPos,
		controlRotTo,
		lookTargetPos,
		lastCamDist,
		isHit);


	// 컷 페이드 사용하면 lerp 사용안하고 바로 세팅
	if (cameraModeManager->GetIsUseCutFadeOut() == true)
	{
		// 나중 돌아갈때 용 백업
		cameraModeManager->SetBackupDialogLookPos(lookTargetPos);
		cameraModeManager->SetBackupDialogCamDist(lastCamDist);

		// 세팅
		rotationWork->SetRotation(controlRotTo);
		locationWork->SetDirectPos(hitPos);

		_directRotToBackup = controlRotTo;
		_isUseDirectRotToBackup = true;

		// 이동 종료 호출
		OnFinishLerpMode();		
	}
	else
	{
		_isUseDirectRotToBackup = false;

		// check use zoom lerp
		bool isUseDialogLerpZoom = 
			cameraModeManager->GetIsZoomFirstCameraMove();

		FVector camRelativeLocation = localChar->GetFollowCamera()->GetRelativeLocation();
		FVector camOriginPos = localChar->GetSpringArm()->GetComponentLocation();

		FVector camTargetOffset = localChar->GetSpringArm()->TargetOffset;

		FVector camSocketOffset = localChar->GetSpringArm()->SocketOffset;
		
		FVector realNowLookPos = camOriginPos + FRotationMatrix(camRot).TransformVector(camRelativeLocation + camSocketOffset) + camTargetOffset;
			
		float realNowCamDist = FVector::Dist(camLoc, realNowLookPos);

		if (isUseDialogLerpZoom == false)
		{
			// 세팅
			rotationWork->SetRotation(camRot);
			locationWork->SetDirectPos(camLoc);

			rotationWork->SetTargetRot(controlRotTo, camRot);

			// 현재 바라보는 곳 저장(나중에 돌아올때 씀)
			cameraModeManager->SetBackupDialog(
				realNowLookPos,
				lookTargetPos,
				lastCamDist);

			FGsCameraLocationDialog* dialogLocation = GetCameraLocationDialog();

			if (dialogLocation == nullptr)
			{
				return;
			}
			dialogLocation->SetData(lastCamDist, realNowCamDist,
				lookTargetPos,
				realNowLookPos,
				hitPos, camLoc);


			MakeDialogSpeed(locationWork, rotationWork);
		}
		// first zoom -> next rotation and location move
		else
		{
			// 세팅
			rotationWork->SetRotation(camRot);
			locationWork->SetDirectPos(camLoc);

			rotationWork->SetTargetRot(controlRotTo, camRot, false);

			FVector camRotVector = camRot.Vector();

			// 현재 바라보는 곳 저장(나중에 돌아올때 씀)
			cameraModeManager->SetBackupDialog(
				realNowLookPos,
				lookTargetPos,
				lastCamDist);

			FGsCameraLocationDialog* dialogLocation = GetCameraLocationDialog();

			if (dialogLocation == nullptr)
			{
				return;
			}

			// zoom finished camera pos
			float diffCamDist = realNowCamDist - lastCamDist;
			FVector zoomFinshedCamPos = camLoc + diffCamDist * camRotVector;

			// zoom finished look pos
			FVector zoomFinishedLookPos = zoomFinshedCamPos + lastCamDist * camRotVector;

			dialogLocation->SetData(lastCamDist, lastCamDist,
				lookTargetPos,
				zoomFinishedLookPos,
				hitPos, zoomFinshedCamPos, false);


			MakeDialogSpeed(locationWork, rotationWork);

			FGsCameraZoomDialog* dialogZoom = GetCameraZoomDialog();
			if (dialogZoom == nullptr)
			{
				return;
			}

			dialogZoom->PosZoomStart(diffCamDist, camLoc, camRotVector);
		}
	}


	// 컷 페이드 사용하면 lerp 사용안하고 바로 세팅
	bool isDirect = (cameraModeManager->GetIsUseCutFadeOut() == true) ? true : false;
	StartViewDataFOV(isDirect);
}

// zoom finish call back
// -> dialog rotation and location start
void FGsCameraModeDialog::OnZoomFinished()
{
	FGsCameraRotationBase* rotationWork = GetCameraRotationBase();
	FGsCameraLocationDialog* dialogLocation = GetCameraLocationDialog();

	if (rotationWork == nullptr ||
		dialogLocation == nullptr)
	{
		return;
	}
	rotationWork->SetIsFinish(false);
	dialogLocation->SetIsFinish(false);
}

void FGsCameraModeDialog::SetDialogCutLoop()
{
	FGsCameraModeManager* cameraModeManager = GSCameraMode();

	if (cameraModeManager == nullptr)
	{
		return;
	}
	const FGsSchemaCameraDialogViewData* viewData = cameraModeManager->GetCurrentViewData();
	if (nullptr == viewData)
	{
		return;
	}

	FGsCameraRotationDialogCutLoop* rotationDialogCutLoop =  GetCameraDialogCutLoop();
	if (rotationDialogCutLoop == nullptr)
	{
		return;
	}
	if (viewData->isCameraMoveable == true)
	{
		FRotator camRot = FRotator::ZeroRotator;

		if (_isUseDirectRotToBackup == true)
		{
			camRot = _directRotToBackup;
		}
		else
		{
			if (_targetPlayer == nullptr)
			{
				return;
			}

			UWorld* world = _targetPlayer->GetWorld();
			if (world == nullptr)
			{
				return;
			}

			FVector camLoc = FVector::ZeroVector;			
			if (APlayerController* controller = UGameplayStatics::GetPlayerController(world, 0))
			{
				if(APlayerCameraManager* camManager = controller->PlayerCameraManager)
				{ 
					camLoc = camManager->GetCameraLocation();
					camRot = camManager->GetCameraRotation();
				}
			}
		}

		rotationDialogCutLoop->StartRotation(camRot);
	}
	else
	{
		rotationDialogCutLoop->StopRotation();
	}
}