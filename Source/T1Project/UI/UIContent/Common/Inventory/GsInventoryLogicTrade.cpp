// Fill out your copyright notice in the Description page of Project Settings.


#include "GsInventoryLogicTrade.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"

#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Popup/GsUIPopupCalculator.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsPersonalTradingManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"


void FGsInventoryLogicTrade::Clear()
{
	_dataList.Empty();

	Super::Clear();
}

void FGsInventoryLogicTrade::OnInvalidateList()
{
	_dataList.Empty();

	if (UGsItemManager* itemManager = GItem())
	{
		switch (_currTabType)
		{
		case EInventoryTab::AllTab:
			GetItemList(ItemType::EQUIP, _dataList);
			GetItemList(ItemType::CONSUME, _dataList);
			GetItemList(ItemType::INGREDIENT, _dataList);
			GetItemList(ItemType::CURRENCY, _dataList);
			break;
		case EInventoryTab::EquipmentTab:
			GetItemList(ItemType::EQUIP, _dataList);
			break;
		case EInventoryTab::ConsumableTab:
			GetItemList(ItemType::CONSUME, _dataList);
			break;
		case EInventoryTab::MaterialTab:
			GetItemList(ItemType::INGREDIENT, _dataList);
			GetItemList(ItemType::CURRENCY, _dataList);
			break;
		}
	}

	_dataList.Sort([this](FGsItem& A, FGsItem& B)
		{
			bool isTradableItemA = IsTradableItem(&A);
			bool isTradableItemB = IsTradableItem(&B);

			if (isTradableItemA)
			{
				if (isTradableItemA == isTradableItemB)
				{
					return SortTradable(A, B);
				}
				return true;
			}
			else
			{
				if (isTradableItemA == isTradableItemB)
				{
					return SortNonTradable(A, B);
				}
			}
			return false;
		});
}

void FGsInventoryLogicTrade::OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;

	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	if (false == _dataList.IsValidIndex(InIndex))
	{
		itemIcon->SetEmptyIcon();
		itemIcon->SetItemNameToolTipFlag(false);
		return;
	}

	if (FGsItem* itemData = _dataList[InIndex])
	{
		if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			itemIcon->OnFinishedLongPressEvent.AddRaw(this, &FGsInventoryLogicTrade::OnLongPressIcon);
		}

		itemIcon->SetOwnedItem(itemData->GetDBID());
		itemIcon->SetActiveIconBg(true);
		itemIcon->SetItemNameVisibility(false);
		itemIcon->SetItemNameToolTipFlag(true);
		itemIcon->SetItemNameToolTipHoverMode(true);
		itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
		itemIcon->SetItemCollectionMarking(itemIcon->GetItemIconData(), true);

		// 팔 수 없으면 딤드
		bool isDimmed = (false == IsTradableItem(itemData));
		itemIcon->SetDimmedImage(isDimmed);
		// 선택된 아이콘 처리
		itemIcon->SetSlotSelectionImage(tradeManager->IsSelectedItem(itemData->GetDBID()));
	}
}

