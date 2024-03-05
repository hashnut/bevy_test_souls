// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupItemRefineStatList.h"
#include "Components/VerticalBox.h"
#include "Management/GsScopeHolder.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemManager.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "GsUIItemRefineOptionEntry.h"
#include "GsUIRefineOptionStatEntry.h"
#include "T1Project/GameObject/Stat/GsStatHelper.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGlobal.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"



void UGsUIPopupItemRefineStatList::BeginDestroy()
{
	if (nullptr != _refineOptionListPanelSlotHelper)
	{
		_refineOptionListPanelSlotHelper = nullptr;
	}
	if (nullptr != _bonusStatListPanelSlotHelper)
	{
		_bonusStatListPanelSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPopupItemRefineStatList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _refineOptionListPanelSlotHelper)
	{
		_refineOptionListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_refineOptionListPanelSlotHelper->Initialize(_refineOptionListEntryWidgetClass, _refineOptionListRootPanel);
		_refineOptionListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupItemRefineStatList::ItemListRefreshEntry);
	}
	if (nullptr == _bonusStatListPanelSlotHelper)
	{
		_bonusStatListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_bonusStatListPanelSlotHelper->Initialize(_bonusStatListEntryWidgetClass, _bonusStatListRootPanel);
		_bonusStatListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupItemRefineStatList::BonusItemListRefreshEntry);
	}

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupItemRefineStatList::OnClickClose);

}

void UGsUIPopupItemRefineStatList::NativeDestruct()
{
	//OnClickClose();

	_targetItemDBID = 0;
	_targetItemTid = 0;
	_materialCategorySub = ItemCategorySub::NONE;

	_refineOptionListPanelSlotHelper->RefreshAll(0);
	_bonusStatListPanelSlotHelper->RefreshAll(0);

	_refineOptionIdList.Empty();
	_bonusStatDataMap.Empty();

	Super::NativeDestruct();
}

void UGsUIPopupItemRefineStatList::OnClickClose()
{
	Close(true);

	//this->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIPopupItemRefineStatList::OpenExpectStatList(const uint64 InEquipItemDBID, const ItemCategorySub InRefineOptionType)
{
	TWeakPtr<FGsItem> finditemData = GItem()->FindItem(InEquipItemDBID);
	if (false == finditemData.IsValid())
		return;

	FGsItemEquipment* equipDatea = finditemData.Pin()->GetCastItem<FGsItemEquipment>();
	if (nullptr == equipDatea)
		return;

	TArray<int32> optionIdList;
	equipDatea->GetRefineOptionIdList(optionIdList);
	if (0 >= optionIdList.Num())
	{
		GSLOG(Error, TEXT("[UGsUIPopupItemRefineStatList::OpenExpectStatList] - GetRefineOptionIdList() is empty "));
		_refineOptionListPanelSlotHelper->RefreshAll(0);
		_bonusStatListPanelSlotHelper->RefreshAll(0);
		_bonusStatRootPanel->SetVisibility(ESlateVisibility::Collapsed);
		Close();
		return;
	}

	_targetItemDBID = finditemData.Pin()->GetDBID();
	_targetItemTid = finditemData.Pin()->GetTID();
	_materialCategorySub = InRefineOptionType;
	_refineOptionIdList = optionIdList;
	_refineOptionListPanelSlotHelper->RefreshAll(_refineOptionIdList.Num());

	TMap<StatType, TPair<int32, int32>> bonusStatDataMap;
	equipDatea->GetAllExpectBonusRefineOptionStatList(bonusStatDataMap);
	_bonusStatDataMap = bonusStatDataMap;
	if (0 >= _bonusStatDataMap.Num())
	{
		_bonusStatListPanelSlotHelper->RefreshAll(0);
		_bonusStatRootPanel->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	_bonusStatRootPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_bonusStatListPanelSlotHelper->RefreshAll(_bonusStatDataMap.Num());
}

void UGsUIPopupItemRefineStatList::ItemListRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	UGsUIItemRefineOptionEntry* entry = Cast<UGsUIItemRefineOptionEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (false == _refineOptionIdList.IsValidIndex(InIndex))
		return;

	entry->SetStatData(_targetItemDBID ,_refineOptionIdList[InIndex], _materialCategorySub, InIndex);
}

void UGsUIPopupItemRefineStatList::BonusItemListRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	UGsUIRefineOptionStatEntry* entry = Cast<UGsUIRefineOptionStatEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (InIndex >= _bonusStatDataMap.Num())
	{
		return;
	}

	int32 index = 0;
	// TMap<StatType, TPair<int32, int32>>
	for (const auto& iter : _bonusStatDataMap)
	{
		if (index == InIndex)
		{
			FText markText;
			FText::FindText(TEXT("MarkFormatingText"), TEXT("SwungDash"), markText);
			FString statValueResultString;

			StatType type = iter.Key;
			//float minValue = ((float)iter.Value.Key / 10000.f) * 100.f;
			//float maxValue = ((float)iter.Value.Value / 10000.f) * 100.f;

			FText statName;
			FGsStatHelper::GetDisPlayStatName(type, statName);

			FText valueMin;
			FText valueMax;
			FGsStatHelper::GetDisPlayStatValueWithMarkFormat(type, iter.Value.Key, valueMin);
			FGsStatHelper::GetDisPlayStatValueWithMarkFormat(type, iter.Value.Value, valueMax);

			statValueResultString = FString::Format(TEXT("{0}{1}{2}"), { *valueMin.ToString(), *markText.ToString(), *valueMax.ToString() });

			FString resultString;
			entry->SetStatDisplayInfo(statName, statName, statName, statValueResultString, true);

			break;
		}
		++index;
	}
}

void UGsUIPopupItemRefineStatList::OnInputOk()
{
	Close();
}

void UGsUIPopupItemRefineStatList::OnInputCancel()
{
	Close();
}