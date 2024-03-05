// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupCommonItemAction.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "TextBlock.h"
#include "RichTextBlock.h"
#include "Image.h"
#include "Button.h"
#include "CanvasPanel.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "PaperSprite.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "Currency/GsCurrencyHelper.h"
#include "../../../Item/GsItemManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Management/GsMessageHolder.h"


void UGsUIPopupCommonItemAction::BeginDestroy()
{
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupCommonItemAction::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnYes->OnClicked.AddDynamic(this, &UGsUIPopupCommonItemAction::OnClickYes);
	_btnNo->OnClicked.AddDynamic(this, &UGsUIPopupCommonItemAction::OnClickNo);
	_btnNotice->OnClicked.AddDynamic(this, &UGsUIPopupCommonItemAction::OnClickYes);

	_slotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_slotHelper->Initialize(_iconSelector);
	_slotHelper->OnCreateIcon.AddDynamic(this, &UGsUIPopupCommonItemAction::OnCreateIcon);
	_slotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIPopupCommonItemAction::OnRefreshIcon);

	_itemTextColor = _itemNameText->ColorAndOpacity;
}

void UGsUIPopupCommonItemAction::NativeConstruct()
{
	Super::NativeConstruct();

	_switcherButtonStyle->SetActiveWidgetIndex(StaticCast<int32>(EButtonStyle::Decidable));
	_itemNameText->SetColorAndOpacity(_itemTextColor);

	// 2022/10/19 PKT - 스킬북 분해 시 Text Color를 변경 하는데 이게 다른 곳에서도 변경된 색이 유지됨. 초기화 코드 넣음.
	_currencyAmountText->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_ENOUGH));

	SetIconVisibility(false);
	SetNameVisibility(false);
	SetDescVisibility(false);
	SetCurrencyVisibility(false);
}

void UGsUIPopupCommonItemAction::NativeDestruct()
{
	_slotHelper->Clear();
	_iconSelector->RemoveAllChildren();
	_buttonCallback = nullptr;
	_itemList.Empty();
	_itemDbidAmountPairList.Empty();

	Super::NativeDestruct();
}

void UGsUIPopupCommonItemAction::OnClickYes()
{
	Close();

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(true);
	}
}

void UGsUIPopupCommonItemAction::OnClickNo()
{
	Close();

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}
}

