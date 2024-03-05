// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillCheckCoolTime.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Util/GsTableUtil.h"
#include "Skill/GsSkill.h"
#include "Skill/GsSchemaSkillSet.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "../GsSkillDataBuilder.h"
#include "../../ObjectClass/GsGameObjectLocalPlayer.h"


EGsSkillCheckResultType FGsSkillCheckCoolTime::GetErrType() const
{
	return EGsSkillCheckResultType::CoolTime;
}

bool FGsSkillCheckCoolTime::ProcessCheck(const FGsSkill* NextSkill)
{
	check(NextSkill->_tableData);

	// 캐스팅 스킬의 경우 캐스팅 완료 상태를 확인
	if (NextSkill->_tableData->category == SkillCategory::CAST)
	{
		UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
		if (nullptr == local)
		{
			return false;
		}

		UGsGameObjectLocalPlayer* castLocal = local->CastGameObject<UGsGameObjectLocalPlayer>();
		FGsSkillHandlerLocalPlayer* skillHandlerLocal = castLocal->GetCastSkill<FGsSkillHandlerLocalPlayer>();
		if (nullptr == skillHandlerLocal)
		{
			return false;
		}

		// 캐스팅 완료 시점만 허용
		if (skillHandlerLocal->IsCastingComplete())
		{
			return true;
		}
	}
	return !GCoolTime()->IsCoolTime(EGsCoolTime::Skill, NextSkill->_tableData->id);
}

FText FGsSkillCheckCoolTime::GetErrMessage() const
{
	FText findText;
	FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_CoolTime"), findText);

	return findText;
}

