#include "GsCameraModeManager.h"

#include "Camera/Define/EGsDialogCameraViewType.h"

#include "Classes/Engine/DataTable.h"
#include "Engine/Public/DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Management/GsMessageholder.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsObserverManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"

#include "Message/GsMessageGameObject.h"

#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Camera/Mode/GsCameraModeControlBase.h"
#include "GameObject/Camera/Mode/GsCameraModeBackView.h"
#include "GameObject/Camera/Mode/GsCameraModeFixedQuarter.h"
#include "GameObject/Camera/Work/Rotation/GsCameraRotationBase.h"
#include "GameObject/Camera/Work/Zoom/GsCameraZoomGame.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

#include "DataSchema/Camera/GsSchemaCameraModeData.h"
#include "DataSchema/Camera/GsSchemaCameraDialogViewData.h"
#include "DataSchema/GsSchemaEnums.h"

#include "Option/GsGameUserSettings.h"

#include "ControllerEx/GsPlayerController.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsText.h"

#include "Core/Public/Misc/DateTime.h"

#include "Engine/Classes/Camera/PlayerCameraManager.h"

//-------------------------------------------------
// 카메라 모드 관리자
//-------------------------------------------------
FGsCameraModeManager::~FGsCameraModeManager()
{
	//GSLOG(Warning, TEXT("FGsCameraModeManager destructor"));
	
}

