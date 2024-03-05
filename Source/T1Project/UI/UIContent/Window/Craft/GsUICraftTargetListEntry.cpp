#include "GsUICraftTargetListEntry.h"

#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

#include "Classes/GsMessageHandlerOneParam.h"
#include "DataSchema/Item/Craft/GsSchemaItemCraftRecipe.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Item/Craft/GsCraftTarget.h"
#include "Item/Craft/GsCraftTargetObject.h"
#include "Item/GsItemManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsCraftManager.h"
#include "Management/ScopeGame/GsTutorialManager.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsCraftMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Tray/GsUITrayTickerSection.h"
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsIconSelectorInterface.h"

void UGsUICraftTargetListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _slotHelperIcon)
	{
		_slotHelperIcon = NewObject<UGsDynamicIconSlotHelper>(this);
		_slotHelperIcon->Initialize(_iconSelector);
		_slotHelperIcon->OnRefreshIcon.AddUniqueDynamic(this, &UGsUICraftTargetListEntry::OnRefreshIconItem);
	}

	_btnLock->OnClicked.AddUniqueDynamic(this, &UGsUICraftTargetListEntry::OnClickedLock);
	_btnFavorite->OnClicked.AddUniqueDynamic(this, &UGsUICraftTargetListEntry::OnClickedFavorite);
}

void UGsUICraftTargetListEntry::NativeDestruct()
{
	_iconSelector->RemoveAllChildren();

	Super::NativeDestruct();
}

void UGsUICraftTargetListEntry::NativeOnListItemObjectSet(UObject* InListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(InListItemObject);

	Invalidate();
}

void UGsUICraftTargetListEntry::Invalidate()
{
	const UGsUICraftTargetListItem* ListItem = GetListItem<UGsUICraftTargetListItem>();
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	const FGsCraftTarget* CraftTarget = CraftManager->GetCraftTarget(ListItem->_craftTargetId);
	if (nullptr == CraftTarget)
	{
		GSLOG(Error, TEXT("nullptr == CraftTarget"));
		return;
	}

	_slotHelperIcon->RefreshAll(1);

	_txtTitle->SetText(CraftTarget->_tableData->recipeText);
	_txtTitle->SetColorAndOpacity(ListItem->_craftTargetObject.GetCommonColor());
	_btnLock->SetVisibility(CraftTarget->IsCraftUnlocked() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	_btnFavorite->SetIsSelected(CraftTarget->IsFavorite());
	_wgtDimmed->SetVisibility(CraftManager->CanCraftNow(CraftTarget->_craftTargetId, 1) ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	_btnMain->SetIsSelectedNoEvent(IsListItemSelected());
}

CraftId UGsUICraftTargetListEntry::GetCraftTargetId() const
{
	const UGsUICraftTargetListItem* ListItem = GetListItem<UGsUICraftTargetListItem>();
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem, GetName():%s"), *GetName());
		return INVALID_CRAFT_ID;
	}

	return ListItem->_craftTargetId;
}

void UGsUICraftTargetListEntry::OnRefreshIconItem(int32 InIndex, UGsUIIconBase* InIcon)
{
	const UGsUICraftTargetListItem* ListItem = GetListItem<UGsUICraftTargetListItem>();
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem, InIndex:%d, GetName():%s"), InIndex, *GetName());
		return;
	}

	UGsUIIconItemInventory* Icon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == Icon)
	{
		GSLOG(Error, TEXT("nullptr == Icon, InIndex:%d, GetName():%s"), InIndex, *GetName());
		return;
	}

	const CostType CurrentCostType = ListItem->_craftTargetObject.GetCostType();
	if (CostType::ITEM == CurrentCostType)
	{
		Icon->SetVisibility(ESlateVisibility::HitTestInvisible);
		Icon->SetNotOwnItem(ListItem->_craftTargetObject.GetItemId(), 1, ListItem->_craftTargetObject.GetItemLevel());

		if (!Icon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			Icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUICraftTargetListEntry::OnFinishedLongPressEventItemIcon);
		}
	}
	else if (CostType::CURRENCY == CurrentCostType)
	{
		Icon->SetVisibility(ESlateVisibility::HitTestInvisible);
		Icon->SetCurrency(ListItem->_craftTargetObject.GetCurrencyType(), 1);

		if (!Icon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			Icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUICraftTargetListEntry::OnFinishedLongPressEventItemIcon);
		}
	}
	else
	{
		GSLOG(Error, TEXT("Invalid CurrentCostType"));
		Icon->SetVisibility(ESlateVisibility::Hidden);
		Icon->OnFinishedLongPressEvent.RemoveAll(this);
	}

	Icon->SetItemNameVisibility(false);
}

void UGsUICraftTargetListEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	_btnMain->SetIsSelectedNoEvent(bIsSelected);
}

void UGsUICraftTargetListEntry::OnClickedLock()
{
	if (GSTutorial()->IsActiveTutorial())
	{
		return;
	}

	const UGsUICraftTargetListItem* ListItem = GetListItem<UGsUICraftTargetListItem>();
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem, GetName():%s"), *GetName());
		return;
	}

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	const FGsCraftTarget* CraftTarget = CraftManager->GetCraftTarget(ListItem->_craftTargetId);
	if (nullptr == CraftTarget)
	{
		GSLOG(Error, TEXT("nullptr == CraftTarget"));
		return;
	}

	FText LockMessage;
	if (!CraftTarget->IsCraftUnlocked(&LockMessage))
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, LockMessage);
	}
}

void UGsUICraftTargetListEntry::OnClickedFavorite()
{
	if (GSTutorial()->IsActiveTutorial())
	{
		return;
	}

	const UGsUICraftTargetListItem* ListItem = GetListItem<UGsUICraftTargetListItem>();
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem, GetName():%s"), *GetName());
		return;
	}

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	const FGsCraftTarget* CraftTarget = CraftManager->GetCraftTarget(ListItem->_craftTargetId);
	if (nullptr == CraftTarget)
	{
		GSLOG(Error, TEXT("nullptr == CraftTarget"));
		return;
	}

	FGsCraftMessageParamTryChangeFavoriteCraftTarget(ListItem->_craftTargetId, !CraftTarget->IsFavorite()).Send();
}

void UGsUICraftTargetListEntry::OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon)
{
	if (GSTutorial()->IsActiveTutorial())
	{
		return;
	}

	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}
