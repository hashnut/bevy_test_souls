// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIItemSetStatListEntry.generated.h"

class UTextBlock;
class UPanelWidget;
class UGsUIItemStatListEntry;
struct FGsItemStatData;

/**
 * 세트 스탯 정보
 */
UCLASS()
class T1PROJECT_API UGsUIItemSetStatListEntry: public UUserWidget
{
	GENERATED_BODY()
	
protected:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelStatListRoot;
	
protected:
	UPROPERTY(Transient)
	TArray<UGsUIItemStatListEntry*> _slotList;

protected:
	virtual void NativeOnInitialized() override;
	
public:
	void SetStatData(const TArray<FGsItemStatData>* InDataArray);
	void SetTextTitle(const FText& InText);
	void SetOnOff(bool bIsOn);
	void SetColorAllText(const FLinearColor& InColor);
};
