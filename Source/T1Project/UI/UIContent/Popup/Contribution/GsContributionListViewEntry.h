// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "GsContributionListViewEntry.generated.h"

/**
 *
 */

 class UWidgetSwitcher;
 class UProgressBar;
 class UTextBlock;

UCLASS()
class UGsContributionListViewEntryData : public UObject
{
	GENERATED_BODY()

public:
	int32 _rank;
	FString _userName;
	float _contribution;
};

UCLASS()
class UGsContributionListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UWidgetSwitcher* _rankWidgetSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _rankTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _nameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UProgressBar* _contributionBar;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _contributionTextBlock;

public:
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;

public:
	void RefreshAll();
	void InvalidateAll(UGsContributionListViewEntryData* InListItem);
	void SetContribution(int32 inRank, const FString& inUserName, float inProgress);
};
