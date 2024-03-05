#include "GsCameraZoomGame.h"

#include "GameObject/Camera/Mode/GsCameraModeBase.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Management/ScopeGame/GsCameraModeManager.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "ControllerEx/GsPlayerController.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"

#include "Curves/CurveFloat.h"

#include "GameFrameWork/SpringArmComponent.h"
#include "Engine/Classes/Camera/PlayerCameraManager.h"
#include "Engine/Classes/Camera/CameraComponent.h"

#include "T1Project.h"

// 초기화
void FGsCameraZoomGame::Initialize()
{
#if WITH_EDITOR || PLATFORM_WINDOWS	
	_camArmLengthSpeed = GData()->GetGlobalData()->CamArmLengthSpeedPC;
#else
	_camArmLengthSpeed = GData()->GetGlobalData()->CamArmLengthSpeed;
#endif

#if WITH_EDITOR || PLATFORM_WINDOWS	
	_camInputArmLengthSpeed = GData()->GetGlobalData()->CamInputArmLengthSpeedPC;
#else
	_camInputArmLengthSpeed = GData()->GetGlobalData()->CamInputArmLengthSpeedMobile;
#endif

	_cameraZoomFinishDiff = GData()->GetGlobalData()->_cameraZoomFinishDiff;


	if (UWorld* world = GLevel()->GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			// 로비에서는 백업 안함
			if (gameMode->GetGameModeType() == EGsGameModeType::Lobby)
			{
				_isBackupZoom = false;
			}
		}
	}
}

// 마지막 처린
void FGsCameraZoomGame::Finalize()
{
	_targetMode = nullptr;
	RemoveLocalPlayer();
}
void FGsCameraZoomGame::RemoveLocalPlayer()
{
	_local = nullptr;
	_targetObj = nullptr;
}

// 최초 한번 초기화
void FGsCameraZoomGame::SetMode(FGsCameraModeBase* In_mode)
{
	_targetMode = In_mode;
}

void FGsCameraZoomGame::InitMode(float In_camZoomMin, float In_camZoomMax)
{
	_camArmLengthMin = In_camZoomMin;
	_camArmLengthMax = In_camZoomMax;

}

// 줌 세팅
void FGsCameraZoomGame::SetZoom(float In_val)
{
	if (_local == nullptr)
	{
		return;
	}

	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}

	localChar->GetSpringArm()->TargetArmLength = In_val;
}
void FGsCameraZoomGame::SetDirectPos(const FVector& In_moveTo)
{
	if (_local == nullptr)
	{
		return;
	}

	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();

	if (localChar == nullptr)
	{
		return;
	}

	if (UCameraComponent* camComponent = localChar->GetFollowCamera())
	{		
		camComponent->SetWorldLocation(In_moveTo);		
	}
}

// 줌갱신
bool FGsCameraZoomGame::UpdateZoom(float In_deltaTime)
{
	if (_local == nullptr)
	{
		//GSLOG(Error, TEXT("In_char == nullptr"));
		return false;
	}
	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
	if (localChar == nullptr)
	{
		//GSLOG(Error, TEXT("localChar == nullptr"));
		return false;
	}

	if (true == _isZoomStart)
	{
		// 1. input에 의한 줌인가(터치, 휠)
		// 2. 모드 전환에 의한 줌인가
		float speed = (_isTouchZoom == true) ? _camInputArmLengthSpeed: _camArmLengthSpeed;

		// 속도가 0이면 안함
		if (speed == 0.0f)
		{
			return true;
		}

		float addRate = In_deltaTime * speed;
		_progressRate = FMath::Clamp<float>(_progressRate + addRate, 0.f, 1.f);
		UCurveFloat* curve = GData()->GetCamFollowCurve();

		float curveVal = 0.0f;
		if (nullptr != curve)
		{
			curveVal = FMath::Clamp<float>(curve->GetFloatValue(_progressRate), 0.f, 1.f);
		}

		float newLength = FMath::Lerp(_armLengthFrom, _armLengthTo, curveVal);


		localChar->GetSpringArm()->TargetArmLength = newLength;

#ifdef TEST_LOG
		GSLOG(Warning, TEXT("localChar->GetSpringArm()->TargetArmLength: %f, _armLengthTo: %f, delta: %f"),
			localChar->GetSpringArm()->TargetArmLength,
			_armLengthTo,
			In_deltaTime);
#endif

		// 차이 절대값
		float diff = FMath::Abs(_armLengthTo - localChar->GetSpringArm()->TargetArmLength);
			
		// 목표점 도달했다면
		// 차이가 일정 이하로 떨어지면...
		if (diff <= _cameraZoomFinishDiff)
		{
			localChar->GetSpringArm()->TargetArmLength = _armLengthTo;
			return true;
		}
	}

	return false;
}

