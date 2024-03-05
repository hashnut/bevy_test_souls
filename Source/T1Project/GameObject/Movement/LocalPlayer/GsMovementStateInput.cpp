#include "GsMovementStateInput.h"

#include "GameFramework/Controller.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorComponentEx/GsQueryFloorHeightComponent.h"

#include "Message/GsMessageGameObject.h"
#include "Management/GsMessageHolder.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"

#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "GsMovementNetSender.h"

#include "Util/GsDir.h"

float FGsMovementStateInput::CacheChangeScaleRateRunWalk = 0.2f;

//---------------------------------------------------------------------------------------------
// FGsMovementState
//---------------------------------------------------------------------------------------------
int16 FGsMovementStateInput::GetDirNetConverted() const
{
	return FGsDir::dirToAngle(GetDirection());
}

float FGsMovementStateInput::GetFallingSpeed() const
{
	return 0.0f;
}

const FVector& FGsMovementStateInput::GetLocation() const
{
	_location  = _characterLocal->GetMovementComponent()->GetActorFeetLocation();
	return _location;
}

const FVector& FGsMovementStateInput::GetDestination() const 
{
	// 목적위치 계산
	_destination = GetLocation() + (GetDirection() * _moveSpeed * FGsMovementNetSender::SendDestinationTime);
	_characterLocal->GetQueryFloorHeightComponent()->TryGetFloor(_destination, _destination);
	return _destination;
}

//-------------------------------------------------------------------------------------------------
// TGsState
//-------------------------------------------------------------------------------------------------
void FGsMovementStateInput::Enter()
{
	check(_characterLocal);	
}

void FGsMovementStateInput::Exit()
{
	ClearMoveScale();
}

void FGsMovementStateInput::Update(float inDeltatime)
{
	if (false == IsMovableScaleValue())
	{
		Stop(_owner, true);
	}
	else
	{
		AddMovementInput(_characterLocal);
	}
}

//-------------------------------------------------------------------------------------------------
// FGsMovementStateInput
//-------------------------------------------------------------------------------------------------
void FGsMovementStateInput::Initialize(UGsGameObjectLocalPlayer* In_local)
{
	_owner = In_local;
}

void FGsMovementStateInput::Start(UGsGameObjectLocalPlayer* inOwner, const FVector& inDir)
{	
	SetDirection(inDir);
	Start(inOwner);
}

void FGsMovementStateInput::Start(UGsGameObjectLocalPlayer* inOwner)
{
	check(inOwner);
	_owner = inOwner;
	//상태 전환
	if (IsMovableScaleValue() && 
		false == inOwner->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT))
	{
		FGsGameObjectStateManager* fsm = inOwner->GetBaseFSM();
		check(fsm);
		fsm->ProcessEvent(IsRunnableScaleValue() ? EGsStateBase::Run : EGsStateBase::ForwardWalk);
	}
}

void FGsMovementStateInput::Stop(class UGsGameObjectCreature* inOwner, bool inSendFsmMessage)
{
	FGsMovementState::Stop(inOwner, inSendFsmMessage);

	// 이동 종료 알림 Message
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_INPUT_MOVE_END, nullptr);
}

void FGsMovementStateInput::SetDirecitonFromMoveScale(const FVector2D& inMoveScale)
{
	SetMoveScale(inMoveScale);	

	FVector newDir = _characterLocal->Controller->GetControlRotation().RotateVector(
		FVector(_moveScale.X, _moveScale.Y, 0.f).GetClampedToMaxSize(1.0f));

	float fDot = 0.0f;
	if (nullptr != _oppositeDirecitonHandle)
	{
		fDot = FVector::DotProduct(GetDirection(), newDir);
		if (-0.2f > fDot)
		{
			_oppositeDirecitonHandle();
		}
	}
	SetDirection(newDir);
	
}

void FGsMovementStateInput::AddMovementInput(AGsCharacterLocalPlayer* inTarget)
{
	check(inTarget);

	float scaleSizeSquared = _moveScale.SizeSquared();
	// 이동 입력 있는가 확인
	if (scaleSizeSquared > 0.f)
	{
		// 걷기 영역일 경우와 뛰기 영역일때 Speed값을 제어
		_owner->UpdateSpeedMovementHandler();

		// Speed를 조절 했으므로 입력 Scale값은 무조건 1
		inTarget->AddMovementInput(GetDirection(), 1.f);
	}
	else
	{
		inTarget->AddMovementInput(GetDirection(), 0.f);
	}
}


void FGsMovementStateInput::SetActorRotationFromJoyAxis(AGsCharacterLocalPlayer* inTarget, const FVector& inDir)
{
	check(inTarget);
	SetDirection(inTarget->Controller->GetControlRotation().RotateVector(
		FVector(inDir.X, inDir.Y, 0.f).GetClampedToMaxSize(1.0f)));

	FRotator Rot = FRotationMatrix::MakeFromX(GetDirection()).Rotator();
	inTarget->SetActorRotation(Rot);
}

bool FGsMovementStateInput::IsRunnableScaleValue() const
{
	float scaleSizeSquared = _moveScale.SizeSquared();
	return (scaleSizeSquared >= FMath::Square(CacheChangeScaleRateRunWalk)) ? true : false;
}

const MotionType FGsMovementStateInput::GetMotionType() const
{
	if (_owner->IsVehicleMode())
	{
		return MotionType::VEHICLE;
	}

	if (false == IsRunnableScaleValue())
	{
		return MotionType::WALK;
	}

	return MotionType::NORMAL;
}