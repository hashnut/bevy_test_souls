#include "GsCameraModeWarpStart.h"

#include "GameObject/Camera/Work/Zoom/GsCameraZoomGame.h"
#include "GameObject/Camera/Work/Rotation/GsCameraRotationWarpStart.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "Camera/Define/GsCameraGameDefine.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Management/GsMessageHolder.h"

#include "GameFramework/SpringArmComponent.h"


FGsCameraModeWarpStart::FGsCameraModeWarpStart() : FGsCameraModeControlBase(EGsCameraMode::WarpStart)
{
}
void FGsCameraModeWarpStart::Initialize()
{
	Super::Initialize();

	IGsCameraWork* rotationWarpStartWork = new FGsCameraRotationWarpStart();
	_mapCamWorks.Add(EGsCameraWorkType::WarpStartRotationWork, rotationWarpStartWork);
}

void FGsCameraModeWarpStart::ChangeModeNormal(const FGsSchemaCameraModeData* In_modeData)
{
	if (FGsCameraZoomGame* zoomWork = GetCameraZoomGame())
	{
		StartModeZoom(EGsCameraChangeZoomType::NoLerp);
	}

	if (FGsCameraRotationWarpStart* rotationWork = GetCameraRotationWarpStart())
	{
		FRotator warpCameraControllerRotatorOffset = GData()->GetGlobalData()->_warpCameraControllerRotatorOffset;

		if (nullptr == _localPlayer)
		{
			return;
		}

		AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
		if (localChar == nullptr)
		{
			return;
		}
		FVector fwdVec =
			localChar->GetActorForwardVector();

		FVector oppVec = fwdVec * -1.0f;

		FRotator inverseRot = oppVec.Rotation();
		FRotator warpCamRot =  warpCameraControllerRotatorOffset + inverseRot;

		rotationWork->SetRotation(warpCamRot);
	}
}

// ¸ðµåÁÜ ½ÃÀÛ
void FGsCameraModeWarpStart::StartModeZoom(EGsCameraChangeZoomType In_type)
{		
	FGsCameraZoomGame* zoomWork = GetCameraZoomGame();

	if (nullptr == zoomWork)
	{
		return;
	}

	float warpZoom = GData()->GetGlobalData()->_warpCameraZoomValue;
	zoomWork->StartModeZoom(warpZoom, In_type);
}

// ÁÜ Á¾·á ÄÝ¹é
void FGsCameraModeWarpStart::OnZoomFinished()
{
}

FGsCameraRotationWarpStart* FGsCameraModeWarpStart::GetCameraRotationWarpStart()
{
	IGsCameraWork* work = GetCameraWork(EGsCameraWorkType::WarpStartRotationWork);
	if (work == nullptr)
	{
		return nullptr;
	}

	FGsCameraRotationWarpStart* rotationWarpStarttWork = FGsClassUtil::FCheckStaticCast<FGsCameraRotationWarpStart>(work);
	return rotationWarpStarttWork;
}