// 초기화
void FGsCameraModeManager::Initialize()
{
	SaveCameraTargetBoneHeight();

	// 백뷰, 쿼터, 고정 쿼터만 수동 입력시 저장 가능
	if (_arraySavePossibleMode.Num() == 0)
	{
		_arraySavePossibleMode.Add(EGsCameraMode::BackView);
		_arraySavePossibleMode.Add(EGsCameraMode::Quarter);
		_arraySavePossibleMode.Add(EGsCameraMode::FixedQuarter);
	}


	_currentMode =
		GData()->GetGlobalData()->_camBaseMode;
	_backupMode = 
		GData()->GetGlobalData()->_camBaseMode;

	_isGreetingView = true;
	// 기존꺼 날림
	ClearBackupData();

	// 초기화
	_isCameraDialogPlay = false;

	_headBoneName = FName("Bip001-Head");

	// 백업 데이터 불러오기
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		float findIsBackup = gameUserSettings->GetCameraModeBackupData(EGsOptionCameraModeBackup::IS_BACKUP);
		bool isBackUp = (findIsBackup == 0.0f || findIsBackup == -1.0f) ? false : true;

		// 백업된게 있다면 가져오기
		if (isBackUp == true)
		{
			_isBackupUserSetting = true;

			int findCamMode = (int)gameUserSettings->GetCameraModeBackupData(EGsOptionCameraModeBackup::CAM_MODE);

			EGsCameraMode findCamModeEnum = (EGsCameraMode)findCamMode;
			// 저장 가능 모드가 아니면 저장 안된걸로 치자(잘못 저장되었음)
			if (_arraySavePossibleMode.Contains(findCamModeEnum) == false)
			{
				SaveUseDefaultZoomAndPitch();
				_isBackupUserSetting = false;
				return;
			}

			_currentMode = findCamModeEnum;
			_backupMode = _currentMode;

			// 모드별 줌값 로드
			float findZoomVal1 = gameUserSettings->GetCameraModeBackupData(EGsOptionCameraModeBackup::ZOOM_VAL_1);

			// 기존 값이 없으면 기본 값 로드
			if (findZoomVal1 == 0.0f)
			{
				const FGsSchemaCameraModeData* camModeData = GetCamModeData(EGsCameraMode::BackView);
				if (nullptr != camModeData)
				{
					findZoomVal1 = camModeData->ArmLengthTo;
				}
			}			
			_dicCamBackupZoom.Add(EGsCameraMode::BackView, findZoomVal1);

			float findZoomVal2 = gameUserSettings->GetCameraModeBackupData(EGsOptionCameraModeBackup::ZOOM_VAL_2);

			// 기존 값이 없으면 기본 값 로드
			if (findZoomVal2 == 0.0f)
			{
				const FGsSchemaCameraModeData* camModeData = GetCamModeData(EGsCameraMode::Quarter);
				if (nullptr != camModeData)
				{
					findZoomVal2 = camModeData->ArmLengthTo;
				}
			}			
			_dicCamBackupZoom.Add(EGsCameraMode::Quarter, findZoomVal2);

			float findZoomVal3 = gameUserSettings->GetCameraModeBackupData(EGsOptionCameraModeBackup::ZOOM_VAL_3);

			// 기존 값이 없으면 기본 값 로드
			if (findZoomVal3 == 0.0f)
			{
				const FGsSchemaCameraModeData* camModeData = GetCamModeData(EGsCameraMode::FixedQuarter);
				if (nullptr != camModeData)
				{
					findZoomVal3 = camModeData->ArmLengthTo;
				}
			}
			_dicCamBackupZoom.Add(EGsCameraMode::FixedQuarter, findZoomVal3);

			// 모드별 PITCH 값 로드
			float findPitchVal1 = gameUserSettings->GetCameraModeBackupData(EGsOptionCameraModeBackup::PITCH_VAL_1);

			// 기존 값이 없으면 기본 값 로드
			if (findPitchVal1 == 0.0f)
			{
				const FGsSchemaCameraModeData* camModeData = GetCamModeData(EGsCameraMode::BackView);
				if (nullptr != camModeData)
				{
					findPitchVal1 = camModeData->ControllerPitch;
				}
			}
			_dicCamBackupPitch.Add(EGsCameraMode::BackView, findPitchVal1);

			float findPitchVal2 = gameUserSettings->GetCameraModeBackupData(EGsOptionCameraModeBackup::PITCH_VAL_2);

			// 기존 값이 없으면 기본 값 로드
			if (findPitchVal2 == 0.0f)
			{
				const FGsSchemaCameraModeData* camModeData = GetCamModeData(EGsCameraMode::Quarter);
				if (nullptr != camModeData)
				{
					findPitchVal2 = camModeData->ControllerPitch;
				}
			}
			_dicCamBackupPitch.Add(EGsCameraMode::Quarter, findPitchVal2);

			float findPitchVal3 = gameUserSettings->GetCameraModeBackupData(EGsOptionCameraModeBackup::PITCH_VAL_3);

			// 기존 값이 없으면 기본 값 로드
			if (findPitchVal3 == 0.0f)
			{
				const FGsSchemaCameraModeData* camModeData = GetCamModeData(EGsCameraMode::FixedQuarter);
				if (nullptr != camModeData)
				{
					findPitchVal3 = camModeData->ControllerPitch;
				}
			}			
			_dicCamBackupPitch.Add(EGsCameraMode::FixedQuarter, findPitchVal3);

			float findQuatX = gameUserSettings->GetCameraModeBackupData(EGsOptionCameraModeBackup::QUAT_X);
			float findQuatY = gameUserSettings->GetCameraModeBackupData(EGsOptionCameraModeBackup::QUAT_Y);
			float findQuatZ = gameUserSettings->GetCameraModeBackupData(EGsOptionCameraModeBackup::QUAT_Z);
			float findQuatW = gameUserSettings->GetCameraModeBackupData(EGsOptionCameraModeBackup::QUAT_W);

			_backupQuat.X = findQuatX;
			_backupQuat.Y = findQuatY;
			_backupQuat.Z = findQuatZ;
			_backupQuat.W = findQuatW;

			int findFixedQuarterYawType = (int)gameUserSettings->GetCameraModeBackupData(EGsOptionCameraModeBackup::FIXED_QUARTER_YAW_TYPE);

			EGsCameraFixedQuarterYawType findFixedQuarterEnum = (EGsCameraFixedQuarterYawType)findFixedQuarterYawType;
			_fixedQuarterYawType = findFixedQuarterEnum;

		}
		// 없으면 백업 데이터를 현재 기준으로 데이터를 만든다
		else
		{
			SaveUseDefaultZoomAndPitch();		
		}
	}
	_cameraShoulderFollowCoolTimeTick = GData()->GetGlobalData()->_cameraShoulderFollowCoolTime * ETimespan::TicksPerSecond;
}
// 기본 zoom pitch 사용
void FGsCameraModeManager::SaveUseDefaultZoomAndPitch()
{
	// 모드별 줌값 기본값(테이블 값)
	// 모드별 pitch 값 기본값(테이블 값)
	float findZoomVal1 = 0.0f;
	float findPitchVal1 = 0.0f;
	const FGsSchemaCameraModeData* camModeDataBackView = GetCamModeData(EGsCameraMode::BackView);
	if (nullptr != camModeDataBackView)
	{
		findZoomVal1 = camModeDataBackView->ArmLengthTo;
		findPitchVal1 = camModeDataBackView->ControllerPitch;
	}
	_dicCamBackupZoom.Add(EGsCameraMode::BackView, findZoomVal1);
	_dicCamBackupPitch.Add(EGsCameraMode::BackView, findPitchVal1);

	float findZoomVal2 = 0.0f;
	float findPitchVal2 = 0.0f;
	const FGsSchemaCameraModeData* camModeDataQuarter = GetCamModeData(EGsCameraMode::Quarter);
	if (nullptr != camModeDataQuarter)
	{
		findZoomVal2 = camModeDataQuarter->ArmLengthTo;
		findPitchVal2 = camModeDataQuarter->ControllerPitch;
	}
	_dicCamBackupZoom.Add(EGsCameraMode::Quarter, findZoomVal2);
	_dicCamBackupPitch.Add(EGsCameraMode::Quarter, findPitchVal2);

	float findZoomVal3 = 0.0f;
	float findPitchVal3 = 0.0f;
	const FGsSchemaCameraModeData* camModeDataFixedQuarter = GetCamModeData(EGsCameraMode::FixedQuarter);
	if (nullptr != camModeDataFixedQuarter)
	{
		findZoomVal3 = camModeDataFixedQuarter->ArmLengthTo;
		findPitchVal3 = camModeDataFixedQuarter->ControllerPitch;
	}
	_dicCamBackupZoom.Add(EGsCameraMode::FixedQuarter, findZoomVal3);
	_dicCamBackupPitch.Add(EGsCameraMode::FixedQuarter, findPitchVal3);
}
// load zoom and pitch values to use in observer mode
void FGsCameraModeManager::LoadObserverCamModeZoomAndPitch()
{
	if (_dicObserverCamBackupZoom.Num() != 0)
	{
		_dicObserverCamBackupZoom.Empty();
	}
	if (_dicObserverCamBackupPitch.Num() != 0)
	{
		_dicObserverCamBackupPitch.Empty();
	}

	// 모드별 줌값 기본값(테이블 값)
	// 모드별 pitch 값 기본값(테이블 값)
	float findZoomVal1 = 0.0f;
	float findPitchVal1 = 0.0f;
	const FGsSchemaCameraModeData* camModeDataBackView = GetCamModeData(EGsCameraMode::BackView);
	if (nullptr != camModeDataBackView)
	{
		findZoomVal1 = camModeDataBackView->ArmLengthTo;
		findPitchVal1 = camModeDataBackView->ControllerPitch;
	}
	_dicObserverCamBackupZoom.Add(EGsCameraMode::BackView, findZoomVal1);
	_dicObserverCamBackupPitch.Add(EGsCameraMode::BackView, findPitchVal1);

	float findZoomVal2 = 0.0f;
	float findPitchVal2 = 0.0f;
	const FGsSchemaCameraModeData* camModeDataQuarter = GetCamModeData(EGsCameraMode::Quarter);
	if (nullptr != camModeDataQuarter)
	{
		findZoomVal2 = camModeDataQuarter->ArmLengthTo;
		findPitchVal2 = camModeDataQuarter->ControllerPitch;
	}
	_dicObserverCamBackupZoom.Add(EGsCameraMode::Quarter, findZoomVal2);
	_dicObserverCamBackupPitch.Add(EGsCameraMode::Quarter, findPitchVal2);

	float findZoomVal3 = 0.0f;
	float findPitchVal3 = 0.0f;
	const FGsSchemaCameraModeData* camModeDataFixedQuarter = GetCamModeData(EGsCameraMode::FixedQuarter);
	if (nullptr != camModeDataFixedQuarter)
	{
		findZoomVal3 = camModeDataFixedQuarter->ArmLengthTo;
		findPitchVal3 = camModeDataFixedQuarter->ControllerPitch;
	}
	_dicObserverCamBackupZoom.Add(EGsCameraMode::FixedQuarter, findZoomVal3);
	_dicObserverCamBackupPitch.Add(EGsCameraMode::FixedQuarter, findPitchVal3);
}
void FGsCameraModeManager::Finalize()
{
	// 저장하기
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		gameUserSettings->SaveCameraModeBackupSetting(_backupMode, _dicCamBackupZoom,
			_dicCamBackupPitch,
			_backupQuat, 
			_fixedQuarterYawType);
	}
}
// 캐릭터 세팅
// 2. 백업데이터를 지울지(시공의 틈새에선 안지운다)
void FGsCameraModeManager::SetCharacter(UGsGameObjectLocalPlayer* In_char
	, bool In_isClearBackup)
{
	_localPlayer = In_char;
	// 사망후 저장값 처리후 클리어
	//
	// 스폰 상태는 freeze 때문에 안됨(무비로 시작시)
	if (In_isClearBackup == true)
	{
		ClearBackupData();
	}
	_isInitCharacter = true;

	// 초기화
	_isCameraDialogPlay = false;
}
// 캐릭터 삭제될때(상태들 가지고 있던 캐릭터 null 처리 해줘야함)
void FGsCameraModeManager::RemoveCharacter()
{	

	_isInitCharacter = false;

	// 이때 카메라 최근값을 저장한다
	// 여기서 백업했었는데
	// 워프 연출이 생겨서
	// warp 상태면 warp enter에서 백업
	// dead 상태면 dead enter에서 백업
	_localPlayer = nullptr;
}
// 되돌릴 tr 저장(줌, pitch 회전(위,아래))
void FGsCameraModeManager::SaveBackUpTransform()
{
	if (_localPlayer == nullptr)
	{
		return;
	}

	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}


	FVector currCamLocation = FVector::ZeroVector;
	FRotator currCamRotation = FRotator::ZeroRotator;

	if (AGsPlayerController* controller = localChar->GetPlayerController())
	{
		APlayerCameraManager* camManager = controller->PlayerCameraManager;
		if (camManager != nullptr)
		{			
			currCamLocation = camManager->GetCameraLocation();				
			currCamRotation = camManager->GetCameraRotation();			
		}
	}

	// 1. check normal mode change possible
	if (_currentMode == EGsCameraMode::BackView ||
		_currentMode == EGsCameraMode::FixedQuarter ||
		_currentMode == EGsCameraMode::Quarter)
	{
		FGsCameraModeHandler* handler = _localPlayer->GetCameraModeHandler();
		if (nullptr == handler)
		{
			return;
		}

		TSharedRef<FGsCameraModeBase> currMode = handler->GetState();

		// 2. check now zoom changing , rotation changing
		FGsCameraModeControlBase* controlMode =
			static_cast<FGsCameraModeControlBase*>(&currMode.Get());


		if (controlMode != nullptr)
		{
			controlMode->PredictTransform(currCamRotation, currCamLocation);
		}
	}

	
	// 카메라 값 저장
	_backupCamLocation = currCamLocation;
	_backupCamRotation = currCamRotation;

	// flag on
	_isBackUpTransform = true;	

	SaveBackUpData();
}

