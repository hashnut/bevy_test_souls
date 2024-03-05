// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialConditionFirstTutorial.h"
#include "DataSchema/Tutorial/GsSchemaTutorial.h"
#include "DataSchema/Tutorial/GsSchemaTutorialTargetId.h"
#include "Message/MessageParam/GsQuestMessageParam.h"


void FGsTutorialConditionFirstTutorial::Initialize(const FGsSchemaTutorial* InTable)
{
	if (const FGsSchemaTutorialTargetId* conditionTable = InTable->conditionRow.GetRowByType<FGsSchemaTutorialTargetId>())
	{
		_storyId = conditionTable->targetId;
	}
	else
	{
		_storyId = 0;
	}
}

bool FGsTutorialConditionFirstTutorial::CheckCondition(const IGsMessageParam* InParam)
{
	if (const FGsStoryParam* param = InParam->Cast<const FGsStoryParam>())
	{
		if (param->_storyId == _storyId)
		{
			return true;
		}
	}

	return false;
}

bool FGsTutorialConditionFirstTutorial::IsValidToStart()
{
	return (0 < _storyId) ? true : false;
}
