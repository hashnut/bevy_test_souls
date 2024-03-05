// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillCollisioHandlerComponent.h"
#include "Engine/Public/TimerManager.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "ActorComponentEx/GsSkillCollisionComponent.h"
#include "ActorComponentEx/GsSkillGuideEffectComponent.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "T1Project.h"


/* void UGsSkillCollisioHandlerComponent::ClearTimer()
{
	if (UWorld* world = GetWorld())
	{
		for (FTimerHandle& el : ListTimerHandle)
		{
			world->GetTimerManager().ClearTimer(el);
			el.Invalidate();
		}
	}

	ListTimerHandle.Reset();
}*/

void UGsSkillCollisioHandlerComponent::ResetComponent(UGsGameObjectBase* owner)
{
	if (nullptr == _skillCollisionComponent)
	{
		_skillCollisionComponent = Cast<UGsSkillCollisionComponent>(
			GetOwner()->GetComponentByClass(UGsSkillCollisionComponent::StaticClass()));
	}

	if (nullptr == _skillGuideEffectComponent)
	{
		_skillGuideEffectComponent = Cast<UGsSkillGuideEffectComponent>(
			GetOwner()->GetComponentByClass(UGsSkillGuideEffectComponent::StaticClass()));
	}

	_owner = owner;
	_target = nullptr;
	_ownerCreature = Cast<UGsGameObjectCreature>(owner);
	//ClearTimer();
}

FVector UGsSkillCollisioHandlerComponent::GetCurrentPos() const
{
	return _fixedWorldPos.IsZero() ? _skillCollisionComponent->GetCollisionLocation() : _fixedWorldPos;
}

void UGsSkillCollisioHandlerComponent::EndPlay(EEndPlayReason::Type inType)
{
	_skillCollisionComponent = nullptr;
	_skillGuideEffectComponent = nullptr;

	Super::EndPlay(inType);
}

void UGsSkillCollisioHandlerComponent::Set(UGsGameObjectBase* owner, const FGsSchemaSkillCollision& Data, int TargetCount,
	UGsGameObjectBase* target)
{
	ResetComponent(owner);

	_target = target;
	
	_skillCollisionComponent->Set(Data);
#ifndef HIT_EFFECT_BY_SERVER
	// 임의의 대상 검출
	// 임시로 자신과 다른 타입만 충돌 유효 판정을 한다.
	// 이부분은 추후 디테일한 작업이 필요!
	TArray<UGsGameObjectBase*> spawnList = GSGameObject()->FindObjectArray(EGsGameObjectType::Creature);
	TArray<UGsGameObjectBase*> hitList;
	for (int i = 0; i < spawnList.Num(); ++i)
	{
		if (hitList.Num() >= TargetCount)
		{
			break;
		}

		if (false == spawnList[i]->CanAttack(_owner))
		{
			continue;
		}

		if (false == _skillCollisionComponent->IsInsideCheck(spawnList[i]->GetLocation(),
			spawnList[i]->GetData()->GetScaledCapsuleRadius()))
		{
			continue;
		}

		hitList.Emplace(spawnList[i]);
	}

	for (UGsGameObjectBase* el : hitList)
	{
		TArray<DamageStatInfo> emptyStatInfo;
		el->OnHit(_owner, nullptr, 0, 0, emptyStatInfo);
	}
#endif // HIT_EFFECT_BY_SERVER
	//switch (Data->NotifyType)
	//{
	//case EGsSkillNotifyType::Collision:
	//	SetSingle(Cast<UGsSkillNotifyDataCollision>(Data), (Target) ? Target->GetLocation() : FVector::ZeroVector);
	//	break;
	//case EGsSkillNotifyType::GroupCollision:
	//	SetGroup(Cast<UGsSkillNotifyDataGroupCollision>(Data));
	//	break;
	//case EGsSkillNotifyType::RandomCollision:
	//	SetRandom(Cast<UGsSkillNotifyDataRandomCollision>(Data));
	//	break;
	//}
}

//void UGsSkillCollisioHandlerComponent::SetSingle(const UGsSkillNotifyDataCollision* Data, FVector Offset)
//{
//	FGsSkillHandlerBase* skillhandler = _ownerCreature->GetSkillHandler();
//	if (nullptr == skillhandler)
//	{
//		return;
//	}
//
//	// 예시선 출력
//	//if (nullptr != Data->SkillGuideEffect)
//	//{
//	//	if (_fixedWorldPos.IsZero())
//	//	{
//	//		_skillGuideEffectComponent->Set(Data->SkillGuideEffect);
//	//	}
//	//	else
//	//	{
//	//		_skillGuideEffectComponent->Set(_fixedWorldPos, Data->SkillGuideEffect);
//	//	}
//	//}
//
//	float skillCoefficentValue = 1.f;
//	// 해당 스킬에대한 계수값을 가져옴
//	if (nullptr != _ownerCreature)
//	{
//		FGsSkillRunnerBase* runnerBase = skillhandler->GetSkillRunner();
//		if (nullptr != runnerBase)
//		{
//			const FGsSchemaSkillSet* skillData = runnerBase->GetSkillData();
//			//skillCoefficentValue = skillData->SkillCoefficientValue == 0.f ? 1.f : skillData->SkillCoefficientValue;
//		}
//	}
//
//	// 충돌 시작 딜레이 설정
//	if (UWorld* world = GetWorld())
//	{
//		if (Data->CollisionDalay > 0.f)
//		{
//			FTimerHandle hadle;
//
//			world->GetTimerManager().SetTimer(hadle,
//				FTimerDelegate::CreateUObject(this,
//					&UGsSkillCollisioHandlerComponent::TimerCallbackSingleCollision, Data, Offset, skillCoefficentValue),
//				Data->CollisionDalay,
//				false);
//		}
//		else
//		{
//			TimerCallbackSingleCollision(Data, Offset , skillCoefficentValue);
//		}
//	}
//}

