// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "GsUIMarketSubCategoryListEntry.generated.h"

class UTextBlock;
class UGsButton;

/**
 * 
 */
UCLASS()
class UGsUIMarketSubCategoryListEntry : public UUserWidget, public IGsToggleGroupEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSlot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;
	
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
};
