// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIQuickEnchantGroupEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Popup/ItemEnchant/GsUIQuickEnchantGroup.h"
#include "UI/UIContent/Popup/ItemEnchant/GsUIQuickEnchantEffectGroup.h"
#include "UI/UIContent/Define/EGsUIColorType.h"

void UGsUIQuickEnchantGroupEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSelect->OnClicked.AddDynamic(this, &UGsUIQuickEnchantGroupEntry::OnClickSelect);
}

void UGsUIQuickEnchantGroupEntry::NativeDestruct()
{
	OnHide();

	Super::NativeDestruct();
}

void UGsUIQuickEnchantGroupEntry::OnShow()
{
	_enchantNum = 0;
	_isSelected = false;
	_eSlotStateType = SlotStateType::State_Normal;

	_widgetSwitcher->SetActiveWidgetIndex(SlotStateType::State_Normal);
}

void UGsUIQuickEnchantGroupEntry::OnHide()
{
	if (OnSelectQuickEnchantDelegate.IsBound())
	{
		OnSelectQuickEnchantDelegate.Clear();
	}
}

void UGsUIQuickEnchantGroupEntry::SetData(const uint8 InEnchantNum, const SlotStateType InSlotStateType , const EGsUIColorType InUIColorType)
{
	_enchantNum = InEnchantNum;
	_eSlotStateType = InSlotStateType;

	_widgetSwitcher->SetActiveWidgetIndex(_eSlotStateType);

	FText markText;
	FText::FindText(TEXT("MarkFormatingText"), TEXT("Plus"), markText);
	FString resultMarkText = markText.ToString() + FString::FromInt(InEnchantNum);
	if (SlotStateType::State_Success == _eSlotStateType)
	{
		_quickEnchantEffect->SetEnchantWarnTextVisibility(false);
		//_quickEnchantEffect->SetEnchantGoalNumText(FText::FromString(resultMarkText));
		_quickEnchantEffect->SetEnchantGoalNumTextColor(InUIColorType);
	}
	else
	{
		UGsUIQuickEnchantGroup* curActiveGroup = GetCurrentActiveGroup();
		curActiveGroup->SetEnchantWarnTextVisibility(false);
		//curActiveGroup->SetEnchantGoalNumText(FText::FromString(resultMarkText));
		curActiveGroup->SetEnchantGoalNumTextColor(InUIColorType);
	}
}

void UGsUIQuickEnchantGroupEntry::OnClickSelect()
{
	if (true == OnSelectQuickEnchantDelegate.IsBound())
	{
		OnSelectQuickEnchantDelegate.Broadcast(_enchantNum);
	}
}

UGsUIQuickEnchantGroup* UGsUIQuickEnchantGroupEntry::GetCurrentActiveGroup()
{
	switch (_eSlotStateType)
	{
	case UGsUIQuickEnchantGroupEntry::State_Normal:
		return _quickEnchantNormal;
	case UGsUIQuickEnchantGroupEntry::State_Selection:
		return _quickEnchantChecked;
	}

	return nullptr;
}