// 줌, 회전, diff rot to(back view), 모드
void FGsCameraModeManager::SaveBackUpData()
{

	if (_localPlayer == nullptr)
	{
		return;
	}

	FGsCameraModeHandler* handler = _localPlayer->GetCameraModeHandler();
	if (nullptr == handler)
	{
		return;
	}

	TSharedRef<FGsCameraModeBase> currMode = handler->GetState();

	// 모드 저장
	_backupMode = _currentMode;

	// 저장 가능한 모드만 저장하자
	EGsCameraMode mode = currMode->GetType();
	if (_dicCamBackupZoom.Contains(mode) == true)
	{
		_dicCamBackupZoom[mode] = currMode->GetCurrentZoom();
	}
	// 현재 회전 구하기(백업시 사용)
	_backupQuat = currMode->GetCurrentQuat();

}
// 갱신
void FGsCameraModeManager::Update(float In_deltaTime)
{
	if (_isInitCharacter == false)
	{
		return;
	}

	if (_isDrawTestHitDialog == true)
	{
		DrawTestHitDialogMode();
	}

	if (_isDialogLerpEnd == true)
	{
		_isDialogLerpEnd = false;
		GMessage()->GetGameObject().SendMessage(MessageGameObject::CAMERA_DIALOG_START_LERP_END, nullptr);
	}
}

