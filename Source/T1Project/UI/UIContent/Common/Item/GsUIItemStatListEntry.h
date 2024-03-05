// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "GsUIItemStatListEntry.generated.h"

class UImage;
class UTextBlock;
class UWidgetSwitcher;
/**
 * 아이템 스탯출력
 */
UCLASS()
class T1PROJECT_API UGsUIItemStatListEntry: public UUserWidget
{
	GENERATED_BODY()

private:
	enum CompareExpectStatStateType : uint8
	{
		None = 0,
		Same,
		UpGrade,
		DownGrade,
		NewStat,
		RemoveStat,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockValue;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _compareTypeSwitcher; // 0 arrow , 1 same 2 new or remove stat
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _compareArrowSwitcher; // 0 down , 1 up
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _arrowDownImg;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _arrowUpImg;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _compareValueText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _onChangeStatText;

private:
	bool _isValidStat = false;;
	CompareExpectStatStateType _eCompareExpectStatType = CompareExpectStatStateType::None;
	
protected:
	virtual void NativeConstruct() override;
	
public:
	bool IsCompareStatTypeRemove() { return CompareExpectStatStateType::RemoveStat == _eCompareExpectStatType; }
	bool IsValidDislayStat();

public:
	void SetTextName(const FText& InText);
	void SetTextValue(const FText& InText);
	void SetColorAllText(const FLinearColor& InColor);
	void SetColorByCompareStateType(const bool InState);

public:
	void SetCompareObjectVisibility(const bool InState);
	void SetCompareArrowSwticher(const bool InIsUpArrow);
	void SetCompareValueText(const FText& InValueText, const FLinearColor& InColor);
	void SetSameValueStat();
	void SetCompareExptectNewOrRemoveStatText(const bool InIsNewStat, const FLinearColor& InColor);
};
