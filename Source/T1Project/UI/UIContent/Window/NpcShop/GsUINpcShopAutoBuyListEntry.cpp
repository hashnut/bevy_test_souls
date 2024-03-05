// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUINpcShopAutoBuyListEntry.h"

#include "Item/GsItemManager.h"

#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/EditableTextBox.h"
#include "Components/ProgressBar.h"

#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"

#include "UI/UIContent/Common/Icon/GsUIIconItemShop.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Management/GsMessageHolder.h"


void UGsUINpcShopAutoBuyListEntry::BeginDestroy()
{
	if (nullptr != _iconSlotHelper)
	{
		_iconSlotHelper->Clear();
		_iconSlotHelper = nullptr;
	}

	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}

	Super::BeginDestroy();
}

void UGsUINpcShopAutoBuyListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_iconSelector);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUINpcShopAutoBuyListEntry::OnRefreshIconWrapBox);
	}

	_amountSlider->OnValueChanged.AddDynamic(this, &UGsUINpcShopAutoBuyListEntry::OnChangeSliderValue);
	_InputText->OnTextCommitted.AddDynamic(this, &UGsUINpcShopAutoBuyListEntry::OnTextConfirmEvent);
	_InputText->OnTextChanged.AddDynamic(this, &UGsUINpcShopAutoBuyListEntry::OnTextChangeEvent);
}

void UGsUINpcShopAutoBuyListEntry::OnHide()
{
	
}

void UGsUINpcShopAutoBuyListEntry::OnShow()
{
	_inputTempCount = 0;
}

void UGsUINpcShopAutoBuyListEntry::SetDisplayItem(const int32 InItemTID , const ItemAmount InSettingCount, const ItemAmount InMaxStackCount)
{
	const FGsSchemaItemCommon* itemCommonData = FGsItem::GetTableDataByID(InItemTID);
	if (nullptr == itemCommonData)
		return;

	_displayItemTID = InItemTID;

	ItemAmount maxCount = (InMaxStackCount < itemCommonData->maxStackCount) ? InMaxStackCount : itemCommonData->maxStackCount;
	
	_amountSlider->SetMinValue(0);
	_amountSlider->SetMaxValue(maxCount);
	SetAmount(InSettingCount);

	_itemNameText->SetText(itemCommonData->name);

	_iconSlotHelper->RefreshAll(1);
}

void UGsUINpcShopAutoBuyListEntry::SetAmount(const ItemAmount InSettingCount, bool InChangeSlider)
{
	if (InChangeSlider)
	{
		_amountSlider->SetValue(InSettingCount);
	}
	
	_InputText->SetText(FText::AsNumber(InSettingCount));

	float amountPercent = (_amountSlider->MaxValue != 0.0f) ? InSettingCount / _amountSlider->MaxValue : 0.0f;
	_progressBarAmount->SetPercent(InSettingCount / _amountSlider->MaxValue);
}

void UGsUINpcShopAutoBuyListEntry::OnClickIconSlot(class UGsUIIconItem& InIcon)
{
	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideButtons = false;
	option._bHideBG = false;
	option._bHideButtonsBottom = true;
	option._bUseLock = false;
	option._bUseCollectionMarking = true;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), UGsUIPopupItemDetailMain::CenterPosType::CenterPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}

void UGsUINpcShopAutoBuyListEntry::OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon)
{
	UGsUIIconItemShop* itemIcon = Cast<UGsUIIconItemShop>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetNotOwnItem(_displayItemTID, 1);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::OnClickType);

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUINpcShopAutoBuyListEntry::OnClickIconSlot);
	}
}

void UGsUINpcShopAutoBuyListEntry::OnChangeSliderValue(float InChangeValue)
{
	ItemAmount convertNum = (ItemAmount)InChangeValue;

	SetAmount(convertNum);
}

void UGsUINpcShopAutoBuyListEntry::OnTextConfirmEvent(const FText& InText, ETextCommit::Type InType)
{
	if (ETextCommit::OnEnter == InType)
	{
		FString confirmStr = InText.ToString();
		if (true == confirmStr.IsEmpty())
		{
			SetAmount(DEFAULT_COUNT);
			return;
		}

		FString replaceStr = confirmStr.Replace(TEXT(","), TEXT(""));
		if (false == replaceStr.IsNumeric())
		{	
			SetAmount(DEFAULT_COUNT);
			return;
		}
		
		// 문자열 길이 체크하여 자릿수가 넘어가면 맥스수치로 변경함
		int32 length = replaceStr.Len();
		FString maxCntLength = FString::FromInt(_amountSlider->MaxValue);
		if (length > maxCntLength.Len())
		{
			replaceStr = maxCntLength;
		}


		ItemAmount resultNum = FMath::Clamp<ItemAmount>(FCString::Atoi(*replaceStr), 0, _amountSlider->MaxValue);
		SetAmount(resultNum);
	}
}

void UGsUINpcShopAutoBuyListEntry::OnTextChangeEvent(const FText& InText)
{
	FString confirmStr = InText.ToString();
	FString replaceStr = confirmStr.Replace(TEXT(","), TEXT(""));
	if (true == replaceStr.IsEmpty())
	{
		SetAmount(DEFAULT_COUNT);
		return;
	}
	else if (false == replaceStr.IsNumeric())
	{
		SetAmount(_inputTempCount);
		return;
	}

	// 문자열 길이 체크하여 자릿수가 넘어가면 맥스수치로 변경함
	int32 length = replaceStr.Len();
	FString maxCntLength = FString::FromInt(_amountSlider->MaxValue);
	if (length > maxCntLength.Len())
	{
		replaceStr = maxCntLength;
	}

	_inputTempCount = FMath::Clamp<ItemAmount>(FCString::Atoi(*replaceStr), 0, _amountSlider->MaxValue);
	SetAmount(_inputTempCount);
}

ItemAmount UGsUINpcShopAutoBuyListEntry::GetSettingItemAmount()
{
	return (ItemAmount)_amountSlider->GetValue();
}
