// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITooltipWorldMarketServerList.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/Entry/GsUISimpleTwoTextEntry.h"
#include "DataSchema/Market/GsSchemaWorldMarketServerMatchingData.h"
#include "DataSchema/InterServer/World/GsSchemaWorldData.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"


void UGsUITooltipWorldMarketServerList::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUITooltipWorldMarketServerList::OnRefreshEntry);
	}
	_slotHelper = nullptr;


	Super::BeginDestroy();
}

void UGsUITooltipWorldMarketServerList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelRootList);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUITooltipWorldMarketServerList::OnRefreshEntry);
}

void UGsUITooltipWorldMarketServerList::Open()
{
	Super::Open();

	_table = FindMyMatchingData();

	if (_table)
	{
		_slotHelper->RefreshAll(_table->worldList.Num());
	}
	else
	{
		_slotHelper->RefreshAll(0);
	}
}

void UGsUITooltipWorldMarketServerList::Close()
{
	_table = nullptr;

	Super::Close();
}

void UGsUITooltipWorldMarketServerList::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (nullptr == _table ||
		false == _table->worldList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUISimpleTwoTextEntry* entry = Cast<UGsUISimpleTwoTextEntry>(InEntry))
	{
		entry->SetTextFront(FText::AsNumber(InIndex + 1));

		if (const FGsSchemaWorldData* worldData = _table->worldList[InIndex].GetRow())
		{
			entry->SetTextBack(worldData->worldName);
		}
	}

	const TArray<UPanelSlot*>& slots = _panelRootList->GetSlots();
	if (slots.IsValidIndex(InIndex))
	{
		if (UUniformGridSlot* gridSlot = Cast<UUniformGridSlot>(slots[InIndex]))
		{
			gridSlot->SetRow(FMath::FloorToInt(InIndex / 2));
			gridSlot->SetColumn(InIndex % 2);
			gridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		}
	}
}

const FGsSchemaWorldMarketServerMatchingData* UGsUITooltipWorldMarketServerList::FindMyMatchingData() const
{
	WorldId myHomeWorldId = INVALID_WORLD_ID;
	if (FGsGameDataManager* gameDataMgr = GGameData())
	{
		if (const FGsNetUserData* userData = GGameData()->GetUserData())
		{
			myHomeWorldId = userData->mHomeWorldId;
		}
	}

	TArray<const FGsSchemaWorldMarketServerMatchingData*> matchingList;
	if (const UGsTable* matchingTable = FGsSchemaWorldMarketServerMatchingData::GetStaticTable())
	{
		matchingTable->GetAllRows(matchingList);
	}

	for (const FGsSchemaWorldMarketServerMatchingData* itMatch : matchingList)
	{
		for (const FGsSchemaWorldDataRow& itWorld : itMatch->worldList)
		{
			if (const FGsSchemaWorldData* worldData = itWorld.GetRow())
			{
				if (myHomeWorldId == worldData->worldId)
				{
					return itMatch;
				}
			}
		}
	}

	return nullptr;
}
