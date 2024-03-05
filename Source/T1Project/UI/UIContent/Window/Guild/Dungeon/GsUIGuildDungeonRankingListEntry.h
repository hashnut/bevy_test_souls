// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GsUIGuildDungeonRankingListEntry.generated.h"

/**
 * 랭킹표시 UI. 기사단 아지트
 */

 class UTextBlock;
 class UWidgetSwitcher;

UCLASS()
class UGsGuildDungeonRankingListItem : public UObject
{
	GENERATED_BODY()

public:
	int32 _rank = 0;
	FString _strName;
	uint64 _point = 0;

public:
	void SetData(int32 InRank, const FString& InName, uint64 InPoint)
	{
		_rank = InRank;
		_strName = InName;
		_point = InPoint;
	}
};

//----------------------------------------------------------------------------------------------------------------------

UCLASS()
class UGsUIGuildDungeonRankingListEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRank;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPoint;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherRank;

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	void Clear();
	void RefreshAll();

	void InvalidateAll(UGsGuildDungeonRankingListItem* InListItem);

private:	
	void SetRank(int32 InRank);
};
