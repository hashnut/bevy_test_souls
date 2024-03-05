#include "GsSummonCameraBlur.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "ActorEx/GsActorSummonSceneObject.h"

#include "Engine/Classes/Curves/CurveFloat.h"
#include "Engine/Classes/Engine/Scene.h"

#include "Camera/CameraComponent.h"

void FGsSummonCameraBlur::Initialize()
{
	_summonCamBlurSpeed = GData()->GetGlobalData()->_summonCamBlurSpeed;
}

// °»½Å
void FGsSummonCameraBlur::Update(float In_deltaTime)
{
	if (_isFinish == true)
	{
		return;
	}
	float speed = _summonCamBlurSpeed;
	float addRate = In_deltaTime * speed;
	_progressRate = FMath::Clamp<float>(_progressRate + addRate, 0.f, 1.f);

	UCurveFloat* curve = GData()->GetCamFollowCurve();

	float curveVal = 0.0f;
	if (nullptr != curve)
	{
		curveVal = FMath::Clamp<float>(curve->GetFloatValue(_progressRate), 0.f, 1.f);
	}

	if (FMath::IsNearlyEqual(1.0f, curveVal, 0.001f) == false)
	{

	}
	else
	{
		_isFinish = true;
	}

	UCameraComponent* camComponent = _owner->GetCameraComponent();
	if (camComponent == nullptr)
	{
		return;
	}
	if (camComponent->PostProcessSettings.WeightedBlendables.Array.Num() == 0)
	{
		return;
	}

	camComponent->PostProcessSettings.WeightedBlendables.Array[0].Weight = curveVal;

	if (_isFinish == true)
	{
		OnFinishLerp();
	}
	
}

void FGsSummonCameraBlur::OnFinishLerp()
{
	if (_callbackFinishLerp == nullptr)
	{
		return;
	}

	_callbackFinishLerp(EGsCameraWorkType::BlurWork);
}