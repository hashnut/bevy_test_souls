// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupCommonItemConsume.h"
#include "RichTextBlock.h"
#include "TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Item/GsItemManager.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/Define/EGsIconType.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "../Common/Icon/GsUIIconItemInventory.h"
#include "../Common/Icon/GsUIIconItem.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContents.h"
#include "Item/GsItem.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Currency/GsCurrencyHelper.h"




void UGsUIPopupCommonItemConsume::NativeConstruct()
{	
	Super::NativeConstruct();

	// 아이콘 생성
	if (nullptr == _uiIcon)
	{
		_uiIcon = Cast<UGsUIIconItemInventory>(_iconSelector->CreateIcon(EGsIconType::ICON_ITEM_INVENTORY_L, 0));
		_iconSelector->AddChild(_uiIcon);

		// 2023/5/11 PKT - long Pressed 지원 안함.
		//                 https://jira.com2us.com/jira/browse/C2URWQ-3832
		_uiIcon->OnClickSlot.AddUObject(this, &UGsUIPopupCommonItemConsume::OnClickItemIcon);
		
	}

	_btnOK->SetVisibility(ESlateVisibility::Visible);
	
	_btnOK->SetIsEnabled(true);


	_btnNo->OnClicked.AddDynamic(this, &UGsUIPopupCommonItemConsume::OnClickCancel);

	_btnOK->OnClicked.AddDynamic(this, &UGsUIPopupCommonItemConsume::OnClickOk);
}



void UGsUIPopupCommonItemConsume::NativeDestruct()
{
	_btnNo->OnClicked.RemoveDynamic(this, &UGsUIPopupCommonItemConsume::OnClickCancel);

	_btnOK->OnClicked.RemoveDynamic(this, &UGsUIPopupCommonItemConsume::OnClickOk);

	_buttonCallback = nullptr;

	// 아이콘 제거(_iconSelector에 Add되었던 것 삭제)
	_iconSelector->RemoveAllChildren();
	_uiIcon = nullptr;
	
	Super::NativeDestruct();
}

void UGsUIPopupCommonItemConsume::SetData(const FText& InContent, int32 InCostItemId, int64 InCostItemAmount,
	TFunction<void(bool)> InCallback)
{
	_buttonCallback = InCallback;
	_richTextContent->SetText(InContent);
	_underContentRichText->SetVisibility(ESlateVisibility::Collapsed);
	_costItemId = InCostItemId;
	_costItemAmount = InCostItemAmount;

	int64 localItemAmount = GItem()->GetAllItemCountByTID(InCostItemId);
	/*if (localItemAmount < InCostItemAmount)
	{
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_NotEnoughScroll"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		OnClickCancel();
		return;
	}*/

	if (0 != InCostItemId)
	{
		_uiIcon->SetNotOwnItem(InCostItemId, 0);
		_uiIcon->SetItemNameVisibility(false);
		_uiIcon->SetActiveIconBg(true);
	}
	else
	{
		_uiIcon->SetEmptyIcon();
	}

	_uiIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::OnClickType);

	SetAmountCount(localItemAmount, InCostItemAmount);
}

void UGsUIPopupCommonItemConsume::SetData(const FText& InContent, const FText& InUnderContent, int32 InCostItemId, 
	int64 InCostItemAmount, TFunction<void(bool)> InCallback)
{
	_buttonCallback = InCallback;
	_richTextContent->SetText(InContent);
	_underContentRichText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_underContentRichText->SetText(InUnderContent);

	int64 localItemAmount = GItem()->GetAllItemCountByTID(InCostItemId);
	if (localItemAmount < InCostItemAmount)
	{
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_NotEnoughScroll"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		OnClickCancel();
		return;
	}

	if (0 != InCostItemId)
	{
		_uiIcon->SetNotOwnItem(InCostItemId, 0);
		_uiIcon->SetItemNameVisibility(false);
		_uiIcon->SetActiveIconBg(true);
	}
	else
	{
		_uiIcon->SetEmptyIcon();
	}

	_uiIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::OnClickType);

	// 2021/12/27 PKT - 보유 수량 / 필요 수량
	SetAmountCount(localItemAmount, InCostItemAmount);	
}

void UGsUIPopupCommonItemConsume::SetDataNotCheckItem(const FText& InContent
													, const FText& InUnderContent
													, int32 InCostItemId
													, int64 InCostItemAmount
													, TFunction<void(bool)> InCallback /*= nullptr*/)
{
	_buttonCallback = InCallback;
	_richTextContent->SetText(InContent);
	if (InUnderContent.IsEmpty())
	{
		_underContentRichText->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_underContentRichText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_underContentRichText->SetText(InUnderContent);
	}
	
	_uiIcon->SetNotOwnItem(InCostItemId, 0);
	_uiIcon->SetItemNameVisibility(false);
	_uiIcon->SetActiveIconBg(true);

	_uiIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::OnClickType);

	ItemAmount localItemAmount = GItem()->GetAllItemCountByTID(InCostItemId);
	if (0 >= localItemAmount)
	{
		_btnOK->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_btnOK->SetIsEnabled(false);
	}
	
	SetAmountCount(localItemAmount, InCostItemAmount);
}

