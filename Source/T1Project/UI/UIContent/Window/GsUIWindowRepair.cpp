#include "GsUIWindowRepair.h"

#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/WrapBox.h"

#include "DataSchema/UI/GsSchemaItemCategoryDisPlayName.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Item/Data/GsItemDeliveryBoxData.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UI/UIContent/Window/Repair/GsUIRepairListEntry.h"

#include "UI/UIContent/Common/GsUITabButtonCommon.h"
#include "UI/UIContent/Common/Item/GsUIItemDefaultInfo.h"
#include "UI/UIContent/Common/Item/GsUIItemDetailInfo.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"

#include "UTIL/GsTimeSyncUtil.h"

#include "Management/ScopeGlobal/GsUIManager.h"

#include "Net/GsNetSendServiceItem.h"


void UGsUIWindowRepair::BeginDestroy()
{
	if (nullptr != _btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIWindowRepair::OnClickCloseAllStack);
	}
	_btnClose = nullptr;

	if (nullptr != _btnBack)
	{
		_btnBack->OnClicked.RemoveDynamic(this, &UGsUIWindowRepair::OnClickBack);
	}
	_btnBack = nullptr;

	if (nullptr != _btnRepair)
	{
		_btnRepair->OnClicked.RemoveDynamic(this, &UGsUIWindowRepair::OnClickRepair);
	}
	_btnRepair = nullptr;

	if (nullptr != _slotHelperCategoryTab)
	{
		_slotHelperCategoryTab->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowRepair::OnRefreshCategoryTab);
	}
	_slotHelperCategoryTab = nullptr;

	if (nullptr != _slotHelperItem)
	{
		_slotHelperItem->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowRepair::OnRefreshItem);
	}

	Super::BeginDestroy();
}

void UGsUIWindowRepair::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowRepair::OnClickCloseAllStack);
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowRepair::OnClickBack);
	_btnRepair->OnClicked.AddDynamic(this, &UGsUIWindowRepair::OnClickRepair);

	_toggleGroupCategoryTab.OnSelectChanged.BindUObject(this, &UGsUIWindowRepair::OnSelectChangedCategoryTab);

	_slotHelperCategoryTab = NewObject<UGsDynamicPanelSlotHelper>(this);
	if (_slotHelperCategoryTab)
	{
		_slotHelperCategoryTab->Initialize(_subClassOfCategoryTab, _scrollBoxCategoryTab);
		_slotHelperCategoryTab->OnRefreshEntry.AddDynamic(this, &UGsUIWindowRepair::OnRefreshCategoryTab);
	}

	_slotHelperItem = NewObject<UGsDynamicPanelSlotHelper>(this);
	if (_slotHelperItem)
	{
		_slotHelperItem->Initialize(_subClassOfItem, _panelItemList);
		_slotHelperItem->OnRefreshEntry.AddDynamic(this, &UGsUIWindowRepair::OnRefreshItem);
	}
}

void UGsUIWindowRepair::NativeConstruct()
{
	Super::NativeConstruct();

	FGsItemDeliveryBoxData& deliveryBoxData = GItem()->GetItemDeliveryBoxData();

	// 기간 텍스트 설정
	FDateTime startTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(deliveryBoxData.GetStartTime());
	FDateTime endTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(deliveryBoxData.GetEndTime());

	FString beginDateText;
	FString endDateText;
	FGsTimeStringHelper::GetTimeStringNYMDHM(startTime, beginDateText);
	FGsTimeStringHelper::GetTimeStringNYMDHM(endTime, endDateText);
	FText dateText = FText::FromString(FString::Format(TEXT("{0} ~ {1}"), { *beginDateText, *endDateText }));

	FText noticeText;
	FText::FindText(DELIVERY_BOX_TEXT, TEXT("DeliveryBoxText_Equip_Notice"), noticeText);
	noticeText = FText::Format(noticeText, dateText);
	_textBlockNotice->SetText(noticeText);

	// 데이터 초기화
	InitializeCategoryTab(deliveryBoxData.GetRepairableItemList());

	_toggleGroupCategoryTab.SetSelectedIndex(0, true);

}

void UGsUIWindowRepair::NativeDestruct()
{
	_toggleGroupCategoryTab.Clear();

	_repairableWeaponList.Empty();
	_repairableItemListMap.Empty();
	_categoryList.Empty();
	_selectedItemSlot.Reset();

	Super::NativeDestruct();
}

