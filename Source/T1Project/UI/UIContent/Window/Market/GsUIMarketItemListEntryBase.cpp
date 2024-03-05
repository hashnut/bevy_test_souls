// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMarketItemListEntryBase.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemStat.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Management/GsMessageHolder.h"


void UGsUIMarketItemListEntryBase::NativeConstruct()
{
	Super::NativeConstruct();

	_iconPtr = GetIcon();
	if (_iconPtr.IsValid())
	{
		_iconPtr->OnFinishedLongPressEvent.AddUObject(this, &UGsUIMarketItemListEntryBase::OnLongPressIcon);
	}
}

void UGsUIMarketItemListEntryBase::NativeDestruct()
{
	if (_iconPtr.IsValid())
	{
		_iconPtr->OnFinishedLongPressEvent.RemoveAll(this);
	}
	_iconPtr = nullptr;

	if (_iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}

	Super::NativeDestruct();
}

void UGsUIMarketItemListEntryBase::OnLongPressIcon(class UGsUIIconItem& InIcon)
{
	OpenItemDetailInfo(GetItem());
}

FGsItem* UGsUIMarketItemListEntryBase::GetItem()
{
	return _iconPtr.IsValid() ? _iconPtr->GetItemIconData() : nullptr;
}

void UGsUIMarketItemListEntryBase::OpenItemDetailInfo(FGsItem* InItem)
{
	if (InItem)
	{
		// 기본 중앙에 띄우기 처리. 필요 시 
		FGsItemDetailOpenMessageParam param(InItem, false, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
	}
}

void UGsUIMarketItemListEntryBase::SetUIItemName(const FText& InNameText, Level InEnchantLevel, ItemGrade InGrade)
{
	_textBlockName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(InGrade));

	if (0 < InEnchantLevel)
	{
		FString strItemName = FString::Format(TEXT("+{0} {1}"), { InEnchantLevel, *InNameText.ToString() });
		_textBlockName->SetText(FText::FromString(strItemName));
	}
	else
	{
		_textBlockName->SetText(InNameText);
	}
}

UGsUIIconItem* UGsUIMarketItemListEntryBase::GetIcon()
{
	if (0 == _iconSelector->GetChildrenCount())
	{
		UGsUIIconBase* newIcon = _iconSelector->CreateIcon();
		if (newIcon)
		{
			_iconPtr = Cast<UGsUIIconItem>(newIcon);
			_iconSelector->AddChild(newIcon);
		}
	}

	// 아이콘을 하나만 생성하므로 0번째 아이콘 리턴
	return Cast<UGsUIIconItem>(_iconSelector->GetChildAt(0));
}

void UGsUIMarketItemListEntryBase::SetUIItemStat(OUT UTextBlock* OutTextBlock)
{
	FText textStat;
	if (GetItemStatText(textStat))
	{
		OutTextBlock->SetText(textStat);
		OutTextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		OutTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool UGsUIMarketItemListEntryBase::GetItemStatText(OUT FText& OutText)
{
	FGsItem* item = nullptr;
	if (UGsUIIconItem* icon = GetIcon())
	{
		item = icon->GetItemIconData();
	}

	if (nullptr == item)
	{
		return false;
	}

	if (ItemType::EQUIP != item->GetType())
	{
		return false;
	}

	FGsItemEquipment* equipItem = item->GetCastItem<FGsItemEquipment>();
	if (nullptr == equipItem)
	{
		return false;
	}

	FString strOut;
	TArray<FGsItemStat*> statList = equipItem->GetCurrentItemStatValue();
	for (FGsItemStat* stat : statList)
	{
		if (false == strOut.IsEmpty())
		{
			strOut.Append(TEXT(", "));
		}

		FString strTemp;
		FGsStatHelper::GetStatWithAdditionalValue(stat->GetStatType(), stat->GetStatOriValue(), 
			stat->GetCalcStatValue(), strTemp);

		strOut.Append(strTemp);
	}
	
	OutText = FText::FromString(strOut);
	
	return true;
}
