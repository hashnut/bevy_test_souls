// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillNotifyNodePreviewBeamParticle.h"
#include "Management/ScopeGame/GsSkillPreviewManager.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyBeamParticle.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"

FGsSkillNotifyNodePreviewBeamParticle::FGsSkillNotifyNodePreviewBeamParticle(const FGsSchemaSkillNotifyBeamParticle* BeamParticleTable)
{
	if (GSSkillPreview())
	{
		_beamParticleEffect = BeamParticleTable;

		TArray<class UGsGameObjectBase*> targetList;
		GSSkillPreview()->GetTargetList(targetList, 0, IffApplyType::SELF);
		if (targetList.Num() > 0)
		{
			_owner = targetList[0];
		}
	}
}

UGsGameObjectBase* FGsSkillNotifyNodePreviewBeamParticle::FindBeamParticleTarget()
{
	if (nullptr == _beamParticleEffect)
	{
		return nullptr;
	}

	if (GSSkillPreview())
	{
		TArray<UGsGameObjectBase*> targetList;
		GSSkillPreview()->GetTargetList(targetList, _owner->GetGameId(), IffApplyType::ENEMY);
		if (targetList.Num() > 0)
		{
			return targetList[0];
		}
	}

	return nullptr;
}