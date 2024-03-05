// Fill out your copyright notice in the Description page of Project Settings.

#include "GsSkillNotifyNodePreviewDamage.h"

#include "Shared/Client/SharedEnums/SharedSkillNotifyEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "ActorComponentEx/GsSkillCollisionComponent.h"

#include "Management/ScopeGame/GsSkillPreviewManager.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Skill/Data/GsSkillDataBase.h"
#include "GameObject/Data/GsGameObjectData.h"

#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHitSet.h"

FGsSkillNotifyPreviewDamage::FGsSkillNotifyPreviewDamage(UParticleSystem* ParticleEffect, const FSoftObjectPath& EffectPath)
	: Super(ParticleEffect, EffectPath)
{
}

void FGsSkillNotifyPreviewDamage::Initialize(UGsGameObjectBase* Owner, const IGsSkillRunnerData* SkillRunnerData,
	const FGsSchemaSkillNotify* NotifyInfo, const FGsSkillNetData* NetData, float OffsetRate)
{
	Super::Initialize(Owner, SkillRunnerData, NotifyInfo, NetData, OffsetRate);

	if (SkillRunnerData->GetType() == ActionCategory::SKILL)
	{
		if (const FGsSkillRunnerSKillData* skillData = static_cast<const FGsSkillRunnerSKillData*>(SkillRunnerData))
		{
			_currentSkillData = skillData->GetTableData();
			//_isSpecialSkill = skillData->GetCollisitonCenterType() == SkillAreaCenter::CURSOR;
		}
	}
}

void FGsSkillNotifyPreviewDamage::Action(UGsGameObjectBase* Owner)
{
	Super::Action(Owner);

	AActor* actor = Owner->GetActor();
	if (UGsSkillCollisionComponent* component = Cast<UGsSkillCollisionComponent>(
		actor->GetComponentByClass(UGsSkillCollisionComponent::StaticClass())))
	{
		TArray<UGsGameObjectBase*> targetList;
		const FGsSchemaSkillTargetFilter* targetFilterSet = _currNotifyData->targetFilterInfo.GetRow();
		if (targetFilterSet)
		{
			GSSkillPreview()->GetTargetList(targetList, Owner->GetGameId(), targetFilterSet);
		}
		else
		{
			targetFilterSet = &_currentSkillData->targetFilterInfo;
			GSSkillPreview()->GetTargetList(targetList, Owner->GetGameId(), targetFilterSet);
		}
		
		TArray<UGsGameObjectBase*> hitList;
		// Target타입은 모든 대상을 hit
		if (_currentSkillData->skillTargetMethod == SkillTargetMethod::TARGET)
		{
			hitList = targetList;
		}
		else
		{
			for (int i = 0; i < targetList.Num(); ++i)
			{
				FGsGameObjectData* gameObjectData = targetList[i]->GetData();
				float raidus = gameObjectData->GetScaledCapsuleRadius();
				if (false == component->IsInsideCheck(targetList[i]->GetLocation(), raidus))
				{
					continue;
				}

				hitList.Emplace(targetList[i]);
			}
		}

		for (UGsGameObjectBase* el : hitList)
		{
			GSSkillPreview()->ExecuteNotify(_netData->_skillId, Owner->GetGameId(), el->GetGameId(), _currNotifyData, 0);
		}
	}
}