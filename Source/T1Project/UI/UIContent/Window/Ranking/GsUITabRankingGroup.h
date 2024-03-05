// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/Public/Blueprint/UserWidget.h"

#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "GsUITabRankingGroup.generated.h"

class UGsSwitcherButton;
class UTextBlock;
class IGsToggleGroupEntry;

/**
 * 랭킹 그룹 카테고리 탭 버튼
 */
UCLASS()
class T1PROJECT_API UGsUITabRankingGroup : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnGroup;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockOff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockOn;

public:
	void SetTabText(const FText& InText);

	IGsToggleGroupEntry* GetToggleGroupEntry();
};
