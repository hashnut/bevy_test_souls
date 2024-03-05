// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "GsGuildDungeonRewardListViewEntry.generated.h"

/**
 *
 */
 struct FGsSchemaRewardItemBagData;

UCLASS()
class T1PROJECT_API UGsGuildDungeonRewardListViewEntryData : public UObject
{
	GENERATED_BODY()

public:
	FGsUIRewardIconData _rewardData;
};

UCLASS()
class T1PROJECT_API UGsGuildDungeonRewardListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:	
	UPROPERTY()
		class UGsDynamicIconSlotHelper* _rewardSlotHelper;
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
		class UGsWrapBoxIconSelector* _rewardIconSelector;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _rewardTextBlock;

private:
	FGsUIRewardIconData _rewardData;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;

public:
	UFUNCTION()
	void OnCreateIcon(UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon);	
	void OnLongPressIcon(UGsUIIconItem& InIcon);
};