bool UGsUIWindowRepair::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsRepair))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REPAIR);
		return true;
	}

	return false;
}

void UGsUIWindowRepair::OnRefreshCategoryTab(int32 InIndex, UWidget* InEntry)
{
	FText categoryName;
	bool existWeaponCategory = (_repairableWeaponList.Num() > 0);
	if (existWeaponCategory && InIndex == 0)
	{
		FText::FindText(TEXT("ItemUIText"), TEXT("Category_Weapon"), categoryName);
	}
	else
	{
		int32 index = existWeaponCategory ? InIndex - 1 : InIndex;
		ItemCategorySub categorySub = _categoryList[index];

		const UGsTableItemCategoryDisPlayName* table = Cast<UGsTableItemCategoryDisPlayName>(FGsSchemaItemCategoryDisPlayName::GetStaticTable());
		if (nullptr == table)
		{
			return;
		}

		const FGsSchemaItemCategoryDisPlayName* row = nullptr;
		if (false == table->FindRowById(categorySub, row))
		{
			return;
		}

		categoryName = row->displayName;
	}
	
	UGsUITabButtonCommon* entry = Cast<UGsUITabButtonCommon>(InEntry);
	if (nullptr == entry)
		return;

	entry->SetTitleText(categoryName);
	_toggleGroupCategoryTab.AddToggleWidget(entry->GetToggleGroupEntry());
}

void UGsUIWindowRepair::OnRefreshItem(int32 InIndex, UWidget* InEntry)
{
	if (UGsUIRepairListEntry* slot = Cast<UGsUIRepairListEntry>(InEntry))
	{
		int32 categoryIndex = _toggleGroupCategoryTab.GetSelectedIndex();
		bool existWeaponCategory = (_repairableWeaponList.Num() > 0);
		if (existWeaponCategory && categoryIndex == 0)
		{
			if (_repairableWeaponList.IsValidIndex(InIndex))
			{
				slot->SetData(_repairableWeaponList[InIndex].itemTableData
							, _repairableWeaponList[InIndex].itemLevel
							, _repairableWeaponList[InIndex].enchantBonusIndex
							, _repairableWeaponList[InIndex].itemDBId);
			}
		}
		else
		{
			categoryIndex = existWeaponCategory ? categoryIndex - 1 : categoryIndex;
			ItemCategorySub categorySub = _categoryList[categoryIndex];
			TArray<RepairItemData>& itemList = _repairableItemListMap[categorySub];
			slot->SetData(itemList[InIndex].itemTableData
						, itemList[InIndex].itemLevel
						, itemList[InIndex].enchantBonusIndex
						, itemList[InIndex].itemDBId);
		}

		slot->OnClickedItemSlot.BindUObject(this, &UGsUIWindowRepair::OnClickedItemSlot);
	}
}

void UGsUIWindowRepair::OnClickRepair()
{
	if (_selectedItemSlot.IsValid())
	{
		// 인벤토리 슬롯 확인
		if (GItem()->IsInvenSlotFull())
		{
			FText fullText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), fullText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, fullText);
			return;
		}

		// 확인 팝업 오픈
		FText alertText;
		FText::FindText(DELIVERY_BOX_TEXT, TEXT("DeliveryBoxText_Popup_Equip_Alert"), alertText);

		FGsItemDeliveryBoxData& deliveryBoxData = GItem()->GetItemDeliveryBoxData();
		const FGsItem* itemData = _selectedItemSlot->GetItemData();
		if (nullptr == itemData)
			return;

		ItemDBId usedBoxItemDBId = deliveryBoxData.GetUsedBoxItemDBId();
		ItemDBId recoverItemDBId = itemData->GetDBID();

		FGsUIHelper::PopupYesNo(alertText, [usedBoxItemDBId, recoverItemDBId](bool bInYes)
			{
				if (bInYes)
				{
					if (GItem()->TryUseItem(usedBoxItemDBId, 1, true))
					{
						FGsNetSendServiceItem::SendItemUseDeliveryBoxEquip(usedBoxItemDBId, recoverItemDBId);
					}
					else
					{
						TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("WindowRepair"));
						if (widget.IsValid())
						{
							widget->Close();
						}
					}
 				}
			});
	}
}

