
#include "GsInventoryLogicItemCollection.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Item/Collection/GsItemCollectionBase.h"
#include "Item/Collection/GsItemCollectionSlotState.h"
#include "Management/GsScopeHolder.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"



void FGsInventoryLogicItemCollection::Clear()
{
	// 2021/08/09 PKT - data reset
	_dataList.Empty();

	_materialSet.Empty();
	
	Super::Clear();
}

void FGsInventoryLogicItemCollection::Tick(int32 InDeltaSeconds)
{
	Super::Tick(InDeltaSeconds);

}

void FGsInventoryLogicItemCollection::InvalidateList(const TArray<TPair<EItemCollectionSlotState, FGsItemCollectionSlotState::MaterialData>>& InMaterialSet)
{
	_materialSet = InMaterialSet;

	// 2021/10/12 PKT - InventoryItemCollection으로 메세지를 보낸다.
	FGsMessageHolder* holder = GMessage();
	if (holder)
	{
		// 2021/10/12 PKT - InventoryCommon에 갱신 하도록 이벤트 전달.
		holder->GetInventoryCommon().SendMessage(MessageContentCommonInven::INVALIDATE_ALL, nullptr);
	}
}

//
//void FGsInventoryLogicItemCollection::InvalidateList(const TArray<ItemDBId>& InImmediateItemIds, const TArray<ItemDBId>& InConditionalItemIds)
//{
//	_immediateItemIds = InImmediateItemIds;
//	_conditionalItemIds = InConditionalItemIds;
//
//	// 2021/10/12 PKT - InventoryItemCollection으로 메세지를 보낸다.
//	FGsMessageHolder* holder = GMessage();
//	if (holder)
//	{	
//		// 2021/10/12 PKT - InventoryCommon에 갱신 하도록 이벤트 전달.
//		holder->GetInventoryCommon().SendMessage(MessageContentCommonInven::INVALIDATE_ALL, nullptr);
//	}
//}


void FGsInventoryLogicItemCollection::InvalidateSelected(ItemDBId InItemDBId)
{
	FGsMessageHolder* holder = GMessage();
	if (holder)
	{
		// 2021/10/12 PKT - InventoryCommon에 갱신 하도록 이벤트 전달.
		const FGsInventoryCommonInvalidate param(InItemDBId);
		holder->GetInventoryCommon().SendMessage(MessageContentCommonInven::INVALIDATE_SELECTED_EFFECT, &param);
	}
} 


void FGsInventoryLogicItemCollection::OnInvalidateList()
{
	// 2021/08/09 PKT - data reset
	_dataList.Empty();

	GetItemList(_currTabType, _materialSet);
}


void FGsInventoryLogicItemCollection::OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	TWeakObjectPtr<UGsUIIconItemCollectionSlot> item = Cast<UGsUIIconItemCollectionSlot>(InIcon);
	if (false == item.IsValid())
	{
		GSLOG(Error, TEXT("nullptr == ItemIcon"));
		return;
	}

	if (false == _dataList.IsValidIndex(InIndex))
	{
		item->SetEmptyIcon();
		return;
	}
	
	// 2021/10/12 PKT - Icon 셋팅
	item->SetOwnedItem(_dataList[InIndex]->GetDBID(), UGsUIIconItem::Storage_All);
	// 2021/10/12 PKT - Focus된 아이템에 대한 표기
	item->SetSlotSelectionImage(_dataList[InIndex]->GetDBID() == _selectedItemDBId);
	item->SetItemNameToolTipFlag(true);
	item->SetItemNameToolTipHoverMode(true);

	// LSJ 추가
	/*TPair<int32, int32> collectCount = GetCollectCountAndIndexByItemId(_dataList[InIndex]->GetDBID(), _dataList[InIndex]->GetTID());
	bool isActiveShiftAbleBtn = (1 < collectCount.Value) ? true : false;
	item->SetIsActiveShiftAbleBtn(isActiveShiftAbleBtn);*/
}


