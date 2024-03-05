// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillCheckSlotEnable.h"
#include "../Skill/GsSkill.h"

EGsSkillCheckResultType FGsSkillCheckSlotEnable::GetErrType() const
{
	return EGsSkillCheckResultType::SkillSlotDisable;
}

bool FGsSkillCheckSlotEnable::ProcessCheck(const FGsSkill* NextSkill)
{
	return NextSkill->IsEnable();
}


FText FGsSkillCheckSlotEnable::GetErrMessage() const
{
	FText findText;
	// FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_SkillSlotDisable"), findText);
	return findText;
}
