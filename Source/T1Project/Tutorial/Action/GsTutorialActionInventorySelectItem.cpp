// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionInventorySelectItem.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "DataSchema/Tutorial/GsSchemaTutorialTargetId.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Item/GsItemManager.h"
#include "Item/GsItem.h"
#include "UI/UIContent/Popup/GsUIPopupInventory.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"


void FGsTutorialActionInventorySelectItem::OnPrevStart(const IGsMessageParam* InParam)
{
	if (const FGsTutorialMsgParamUInt32* param = InParam->Cast<const FGsTutorialMsgParamUInt32>())
	{
		_itemId = static_cast<ItemId>(param->_data);
	}
}

bool FGsTutorialActionInventorySelectItem::OnStart()
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

	_popup = Cast<UGsUIPopupInventory>(widget.Get());
	if (false == _popup.IsValid())
	{
		return false;
	}

	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
	{
		return false;
	}

	if (INVALID_ITEM_ID == _itemId)
	{
		const FGsSchemaTutorialTargetId* idTable = _table->actionRow.GetRowByType<FGsSchemaTutorialTargetId>();
		if (nullptr == idTable)
		{
			return false;
		}

		_itemId = static_cast<ItemId>(idTable->targetId);

		if (INVALID_ITEM_ID == _itemId)
		{
			return false;
		}
	}

	TArray<TWeakPtr<FGsItem>> itemList = itemMgr->FindByTID(_itemId);
	if (0 == itemList.Num())
	{
		return false;
	}

	_popup->OnTutorialInventoryListEvent.BindRaw(this, &FGsTutorialActionInventorySelectItem::OnTutorialInventorytListEvent);
	_popup->SetTutorialItemDBId(itemList[0].Pin()->GetDBID());

	return true;
}

void FGsTutorialActionInventorySelectItem::OnEnd(bool bInSendMessage)
{
	if (_popup.IsValid())
	{
		_popup->SetTutorialItemDBId(0);
	}

	_actionUITable = nullptr;
	_popup = nullptr;

	Super::OnEnd(bInSendMessage);
}

void FGsTutorialActionInventorySelectItem::OnTutorialInventorytListEvent(UWidget* InTargetWidget)
{
	if (false == _popup.IsValid())
	{
		return;
	}

	_popup->OnTutorialInventoryListEvent.Unbind();

	if (InTargetWidget->IsA<UUserWidget>())
	{
		if (EGsTutorialUIInteractionType::INTERFACE_EVENT == _actionUITable->targetInteractionType)
		{
			UWidget* visualTargetWidget = UGsUITrayTutorial::RecursiveFindWidget(Cast<UUserWidget>(InTargetWidget),
				FName(_actionUITable->visualRootWidget), FName(_actionUITable->visualTargetWidget));

			if (false == _tutorialUI->SetTargetData(_table->id, _actionUITable, _popup.Get(), visualTargetWidget))
			{
				OnEnd();
			}
		}
		else
		{
			UWidget* targetWidget = UGsUITrayTutorial::RecursiveFindWidget(Cast<UUserWidget>(InTargetWidget),
				FName(_actionUITable->targetRootWidget), FName(_actionUITable->targetWidget));

			if (false == _tutorialUI->SetTargetData(_table->id, _actionUITable, targetWidget))
			{
				OnEnd();
			}
		}
	}
}
