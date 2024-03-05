// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupMultiLevelRankStat.h"

#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"

#include "GameObject/Stat/GsStatHelper.h"

#include "Management/ScopeGame/GsMultiLevelRankManager.h"

#include "MultiLevelRank/GsMultiLevelRankData.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "UI/UIContent/Window/MultiLevelRank/GsUIMultiLevelRankStatEntry.h"

#include "T1Project.h"


void UGsUIPopupMultiLevelRankStat::BeginDestroy()
{
	if (nullptr != _slotHelperPassivity)
	{
		_slotHelperPassivity->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupMultiLevelRankStat::OnRefreshPassivityEntryItem);
	}
	_slotHelperPassivity = nullptr;

	if (nullptr != _btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIPopupMultiLevelRankStat::OnClickClose);
	}
	_btnClose = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupMultiLevelRankStat::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperPassivity = NewObject<UGsDynamicPanelSlotHelper>(this);
	if (nullptr != _slotHelperPassivity)
	{
		_slotHelperPassivity->Initialize(_subClassOfPassivityEntry, _scrollBoxPassivityList);
		_slotHelperPassivity->OnRefreshEntry.AddDynamic(this, &UGsUIPopupMultiLevelRankStat::OnRefreshPassivityEntryItem);
	}

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankStat::OnClickClose);
}

void UGsUIPopupMultiLevelRankStat::NativeConstruct()
{
	Super::NativeConstruct();
	
	InvalidateStat();
}

void UGsUIPopupMultiLevelRankStat::OnClickClose()
{
	Close();
}

void UGsUIPopupMultiLevelRankStat::OnRefreshPassivityEntryItem(int32 InIndex, class UWidget* InEntry)
{
	if (false == _refreshStatBuffer.IsValidIndex(InIndex))
	{
		return;
	}

	const TPair<StatType, int32>& statValue = _refreshStatBuffer[InIndex];

	FText statName;
	if (false == FGsStatHelper::GetDisPlayStatName(statValue.Key, statName))
	{
#if  WITH_EDITOR
		GSLOG(Warning, TEXT("UGsUIPopupMultiLevelRankStat::OnRefreshPassivityEntryItem : not find stat name : %d"), int32(statValue.Key));
#endif	//WITH_EDITOR
		return;
	}

	TWeakObjectPtr<UGsUIMultiLevelRankStatEntry> item = Cast<UGsUIMultiLevelRankStatEntry>(InEntry);
	if (item.IsValid())
	{
		FText textAfterStat;
		FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statValue.Key, statValue.Value, textAfterStat);

		item->SetData(statName, textAfterStat);
	}
}

void UGsUIPopupMultiLevelRankStat::InvalidateStat()
{
	// 마지막 랭크에서 첫번째 랭크까지 탐색하며 획득한 스탯 합산
	_refreshStatBuffer.Empty();

	FGsEffectStringHelper::FGsEffectAddData outStatList;
	const FGsMultiLevelRankSlotData* findSlot = GSMultiLevelRank()->GetActiveLastSlot();
	while (true)
	{
		if (nullptr == findSlot)
			break;

		for (const FGsSchemaPassivitySetRow& SetRow : findSlot->_slotSchema->passivityIdList)
		{
			FGsEffectStringHelper::AddPassivityEffect(SetRow.GetRow(), outStatList);
		}

		// prev...
		findSlot = findSlot->_linkedPrevSlot;
	}

	// 합산된 스탯들을 Array로 변경
	for (const auto& data : outStatList._statMap)
	{	
		if (data.Value != 0)
		{
			_refreshStatBuffer.Emplace(data.Key, data.Value);
		}
	}

	if (_refreshStatBuffer.Num() == 0)
	{
		_switcherList->SetActiveWidgetIndex(1);
		_slotHelperPassivity->RefreshAll(0);
	}
	else
	{
		_switcherList->SetActiveWidgetIndex(0);
		
		// 정렬
		_refreshStatBuffer.Sort([=](const TPair<StatType, int32>& source, const TPair<StatType, int32>& dest)->bool
			{
				return source.Key < dest.Key;
			});
		// 갱신
		_slotHelperPassivity->RefreshAll(_refreshStatBuffer.Num());
	}
}

void UGsUIPopupMultiLevelRankStat::OnInputCancel()
{
	OnClickClose();
}