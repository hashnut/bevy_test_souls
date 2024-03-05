// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/SelectItemBox/GsUIPopupSelectItemBox.h"

#include "Components/WrapBox.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"

#include "GsUISelectItemEntry.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UI/UIContent/Popup/GsUIPopupCalculator.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"


void UGsUIPopupSelectItemBox::BeginDestroy()
{
	if (nullptr != _itemListPanelSlotHelper)
	{
		_itemListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupSelectItemBox::OnRefreshItemGroup);
		_itemListPanelSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPopupSelectItemBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnConfirm->OnClicked.AddDynamic(this, &UGsUIPopupSelectItemBox::OnConfrim);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupSelectItemBox::OnCancel);
	_btnCalcPlus->OnClicked.AddDynamic(this, &UGsUIPopupSelectItemBox::OnPlus);
	_btnCalcMin->OnClicked.AddDynamic(this, &UGsUIPopupSelectItemBox::OnMin);
	_btnCalculator->OnClicked.AddDynamic(this, &UGsUIPopupSelectItemBox::OnCalculator);

	if (nullptr == _itemListPanelSlotHelper)
	{
		_itemListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_itemListPanelSlotHelper->Initialize(_itemListEntryWidgetClass, _entryRootPanel);
		_itemListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupSelectItemBox::OnRefreshItemGroup);
	}
}

void UGsUIPopupSelectItemBox::NativeConstruct()
{
	Super::NativeConstruct();

	_isEnableCalc = false;
	_curCalcNum = 0;
	_itemDBId = 0;
	_selectedIndex = -1;
}

void UGsUIPopupSelectItemBox::NativeDestruct()
{
	if (UGsUIManager* uiManager = GUI())
	{
		if (uiManager->IsActiveWidget(TEXT("PopupCalculator")))
		{
			uiManager->CloseByKeyName(TEXT("PopupCalculator"));
		}
	}
	_itemListPanelSlotHelper->RefreshAll(0);
	_boxData = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupSelectItemBox::SetItemList(const uint64 InItemDBID, const FGsSchemaRewardBoxData* InBoxData, const bool InIsSeveralUse)
{
	if (nullptr == InBoxData)
		return;

	_isEnableCalc = InIsSeveralUse;
	_calcRootPanel->SetVisibility(_isEnableCalc ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	_itemDBId = InItemDBID;
	_boxData = InBoxData;
	_itemListPanelSlotHelper->RefreshAll(_boxData->itemBagList.Num());

	InvalidateCalc();
}

void UGsUIPopupSelectItemBox::OnConfrim()
{
	if (0 > _selectedIndex)
	{
		// 아이템을 선택해주세요.
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_ItemBox_SelectionRequired"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}

	if (_isEnableCalc && 0 >= _curCalcNum)
	{
		// 수량을 선택해주세요.
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_ItemBox_CountRequired"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}

	struct FGsConfirmPickOne paramData(_itemDBId, _selectedIndex, _curCalcNum);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::PICKONEBOX_SELECT, &paramData);
}

void UGsUIPopupSelectItemBox::OnCancel()
{
	Close();
}

void UGsUIPopupSelectItemBox::OnRefreshItemGroup(int32 InIndex, UWidget* InEntry)
{
	UGsUISelectItemEntry* entry = Cast<UGsUISelectItemEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (false == _boxData->itemBagList.IsValidIndex(InIndex))
		return;

	if (false == entry->OnSelectIndexDelegate.IsBoundToObject(this))
	{
		entry->OnSelectIndexDelegate.AddUObject(this, &UGsUIPopupSelectItemBox::OnClickEntryWidget);
	}
	entry->SetData(InIndex, &_boxData->itemBagList[InIndex]);
}

void UGsUIPopupSelectItemBox::OnClickEntryWidget(uint8 InIndex)
{
	for (uint16 i = 0; i < _entryRootPanel->GetChildrenCount(); ++i)
	{
		UWidget* widget = _entryRootPanel->GetChildAt(i);
		if (nullptr == widget)
			continue;

		UGsUISelectItemEntry* entry = Cast<UGsUISelectItemEntry>(widget);
		if (nullptr == entry)
			continue;

		entry->SetSelectedImage(i == InIndex);
	}

	_selectedIndex = InIndex;
}

void UGsUIPopupSelectItemBox::InvalidateCalc()
{
	if (false == _isEnableCalc)
		return;

	TWeakPtr<FGsItem> itemData = GItem()->FindItem(_itemDBId);
	if (itemData.IsValid())
	{
		if (itemData.Pin()->GetAmount() < _curCalcNum)
		{
			_curCalcNum = itemData.Pin()->GetAmount();
		}
		FString resultStr;
		FText markText;
		FText::FindText(TEXT("MarkFormatingText"), TEXT("Slash"), markText);
		resultStr = FString::FromInt(_curCalcNum) + markText.ToString() + FString::FromInt(itemData.Pin()->GetAmount());
		_calcCountNum->SetText(FText::FromString(resultStr));	
	}
}

void UGsUIPopupSelectItemBox::OnPlus()
{
	++_curCalcNum;
	InvalidateCalc();
}

void UGsUIPopupSelectItemBox::OnMin()
{
	if (0 >= _curCalcNum)
	{
		_curCalcNum = 0;
	}
	else
	{
		--_curCalcNum;
	}
	InvalidateCalc();
}

void UGsUIPopupSelectItemBox::OnCalculator()
{
	TWeakPtr<FGsItem> itemData = GItem()->FindItem(_itemDBId);
	if (itemData.IsValid())
	{
		// 계산기 출력
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCalculator"));
		if (widget.IsValid())
		{
			UGsUIPopupCalculator* popup = Cast<UGsUIPopupCalculator>(widget.Get());
			if (nullptr == popup)
				return;

			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Button_UseAll"), findText);
			popup->SetDimmed(true);
			popup->SetTitleText(findText);
			popup->SetMinimumCount(0);
			popup->SetMaximumCount(itemData.Pin()->GetAmount());
			popup->SetDefaultCount(0);
			popup->SetButtonCallBack([&, this, popup](bool state)
				{
					if (state)
					{
						OnConfirmSeveralUseCalculator(popup->GetCurrentNum());
					}
				});
		}
	}
}

void UGsUIPopupSelectItemBox::OnConfirmSeveralUseCalculator(uint32 InCount)
{
	_curCalcNum = InCount;
	InvalidateCalc();
}