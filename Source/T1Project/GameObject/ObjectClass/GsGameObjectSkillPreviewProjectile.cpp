// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/ObjectClass/GsGameObjectSkillPreviewProjectile.h"
#include "TimerManager.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Client/SharedEnums/SharedProjectileEnum.h"
#include "ActorEx/GsActorProjectile.h"
#include "ActorComponentEx/GsSkillCollisionComponent.h"
#include "Management/ScopeGame/GsSkillPreviewManager.h"

#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/GameObject/Projectile/GsSchemaProjectileSet.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHitSet.h"

#include "GameObject/Skill/Notify/SkillPreview/GsSkillNotifyNodePreviewProjectile.h"
#include "GameObject/Data/GsGameObjectData.h"

#if WITH_EDITOR
#include "Management/ScopeGame/GsGameObjectManager.h"
#endif

void UGsGameObjectSkillPreviewProjectile::InitializeActor(AActor* Owner)
{
	Super::InitializeActor(Owner);
	
	FVector destPos = Owner->GetActorLocation();
	switch (_projectileData->moveType)
	{
	case ProjectileMoveType::FIXED:
		_actor->SetProjectileSpeed(0.f);
		break;
	case ProjectileMoveType::MOVE_TARGET:
	{
		TArray<UGsGameObjectBase*> targetList;
		GSSkillPreview()->GetTargetList(targetList, GetGameId(), IffApplyType::ENEMY);
		if (targetList.Num() == 0)
		{
			return;
		}
		destPos = targetList[0]->GetLocation();
	}
		break;
	case ProjectileMoveType::EXPLOSION:
		_triggerType = _projectileData->endTriggerType;
		break;
	default:
		break;
	}

	ClearTimerEvent();
	_listActionCallCount.Empty();
	
	for (int i = 0; i < _projectileData->actionInfoList.Num(); ++i)
	{
		// 서버 로직과 동일하게 Action 실행 카운트를 계산한다.
		// (발사체의 유지 시간 - 최초 효과 발동 시간) / 발동 주기 = 효과 발동 횟수
		float callCount = (_projectileData->lifeTime - _projectileData->actionInfoList[i].tickStartOffset) /
			_projectileData->actionInfoList[i].tickInterval;
		// 소숫점 아랫자리에서 잘못 적용 되는 경우가 있어 반올림 처리
		callCount += 0.0005f;
		_listActionCallCount.Emplace((int)callCount);

		// 최초 틱 오프셋 검사
		if (FMath::IsNearlyZero(_projectileData->actionInfoList[i].tickStartOffset))
		{
			CheckCollision(i);
		}

		FTimerHandle newTimerHandler;
		GetWorld()->GetTimerManager().SetTimer(newTimerHandler,
			FTimerDelegate::CreateLambda([this, i]() { CheckCollision(i);}),
			_projectileData->actionInfoList[i].tickInterval, true, _projectileData->actionInfoList[i].tickStartOffset);
		_listTimeHandler.Emplace(newTimerHandler);
	}
}

void UGsGameObjectSkillPreviewProjectile::ActorDespawn()
{
	ClearTimerEvent();

	if (_triggerType == ProjectileTriggerType::EFFECT_END_POINT_DES)
	{
		// 대상은 없다.
		TArray<UGsGameObjectBase*> emptyList;
		for (int i = 0; i < _projectileData->actionInfoList.Num(); ++i)
		{
			Action(&_projectileData->actionInfoList[i], emptyList);
		}
	}
	
	Super::ActorDespawn();
}

void UGsGameObjectSkillPreviewProjectile::ClearTimerEvent()
{
	for (FTimerHandle& el : _listTimeHandler)
	{
		GetWorld()->GetTimerManager().ClearTimer(el);
	}
	_listTimeHandler.Empty();
}

