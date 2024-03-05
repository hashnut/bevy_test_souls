// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionHUDSkillBtnSelect.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "DataSchema/Tutorial/GsSchemaTutorialTargetId.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDSkill.h"
#include "UI/UIContent/HUD/Skill/GsUISkillSlotSetHUD.h"
#include "UI/UIContent/HUD/Skill/GsUISkillButtonActive.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Skill/GsSkillUIHandler.h"


bool FGsTutorialActionHUDSkillBtnSelect::OnStart()
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

	_skillSlotSetUI = nullptr;

	// 안전지대(마을)일 경우, 스킬 슬롯이 NPC슬롯으로 바뀌므로 false 처리
	if (FGsSkillUIHandler::IsSaftyZone())
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
				_skillSlotSetUI = hudSkill->GetSkillSlotSet();
			}
		}
	}

	if (false == _skillSlotSetUI.IsValid())
	{
		return false;
	}

	UGsUISkillButtonBase* skillBtnBase = _skillSlotSetUI->SetTutorialTarget(idTable->targetId);
	if (nullptr == skillBtnBase)
	{
		return false;
	}
	
	UGsUISkillButtonActive* skillBtn = Cast<UGsUISkillButtonActive>(skillBtnBase);
	if (nullptr == skillBtn)
	{
		return false;
	}

	if (EGsTutorialUIInteractionType::INTERFACE_EVENT == actionUITable->targetInteractionType)
	{
		UWidget* visualWidget = UGsUITrayTutorial::RecursiveFindWidget(skillBtn,
			FName(actionUITable->visualRootWidget), FName(actionUITable->visualTargetWidget));

		if (false == _tutorialUI->SetTargetData(_table->id, actionUITable, skillBtn, visualWidget))
		{
			return false;
		}
	}
	else
	{
		UWidget* targetWidget = UGsUITrayTutorial::RecursiveFindWidget(skillBtn,
			FName(actionUITable->targetRootWidget), FName(actionUITable->targetWidget));

		if (false == _tutorialUI->SetTargetData(_table->id, actionUITable, targetWidget))
		{
			return false;
		}
	}

	return true;
}

void FGsTutorialActionHUDSkillBtnSelect::OnEnd(bool bInSendMessage)
{
	if (_skillSlotSetUI.IsValid())
	{
		_skillSlotSetUI->SetTutorialTarget(-1);
	}

	_skillSlotSetUI = nullptr;
	
	Super::OnEnd(bInSendMessage);
}
