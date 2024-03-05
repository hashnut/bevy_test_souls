// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionFairySelect.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "DataSchema/Tutorial/GsSchemaTutorialTargetId.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Window/GsUIWindowFairy.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"


bool FGsTutorialActionFairySelect::OnStart()
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

	_window = Cast<UGsUIWindowFairy>(widget.Get());
	if (false == _window.IsValid())
	{
		return false;
	}

	_window->OnTutorialListEvent.BindRaw(this, &FGsTutorialActionFairySelect::OnListEvent);
	_window->SetTutorial(idTable->targetId);

	return true;
}

void FGsTutorialActionFairySelect::OnEnd(bool bInSendMessage)
{
	if (_window.IsValid())
	{
		_window->SetTutorial(-1);
	}	
	_window = nullptr;
	_actionUITable = nullptr;
	
	Super::OnEnd(bInSendMessage);
}

void FGsTutorialActionFairySelect::OnListEvent(UWidget* InTargetWidget)
{
	if (false == _window.IsValid())
	{
		OnEnd();
		return;
	}
	
	_window->OnTutorialListEvent.Unbind();

	if (InTargetWidget->IsA<UUserWidget>())
	{
		UWidget* targetWidget = UGsUITrayTutorial::RecursiveFindWidget(Cast<UUserWidget>(InTargetWidget),
			FName(_actionUITable->targetRootWidget), FName(_actionUITable->targetWidget));

		if (false == _tutorialUI->SetTargetData(_table->id, _actionUITable, targetWidget))
		{
			OnEnd();
		}
	}
}
