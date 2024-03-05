// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionSkillEquip.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Window/GsUIWindowSkill.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"


bool FGsTutorialActionSkillEquip::OnStart()
{
	if (false == Super::OnStart())
	{
		return false;
	}

	_actionUITable = _table->actionUIInteractionRow.GetRow();
	if (nullptr == _actionUITable)
	{
		return false;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(_actionUITable->wbpKey);
	if (false == widget.IsValid())
	{
		return false;
	}

	UGsUIWindowSkill* window = Cast<UGsUIWindowSkill>(widget.Get());
	if (nullptr == window)
	{
		return false;
	}

	UWidget* slotWidget = window->GetFirstSkillSlot();
	if (nullptr == slotWidget)
	{
		return false;
	}

	UWidget* targetWidget = UGsUITrayTutorial::RecursiveFindWidget(Cast<UUserWidget>(slotWidget),
		FName(_actionUITable->targetRootWidget), FName(_actionUITable->targetWidget));
	if (nullptr == targetWidget)
	{
		return false;
	}

	if (false == _tutorialUI->SetTargetData(_table->id, _actionUITable, targetWidget))
	{
		return false;
	}

	return true;
}

void FGsTutorialActionSkillEquip::OnEnd(bool bInSendMessage)
{
	_actionUITable = nullptr;

	Super::OnEnd(bInSendMessage);
}
