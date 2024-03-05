// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialConditionOpenUICheckParam.h"
#include "DataSchema/Tutorial/GsSchemaTutorial.h"
#include "DataSchema/Tutorial/GsSchemaTutorialConditionOpenUI.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"
#include "Management/ScopeGlobal/GsUIManager.h"


void FGsTutorialConditionOpenUICheckParam::Initialize(const FGsSchemaTutorial* InTable)
{
	const FGsSchemaTutorialConditionOpenUI* conditionTable =
		InTable->conditionRow.GetRowByType<FGsSchemaTutorialConditionOpenUI>();

	if (conditionTable)
	{
		_widgetKey = FName(conditionTable->widgetKey);
		_paramStr = conditionTable->paramString;
		_paramInt = conditionTable->paramInt;
	}
}

bool FGsTutorialConditionOpenUICheckParam::CheckCondition(const IGsMessageParam* InParam)
{
	if (const FGsTutorialMsgParamOpenUI* param = InParam->Cast<const FGsTutorialMsgParamOpenUI>())
	{
		if (param->_widgetKey == _widgetKey &&
			param->_paramStr == _paramStr &&
			param->_paramInt == _paramInt)
		{
			return true;
		}
	}

	return false;
}

bool FGsTutorialConditionOpenUICheckParam::IsValidToStart()
{
	return GUI()->IsActiveWidget(_widgetKey);
}
