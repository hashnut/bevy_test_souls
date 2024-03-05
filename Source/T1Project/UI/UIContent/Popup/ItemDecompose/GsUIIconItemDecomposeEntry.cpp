// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIIconItemDecomposeEntry.h"
#include "T1Project.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemEquipment.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "Management/GsScopeHolder.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"


void UGsUIIconItemDecomposeEntry::BeginDestroy()
{
	if (nullptr != _iconHelper)
	{
		_iconHelper->Clear();
		_iconHelper = nullptr;
	}

	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}

	Super::BeginDestroy();
}

void UGsUIIconItemDecomposeEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _iconHelper)
	{
		_iconHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconHelper->Initialize(_iconSelector);
		_iconHelper->OnRefreshIcon.AddDynamic(this, &UGsUIIconItemDecomposeEntry::OnRefreshIconWrapBox);
		_iconHelper->OnCreateIcon.AddDynamic(this, &UGsUIIconItemDecomposeEntry::OnCreateIconWrapBox);
	}

	_btnSelect->OnClicked.AddDynamic(this, &UGsUIIconItemDecomposeEntry::OnClickSelect);
}

void UGsUIIconItemDecomposeEntry::NativeDestruct()
{
	OnHide();

	Super::NativeDestruct();
}

void UGsUIIconItemDecomposeEntry::OnShow()
{
	_itemGrade = ItemGrade::NONE;
	_currentInputCount = 0;
	_maxStackCount = 0;
	_itemDBID = 0;
	_widgetIndex = 0;
	_isSelected = false;
	_selectSwitcher->SetActiveWidgetIndex(!_isSelected);
}

void UGsUIIconItemDecomposeEntry::OnHide()
{
	if (true == OnChangeSelectDelegate.IsBound())
	{
		OnChangeSelectDelegate.Clear();
	}
	if (true == OnChangeUserSelectDelegate.IsBound())
	{
		OnChangeUserSelectDelegate.Clear();
	}

	_itemGrade = ItemGrade::NONE;
	_currentInputCount = 0;
	_maxStackCount = 0;
	_itemDBID = 0;
	_widgetIndex = 0;
	_isSelected = false;

	_iconHelper->Clear();
}

void UGsUIIconItemDecomposeEntry::SetItemInfo(const uint64 InItemDBID , const int32 InIndex)
{
	TWeakPtr<FGsItem> findItem = GItem()->FindItem(InItemDBID);
	if (false == findItem.IsValid())
		return;

	FGsItem* itemData = findItem.Pin().Get();
	_itemDBID = InItemDBID;
	_widgetIndex = InIndex;
	_itemGrade = itemData->GetGradeType();

	_itemNameText->SetText(itemData->GetName());
	SetItemTextColor(itemData->GetGradeType(), *_itemNameText);

	if (itemData->IsEquipment())
	{
		_displayTypeSwitcher->SetActiveWidgetIndex(0);
		FGsItemEquipment* equipData = itemData->GetCastItem<FGsItemEquipment>();
		if (nullptr == equipData)
			return;

		FString name = equipData->GetBonusStatTitleName();
		if (name.IsEmpty())
		{
			_itemSpecialStatNameText->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			_itemSpecialStatNameText->SetVisibility(ESlateVisibility::Visible);
			_itemSpecialStatNameText->SetText(FText::FromString(name));
			SetItemTextColor(itemData->GetGradeType(), *_itemSpecialStatNameText);
		}

		uint8 count = equipData->GetEnchantShieldCount();
		_itemShieldCountText->SetText(FText::AsNumber(count));
	}
	else
	{
		_displayTypeSwitcher->SetActiveWidgetIndex(1);

		FText subCategoryNameText;
		itemData->GetCategoryName(subCategoryNameText);
		_skillbookTypeText->SetText(subCategoryNameText);
		
		FString result = FString::Format(TEXT("{0}/{1}"), { FString::FromInt(_currentInputCount) , FString::FromInt(itemData->GetAmount()) });
		_skillbookAmountText->SetText(FText::FromString(result));

		_maxStackCount = itemData->GetAmount();
	}

	_iconHelper->RefreshAll(1);
}

void UGsUIIconItemDecomposeEntry::OnClickSelect()
{
	_isSelected = true == _isSelected ? false : true;

	// 0 : on  1 : off
	_selectSwitcher->SetActiveWidgetIndex(!_isSelected);

	if (true == OnChangeUserSelectDelegate.IsBound())
	{
		OnChangeUserSelectDelegate.Broadcast(_itemDBID, _isSelected , _widgetIndex);
	}
}

// 외부에서 강제로 선택 처리 함
void UGsUIIconItemDecomposeEntry::SetSelectionInfo(const bool InFlag, const bool InExcuteEvent)
{
	_isSelected = InFlag;

	// 0 : on  1 : off
	_selectSwitcher->SetActiveWidgetIndex(!_isSelected);

	if (true == OnChangeSelectDelegate.IsBound() && true == InExcuteEvent)
	{
		OnChangeSelectDelegate.Broadcast(_itemDBID, _isSelected, _widgetIndex);
	}
}

void UGsUIIconItemDecomposeEntry::OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	if(0 >= _itemDBID)
	{
		itemIcon->SetOwnedItem(0);
		return;
	}

	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIIconItemDecomposeEntry::OnLongPressIcon);
	}

	itemIcon->SetOwnedItem(_itemDBID);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
	itemIcon->SetItemCollectionMarking(itemIcon->GetItemIconData(), true);
}

void UGsUIIconItemDecomposeEntry::OnCreateIconWrapBox(class UGsUIIconBase* InIcon)
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

void UGsUIIconItemDecomposeEntry::OnLongPressIcon(class UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideBG = true;
	option._bUseCollectionMarking = true;
	option._bHideButtons = true;
	option._bHideButtonsBottom = true;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), FGsItemDetailOpenMessageParamEX::PosDir::RightPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}

void UGsUIIconItemDecomposeEntry::SetItemTextColor(ItemGrade InGrade, UTextBlock& InTextBlock)
{
	InTextBlock.SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(InGrade));
}

void UGsUIIconItemDecomposeEntry::InvalidateCurrentInputCount()
{
	FString result = FString::Format(TEXT("{0}/{1}"), { FString::FromInt(_currentInputCount) , FString::FromInt(_maxStackCount) });
	_skillbookAmountText->SetText(FText::FromString(result));
}

void UGsUIIconItemDecomposeEntry::SetStackCount(const int32 InStackCount)
{
	_currentInputCount = InStackCount;
	InvalidateCurrentInputCount();
}

int32 UGsUIIconItemDecomposeEntry::GetStackCount()
{
	return _currentInputCount;
}