// Fill out your copyright notice in the Description page of Project Settings.

#include "SGsTerrEdBaseFinderCompoundwidget.h"
#include "SScrollBox.h"
#include "STextBlock.h"
#include "SBorder.h"
#include "SButton.h"
#include "SComboBox.h"
#include "GsTableManagerEditor.h"
#include "GameObject/Npc/GsSchemaNpcData.h"
#include "LogMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Editor.h"
#include "Actor/Spawn/GsTerritoryActor.h"
#include "Actor/Spawn/SpawnActor/GsTerritoryNpc.h"
#include "DetailLayoutBuilder.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Util/GsTerrEdSlateUtil.h"
#include "SNumericEntryBox.h"
#include "TerritoryEditor.h"
#include "ActorEx/GsCharacterNonPlayer.h"
#include "UTIL/GsText.h"
#include "Actor/Spot/GsTerritorySpot.h"

#define LOCTEXT_NAMESPACE "TerritoryEditor"


TSharedPtr<SWidget> SGsTerrEdBaseFinderCompoundwidget::CreateSelectCountWidget()
{
	TSharedPtr<SWidget> fieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Select count"));
	_selectCountTextBlock = SNew(STextBlock).Font(IDetailLayoutBuilder::GetDetailFont());
	_selectCountTextBlock->SetText(FText::FromString(FString::FromInt(_selectCount)));

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			fieldName.ToSharedRef()
		]
	+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			_selectCountTextBlock.ToSharedRef()
		];
}

TSharedPtr<SWidget> SGsTerrEdBaseFinderCompoundwidget::CreateTotalCountWidget()
{
	TSharedPtr<SWidget> fieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Total count"));
	_totalCountTextBlock = SNew(STextBlock).Font(IDetailLayoutBuilder::GetDetailFont());
	_totalCountTextBlock->SetText(FText::FromString(FString::FromInt(_totalCount)));

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			fieldName.ToSharedRef()
		]
	+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			_totalCountTextBlock.ToSharedRef()
		];
}


void SGsTerrEdBaseFinderCompoundwidget::ClearListView()
{
	_selectCount = 0;
	_totalCount = 0;

	if (_findActorListView._listView.IsValid())
	{
		_findActorListView.Clear();
	}

	if (_selectCountTextBlock.IsValid())
	{
		_selectCountTextBlock->SetText(FText::FromString(FString::FromInt(_selectCount)));
	}

	if (_totalCountTextBlock.IsValid())
	{
		_totalCountTextBlock->SetText(FText::FromString(FString::FromInt(_totalCount)));
	}

	_selectAllActorCheckBox->Invalidate(EInvalidateWidgetReason::Layout);

	GEditor->SelectNone(false, true, false);
}

#undef LOCTEXT_NAMESPACE