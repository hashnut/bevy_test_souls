// Fill out your copyright notice in the Description page of Project Settings.

#include "GsSkillCheckSame.h"
#include "Util/GsTableUtil.h"
#include "../Runner/GsSkillRunnerBase.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "Skill/GsSkill.h"

EGsSkillCheckResultType FGsSkillCheckSame::GetErrType() const
{
	return EGsSkillCheckResultType::Same;
}

bool FGsSkillCheckSame::ProcessCheck(const FGsSkill* NextSkill)
{
	check(NextSkill->_tableData);
	const FGsSchemaSkillSet* data = NextSkill->_tableData;
	// 평타(콤보) 버튼 중복 예약 입력 확인
	if (data->isNormal && data->sequenceInfo.comboInfo.nextComboSkillId != 0)
	{
		// 자동 이동 중인가 최종 판별
// 		if (_fsm->IsState(EGsStateBase::AutoMove))
// 		{
// 			return false;
// 		}
		// 단일 콤보 설정 확인
		if (data->sequenceInfo.maxStage == 1)
		{
			return true;
		}
	}

	// 여기서 예약중 스킬과 비교하면 MoveToSkill 도착시에도 에러 상황이 됨.
	// 같은 실수를 반복하지않기 위해 히스토리 남김....
	// 예약 중인 스킬 확인
	//const FGsUseSkillRequest& reserveSkillInfo = _skillHandlerLocal->GetReserveSkill();
	//if (reserveSkillInfo.IsVaild() && reserveSkillInfo._skillData->id == data->id)
	//{
	//	return false;
	//}

	// 실행중인 스킬 확인
	FGsSkillRunnerBase* skillRunner = _skillHandlerLocal->GetSkillRunner();
	const FGsSchemaSkillSet* currSkillData = skillRunner->GetSkillData();
	if (currSkillData && data->id == currSkillData->id)
	{
		// CutRemain타임 확인
		if (skillRunner->CanCancel())
		{
			return true;
		}

		return false;
	}

	return true;
}

FText FGsSkillCheckSame::GetErrMessage() const
{
	FText findText;
	// FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_Same"), findText);
	return findText;
}