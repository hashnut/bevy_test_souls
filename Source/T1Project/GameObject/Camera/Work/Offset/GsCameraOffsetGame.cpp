#include "GsCameraOffsetGame.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Engine/Classes/Curves/CurveFloat.h"

void FGsCameraOffsetGame::Finalize()
{
	RemoveLocalPlayer();
}

// 모드 소켓 오프셋 시작
void FGsCameraOffsetGame::StartModeSocketOffset(FVector In_offsetFrom, FVector In_offsetTo)
{
	// 시작점
	_modeSocketOffsetFrom = In_offsetFrom;
	// 목표점
	_modeSocketOffsetTo = In_offsetTo;

	// 같으면 시작 할 필요 없음
	if (true == _modeSocketOffsetTo.Equals(_modeSocketOffsetFrom))
	{
		_isModeSocketOffsetChanging = false;
	}
	else
	{
		_isModeSocketOffsetChanging = true;
		_progressModeSocketOffsetRate = 0.0f;
	}
}
// 캐릭터 삭제 처리
void FGsCameraOffsetGame::RemoveLocalPlayer()
{
	_local = nullptr;
	_targetObj = nullptr;
}

// 소켓 오프셋 갱신
void FGsCameraOffsetGame::Update(float In_deltaTime)
{
	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}

	if (false == _isModeSocketOffsetChanging)
	{
		return;
	}

	// 속도
	float speed =
		GData()->GetGlobalData()->CamModeChangeRotLerpSpeed;

	float addRate = In_deltaTime * speed;
	_progressModeSocketOffsetRate = FMath::Clamp<float>(_progressModeSocketOffsetRate + addRate, 0.f, 1.f);
	UCurveFloat* curve = GData()->GetCamFollowCurve();

	float curveVal = 0.0f;
	if (nullptr != curve)
	{
		curveVal = FMath::Clamp<float>(curve->GetFloatValue(_progressModeSocketOffsetRate), 0.f, 1.f);
	}

	FVector curveNewOffset =
		FMath::Lerp(_modeSocketOffsetFrom, _modeSocketOffsetTo, curveVal);

	if (false == curveNewOffset.Equals(_modeSocketOffsetTo, 0.1f))
	{

	}
	else
	{
		_isModeSocketOffsetChanging = false;
	}
	localChar->GetSpringArm()->SocketOffset = curveNewOffset;
	
}

void FGsCameraOffsetGame::SetDirectSocketOffset(FVector In_offsetTo)
{
	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return;
	}

	localChar->GetSpringArm()->SocketOffset = In_offsetTo;
}