// 현재 그룹 충돌은 따로 위치 정보가 없음
// 일단 타겟 확정으로 구현
//void UGsSkillCollisioHandlerComponent::SetGroup(const UGsSkillNotifyDataGroupCollision* Data)
//{
//	_fixedWorldPos = FVector::ZeroVector;
//	// 타겟 위치 반영
//	if (_target)
//	{
//		_fixedWorldPos = _target->GetLocation();
//		// z값 보정
//		_fixedWorldPos.Z -= _target->GetData()->GetScaledCapsuleHalfHeight();
//	}
//
//	// 예시선 출력
//	//if (nullptr != Data->SkillGuideEffect)
//	//{
//	//	if (_fixedWorldPos.IsZero())
//	//	{
//	//		_skillGuideEffectComponent->Set(Data->SkillGuideEffect);
//	//	}
//	//	else
//	//	{
//	//		_skillGuideEffectComponent->Set(_fixedWorldPos, Data->SkillGuideEffect);
//	//	}
//	//	
//	//}
//
//	// 충돌 정보 설정
//	_listCollisionInfo.Reset();
//	_listCollisionInfo = Data->ListCollisionInfo;
//
//	// 충돌 시작 딜레이 설정
//	if (UWorld* world = GetWorld())
//	{
//		if (Data->GroupCollisionDalay > 0.f)
//		{
//			FTimerHandle hadle;
//
//			world->GetTimerManager().SetTimer(hadle,
//				FTimerDelegate::CreateUObject(this,
//					&UGsSkillCollisioHandlerComponent::TimerCallbackGroupCollision),
//				Data->GroupCollisionDalay,
//				false);
//		}
//		else
//		{
//			TimerCallbackGroupCollision();
//		}
//	}
//}

//void UGsSkillCollisioHandlerComponent::SetRandom(const UGsSkillNotifyDataRandomCollision* Data)
//{
//	// 예시선 출력
//	if (nullptr != Data->SkillGuideEffect)
//	{
//		//_skillGuideEffectComponent->Set(Data->SkillGuideEffect);
//	}
//
//	// 랜덤 픽
//	int randIdx = FMath::RandRange(0, Data->ListGroupCollisionInfo.Num() - 1);
//	if (Data->ListGroupCollisionInfo.IsValidIndex(randIdx))
//	{
//		_pickCollision = Data->ListGroupCollisionInfo[randIdx];
//
//		// 랜덤 시작 딜레이 설정
//		if (UWorld* world = GetWorld())
//		{
//			if (Data->Delay > 0.f)
//			{
//				FTimerHandle hadle;
//
//				world->GetTimerManager().SetTimer(hadle,
//					FTimerDelegate::CreateUObject(this,
//						&UGsSkillCollisioHandlerComponent::TimerCallbackRandomStart),
//					Data->Delay,
//					false);
//			}
//			else
//			{
//				TimerCallbackRandomStart();
//			}
//		}
//	}
//}

//void UGsSkillCollisioHandlerComponent::TimerCallbackRandomStart()
//{
//	if (_pickCollision)
//	{
//		SetGroup(_pickCollision);
//	}
//	_pickCollision = nullptr;
//}
//
//void UGsSkillCollisioHandlerComponent::TimerCallbackGroupCollision()
//{
//	bool isFixed = !_fixedWorldPos.IsZero();
//
//	// 충돌 판정
//	for (UGsSkillNotifyDataCollision* el : _listCollisionInfo)
//	{
//		if (isFixed)
//		{
//			FRotator rot = _skillCollisionComponent->GetCollisionRotate() + FRotator::MakeFromEuler(FVector(0.f, 0.f, el->Angle));
//			FVector offset = rot.Vector() * el->Distance + _fixedWorldPos;
//
//			// 충돌 처리
//			SetSingle(el, offset);
//		}
//		else
//		{
//			SetSingle(el);
//		}
//	}
//	_listCollisionInfo.Reset();
//}

//void UGsSkillCollisioHandlerComponent::TimerCallbackSingleCollision(
//	const UGsSkillNotifyDataCollision* Data, FVector Offset, float SkillCoefficientValue
//)
//{
//	// 충돌 타입에 타른 위치값 처리
//	if (Offset.IsZero())
//	{
//		//SkillCollisionComponent->Set(Data);
//	}
//	else
//	{
//		//SkillCollisionComponent->Set(Offset, Data);
//	}
//
//	// 임의의 대상 검출
//	// 임시로 자신과 다른 타입만 충돌 유효 판정을 한다.
//	// 이부분은 추후 디테일한 작업이 필요!
//	TArray<UGsGameObjectBase*> spawnList = GSGameObject()->FindObjectArray(EGsGameObjectType::Creature);
//	for (UGsGameObjectBase* el : spawnList)
//	{
//		if (el->CanAttack(_owner) &&
//			_skillCollisionComponent->IsInsideCheck(el->GetLocation(), el->GetData()->GetScaledCapsuleRadius()))
//		{
//			//el->OnHit(_owner, SkillCoefficientValue);
//		}
//	}
//}