// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIRefineOptionStatListEntry.generated.h"


class UTextBlock;
class UPanelWidget;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIRefineOptionStatListEntry : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _rootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _optionNumText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _optionNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _optionValueText;
	
public:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetData(int32 InIndex, FText& InStatNameText, FText& InStatValueText, const FLinearColor& InColorData);
	
};
