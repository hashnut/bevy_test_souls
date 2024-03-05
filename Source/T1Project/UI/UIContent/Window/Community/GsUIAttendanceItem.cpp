// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Community/GsUIAttendanceItem.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"

#include "item/GsSchemaItemCommon.h"
#include "Attendance/Reward/GsSchemaAttendanceReward.h"
#include "Attendance/Reward/GsSchemaAttendanceCurrency.h"
#include "Attendance/Reward/GsSchemaAttendanceItem.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/GsMessageContents.h"
#include "Management/GsMessageHolder.h"

#include "Text.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "UMG/Public/Components/SizeBox.h"

void UGsUIAttendanceItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();	
}

void UGsUIAttendanceItem::NativeDestruct()
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

void UGsUIAttendanceItem::SetData(int inIndex, bool inIsRewarded, bool inIsAcceptable, const FGsSchemaAttendanceReward* inRewardData)
{
	if (nullptr == inRewardData)
		return;

	_slotIndex = inIndex;

	int day = inRewardData->day;
	FText dayText = FText::FromString(FString::FromInt(day));

	_textNormalDay->SetText(dayText);
	_text7Day->SetText(dayText);

	int remainder = day % Special_Day_Cycle;
	_dayNumBgSwitcher->SetActiveWidgetIndex((0 == remainder)? 1 : 0);
	_bgSwitcher->SetActiveWidgetIndex((0 == remainder) ? 1 : 0 );

	// 아이콘
	if (UGsUIIconItemReward* itemIcon = GetIcon())
	{
		itemIcon->SetIsRewarded(inIsRewarded);
		itemIcon->SetIsAcceptable(inIsAcceptable);
		itemIcon->SetEnableDetailPopup(true);
		itemIcon->SetCenterPopup(true);
		itemIcon->SetItemNameToolTipFlag(true);

		if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
		{
			itemIcon->OnClickSlot.AddUObject(this, &UGsUIAttendanceItem::OnClickIconSlot);
		}
		if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIAttendanceItem::OnLongPressIcon);
		}

		if (const FGsSchemaItemCommon* itemCommon = inRewardData->itemId.GetRow())
		{
			itemIcon->SetNotOwnItem(itemCommon->id, inRewardData->itemCount);
			itemIcon->SetSkillOptionImage(itemIcon->GetItemIconData());
		}
	}
}

UGsUIIconItemReward* UGsUIAttendanceItem::GetIcon()
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

void UGsUIAttendanceItem::SetIsSelected(bool inIsSelected)
{
	if (UGsUIIconItemReward* itemIcon = GetIcon())
	{
		itemIcon->SetIsSelected(inIsSelected);
	}
}

void UGsUIAttendanceItem::SetIsRewarded(bool inIsRewarded)
{
	if (UGsUIIconItemReward* itemIcon = GetIcon())
	{
		itemIcon->SetIsRewarded(inIsRewarded);
	}
}

void UGsUIAttendanceItem::OnClickIconSlot(class UGsUIIconItem& InIcon)
{
	UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(&InIcon);
	if (nullptr == itemIcon)
		return;

	if (_delegateClickSlot.IsBound())
	{
		_delegateClickSlot.ExecuteIfBound(_slotIndex, this);
	}
}

void UGsUIAttendanceItem::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

float UGsUIAttendanceItem::GetHeightSize()
{
	if (_sizebox)
	{
		return _sizebox->HeightOverride;
	}
	return 0.0f;
}

