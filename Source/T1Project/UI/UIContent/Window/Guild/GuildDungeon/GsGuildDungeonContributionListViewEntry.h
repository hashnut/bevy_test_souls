// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
#include "GsGuildDungeonContributionListViewEntry.generated.h"

/**
 *
 */

 class UTextBlock;

UCLASS()
class T1PROJECT_API UGsGuildDungeonContributionListViewEntryData : public UObject
{
	GENERATED_BODY()

public:
	int32 _rank;
	FString _name;
	ContributePoint _contribution;

public:
	void SetData(int32 inRank, const FString& inName, ContributePoint inContribution)
	{
		_rank = inRank;
		_name = inName;
		_contribution = inContribution;
	}
};

UCLASS()
class T1PROJECT_API UGsGuildDungeonContributionListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _rankTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _nameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _contributionTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _rankWidgetSwitcher;

private:
	FText _dungeonNRankText;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;

public:
	void Clear();

private:
	void SetRank(int32 inRank);
	void SetNameText(const FString& inName);
	void SetContributionText(int32 inContribution);
};
