// Fill out your copyright notice in the Description page of Project Settings.

#include "GsSkillNotifyNodePreviewEndSection.h"
#include "../GsSkillNotifyNodeBase.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "../../../ObjectClass/GsGameObjectBase.h"
#include "../../GsSkillHandlerBase.h"
#include "../../../../Management/ScopeGame/GsSkillPreviewManager.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "../../../../UTIL/GsDir.h"

FGsSkillNotifyNodePreviewEndSection::FGsSkillNotifyNodePreviewEndSection(IffApplyType SkillIffType)
{
	_skillIffType = SkillIffType;
}

void FGsSkillNotifyNodePreviewEndSection::Action(UGsGameObjectBase* Owner)
{
	Super::Action(Owner);

	// 타겟 정보 얻기
	TArray<UGsGameObjectBase*> targetList;
	GSSkillPreview()->GetTargetList(targetList, Owner->GetGameId(), _skillIffType);
	 
	if (targetList.Num() > 0)
	{
		PD::SC::PKT_SC_START_SKILL_READ packet;
		packet.mCasterId = Owner->GetGameId();
		packet.mSkillId = _netData->_skillId;
		packet.mSkillLevel = _netData->_skillLevel;
		packet.mCurPos = GSSkillPreview()->GetOwnerPostion();
		packet.mCurrDir = FGsDir::dirToAngle(GSSkillPreview()->GetOwnerDirection());
		packet.mTargetId = targetList[0]->GetGameId();
		packet.mSkillStartType = SkillStartType::SKILL;
		packet.mReactionId = 0;

		GSSkillPreview()->ExcuteStartSkill(Owner, &packet);
	}
}
