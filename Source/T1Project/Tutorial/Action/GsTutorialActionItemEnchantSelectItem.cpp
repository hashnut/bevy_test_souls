// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionItemEnchantSelectItem.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Item/GsItemManager.h"
#include "Item/GsItem.h"
#include "UI/UIContent/Popup/ItemEnchant/GsUIPopupItemEnchantMain.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"


bool FGsTutorialActionItemEnchantSelectItem::OnStart()
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

	_popup = Cast<UGsUIPopupItemEnchantMain>(widget.Get());
	if (false == _popup.IsValid())
	{
		return false;
	}

	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
	{
		return false;
	}

	// 장착중인 무기를 얻어온다
	TWeakPtr<FGsItem> itemPtr = itemMgr->GetPlayerEquipedItemBySlotType(EGsEquipSlotType::WEAPON_TYPE);
	if (false == itemPtr.IsValid())
	{
		// 없으면 바로 종료
		return false;
	}

	if (itemPtr.Pin()->GetLevel() == itemPtr.Pin()->GetMaxLevel())
	{
		// 최대 레벨이면 바로 종료
		return false;
	}

	_popup->OnTutorialListEvent.BindRaw(this, &FGsTutorialActionItemEnchantSelectItem::OnTutorialListEvent);
	_popup->SetTutorial(itemPtr.Pin()->GetDBID());

	return true;
}

void FGsTutorialActionItemEnchantSelectItem::OnEnd(bool bInSendMessage)
{
	if (_popup.IsValid())
	{
		_popup->SetTutorial(INVALID_ITEM_DB_ID);
	}

	_actionUITable = nullptr;
	_popup = nullptr;

	Super::OnEnd(bInSendMessage);
}

void FGsTutorialActionItemEnchantSelectItem::OnTutorialListEvent(UWidget* InTargetWidget)
{
	if (false == _popup.IsValid())
	{
		return;
	}

	_popup->OnTutorialListEvent.Unbind();

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
