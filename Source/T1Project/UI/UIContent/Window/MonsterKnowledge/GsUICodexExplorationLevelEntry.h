// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "GsUICodexExplorationLevelEntry.generated.h"

class UWidgetSwitcher;
class UTextBlock;

enum class EGsExplorationLevelState : uint8
{
	LEVEL_INACTIVE_LOCAL,
	LEVEL_UP_LOCAL,
	LEVEL_ACHIEVED_LOCAL,
	LEVEL_INACTIVE_GLOBAL,
	LEVEL_UP_GLOBAL,
	LEVEL_ACHIEVED_GLOBAL,
};

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUICodexExplorationLevelItem : public UObject
{
	GENERATED_BODY()

private:
	bool _isFirst;
	bool _isLineOn;
	EGsExplorationLevelState _levelState;
	FText _requireLevelUpCount;
	FText _stat;

public:
	void SetData(bool InIsFirst, bool InIsLineOn, EGsExplorationLevelState InLevelState, FText&& InRequireLevelUpCount, FText&& InStat)
	{
		_isFirst = InIsFirst;
		_isLineOn = InIsLineOn;
		_levelState = InLevelState;
		_requireLevelUpCount = MoveTemp(InRequireLevelUpCount);
		_stat = MoveTemp(InStat);
	}

	bool IsFirst() const { return _isFirst; }
	bool IsLineOn() const { return _isLineOn; }
	EGsExplorationLevelState LevelState() const { return _levelState; }
	FText RequireLevelUpCount() const { return _requireLevelUpCount; }
	FText Stat() const { return _stat; }
};

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUICodexExplorationLevelEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _widgetSwitcherLine;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _widgetSwitcherLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textAchievedLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textUnAchievedLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textAchievedLevelGlobal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textUnAchievedLevelGlobal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textExplorationPassivity;

	/************************************************************************/
	/* Overrides                                                            */
	/************************************************************************/
protected:
	void NativeOnInitialized() final;

	/** 실제 엔트리의 데이터 세팅 */
	void NativeOnListItemObjectSet(UObject* InListItem) final;
};
