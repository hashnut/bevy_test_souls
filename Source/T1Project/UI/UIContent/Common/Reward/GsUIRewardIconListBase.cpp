// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRewardIconListBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Item/GsItem.h"


void UGsUIRewardIconListBase::NativeDestruct()
{
	_rewardData = nullptr;

	Super::NativeDestruct();
}

void UGsUIRewardIconListBase::SetData(const FGsRewardUnboxingData* InData)
{
	_rewardData = InData;
}

void UGsUIRewardIconListBase::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}
