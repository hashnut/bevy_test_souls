// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUINumpadDefault.generated.h"

class UTextBlock;
class UGsButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUINumpadDefault: public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnChangeNumber, int32);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNumber;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPlus;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMinus;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMax;

	// 숫자는 블루프린트에서 함수호출로 대체

protected:
	int32 _currNumber = 0;
	int32 _maxNumber = 0;
	bool _bFirstFocused = false;

protected:
	FOnChangeNumber _onChangeNumber;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetMaxNumber(int32 InMaxNumber);
	void SetNumber(int32 InNumber, bool InIsFirst = true);

protected:
	void SetNumberInter(int32 InNumber);

protected:
	// 숫자 버튼은 블루프린트에서 이 함수를 연결한다
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickNumber(int32 InNumber);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickNumberAdd(int32 InNumber);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickClear();

	UFUNCTION()
	void OnClickPlus();
	UFUNCTION()
	void OnClickMinus();
	UFUNCTION()
	void OnClickBack();
	UFUNCTION()
	void OnClickMax();

public:
	FOnChangeNumber& OnChangeNumber() { return _onChangeNumber; }
};
