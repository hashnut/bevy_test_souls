#include "GsUISealPackGainListEntry.h"

#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Runtime/UMG/Public/Components/ListView.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "GsUISealStatListEntry.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"

void UGsUISealPackGainListEntry::NativeOnListItemObjectSet(UObject* InListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(InListItemObject);

	Invalidate(InListItemObject);
}

void UGsUISealPackGainListEntry::Invalidate(const UObject* InListItemObject)
{
	const UGsUISealPackGainListItem* ListItem = Cast<UGsUISealPackGainListItem>(InListItemObject);
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	FText CompleteFormat;
	FText::FindText(TEXT("SealText"), TEXT("UI_Essence_Matched"), CompleteFormat);
	const FText TitleText = FText::Format(CompleteFormat, ListItem->_count);
	const bool IsActivated = ListItem->_activated;
	if (IsActivated)
	{
		if (nullptr != _activationSwitcher)
		{
			_activationSwitcher->SetActiveWidgetIndex(1);
		}

		if (nullptr != _activationTxt)
		{
			_activationTxt->SetText(TitleText);
		}
	}
	else
	{
		if (nullptr != _activationSwitcher)
		{
			_activationSwitcher->SetActiveWidgetIndex(0);
		}

		if (nullptr != _inactivationTxt)
		{
			_inactivationTxt->SetText(TitleText);
		}
	}

	if (nullptr == _statListView)
	{
		GSLOG(Error, TEXT("nullptr == _statListView"));
		return;
	}

	// add item
	int32 i = 0;
	for (; i < ListItem->_statInfos.Num(); ++i)
	{
		UGsUISealStatListItem* SealStatListItem = nullptr;

		// get item
		const TPair<StatType, StatValue>& StatInfo = ListItem->_statInfos[i];
		if (_statListItems.IsValidIndex(i))
		{
			SealStatListItem = _statListItems[i];
		}
		else
		{
			SealStatListItem = NewObject<UGsUISealStatListItem>();
			_statListItems.Emplace(SealStatListItem);
		}

		if (nullptr == SealStatListItem)
		{
			GSLOG(Error, TEXT("nullptr == SealStatListItem, i: %d"), i);
			continue;
		}

		if (StatType::NONE == StatInfo.Key)
		{
			GSLOG(Error, TEXT("StatType::NONE == StatInfo.Key, i: %d"), i);
			continue;
		}

		// set item
		SealStatListItem->_activated = IsActivated;
		SealStatListItem->_statType = StatInfo.Key;
		SealStatListItem->_statValue = StatInfo.Value;
	}

	// trim list
	_statListItems.SetNum(ListItem->_statInfos.Num());

	_statListView->SetListItems(_statListItems);
	_statListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_statListView->RegenerateAllEntries();
}
