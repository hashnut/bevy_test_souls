
#include "GsUIPopupMonsterBookExpItem.h"
#include "../Item/GsItem.h"
#include "../Item/GsItemManager.h"
#include "../Currency/GsCostPackage.h"
#include "../Message/GsMessageItem.h"
#include "../Management/GsMessageHolder.h"
#include "../Management/ScopeGame/GsMonsterCollectionManager.h"
#include "../Message/GsMessageContents.h"
#include "../Message/MessageParam/GsItemContentsMessageParam.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "../UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "../UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "../UI/UIContent/Common/Inventory/GsWrapBoxInventoryIconSelector.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "T1Project.h"




void UGsUIPopupMonsterBookExpItem::BeginDestroy()
{
	if (_iconSlotHelper)
	{
		_iconSlotHelper->OnCreateIcon.RemoveDynamic(this, &UGsUIPopupMonsterBookExpItem::OnCreateItemIcon);
		_iconSlotHelper->OnRefreshIcon.RemoveDynamic(this, &UGsUIPopupMonsterBookExpItem::OnRefreshItemIcon);		
	}
	_iconSlotHelper = nullptr;

	if (_btnCalcReset)
	{
		_btnCalcReset->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterBookExpItem::OnClickedReset);		
	}
	_btnCalcReset = nullptr;

	if (_btnCalcMax)
	{
		_btnCalcMax->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterBookExpItem::OnClickedMax);
	}
	_btnCalcMax = nullptr;

	if (_btnDecrease)
	{
		_btnDecrease->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterBookExpItem::OnClickedDecrease);
	}
	_btnDecrease = nullptr;
	
	if (_btnIncrease)
	{
		_btnIncrease->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterBookExpItem::OnClickedIncrease);
	}
	_btnIncrease = nullptr;

	if (_btnDecreaseLarge)
	{
		_btnDecreaseLarge->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterBookExpItem::OnClickedLargeDecrease);
	}
	_btnDecreaseLarge = nullptr;

	if (_btnIncreaseLarge)
	{
		_btnIncreaseLarge->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterBookExpItem::OnClickedLargeIncrease);
	}
	_btnIncreaseLarge = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupMonsterBookExpItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_iconSlotHelper->Initialize(Cast<IGsIconSelectorInterface>(_iconSelector));
	_iconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUIPopupMonsterBookExpItem::OnCreateItemIcon);
	_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIPopupMonsterBookExpItem::OnRefreshItemIcon);
	
	_btnCalcReset->OnClicked.AddDynamic(this, &UGsUIPopupMonsterBookExpItem::OnClickedReset);
	_btnCalcMax->OnClicked.AddDynamic(this, &UGsUIPopupMonsterBookExpItem::OnClickedMax);
	_btnDecrease->OnClicked.AddDynamic(this, &UGsUIPopupMonsterBookExpItem::OnClickedDecrease);
	_btnIncrease->OnClicked.AddDynamic(this, &UGsUIPopupMonsterBookExpItem::OnClickedIncrease);
	_btnDecreaseLarge->OnClicked.AddDynamic(this, &UGsUIPopupMonsterBookExpItem::OnClickedLargeDecrease);
	_btnIncreaseLarge->OnClicked.AddDynamic(this, &UGsUIPopupMonsterBookExpItem::OnClickedLargeIncrease);
}

void UGsUIPopupMonsterBookExpItem::NativeConstruct()
{
	Super::NativeConstruct();

	_iconSlotHelper->Clear();

	_selectedIndex = UNSELECTED_INDEX;
	_amountUsed = EMPTY_AMOUNT;
		
	_textDecreaseLarge->SetText(FText::FromString(FString::Format(TEXT("-{0}"), { LAGRAGE_AMOUNT })));
	_textIncreaseLarge->SetText(FText::FromString(FString::Format(TEXT("+{0}"), { LAGRAGE_AMOUNT })));

	// 2021/08/02 PKT - inventory Item - New ..
	_listItemActionDelegates.Emplace(GMessage()->GetItem().AddUObject(MessageItem::ADDITEMLIST, this, &UGsUIPopupMonsterBookExpItem::OnAddItem));
	// 2021/08/02 PKT - inventory Item - Update ..
	_listItemActionDelegates.Emplace(GMessage()->GetItem().AddUObject(MessageItem::UPDATEITEM, this, &UGsUIPopupMonsterBookExpItem::OnUpdateItem));
	// 2021/08/02 PKT - inventory Item - Remove ..
	_listItemActionDelegates.Emplace(GMessage()->GetItem().AddUObject(MessageItem::REMOVEITEM, this, &UGsUIPopupMonsterBookExpItem::OnRemoveItem));
}

