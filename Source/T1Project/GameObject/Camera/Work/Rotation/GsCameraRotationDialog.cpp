#include "GsCameraRotationDialog.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Camera/Mode/GsCameraModeBase.h"

#include "GameFramework/PlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Engine/World.h"
#include "Engine/Classes/Camera/CameraComponent.h"

#include "T1Project.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataSchema/Camera/GsSchemaCameraModeData.h"

#include "Management/ScopeGame/GsCameraModeManager.h"

#include "Camera/Define/GsCameraGameDefine.h"

// 대사 카메라는 월프 포지션 이동만 한다
void FGsCameraRotationDialog::SetRotation(const FRotator& In_rot)
{
	if (_local == nullptr)
	{
		return;
	}
	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
	if (UCameraComponent* camComponent = localChar->GetFollowCamera())
	{
		camComponent->SetWorldRotation(In_rot);
	}
}
void FGsCameraRotationDialog::OnFinishLerp()
{
	Super::OnFinishLerp();
	if (nullptr != _owner)
	{
		_owner->OnFinishLerpType(EGsDialogCameraFinishType::Rotation);
	}
}

// 회전속도 구하기
float FGsCameraRotationDialog::GetRotSpeed()
{
	float rotationSpeed = GData()->GetGlobalData()->CamDialogRotLerpSpeed;
	return rotationSpeed;
}
