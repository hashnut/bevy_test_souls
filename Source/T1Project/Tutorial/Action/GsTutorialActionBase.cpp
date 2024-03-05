// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionBase.h"
#include "DataSchema/Tutorial/GsSchemaTutorialAction.h"
#include "DataSchema/Tutorial/GsSchemaTutorialConditionOpenUI.h"
#include "DataSchema/SubDialog/GsSchemaSubDialogCutData.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"


void FGsTutorialActionBase::Initialize(const FGsSchemaTutorialAction* InTable)
{
	_table = InTable;
}

bool FGsTutorialActionBase::OnStart()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("TrayTutorial"));
	if (false == widget.IsValid())
	{
		return false;
	}

	_tutorialUI = Cast<UGsUITrayTutorial>(widget.Get());

	const FGsSchemaSubDialogCutData* cutData = _table->subDialogCutDataRow.GetRow();
	if (cutData)
	{
		_tutorialUI->SetSubDialog(cutData, _table->subDialogDirection);
	}

	return true;
}

void FGsTutorialActionBase::OnEnd(bool bInSendMessage)
{
	if (_bIsComplete)
	{
		return;
	}

	_bIsComplete = true;

	if (bInSendMessage)
	{
		if (_table)
		{
			FGsTutorialMsgParamInt param(_table->id);
			GMessage()->GetTutorial().SendMessage(MessageContentTutorial::END_ACTION, &param);
		}
	}
}

void FGsTutorialActionBase::OnEndUIInteraction()
{
	_bIsInteractionComplete = true;

	// 종료 조건 체크
	if (EGsTutorialActionConditionType::UI_INTERACTION == _table->endCondition)
	{
		SetEndCondition(true);
	}

	if (CheckEndCondition())
	{
		OnEnd();
	}
}

void FGsTutorialActionBase::OnOpenUI(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsTutorialMsgParamName* param = InParam->Cast<const FGsTutorialMsgParamName>();
	if (nullptr == param)
	{
		return;
	}

	// 종료 조건 체크
	if (EGsTutorialActionConditionType::OPEN_UI == _table->endCondition)
	{
		if (const FGsSchemaTutorialConditionOpenUI* conditionTable = 
			_table->endConditionRow.GetRowByType<FGsSchemaTutorialConditionOpenUI>())
		{
			if (param->_data == conditionTable->widgetKey)
			{
				SetEndCondition(true);
			}
		}		
	}

	if (CheckEndCondition())
	{
		OnEnd();
	}
}

void FGsTutorialActionBase::OnSummonButtonState(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsTutorialMsgParamSummonButtonState* param = InParam->Cast<const FGsTutorialMsgParamSummonButtonState>();
	if (nullptr == param)
	{
		return;
	}

	if (EGsTutorialActionConditionType::SUMMON_STATE_END == _table->endCondition)
	{
		if (EGsSummonBtnType::QUIT == param->_data ||
			EGsSummonBtnType::RETRY_ITEM == param->_data)
		{
			SetEndCondition(true);
		}
	}

	if (CheckEndCondition())
	{
		OnEnd();
	}
}

bool FGsTutorialActionBase::CheckEndCondition()
{
	if (_bIsComplete)
	{
		return false;
	}

	if (false == _bIsInteractionComplete)
	{
		return false;
	}

	if (false == _bIsEndConditionComplete)
	{
		return false;
	}

	return true;
}

EGsTutorialActionType FGsTutorialActionBase::GetActionType() const
{
	if (_table)
	{
		return _table->actionType;
	}

	return EGsTutorialActionType::NONE;
}
