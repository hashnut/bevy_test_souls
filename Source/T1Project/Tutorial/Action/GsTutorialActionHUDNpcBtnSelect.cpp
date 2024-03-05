// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionHUDNpcBtnSelect.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "DataSchema/Tutorial/GsSchemaTutorialTargetId.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDSkill.h"
#include "UI/UIContent/HUD/NPCFunction/GsUINPCFunctionSlotSetHUD.h"
#include "UI/UIContent/HUD/NPCFunction/GsUINPCFunctionButton.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Skill/GsSkillUIHandler.h"



bool FGsTutorialActionHUDNpcBtnSelect::OnStart()
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

	_npcSlotSetUI = nullptr;

	// 안전지대(마을)가 아닐 경우, NPC슬롯이 스킬슬롯으로 바뀌므로 false 처리
	if (false == FGsSkillUIHandler::IsSaftyZone())
	{
		return false;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(actionUITable->wbpKey);
	if (widget.IsValid())
	{
		if (UGsUIHUDFrameMain* hudMain = Cast<UGsUIHUDFrameMain>(widget.Get()))
		{
			if (UGsUIHUDSkill* hudSkill = hudMain->GetSkill())
			{
				_npcSlotSetUI = hudSkill->GetNpcSlotSet();
			}
		}
	}

	if (false == _npcSlotSetUI.IsValid())
	{
		return false;
	}

	UGsUINPCFunctionButton* npcBtn = _npcSlotSetUI->SetTutorialTarget(idTable->targetId);
	if (nullptr == npcBtn)
	{
		return false;
	}

	if (EGsTutorialUIInteractionType::INTERFACE_EVENT == actionUITable->targetInteractionType)
	{
		UWidget* visualWidget = UGsUITrayTutorial::RecursiveFindWidget(npcBtn,
			FName(actionUITable->visualRootWidget), FName(actionUITable->visualTargetWidget));

		if (false == _tutorialUI->SetTargetData(_table->id, actionUITable, npcBtn, visualWidget))
		{
			return false;
		}
	}
	else
	{
		UWidget* targetWidget = UGsUITrayTutorial::RecursiveFindWidget(npcBtn,
			FName(actionUITable->targetRootWidget), FName(actionUITable->targetWidget));

		if (false == _tutorialUI->SetTargetData(_table->id, actionUITable, targetWidget))
		{
			return false;
		}
	}
	
	return true;
}

void FGsTutorialActionHUDNpcBtnSelect::OnEnd(bool bInSendMessage)
{
	if (_npcSlotSetUI.IsValid())
	{
		_npcSlotSetUI->SetTutorialTarget(-1);
	}

	_npcSlotSetUI = nullptr;
	
	Super::OnEnd(bInSendMessage);
}
