// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "GsUICodexGemRequirementListEntry.generated.h"

class UTextBlock;
class UWidgetSwitcher;


/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUICodexGemRequirementListItem : public UObject
{
	GENERATED_BODY()

private:
	FText _requireLevel;
	FText _requireKnowledge;
	bool _isCompleted;

public:
	void SetData(FText&& InLevel, FText&& InKnowledge, bool InIsCompleted)
	{
		_requireLevel = MoveTemp(InLevel);
		_requireKnowledge = MoveTemp(InKnowledge);
		_isCompleted = InIsCompleted;
	}

	FText RequireLevel() { return _requireLevel; }
	FText RequireKnowledge() { return _requireKnowledge; }
	bool IsCompleted() { return _isCompleted; }
};


/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUICodexGemRequirementListEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textKnowledgeLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _widgetSwitcherRequirement;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textRequireKnowledge;

	void NativeOnListItemObjectSet(UObject* InListItemData) final;
};
