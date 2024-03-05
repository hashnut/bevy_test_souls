// Fill out your copyright notice in the Description page of Project Settings.


#include "GsInventoryLogicMarket.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Management/GsScopeHolder.h"
#include "Management/GsMessageHolder.h"
#include "Market/GsMarketHelper.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"


void FGsInventoryLogicMarket::Clear()
{
	_dataList.Empty();

	Super::Clear();
}

void FGsInventoryLogicMarket::OnInvalidateList()
{
	_dataList.Empty();

	GetItemList(_currTabType, _dataList);
}

void FGsInventoryLogicMarket::OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	if (_dataList.IsValidIndex(InIndex))
	{
		if (UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon))
		{
			if (FGsItem* itemData = _dataList[InIndex])
			{
				itemIcon->SetOwnedItem(itemData->GetDBID(), UGsUIIconItem::Storage_All);
				itemIcon->SetActiveIconBg(true);
				itemIcon->SetItemNameVisibility(false);
				itemIcon->SetItemNameToolTipFlag(true);
				itemIcon->SetItemNameToolTipHoverMode(true);
				itemIcon->SetItemCollectionMarking(itemIcon->GetItemIconData(), true);

				// 롱프레스
				if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
				{
					itemIcon->OnFinishedLongPressEvent.AddRaw(this, &FGsInventoryLogicMarket::OnLongPressIcon);
				}
				itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);

				// 팔 수 없으면 Dimmed 처리
				itemIcon->SetDimmedImage(!CanSellItem(itemData));
				
				// 선택된 아이콘 처리
				itemIcon->SetSlotSelectionImage((itemData->GetDBID() == _selectedItemDBId) ? true : false);
			}
		}
	}
}

void FGsInventoryLogicMarket::OnSelectItemIcon(UGsUIIconItem* InPrevIcon, UGsUIIconItem* InCurrIcon)
{
	// 이전 아이콘 취소 표시
	if (InPrevIcon)
	{
		if (UGsUIIconItemInventory* prevIcon = Cast<UGsUIIconItemInventory>(InPrevIcon))
		{
			prevIcon->SetSlotSelectionImage(false);
		}
	}
	
	// 현재 아이콘 선택 표시
	if (InCurrIcon)
	{
		if (FGsItem* item = InCurrIcon->GetItemIconData())
		{
			if (UGsUIIconItemInventory* currIcon = Cast<UGsUIIconItemInventory>(InCurrIcon))
			{
				currIcon->SetSlotSelectionImage(true);
			}

			SetSelectedItemDBId(item->GetDBID());
		}
		else
		{ 
			SetSelectedItemDBId(INVALID_ITEM_DB_ID);
		}
	}
}

int32 FGsInventoryLogicMarket::GetDataIndex(ItemDBId InItemDBId) const
{
	for (int32 i = 0, maxCount = _dataList.Num(); i < maxCount; ++i)
	{
		if (_dataList[i]->GetDBID() == InItemDBId)
		{
			return i;
		}
	}

	return -1;
}

void FGsInventoryLogicMarket::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideButtons = false;
	option._bHideBG = false;
	option._bHideButtonsBottom = true;
	option._bUseLock = false;
	option._bUseCollectionMarking = true;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), UGsUIPopupItemDetailMain::CenterPosType::CenterPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}

