// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/Skill/Runner/GsSkillRunnerSkillPreviewPlayer.h"
#include "Shared/Client/SharedEnums/SharedSkillNotifyEnum.h"

#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyProjectile.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyBeamParticle.h"

#include "../Notify/SkillPreview/GsSkillNotifyNodePreviewProjectile.h"
#include "../Notify/SkillPreview/GsSkillNotifyNodePreviewEndSection.h"
#include "../Notify/SkillPreview/GsSkillNotifyNodePreviewMoveLocation.h"
#include "../Notify/SkillPreview/GsSkillNotifyNodePreviewDamage.h"
#include "../Notify/SkillPreview/GsSkillNotifyNodePreviewBeamParticle.h"
#include "../Notify/GsSkillNotifyNodeBase.h"

FGsSkillRunnerSkillPreviewPlayer::FGsSkillRunnerSkillPreviewPlayer(TFunction<void(int, bool)> EndRun) : Super(EndRun)
{

}

void FGsSkillRunnerSkillPreviewPlayer::StartRunner(UGsGameObjectBase* Owner)
{
	Super::StartRunner(Owner);

	// 이제 인게임에서 클라이언트에서 Notify 처리를 하지 않으므로 따로 설정해준다.
	if (_skillData.IsValid())
	{
		if (const TArray<FGsSchemaSkillNotify>* notifyList = _skillData.GetNotifyList())
		{
			SetNotifyNode(*notifyList);
		}
	}
}

TSharedPtr<FGsSkillNotifyNodeBase> FGsSkillRunnerSkillPreviewPlayer::CreateSkillNode(const FGsSchemaSkillNotifySet* Data)
{
	// 상위 호출을 사용하지 않는다.
	if (Data->type == SkillNotifyType::DAMAGE || Data->type == SkillNotifyType::VARY_HP_MP)
	{		
		return MakeShareable(new FGsSkillNotifyPreviewDamage());
	}
	else if (Data->type == SkillNotifyType::FIRE_PROJECTILE)
	{
		if (const FGsSchemaSkillSet* skillData = GetSkillData())
		{
			return MakeShareable(new FGsSkillNotifyNodePreviewProjectile(skillData->targetFilterInfo.iffType,
				skillData->id));
		}
	}
	else if (Data->type == SkillNotifyType::END_SECTION)
	{
		if (const FGsSchemaSkillSet* skillData = GetSkillData())
		{
			return MakeShareable(new FGsSkillNotifyNodePreviewEndSection(skillData->targetFilterInfo.iffType));
		}
	}
	else if (Data->type == SkillNotifyType::MOVE_LOCATION)
	{
		if (const FGsSchemaSkillSet* skillData = GetSkillData())
		{
			return MakeShareable(new FGsSkillNotifyNodePreviewMoveLocation(skillData->targetFilterInfo.iffType));
		}
	}
	else if (Data->type == SkillNotifyType::BEAM_PARTICLE_EFFECT)
	{
		if (const FGsSchemaSkillSet* skillData = GetSkillData())
		{
			const FGsSchemaSkillNotifyElement* element = Data->notifyElementList[0].GetRow();
			if (nullptr == element)
			{
				return nullptr;
			}

			const FGsSchemaSkillNotifyBeamParticle* beamParticleEffect = element->typeInfo.GetRowByType<FGsSchemaSkillNotifyBeamParticle>();
			return MakeShareable(new FGsSkillNotifyNodePreviewBeamParticle(beamParticleEffect));
		}
	}

	return nullptr;
}