// 모드에 따른 카메라 데이터 가져오기
const FGsSchemaCameraModeData* FGsCameraModeManager::GetCamModeData(EGsCameraMode In_mode)
{		
	const UGsTableCameraModeData* tbl =
		Cast<UGsTableCameraModeData>(FGsSchemaCameraModeData::GetStaticTable());

	const FGsSchemaCameraModeData* findTbl = nullptr;
	if (tbl->FindRowById(In_mode, findTbl) == false)
	{
		return nullptr;
	}
	return findTbl;
}

// 현재 모드 카메라 데이터 가져오기
const FGsSchemaCameraModeData* FGsCameraModeManager::GetCurrCamModeData()
{
	return FGsCameraModeManager::GetCamModeData(_currentMode);
}
// 백업 데이터 클리어
void FGsCameraModeManager::ClearBackupData()
{
	_isBackUpTransform = false;
	// 백업 cam zoom은 user data에서도 저장하므로 flag 값만 클리어 하자
}
// 저장 모드로 변경
void FGsCameraModeManager::ChangeToBackupMode()
{
	if (_backupMode == _currentMode)
	{
		return;
	}

	if (nullptr != _localPlayer)
	{
		if (FGsCameraModeHandler* handler = _localPlayer->GetCameraModeHandler())
		{
			// 대화 카메라 모드로 변경
			handler->ChangeState(_backupMode);
		}
	}

	// 백업값 지우기
	ClearBackupData();
}

// 대사 모드 hit 테스트용 그리기
void FGsCameraModeManager::DrawTestHitDialogMode()
{

	// 대화 대상 위치
	FVector dialogTargetPos = FVector::ZeroVector;
	UGsGameObjectBase* targetObj = nullptr;
	if (FGsCameraModeHandler* cameraModehandler = _localPlayer->GetCameraModeHandler())
	{
		TArray<UGsGameObjectBase*> targetObjs = cameraModehandler->GetDialogTargets();

		if (targetObjs.Num() == 0)
		{
			return;
		}
		if (GetDialogCameraViewType() == EGsDialogCameraViewType::DialogBackLocalMultiNpc)
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

			targetObj = targetObjs[0];
		}
	}
	// 로컬 위치
	FVector nowMyPos = _localPlayer->GetLocation();
	EGsDialogCameraViewType viewType = GetDialogCameraViewType();

	
	UWorld* world = _localPlayer->GetWorld();
	if (world == nullptr)
	{
		return;
	}

	FVector hitPos = FVector::ZeroVector;
	FRotator controlRotTo = FRotator::ZeroRotator;
	FVector lookTargetPos = FVector::ZeroVector;
	float lastCamDist = 0.0f;
	bool isHit = false;

	const FGsSchemaCameraModeData* data = GetCamModeData(EGsCameraMode::Dialog);
	
	GetDialogRotAndPos(
		_localPlayer, 
		targetObj,
		data,
		dialogTargetPos,
		nowMyPos,
		hitPos,
		controlRotTo,
		lookTargetPos,
		lastCamDist,
		isHit);

	FVector camLoc = FVector::ZeroVector;
	FRotator camRot = FRotator::ZeroRotator;
	if (APlayerController* controller = UGameplayStatics::GetPlayerController(world, 0))
	{
		APlayerCameraManager* camManager = controller->PlayerCameraManager;
		camLoc = camManager->GetCameraLocation();
		camRot = camManager->GetCameraRotation();
	}

	FColor nowColor = (isHit == true) ? FColor::Red : FColor::Green;
	// 카메라 예상 위치
	DrawDebugCircle(world, hitPos,
		100.0f, 100, nowColor, false, 1.f,
		0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));


	// 바라볼 위치
	DrawDebugCircle(world, lookTargetPos,
		50.0f, 100, FColor::Cyan, false, 1.f,
		0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));

	// 예상위치에 대상 방향(예측값)
	DrawDebugDirectionalArrow(world, hitPos, lookTargetPos, 100.0f, nowColor, false, 1.f);

	FVector camTargetPos =  camLoc + camRot.Vector() * 500.0f;

	// 실제 현재 카메라 방향
	DrawDebugDirectionalArrow(world, camLoc, camTargetPos, 100.0f, FColor::Cyan, false, 1.f);	
}

// 대사 모드 hit 테스트용 그리기 액티브
void FGsCameraModeManager::DrawTestActive(bool In_val)
{
	_isDrawTestHitDialog = In_val;
}
// hit pos 구하기
bool FGsCameraModeManager::GetDialogHitPos(UGsGameObjectBase* In_camParent,
	const FVector& In_farPos,
	const FVector& In_nearPos,
	const FVector& In_lookTargetPos,
	FVector& Out_pos)
{
	Out_pos = FVector::ZeroVector;
	if (In_camParent == nullptr ||
		nullptr == _currDialogViewData)
	{		
		return false;
	}

	bool isHit = false;
	
	// 내위치에 맞춤(언덕같이 높이가 다를수 있어서)
	FVector rootOffset = _currDialogViewData->rootOffset;

	FVector calcMyPos = In_nearPos + rootOffset;

	// 대화 대상과 나사이 중간 위치(바라볼 위치)
	FVector dialogCenterPos =
		In_lookTargetPos;
	// 로컬에서 대화 대상 바라보는 방향
	FVector currlookVec =
		dialogCenterPos - calcMyPos;

	currlookVec.Normalize();

	float armLength = _currDialogViewData->armLength;
	FVector shiftOffset = _currDialogViewData->shiftOffset;

	// 오프셋 이동할값
	FVector calcOffset =
		FRotationMatrix(currlookVec.Rotation()).TransformVector(shiftOffset);

	// 로컬에서 오프셋 더한값
	FVector moveLocalPos = In_nearPos + calcOffset - armLength * currlookVec;

	// 액터 타겟 바라본 방향의 카메라에서 타겟 바라볼 방향
	FVector oldCamLookVec = dialogCenterPos - moveLocalPos;

	oldCamLookVec.Normalize();

	// 오프셋 이동할값(최종)
	FVector lastCalcOffset =
		FRotationMatrix(oldCamLookVec.Rotation()).TransformVector(shiftOffset);

	// 로컬에서 오프셋 더한값(최종)
	FVector lastmoveLocalPos = 
		In_nearPos + lastCalcOffset - armLength * oldCamLookVec;

	Out_pos = lastmoveLocalPos;
	isHit = GetHitPos(In_camParent, In_nearPos, lastmoveLocalPos, Out_pos);		
	return isHit;
}


