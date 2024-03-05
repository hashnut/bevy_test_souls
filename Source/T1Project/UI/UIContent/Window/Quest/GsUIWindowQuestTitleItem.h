// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GsUIWindowQuestTitleItem.generated.h"


/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowQuestTitleItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<UGsUIWindowQuestTitleItem*> _childList;

public:
	void AddChild(UGsUIWindowQuestTitleItem* InItem);
	void removeChild(UGsUIWindowQuestTitleItem* InItem);
	void removeAllChildren();

	TArray<UGsUIWindowQuestTitleItem*> GetChildList() const { return _childList; }

	void OnGetChildren(UObject* InItem, TArray<UObject*>& InArray);
};
