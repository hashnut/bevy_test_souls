// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIBMShopProductTextEntry.generated.h"

class UTextBlock;
class URichTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBMShopProductTextEntry : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _titleText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _descRichText;
	

public:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetTitleText(const FText& InText);
	void SetDescText(const FText& InText);

};
