// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionHUDScanSlotSelect.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "DataSchema/Tutorial/GsSchemaTutorialTargetId.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDScan.h"
#include "UI/UIContent/HUD/Scan/GsUIScanMemberItem.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Scan/GsScanMemberInfo.h"


bool FGsTutorialActionHUDScanSlotSelect::OnStart()
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
	
	_scanUI = nullptr;

	int32 slotIndex = idTable->targetId;

	// 스캔정보가 슬롯 인덱스보다 적어서 슬롯이 빌 경우 false
	TMap<int64, FGsScanMemberInfo*> mapInfo = GSGameObject()->GetScanInfo();
	if (slotIndex > mapInfo.Num())
	{
		return false;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(actionUITable->wbpKey);
	if (widget.IsValid())
	{
		if (UGsUIHUDFrameMain* hudMain = Cast<UGsUIHUDFrameMain>(widget.Get()))
		{
			_scanUI = hudMain->GetScan();
		}
	}

	if (false == _scanUI.IsValid())
	{
		return false;
	}

	UGsUIScanMemberItem* scanSlot = _scanUI->SetTutorialTarget(slotIndex);
	if (nullptr == scanSlot)
	{
		return false;
	}

	if (EGsTutorialUIInteractionType::INTERFACE_EVENT == actionUITable->targetInteractionType)
	{
		UWidget* visualWidget = UGsUITrayTutorial::RecursiveFindWidget(scanSlot,
			FName(actionUITable->visualRootWidget), FName(actionUITable->visualTargetWidget));

		if (false == _tutorialUI->SetTargetData(_table->id, actionUITable, scanSlot, visualWidget))
		{
			return false;
		}
	}
	else
	{
		UWidget* targetWidget = UGsUITrayTutorial::RecursiveFindWidget(scanSlot,
			FName(actionUITable->targetRootWidget), FName(actionUITable->targetWidget));

		if (false == _tutorialUI->SetTargetData(_table->id, actionUITable, targetWidget))
		{
			return false;
		}
	}
	
	return true;
}

void FGsTutorialActionHUDScanSlotSelect::OnEnd(bool bInSendMessage)
{
	if (_scanUI.IsValid())
	{
		_scanUI->SetTutorialTarget(-1);
	}

	_scanUI = nullptr;

	Super::OnEnd(bInSendMessage);
}
