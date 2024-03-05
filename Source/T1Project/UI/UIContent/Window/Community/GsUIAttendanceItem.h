// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIAttendanceItem.generated.h"

class UWidgetSwitcher;
class UGsUIIconItemReward;
struct FGsSchemaAttendanceReward;
class UTextBlock;
class UGsUIIconItem;
class USizeBox;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIAttendanceItem : public UUserWidget
{
	GENERATED_BODY()
	
private:
	static const int8 Special_Day_Cycle = 7;

protected:
	DECLARE_DELEGATE_TwoParams(FOnClickSlot, int, class UWidget*);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UGsHorizontalBoxIconSelector* _iconSelector;
	
	// 바닥 색상 (bg)
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UWidgetSwitcher* _bgSwitcher;

	// 날짜 색상 (bg)
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UWidgetSwitcher* _dayNumBgSwitcher;

	// 슬롯 크기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	USizeBox* _sizebox;

	// dayNumText
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textDayNum;

	UPROPERTY()
	int _slotIndex = 0;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textNormalDay;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _text7Day;


public:
	FOnClickSlot _delegateClickSlot;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	void SetData(int inIndex, bool inIsRewarded, bool inIsAcceptable, const FGsSchemaAttendanceReward* inRewardData);
	
	void SetIsSelected(bool inIsSelected);
	void SetIsRewarded(bool inIsRewarded);

protected:
	UGsUIIconItemReward* GetIcon();
	void OnClickIconSlot(class UGsUIIconItem& InIcon);
	void OnLongPressIcon(class UGsUIIconItem& InIcon);

public:
	float GetHeightSize();
};
