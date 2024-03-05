// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Community/GsUICommunityEventMissionTab.h"

#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CanvasPanel.h"
#include "UI/UIContent/Helper/GsUIHelper.h"


void UGsUICommunityEventMissionTab::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_switcherBtn->OnClicked.AddDynamic(this, &UGsUICommunityEventMissionTab::OnClickButton);
}

// 데이터 셋팅
void UGsUICommunityEventMissionTab::SetData(const FText& inTabName)
{
	_textName = inTabName;	
}

// 데이터 셋팅
void UGsUICommunityEventMissionTab::SetData(const FText& inTabName, bool inIsLock, bool inIsRedDot, bool inIsAllRewarded)
{
	_textName = inTabName;
	_lock->SetVisibility(inIsLock ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	_redDot->SetVisibility(inIsRedDot ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	_allRewarded->SetVisibility(inIsAllRewarded ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

// 레드닷 셋팅
void UGsUICommunityEventMissionTab::SetRedDot(bool inIsRedDot)
{
	_redDot->SetVisibility(inIsRedDot? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUICommunityEventMissionTab::OnClickButton()
{
	/*if (_lock)
	{
		if (_lock->IsVisible())
		{
			FText result;
			if (FText::FindText(TEXT("CommunityGirlText"), TEXT("Notice_Disabled_MissionTab"), result))
			{
				FGsUIHelper::TrayMessageTicker(result);
			}
			//return;
		}
	}*/

	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, true);
	}
}

void UGsUICommunityEventMissionTab::SetIsSelected(bool bInIsSelected)
{	
	_bIsSelected = bInIsSelected;
	_switcherBtn->SetIsSelected(static_cast<int32>(bInIsSelected));
}

bool UGsUICommunityEventMissionTab::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUICommunityEventMissionTab::SetTabId(CommunityEventMissionTabId inTabId)
{
#if UE_EDITOR
	_offMissionTabIdText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_onMissionTabIdText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	FText textTabId = FText::FromString(FString::Printf(TEXT("%lld"), inTabId));	

	_onMissionTabIdText->SetText(textTabId);
	_offMissionTabIdText->SetText(textTabId);
#else
	_offMissionTabIdText->SetVisibility(ESlateVisibility::Collapsed);
	_onMissionTabIdText->SetVisibility(ESlateVisibility::Collapsed);
#endif
}