void FGsCameraZoomGame::Update(float In_deltaTime)
{
	if (_isAfterDialog == true)
	{
		UpdatePosZoom(In_deltaTime);
		return;
	}

	bool isEnd = UpdateZoom(In_deltaTime);

	// 줌일때만 함
	if (true == _isZoomStart &&
		false == _isGreetingOffZoom)
	{
		CheckZoomMin();
	}
	// 업데이트 줌 하고 나서 끝에 도달했는지 처리가 필요함
	// 상속받은 quarter에서도 마지막 세팅 처리가 필요하므로

	// 목표점 도달했다면
	// 차이가 일정 이하로 떨어지면...
	if (true == isEnd)
	{
		// 플래그 끔
		_isZoomStart = false;

		// 종료 콜백 호출
		OnFinishLerp();
		//GSLOG(Warning, TEXT("zoom finish"));
	}


}

// 줌인
void FGsCameraZoomGame::ZoomIn(float In_mulVal)
{
	_isTouchZoom = true;

	float nowZoom = GetNowZoom();
	float zoomTo =
		FMath::Clamp((nowZoom - In_mulVal),
			_camArmLengthMin,
			_camArmLengthMax);
	ZoomStart(nowZoom, zoomTo);

	if (true == _isBackupZoom)
	{
		// 카메라 값 백업해놓기(유효한 상황에서만 백업/gameusersetting에 저장될 값)
		//카메라 회전
		//카메라 줌
		//모드 변경
		//back view 갱신시
		GSCameraMode()->SetBackupCamZoom(zoomTo);
	}
}
// 줌아웃
void FGsCameraZoomGame::ZoomOut(float In_mulVal)
{
	_isTouchZoom = true;

	float nowZoom = GetNowZoom();
	float zoomTo =
		FMath::Clamp((nowZoom + In_mulVal),
			_camArmLengthMin,
			_camArmLengthMax);
	ZoomStart(nowZoom, zoomTo);

	if (true == _isBackupZoom)
	{
		// 카메라 값 백업해놓기(유효한 상황에서만 백업/gameusersetting에 저장될 값)
		//카메라 회전
		//카메라 줌
		//모드 변경
		//back view 갱신시
		GSCameraMode()->SetBackupCamZoom(zoomTo);
	}
}

// 그리팅 뷰 세팅
void FGsCameraZoomGame::SetGreetingView()
{
	_isTouchZoom = false;

	float greetingZoom = GData()->GetGlobalData()->CamGreetingZoom;

	SetZoom(greetingZoom);
	// 줌 밀어놓고 min 체크후 강제 변경
	CheckZoomMin(true);

}
// 모드줌 시작
void FGsCameraZoomGame::StartModeZoom(float In_armLengthTo, EGsCameraChangeZoomType In_type)
{
	_isTouchZoom = false;


	float zoomFrom = 0.0f;
	// 워프 시작이면
	if (In_type == EGsCameraChangeZoomType::NoLerp||
		In_type == EGsCameraChangeZoomType::SpawnStart)
	{
		SetZoom(In_armLengthTo);
		// 줌 밀어놓고 min 체크후 강제 변경
		CheckZoomMin(true);
	}
	else
	{
		// 줌 도착시 min 체크 강제로 변경 해야함
		_isDirtyMinCheckForcedChange = true;

		// 현재 길이
		zoomFrom = GetNowZoom();
		ZoomStart(zoomFrom, In_armLengthTo);
	}

	
}
// 현재 줌 구하기
float FGsCameraZoomGame::GetNowZoom()
{
	float resVal = 0.0f;
	if (_local == nullptr)
	{
		return resVal;
	}

	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
	if (localChar == nullptr)
	{
		GSLOG(Error, TEXT("localChar == nullptr"));
		return resVal;
	}
	// 현재 길이
	resVal = localChar->GetSpringArm()->TargetArmLength;
	return resVal;
}