void UGsUIPopupMonsterBookExpItem::NativeDestruct()
{
	for (const TPair<MessageItem, FDelegateHandle>& ItemDelegate : _listItemActionDelegates)
	{
		GMessage()->GetItem().Remove(ItemDelegate);
	}
	_listItemActionDelegates.Empty();

	Super::NativeDestruct();
}

void UGsUIPopupMonsterBookExpItem::OnAddItem(FGsItemMessageParamBase& InParam)
{
	const FGsUpdateItem* Param = StaticCast<const FGsUpdateItem*>(&InParam);
	check(Param);

	FGsAddItemList* param = static_cast<FGsAddItemList*>(&InParam);
	if (nullptr == param)
	{
		return;
	}

	TWeakObjectPtr<UGsItemManager> itemManager = GItem();

	for (int32 idx = 0; idx < param->_itemDbidList.Num(); ++idx)
	{
		TWeakPtr<FGsItem> item = itemManager->FindItem(param->_itemDbidList[idx]);
		if (false == item.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == Item, Param->_itemDBID:%lld"), param->_itemDbidList[idx]);
			return;
		}

		if (ItemType::INGREDIENT != item.Pin().Get()->GetType())
		{
			return;
		}

		for (int32 i = 0; i < GsMonsterCollection()->BookExpCostSet().Num(); ++i)
		{
			const auto& costItem = GsMonsterCollection()->BookExpCostSet()[i];
			const FGsCostElement* costElement = costItem.GetFirstCostElement();
			if (nullptr == costElement)
			{
				GSLOG(Error, TEXT("nullptr == costElement"));
				continue;
			}

			if (item.Pin().Get()->GetTID() == costElement->GetItemId())
			{
				Invalidate();
			}
		}
	}
}

void UGsUIPopupMonsterBookExpItem::OnUpdateItem(FGsItemMessageParamBase& InParam)
{
	const FGsUpdateItem* Param = StaticCast<const FGsUpdateItem*>(&InParam);
	check(Param);

	TWeakPtr<const FGsItem> item = GItem()->FindItem(Param->_itemDBID);
	if (false == item.IsValid())
	{
		GSLOG(Error, TEXT("false == item.IsValid()"));
		return;
	}

	if (ItemType::INGREDIENT != item.Pin().Get()->GetType())
	{
		return;
	}

	for (int32 idx = 0; idx < GsMonsterCollection()->BookExpCostSet().Num(); ++idx)
	{
		const auto& costItem = GsMonsterCollection()->BookExpCostSet()[idx];
		const FGsCostElement* costElement = costItem.GetFirstCostElement();
		if (nullptr == costElement)
		{
			GSLOG(Error, TEXT("nullptr == costElement"));
			continue;
		}

		if (item.Pin().Get()->GetTID() == costElement->GetItemId())
		{
			Invalidate();
		}
	}
}

void UGsUIPopupMonsterBookExpItem::OnRemoveItem(FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	check(Param);

	for (int32 idx = 0; idx < GsMonsterCollection()->BookExpCostSet().Num(); ++idx)
	{
		const auto& costItem = GsMonsterCollection()->BookExpCostSet()[idx];
		const FGsCostElement* costElement = costItem.GetFirstCostElement();
		if (nullptr == costElement)
		{
			GSLOG(Error, TEXT("nullptr == costElement"));
			continue;
		}

		if (Param->_itemTID == costElement->GetItemId())
		{
			Invalidate();
		}
	}
}

void UGsUIPopupMonsterBookExpItem::OnCreateItemIcon(UGsUIIconBase* InIcon)
{
	TWeakObjectPtr<UGsUIIconItemInventory> itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (itemIcon.IsValid())
	{
		if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
		{
			itemIcon->OnClickSlot.AddUObject(this, &UGsUIPopupMonsterBookExpItem::OnSelectedItemIcon);
		}

		if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPopupMonsterBookExpItem::OnLongPressedItemIcon);
		}
	}
}

