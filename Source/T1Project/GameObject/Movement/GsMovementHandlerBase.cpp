// Fill out your copyright notice in the Description page of Project Settings.

#include "GsMovementHandlerBase.h"
#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "ActorEx/GsCharacterBase.h"
#include "ActorComponentEx/GsQueryFloorHeightComponent.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"

#include "DataSchema/Skill/RootMotion/GsSchemaRootmotionSet.h"

#include "UTIL/GsDir.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsLevelUtil.h"

#include "LocalPlayer/GsMovementNetSender.h"

#if UE_EDITOR
#include "DrawDebugHelpers.h"
#endif

void FGsMovementHandlerBase::Initialize(UGsGameObjectBase* inOwner)
{
	_owner = inOwner;
	check(_owner);
}

void FGsMovementHandlerBase::InitializeActor(AActor* Owner)
{
	AGsCharacterBase* character = Cast<AGsCharacterBase>(Owner);
	if (nullptr != character)
	{
		if (UCharacterMovementComponent* moveComponent = Cast<UCharacterMovementComponent>(character->GetMovementComponent()))
		{
			moveComponent->MaxWalkSpeed = (float)GetMoveSpeed();
			// 이현우 차석님의 최적화 작업으로 이동 처리에필요한 다른 설정값을 이용하여 처리하여 대응
			// https://jira.com2us.com/wiki/pages/viewpage.action?pageId=264716723
			// 추후 다른 문제가 발생한다면 AISupport 플러그인을 다시 활성화 시켜 복구한다.
			moveComponent->bRunPhysicsWithNoController = true;

			// bak1210 :
			// 해당로직은 가감속처리시 동기화가 깨질것이 우려되어 입력된 코드이나,
			// 자동이동 (UPathFollowComponent) 으로 엣지 이동시 이동량 계산이 튀는 현상이 발견되어서 주석으로 막음
			// Accerleration 처리가 활성화되면, 남은거리가 짧아질때 속도를 줄여서 거리를 넘지않게 처리되고 있었음, => 좌표튀는 이슈가 없는 이유
			/*moveComponent->bRequestedMoveUseAcceleration = false;*/
		}
		_queryFloorHeightComponent = character->GetQueryFloorHeightComponent();
	}
}

UCharacterMovementComponent* FGsMovementHandlerBase::GetMovementComponent() const 
{
	AGsCharacterBase* character = GetCharacter();
	if (nullptr == character)
	{
		return nullptr;
	}

	return Cast<UCharacterMovementComponent>(character->GetMovementComponent());
}


AGsCharacterBase* FGsMovementHandlerBase::GetCharacter() const
{
	if (nullptr == _owner)
	{
		return nullptr;

	}
	return Cast<AGsCharacterBase>(_owner->GetActor());
}

void FGsMovementHandlerBase::Finalize()
{
}

void FGsMovementHandlerBase::InitSpeed(int32 inMoveSpeed, int32 inWalkSpeed)
{
	_moveWalkSpeed = inWalkSpeed;
	SetSpeedMovementComponent(inMoveSpeed);
}

void FGsMovementHandlerBase::Update(float inDelta)
{
	if (nullptr != _active)
	{
		_active->Update(inDelta);
	}	
}

void FGsMovementHandlerBase::Stop(bool inSendFsmMessage)
{
	SetMoveState(false);
	if (nullptr != _active)
	{
		_active->Stop(_owner->CastGameObject<UGsGameObjectCreature>(), inSendFsmMessage);
	}
}

//void FGsMovementHandlerBase::Reset(uint16 inDir, const FVector& inPos)
//{
//	if (nullptr == _character) return;
//
//	if (UCharacterMovementComponent* moveComponent = Cast<UCharacterMovementComponent>(_character->GetMovementComponent()))
//	{
//		moveComponent->StopActiveMovement();
//	}
//}

void FGsMovementHandlerBase::DirectMoveUpdate(const FVector& inPos, const FRotator& inRot)
{
	// 루트 모션등을 케릭터가 없을때 시행하면 위치 처리
	if (_owner->GetActorState() == ActorState::LOADING)
	{
		_owner->SetActorLocation(inPos);
		_owner->SetActorRotation(inRot);
	}

	if (AGsCharacterBase* character = GetCharacter())
	{
		if (UCapsuleComponent* capsuleComponet = character->GetCapsuleComponent())
		{
			float halfHeight = 88.f;
			float radius = 60.f;
			capsuleComponet->GetScaledCapsuleSize(radius, halfHeight);
			FHitResult hit;
			if (UGsLevelUtil::TrySweepToLand(hit, character->GetWorld(), inPos, radius, halfHeight))
			{
				FVector reCalcPos = hit.ImpactPoint + FVector(0, 0, halfHeight);
				character->SetActorLocationAndRotation(reCalcPos, inRot, false, &hit, ETeleportType::TeleportPhysics);
				return;
			}
		}
		// _character->TeleportTo(inPos, inRot);
	}
}

void FGsMovementHandlerBase::DirectMoveUpdate(const FVector& inPos, const FVector& inDir)
{
	DirectMoveUpdate(inPos, inDir.Rotation());
}

bool FGsMovementHandlerBase::ChangeState(FGsMovementState& inState)
{
	if (nullptr != _active)
	{
		if (_active->GetType() == inState.GetType())
		{
			return false;
		}

		if (false == _active->CanTransition(inState.GetType()))
		{
			return false;
		}

		_active->Exit();
	}	
	_active = &inState;
	_active->Enter();
	return true;

}

// 서버에서 이동정지명령을 내릴때 처리
// 리즌별로 처리는 나중에 기획에 맞춰서
void FGsMovementHandlerBase::ReceveNetMotionStopReason(MotionStopReason inReason)
{
	if (UCharacterMovementComponent* moveComponent = GetMovementComponent())
	{
		moveComponent->StopActiveMovement();
	}
}

// 현재 상태 얻기
EGsMovementState FGsMovementHandlerBase::GetState() const
{
	if (nullptr == _active)
	{
		return EGsMovementState::STOP;
	}

	return _active->GetType();
}



void FGsMovementHandlerBase::SetSpeedMovementComponent(StatValue inSpeed)
{
	if (UCharacterMovementComponent* moveComponent = GetMovementComponent())
	{
		moveComponent->MaxWalkSpeed = inSpeed;
	}
	else
	{
		_moveMoveSpeed = inSpeed;
	}
}

int FGsMovementHandlerBase::GetMoveSpeed() const
{
	if (UCharacterMovementComponent* moveComponent = GetMovementComponent())
	{
		return moveComponent->MaxWalkSpeed;
	}
	return _moveMoveSpeed;
}

const MotionType FGsMovementHandlerBase::GetMotionType() const
{
	if (nullptr == _active)
	{
		return MotionType::STOP;
	}

	return _active->GetMotionType();
}