// 줌 종료 콜백
void FGsCameraZoomGame::OnFinishLerp()
{
	if (_targetMode != nullptr)
	{
		_targetMode->OnZoomFinished();
	}

	if (_isDirtyMinCheckLerpEnd == true)
	{
		_isDirtyMinCheckLerpEnd = false;
		bool isNowMin = IsMinZoom();
		ProcZoomMin(isNowMin);
	}

//	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
//	if (localChar == nullptr)
//	{
//		return;
//	}
//	FVector currCamLocation = FVector::ZeroVector;
//
//	if (AGsPlayerController* controller = localChar->GetPlayerController())
//	{
//		APlayerCameraManager* camManager = controller->PlayerCameraManager;
//		if (camManager != nullptr)
//		{
//			currCamLocation = camManager->GetCameraLocation();
//		}
//	}
//
//
//#if WITH_EDITOR
//	GSLOG(Error, TEXT("Zoom Finish OnFinishLerp --  currCamLocation[%s] "),
//		*currCamLocation.ToString());
//#endif

}

// 줌 시작
void FGsCameraZoomGame::ZoomStart(float In_zoomFrom, float In_zoomTo)
{
	_armLengthFrom = In_zoomFrom;
	_armLengthTo = In_zoomTo;

	_isZoomStart = true;

	_progressRate = 0.0f;
}

void FGsCameraZoomGame::PosZoomStart(float In_distance, const FVector& In_startPos, const FVector& In_startRot,
	bool In_isNowMove)
{
	_armLengthFrom = 0;
	_armLengthTo = In_distance;

	_isZoomStart = In_isNowMove;

	_progressRate = 0.0f;

	_camStartPos = In_startPos;
	_camStartRot = In_startRot;
}

// 줌 최소값 도달했는지 체크
void FGsCameraZoomGame::CheckZoomMin(bool In_isForced)
{
	if (_targetMode == nullptr)
	{
		return;
	}
	// zoom min 사용 하는지
	// forced면 무조껀 해야될듯
	if (_targetMode->GetIsNowCheckZoomMin() == false &&
		In_isForced == false)
	{
		return;
	}

	bool isNowMin = IsMinZoom();
	
	// 이전값과 다르거나
	// 강제이거나
	if (_isReachZoomMin != isNowMin ||
		true == In_isForced ||
		true == _isDirtyMinCheckForcedChange)
	{
		ProcZoomMin(isNowMin);
	}
	else
	{
		if (true == IsNeedCheckZoomMin())
		{
			_isDirtyMinCheckLerpEnd = true;
		}
	}

}
void FGsCameraZoomGame::ProcZoomMin(bool In_isNowMin)
{
	_isDirtyMinCheckForcedChange = false;

	_isReachZoomMin = In_isNowMin;
	_targetMode->OnReachZoomMin(_isReachZoomMin);
}
// 줌 최소 값인가
bool FGsCameraZoomGame::IsMinZoom()
{
	float nowZoom = GetNowZoom();
	bool isNowMin = (_camArmLengthMin >= nowZoom) ? true : false;
	return isNowMin;
}
// Values ​​less than the minimum should be checked.
bool FGsCameraZoomGame::IsNeedCheckZoomMin()
{
	float nowZoom = GetNowZoom();
	return (_camArmLengthMin > nowZoom) ? true : false;
}

void FGsCameraZoomGame::UpdatePosZoom(float In_deltaTime)
{
	if (_isZoomStart == false)
	{
		return;
	}

	// 1. input에 의한 줌인가(터치, 휠)
	// 2. 모드 전환에 의한 줌인가
	float speed =  _camArmLengthSpeed;

	// 속도가 0이면 안함
	if (speed == 0.0f)
	{
		return;
	}

	float addRate = In_deltaTime * speed;
	_progressRate = FMath::Clamp<float>(_progressRate + addRate, 0.f, 1.f);
	UCurveFloat* curve = GData()->GetCamFollowCurve();

	float curveVal = 0.0f;
	if (nullptr != curve)
	{
		curveVal = FMath::Clamp<float>(curve->GetFloatValue(_progressRate), 0.f, 1.f);
	}

	float newLength = FMath::Lerp(_armLengthFrom, _armLengthTo, curveVal);

	FVector nowPos = _camStartPos + newLength * _camStartRot;

	SetDirectPos(nowPos);

	if (_progressRate >= 1.0f)
	{
		_isZoomStart = false;

		// 종료 콜백 호출
		OnFinishLerp();
	}	
}