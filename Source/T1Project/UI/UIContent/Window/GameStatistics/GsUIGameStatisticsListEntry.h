// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GsUIGameStatisticsListEntry.generated.h"

class UImage;
class UTextBlock;
class UWidgetSwitcher;
class UGsButton;
class FGsGameStatisticsData;

/**
 * 통계 리스트
 */
UCLASS()
class T1PROJECT_API UGsUIGameStatisticsListEntry : 
	public UUserWidget, 
	public IGsToggleGroupEntry, 
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSlot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSubCategory;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockThisWeek;
	
	// 0: 텍스트표시 1: "-" 표시
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherValue;

	// 0: 아래 화살표, 1: 위 화살표 
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherArrow;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLastWeek;

	// 0: 없음, 1: 있음
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherUseDaily;

protected:
	bool _bIsSelected = false;

public:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

public:
	void SetData(const FGsGameStatisticsData* InData);

private:
	void GetValueText(int64 InValue, EGsStatisticsValueType InType, OUT FText& OutText);
	void GetValueText(const FGsGameStatisticsData& InData, OUT FText& OutText);
	// T: 양수, F:음수
	bool GetCompareText(const FGsGameStatisticsData& InData, OUT FText& OutText);

private:
	UFUNCTION()
	void OnClickSlot();
};
