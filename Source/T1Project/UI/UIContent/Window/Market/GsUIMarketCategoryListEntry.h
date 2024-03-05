// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "GsUIMarketCategoryListEntry.generated.h"

class UTextBlock;
class UPanelWidget;
class UGsSwitcherButton;
/**
 * 
 */
UCLASS()
class UGsUIMarketCategoryListEntry : public UUserWidget, public IGsToggleGroupEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnSlot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNameOn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNameOff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSubRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSubName;

protected:
	bool _bIsSelected = false;

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void SetIsSelected(bool bInIsSelected);
	virtual bool GetIsSelected() const { return _bIsSelected; }

protected:
	UFUNCTION()
	void OnClickSlot();
	
public:
	void SetUIName(const FText& InText);
	void SetUISubName(const FText& InText);
};
