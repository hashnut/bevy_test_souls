// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Message/GsMessageUserInfo.h"

#include "GsUICurrencyList.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHideCurrencyList);

/**
 * 보유한 재화 정보를 표시하는 목록
 */
class UWrapBox;
class UGsUICurrencySlot;
class UGsDynamicPanelSlotHelper;
class UGsUICurrencyDetailInfo;
struct FGsSchemaCurrencyData;
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUICurrencyList : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUICurrencyList")
	TSubclassOf<UGsUICurrencySlot> _currencyEntryClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUICurrencyList", meta = (BindWidget))
	UWrapBox* _panelCurrency;

	UPROPERTY(BlueprintReadOnly, Category = "GsUICurrencyList", meta = (BindWidget))
	UPanelWidget* _panelWarning;

	UPROPERTY()
	UGsUICurrencyDetailInfo* _currencyInfo;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _currencySlotHelper;

public:
	FOnHideCurrencyList OnHide;

protected:
	TArray<TPair<MessageUserInfo, FDelegateHandle>> _handlerUserInfoList;

	TArray<const FGsSchemaCurrencyData*> _currencyList;

	bool _bIsCurrencyInfoOpen = false;
	uint64 _openedCurrencyInfoType;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

public:
	void Show();
	void Hide();
	void InvalidateCurrencyList();

	void SetCurrencyInfo(UGsUICurrencyDetailInfo* InCurrencyInfo);

protected:
	void OnInvalidateCurrency(uint64 InType);

	UFUNCTION()
	void OnCreateCurrencyEntry(UWidget* InEntry);
	UFUNCTION()
	void OnRefreshCurrencyEntry(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickCurrencyEntry(UWidget* InEntry);

	void OnHideCurrencyInfo();

};
