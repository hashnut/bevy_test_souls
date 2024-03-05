// Fill out your copyright notice in the Description page of Project Settings.

#include "GsSkillNotifyNodePreviewProjectile.h"

#include "ActorComponentEx/GsQueryFloorHeightComponent.h"

#include "Management/ScopeGame/GsSkillPreviewManager.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Data/GsGameObjectData.h"

#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyProjectile.h"
#include "DataSchema/GameObject/Projectile/GsSchemaProjectileSet.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsDir.h"

FGsSkillNotifyNodePreviewProjectile::FGsSkillNotifyNodePreviewProjectile(IffApplyType SkillIffType, uint32 SkillId)
{
	_skillIffType = SkillIffType;
	_skillId = SkillId;
}

void FGsSkillNotifyNodePreviewProjectile::Action(UGsGameObjectBase* Owner)
{
	Super::Action(Owner);

	// 리스트 형태이지만 리스트로 활용하지 않음. 0번만 사용한다
	const FGsSchemaSkillNotifyElement* element = _currNotifyData->notifyElementList[0].GetRow();
	if (const FGsSchemaSkillNotifyProjectile* projectileNotify = element->typeInfo.GetRowByType<FGsSchemaSkillNotifyProjectile>())
	{
		const FGsSchemaProjectileSet* projectileData = UGsTableUtil::FindRowById<
			UGsTableProjectileSet, FGsSchemaProjectileSet>(projectileNotify->projecttileId);
		if (nullptr == projectileData)
		{
			return;
		}
		
		UGsGameObjectBase* spawnTarget = nullptr;
		FVector spawnPos = FVector::ZeroVector;
		if (projectileData->spawnType == ProjectileSpawnType::CASTER)
		{
			spawnTarget = Owner;			
		}
		else if (projectileData->spawnType == ProjectileSpawnType::TARGET)
		{
			// 타겟 정보 얻기
			TArray<UGsGameObjectBase*> targetList;
			if (const FGsSchemaSkillTargetFilter* targetFilterSet = _currNotifyData->targetFilterInfo.GetRow())
			{
				GSSkillPreview()->GetTargetList(targetList, Owner->GetGameId(), targetFilterSet);
			}
			else
			{
				GSSkillPreview()->GetTargetList(targetList, Owner->GetGameId(), _skillIffType);
			}

			if (targetList.Num() == 0)
			{
				return;
			}
			spawnTarget = targetList[0];
		}

		if (nullptr != spawnTarget)
		{
			spawnPos = spawnTarget->GetLocation();
			float capsuleY = spawnTarget->GetData()->GetScaledCapsuleHalfHeight();
			spawnPos.Z -= capsuleY + projectileNotify->fireHeightOffset;
			FVector ownerDir = spawnTarget->GetRotation().Vector().GetSafeNormal();
			spawnPos += ownerDir * projectileNotify->fireOffset;
		}
		else
		{
			// 랜덤 포지션 설정

		}

		int netDir = projectileNotify->initAngleOffset;
		for (int i = 0; i < projectileNotify->fireCount; ++i)
		{
			FVector dir = Owner->GetRotation().RotateVector(FGsDir::angleToDir(netDir));
			GSSkillPreview()->SpawnProjectile(projectileData, spawnPos, dir, _skillId);
			netDir += projectileNotify->perAngleOffset;
		}
	}
}