void UGsUIWindowRepair::OnSelectChangedCategoryTab(int32 InIndex)
{
	bool existWeaponCategory = (_repairableWeaponList.Num() > 0);
	if (existWeaponCategory && InIndex == 0)
	{
		_slotHelperItem->RefreshAll(_repairableWeaponList.Num());
	}
	else
	{
		int32 index = existWeaponCategory ? InIndex - 1 : InIndex;
		_slotHelperItem->RefreshAll(_repairableItemListMap[_categoryList[index]].Num());
	}

	_scrollBoxItem->ScrollToStart();
	_selectedItemSlot.Reset();
	InvalidateInfo();
}

void UGsUIWindowRepair::OnClickedItemSlot(UUserWidget* InWidget)
{
	UGsUIRepairListEntry* slot = Cast<UGsUIRepairListEntry>(InWidget);
	if (nullptr == slot)
		return;

	if (_selectedItemSlot.IsValid())
	{
		_selectedItemSlot->SetIsSelected(false);
	}

	_selectedItemSlot = slot;
	_selectedItemSlot->SetIsSelected(true);

	InvalidateInfo();
}

void UGsUIWindowRepair::InitializeCategoryTab(const TArray<ItemDataCreated>& InItemList)
{
	_repairableWeaponList.Empty();
	_repairableItemListMap.Empty();
	for (const ItemDataCreated& itemData : InItemList)
	{
		const FGsSchemaItemCommon* tableData = FGsItem::GetTableDataByID(itemData.mTemplateId);
		if (nullptr == tableData)
			continue;
		
		ItemCategoryMain main = tableData->categoryMain;
		ItemCategorySub sub = tableData->categorySub;

		RepairItemData repairItemData;
		repairItemData.itemTableData = tableData;
		repairItemData.itemLevel = itemData.mLevel;
		repairItemData.enchantBonusIndex = itemData.mEnchantBonusIndex;
		repairItemData.itemDBId = itemData.mItemDBId;

		if (main == ItemCategoryMain::WEAPON)
		{
			_repairableWeaponList.Emplace(repairItemData);
		}
		else
		{
			if (_repairableItemListMap.Contains(sub))
			{
				_repairableItemListMap[sub].Emplace(repairItemData);
			}
			else
			{
				_repairableItemListMap.Emplace(sub, TArray<RepairItemData>{repairItemData});
			}
		}
	}
	
	SortItemList(_repairableWeaponList);

	_categoryList.Empty();
	for (TPair<ItemCategorySub, TArray<RepairItemData>>& repairableItemList : _repairableItemListMap)
	{
		SortItemList(repairableItemList.Value);
		_categoryList.Emplace(repairableItemList.Key);
	}

	_categoryList.Sort([](ItemCategorySub a, ItemCategorySub b)
		{
			return static_cast<uint8>(a) < static_cast<uint8>(b);
		});
	
	int32 count = (_repairableWeaponList.Num() > 0) ? 1 : 0;
	count += _repairableItemListMap.Num();
	_slotHelperCategoryTab->RefreshAll(count);
}

void UGsUIWindowRepair::InvalidateInfo()
{
	if (_selectedItemSlot.IsValid())
	{
		// 아이템 정보 표시
		_switcherInfo->SetActiveWidgetIndex(0);
		_itemDefaultInfo->SetData(_selectedItemSlot->GetItemData(), true);
		_itemDetailInfo->SetData(_selectedItemSlot->GetItemData());
	}
	else
	{
		// 아이템이 선택되지 않았을 경우 미표시
		_switcherInfo->SetActiveWidgetIndex(1);
	}
}

void UGsUIWindowRepair::SortItemList(TArray<RepairItemData>& InItemList)
{
	InItemList.Sort([](const RepairItemData& itemA, const RepairItemData& itemB)
		{
			// 아이템 세부 카테고리
			if (itemB.itemTableData->categorySub != itemA.itemTableData->categorySub)
			{
				return (uint8)itemB.itemTableData->categorySub > (uint8)itemA.itemTableData->categorySub;
			}

			// 아이템 등급
			if (itemB.itemTableData->grade != itemA.itemTableData->grade)
			{
				return (uint8)itemB.itemTableData->grade < (uint8)itemA.itemTableData->grade;
			}

			// 아이템 TID 
			if (itemB.itemTableData->id != itemA.itemTableData->id)
			{
				return itemB.itemTableData->id > itemA.itemTableData->id;
			}

			// 아이템 레벨
			if (itemB.itemLevel != itemA.itemLevel)
			{
				return itemB.itemLevel < itemA.itemLevel;
			}

			return itemB.enchantBonusIndex > itemA.enchantBonusIndex;
		});
}