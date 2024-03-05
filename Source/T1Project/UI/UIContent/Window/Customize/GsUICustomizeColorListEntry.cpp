// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICustomizeColorListEntry.h"
#include "Components/Image.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUICustomizeColorListEntry::NativeOnInitialized()
{
	_btnSlot->OnClicked.AddDynamic(this, &UGsUICustomizeColorListEntry::OnClickSlot);

	Super::NativeOnInitialized();
}

void UGsUICustomizeColorListEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	_imgSelected->SetVisibility((bInIsSelected) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

bool UGsUICustomizeColorListEntry::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUICustomizeColorListEntry::OnClickSlot()
{
	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}

void UGsUICustomizeColorListEntry::SetColor(const FColor& InColor)
{
	// 버튼에 컬러 설정
	_btnSlot->SetBackgroundColor(InColor);
}
