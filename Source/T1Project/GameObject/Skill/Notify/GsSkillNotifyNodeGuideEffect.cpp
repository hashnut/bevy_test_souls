// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillNotifyNodeGuideEffect.h"
#include "GameFramework/Actor.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifySkillGuide.h"
#include "GameObject/Skill/Data/GsSkillDataBase.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "ActorEx/GsCharacterBase.h"


void FGsSkillNotifyNodeGuideEffect::Initialize()
{
	Super::Initialize();
}

void FGsSkillNotifyNodeGuideEffect::Initialize(UGsGameObjectBase* Owner, const IGsSkillRunnerData* SkillRunnerData,
	const FGsSchemaSkillNotify* NotifyInfo, const FGsSkillNetData* NetData, float OffsetRate)
{
	Super::Initialize(Owner, SkillRunnerData, NotifyInfo, NetData, OffsetRate);

	if (SkillRunnerData->GetType() == ActionCategory::SKILL && _currNotifyData)
	{
		const FGsSkillRunnerSKillData* castData = static_cast<const FGsSkillRunnerSKillData*>(SkillRunnerData);
		const FGsSchemaSkillSet* skillSetTable = castData->GetTableData();
		const FGsSchemaSkillNotifyElement* element = _currNotifyData->notifyElementList[0].GetRow();
		const FGsSchemaSkillNotifySkillGuide* skillGuideTable = element->typeInfo.GetRowByType<FGsSchemaSkillNotifySkillGuide>();
		SetData(skillGuideTable, &skillSetTable->collisionInfoList.Last());
	}
}

void FGsSkillNotifyNodeGuideEffect::Finalize()
{
	Super::Finalize();
}

void FGsSkillNotifyNodeGuideEffect::SetData(const FGsSchemaSkillNotifySkillGuide* Data,
	const FGsSchemaSkillCollision* CollisionInfo)
{
	_guideEffectData = Data;
	_collisionData = CollisionInfo;
}

void FGsSkillNotifyNodeGuideEffect::Action(UGsGameObjectBase* Owner)
{
	Super::Action(Owner);

	if (AGsCharacterBase* actor = Owner->GetCharacter())
	{
		if (UGsSkillGuideEffectComponent* guideEffect = actor->GetGuideEffect())
		{
			// 오버라이드 확인
			if (_guideEffectData->isOverride)
			{
				if (const FGsSchemaSkillCollision* collistionSet = _guideEffectData->overrideCollisionInfo.GetRow())
				{
					guideEffect->Set(*collistionSet, _guideEffectData->effectPath,
						_guideEffectData->duration, _guideEffectData->isAttach);
				}
			}
			else
			{
				guideEffect->Set(*_collisionData, _guideEffectData->effectPath,
					_guideEffectData->duration, _guideEffectData->isAttach);
			}

			//UGsSkillNotifyDataSkillGuide* castNotifyData = Cast<UGsSkillNotifyDataSkillGuide>(_currNotifyData);
			//check(castNotifyData);

			//if (target)
			//{
			//	guideEffect->Set(target->GetLocation(), castNotifyData);
			//}
			//else
			//{
			//	guideEffect->Set(castNotifyData, _isMoveble);
			//}
		}
	}
}