// side hit pos 구하기(DialogSide)
bool FGsCameraModeManager::GetDialogHitPosSide(UGsGameObjectBase* In_camParent,
	const FVector& In_targetPos,
	const FVector& In_localPos,
	const FVector& In_lookTargetPos,
	FVector& Out_pos)
{
	Out_pos = FVector::ZeroVector;
	if (In_camParent == nullptr ||
		nullptr == _currDialogViewData)
	{
		return false;
	}

	bool isHit = false;
	
	FVector rootOffset = _currDialogViewData->rootOffset;
	FVector calcMyPos = In_localPos + rootOffset;

	// 바라 보는 방향
	FVector currlookVec =
		In_targetPos - calcMyPos;

	currlookVec.Normalize();
	// 90도 회전
	FVector sideDir = currlookVec.RotateAngleAxis(90.0f, FVector::UpVector);

	float armLength = _currDialogViewData->armLength;

	// 대화 대상과 나사이 중간 위치(바라볼 위치)
	FVector dialogCenterPos =
		In_lookTargetPos;
	FVector movePos = dialogCenterPos - sideDir * armLength;

	Out_pos = movePos;
	isHit = GetHitPos(In_camParent, dialogCenterPos, movePos, Out_pos);
	return isHit;
}
// greeting hit pos 구하기(DialogGreetingLocal, DialogGreetingNPC)
bool FGsCameraModeManager::GetDialogHitPosGreeting(UGsGameObjectBase* In_camParent,
	const FVector& In_lookTargetPos,
	const FVector& In_forwardVector,
	FVector& Out_pos)
{
	Out_pos = FVector::ZeroVector;
	if (In_camParent == nullptr ||
		nullptr == _currDialogViewData)
	{
		return false;
	}

	bool isHit = false;

	// 바라 보는 방향
	FVector currlookVec = -In_forwardVector;

	currlookVec.Normalize();
	
	float armLength = _currDialogViewData->armLength;
	FVector movePos = In_lookTargetPos - currlookVec * armLength;

	Out_pos = movePos;
	isHit = GetHitPos(In_camParent, In_lookTargetPos, movePos, Out_pos);
	return isHit;
}
// greeting object hit pos 구하기(DialogGreetingObject)
bool FGsCameraModeManager::GetDialogHitPosGreetingObject(UGsGameObjectBase* In_camParent,
	const FVector& In_targetPos,
	const FRotator& In_targetRot,
	const FVector& In_lookTargetPos,
	FVector& Out_pos)
{
	Out_pos = FVector::ZeroVector;
	if (In_camParent == nullptr ||
		nullptr == _currDialogViewData)
	{
		return false;
	}

	bool isHit = false;

	// 타겟 회전을 벡터로
	FVector targetRotVec = In_targetRot.Vector();

	float armLength = _currDialogViewData->armLength;
	// 타겟을 보는 위치 구함(타겟 위치 + 타겟 방향에서 length만큼 뒤로 빠짐)
	FVector lookPos = In_targetPos + targetRotVec * armLength;

	FVector rootOffset = _currDialogViewData->rootOffset;
	// 보는 위치 오프셋 계산된 값
	FVector calcLookPos = lookPos + rootOffset;

	// 바라 보는 방향
	FVector currlookVec =
		In_lookTargetPos - calcLookPos;

	currlookVec.Normalize();
	
	FVector movePos = In_lookTargetPos - currlookVec * armLength;

	Out_pos = movePos;
	isHit = GetHitPos(In_camParent, In_lookTargetPos, movePos, Out_pos);
	return isHit;
}


