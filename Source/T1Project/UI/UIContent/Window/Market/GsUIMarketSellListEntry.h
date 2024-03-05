// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Window/Market/GsUIMarketItemListEntryBase.h"
#include "GsUIMarketSellListEntry.generated.h"

class UTextBlock;
class UGsButton;
struct FGsMarketItemData;
class UGsSwitcherButton;
/**
 * 판매등록 페이지 리스트 UI
 */
UCLASS()
class UGsUIMarketSellListEntry : public UGsUIMarketItemListEntryBase
{
	GENERATED_BODY()

public:
	enum EGsSellStateType
	{
		Waiting, // 판매 대기중
		OnSale,	// 판매중
		SaleFailed, // 판매 실패
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockStat;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPricePerUnit;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPrice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockStateText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnCancel;

protected:
	const float REFRESH_TIME_DURATION_SEC = 30.f;
	MarketTransactionId _transactionId = 0;
	time_t _sellStartTime = 0;
	EGsSellStateType _currState = EGsSellStateType::Waiting;
	float _stateCheckRemainTime = 0.f;
	bool _bIsEquipType = false;
	int32 _itemWeight = 0;
	bool _bIsStackable = false;
	bool _bIsWorldMarket = false;
	time_t _registeredServerTime = 0;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void OnLongPressIcon(class UGsUIIconItem& InIcon) override;

public:
	void SetData(bool bIsWorldMarket, FGsMarketItemData* InData);

protected:
	UFUNCTION()
	void OnClickCancel();

protected:
	void UpdateState();
};
