#include "GsCameraLocationGame.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Camera/GsCameraModeHandler.h"

#include "GameObject/Camera/Mode/GsCameraModeDialog.h"

#include "GameObject/Camera/Work/Rotation/GsCameraRotationDialog.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "GameFramework/PlayerController.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Management/ScopeGame/GsCameraModeManager.h"

#include "Camera/Define/GsCameraGameDefine.h"

#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "Engine/Public/CollisionQueryParams.h"
#include "PhysicsCore/Public/CollisionShape.h"

void FGsCameraLocationGame::Initialize()
{
	ProbeChannel = ECollisionChannel::ECC_Camera;
	ProbeSize = 12.0f;
}

void FGsCameraLocationGame::Finalize()
{
	RemoveLocalPlayer();
	_owner = nullptr;
}

void FGsCameraLocationGame::RemoveLocalPlayer()
{
	_local = nullptr;
	_targetObj = nullptr;
}

// 업데이트
void FGsCameraLocationGame::Update(float In_deltaTime)
{

	if (_isAfterDialog == true)
	{
		UpdateLookAt(In_deltaTime);
		return;
	}

	if (_isFinish == true)
	{
		return;
	}
	
	float moveSpeed= _nowSpeed;

	float addRate = In_deltaTime * moveSpeed;
	_progressRate = FMath::Clamp<float>(_progressRate + addRate, 0.f, 1.f);
	UCurveFloat* curve = GData()->GetCamFollowCurve();

	float curveVal = 0.0f;
	if (nullptr != curve)
	{
		curveVal = FMath::Clamp<float>(curve->GetFloatValue(_progressRate), 0.f, 1.f);
	}

	FVector newPos = FMath::Lerp(_locationFrom, _locationTo, curveVal);

	// 목적지 값이랑 같은가
	if (false == newPos.Equals(_locationTo, 0.1f))
	{
	}
	else
	{
		// 도달 하면 flag 변환
		newPos = _locationTo;
		_isFinish = true;
	}

	SetDirectPos(newPos);

	if (_isFinish == true)
	{
		OnFinishLerp();
	}		
	
}

// 목표 위치 도달
void FGsCameraLocationGame::OnFinishLerp()
{
	// is use callback 
	if ( _isUseFinishCallback == false||
		nullptr == _owner)
	{
		return;
	}	
	_owner->OnFinishLerpType(EGsDialogCameraFinishType::Location);		
}


// 회전속도 구하기
float FGsCameraLocationGame::GetRotSpeed()
{
	float speed = 0.0f;
	if (_isAfterDialog == true)
	{
		speed =
			GData()->GetGlobalData()->CamDialogLocLerpSpeed;
	}
	else
	{
		speed =
			GData()->GetGlobalData()->CamModeChangeRotLerpSpeed;
	}
	return speed;
}
// 속도 만들기
void FGsCameraLocationGame::MakeSpeed()
{
	if (_isAfterDialog == true)
	{
		// 회전과 이동 같은 속도로 처리하게 하기위해 외부에서 밀어넣음
	}
	else
	{
		_nowSpeed = GetRotSpeed();
	}
}

// 로직 함수
float FGsCameraLocationGame::MakeDistSpeed()
{
	float dist =
		FVector::Dist(_locationTo, _locationFrom);

	if (dist == 0.f)
	{
		dist = 1.0f;
	}

	// 거리가 멀수록 느려짐(rate 로 진행되므로 일정한 속도 만든다)
	float nowSpeed = GetRotSpeed() / dist;
	return nowSpeed;
}

void FGsCameraLocationGame::SetData(float In_distTo, float In_distFrom,
	const FVector& In_lookTargetTo,
	const FVector& In_lookTargetFrom,
	const FVector& In_moveTo, const FVector& In_moveFrom,
	bool In_isNowMove)
{
	_locationTo = In_moveTo;
	_locationFrom = In_moveFrom;

	// 이동하기위해서 초기화
	_isFinish = !In_isNowMove;
	// 진행도 초기화
	_progressRate = 0.0f;

	_camDistFrom = In_distFrom;
	_camDistTo = In_distTo;

	_lookTargetTo = In_lookTargetTo;
	_lookTargetFrom = In_lookTargetFrom;

	MakeSpeed();
}

// 바라보면서 업데이트
void FGsCameraLocationGame::UpdateLookAt(float In_deltaTime)
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
	if (FMath::IsNearlyEqual(newDist , _camDistTo, 0.01f) == false ||
		nowLookTarget.Equals( _lookTargetTo, 0.1f) == false )
	{
	}
	else
	{
		newDist = _camDistTo;
		nowLookTarget = _lookTargetTo;

		// 도달 하면 flag 변환
		_isFinish = true;
	}

	FRotator camRot = FRotator::ZeroRotator;


	if (_owner != nullptr)
	{
		FGsCameraRotationBase* rotationWork =
			_owner->GetCameraRotationBase();

		if (rotationWork != nullptr)
		{
			camRot = rotationWork->GetCurrentRotator();
		}
	}

	FVector currCamVec = camRot.Vector();
	currCamVec.Normalize();
	FVector newPos = nowLookTarget - newDist * currCamVec;

	// camera hit check

	if (_targetObj == nullptr)
	{
		return;
	}

	AActor* targetActor = _targetObj->GetActor();

	if (targetActor == nullptr)
	{
		return;
	}

	UWorld* world = targetActor->GetWorld();
	if (world == nullptr)
	{
		return;
	}

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SpringArm), false, targetActor);

	FHitResult Result;
	world->SweepSingleByChannel(Result, nowLookTarget, newPos, FQuat::Identity, ProbeChannel, FCollisionShape::MakeSphere(ProbeSize), QueryParams);
	
	FVector ResultLoc = BlendLocations(newPos, Result.Location, Result.bBlockingHit, In_deltaTime);

	SetDirectPos(ResultLoc);
	if (_isFinish == true)
	{
		OnFinishLerp();
	}
}
// 위치 설정 바로 하기
void FGsCameraLocationGame::SetDirectPos(const FVector& In_moveTo)
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
		if (_locationSpaceType == EGsCameraLocationSpaceType::WorldLocation)
		{
			camComponent->SetWorldLocation(In_moveTo);
		}
		else
		{
			camComponent->SetRelativeLocation(In_moveTo);
		}
	}
}

void FGsCameraLocationGame::SetDataNormal(
	const FVector& In_moveTo, const FVector& In_moveFrom)
{
	_locationTo = In_moveTo;
	_locationFrom = In_moveFrom;

	// 이동하기위해서 초기화
	_isFinish = false;
	// 진행도 초기화
	_progressRate = 0.0f;

	MakeSpeed();
}

FVector FGsCameraLocationGame::BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime)
{
	return bHitSomething ? TraceHitLocation : DesiredArmLocation;
}