void FGsInventoryLogicTrade::OnClickItemIcon(UGsUIIconItem& InIcon)
{
	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	if (tradeManager->GetLocalPersonalTradingState() != PersonalTradingState::TRADING)
		return;

	// 이미 요청중인지 확인
	if (tradeManager->IsRequestingItem(InIcon.GetDBID()))
		return;

	FGsItem* itemData = InIcon.GetItemIconData();
	if (nullptr == itemData)
		return;

	if (false == IsTradableItem(itemData))
		return;

	if (itemData->IsStackable() && itemData->GetAmount() > 1)
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCalculator"));
		if (!widget.IsValid())
			return;

		UGsUIPopupCalculator* popupCalculator = Cast<UGsUIPopupCalculator>(widget.Get());
		if (nullptr == popupCalculator)
			return;

		ItemAmount defaultCount = 1;
		const TArray<TPair<ItemDBId, ItemAmount>>& tradeItemDataList = tradeManager->GetLocalItemDataList();
		for (int32 i = 0;i < tradeItemDataList.Num();++i)
		{
			if (tradeItemDataList[i].Key == InIcon.GetDBID())
			{
				defaultCount = tradeItemDataList[i].Value;
			}
		}

		_selectedItemDBIdCalc = InIcon.GetDBID();
		popupCalculator->SetDimmed(true);
		popupCalculator->SetResetCount();
		popupCalculator->SetMinimumCount(0);
		popupCalculator->SetMaximumCount(itemData->GetAmount());
		popupCalculator->SetDefaultCount(defaultCount);
		popupCalculator->InvalidateNumText();

		if (!popupCalculator->OnFinishCalculateDelegate.IsBoundToObject(this))
		{
			popupCalculator->OnFinishCalculateDelegate.AddRaw(this, &FGsInventoryLogicTrade::OnFinishCalculate);
		}
	}
	else
	{
		const TArray<TPair<ItemDBId, ItemAmount>>& tradeItemDataList = tradeManager->GetLocalItemDataList();
		for (int32 i = 0;i < tradeItemDataList.Num();++i)
		{
			if (tradeItemDataList[i].Key == InIcon.GetDBID())
			{
				// 이미 선택한 아이템이다.
				return;
			}
		}

		// 등록된 아이템 수량 + 요청중인 수량 확인
		if (tradeItemDataList.Num() + tradeManager->GetLocalItemDataWaitListCount() < tradeManager->GetSlotCountMax())
		{
			tradeManager->ReqLocalItemUpdate(InIcon.GetDBID(), 1);
		}
		else
		{
			// 최대로 등록된 상태
			FText findText;
			FText::FindText(TEXT("TradeText"), TEXT("Notice_OverSlotCount"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		}
	}
}

void FGsInventoryLogicTrade::OnSelectTab(EInventoryTab InType)
{
	_currTabType = InType;
}

int32 FGsInventoryLogicTrade::GetDataCount() const
{
	return GSInventory()->GetInvenSlotMaxCount();
}

int32 FGsInventoryLogicTrade::GetDataIndex(ItemDBId InItemDBId) const
{
	return _dataList.IndexOfByPredicate([InItemDBId](const FGsItem* InItem)
		{
			return (nullptr != InItem && InItem->GetDBID() == InItemDBId);
		}
	);
}

void FGsInventoryLogicTrade::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
		return;

	FGsItem* itemData = itemIcon->GetItemIconData();
	FGsItem* itemRealData = nullptr;
	for (int32 i = 0; i < _dataList.Num(); ++i)
	{
		itemRealData = _dataList[i];
		if (itemData->GetDBID() == itemRealData->GetDBID())
		{
			itemIcon->SetRedDot(false);
			itemData->SetIsNew(false);
			itemRealData->SetIsNew(false);
		}
	}

	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideButtons = true;
	option._bHideBG = true;
	option._bHideButtonsBottom = false;
	option._bUseLock = false;
	option._bUseCollectionMarking = true;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), UGsUIPopupItemDetailMain::CenterPosType::RightPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}

void FGsInventoryLogicTrade::OnFinishCalculate(int64 InValue)
{
	ItemDBId itemDBId = _selectedItemDBIdCalc;
	_selectedItemDBIdCalc = INVALID_ITEM_DB_ID;

	TWeakPtr<FGsItem> itemData = GItem()->FindItem(itemDBId);
	if (false == itemData.IsValid())
		return;
	
	bool isAdd = true;
	int64 amount = FMath::Min<int64>(itemData.Pin()->GetAmount(), InValue);
	if (FGsPersonalTradingManager* tradeManager = GSPersonalTrading())
	{
		const TArray<TPair<ItemDBId, ItemAmount>>& tradeItemDataList = tradeManager->GetLocalItemDataList();
		for (int32 i = 0;i < tradeItemDataList.Num();++i)
		{
			if (tradeItemDataList[i].Key == itemDBId)
			{
				isAdd = false;
				if (InValue == tradeItemDataList[i].Value)
				{
					// 동일 수량일 경우 패킷을 보내지 않는다.
					return;
				}
			}
		}

		if (isAdd && tradeItemDataList.Num() >= tradeManager->GetSlotCountMax())
		{
			// 최대로 등록된 상태
			FText findText;
			FText::FindText(TEXT("TradeText"), TEXT("Notice_OverSlotCount"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		}
		else
		{
			tradeManager->ReqLocalItemUpdate(itemDBId, amount);
		}
	}
}

void FGsInventoryLogicTrade::GetItemList(ItemType InItemType, OUT TArray<FGsItem*>& OutList)
{
	if (UGsItemManager* itemManager = GItem())
	{
		TArray<TWeakPtr<FGsItem>> itemList = itemManager->FindItem(InItemType);
		for (TWeakPtr<FGsItem> iter : itemList)
		{
			if (false == iter.IsValid())
				continue;

			OutList.Emplace(iter.Pin().Get());
		}
	}
}

bool FGsInventoryLogicTrade::IsTradableItem(const FGsItem* InItem)
{
	if (InItem->GetLock())
		return false;
	
	if (InItem->IsEquipped())
		return false;

	if (InItem->IsPresetEquipped())
		return false;

	return InItem->IsPersonalTradable();
}

bool FGsInventoryLogicTrade::SortTradable(FGsItem& A, FGsItem& B)
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

bool FGsInventoryLogicTrade::SortNonTradable(FGsItem& A, FGsItem& B)
{
	if (A.IsEquipped() || A.IsPresetEquipped())
	{
		if (B.IsEquipped() || B.IsPresetEquipped())
		{
			return SortTradable(A, B);
		}

		return true;
	}
	else
	{
		if (false == B.IsEquipped() && false == B.IsPresetEquipped())
		{
			return SortTradable(A, B);
		}
	}

	return false;
}
