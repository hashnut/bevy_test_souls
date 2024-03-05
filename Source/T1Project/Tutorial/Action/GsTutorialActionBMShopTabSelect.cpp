// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionBMShopTabSelect.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "DataSchema/Tutorial/GsSchemaTutorialTargetId.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Window/BMShop/GsUIWindowBMShop.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"


bool FGsTutorialActionBMShopTabSelect::OnStart()
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

	_window = Cast<UGsUIWindowBMShop>(widget.Get());
	if (false == _window.IsValid())
	{
		return false;
	}

	_window->OnTutorialListEvent.BindRaw(this, &FGsTutorialActionBMShopTabSelect::OnListEvent);
	_window->SetTutorialTab(idTable->targetId);

	return true;
}

void FGsTutorialActionBMShopTabSelect::OnEnd(bool bInSendMessage)
{
	if (_window.IsValid())
	{
		_window->SetTutorialTab(0);
	}

	_actionUITable = nullptr;
	_window = nullptr;
	
	Super::OnEnd(bInSendMessage);
}

void FGsTutorialActionBMShopTabSelect::OnListEvent(UWidget* InWidget)
{
	if (false == _window.IsValid())
	{
		OnEnd();
		return;
	}

	_window->OnTutorialListEvent.Unbind();

	if (InWidget->IsA<UUserWidget>())
	{
		UWidget* targetWidget = UGsUITrayTutorial::RecursiveFindWidget(Cast<UUserWidget>(InWidget),
			FName(_actionUITable->targetRootWidget), FName(_actionUITable->targetWidget));

		if (false == _tutorialUI->SetTargetData(_table->id, _actionUITable, targetWidget))
		{
			OnEnd();
		}
	}
}
