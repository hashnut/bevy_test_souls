// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillCheckStageMode.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/Stage/GsStageMode.h"
#include "Skill/GsSkill.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

EGsSkillCheckResultType FGsSkillCheckStageMode::GetErrType() const
{
	return EGsSkillCheckResultType::NotStageMode;
}

bool FGsSkillCheckStageMode::ProcessCheck(const FGsSkill* NextSkill)
{
	check(NextSkill->_tableData);

	// 강제로 막았으 경우 
	if (GSkill()->IsForcedDisableSkill())
	{
		return false;
	}	

	if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
	{
		if (FGsStageManagerGame* stage = gameFlow->GetStageManagerGame())
		{
			//침공전 상태에서는 아레나 준비 공간에서도 전투 가능하다
			if (GGameData()->IsInvadeWorld() 
				|| GGameData()->GetInvasionDefenseState())
			{			
				//침공전시에는 안전지대는 전투가 되지 말아야한다. 
				return stage->GetCurrentStageMode() != FGsStageMode::SAFETY_ZONE;
			}
			
			// 사용 불가 스테이지 모드를 찾아서 추가
			return stage->GetCurrentStageMode() != FGsStageMode::SAFETY_ZONE &&
				stage->GetCurrentStageMode() != FGsStageMode::TOWN &&
				stage->GetCurrentStageMode() != FGsStageMode::ARENA_READY;
		}
	}
	
	return false;
}

FText FGsSkillCheckStageMode::GetErrMessage() const
{
	FText findText;
	FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_NotStageMode"), findText);
	return findText;
}

