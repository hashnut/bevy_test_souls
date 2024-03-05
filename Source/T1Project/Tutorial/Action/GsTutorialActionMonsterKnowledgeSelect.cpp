// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionMonsterKnowledgeSelect.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Window/GsUIWindowMonsterCollection.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"

bool FGsTutorialActionMonsterKnowledgeSelect::OnStart()
{
	if (false == Super::OnStart())
	{
		return false;
	}

	const FGsSchemaTutorialActionUIInteraction* actionUITable = _table->actionUIInteractionRow.GetRow();
	if (nullptr == actionUITable)
	{
		return false;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(actionUITable->wbpKey);
	if (false == widget.IsValid())
	{
		return false;
	}

	_window = Cast<UGsUIWindowMonsterCollection>(widget.Get());
	if (false == _window.IsValid())
	{
		return false;
	}

	UWidget* interactionTarget = _window->GetCodexPage();
	UWidget* firstEntry = _window->SetTutorial(true);
	if (nullptr == interactionTarget ||
		nullptr == firstEntry)
	{
		return false;
	}

	UWidget* visualTarget = UGsUITrayTutorial::RecursiveFindWidget(Cast<UUserWidget>(firstEntry),
		FName(actionUITable->visualRootWidget), FName(actionUITable->visualTargetWidget));
	if (nullptr == visualTarget)
	{
		return false;
	}

	if (false == _tutorialUI->SetTargetData(_table->id, actionUITable, interactionTarget, visualTarget))
	{
		return false;
	}
	
	return true;
}

void FGsTutorialActionMonsterKnowledgeSelect::OnEnd(bool bInSendMessage)
{
	if (_window.IsValid())
	{
		_window->SetTutorial(false);
	}
	_window = nullptr;
	
	Super::OnEnd(bInSendMessage);
}
