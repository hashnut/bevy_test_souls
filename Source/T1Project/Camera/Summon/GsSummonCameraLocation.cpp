#include "GSSummonCameraLocation.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Camera/Summon/GsSummonCameraRotaion.h"

#include "Management/ScopeGlobal/GsLevelManager.h"

#include "ActorEx/GsActorSummonSceneObject.h"

#include "Engine/Classes/Curves/CurveFloat.h"
#include "Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Classes/Materials/MaterialInstance.h"
#include "Engine/Classes/Materials/MaterialLayersFunctions.h"
#include "Engine/Classes/Engine/World.h"
#include "Engine/Classes/Camera/PlayerCameraManager.h"
#include "Engine/Classes/Engine/Engine.h"

#include "Camera/CameraComponent.h"


const FName NameBlurXOffset = TEXT("OffsetX");
const FName NameBlurYOffset = TEXT("OffsetY");

// 초기화
void FGsSummonCameraLocation::Initialize()
{
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(_viewSize);
	}



}

// 갱신
void FGsSummonCameraLocation::Update(float In_deltaTime)
{	
	if (_isFinish == true)
	{
		return;
	}

	float moveSpeed = _nowSpeed;

	float addRate = In_deltaTime * moveSpeed;
	_progressRate = FMath::Clamp<float>(_progressRate + addRate, 0.f, 1.f);
	UCurveFloat* curve = GData()->GetCamFollowCurve();

	float curveVal = 0.0f;
	if (nullptr != curve)
	{
		curveVal = FMath::Clamp<float>(curve->GetFloatValue(_progressRate), 0.f, 1.f);
	}

	float newDist = FMath::Lerp(_camDistFrom, _camDistTo, curveVal);
	FVector nowLookTarget = FMath::Lerp(_lookTargetFrom, _lookTargetTo, curveVal);

	// 목적지 값이랑 같은가(!= 로 하니까 미세하게 다르게 나올때 있음)
	if (FMath::IsNearlyEqual(newDist, _camDistTo, 0.01f) == false)
	{
	}
	else
	{
		// 도달 하면 flag 변환
		_isFinish = true;
	}

	FRotator camRot = FRotator::ZeroRotator;


	if (_owner != nullptr)
	{
		FGsSummonCameraRotation* rotationWork =
			_owner->GetCameraRotation();

		if (rotationWork != nullptr)
		{
			camRot = rotationWork->GetCurrentRotator();
		}
	}

	FVector currCamVec = camRot.Vector();
	currCamVec.Normalize();
	FVector newPos = nowLookTarget - newDist * currCamVec;

	UCameraComponent* camComponent = _owner->GetCameraComponent();
	if (camComponent == nullptr)
	{
		return;
	}

	// 월드
	camComponent->SetWorldLocation(newPos);
	

	// 바라보는 대상의 위치를 screen 좌표로 변경
	if(APlayerController* controller = GLevel()->GetWorld()->GetFirstPlayerController())
	{
		FVector2D screenLocation = FVector2D::ZeroVector;
		controller->ProjectWorldLocationToScreen(_lookTargetTo, screenLocation);

		// (-0.5,-0.5) ~ (0.5,0.5) 영역으로 만든다
		// x/viewX - 0.5, y/viewY - 0.5

		if (_viewSize.IsZero() == false)
		{
			float xRate = screenLocation.X / _viewSize.X - 0.5f;
			float yRate = screenLocation.Y / _viewSize.Y - 0.5f;


			//// fov 가 80에 맞춰저있으므로
			//// 현재(50)은 추가 계산 필요
			//// fov가 작아질수록 rate가 줄어들어야 함
			//// 현재fov/80 으로 해보자 ㄷㄷ
			//APlayerCameraManager* camManager = controller->PlayerCameraManager;
			//if (camManager == nullptr)
			//{
			//	return;
			//}

			//float orgFov =
			//	GData()->GetGlobalData()->_summonTestedBlurFOV;

			//if (orgFov == 0.0f)
			//{
			//	return;
			//}

			//float currentFov = camManager->GetFOVAngle();

			//// 곱해야 하는 값
			//// fov가 작을수록 rate가 줄어들어야 한다
			//_fovFactor = currentFov / orgFov;

			//float fovMulVal =
			//	GData()->GetGlobalData()->_summonFOVMulFactor;

			//_fovFactor *= fovMulVal;
			//


			//xRate *= _fovFactor;
			//yRate *= _fovFactor;
			
			if (camComponent->PostProcessSettings.WeightedBlendables.Array.Num() == 0)
			{
				return;
			}

			UMaterialInstance* materialInstance =
				Cast<UMaterialInstance>(camComponent->PostProcessSettings.WeightedBlendables.Array[0].Object);

			if (materialInstance != nullptr &&
				materialInstance->ScalarParameterValues.Num() >= 3)
			{

				// x offset
				if (materialInstance->ScalarParameterValues[1].ParameterInfo.Name 
					== NameBlurXOffset)
				{
					materialInstance->ScalarParameterValues[1].ParameterValue = xRate;
				}

				// y offset
				if (materialInstance->ScalarParameterValues[2].ParameterInfo.Name
					== NameBlurYOffset)
				{
					materialInstance->ScalarParameterValues[2].ParameterValue = yRate;
				}
			}

		}
	}

	if (_isFinish == true)
	{
		OnFinishLerp();
	}
}

void FGsSummonCameraLocation::SetData(float In_distTo, float In_distFrom,
	const FVector& In_lookTargetTo,
	const FVector& In_lookTargetFrom,
	const FVector& In_moveTo, const FVector& In_moveFrom)
{
	_locationTo = In_moveTo;
	_locationFrom = In_moveFrom;

	// 이동하기위해서 초기화
	_isFinish = false;
	// 진행도 초기화
	_progressRate = 0.0f;

	_camDistFrom = In_distFrom;
	_camDistTo = In_distTo;

	_lookTargetTo = In_lookTargetTo;
	_lookTargetFrom = In_lookTargetFrom;
	
}

// 거리에 의한 속도 구하기
float FGsSummonCameraLocation::MakeDistSpeed()
{
	float dist =
		FVector::Dist(_locationTo, _locationFrom);

	if (dist == 0.f)
	{
		dist = 1.0f;
	}
	float speed =
		GData()->GetGlobalData()->_summonCameraHighGradeSpeed;
	// 거리가 멀수록 느려짐(rate 로 진행되므로 일정한 속도 만든다)
	float nowSpeed = speed / dist;
	return nowSpeed;
}

void FGsSummonCameraLocation::OnFinishLerp()
{
	if (_callbackFinishLerp == nullptr)
	{
		return;
	}

	_callbackFinishLerp(EGsCameraWorkType::LocationWork);
}