void FGsInventoryLogicMarket::GetItemList(EInventoryTab InTabType, OUT TArray<FGsItem*>& OutList)
{
	UGsItemManager* itemMgr = GItem();
	if (nullptr == GItem())
	{
		return;
	}

	switch (InTabType)
	{
	case EInventoryTab::AllTab:
	{
		TArray<TWeakPtr<FGsItem>> consumList = itemMgr->FindItem(ItemType::CONSUME);
		for (TWeakPtr<FGsItem> iter : consumList)
		{
			if (false == iter.IsValid())
				continue;

			OutList.Emplace(iter.Pin().Get());
		}

		TArray<TWeakPtr<FGsItem>> equipList = itemMgr->FindItem(ItemType::EQUIP);
		for (TWeakPtr<FGsItem> iter : equipList)
		{
			if (false == iter.IsValid())
				continue;

			OutList.Emplace(iter.Pin().Get());
		}

		TArray<TWeakPtr<FGsItem>> ingredientList = itemMgr->FindItem(ItemType::INGREDIENT);
		for (TWeakPtr<FGsItem> iter : ingredientList)
		{
			if (false == iter.IsValid())
				continue;

			OutList.Emplace(iter.Pin().Get());
		}

		TArray<TWeakPtr<FGsItem>> currencyList = itemMgr->FindItem(ItemType::CURRENCY);
		for (TWeakPtr<FGsItem> iter : currencyList)
		{
			if (false == iter.IsValid())
				continue;

			OutList.Emplace(iter.Pin().Get());
		}
	}
	break;
	case EInventoryTab::EquipmentTab:
	{
		TArray<TWeakPtr<FGsItem>> equipList = itemMgr->FindItem(ItemType::EQUIP);
		for (TWeakPtr<FGsItem> iter : equipList)
		{
			if (false == iter.IsValid())
				continue;

			OutList.Emplace(iter.Pin().Get());
		}
	}
	break;
	case EInventoryTab::ConsumableTab:
	{
		TArray<TWeakPtr<FGsItem>> consumList = itemMgr->FindItem(ItemType::CONSUME);
		for (TWeakPtr<FGsItem> iter : consumList)
		{
			if (false == iter.IsValid())
				continue;

			OutList.Emplace(iter.Pin().Get());
		}
	}
	break;
	case EInventoryTab::MaterialTab:
	{
		TArray<TWeakPtr<FGsItem>> ingredientList = itemMgr->FindItem(ItemType::INGREDIENT);
		for (TWeakPtr<FGsItem> iter : ingredientList)
		{
			if (false == iter.IsValid())
				continue;

			OutList.Emplace(iter.Pin().Get());
		}

		TArray<TWeakPtr<FGsItem>> currencyList = itemMgr->FindItem(ItemType::CURRENCY);
		for (TWeakPtr<FGsItem> iter : currencyList)
		{
			if (false == iter.IsValid())
				continue;

			OutList.Emplace(iter.Pin().Get());
		}
	}
	break;
	}

	// 소팅 
	// 1순위 판매가능 아이템
	// 2순위 (판매 불가능 경우) 장착 아이템
	// 3순위 카테고리
	// 4순위 등급
	// 5순위 강화도
	OutList.Sort([this](FGsItem& A, FGsItem& B)
		{
			bool bSellableA = CanSellItem(&A);
			bool bSellableB = CanSellItem(&B);

			if (bSellableA)
			{
				if (bSellableA == bSellableB)
				{
					return SortItem(A, B);
				}

				return true;
			}
			else
			{
				if (bSellableA == bSellableB)
				{
					return SortUnsellableItem(A, B);
				}
			}

			return false;
		});
}

bool FGsInventoryLogicMarket::SortUnsellableItem(FGsItem& A, FGsItem& B)
{
	if (A.IsEquipped())
	{
		if (B.IsEquipped())
		{
			return SortItem(A, B);
		}

		return true;
	}
	else
	{
		if (false == B.IsEquipped())
		{
			return SortItem(A, B);
		}
	}

	return false;
}

bool FGsInventoryLogicMarket::SortItem(FGsItem& A, FGsItem& B)
{
	if (A.GetSubCategory() < B.GetSubCategory())
	{
		return true;
	}
	else if (A.GetSubCategory() == B.GetSubCategory())
	{
		if (A.GetGradeType() > B.GetGradeType())
		{
			return true;
		}
		else if (A.GetGradeType() == B.GetGradeType())
		{
			return (A.GetLevel() >= B.GetLevel()) ? true : false;
		}
	}

	return false;
}

bool FGsInventoryLogicMarket::CanSellItem(const FGsItem* InItem, bool bShowTicker)
{
	if (false == InItem->GetLock())
	{
		if (InItem->GetTableData() &&
			InItem->GetTableData()->tradable &&
			InItem->GetGradeType() >= _minItemGrade)
		{
			if (InItem->IsEquipped() || InItem->IsPresetEquipped())
			{
				if (bShowTicker)
				{
					// TEXT: 장비 중인 아이템은 판매할 수 없습니다. 판매하시려면 장착을 해제하고 다시 시도해 주세요
					FText textMsg;
					if (FText::FindText(TEXT("MarketText"), TEXT("Ticker_Sales_Fail_Equipped"), textMsg))
					{
						FGsUIHelper::TrayMessageTicker(textMsg);
					}
				}

				return false;
			}
			else
			{
				if (false == GItem()->IsCurrentPresetItem(InItem))
				{
					return true;
				}
			}
		}
	}

	if (bShowTicker)
	{
		// TEXT: 판매할 수 없는 아이템입니다
		FText textMsg;
		if (FText::FindText(TEXT("MarketText"), TEXT("Ticker_Sales_Fail_Bound"), textMsg))
		{
			FGsUIHelper::TrayMessageTicker(textMsg);
		}
	}

	return false;
}

void FGsInventoryLogicMarket::SetIsWorldMarket(bool bIsWorldMarket)
{
	_minItemGrade = bIsWorldMarket ? FGsMarketHelper::GetWorldMarketMinimumItemGrade() : ItemGrade::NONE;
}
