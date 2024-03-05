// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICharacterSlot.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Components/TextBlock.h"
#include "UserWidget.h"
#include "WidgetSwitcher.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "CanvasPanel.h"

void UGsUICharacterSlot::NativeOnInitialized()
{
	_btnNormal->OnClicked.AddDynamic(this, &UGsUICharacterSlot::OnClickNormal);
	_btnSelected->OnClicked.AddDynamic(this, &UGsUICharacterSlot::OnClickSelected);
	_btnEmpty->OnClicked.AddDynamic(this, &UGsUICharacterSlot::OnClickEmpty);
	_btnLocked->OnClicked.AddDynamic(this, &UGsUICharacterSlot::OnClickLocked);

	Super::NativeOnInitialized();
}

void UGsUICharacterSlot::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;	

	switch (_slotType)
	{
	case ECharacterSlotType::SLOT_NORMAL:
	case ECharacterSlotType::SLOT_SLEECTED:
		{
			_switcher->SetActiveWidgetIndex((bInIsSelected) ?
				static_cast<int32>(ECharacterSlotType::SLOT_SLEECTED) : static_cast<int32>(ECharacterSlotType::SLOT_NORMAL));
		}
		break;
	default:
		break;
	}
}

bool UGsUICharacterSlot::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUICharacterSlot::OnClickNormal()
{
	OnClickSlot.ExecuteIfBound(ECharacterSlotType::SLOT_NORMAL, _characterIndex);
}

void UGsUICharacterSlot::OnClickSelected()
{
	if (_slotType == ECharacterSlotType::SLOT_SLEECTED)
		return;

	_slotType = ECharacterSlotType::SLOT_SLEECTED;
	OnClickSlot.ExecuteIfBound(ECharacterSlotType::SLOT_SLEECTED, _characterIndex);
}

void UGsUICharacterSlot::OnClickEmpty()
{
	_slotType = ECharacterSlotType::SLOT_EMPTY;
	OnClickSlot.ExecuteIfBound(ECharacterSlotType::SLOT_EMPTY, _characterIndex);
}

void UGsUICharacterSlot::OnClickLocked()
{
	_slotType = ECharacterSlotType::SLOT_LOCKED;
	OnClickSlot.ExecuteIfBound(ECharacterSlotType::SLOT_LOCKED, _characterIndex);
}

void UGsUICharacterSlot::SetData(uint8 InIndex, ECharacterSlotType InType, 
									 FString InName, uint16 InLevel, MapId InMapId, int32 InWeaponType, bool InisDeleteing)
{	
	_slotType = InType;
	_characterIndex = InIndex;

	if (false == InName.IsEmpty())
	{
		_textName = FText::FromString(InName);
		_textLevel = FText::FromString(FString::Printf(TEXT("Lv.%d"), InLevel));

		_switcherWeaponNormal->SetActiveWidgetIndex(InWeaponType);
		_switcherWeaponSelected->SetActiveWidgetIndex(InWeaponType);		
	}

	_switcher->SetActiveWidgetIndex(static_cast<int32>(_slotType));
	_deletingCanvas->SetVisibility((InisDeleteing) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}
