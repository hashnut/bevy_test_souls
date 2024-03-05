// Fill out your copyright notice in the Description page of Project Settings.


#include "GsBossInfoWeekListViewEntry.h"
#include "GsUIBossDetailInfoPanel.h"
#include "UMG/Public/Components/TextBlock.h"
#include "T1Project/UI/UIControlLib/ContentWidget/GsButton.h"

void UGsBossInfoWeekListViewEntry::NativeOnInitialized()
{
	_button->OnClicked.AddDynamic(this, &UGsBossInfoWeekListViewEntry::OnClickWeek);
}

void UGsBossInfoWeekListViewEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsBossInfoWeekListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsBossInfoWeekListViewEntryData* data = Cast<UGsBossInfoWeekListViewEntryData>(inListItemObject);
	if (data)
	{		
		_weekIndex = data->_weekIndex;
		_bossData = data->_bossData;
		_parent = data->_parent;		
	}	

	switch (_weekIndex)
	{	
	case 0:
		_textBlock->SetText(FText::FromString("Monday"));
		break;
	case 1:
		_textBlock->SetText(FText::FromString("Tuesday"));
		break;
	case 2:
		_textBlock->SetText(FText::FromString("Wednesday"));
		break;
	case 3:
		_textBlock->SetText(FText::FromString("Thursday"));		
		break;
	case 4:
		_textBlock->SetText(FText::FromString("Friday"));		
		break;
	case 5:
		_textBlock->SetText(FText::FromString("Saturday"));
		break;
	case 6:
		_textBlock->SetText(FText::FromString("Sunday"));
		break;
	default:
		break;
	}
}

void UGsBossInfoWeekListViewEntry::OnClickWeek()
{
	_parent->SetWeek(_weekIndex);
}
