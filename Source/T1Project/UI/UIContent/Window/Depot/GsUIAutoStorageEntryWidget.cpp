// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIAutoStorageEntryWidget.h"
#include "Item/GsItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Management/GsMessageHolder.h"


void UGsUIAutoStorageEntryWidget::BeginDestroy()
{
	if (nullptr != _iconSlotHelper)
	{
		_iconSlotHelper = nullptr;
	}

	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}

	Super::BeginDestroy();
}

void UGsUIAutoStorageEntryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_iconSelector);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIAutoStorageEntryWidget::OnRefreshIconWrapBox);
		_iconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUIAutoStorageEntryWidget::OnCreateIconWrapBox);
	}
	
	_btnSelect->OnClicked.AddDynamic(this, &UGsUIAutoStorageEntryWidget::OnClickSlot);
}

void UGsUIAutoStorageEntryWidget::NativeDestruct()
{
	OnHide();

	Super::NativeDestruct();
}

void UGsUIAutoStorageEntryWidget::OnShow()
{
	_isSelected = false;
	_displayItemTID = 0;
	_selectedImg->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIAutoStorageEntryWidget::OnHide()
{
	_iconSlotHelper->Clear();

	if (OnSelectAutoEntrySlot.IsBound())
	{
		OnSelectAutoEntrySlot.Clear();
	}
}

void UGsUIAutoStorageEntryWidget::SetDisPlayItem(const int32 InItemTID, const bool InIsSelected)
{
	const FGsSchemaItemCommon* commonData = FGsItem::GetTableDataByID(InItemTID);
	if (nullptr == commonData)
		return;

	_displayItemTID = InItemTID;

	_isSelected = InIsSelected;
	_selectedImg->SetVisibility(_isSelected == true ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	_itemNameText->SetText(commonData->name);
	_iconSlotHelper->RefreshAll(1);
}

void UGsUIAutoStorageEntryWidget::OnClickSlot()
{
	_isSelected = !_isSelected;
	_selectedImg->SetVisibility(_isSelected == true ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (true == OnSelectAutoEntrySlot.IsBound())
	{
		OnSelectAutoEntrySlot.Broadcast(this);
	}
}

void UGsUIAutoStorageEntryWidget::OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIAutoStorageEntryWidget::OnClickIconSlot);
	}	

	itemIcon->SetNotOwnItem(_displayItemTID,1);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::OnClickType);
}

void UGsUIAutoStorageEntryWidget::OnCreateIconWrapBox(class UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemCount(0);
	itemIcon->SetCoolTimeBg(false);
}

void UGsUIAutoStorageEntryWidget::OnClickIconSlot(class UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	if (nullptr == itemIcon->GetItemIconData())
		return;

	FGsItemDetailOpenMessageParam param(itemIcon->GetItemIconData(), true, true, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
}