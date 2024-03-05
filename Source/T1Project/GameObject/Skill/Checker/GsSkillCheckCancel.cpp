// Fill out your copyright notice in the Description page of Project Settings.

#include "GsSkillCheckCancel.h"
#include "../Skill/GsSkill.h"
#include "../GsSkillHandlerLocalPlayer.h"
#include "../Runner/GsSkillRunnerBase.h"

EGsSkillCheckResultType FGsSkillCheckCancel::GetErrType() const
{
	return EGsSkillCheckResultType::NotCancel;
}

bool FGsSkillCheckCancel::ProcessCheck(const FGsSkill* NextSkill)
{
	if (NextSkill->IsBuffSkill())
	{
		return true;
	}

	if (_skillHandlerLocal->IsCasting())
	{
		return false;
	}

	FGsSkillRunnerBase* skillRunner = _skillHandlerLocal->GetSkillRunner();
	if (skillRunner->IsValid())
	{
		return skillRunner->CanCancel();
	}
	return true;
}

FText FGsSkillCheckCancel::GetErrMessage() const
{
	FText findText;
// #if WITH_EDITOR
// 	FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_NotCancel"), findText);
// #endif
	return findText;
}
