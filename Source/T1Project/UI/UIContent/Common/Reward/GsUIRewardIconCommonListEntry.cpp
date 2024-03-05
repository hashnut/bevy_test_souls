// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRewardIconCommonListEntry.h"
#include "UI/UIContent/Common/Reward/GsRewardIconListRandomListItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"


void UGsRewardIconCommonListItem::Clear()
{
	_iconData.Clear();
	_bCenterPopup = false;
	_bEnableDetailPopup = false;
}

void UGsRewardIconCommonListItem::SetData(FGsRewardIconDataWrapper* InIconData,
	bool bInCenterPopup, bool bInEnableDetailPopup)
{
	_iconData.CopyData(InIconData);
	_bCenterPopup = bInCenterPopup;
	_bEnableDetailPopup = bInEnableDetailPopup;
}
//----------------------------------------------------------------------------------------------------------------------
void UGsUIRewardIconCommonListEntry::NativeConstruct()
{
	Super::NativeConstruct();

	// UGsUIIconItem의 NativeDestruct 쪽에서 비워주므로 따로 비우는 코드를 넣지 않았다
	_icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIRewardIconCommonListEntry::OnLongPressIcon);
}

void UGsUIRewardIconCommonListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (UGsRewardIconCommonListItem* item = Cast<UGsRewardIconCommonListItem>(ListItemObject))
	{
		InvalidateAll(item);
	}
}

void UGsUIRewardIconCommonListEntry::RefreshAll()
{
	InvalidateAll(GetListItem<UGsRewardIconCommonListItem>());
}

void UGsUIRewardIconCommonListEntry::InvalidateAll(UGsRewardIconCommonListItem* InListItem)
{
	FGsRewardIconDataWrapper& iconData = InListItem->GetIconData();
	if (false == iconData.IsValid())
	{
		_icon->SetNotOwnItem(0, 0);
		return;
	}

	_icon->SetEnableDetailPopup(InListItem->IsEnableDetailPopup());
	_icon->SetCenterPopup(InListItem->IsCenterPopup());

	// 아이콘 세팅
	FGsRewardIconDataWrapper::SetUIIconItem(&iconData, _icon);
}

void UGsUIRewardIconCommonListEntry::OnLongPressIcon(class UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}
