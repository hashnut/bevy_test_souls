// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIAutoDepositEntry.h"

#include "Components/Slider.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/ProgressBar.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"

#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "Item/GsItemHelper.h"

#include "Paper2D/Classes/PaperSprite.h"


void UGsUIAutoDepositEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIAutoDepositEntry::OnClickSlot);
	_btnTooltip->OnPressed.AddDynamic(this, &UGsUIAutoDepositEntry::OnClickTooltip);

	_sliderGrade->OnValueChanged.AddDynamic(this, &UGsUIAutoDepositEntry::OnChangeSliderValue);
	_sliderGrade->MouseUsesStep = true;
	_sliderGrade->StepSize = 1.0f;
}

void UGsUIAutoDepositEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_tooltipDesc->Close();
}

void UGsUIAutoDepositEntry::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIAutoDepositEntry::SetIcon(const FSoftObjectPath& InIconPath)
{
	//_imgIcon->SetBrushFromTexture(FGsUIHelper::LoadTextureFromPath(InIconPath));
	_imgIcon->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(InIconPath));
}

void UGsUIAutoDepositEntry::SetGroupName(const FText& InGroupName)
{
	_textBlockGroupName->SetText(InGroupName);
}

void UGsUIAutoDepositEntry::SetGradeRange(const ItemGrade InMinGrade, const ItemGrade InMaxGrade)
{
	_minGrade = InMinGrade;
	_maxGrade = InMaxGrade;

	int32 minGrade = static_cast<int32>(_minGrade);
	int32 maxGrade = static_cast<int32>(_maxGrade);

	_sliderGrade->SetMinValue(0.0f);
	_sliderGrade->SetMaxValue(maxGrade - minGrade + 1);
}

void UGsUIAutoDepositEntry::SetGrade(ItemGrade InGrade, bool InUseUserSetting)
{
	int32 grade = static_cast<int32>(InGrade);
	int32 minGrade = static_cast<int32>(_minGrade);
	int32 maxGrade = static_cast<int32>(_maxGrade);
	int32 value = FMath::Clamp(grade - minGrade + 1, 0, maxGrade - minGrade + 1);

	_sliderGrade->SetValue(value);

	if (_sliderGrade->MaxValue != 0.0f)
	{
		_progressBarGrade->SetPercent(value / _sliderGrade->MaxValue);
		_prevSliderValue = value;
	}

	FText gradeText;
	FLinearColor gradeColor = FGsUIColorHelper::GetColorItemGrade(ItemGrade::NONE);
	if (ItemGrade::NONE == InGrade && InUseUserSetting)
	{ // TEXT: 유저 설정
		FText::FindText(TEXT("DepotText"), TEXT("UI_Text_AutoDeposit_UserSetting"), gradeText);
	}
	else
	{ 
		if (value == 0)
		{ // TEXT: 없음
			FText::FindText(TEXT("ItemUIText"), TEXT("Grade_None"), gradeText);
		}
		else
		{
			FGsItemHelper::GetItemGradeText(InGrade, gradeText);
			gradeColor = FGsUIColorHelper::GetColorItemGrade(InGrade);
		}
	}
	_textBlockGrade->SetText(gradeText);
	_textBlockGrade->SetColorAndOpacity(gradeColor);
}

void UGsUIAutoDepositEntry::SetTooltipText(const FText& InTooltip)
{
	_tooltipDesc->SetDescText(InTooltip);
}

void UGsUIAutoDepositEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;
	_btnSlot->SetIsSelected(bInIsSelected);
	//_imgSelected->SetVisibility(bInIsSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIAutoDepositEntry::OnClickSlot()
{
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}

void UGsUIAutoDepositEntry::OnClickTooltip()
{
	_tooltipDesc->ToggleOpenClose();
}

void UGsUIAutoDepositEntry::OnChangeSliderValue(float InChangeValue)
{
	int32 changeValue = FMath::RoundToInt(InChangeValue);
	if (_prevSliderValue == changeValue)
		return;

	// 슬롯 선택
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, true);
	}

	if (_sliderGrade->MaxValue != 0.0f)
	{
		_progressBarGrade->SetPercent(InChangeValue / _sliderGrade->MaxValue);
		_prevSliderValue = changeValue;
	}

	ItemGrade changeGrade = ItemGrade::NONE;
	if (0 >= InChangeValue)
	{
		FText gradeText;
		FText::FindText(TEXT("ItemUIText"), TEXT("Grade_None"), gradeText);
		_textBlockGrade->SetText(gradeText);
		_textBlockGrade->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(changeGrade));
	}
	else
	{
		// slider value 에서 ItemGrade 로 변환
		int32 minGrade = static_cast<int32>(_minGrade);
		int32 maxGrade = static_cast<int32>(_maxGrade);
		int32 gradeValue = FMath::Clamp<int32>(InChangeValue + minGrade - 1, minGrade, maxGrade);

		changeGrade = static_cast<ItemGrade>(gradeValue);

		FText gradeText;
		FGsItemHelper::GetItemGradeText(changeGrade, gradeText);
		_textBlockGrade->SetText(gradeText);
		_textBlockGrade->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(changeGrade));
	}

	OnChangeAutoDepositGrade.Broadcast(_indexInGroup, changeGrade);
}
