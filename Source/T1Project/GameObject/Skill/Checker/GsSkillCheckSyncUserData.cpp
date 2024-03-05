// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillCheckSyncUserData.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Util/GsTableUtil.h"
#include "../Skill/GsSkill.h"

EGsSkillCheckResultType FGsSkillCheckSyncUserData::GetErrType() const
{
	return EGsSkillCheckResultType::NotSyncUserData;
}

bool FGsSkillCheckSyncUserData::ProcessCheck(const FGsSkill* NextSkill)
{
	// 검증용 로그 출력
	const FGsSchemaSkillSet* data = NextSkill->_tableData;
	const FGsSkill* findSkillData = GSkill()->FindActiveSkill(data->id);
	if (nullptr == findSkillData)
	{
		// 실제 데이터가 설정 되어 있는지 서버와의 Sync가 맞지 않는지 확인
		if (const FGsSkill* realData = GSkill()->FindSkill(data->id))
		{
			GSLOG(Error, TEXT("Skill Check NetData Sync Fail, Id : %d, IsLearn : %s"),
				realData->_tableData->id, realData->_isLearn ? TEXT("true") : TEXT("false"));
		}
		ensure(0);
		return false;
	}
	return true;
}

FText FGsSkillCheckSyncUserData::GetErrMessage() const
{
	FText findText;
	// FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_NotSyncUserData"), findText);
	return findText;
}