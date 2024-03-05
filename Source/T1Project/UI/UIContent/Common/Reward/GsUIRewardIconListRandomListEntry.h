// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GsUIRewardIconListRandomListEntry.generated.h"

class UGsUIIconItemReward;
class UGsRewardIconListRandomListItem;

/**
 * 보상 랜덤 ListView의 UI부분
 */
UCLASS()
class T1PROJECT_API UGsUIRewardIconListRandomListEntry : 
	public UUserWidget,
	public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIIconItemReward* _icon;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	void RefreshAll();

protected:
	void InvalidateAll(UGsRewardIconListRandomListItem* InListItem);

	void OnLongPressIcon(class UGsUIIconItem& InIcon);
};
