// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillNotifyNodeBeamParticle.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Target/GsTargetHandlerBase.h"

#include "ActorComponentEx/GsEffectComponent.h"
#include "ActorEx/GsCharacterBase.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyBeamParticle.h"


void FGsSkillNotifyNodeBeamParticle::Initialize(UGsGameObjectBase* Owner, const FGsSchemaSkillNotifySet* NotifySetData,
	const FGsSkillNetData* NetData)
{
	Super::Initialize(Owner, NotifySetData, NetData);

	_owner = Owner;
}

void FGsSkillNotifyNodeBeamParticle::Finalize()
{
	Super::Finalize();

	RemoveTargetBeamParticle();

	_owner = nullptr;
}

void FGsSkillNotifyNodeBeamParticle::SetData(const FGsSchemaSkillNotifyBeamParticle* BeamParticleTable)
{
	_beamParticleEffect = BeamParticleTable;
}

void FGsSkillNotifyNodeBeamParticle::Action(UGsGameObjectBase* Owner)
{
	Super::Action(Owner);

	if (Owner)
	{
		_owner = Owner;
		UGsGameObjectBase* target = FindBeamParticleTarget();
		if (target)
		{
			// 대상을 찾았으면 빔 파티클 활성화
			if (AGsCharacterBase* actor = _owner->GetCharacter())
			{
				if (UGsEffectComponent* effectComponent = actor->GetEffectComponent())
				{
					if (UObject* res = UAssetManager::GetStreamableManager().LoadSynchronous(_beamParticleEffect->effectPath))
					{
						effectComponent->OnBeamParticleEffectActive(Cast<UParticleSystem>(res), target->GetActor(),
							_beamParticleEffect->targetSocketName, _beamParticleEffect->duration);
					}
				}
			}
			// 스폰 델리게이트 제거
			// GMessage()->GetGameObject().Remove(_spawnDelegate);
			//
		}
	}
}

UGsGameObjectBase* FGsSkillNotifyNodeBeamParticle::FindBeamParticleTarget()
{
	if (nullptr == _beamParticleEffect)
	{
		return nullptr;
	}

	if (_beamParticleEffect->targetType == SkillTargetType::TARGET)
	{
		// 내플레이어는 현재 타겟 정보로
		// 그외는 서버에서 내려준 스킬 사용 대상정보
		if (_owner->IsObjectType(EGsGameObjectType::LocalPlayer))
		{
			FGsTargetHandlerBase* targetHandler = _owner->GetTargetHandler();
			return targetHandler->GetTarget();
		}
		else
		{
			FGsSkillHandlerBase* skillhandler = _owner->GetSkillHandler();
			const FGsSkillNetData* netSkillData = skillhandler->GetNetSkillData();
			return GSGameObject()->FindObject(EGsGameObjectType::Creature, netSkillData->_targetGameId);
		}
	}
	else
	{
		EGsGameObjectType findType = EGsGameObjectType::Base;
		switch (_beamParticleEffect->targetCreatureType)
		{
		case BP_EGsGameObjectType::Projectile:
			findType = EGsGameObjectType::Projectile;
			break;
		case BP_EGsGameObjectType::NonPlayer:
			findType = EGsGameObjectType::NonPlayerBase;
			break;
		case BP_EGsGameObjectType::Player:
			findType = EGsGameObjectType::RemotePlayer;
			break;
		case BP_EGsGameObjectType::LocalPlayer:
			findType = EGsGameObjectType::LocalPlayer;
			break;
		}
		
		TArray<UGsGameObjectBase*> findObjects = GSGameObject()->FindObjectArrayByTid(findType, 
			_beamParticleEffect->targetCreatureId);

		if (findObjects.Num() > 0)
		{
			UGsGameObjectBase* caster = _owner;
			// 하나 이상일 경우에 대한 처리 정의가 없으므로 일단 거리 기반 정렬
			findObjects.Sort([caster](const UGsGameObjectBase& el1, const UGsGameObjectBase& el2) {
				return caster->GetTargetDistSquared(&el1) < caster->GetTargetDistSquared(&el2);
				});

			return findObjects[0];
		}
	}
	return nullptr;
}

void FGsSkillNotifyNodeBeamParticle::RemoveTargetBeamParticle()
{
	if (_owner)
	{
		if (AGsCharacterBase* actor = _owner->GetCharacter())
		{
			if (UGsEffectComponent* effectComponent = actor->GetEffectComponent())
			{
				effectComponent->OnBeamParticleEffectDeactive();
			}
		}
	}
}