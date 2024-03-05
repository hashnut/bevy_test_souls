#include "GsCameraRotationQuarter.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/Camera/Mode/GsCameraModeBase.h"

#include "GameFramework/PlayerController.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Management/ScopeGame/GsCameraModeManager.h"

#include "Camera/Define/GsCameraGameDefine.h"

#include "Engine/Classes/Camera/CameraComponent.h"


// 회전속도 구하기
float FGsCameraRotationQuarter::GetRotSpeed()
{
	float rotSpeed = 0.0f;
	if (_isAfterDialog == true)
	{
		rotSpeed =
			GData()->GetGlobalData()->CamDialogRotLerpSpeed;
	}
	else
	{
		rotSpeed =
			GData()->GetGlobalData()->CamModeChangeRotLerpSpeed;
	}
	return rotSpeed;
}

// 속도 만들기
void FGsCameraRotationQuarter::MakeSpeed()
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

void FGsCameraRotationQuarter::InitMode()
{
	// 줌 회전 플래그 초기화
	_isUsingZoomRot = false;
}

// 버튼클릭에 의한 모드 전환은 컨트롤 회전
// 이전에 다이얼로그였으면 카메라의 world 회전이다
void FGsCameraRotationQuarter::SetRotation(const FRotator& In_rot)
{
	if (_local == nullptr)
	{
		return;
	}
	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();

	if (_rotationType == EGsCameraRotationType::ControlRotaion)
	{
		if (localChar->Controller == nullptr ||
			localChar->Controller->IsLocalPlayerController() == false)
		{
			return;
		}
		localChar->Controller->SetControlRotation(In_rot);		
	}
	else if (_rotationType == EGsCameraRotationType::WorldRotation)
	{
		if (UCameraComponent* camComponent = localChar->GetFollowCamera())
		{
			camComponent->SetWorldRotation(In_rot);
		}
	}

}

bool FGsCameraRotationQuarter::IsRotationPossible()
{
	return ( false == _isFinish) &&
		// 줌인 입력 되면 줌인 용 회전 처리가 있으므로 막는다
		(false == _isUsingZoomRot);
}

void FGsCameraRotationQuarter::OnFinishLerp()
{
	Super::OnFinishLerp();
	if (_isUseFinishCallback == true && _owner != nullptr)
	{
		_owner->OnFinishLerpType(EGsDialogCameraFinishType::Rotation);
	}
}