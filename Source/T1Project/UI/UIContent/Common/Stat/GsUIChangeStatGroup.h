// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIChangeStatGroup.generated.h"

class UImage;
class UPanelWidget;
class UTextBlock;
/**
 * 스탯 전 후 비교 UI
 * - 아이템 강화에서 사용
 * - 스킬 강화에서 사용
 */
UCLASS()
class T1PROJECT_API UGsUIChangeStatGroup : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _statName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _currentValue;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _expectValue;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _arrowImage;


public:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetStatName(const FText& InStatName);
	void SetCurrentStatValue(const FText& InCurValue);
	void SetExpectStatValue(const FText& InExpectValue);
	void SetStatGroupUIPanel(bool InActive, bool InIsRefineOption = false);
	void SetStatNameColor(const FLinearColor& InColor);
	void SetExpectValueColor(const FLinearColor& InColor);
};
