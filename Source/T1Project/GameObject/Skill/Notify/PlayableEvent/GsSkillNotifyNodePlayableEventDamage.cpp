#include "GsSkillNotifyNodePlayableEventDamage.h"

#include "GameObject/ObjectClass/GsGameObjectPlayableEventLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectPlayableEventNonPlayer.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"

#include "Management/ScopeGlobal/GsSkillManager.h"

#include "Management/ScopeLobby/GsLobbyCharacterManager.h"
#include "Management/ScopeLobby/GsLobbyPlayableEventManager.h"

#include "ActorEx/GsCharacterBase.h"

#include "ActorComponentEx/GsSkillCollisionComponent.h"
#include "ActorComponentEx/GsPlayableEventNonPlayerComponent.h"

#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/Skill/GsSchemaSkillCollision.h"

#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Util/GsLevelUtil.h"

#include "Classes/GsSpawn.h"

#include "GameFramework/Actor.h"

#include "Kismet/GameplayStatics.h"

FGsSkillNotifyNodePlayableEventDamage::FGsSkillNotifyNodePlayableEventDamage(UParticleSystem* ParticleEffect, const FSoftObjectPath& EffectPath)
	: Super(ParticleEffect, EffectPath)
{

}

void FGsSkillNotifyNodePlayableEventDamage::Action(UGsGameObjectBase* Owner)
{
	UGsGameObjectPlayableEventLocalPlayer* local = LSLobbyCharacter()->GetPlayableEventLocalPlayer();
	if (local == nullptr)
	{
		return;
	}

	bool isLocal = (Owner == local) ? true : false;

	AActor* actor = Owner->GetActor();
	UGsSkillCollisionComponent* skillCollsionComponent = Cast<UGsSkillCollisionComponent>(
		actor->GetComponentByClass(UGsSkillCollisionComponent::StaticClass()));

	if (skillCollsionComponent == nullptr)
	{
		return;
	}
	

	if (isLocal == true)
	{
		// preview 꺼 판정은 안쓴다
		FGsSkillNotifyNodeCollision::Action(Owner);
	}
	else
	{
		// 몬스터는 왜인지 모르겠지만 Skill collision handler component 를 안써서 데이터 못 넘겨줌
		// 거기서하던거 직접 처리
		FGsSkillNotifyNodeBase::Action(Owner);
		

		FVector effectPos = _netData ? _netData->_aimTargetPos : FVector::ZeroVector;

		const FGsSchemaSkillCollision* collisionInfo = &_currentSkillData->collisionInfoList.Last();
		// 스킬 레벨이 1이 아닌경우 Collision정보 찾기
		if (_netData->_skillLevel > 1)
		{
			if (const FGsSchemaSkillCollision* findCollisionInfo =
				UGsSkillManager::FindSkillLevelSetToColliosion(_currentSkillData->id, _netData->_skillLevel))
			{
				collisionInfo = findCollisionInfo;
			}
		}

		UGsGameObjectBase* target = local;
		skillCollsionComponent->Set(_currentSkillData->collisionInfoList.Last());

		effectPos = skillCollsionComponent->GetCollisionLocation();
		// 지형 높이 보정
		FHitResult hit;
		UGsLevelUtil::TryLineTraceToLand(hit, Owner->GetWorld(), effectPos);
		effectPos = hit.Location;
		

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



	TArray<UGsGameObjectBase*> hitList;

	// 사용자가 로컬이면 타겟은 몬스터
	if (Owner == local)
	{
		TArray<UGsGameObjectPlayableEventNonPlayer*> targetList = LSLobbyPlayableEvent()->GetAllMonsters();
		for (int i = 0; i < targetList.Num(); ++i)
		{
			// 타겟이 죽었나
			UGsGameObjectPlayableEventNonPlayer* target = targetList[i];

			if (target == nullptr)
			{
				continue;
			}
			AGsCharacterBase* targetChar = target->GetCharacter();
			if (targetChar == nullptr)
			{
				continue;
			}

			UGsPlayableEventNonPlayerComponent* playableEventNonPlayerComponent =
				targetChar->FindComponentByClass<UGsPlayableEventNonPlayerComponent>();

			if (playableEventNonPlayerComponent == nullptr)
			{
				continue;
			}

			if (playableEventNonPlayerComponent->IsAlive() == false)
			{
				continue;
			}

			float raidus = target->GetData()->GetScaledCapsuleRadius();
			if (false == skillCollsionComponent->IsInsideCheck(target->GetLocation(), raidus))
			{
				continue;
			}

			hitList.Emplace(target);
		}
	}
	else
	{
		UGsGameObjectBase* target = local;
		float raidus = target->GetData()->GetScaledCapsuleRadius();
		if (true == skillCollsionComponent->IsInsideCheck(target->GetLocation(), raidus))
		{
			hitList.Emplace(target);
		}			
	}

	hitList.Sort([local](const UGsGameObjectBase& lhs, const UGsGameObjectBase& rhs)
		{
			return lhs.GetTargetDistSquared(local) < rhs.GetTargetDistSquared(local);
		});

	for (int i = 0; i < hitList.Num(); ++i)
	{
		if (_currentSkillData->targetFilterInfo.targetCount <= i)
		{
			break;
		}
		// 데미지 처리		
		// 데미지 정보는 출력할 필요없으므로 설정 하지 않음
		hitList[i]->OnHit(Owner, nullptr, _netData->_skillId, _currNotifyData->id,
			HitResultType::NORMAL, DamageByType::SKILL, DamageStatType::MAX, HealByType::MAX, 0);

	}
}