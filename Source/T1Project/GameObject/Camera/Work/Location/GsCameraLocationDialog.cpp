#include "GameObject/Camera/Work/Location/GsCameraLocationDialog.h"

#include "GameObject/Camera/Work/Rotation/GsCameraRotationDialog.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/Camera/Mode/GsCameraModeDialog.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Management/ScopeGame/GsCameraModeManager.h"

#include "UTIL/GsClassUtil.h"

#include "Engine/Classes/Curves/CurveFloat.h"
#include "Engine/Classes/Camera/CameraComponent.h"

#include "Kismet/GameplayStatics.h"

void FGsCameraLocationDialog::OnFinishLerp()
{
	if (nullptr == _owner)
	{
		return;
	}
	_owner->OnFinishLerpType(EGsDialogCameraFinishType::Location);	
}

// 회전속도 구하기
float FGsCameraLocationDialog::GetRotSpeed()
{
	float speed = GData()->GetGlobalData()->CamDialogLocLerpSpeed;
	return speed;
}

// 업데이트
void FGsCameraLocationDialog::Update(float In_deltaTime)
{
	UpdateLookAt(In_deltaTime);
	
}

