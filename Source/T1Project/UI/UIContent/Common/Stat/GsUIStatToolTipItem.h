// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIStatToolTipItem.generated.h"

class UImage;
class UTextBlock;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIStatToolTipItem : public UUserWidget
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _rootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _statImage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _statName;


protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetStatGroupUIPanel(bool InActive);
	void SetStatName(const FText InStatName);
	void SetStatImage(const FString InImagePath);

};