void UGsUIPopupCommonItemConsume::SetDataNotCheckItemByGroupId(const FText& InContent
															, const FText& InUnderContent
															, int32 InDispayItemId
															, int64 InCostItemAmount
															, TFunction<void(bool)> InCallback /*= nullptr*/)
{
	_buttonCallback = InCallback;
	_richTextContent->SetText(InContent);
	if (InUnderContent.IsEmpty())
	{
		_underContentRichText->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_underContentRichText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_underContentRichText->SetText(InUnderContent);
	}

	_uiIcon->SetNotOwnItem(InDispayItemId, 0);
	_uiIcon->SetItemNameVisibility(false);
	_uiIcon->SetActiveIconBg(true);

	_uiIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::OnClickType);

	TArray<TWeakPtr<FGsItem>> itemGroupList = GItem()->GetAllOwnedItemsTheSameGroup(InDispayItemId);

	ItemAmount localItemAmount = 0;
	for (const auto& itemData : itemGroupList)
	{
		localItemAmount += itemData.Pin()->GetAmount();
	}

	if (0 >= localItemAmount)
	{
		_btnOK->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_btnOK->SetIsEnabled(false);
	}

	SetAmountCount(localItemAmount, InCostItemAmount);
}

void UGsUIPopupCommonItemConsume::SetDataByGroupId(const FText& InContent
												, const FText& InUnderContent
												, int32 InDispayItemId
												, int64 InCostItemAmount
												, TFunction<void(bool)> InCallback /*= nullptr*/)
{
	_buttonCallback = InCallback;
	_richTextContent->SetText(InContent);
	if (InUnderContent.IsEmpty())
	{
		_underContentRichText->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_underContentRichText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_underContentRichText->SetText(InUnderContent);
	}

	TArray<TWeakPtr<FGsItem>> itemGroupList = GItem()->GetAllOwnedItemsTheSameGroup(InDispayItemId);
	ItemId displayItemID = INVALID_ITEM_ID;
	ItemDBId displayItemDBID = INVALID_ITEM_DB_ID;
	ItemAmount localItemAmount = 0;

	for (auto item : itemGroupList)
	{
		if (false == item.IsValid() || nullptr == item.Pin().Get()->GetTableData())
		{
			continue;
		}

		if (false == item.Pin().Get()->GetTableData()->tradable)
		{	// 2023/12/11 PKT - 최 우선적으로 귀속 아이템을 넣어 주고
			displayItemID = item.Pin().Get()->GetTID();
			displayItemDBID = item.Pin().Get()->GetDBID();
		}
		else if (displayItemID == INVALID_ITEM_ID)
		{	// 2023/12/11 PKT - 귀속 아이템이 없을때에는 일반(비귀속) 아이템을 넣어 준다.
			displayItemID = item.Pin().Get()->GetTID();
			displayItemDBID = item.Pin().Get()->GetDBID();
		}

		localItemAmount += item.Pin().Get()->GetAmount();
	}

	if (INVALID_ITEM_DB_ID < displayItemDBID)
	{
		_uiIcon->SetOwnedItem(displayItemDBID);
	}
	else
	{
		_uiIcon->SetNotOwnItem(InDispayItemId, 0);
	}
	_uiIcon->SetItemNameVisibility(false);
	_uiIcon->SetActiveIconBg(true);
	_uiIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::OnClickType);

	if (0 >= localItemAmount)
	{
		_btnOK->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_btnOK->SetIsEnabled(false);
	}

	SetAmountCount(localItemAmount, InCostItemAmount);
}

void UGsUIPopupCommonItemConsume::OnClickOk()
{	
	int64 localItemAmount = GItem()->GetAllItemCountByTID(_costItemId);
	if (localItemAmount < _costItemAmount)
	{
		FGsCurrencyHelper::OpenLackItemPopup(_costItemId, _costItemAmount, false);
	}
	else
	{
		if (nullptr != _buttonCallback)
		{
			_buttonCallback(true);
		}
	}	

	// 창 닫기
	Close();
}

void UGsUIPopupCommonItemConsume::OnClickCancel()
{
	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}

	// 창 닫기
	Close();
}


void UGsUIPopupCommonItemConsume::OnClickItemIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (itemIcon)
	{
		if (FGsItem* item = itemIcon->GetItemIconData())
		{
			FGsItemDetailOpenMessageParam param(item, true, true, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
			GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
		}
	}
}

void UGsUIPopupCommonItemConsume::SetAmountCount(const int64 InCostItemAmount, const int64 localItemAmount)
{
	_textItemCostAmount->SetText(FText::AsNumber(InCostItemAmount));

	const static int32 DEFAULT_LACK_AMOUNT = 0;
	if (DEFAULT_LACK_AMOUNT >= InCostItemAmount)
	{
		_textItemCostAmount->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_LACK));
	}
	else
	{
		_textItemCostAmount->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_VALID));
	}

	_textLocalItemAmount->SetText(FText::AsNumber(localItemAmount));	
}

// 2022/10/20 PKT - KeyBoard Mapping
void UGsUIPopupCommonItemConsume::OnInputCancel()
{
	OnClickCancel();
}