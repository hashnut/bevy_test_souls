// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIChangeItemPropertyEntry.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIChangeItemPropertyEntry : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _statNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _statValueText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _expectGainText;

public:
	virtual void OnShow() override;
	virtual void OnHide() override;


public:
	// (공격력) (30 (강화 +8)) (+8강 성공시 획득가능)
	void SetDataText(FText InStatNameText, FText InStatValueText);
	void SetExpectStatText(FText InExpectText);
	void SetExpectTextVisible(const bool InState);
	void SetChangeTextColor(const FLinearColor& InColor);
	void SetChangeExpectTextColor(bool InIsCursed);

};