void FGsInventoryLogicItemCollection::OnSelectItemIcon(UGsUIIconItem* InPrevIcon, UGsUIIconItem* InCurrIcon)
{
	// 이전 아이콘 취소 표시
	if (InPrevIcon)
	{
		if (UGsUIIconItemCollectionSlot* prevIcon = Cast<UGsUIIconItemCollectionSlot>(InPrevIcon))
		{
			prevIcon->SetSlotSelectionImage(false);
			// 2021/10/12 PKT - 일단 셀렉트 된 DBId 초기화
			SetSelectedItemDBId(INVALID_ITEM_DB_ID);
		}
	}

	// 현재 아이콘 선택 표시
	if (InCurrIcon)
	{
		if (FGsItem* item = InCurrIcon->GetItemIconData())
		{
			if (UGsUIIconItemCollectionSlot* currIcon = Cast<UGsUIIconItemCollectionSlot>(InCurrIcon))
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


int32 FGsInventoryLogicItemCollection::GetDataCount() const
{
	return _dataList.Num();
}

int32 FGsInventoryLogicItemCollection::GetDataIndex(ItemDBId InItemDBId) const 
{
	return _dataList.IndexOfByPredicate([InItemDBId](const FGsItem* InItem)
		{
			return (nullptr != InItem && InItem->GetDBID() == InItemDBId);
		}
	);
}

const FGsItem* FGsInventoryLogicItemCollection::GetItemByIndex(int32 InIndex) const
{
	return _dataList.IsValidIndex(InIndex) ? _dataList[InIndex] : nullptr;
}

const FGsItem* FGsInventoryLogicItemCollection::GetItemByDBId(ItemDBId InItemDBId) const
{
	if (INVALID_ITEM_DB_ID == InItemDBId)
	{
		return nullptr;
	}

	int32 findIndex = _dataList.IndexOfByPredicate([InItemDBId](const FGsItem* InItem)
													{
														return (nullptr != InItem && InItem->GetDBID() == InItemDBId);
													});

	return (INDEX_NONE != findIndex) ? _dataList[findIndex] : nullptr;
}

void FGsInventoryLogicItemCollection::FindItemSet(const TArray<TPair<EItemCollectionSlotState, FGsItemCollectionSlotState::MaterialData>>& InMaterialSet
	, TArray<ItemDBId>* OutFindItemSet) const
{
	(*OutFindItemSet).Empty();
	// 2022/06/30 PKT - MaterialSet 순서대로 아이템을 찾는다.
	for (const TPair<EItemCollectionSlotState, FGsItemCollectionSlotState::MaterialData>& materialData : InMaterialSet)
	{
		const FGsItemCollectionSlotState::MaterialData& data = materialData.Value;

		for (int32 idx = 0; idx < _dataList.Num(); ++idx)
		{
			if (data.Key == _dataList[idx]->GetTID() && data.Value == _dataList[idx]->GetLevel())
			{
				ItemDBId findItemDBId = _dataList[idx]->GetDBID();
				if (nullptr == (*OutFindItemSet).FindByPredicate([findItemDBId](const ItemDBId InItemID) 
					{
						return InItemID == findItemDBId;
					}
				))
				{
					(*OutFindItemSet).Add(_dataList[idx]->GetDBID());
				}				
			}
		}
	}
}

void FGsInventoryLogicItemCollection::GetItemList(EInventoryTab InTabType
												, const TArray<TPair<EItemCollectionSlotState, FGsItemCollectionSlotState::MaterialData>>& InMaterialSet)
{
	const static TMap<EInventoryTab, ItemType> ConvertInventoryTabToItemType =
	{
		{EInventoryTab::AllTab, ItemType::MAX}
		, {EInventoryTab::EquipmentTab, ItemType::EQUIP}
		, {EInventoryTab::ConsumableTab, ItemType::INGREDIENT}
		, {EInventoryTab::MaterialTab, ItemType::CONSUME}
	};

	if (false == ConvertInventoryTabToItemType.Contains(InTabType))
	{
		GSLOG(Error, TEXT("false == ConvertInventoryTabToItemType.Contains(InTabType)"));
		return;
	}

	ItemType itemType = ConvertInventoryTabToItemType[InTabType];

	UGsItemManager* itemManager = GItem();
	FGsDepotManager* depotManager = GSDepot();
	if (nullptr == itemManager || nullptr == depotManager)
	{
		GSLOG(Warning, TEXT("nullptr == itemManager or nullptr == depotManager"));
		return;
	}

	TArray<const FGsItem*> immediateItemIdSet;
	TArray<const FGsItem*> conditionalItemIdSet;

	for (const auto& materialPair : InMaterialSet)
	{
		if (EItemCollectionSlotState::ImmediatePossible == materialPair.Key)
		{
			const FGsItemCollectionSlotState::MaterialData& material = materialPair.Value;

			// 2022/03/07 PKT - Inventory Check
			auto findItemSet = itemManager->FindByTID(material.Key, itemType);
			for (const TWeakPtr<FGsItem>& item : findItemSet)
			{
				if (item.IsValid() && item.Pin().Get() && item.Pin().Get()->GetLevel() == material.Value)
				{
					const FGsItem* itemData = item.Pin().Get();
					if (false == itemData->IsEquipped() && false == itemData->GetLock() && false == itemData->IsPresetEquipped())
					{
						immediateItemIdSet.Emplace(itemData);
					}
				}
			}

			// 2022/03/07 PKT - Depot Check
			const TMap<ItemType, TMap<uint64, TSharedPtr<FGsItem>>>& depotItemBuffers = depotManager->GetItemBuffers();
			for (const auto& bufferPair : depotItemBuffers)
			{
				const TMap <uint64, TSharedPtr<FGsItem>>& itemMap = bufferPair.Value;
				for (const auto& itemPair : itemMap)
				{
					if (itemPair.Value.IsValid() && nullptr != itemPair.Value.Get())
					{
						if (ItemType::MAX != itemType && itemPair.Value.Get()->GetType() != itemType)
						{
							continue;
						}

						if (itemPair.Value.Get()->GetTID() == material.Key && itemPair.Value.Get()->GetLevel() == material.Value)
						{
							const FGsItem* itemData = itemPair.Value.Get();
							if (false == itemData->IsEquipped() && false == itemData->GetLock() && false == itemData->IsPresetEquipped())
							{
								immediateItemIdSet.Emplace(itemData);
							}							
						}
					}
				}
			}
			
		}
		else if (EItemCollectionSlotState::ConditionalPossible == materialPair.Key)
		{
			const FGsItemCollectionSlotState::MaterialData& material = materialPair.Value;
			auto findItemSet = itemManager->FindByTID(material.Key, itemType);

			for (const TWeakPtr<FGsItem>& item : findItemSet)
			{
				if (item.IsValid() && item.Pin().Get())
				{
					const FGsItem* itemData = item.Pin().Get();
					if (false == itemData->IsEquipped() && false == itemData->GetLock() && false == itemData->IsPresetEquipped())
					{
						conditionalItemIdSet.Emplace(itemData);
					}					
				}
			}
		}
	}

	for (const FGsItem* item : immediateItemIdSet)
	{
		if (nullptr == _dataList.FindByPredicate([item](const FGsItem* InData)
			{
				return (InData->GetDBID() == item->GetDBID());
			}
		))
		{
			_dataList.Emplace(item);
		}
	}

	for (const FGsItem* item : conditionalItemIdSet)
	{
		if (nullptr == _dataList.FindByPredicate([item](const FGsItem* InData)
			{
				return (InData->GetDBID() == item->GetDBID());
			}
		))
		{
			_dataList.Emplace(item);
		}
	}
}