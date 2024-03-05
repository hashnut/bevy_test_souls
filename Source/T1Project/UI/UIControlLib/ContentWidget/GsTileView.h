// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TileView.h"

#include "T1Project.h"

#include "GsTileView.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsTileView : public UTileView
{
	GENERATED_BODY()

private:
	void SetEntryWidgetClass(TSubclassOf<UUserWidget> InEntryWidgetClass)
	{
		EntryWidgetClass = InEntryWidgetClass;
	}

public:
	EListItemAlignment GetTileAlignment()
	{
		return TileAlignment;
	}
	
	void SetTileAlignment(EListItemAlignment InTileAlignment)
	{
		TileAlignment = InTileAlignment;
	}

public:
	void SetTileViewProperties(UGsTileView* InTileView)
	{
		if (nullptr == InTileView)
		{
			GSLOG(Error, TEXT("nullptr == InTileView"));
			return;
		}

		SetEntryWidgetClass(InTileView->GetEntryWidgetClass());

		SetEntryHeight(InTileView->GetEntryHeight());
		SetEntryWidth(InTileView->GetEntryWidth());

		SetTileAlignment(InTileView->GetTileAlignment());
	}
};
