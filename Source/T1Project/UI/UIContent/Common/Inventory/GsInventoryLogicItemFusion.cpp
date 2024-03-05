#include "GsInventoryLogicItemFusion.h"
#include "../Item/GsItem.h"
#include "../Item/GsItemManager.h"
#include "../Management/ScopeGame/GsInventoryManager.h"
#include "../Message/GsMessageContents.h"
#include "../Message/MessageParam/GsItemContentsMessageParam.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/Fusion/Handler/GsItemFusionHandler.h"
#include "../Icon/GsUIIconItemInventory.h"
#include "../../Popup/GsUIPopupItemDetailMain.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"


void FGsInventoryLogicItemFusion::Clear()
{
	Super::Clear();

	// 2023/9/27 PKT - Handler 초기화
	_fusionHandler = nullptr;

	_itemDataList.Empty();
}

void FGsInventoryLogicItemFusion::OnInvalidateList()
{
	if (nullptr == _fusionHandler)
	{
		return;
	}

	_itemDataList.Empty();

	for (const ItemDBId itemDBId : GSInventory()->GetDisplayItemList(_currTabType))
	{
		TWeakPtr<FGsItem> item = GItem()->FindItem(itemDBId, ItemType::EQUIP);
		if (false == item.IsValid())
		{
			continue;
		}

		const auto result = _fusionHandler->IsTargetItemSuitable(item);
		if (FGsItemFusionHandler::ECheckValid::Valid == result.Key				// 2023/10/11 PKT - 그냥 가능 한 경우
			|| FGsItemFusionHandler::ECheckValid::EquipInvalid == result.Key	// 2023/10/11 PKT - 장착 중인 애들도 표기는 해준다.
			|| FGsItemFusionHandler::ECheckValid::PresetInvalid == result.Key	// 2023/10/11 PKT - 프리셋 애들도 표기는 해준다.
			|| FGsItemFusionHandler::ECheckValid::LockInvalid == result.Key		// 2023/10/11 PKT - 잠김 상태 애들도 표기는 해준다.
			)
		{
			if (item.IsValid())
			{
				_itemDataList.Emplace(item.Pin().Get());
			}
		}
	}
}

void FGsInventoryLogicItemFusion::OnCreateItemIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;
}

void FGsInventoryLogicItemFusion::OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	if (nullptr == _fusionHandler)
	{
		return;
	}

	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;

	if (false == _itemDataList.IsValidIndex(InIndex))
	{
		itemIcon->SetEmptyIcon();
	}
	else
	{
		itemIcon->SetOwnedItem(_itemDataList[InIndex]->GetDBID());
		itemIcon->SetActiveIconBg(true);
		itemIcon->SetItemNameVisibility(false);
		itemIcon->SetItemNameToolTipFlag(true);
		itemIcon->SetItemNameToolTipHoverMode(true);
		itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
		// 2023/10/10 PKT - src Item or Dest Item
		bool isShowSelectedImage = (_fusionHandler->GetSrcItemDBId() == _itemDataList[InIndex]->GetDBID() || _fusionHandler->GetDestItemDBId() == _itemDataList[InIndex]->GetDBID());
		itemIcon->SetSlotSelectionImage(isShowSelectedImage);
		itemIcon->SetItemCollectionMarking(itemIcon->GetItemIconData(), true);
		
		if (INVALID_ITEM_DB_ID != _fusionHandler->GetSrcItemDBId() && false == _fusionHandler->IsSuitableForDestItem(_itemDataList[InIndex]->GetDBID()))
		{
			itemIcon->SetDimmedImage(true);
		}
		else
		{
			itemIcon->SetDimmedImage(false);
		}

		if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			itemIcon->OnFinishedLongPressEvent.AddRaw(this, &FGsInventoryLogicItemFusion::OnLongPressIcon);
		}
	}
}

void FGsInventoryLogicItemFusion::OnSelectItemIcon(UGsUIIconItem* InPrevIcon, UGsUIIconItem* InCurrIcon)
{
	// 이전 아이콘 취소 표시
	//if (InPrevIcon)
	//{
	//	if (UGsUIIconItemInventory* prevIcon = Cast<UGsUIIconItemInventory>(InPrevIcon))
	//	{
	//		prevIcon->SetSlotSelectionImage(false);
	//	}
	//}

	//// 현재 아이콘 선택 표시
	//if (InCurrIcon)
	//{
	//	if (FGsItem* item = InCurrIcon->GetItemIconData())
	//	{
	//		if (UGsUIIconItemInventory* currIcon = Cast<UGsUIIconItemInventory>(InCurrIcon))
	//		{
	//			currIcon->SetSlotSelectionImage(true);
	//		}

	//		SetSelectedItemDBId(item->GetDBID());
	//	}
	//	else
	//	{
	//		SetSelectedItemDBId(INVALID_ITEM_DB_ID);
	//	}
	//}
}

int32 FGsInventoryLogicItemFusion::GetDataCount() const
{
	return _itemDataList.Num();
}

// 해당 ItemDBId의 인덱스 얻기
int32 FGsInventoryLogicItemFusion::GetDataIndex(ItemDBId InItemDBId) const
{
	return _itemDataList.IndexOfByPredicate([InItemDBId](const FGsItem* InData)
		{
			return InData->GetDBID() == InItemDBId;
		}
	);
}

void FGsInventoryLogicItemFusion::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
		return;

	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideButtons = true;
	option._bHideBG = false;
	option._bHideButtonsBottom = false;
	option._bUseLock = true;
	option._bUseCollectionMarking = true;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), UGsUIPopupItemDetailMain::CenterPosType::RightPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}

void FGsInventoryLogicItemFusion::SetData(const FGsItemFusionHandler* InFusionHandler)
{
	Clear();

	_fusionHandler = InFusionHandler;

	_currTabType = EInventoryTab::EquipmentTab;

	// 2023/10/11 PKT - 합성은 아이템 디테일 팝업창을 통해 호출 되는데 이때 팝업창을 닫아줘야 한다.
	FGsItemDetailCloseMessageParam param;
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, &param);
}