void UGsUIPopupMonsterBookExpItem::OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	TWeakObjectPtr<UGsUIIconItemInventory> itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (false == itemIcon.IsValid())
	{
		GSLOG(Error, TEXT("false == itemIcon.IsValid()"));
		return;
	}

	if (false == GsMonsterCollection()->BookExpCostSet().IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("false == GsMonsterCollection()->BookExpCostSet().IsValidIndex(InIndex) : [%d]"), InIndex);
		return;
	}
	
	const auto& costItem = GsMonsterCollection()->BookExpCostSet()[InIndex];
	const FGsCostElement* costElement = costItem.GetFirstCostElement();
	if (nullptr == costElement)
	{
		GSLOG(Error, TEXT("nullptr == costElement"));
		return;
	}

	ItemDBId itemDBId = INVALID_ITEM_DB_ID;
	if (costElement->PlayerHasAmountMax())
	{
		const auto& findSet = GItem()->FindByTID(costElement->GetItemId(), ItemType::INGREDIENT);
		if (0 < findSet.Num() && findSet[0].IsValid())
		{
			itemDBId = findSet[0].Pin().Get()->GetDBID();
		}
	}


	// 2023/2/2 PKT - 선택한 것이 있는데 아이템 DB가 없으면 중간에 사라진것. 초기화 해준다.
	if (_selectedIndex == InIndex && itemDBId == INVALID_ITEM_DB_ID)
	{
		_selectedIndex = UNSELECTED_INDEX;
		InvalidateAmount(EMPTY_AMOUNT);
	}

	if (INVALID_ITEM_DB_ID != itemDBId)
	{
		itemIcon->SetOwnedItem(itemDBId);
		itemIcon->SetDimmedImage(false);
		itemIcon->SetItemName(FText::GetEmpty());
		itemIcon->SetRedDot(false);

		int32 countAmount = StaticCast<int32>(costElement->PlayerHasAmountMax());
		if (_selectedIndex == InIndex)
		{
			countAmount = FMath::Max(StaticCast<int32>(costElement->PlayerHasAmountMax()) - _amountUsed, 0);
		}		
		itemIcon->SetItemCountEX(countAmount);
	}
	else
	{
		itemIcon->SetNotOwnItem(costElement->GetItemId(), 0);
		itemIcon->SetDimmedImage(true);
		itemIcon->SetItemName(FText::GetEmpty());
	}

	itemIcon->SetSlotSelectionImage(_selectedIndex == InIndex);
}

void UGsUIPopupMonsterBookExpItem::OnClickedReset()
{
	int32 prevAmountUsed = _amountUsed;
	InvalidateAmount(DEFAULT_MIN_AMOUNT);
	if (prevAmountUsed != _amountUsed)
	{
		OnBtnClickedChangedAmount();
	}
}

void UGsUIPopupMonsterBookExpItem::OnClickedMax()
{
	int32 prevAmountUsed = _amountUsed;
	if (GsMonsterCollection()->BookExpCostSet().IsValidIndex(_selectedIndex))
	{
		const auto& costItem = GsMonsterCollection()->BookExpCostSet()[_selectedIndex];
		const FGsCostElement* costElement = costItem.GetFirstCostElement();
		if (nullptr == costElement)
		{
			GSLOG(Error, TEXT("nullptr == costElement"));
			return;
		}

		InvalidateAmount(StaticCast<int32>(costElement->PlayerHasAmountMax()));
	}
	if (prevAmountUsed != _amountUsed)
	{
		OnBtnClickedChangedAmount();
	}	
}

void UGsUIPopupMonsterBookExpItem::OnClickedDecrease()
{
	int32 prevAmountUsed = _amountUsed;
	
	int32 newAmount = FMath::Max(_amountUsed - 1, DEFAULT_MIN_AMOUNT);
	InvalidateAmount(newAmount);

	if (prevAmountUsed != _amountUsed)
	{
		OnBtnClickedChangedAmount();
	}	
}

void UGsUIPopupMonsterBookExpItem::OnClickedIncrease()
{
	int32 prevAmountUsed = _amountUsed;
	if (GsMonsterCollection()->BookExpCostSet().IsValidIndex(_selectedIndex))
	{
		const auto& costItem = GsMonsterCollection()->BookExpCostSet()[_selectedIndex];
		const FGsCostElement* costElement = costItem.GetFirstCostElement();
		if (nullptr == costElement)
		{
			GSLOG(Error, TEXT("nullptr == costElement"));
			return;
		}

		int32 newAmount = FMath::Min(StaticCast<int32>(costElement->PlayerHasAmountMax()), _amountUsed + 1);
		InvalidateAmount(newAmount);
	}
	if (prevAmountUsed != _amountUsed)
	{
		OnBtnClickedChangedAmount();
	}
}

void UGsUIPopupMonsterBookExpItem::OnClickedLargeDecrease()
{
	int32 prevAmountUsed = _amountUsed;
	
	int32 newAmount = FMath::Max(_amountUsed - LAGRAGE_AMOUNT, DEFAULT_MIN_AMOUNT);
	InvalidateAmount(newAmount);
	
	if (prevAmountUsed != _amountUsed)
	{
		OnBtnClickedChangedAmount();
	}
}

