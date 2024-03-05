// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildAcquisitionEntry.h"

#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

#include "DataSchema/Guild/GsSchemaGuildAcquisition.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UI/UIContent/Popup/Guild/GsUIGuildAcquisitionOptionButton.h"


void UGsUIGuildAcquisitionEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _slotHelper)
	{
		_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_slotHelper->Initialize(_entryWidgetClass, _panelOptionList);
		_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIGuildAcquisitionEntry::OnRefreshEntryOption);
	}

	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIGuildAcquisitionEntry::OnChangedOption);
}

void UGsUIGuildAcquisitionEntry::OnRefreshEntryOption(int32 InIndex, class UWidget* InEntry)
{
	if (false == _gradeList.IsValidIndex(InIndex))
		return;

	UGsUIGuildAcquisitionOptionButton* entry = Cast<UGsUIGuildAcquisitionOptionButton>(InEntry);
	if (nullptr == entry)
		return;

	FText gradeText;
	if (_gradeList[InIndex] <= 0)
	{
		// TEXT: Off
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Acquisition_Button_Whole_Off"), gradeText);
	}
	else
	{
		if (_data->gradeAll)
		{
			// TEXT: On
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Acquisition_Button_Whole_On"), gradeText);
		}
		else
		{
			GetGradeText(_gradeList[InIndex], gradeText);
		}
	}

	entry->SetText(gradeText);
}

void UGsUIGuildAcquisitionEntry::OnChangedOption(int32 InIndex)
{
	if (false == _gradeList.IsValidIndex(InIndex))
		return;

	OnChangeOption.ExecuteIfBound(_data, _gradeList[InIndex]);
}

void UGsUIGuildAcquisitionEntry::GetGradeText(int32 InGrade, OUT FText& OutGradeText)
{
	ItemGrade itemGrade = static_cast<ItemGrade>(InGrade);
	switch (itemGrade)
	{
	case ItemGrade::NORMAL:
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Acquisition_Button_Grade_Normal"), OutGradeText);
		break;
	case ItemGrade::MAGIC:
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Acquisition_Button_Grade_Magic"), OutGradeText);
		break;
	case ItemGrade::RARE:
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Acquisition_Button_Grade_Rare"), OutGradeText);
		break;
	case ItemGrade::EPIC:
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Acquisition_Button_Grade_Epic"), OutGradeText);
		break;
	case ItemGrade::UNIQUE:
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Acquisition_Button_Grade_Unique"), OutGradeText);
		break;
	case ItemGrade::LEGEND:
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Acquisition_Button_Grade_Legend"), OutGradeText);
		break;
	default:
		break;
	}
}

void UGsUIGuildAcquisitionEntry::SetData(const FGsSchemaGuildAcquisition* InData, int32 InValue)
{
	if (nullptr == InData)
		return;

	_data = InData;

	_textBlockName->SetText(InData->name);

	_gradeList.Empty();
	if (InData->gradeAll)
	{
		_gradeList.Emplace(1); // On
		_gradeList.Emplace(0); // Off
	}
	else
	{
		int32 gradeMaxValue = (int32)InData->gradeMax;
		int32 gradeMinValue = (int32)InData->gradeMin;
		for (int32 grade = gradeMaxValue; grade >= gradeMinValue; --grade)
		{
			_gradeList.Emplace(grade);
		}
		_gradeList.Emplace(0); // Off
	}

	// 버튼 생성
	_slotHelper->RefreshAll(_gradeList.Num());

	// 현재 설정된 옵션 index
	int32 selectedIndex = 0;
	for (int32 i = 0; i < _gradeList.Num(); ++i)
	{
		if (_gradeList[i] == InValue)
		{
			selectedIndex = i;
			break;
		}
	}
	
	_toggleGroup.Clear();
	_toggleGroup.AddToggleWidgetByParentPanel(_panelOptionList);
	_toggleGroup.SetSelectedIndexNoEvent(selectedIndex);
}