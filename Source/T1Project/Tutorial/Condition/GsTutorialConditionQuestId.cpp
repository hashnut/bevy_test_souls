// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialConditionQuestId.h"
#include "DataSchema/Tutorial/GsSchemaTutorial.h"
#include "DataSchema/Tutorial/GsSchemaTutorialTargetId.h"
#include "Message/MessageParam/GsQuestMessageParam.h"


void FGsTutorialConditionQuestId::Initialize(const FGsSchemaTutorial* InTable)
{
	if (const FGsSchemaTutorialTargetId* conditionTable = InTable->conditionRow.GetRowByType<FGsSchemaTutorialTargetId>())
	{
		_questId = conditionTable->targetId;
	}
	else
	{
		_questId = 0;
	}
}

bool FGsTutorialConditionQuestId::CheckCondition(const IGsMessageParam* InParam)
{
	if (const FGsQuestParam* param = InParam->Cast<const FGsQuestParam>())
	{
		if (param->_questId == _questId)
		{
			return true;
		}
	}

	return false;
}

bool FGsTutorialConditionQuestId::IsValidToStart()
{
	return (0 < _questId) ? true : false;
}
