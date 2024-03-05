// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GsUIGuildDungeonPlayerListEntry.generated.h"

/**
 * 참여자 표시 UI. 기사단 아지트
 */

 class UTextBlock;

UCLASS()
class UGsGuildDungeonPlayerListItem : public UObject
{
	GENERATED_BODY()

public:
	int32 _level = 0;
	FString _strName;

public:
	void SetData(int32 InLevel, const FString& InName)
	{
		_level = InLevel;
		_strName = InName;
	}
};

//----------------------------------------------------------------------------------------------------------------------

UCLASS()
class UGsUIGuildDungeonPlayerListEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	void RefreshAll();
	void InvalidateAll(UGsGuildDungeonPlayerListItem* InListItem);
};
