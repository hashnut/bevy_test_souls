// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionHUDPotionIconSelect.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDConsumable.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "T1Project.h"


bool FGsTutorialActionHUDPotionIconSelect::OnStart()
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

	TWeakObjectPtr<UGsUIHUDConsumable> potionUI;
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(actionUITable->wbpKey);
	if (widget.IsValid())
	{
		if (UGsUIHUDFrameMain* hudMain = Cast<UGsUIHUDFrameMain>(widget.Get()))
		{
			potionUI = hudMain->GetConsumableMenu();
		}
	}

	if (false == potionUI.IsValid())
	{
		return false;
	}

	UWidget* itemIconWidget = nullptr;
	if (UGsHorizontalBoxIconSelector* iconSelector = potionUI->GetIconSelector())
	{
		itemIconWidget = iconSelector->GetChildAt(0);
	}

	if (nullptr == itemIconWidget)
	{
		return false;
	}

	if (false == _tutorialUI->SetTargetData(_table->id, actionUITable, itemIconWidget))
	{
		return false;
	}

	return true;
}
