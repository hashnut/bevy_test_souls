// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "GsGuildDungeonParticipationListViewEntry.generated.h"

/**
 *
 */

 class UTextBlock;

UCLASS()
class T1PROJECT_API UGsGuildDungeonParticipationListViewEntryData : public UObject
{
	GENERATED_BODY()

public:
	int32 _level;
	FString _name;

public:
	void SetData(int32 inLevel, const FString& inName)
	{
		_level = inLevel;
		_name = inName;
	}
};

UCLASS()
class T1PROJECT_API UGsGuildDungeonParticipationListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _levelTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _nameTextBlock;

private:
	FText _dungeonUserLevelText;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;

private:	
	void SetLevelText(int32 inLevel);
	void SetNameText(const FString& inName);
};
