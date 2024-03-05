#include "GsCameraTargetOffsetGame.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Engine/Classes/Curves/CurveFloat.h"

void FGsCameraTargetOffsetGame::Finalize()
{
	RemoveLocalPlayer();
}

// 모드 target 오프셋 시작
void FGsCameraTargetOffsetGame::StartModeTargetOffset(FVector In_offsetFrom, FVector In_offsetTo)
{
	// 시작점
	_modeTargetOffsetFrom = In_offsetFrom;
	// 목표점
	_modeTargetOffsetTo = In_offsetTo;

	// 같으면 시작 할 필요 없음
	if (true == _modeTargetOffsetTo.Equals(_modeTargetOffsetFrom))
	{
		_isModeTargetOffsetChanging = false;
	}
	else
	{
		_isModeTargetOffsetChanging = true;
		_progressModeTargetOffsetRate = 0.0f;
	}
}
// 캐릭터 삭제 처리
void FGsCameraTargetOffsetGame::RemoveLocalPlayer()
{
	_local = nullptr;
	_targetObj = nullptr;
}

// 소켓 오프셋 갱신
void FGsCameraTargetOffsetGame::Update(float In_deltaTime)
{
	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}

	if (false == _isModeTargetOffsetChanging)
	{
		return;
	}

	// 속도
	float speed =
		GData()->GetGlobalData()->CamModeChangeRotLerpSpeed;

	float addRate = In_deltaTime * speed;
	_progressModeTargetOffsetRate = FMath::Clamp<float>(_progressModeTargetOffsetRate + addRate, 0.f, 1.f);
	UCurveFloat* curve = GData()->GetCamFollowCurve();

	float curveVal = 0.0f;
	if (nullptr != curve)
	{
		curveVal = FMath::Clamp<float>(curve->GetFloatValue(_progressModeTargetOffsetRate), 0.f, 1.f);
	}

	FVector curveNewOffset =
		FMath::Lerp(_modeTargetOffsetFrom, _modeTargetOffsetTo, curveVal);

	if (false == curveNewOffset.Equals(_modeTargetOffsetTo, 0.1f))
	{

	}
	else
	{
		_isModeTargetOffsetChanging = false;
	}
	localChar->GetSpringArm()->TargetOffset = curveNewOffset;

}

void FGsCameraTargetOffsetGame::SetDirectTargetOffset(FVector In_offsetTo)
{
	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}

	localChar->GetSpringArm()->TargetOffset = In_offsetTo;
}