void UGsUIPopupCommonItemAction::SetCurrencyData(CurrencyType InCurrencyType, Currency InCurrency)
{
	SetCurrencyVisibility(true);

	// 재화 이미지
	if (_currencyImage)
	{
		UPaperSprite* sprite = GetIconImage(InCurrencyType);
		if (sprite)
		{
			_currencyImage->SetVisibility(ESlateVisibility::Visible);
			_currencyImage->SetBrushFromAtlasInterface(sprite);
		}
		else
		{
			_currencyImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	// 재화 수량
	_currencyAmountText->SetText(FText::AsNumber(InCurrency));
}

void UGsUIPopupCommonItemAction::SetCurrencyItemData(ItemId InCurrencyItemId, ItemAmount InAmount)
{
	SetCurrencyVisibility(true);

	// 재화 아이템 이미지
	if (_currencyImage)
	{
		UPaperSprite* sprite = GetIconImage(InCurrencyItemId);
		if (sprite)
		{
			_currencyImage->SetVisibility(ESlateVisibility::Visible);
			_currencyImage->SetBrushFromAtlasInterface(sprite);
		}
		else
		{
			_currencyImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	// 재화 아이템 수량
	_currencyAmountText->SetText(FText::AsNumber(InAmount));
}

void UGsUIPopupCommonItemAction::SetDescription(const FText& InContent)
{
	SetDescVisibility(true);

	// 부가 설명
	_contentDescText->SetText(InContent);

	FText yesText;
	FText noText;
	FText::FindText(TEXT("UICommonText"), TEXT("DefaultPopupButtonOK"), yesText); // TEXT: 확인
	FText::FindText(TEXT("UICommonText"), TEXT("DefaultPopupButtonNo"), noText); // TEXT: 취소
	_yesButtonText->SetText(yesText);
	_noButtonText->SetText(noText);
}

void UGsUIPopupCommonItemAction::SetItem(FGsItem* InItem, ItemAmount InAmount /*= 1*/, bool bUseItemColor /*= false*/, bool bUseLongPress)
{
	SetIconVisibility(true);
	SetNameVisibility(true);

	// 아이템 아이콘
	_itemDbidAmountPairList.Empty();
	_itemList.Empty();
	_itemList.Emplace(InItem);

	_useLongPress = bUseLongPress;
	_slotHelper->RefreshAll(1);

	// 아이템 이름
	if (1 == InAmount)
	{
		_itemNameText->SetText(InItem->GetName());
	}
	else
	{
		FText multiplyText;
		FText::FindText(TEXT("MarkFormatingText"), TEXT("Multiply"), multiplyText);
		FText itemNameWithCount = FText::FromString(FString::Format(TEXT("{0} {1} {2}"), 
			{ InItem->GetName().ToString(), multiplyText.ToString(), FString::FormatAsNumber(InAmount) }));
		_itemNameText->SetText(itemNameWithCount);
	}

	if (bUseItemColor)
	{
		_itemNameText->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(InItem->GetGradeType()));
	}
}

void UGsUIPopupCommonItemAction::SetItemEx(FGsItem* InItem, ItemAmount InAmount /*= 1*/, bool bUseItemColor /*= false*/, bool bUseLongPress)
{
	SetIconVisibility(true);
	SetNameVisibility(true);

	// 아이템 아이콘
	_itemDbidAmountPairList.Empty();
	_itemList.Empty();
	_itemList.Emplace(InItem);

	_useLongPress = bUseLongPress;
	_slotHelper->RefreshAll(1);

	FString strItemName;
	if (0 < InItem->GetLevel())
	{
		strItemName = FString::Format(TEXT("+{0} {1}"), { InItem->GetLevel(), *InItem->GetName().ToString() });
	}
	else
	{
		strItemName = InItem->GetName().ToString();
	}

	// 아이템 이름
	if (1 == InAmount)
	{
		_itemNameText->SetText(FText::FromString(strItemName));
	}
	else
	{
		FText multiplyText;
		FText::FindText(TEXT("MarkFormatingText"), TEXT("Multiply"), multiplyText);
		FText itemNameWithCount = FText::FromString(FString::Format(TEXT("{0} {1} {2}"),
			{ strItemName, multiplyText.ToString(), FString::FormatAsNumber(InAmount) }));
		_itemNameText->SetText(itemNameWithCount);
	}

	if (bUseItemColor)
	{
		_itemNameText->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(InItem->GetGradeType()));
	}
}

void UGsUIPopupCommonItemAction::SetItemList(const TArray<FGsItem*>& InItemList, bool bUseLongPress)
{
	SetIconVisibility(true);

	// 아이템 아이콘
	_itemDbidAmountPairList.Empty();
	_itemList.Empty();
	for (int32 i = 0; i < InItemList.Num(); ++i)
	{
		if (nullptr == InItemList[i])
			continue;
		_itemList.Emplace(InItemList[i]);
	}

	if (1 == _itemList.Num())
	{
		_itemNameText->SetText(_itemList[0]->GetName());
		SetNameVisibility(true);
	}
	else
	{
		SetNameVisibility(false);
	}

	_useLongPress = bUseLongPress;
	_slotHelper->RefreshAll(_itemList.Num());
}

void UGsUIPopupCommonItemAction::SetItemList(const TArray<ItemDBIdAmountPair>& InDbidAmountPairList, bool bUseLongPress)
{
	SetIconVisibility(true);

	// 아이템 아이콘
	_itemDbidAmountPairList.Empty();
	_itemList.Empty();

	for (int32 i = 0; i < InDbidAmountPairList.Num(); ++i)
	{
		_itemDbidAmountPairList.Emplace(InDbidAmountPairList[i]);
	}

	if (1 == _itemDbidAmountPairList.Num())
	{
		TWeakPtr<FGsItem> findData = GItem()->FindItem(_itemDbidAmountPairList[0].mItemDBId);
		if (false == findData.IsValid())
		{
			SetNameVisibility(false);
		}
		else
		{
			_itemNameText->SetText(findData.Pin()->GetName());
			SetNameVisibility(true);
		}
	}
	else
	{
		SetNameVisibility(false);
	}

	_useLongPress = bUseLongPress;
	_slotHelper->RefreshAll(_itemDbidAmountPairList.Num());
}

void UGsUIPopupCommonItemAction::SetCallBack(TFunction<void(bool)> InCallback)
{
	_buttonCallback = InCallback;
}

void UGsUIPopupCommonItemAction::SetButtonStyle(EButtonStyle InStyle)
{
	_switcherButtonStyle->SetActiveWidgetIndex(StaticCast<int32>(InStyle));
}

UGsUIIconItemInventory* UGsUIPopupCommonItemAction::GetIcon()
{
	if (0 == _iconSelector->GetChildrenCount())
	{
		UGsUIIconBase* newIcon = _iconSelector->CreateIcon();
		if (newIcon)
		{
			_iconSelector->AddChild(newIcon);

			if (UGsUIIconItemInventory* newItemIcon = Cast<UGsUIIconItemInventory>(newIcon))
			{
				newItemIcon->ResetCoolTime();
				newItemIcon->SetItemCount(0);

				return newItemIcon;
			}
		}
	}

	// 아이콘을 하나만 생성하므로 0번째 아이콘 리턴
	return Cast<UGsUIIconItemInventory>(_iconSelector->GetChildAt(0));
}


UPaperSprite* UGsUIPopupCommonItemAction::GetIconImage(CurrencyType InType)
{
	return FGsUIHelper::LoadSpriteFromPath(FGsCurrencyHelper::GetCurrencyIconPath(InType));
}

UPaperSprite* UGsUIPopupCommonItemAction::GetIconImage(int32 InItemId)
{
	const UGsTableItemCommon* table = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	if (table)
	{
		const FGsSchemaItemCommon* row;
		if (table->FindRowById(InItemId, row))
		{
			return FGsUIHelper::LoadSpriteFromPath(row->icon);
		}
	}
#if WITH_EDITOR
	GSLOG(Warning, TEXT("Fail to get icon. ItemId: %d"), InItemId);
#endif

	return nullptr;
}

void UGsUIPopupCommonItemAction::OnCreateIcon(UGsUIIconBase* InIcon)
{

}

void UGsUIPopupCommonItemAction::OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	if (false == _itemList.IsValidIndex(InIndex) && false == _itemDbidAmountPairList.IsValidIndex(InIndex))
		return;

	if (UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon))
	{
		if (true == _itemList.IsValidIndex(InIndex))
		{
			FGsItem* itemData = _itemList[InIndex];
			if (GItem()->IsOwnedItem(itemData->GetDBID(), itemData->GetType()))
			{
				itemIcon->SetOwnedItem(itemData->GetDBID());
			}
			else
			{
				itemIcon->SetNotOwnItem(itemData->GetTID(), itemData->GetAmount(), itemData->GetLevel());
			}
		}
		else if(true == _itemDbidAmountPairList.IsValidIndex(InIndex))
		{
			itemIcon->SetOwnedItem(_itemDbidAmountPairList[InIndex].mItemDBId);
			itemIcon->SetItemCount(_itemDbidAmountPairList[InIndex].mItemAmount);
		}

		itemIcon->SetActiveIconBg(true);
		itemIcon->SetItemNameVisibility(false);
		UGsUIIconItem::UseDisplayDetailWndType longPressType = _useLongPress ? UGsUIIconItem::UseDisplayDetailWndType::LongPressType : UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType;
		itemIcon->SetOnChangeDisplayDetailWndType(longPressType);

		if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPopupCommonItemAction::OnLongPressIcon);
		}
	}
}