// 히트 체크
bool FGsCameraModeManager::GetHitPos(UGsGameObjectBase* In_camParent,
	const FVector& In_startPos, const FVector& In_endPos,
	FVector& Out_hitPos)
{
	bool isHit = false;
	UWorld* world = In_camParent->GetWorld();
	if (world != nullptr)
	{
		ECollisionChannel collisionChannel =
			(ECollisionChannel)GData()->GetGlobalData()->_testCamDialogCollsion;

		TArray<FHitResult> arrayHitRes;
		world->LineTraceMultiByObjectType(
			OUT arrayHitRes,
			In_startPos,
			In_endPos,
			FCollisionObjectQueryParams(collisionChannel),
			FCollisionQueryParams()
		);

		if (arrayHitRes.Num() > 0)
		{
			for (FHitResult& hitRes : arrayHitRes)
			{
				UPrimitiveComponent* hitComponent = hitRes.GetComponent();
				AActor* hitActor = hitRes.GetActor();
				if (hitComponent && hitActor)
				{
					// 카메라 통과인지 체크(무시 아닌지 체크)
					if (hitComponent->GetCollisionResponseToChannel(ECollisionChannel::ECC_Camera) !=
						ECollisionResponse::ECR_Ignore)
					{
						Out_hitPos = hitRes.ImpactPoint;
						isHit = true;
						break;
					}
				}
			}
		}
	}

	return isHit;
}
// 대사 회전과 위치 가져오기
bool FGsCameraModeManager::GetDialogRotAndPos(UGsGameObjectBase* In_camParent,
	UGsGameObjectBase* In_targetObj,
	const FGsSchemaCameraModeData* In_camData,
	const FVector& In_targetPos,
	const FVector& In_localPos,
	FVector& Out_pos,
	FRotator& Out_rot,
	FVector& Out_lookTargetPos,
	float& Out_distCam,
	bool& Out_isHit)
{	
	EGsDialogCameraViewType viewType = GetDialogCameraViewType();

	if (_currDialogViewData == nullptr)
	{
		Out_pos = FVector::ZeroVector;
		Out_rot = FRotator::ZeroRotator;
		Out_isHit = false;
		return false;
	}

	FVector lookTargetOffset = _currDialogViewData->lookTargetOffset;

	FVector hitPos = FVector::ZeroVector;
	FVector lookTargetPos = FVector::ZeroVector;
	if (viewType == EGsDialogCameraViewType::DialogBackLocalSingleNpc ||
		viewType == EGsDialogCameraViewType::DialogBackNpc ||
		viewType == EGsDialogCameraViewType::DialogBackLocalMultiNpc)
	{
		FVector farPos = FVector::ZeroVector;
		FVector nearPos = FVector::ZeroVector;
		if (viewType == EGsDialogCameraViewType::DialogBackLocalSingleNpc ||
			viewType == EGsDialogCameraViewType::DialogBackLocalMultiNpc)
		{
			farPos = In_targetPos;
			nearPos = In_localPos;
		}
		else if (viewType == EGsDialogCameraViewType::DialogBackNpc)
		{
			farPos = In_localPos;
			nearPos = In_targetPos;
		}

		// 화자와 나 중간위치
		lookTargetPos = FMath::Lerp(In_targetPos, In_localPos, 0.5f) +
			lookTargetOffset;

		Out_isHit = GetDialogHitPos(In_camParent,
			farPos, nearPos, lookTargetPos,
			hitPos);		
	}
	else if (viewType == EGsDialogCameraViewType::DialogSide)
	{
		// 화자와 나 중간위치
		lookTargetPos = FMath::Lerp(In_targetPos, In_localPos, 0.5f)
			+ lookTargetOffset;

		Out_isHit = GetDialogHitPosSide(In_camParent,
			In_targetPos, In_localPos, 
			lookTargetPos,
			hitPos);		
	}
	else if (viewType == EGsDialogCameraViewType::DialogGreetingLocal ||
		viewType == EGsDialogCameraViewType::DialogGreetingNpc ||
		viewType == EGsDialogCameraViewType::DialogGreetingNpcFairy ||
		viewType == EGsDialogCameraViewType::DialogGreetingLocalFairy ||
		viewType == EGsDialogCameraViewType::DialogGreetingSpawnFairy)
	{
		FVector farPos = FVector::ZeroVector;

		UGsGameObjectBase* farObj = nullptr;
		if (viewType == EGsDialogCameraViewType::DialogGreetingLocal ||
			viewType == EGsDialogCameraViewType::DialogGreetingLocalFairy)
		{
			farObj = In_camParent;
		}
		else if (viewType == EGsDialogCameraViewType::DialogGreetingNpc ||
			viewType == EGsDialogCameraViewType::DialogGreetingNpcFairy)
		{
			farObj = In_targetObj;
		}
		else if (viewType == EGsDialogCameraViewType::DialogGreetingSpawnFairy)
		{			
			farObj = FindDialogFairyObj();
		}
		
		FVector forwardVector = FVector::ZeroVector;
		if (farObj != nullptr)
		{
			if (AGsCharacterBase* farChar = farObj->GetCharacter())
			{
				if (USkeletalMeshComponent* meshPtr = farChar->GetMesh())
				{
					if (viewType == EGsDialogCameraViewType::DialogGreetingSpawnFairy ||
						viewType == EGsDialogCameraViewType::DialogGreetingNpcFairy)
					{
						FTransform trans =
							meshPtr->GetSocketTransform(TEXT("GreetingViewSocket"), RTS_World);
						farPos = trans.GetTranslation();
					}
					else
					{
						farPos =
							meshPtr->GetBoneLocation(_headBoneName, EBoneSpaces::WorldSpace);
					}



#ifdef TEST_DRAW_POS
					if (UWorld* world = In_camParent->GetWorld())
					{
						DrawDebugCircle(world, farPos,
							100.0f, 100, FColor::Black, false, 60.f,
							0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));
					}
#endif

					// 로컬, 페어리 그리팅은 
					// 로컬과 페어리 중간위치를 본다
					if (viewType == EGsDialogCameraViewType::DialogGreetingLocalFairy)
					{
						UGsGameObjectBase* fairyObj = FindDialogFairyObj();
						if (fairyObj != nullptr)
						{
							FVector fairyPos = fairyObj->GetLocation();

							// z 값은 그리팅 본에 맞춤
							fairyPos.Z = farPos.Z;

							// 페어리, 로컬 본 중간 위치 구하기
							farPos = 
								FMath::Lerp(fairyPos,
									farPos, 0.5f);
						}
					}
				}
				forwardVector = farChar->GetActorForwardVector();
				
			}			
		}

		lookTargetPos = farPos + lookTargetOffset;

		Out_isHit = GetDialogHitPosGreeting(In_camParent,
			lookTargetPos,
			forwardVector,
			hitPos);		
	}
	else if (viewType == EGsDialogCameraViewType::DialogGreetingObject)
	{
		lookTargetPos = In_targetPos + lookTargetOffset;

		FRotator targetRot = FRotator::ZeroRotator;
		if (In_targetObj != nullptr)
		{
			targetRot = In_targetObj->GetRotation();
		}

		Out_isHit = GetDialogHitPosGreetingObject(In_camParent,
			In_targetPos, targetRot,
			lookTargetPos,
			hitPos);
	}

	Out_lookTargetPos = lookTargetPos;
	
	FVector movedCamLookVec = lookTargetPos - hitPos;
	movedCamLookVec.Normalize();

	FRotator controlRotTo = movedCamLookVec.Rotation();

	// 이거때문에 계산이 망함
	// 회전 잡아놓은거 다 이상하게됨
	//// 범위 넘어가면 튈수도 있어서 
	//float checkedControllerPitch =
	//	FMath::Clamp<float>(
	//		controlRotTo.Pitch,
	//		In_camData->ViewPitchMin,
	//		In_camData->ViewPitchMax);

	//controlRotTo.Pitch = checkedControllerPitch;

	Out_pos = hitPos;
	Out_rot = controlRotTo;

	float distanceCam = FVector::Distance(Out_pos, Out_lookTargetPos);
	Out_distCam = distanceCam;

	return true;
}
UGsGameObjectBase* FGsCameraModeManager::FindDialogFairyObj()
{
	int dialogFairyTblId =
		GSClientSpawn()->GetDialogSpawnedFairyTblId();
	UGsGameObjectBase* fairyObj =
		GSGameObject()->FindObjectLocalNearestNonPlayerByNPCId(dialogFairyTblId);

	return fairyObj;
}

