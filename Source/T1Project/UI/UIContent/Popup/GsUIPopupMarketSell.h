// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Message/GsMessageContents.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupMarketSell.generated.h"

class UTextBlock;
class UWidgetSwitcher;
class UGsButton;
class UGsToggleButton;
class UGsSwitcherButton;
struct IGsMessageParam;
struct FGsSchemaMarketConfig;

/**
 * 거래소 판매 등록 팝업
 */
UCLASS()
class UGsUIPopupMarketSell : public UGsUIPopup
{
	GENERATED_BODY()

public:
	enum EGSNumpadFocusedType
	{
		SELL_COUNT,
		PRICE,
	};

	struct FGsMarketConfigValue
	{
		int32 _registrationFee = 0;
		int32 _minPrice = 0;
		int32 _maxPrice = 0;
		int32 _maxRegistrationCount = 0;
		int32 _maxSettlementCount = 0;

		FGsMarketConfigValue() = default;

		void SetConfigValue(const FGsSchemaMarketConfig* InConfig, bool bIsWorldMarket);
		int32 CalcRegistrationFee(int32 InPrice, bool bIsWorldMarket);
	};

public:
	using MsgHandleMarket = TPair<MessageContentMarket, FDelegateHandle>;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIItemDefaultInfo* _defaultInfoUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIMarketPriceInfo* _priceInfoUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIItemDetailInfo* _detailInfoUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUINumpadDefault* _numPadUI;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _toggleSellCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _togglePrice;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSellCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPricePerUnit;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPrice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRegistrationFee;
	// 0: 일반(골드) / 1: 월드(젠)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherRegistrationFeeType;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTax;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTaxSub;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnSwap;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

protected:
	TArray<MsgHandleMarket> _msgHandleMarketList;

	UTextBlock* _textBlockFocused = nullptr;

	ItemDBId _itemDBId = INVALID_ITEM_DB_ID;
	ItemId _itemId = INVALID_ITEM_ID;
	ItemAmount _itemAmount = 0;

	FGsMarketConfigValue _configValue;
	ItemId _wdRegistrationFeeType = INVALID_ITEM_ID;

	bool _bIsWorldMarket = false;
	bool _bNeedCheckCloseTooltip = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

protected:
	void BindMessages();
	void UnbindMessages();
	
public:
	void SetData(bool bIsWorldMarket, ItemDBId InItemDBId);

protected:
	void SetUISwapInfoPage(int32 InIndex);
	void InvalidateSellInfo();	
	void SetFocusedNumpad(EGSNumpadFocusedType InType);
	int32 ConvertTextToNumber(const FText& InTextBlock);

protected:
	void OnReceiveItemPriceInfo(const IGsMessageParam*);

protected:
	void OnChangeNumpadNumber(int32 InNumber);

	UFUNCTION()
	void OnClickSwap();
	UFUNCTION()
	void OnClickSellCount();
	UFUNCTION()
	void OnClickPrice();
	UFUNCTION()
	void OnClickOk();
	UFUNCTION()
	void OnClickCancel();

protected:
	void ShowTicker(const FTextKey& InKey);

public:
	bool IsWorldMarket() const { return _bIsWorldMarket; }
	ItemDBId GetItemDBId() const { return _itemDBId; }
};
