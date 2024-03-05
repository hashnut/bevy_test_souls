// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialConditionOpenUI.h"
#include "DataSchema/Tutorial/GsSchemaTutorial.h"
#include "DataSchema/Tutorial/GsSchemaTutorialConditionOpenUI.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"
#include "Management/ScopeGlobal/GsUIManager.h"


void FGsTutorialConditionOpenUI::Initialize(const FGsSchemaTutorial* InTable)
{
	const FGsSchemaTutorialConditionOpenUI* conditionTable =
		InTable->conditionRow.GetRowByType<FGsSchemaTutorialConditionOpenUI>();

	if (conditionTable)
	{
		_nameWidgetKey = FName(conditionTable->widgetKey);
	}
}

bool FGsTutorialConditionOpenUI::CheckCondition(const IGsMessageParam* InParam)
{
	if (const FGsTutorialMsgParamName* param = InParam->Cast<const FGsTutorialMsgParamName>())
	{
		if (param->_data == _nameWidgetKey)
		{
			return true;
		}
	}

	return false;
}

bool FGsTutorialConditionOpenUI::IsValidToStart()
{
	return GUI()->IsActiveWidget(_nameWidgetKey);
}