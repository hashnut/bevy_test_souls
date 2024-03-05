// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialConditionQuest.h"
#include "DataSchema/Tutorial/GsSchemaTutorial.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"


void FGsTutorialConditionQuest::Initialize(const FGsSchemaTutorial* InTable)
{
	_tutorialId = InTable->id;
}

bool FGsTutorialConditionQuest::CheckCondition(const IGsMessageParam* InParam)
{
	if (const FGsTutorialMsgParamTutorialId* param = InParam->Cast<const FGsTutorialMsgParamTutorialId>())
	{
		if (param->_data == _tutorialId)
		{
			return true;
		}
	}

	return false;
}
