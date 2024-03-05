// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionHUDQuickSlotSelect.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "DataSchema/Tutorial/GsSchemaTutorialTargetId.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDQuickSlot.h"
#include "UI/UIContent/Common/ReuseIconSlot/GsUIReuseScrollHUD.h"
#include "UI/UIContent/Common/ReuseIconSlot/GsUIReuseItemIconSlotHud.h"


bool FGsTutorialActionHUDQuickSlotSelect::OnStart()
{
	if (false == Super::OnStart())
	{
		return false;
	}

	const FGsSchemaTutorialTargetId* idTable = _table->actionRow.GetRowByType<FGsSchemaTutorialTargetId>();
	if (nullptr == idTable)
	{
		return false;
	}

	const FGsSchemaTutorialActionUIInteraction* actionUITable = _table->actionUIInteractionRow.GetRow();
	if (nullptr == actionUITable)
	{
		return false;
	}

	_quickSlotSetUI = nullptr;

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(actionUITable->wbpKey);
	if (widget.IsValid())
	{
		if (UGsUIHUDFrameMain* hudMain = Cast<UGsUIHUDFrameMain>(widget.Get()))
		{
			if (UGsUIHUDQuickSlot* hudQuickSlot = hudMain->GetQuickSlot())
			{
				_quickSlotSetUI = hudQuickSlot->GetQuickSlotSet();
			}
		}
	}

	UGsUIReuseItemIconSlotHud* itemSlot = _quickSlotSetUI->SetTutorialTarget(idTable->targetId);
	if (nullptr == itemSlot)
	{
		return false;
	}

	if (EGsTutorialUIInteractionType::INTERFACE_EVENT == actionUITable->targetInteractionType)
	{
		UWidget* visualWidget = UGsUITrayTutorial::RecursiveFindWidget(itemSlot,
			FName(actionUITable->visualRootWidget), FName(actionUITable->visualTargetWidget));

		if (false == _tutorialUI->SetTargetData(_table->id, actionUITable, itemSlot, visualWidget))
		{
			return false;
		}
	}
	else
	{
		UWidget* targetWidget = UGsUITrayTutorial::RecursiveFindWidget(itemSlot,
			FName(actionUITable->targetRootWidget), FName(actionUITable->targetWidget));

		if (false == _tutorialUI->SetTargetData(_table->id, actionUITable, targetWidget))
		{
			return false;
		}
	}

	return true;
}

void FGsTutorialActionHUDQuickSlotSelect::OnEnd(bool bInSendMessage)
{
	if (_quickSlotSetUI.IsValid())
	{
		_quickSlotSetUI->SetTutorialTarget(-1);
	}

	_quickSlotSetUI = nullptr;
	
	Super::OnEnd(bInSendMessage);
}