// 대사 카메라 뷰 타입 세팅
void FGsCameraModeManager::SetDialogCameraViewData(const FGsSchemaCameraDialogViewData* In_data)
{
	if (In_data == nullptr)
	{
		return;
	}

	_dialogCameraViewType = In_data->type;
	_currDialogViewData = In_data;
}

void	FGsCameraModeManager::SetBackupDialog(const FVector& In_beforeDialogLookPos, 
	const FVector& In_dialogLookPos,
	float In_dialogCamDist)
{
	_backupBeforeDialogLookPos = In_beforeDialogLookPos;
	_backupDialogLookPos = In_dialogLookPos;
	_bakcupDialogCamDist = In_dialogCamDist;
}

// 모드에 다른 디폴트 값으로 저장
void FGsCameraModeManager::SaveModeBackupData(EGsCameraMode In_mode)
{
	switch (In_mode)
	{
		// 백업 안하는 모드는 재낀다
	case EGsCameraMode::Boss:
	case EGsCameraMode::Dialog:
	case EGsCameraMode::WarpStart:
	case EGsCameraMode::None:
	case EGsCameraMode::SpaceCrack:
	case EGsCameraMode::PlayableEvent:
	case EGsCameraMode::EquipView:
		return;
	}
	const FGsSchemaCameraModeData* data = GetCamModeData(In_mode);

	// 모드 저장
	_backupMode = In_mode;

	if (_localPlayer == nullptr)
	{
		return;
	}

	// 회전 저장
	FGsCameraModeHandler* handler = _localPlayer->GetCameraModeHandler();
	if (nullptr == handler)
	{
		return;
	}

	TSharedRef<FGsCameraModeBase> currMode = handler->GetState();

	FGsCameraModeControlBase* controlMode =
		static_cast<FGsCameraModeControlBase*>(&currMode.Get());

	if (controlMode == nullptr)
	{
		return;
	}
	_backupQuat = controlMode->GetModeRot(data).Quaternion();

}
// 그리팅일땐 저장하지 말자
void FGsCameraModeManager::SetBackupCamZoom(float In_zoom)
{ 
	// 줌 저장
	FGsCameraModeHandler* handler = _localPlayer->GetCameraModeHandler();
	if (nullptr == handler)
	{
		return;
	}

	TSharedRef<FGsCameraModeBase> currMode = handler->GetState();

	if (currMode->GetIsBeforeGreeting() == true)
	{
		return;
	}


	// 저장 가능한 모드만 저장하자
	EGsCameraMode mode = currMode->GetType();


	if (GSObserver()->GetIsObserveStart() == false)
	{
		if (_dicCamBackupZoom.Contains(mode) == false)
		{
			return;
		}
		_dicCamBackupZoom[mode] = In_zoom;
	}
	else
	{
		if (_dicObserverCamBackupZoom.Contains(mode) == false)
		{
			return;
		}
		_dicObserverCamBackupZoom[mode] = In_zoom;
	}
	
}

// 현재 저장 가능한지
bool FGsCameraModeManager::GetIsSavePossible()
{
	return _arraySavePossibleMode.Contains(_currentMode);
}

bool	FGsCameraModeManager::GetBackupCamZoom(EGsCameraMode In_mode, float& Out_Zoom)
{
	FGsObserverManager* observerManager = GSObserver();
	if (observerManager != nullptr && 
		observerManager->GetIsObserveStart() == true)
	{
		if (_dicObserverCamBackupZoom.Contains(In_mode) == false)
		{
			return false;
		}

		Out_Zoom = _dicObserverCamBackupZoom[In_mode];		
	}
	else
	{
		if (_dicCamBackupZoom.Contains(In_mode) == false)
		{
			return false;
		}

		Out_Zoom = _dicCamBackupZoom[In_mode];
	}

	return true;
}

