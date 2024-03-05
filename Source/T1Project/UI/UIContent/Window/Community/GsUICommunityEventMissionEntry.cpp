// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICommunityEventMissionEntry.h"

#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UserWidget.h"
#include "CanvasPanel.h"

#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "CommunityEventMission/GsCommunityEventMissionData.h"
#include "UI/UIContent/Common/GsUIProgressBar.h"


void UGsUICommunityEventMissionEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUICommunityEventMissionEntry::NativeDestruct()
{
	if (_iconSelector)
	{
		if (auto icon = GetIcon())
		{
			icon->ReleaseToPool();
		}

		_iconSelector->RemoveAllChildren();
	}

	Super::NativeDestruct();
}

void UGsUICommunityEventMissionEntry::OnClickSlot()
{
	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, true);
	}
}

void UGsUICommunityEventMissionEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

}

bool UGsUICommunityEventMissionEntry::GetIsSelected() const
{
	return _bIsSelected;
}

// 데이터 셋팅
void UGsUICommunityEventMissionEntry::SetData(const FText& inCommonName, TSharedPtr<FGsCommunityEventMissionCommon> inMissionCommon, bool inIsRedDot)
{
#if UE_EDITOR
	FText textCommonId = FText::FromString(FString::Printf(TEXT("%lld"), inMissionCommon->GetMissionCommonId()));
	_textName = FText::Format(FTextFormat::FromString("{0} {1}"), inCommonName, textCommonId);
#else
	_textName = inCommonName;
#endif
}

// 데이터 셋팅
void UGsUICommunityEventMissionEntry::SetData(TSharedPtr<FGsCommunityEventMissionCommon> inMissionCommon)
{
	if (!inMissionCommon.IsValid())
	{
		return;
	}

	_commonId = inMissionCommon->GetMissionCommonId();
#if UE_EDITOR
	FText textCommonId = FText::FromString(FString::Printf(TEXT("%lld"), inMissionCommon->GetMissionCommonId()));
	_textName = FText::Format(FTextFormat::FromString("{0} {1}"), inMissionCommon->GetMissionCommonText(), textCommonId);
#else
	_textName = inMissionCommon->GetMissionCommonText();
#endif

	uint64 achievePoint = inMissionCommon->GetAchievePoint();
	uint64 completePoint = inMissionCommon->GetCompletePoint();

	_missionProgress->SetPercent(completePoint > 0 ? float(achievePoint) / float(completePoint) : 0.f);
	_missionProgress->SetProgressText(FText::FromString(FString::Format(TEXT("{0}/{1}"), { achievePoint, completePoint })));

	_completeCanvasPanel->SetVisibility(inMissionCommon->IsRewarded() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	FGsRewardUnboxingData reward = inMissionCommon->GetRewardData();

	// 아이콘
	if (UGsUIIconItemReward* itemIcon = GetIcon())
	{
		itemIcon->SetIsRewarded(inMissionCommon->IsRewarded());
		itemIcon->SetIsAcceptable(inMissionCommon->IsRedDot());
		itemIcon->SetEnableDetailPopup(true);
		itemIcon->SetCenterPopup(true);
		itemIcon->SetItemNameToolTipFlag(true);
		itemIcon->SetIsRewardNoti(inMissionCommon->IsRedDot());

		if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
		{
			itemIcon->OnClickSlot.AddUObject(this, &UGsUICommunityEventMissionEntry::OnClickIconSlot);
		}
		if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUICommunityEventMissionEntry::OnLongPressIcon);
		}

		if (reward._itemDataList.IsValidIndex(0))
		{
			itemIcon->SetNotOwnItem(reward._itemDataList[0]->itemId, reward._itemDataList[0]->itemCountMin);
		}
	}
}

UGsUIIconItemReward* UGsUICommunityEventMissionEntry::GetIcon()
{
	if (0 == _iconSelector->GetChildrenCount())
	{
		UGsUIIconBase* newIcon = _iconSelector->CreateIcon();
		if (newIcon)
		{
			_iconSelector->AddChild(newIcon);
		}
	}

	// 아이콘을 하나만 생성하므로 0번째 아이콘 리턴
	return Cast<UGsUIIconItemReward>(_iconSelector->GetChildAt(0));
}

void UGsUICommunityEventMissionEntry::OnClickIconSlot(class UGsUIIconItem& InIcon)
{
	UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(&InIcon);
	if (nullptr == itemIcon)
		return;

	if (_delegateClickSlot.IsBound())
	{
		_delegateClickSlot.ExecuteIfBound(_commonId);
	}
}

void UGsUICommunityEventMissionEntry::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

