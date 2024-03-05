// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Community/GsUIAttendanceTab.h"

#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UGsUIAttendanceTab::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnNormal->OnClicked.AddDynamic(this, &UGsUIAttendanceTab::OnClickNormal);
	_btnSelected->OnClicked.AddDynamic(this, &UGsUIAttendanceTab::OnClickSelected);
}

void UGsUIAttendanceTab::SetIsSelected(bool bInIsSelected)
{	
	_bIsSelected = bInIsSelected;
	_btnSwitcher->SetActiveWidgetIndex(static_cast<int32>(bInIsSelected));

	//_btnNormal->SetVisibility((bInIsSelected) ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	//_btnSelected->SetVisibility((bInIsSelected) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

bool UGsUIAttendanceTab::GetIsSelected() const
{
	return _bIsSelected;
}

// 데이터 셋팅
void UGsUIAttendanceTab::SetData(CommunityEventId id, const FText& inTabName)
{
	_eventId = id;
	_textName = inTabName;	
}

// 데이터 셋팅
void UGsUIAttendanceTab::SetData(CommunityEventId id, const FText& inTabName, bool inIsRedDot)
{
	_eventId = id;
	_textName = inTabName;
	_redDot->SetVisibility(inIsRedDot ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}


void UGsUIAttendanceTab::SetData(const FText& inTabName, bool inIsRedDot)
{
	_textName = inTabName;
	_redDot->SetVisibility(inIsRedDot ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

// 레드닷 셋팅
void UGsUIAttendanceTab::SetRedDot(bool inIsRedDot)
{
	_redDot->SetVisibility(inIsRedDot? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUIAttendanceTab::OnClickNormal()
{
	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, true);
	}

	if (_delegateClickTab.IsBound())
	{
		_delegateClickTab.ExecuteIfBound(_eventId, true);
	}
}

void UGsUIAttendanceTab::OnClickSelected()
{
	// ToggleGroup에 콜백 전달. 끄기 위해 false 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, false);
	}

	if (_delegateClickTab.IsBound())
	{
		_delegateClickTab.ExecuteIfBound(_eventId, false);
	}
}
