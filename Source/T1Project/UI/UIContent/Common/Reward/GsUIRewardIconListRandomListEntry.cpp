// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRewardIconListRandomListEntry.h"
#include "UI/UIContent/Common/Reward/GsRewardIconListRandomListItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "Reward/GsRewardIconDataWrapper.h"


void UGsUIRewardIconListRandomListEntry::NativeConstruct()
{
	Super::NativeConstruct();

	// UGsUIIconItem의 NativeDestruct 쪽에서 비워주므로 따로 비우는 코드를 넣지 않았다
	_icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIRewardIconListRandomListEntry::OnLongPressIcon);
}

void UGsUIRewardIconListRandomListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (UGsRewardIconListRandomListItem* item = Cast<UGsRewardIconListRandomListItem>(ListItemObject))
	{
		InvalidateAll(item);
	}
}

void UGsUIRewardIconListRandomListEntry::RefreshAll()
{
	InvalidateAll(GetListItem<UGsRewardIconListRandomListItem>());
}

void UGsUIRewardIconListRandomListEntry::InvalidateAll(UGsRewardIconListRandomListItem* InListItem)
{
	FGsRewardIconDataWrapper* iconData = InListItem->GetIconData();
	if (nullptr == iconData)
	{
		_icon->SetNotOwnItem(0, 0);
		return;
	}

	_icon->SetEnableDetailPopup(InListItem->IsEnableDetailPopup());
	_icon->SetCenterPopup(InListItem->IsCenterPopup());

	// 아이콘 세팅
	FGsRewardIconDataWrapper::SetUIIconItem(iconData, _icon);
}

void UGsUIRewardIconListRandomListEntry::OnLongPressIcon(class UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}
