// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Common/Inventory/GsInventoryLogicEnchantInven.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Item/GsItem.h"
#include "Item/GsItemIngredient.h"
#include "Item/GsItemManager.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Management/GsScopeHolder.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"


void FGsInventoryLogicEnchantInven::Clear()
{
	_dataList.Empty();
	_selectedItemDbidList.Empty();
	_ingredientEffectType = ItemEffectType::NONE;

	Super::Clear();
}

void FGsInventoryLogicEnchantInven::OnInvalidateList()
{
	_dataList.Empty();

	GetItemList(_currTabType, _dataList);
}

void FGsInventoryLogicEnchantInven::OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;

	if (_dataList.IsValidIndex(InIndex))
	{
		FGsItem* itemData = _dataList[InIndex];
		if (nullptr == itemData)
			return;

		if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			itemIcon->OnFinishedLongPressEvent.AddRaw(this, &FGsInventoryLogicEnchantInven::OnLongPressIcon);
		}

		itemIcon->SetOwnedItem(itemData->GetDBID());
		itemIcon->SetActiveIconBg(true);
		itemIcon->SetItemNameVisibility(false);
		itemIcon->SetItemNameToolTipFlag(true);
		itemIcon->SetItemNameToolTipHoverMode(true);
		itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
		itemIcon->SetItemCollectionMarking(itemIcon->GetItemIconData(), true);

		if (nullptr != itemIcon->GetItemIconData())
		{
			itemIcon->SetCoolTimeVisibility(itemIcon->GetItemIconData()->IsEquipment());
		}

		// 선택된 아이콘 처리
		bool isSelected = INDEX_NONE != _selectedItemDbidList.Find(itemData->GetDBID());
		itemIcon->SetSlotSelectionImage(isSelected);

		return;
	}

	itemIcon->SetEmptyIcon();
}

void FGsInventoryLogicEnchantInven::OnLongPressIcon(class UGsUIIconItem& InIcon)
{
	// 튜토리얼에선 롱프레스 막음
	if (IsTutorial())
	{
		return;
	}

	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideButtons = true;
	option._bHideBG = false;
	option._bUseCollectionMarking = true;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), FGsItemDetailOpenMessageParamEX::PosDir::RightPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}

void FGsInventoryLogicEnchantInven::OnSelectItemIcon(UGsUIIconItem* InPrevIcon, UGsUIIconItem* InCurrIcon)
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

int32 FGsInventoryLogicEnchantInven::GetDataIndex(ItemDBId InItemDBId) const
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

void FGsInventoryLogicEnchantInven::GetItemList(EInventoryTab InTabType, OUT TArray<FGsItem*>& OutList)
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
		TArray<TWeakPtr<FGsItem>> consumList = itemMgr->FindItem(ItemType::INGREDIENT);
		for (TWeakPtr<FGsItem> iter : consumList)
		{
			if(false == iter.IsValid())
				continue;

			if (true == iter.Pin()->GetLock() || (false == iter.Pin()->IsEnchantScrollItem() && false == iter.Pin()->IsRefineOptionStoneItem()))
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
	case EInventoryTab::MaterialTab:
	{
		TArray<TWeakPtr<FGsItem>> ingredientItems = itemMgr->FindItem(ItemType::INGREDIENT, ItemCategoryMain::SCROLL);
		for (TWeakPtr<FGsItem> iter : ingredientItems)
		{
			if(false == iter.IsValid())
				continue;
			
			FGsItemIngredient* ingredient = iter.Pin()->GetCastItem<FGsItemIngredient>();
			if(nullptr == ingredient)
				continue;

			switch (_ingredientEffectType)
			{
			case ItemEffectType::ITEM_ENCHANT:
			{
				if (ingredient->GetEffectType() != ItemEffectType::ITEM_ENCHANT)
				{
					continue;
				}
			}
				break;
			case ItemEffectType::OPTION_STONE:
			{
				if (ingredient->GetEffectType() != ItemEffectType::OPTION_STONE && ingredient->GetEffectType() != ItemEffectType::BLESSED_OPTION_STONE)
				{
					continue;
				}
			}
				break;
			case ItemEffectType::MAGICAL_FORGE:
			{
				if (ingredient->GetEffectType() != ItemEffectType::MAGICAL_FORGE)
				{
					continue;
				}
			}
				break;
			default:
				break;
			}

			OutList.Emplace(iter.Pin().Get());
		}	
	}
	break;
	}	

	itemMgr->SortDefaultType(OutList);

	// 주의: 강제로 첫 슬롯으로 세팅하므로 Sorting 다음에 와야함
	if (IsTutorial())
	{
		int32 targetIndex = -1;
		for (int i = 0, maxCount = OutList.Num(); i < maxCount; ++i)
		{
			if (_tutorialItemDBId == OutList[i]->GetDBID())
			{
				targetIndex = i;
				break;
			}
		}

		// 강제로 첫 슬롯으로 설정
		if (0 < targetIndex)
		{
			FGsItem* targetData = OutList[targetIndex];
			OutList.RemoveAt(targetIndex);
			OutList.EmplaceAt(0, targetData);
		}
	}
}

void FGsInventoryLogicEnchantInven::SetTutorial(ItemDBId InItemDBId)
{
	_tutorialItemDBId = InItemDBId;
}

void FGsInventoryLogicEnchantInven::SetSeletedItemDbidList(TArray<ItemDBId> InItemDbidList)
{
	_selectedItemDbidList.Empty();
	_selectedItemDbidList = MoveTemp(InItemDbidList);
}