void UGsUIPopupMonsterBookExpItem::OnClickedLargeIncrease()
{
	int32 prevAmountUsed = _amountUsed;

	if (GsMonsterCollection()->BookExpCostSet().IsValidIndex(_selectedIndex))
	{
		const auto& costItem = GsMonsterCollection()->BookExpCostSet()[_selectedIndex];
		const FGsCostElement* costElement = costItem.GetFirstCostElement();
		if (nullptr == costElement)
		{
			GSLOG(Error, TEXT("nullptr == costElement"));
			return;
		}

		int32 newAmount = FMath::Min(StaticCast<int32>(costElement->PlayerHasAmountMax()), _amountUsed + LAGRAGE_AMOUNT);
		InvalidateAmount(newAmount);
	}

	if (prevAmountUsed != _amountUsed)
	{
		OnBtnClickedChangedAmount();
	}
}

void UGsUIPopupMonsterBookExpItem::OnSelectedItemIcon(UGsUIIconItem& InIcon)
{
	if (nullptr == InIcon.GetItemIconData())
	{
		GSLOG(Error, TEXT("nullptr == InIcon.GetItemIconData()"));
		return;
	}

	if (INVALID_ITEM_DB_ID == InIcon.GetItemIconData()->GetDBID())
	{
		FText ticker;
		FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("UI_POPUP_USEITEM_EMPTY"), ticker);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, ticker);
		return;
	}

	int32 selectedIndex = UNSELECTED_INDEX;
	for (int32 idx = 0; idx < GsMonsterCollection()->BookExpCostSet().Num(); ++idx)
	{
		const auto& costItem = GsMonsterCollection()->BookExpCostSet()[idx];
		const FGsCostElement* costElement = costItem.GetFirstCostElement();
		if (nullptr == costElement)
		{
			GSLOG(Error, TEXT("nullptr == costElement"));
			continue;
		}

		if (InIcon.GetTableId() == costElement->GetItemId())
		{
			selectedIndex = idx;
			break;
		}
	}

	if (UNSELECTED_INDEX >= selectedIndex || selectedIndex == _selectedIndex)
	{
		return;
	}

	SetSelectedItem(selectedIndex);
}

void UGsUIPopupMonsterBookExpItem::OnLongPressedItemIcon(UGsUIIconItem& InIcon)
{
	FGsItemDetailOpenMessageParam param(InIcon.GetItemIconData(), true, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
}

void UGsUIPopupMonsterBookExpItem::SetSelectedItem(int32 InIndex)
{
	if (false == GsMonsterCollection()->BookExpCostSet().IsValidIndex(InIndex))
	{
		_selectedIndex = UNSELECTED_INDEX;
		InvalidateAmount(EMPTY_AMOUNT);
	}
	else
	{
		_selectedIndex = InIndex;
		InvalidateAmount(DEFAULT_MIN_AMOUNT);

		OnBtnClickedChangedAmount();
	}

	int32 maxCount = GsMonsterCollection()->BookExpCostSet().Num();
	_iconSlotHelper->RefreshAll(maxCount);
}

void UGsUIPopupMonsterBookExpItem::InvalidateAmount(int32 InAmountUsed)
{
	_amountUsed = InAmountUsed;
	_textCurrentAnount->SetText(FText::AsNumber(_amountUsed));

	if (UNSELECTED_INDEX < _selectedIndex)
	{
		_iconSlotHelper->RefreshByIndex(_selectedIndex);
	}	
}

void UGsUIPopupMonsterBookExpItem::Invalidate()
{
	int32 maxCount = GsMonsterCollection()->BookExpCostSet().Num();
	_iconSlotHelper->RefreshAll(maxCount);

	if (UNSELECTED_INDEX >= _selectedIndex)
	{
		for (int32 idx = 0; idx < GsMonsterCollection()->BookExpCostSet().Num(); ++idx)
		{			
			const auto& costItem = GsMonsterCollection()->BookExpCostSet()[idx];
			const FGsCostElement* costElement = costItem.GetFirstCostElement();
			if (nullptr == costElement)
			{
				GSLOG(Error, TEXT("nullptr == costElement"));
				continue;
			}

			if (costElement->PlayerHasAmountMax())
			{
				const auto& findSet = GItem()->FindByTID(costElement->GetItemId(), ItemType::INGREDIENT);
				if (0 < findSet.Num() && findSet[0].IsValid())
				{
					SetSelectedItem(idx);
					break;
				}
			}
		}
	}
	
	InvalidateAmount(_amountUsed);

	_deemedCalc->SetVisibility(UNSELECTED_INDEX >= _selectedIndex ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}