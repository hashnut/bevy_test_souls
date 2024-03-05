// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/ItemEnchant/GsUIItemRefineOptionEntry.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemManager.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "GsUIRefineOptionStatEntry.h"
#include "T1Project/GameObject/Stat/GsStatHelper.h"


void UGsUIItemRefineOptionEntry::BeginDestroy()
{
	if (nullptr != _refineStatListPanelSlotHelper)
	{
		_refineStatListPanelSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIItemRefineOptionEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _refineStatListPanelSlotHelper)
	{
		_refineStatListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_refineStatListPanelSlotHelper->Initialize(_refineStatListEntryWidgetClass, _defaultStatListRootPanel);
		_refineStatListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIItemRefineOptionEntry::ItemListRefreshEntry);
	}

}

void UGsUIItemRefineOptionEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_defaultStatPairDataMap.Empty();
}

void UGsUIItemRefineOptionEntry::NativeDestruct()
{
	_refineStatListPanelSlotHelper->RefreshAll(0);

	Super::NativeDestruct();
}

void UGsUIItemRefineOptionEntry::SetStatData(uint64 InTargetItemDBID, const int32 InOptionID, const ItemCategorySub InCategorySub , int32 inIndex)
{
	if (0 >= InTargetItemDBID)
		return;

	TWeakPtr<FGsItem> findItem = GItem()->FindItem(InTargetItemDBID, ItemType::EQUIP);
	if (false == findItem.IsValid())
		return;

	FGsItemEquipment* equipData = findItem.Pin()->GetCastItem<FGsItemEquipment>();
	if (nullptr == equipData)
		return;

	FText findText;
	if (true == FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Refine_Level_Title"), findText))
	{
		FText textMsg = FText::Format(findText, inIndex + 1);
		_titleText->SetText(textMsg);
	}

	_defaultStatPairDataMap.Empty();

	// 스텟타입,<최소값,최대값>
	TMap<StatType, TPair<int32, int32>> statListdataMap;
	equipData->GetAllExpectRefineOptionStatList(InOptionID, InCategorySub, statListdataMap);
	_defaultStatPairDataMap = statListdataMap;

	_refineStatListPanelSlotHelper->RefreshAll(_defaultStatPairDataMap.Num());
}

void UGsUIItemRefineOptionEntry::ItemListRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	UGsUIRefineOptionStatEntry* entry = Cast<UGsUIRefineOptionStatEntry>(InEntry);
	if (nullptr == entry)
		return;

	uint8 index = 0;
	for (const auto& iter : _defaultStatPairDataMap)
	{
		if (index == InIndex)
		{
			StatType type = iter.Key;
			TPair<int32,int32> valuePairData = iter.Value;

			FText statNameText;
			FGsStatHelper::GetDisPlayStatName(type, statNameText);

			FText curFirstValueText;
			FGsStatHelper::GetDisPlayStatValueWithMarkFormat(type, valuePairData.Key, curFirstValueText);

			FText curSecondValueText;
			FGsStatHelper::GetDisPlayStatValueWithMarkFormat(type, valuePairData.Value, curSecondValueText);

			// 스텟이름 , n ~ n 값
			entry->SetStatDisplayInfo(statNameText, curFirstValueText , curSecondValueText , "", false);
			break;
		}
		++index;
	}
}
