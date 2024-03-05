// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillNotifyNodeCollision.h"
#include "Kismet/GameplayStatics.h"

#include "ActorComponentEx/GsSkillCollisioHandlerComponent.h"

#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Stat/GsStatBase.h"
#include "GameObject/Skill/Data/GsSkillDataBase.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"

#include "Management/ScopeGlobal/GsSkillManager.h"
#include "DataSchema/Skill/GsSchemaSkillCollision.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "UTIL/GsLevelUtil.h"
#include "Classes/GsSpawn.h"


FGsSkillNotifyNodeCollision::FGsSkillNotifyNodeCollision(UParticleSystem* ParticleEffect, const FSoftObjectPath& EffectPath)
{
	SetData(ParticleEffect, EffectPath);
}

void FGsSkillNotifyNodeCollision::Initialize(UGsGameObjectBase* Owner, const IGsSkillRunnerData* SkillRunnerData,
	const FGsSchemaSkillNotify* NotifyInfo, const FGsSkillNetData* NetData, float OffsetRate)
{
	Super::Initialize(Owner, SkillRunnerData, NotifyInfo, NetData, OffsetRate);
	
	if (SkillRunnerData->GetType() == ActionCategory::SKILL)
	{
		if (const FGsSkillRunnerSKillData* skillData = static_cast<const FGsSkillRunnerSKillData*>(SkillRunnerData))
		{
			_currentSkillData = skillData->GetTableData();
			//_isSpecialSkill = skillData->GetCollisitonCenterType() == SkillAreaCenter::CURSOR;

			// NonTargetType일때 사거리 스탯 얻기
			if (_currentSkillData->skillTargetMethod == SkillTargetMethod::NON_TARGET)
			{
				FGsGameObjectDataCreature* creatureData = Owner->GetCastData<FGsGameObjectDataCreature>();
				TSharedPtr<FGsStatBase> statBase = creatureData->GetStatBase();
				if (const FGsStatInfo* info = statBase->GetStatInfo(StatType::BONUS_APPROACH_RANGE_RATE))
				{
					_collisionAddRange = info->GetStatValue();
				}
				if (const FGsStatInfo* info = statBase->GetStatInfo(StatType::BONUS_APPROACH_RANGE))
				{
					int scaleVal = info->GetStatValue();
					if (scaleVal > 0)
					{
						_collisionScale = (1 + scaleVal / 10000);
					}
				}
			}
		}
	}
}

void FGsSkillNotifyNodeCollision::SetData(class UParticleSystem* ParticleEffect, const FSoftObjectPath& EffectPath)
{
	_effect = ParticleEffect;
	_effectPath = EffectPath;
}

void FGsSkillNotifyNodeCollision::Action(UGsGameObjectBase* Owner)
{
	Super::Action(Owner);

	AActor* actor = Owner->GetActor();
	FVector effectPos = _netData ? _netData->_aimTargetPos : FVector::ZeroVector;
	if (UGsSkillCollisioHandlerComponent* component = Cast<UGsSkillCollisioHandlerComponent>(
		actor->GetComponentByClass(UGsSkillCollisioHandlerComponent::StaticClass())))
	{
		FGsTargetHandlerBase* targetMgr = Owner->GetTargetHandler();
		check(targetMgr);

		FGsSchemaSkillCollision collisionInfo;
		// 스킬 레벨이 1이 아닌경우 Collision정보 찾기
		if (_netData->_skillLevel > 1)
		{
			if (const FGsSchemaSkillCollision* findCollisionInfo = 
				UGsSkillManager::FindSkillLevelSetToColliosion(_currentSkillData->id, _netData->_skillLevel))
			{
				collisionInfo = *findCollisionInfo;
			}
		}
		else
		{
			if (_currentSkillData->collisionInfoList.Num() <= 0)
			{
				return;
			}
			collisionInfo = _currentSkillData->collisionInfoList.Last();
		}

		// 스탯 반영
		switch (collisionInfo.type)
		{
		case SkillCollisionType::CIRCLE:
		case SkillCollisionType::FAN:
			collisionInfo.maxRadius += _collisionAddRange;
			collisionInfo.maxRadius *= _collisionScale;
			break;
		case SkillCollisionType::RECT:
			collisionInfo.height += _collisionAddRange;
			collisionInfo.height *= _collisionScale;
			break;
		}

		UGsGameObjectBase* target = targetMgr->GetTarget();
		component->Set(Owner, collisionInfo, _currentSkillData->targetFilterInfo.targetCount,
			target);

		effectPos = component->GetCurrentPos();
		// 지형 높이 보정
		FHitResult hit;
		UGsLevelUtil::TryLineTraceToLand(hit, Owner->GetWorld(), effectPos);
		effectPos = hit.Location;
	}
	
	// 효과 처리
	if (_effect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			Owner->GetWorld(),
			_effect,
			effectPos,
			FRotator::ZeroRotator,
			true,
			EPSCPoolMethod::AutoRelease);
	}

	if (_effectPath.IsValid())
	{
		FString path = _effectPath.ToString();
		path += TEXT("_C");
		if (UClass* bpClass = LoadObject<UClass>(nullptr, *path))
		{
			GsSpawn::BPClass(Owner->GetWorld(), bpClass, effectPos, FRotator::ZeroRotator);
		}
	}
}