void UGsUIPopupCommonItemAction::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
		return;

	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideButtons = true;
	option._bHideBG = false;
	option._bHideButtonsBottom = true;
	option._bUseLock = true;
	option._bUseCollectionMarking = true;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), UGsUIPopupItemDetailMain::CenterPosType::CenterPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}

void UGsUIPopupCommonItemAction::SetIconVisibility(bool InActive)
{
	_iconSelector->SetVisibility(InActive ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUIPopupCommonItemAction::SetNameVisibility(bool InActive)
{
	_itemNameText->SetVisibility(InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIPopupCommonItemAction::SetDescVisibility(bool InActive)
{
	_contentDescText->SetVisibility(InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIPopupCommonItemAction::SetCurrencyVisibility(bool InActive)
{
	_currencySlot->SetVisibility(InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIPopupCommonItemAction::ClickNo()
{
	OnClickNo();
}

bool UGsUIPopupCommonItemAction::IsEnoughCurrencyAmount(ItemAmount InTargetAmount, ItemAmount InCurrencyAmount)
{
	EGsUIColorType colorType = InTargetAmount <= InCurrencyAmount ? EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK;
	_currencyAmountText->SetColorAndOpacity(FGsUIColorHelper::GetColor(colorType));

	return EGsUIColorType::DEFAULT_ENOUGH == colorType;
}

void UGsUIPopupCommonItemAction::OnInputCancel()
{
	OnClickNo();
}