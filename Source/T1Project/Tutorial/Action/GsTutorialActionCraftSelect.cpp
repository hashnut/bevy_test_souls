// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionCraftSelect.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Window/GsUIWindowCraft.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"


bool FGsTutorialActionCraftSelect::OnStart()
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

	_window = Cast<UGsUIWindowCraft>(widget.Get());
	if (false == _window.IsValid())
	{
		return false;
	}

	UWidget* slotWidget = _window->SetTutorial(true);
	if (nullptr == slotWidget)
	{
		return false;
	}

	UWidget* visualTarget = UGsUITrayTutorial::RecursiveFindWidget(Cast<UUserWidget>(slotWidget),
		FName(_actionUITable->visualRootWidget), FName(_actionUITable->visualTargetWidget));

	// 인터렉션 이벤트를 위해 대상 윈도우를 넘김
	if (false == _tutorialUI->SetTargetData(_table->id, _actionUITable, _window.Get(), visualTarget))
	{
		return false;
	}

	return true;
}

void FGsTutorialActionCraftSelect::OnEnd(bool bInSendMessage)
{
	if (_window.IsValid())
	{
		_window->SetTutorial(false);
	}

	_actionUITable = nullptr;
	_window = nullptr;
	
	Super::OnEnd(bInSendMessage);
}
