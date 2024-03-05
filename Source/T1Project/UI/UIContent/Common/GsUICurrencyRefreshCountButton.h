// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "GsUICurrencyRefreshCountButton.generated.h"

class UGsButton;
class UTextBlock;
class UImage;
class UWidgetSwitcher;

/**
 * 새로고침 버튼(재화 소진)
 */
UCLASS()
class T1PROJECT_API UGsUICurrencyRefreshCountButton : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnClickRefreshButton);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSlot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRemainCount;

	// 0: 불가, 1: 가능
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherState;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockAmount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIcon;

protected:
	CurrencyType _currencyType = CurrencyType::NONE;
	uint64 _amount = 0;
	int32 _remainCount = 0;

public:
	FOnClickRefreshButton OnClickRefreshButton;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetData(int32 InRemainCount, CurrencyType InType, uint64 InAmount);
	
	void SetRemainCount(int32 InRemainCount);	
	void SetEnableButton(bool bInEnable);

	void SetAmount(uint64 InAmount);
	void SetAmountTextColor(EGsUIColorType InColorType);

	// 유저 재화 보유량 비교해서 텍스트 컬러 세팅
	void UpdateAmountTextColorLocalPlayer();

public:
	CurrencyType GetCurrencyType() const { return _currencyType; }
	uint64 GetAmount() const { return _amount; }
	int32 GetRemainCount() const { return _remainCount; }
	
protected:
	UFUNCTION()
	void OnClickSlot();
};