void UGsGameObjectSkillPreviewProjectile::CheckCollision(int Index)
{
	if (_listActionCallCount[Index] <= 0)
	{
		return;
	}

	_listActionCallCount[Index]--;

	// 판정 처리
	for(const FGsSchemaProjectileCollisionInfo& collision : _projectileData->collisionInfoList)
	{
		TArray<UGsGameObjectBase*> hitList;
		GSSkillPreview()->GetTargetList(hitList, GetGameId(), &_projectileData->targetFilterInfo);
		hitList = CheckCollision(collision, hitList);
		if (hitList.Num() > 0)
		{
			// 액션 처리
			Action(&_projectileData->actionInfoList[Index], hitList);

			// 종료
			break;
		}
	}
}

void UGsGameObjectSkillPreviewProjectile::Action(const FGsSchemaProjectileActionInfo* ActionInfo, TArray<UGsGameObjectBase*>& ListHitObject)
{
	if (nullptr == ActionInfo)
	{
		return;
	}

	const FGsSchemaSkillNotifySet* notifySet = ActionInfo->skillNotifyId.GetRow();
	// 노티파이 실행
	if (notifySet)
	{
		if (notifySet->type == SkillNotifyType::FIRE_PROJECTILE)
		{
			const FGsSchemaSkillTargetFilter* targetFilterSet = notifySet->targetFilterInfo.GetRow();
			FGsSkillNotifyNodePreviewProjectile createProjectile(targetFilterSet->iffType, GetSkillId());
			createProjectile.Initialize(this, notifySet, nullptr);
			createProjectile.Action(this);
		}
		// FGsSkillNotifyNodeCollision와 구조가 달라서 따로 구현..
		else
		{
			// 프로젝타일일 경우 SubOwner로 적용
			UGsGameObjectBase* casterObject = GSSkillPreview()->FindObject((int64)EGsSkillPreivewGameObjectStartID::OwnerPlayer);
			UGsGameObjectBase* subObject = this;
			for (UGsGameObjectBase* el : ListHitObject)
			{
				GSSkillPreview()->ExecuteNotify(GetSkillId(), _casterGameId,  el->GetGameId(), notifySet, subObject->GetGameId());
			}
		}
	}

	if (_projectileData->moveType == ProjectileMoveType::EXPLOSION)
	{
		AActor* actor = GetActor();
		// 타이머는 클리어 처리하고 다음 틱에 디스폰 액터를 호출
		ClearTimerEvent();
		GetWorld()->GetTimerManager().SetTimerForNextTick([actor]()
			{
				actor->Destroy();
			});
	}
}

TArray<UGsGameObjectBase*> UGsGameObjectSkillPreviewProjectile::CheckCollision(
	const FGsSchemaProjectileCollisionInfo& Collision, TArray<UGsGameObjectBase*> TargetList)
{
	AActor* actor = GetActor();
	UGsSkillCollisionComponent* component = Cast<UGsSkillCollisionComponent>(
		actor->GetComponentByClass(UGsSkillCollisionComponent::StaticClass()));
	// 없다면 생성
	if (nullptr == component)
	{
		component = NewObject<UGsSkillCollisionComponent>(actor);
		component->SetupAttachment(actor->GetRootComponent());
		component->RegisterComponent();
	}

	FGsSchemaSkillCollision skillCollision;
	skillCollision.type = Collision.type;
	skillCollision.width = Collision.width;
	skillCollision.height = Collision.height;
	skillCollision.minRadius = Collision.minRadius;
	skillCollision.maxRadius = Collision.maxRadius;
	skillCollision.angle = Collision.angle;

	component->Set(skillCollision);

#if WITH_EDITOR
	if (GSGameObject()->_isCollisionDebugShape)
	{
		component->DrawCollision(0.5f);
	}
#endif

	TArray<UGsGameObjectBase*> hitList;
	for (int i = 0; i < TargetList.Num(); ++i)
	{
		FGsGameObjectData* gameObjectData = TargetList[i]->GetData();
		float raidus = gameObjectData->GetScaledCapsuleRadius();
		if (false == component->IsInsideCheck(TargetList[i]->GetLocation(), raidus))
		{
			continue;
		}

		hitList.Emplace(TargetList[i]);
	}

	return hitList;
}

void UGsGameObjectSkillPreviewProjectile::SetSpawnSKillId(uint32 SkillId)
{
	_skillId = SkillId;
}

uint32 UGsGameObjectSkillPreviewProjectile::GetSkillId() const
{
	return _skillId;
}