// 현재 모드의 pitch를 저장한다
void	FGsCameraModeManager::SetBackupCamPitch()
{
	// 줌 저장
	FGsCameraModeHandler* handler = _localPlayer->GetCameraModeHandler();
	if (nullptr == handler)
	{
		return;
	}

	TSharedRef<FGsCameraModeBase> currMode = handler->GetState();
	// 그리팅일때 저장 안함
	if (currMode->GetIsBeforeGreeting() == true)
	{
		return;
	}

	AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}
	AGsPlayerController* controller = localChar->GetPlayerController();
	if (controller == nullptr)
	{
		return;
	}
	FRotator currRot = controller->GetControlRotation();


	// 저장 가능한 모드만 저장하자
	EGsCameraMode mode = currMode->GetType();

	if (GSObserver()->GetIsObserveStart() == false)
	{
		if (_dicCamBackupPitch.Contains(mode) == false)
		{
			return;
		}

		_dicCamBackupPitch[mode] = currRot.Pitch;
	}
	else
	{
		if (_dicObserverCamBackupPitch.Contains(mode) == false)
		{
			return;
		}

		_dicObserverCamBackupPitch[mode] = currRot.Pitch;
	}
}

// 저장된 pitch 값을 구한다
bool	FGsCameraModeManager::GetBackupCamPitch(EGsCameraMode In_mode, float& Out_pitch)
{
	if (GSObserver()->GetIsObserveStart() == false)
	{
		if (_dicCamBackupPitch.Contains(In_mode) == false)
		{
			return false;
		}


		Out_pitch = _dicCamBackupPitch[In_mode];
	}
	else
	{
		if (_dicObserverCamBackupPitch.Contains(In_mode) == false)
		{
			return false;
		}


		Out_pitch = _dicObserverCamBackupPitch[In_mode];
	}
	return true;
}

// backup pitch 구하기
// 1. backup 값
// 2. 없으면 기본 모드값
// 3. min, max 검증 까지 함
float FGsCameraModeManager::GetModeBackupCheckedPitch(EGsCameraMode In_mode)
{
	const FGsSchemaCameraModeData* camModeData = GetCamModeData(In_mode);
	if (nullptr == camModeData)
	{
		return 0.0f;
	}

	float pitchRot = 0.0f;
	// 백업한거 있으면 백업한걸로 가자
	if (GetBackupCamPitch(In_mode, pitchRot) == false)
	{
		// 백업이 없다면 기본 데이터로 간다
		pitchRot = camModeData->ControllerPitch;
	}

	// min, max가 음수값이면 제대로 체크를 못해서 변환해서 체크
	float clampAnglePitch = FMath::ClampAngle(pitchRot, camModeData->ViewPitchMin, camModeData->ViewPitchMax);
	float checkedControllerPitch = FRotator::ClampAxis(clampAnglePitch);


	return checkedControllerPitch;
}

// get bone height value from game data at initialization time 
// (data generated when lobby character is selected)
void FGsCameraModeManager::SaveCameraTargetBoneHeight()
{
	if (FGsGameDataManager* dataManager = GGameData())
	{
		float savedCameraTargetBoneHeight = dataManager->GetSavedCameraTargetBoneHeight();
		SetSavedCameraTargetBoneHeight(savedCameraTargetBoneHeight);
	}
}

EGsCameraFixedQuarterYawType FGsCameraModeManager::GetFixedQuarterYawType()
{
	return _fixedQuarterYawType;
}

// 그리팅 뷰인가
//https://jira.com2us.com/jira/browse/CHR-15270
// first quest dialog no greeting
bool	FGsCameraModeManager::GetIsGreetingView()
{ 
	bool isFirstQuest = false;
	if (UGsQuestManager* QuestMgr = GSQuest())
	{
		isFirstQuest = QuestMgr->IsReadyFirstQuest();
		if (isFirstQuest == true && _isGreetingView == true)
		{
			_isGreetingView = false;
		}
	}
	return _isGreetingView; 
}

AActor* FGsCameraModeManager::GetDialogTargetAcotr(UGsGameObjectBase* In_targetObj)
{
	EGsDialogCameraViewType viewType = GetDialogCameraViewType();

	AActor* findTargetActor = nullptr;

	if (In_targetObj == nullptr ||
		_localPlayer == nullptr)
	{
		return nullptr;
	}

	if (viewType == EGsDialogCameraViewType::DialogBackLocalSingleNpc ||
		viewType == EGsDialogCameraViewType::DialogBackLocalMultiNpc ||
		viewType == EGsDialogCameraViewType::DialogGreetingNpc || 
		viewType == EGsDialogCameraViewType::DialogGreetingObject ||
		viewType == EGsDialogCameraViewType::DialogGreetingNpcFairy)
	{
		findTargetActor = In_targetObj->GetActor();
	}
	else if (viewType == EGsDialogCameraViewType::DialogBackNpc ||
		viewType == EGsDialogCameraViewType::DialogSide ||
		viewType == EGsDialogCameraViewType::DialogGreetingLocal||
		viewType == EGsDialogCameraViewType::DialogGreetingLocalFairy)
	{
		findTargetActor = _localPlayer->GetActor();
	}
	else if (viewType == EGsDialogCameraViewType::DialogGreetingSpawnFairy)
	{
		if (UGsGameObjectBase* findObj = FindDialogFairyObj())
		{
			findTargetActor = findObj->GetActor();
		}
	}

	return findTargetActor;
}

// mode pitch 구하기
// 1. 기본 모드값
// 2. min, max 검증 까지 함
float FGsCameraModeManager::GetModeCheckedPitch(EGsCameraMode In_mode)
{
	const FGsSchemaCameraModeData* camModeData = GetCamModeData(In_mode);
	if (nullptr == camModeData)
	{
		return 0.0f;
	}

	float pitchRot = camModeData->ControllerPitch;
	

	// min, max가 음수값이면 제대로 체크를 못해서 변환해서 체크
	float clampAnglePitch = FMath::ClampAngle(pitchRot, camModeData->ViewPitchMin, camModeData->ViewPitchMax);
	float checkedControllerPitch = FRotator::ClampAxis(clampAnglePitch);


	return checkedControllerPitch;
}