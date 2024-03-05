// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupCalculator.generated.h"


class UTextBlock;
class UGsButton;
class UWidget;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFinishCalculateDelegate, int64);
DECLARE_MULTICAST_DELEGATE(FOnCancelCalculateDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNativeDestructDelegate);
DECLARE_MULTICAST_DELEGATE(FOnResetCalculateDelegate);
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupCalculator : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _dimmed;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _numText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _titleText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMinus;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPlus;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNumPad1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNumPad2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNumPad3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNumPad4;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNumPad5;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNumPad6;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNumPad7;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNumPad8;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNumPad9;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNumPad0;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClear;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEraseBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAdd10;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAdd100;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAdd1000;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAddMax;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMax70;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMax90;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockFirst;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSecond;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

private:
	FString _numString;
	int64 _num = 0;
	int64 _maximumCount = 0;
	int64 _minimumCount = 0;
	bool _touched = true; // false 일 경우 입력된 값을 초기화한 뒤 입력처리

	int32 _maxWeight;		// 가질 수 있는 최대 무게
	int32 _currentWeight;	// 현재 무게
	int32 _itemWeight;		// 선택된 아이템 무게
	bool _isAddMode = true;	// 추가(구매, 찾기) or 제거(판매, 보관) 구분

	int32 _weightPenalty1Phase = 0;
	int32 _weightPenalty2Phase = 0;


private:
	// T: OK, F: Cancel
	TFunction<void(bool)> _buttonCallback;


public:
	// finish callback
	FOnFinishCalculateDelegate OnFinishCalculateDelegate;
	FOnCancelCalculateDelegate OnCancelCalculateDelegate;
	FOnNativeDestructDelegate OnNativeDestructDelegate;
	FOnResetCalculateDelegate OnResetCalculateDelegate;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	void SetDimmed(const bool InDimmed);
	void SetMinimumCount(int64 InMinimumCnt);
	void SetMaximumCount(int64 InMaximumCnt);
	void SetDefaultCount(int64 InDefaultCnt);
	void SetNpcShopCalcInfo(bool IsBuyMode);
	void SetTitleText(FText InText);
	void SetResetCount();
	void InvalidateNumText();
	void SetTouchState(bool InState);
	void SetWeight(int32 InItemWeight, int32 InMaxWeight, int32 InCurrentWeight);
	void SetMode(bool InIsAddMode);

public:
	int64 GetCurrentNum() { return _num; }


public:
	void SetButtonCallBack(TFunction<void(bool)> InCallback);

private:
	void AppendNum(uint32 InAppendNum);
	void RemoveEnd();
	void AddedNum(int64 InAddedNum);

private:
	bool IsMaximum();
	bool IsMinimum();
	bool CheckMinMaxValue();

private:
	int64 GetMaximumCount(int32 InPenaltyValue, const FTextKey& InOverKey, const FTextKey& InUnderKey);

private:
	UFUNCTION()
	void OnClickMinus();
	UFUNCTION()
	void OnClickPlus();
	UFUNCTION()
	void OnClickNumPad1();
	UFUNCTION()
	void OnClickNumPad2();
	UFUNCTION()
	void OnClickNumPad3();
	UFUNCTION()
	void OnClickNumPad4();
	UFUNCTION()
	void OnClickNumPad5();
	UFUNCTION()
	void OnClickNumPad6();
	UFUNCTION()
	void OnClickNumPad7();
	UFUNCTION()
	void OnClickNumPad8();
	UFUNCTION()
	void OnClickNumPad9();
	UFUNCTION()
	void OnClickNumPad0();
	UFUNCTION()
	void OnClickClear();
	UFUNCTION()
	void OnClickRemove();
	UFUNCTION()
	void OnClickAdd10();
	UFUNCTION()
	void OnClickAdd100();
	UFUNCTION()
	void OnClickAdd1000();
	UFUNCTION()
	void OnClickAddMax();
	UFUNCTION()
	void OnClickOk();
	UFUNCTION()
	void OnClickCancel();
	UFUNCTION()
	void OnClickClose();
	UFUNCTION()
	void OnClickMaxFirst();
	UFUNCTION()
	void OnClickMaxSecond();


};
