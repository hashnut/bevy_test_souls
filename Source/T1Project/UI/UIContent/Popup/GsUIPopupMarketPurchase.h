// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageContents.h"
#include "GsUIPopupMarketPurchase.generated.h"

class UTextBlock;
class UWidgetSwitcher;
class UGsButton;
class UGsSwitcherButton;
struct FGsMarketItemData;
struct IGsMessageParam;

/**
 * 거래소 아이템 구매창
 */
UCLASS()
class UGsUIPopupMarketPurchase : public UGsUIPopup
{
	GENERATED_BODY()

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
	UTextBlock* _textBlockPrice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPricePerUnit;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnSwap;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnCompare; // 0: disable, 1: enable
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPurchase;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

protected:
	TArray<MsgHandleMarket> _msgHandleMarketList;
	const FGsMarketItemData* _itemData = nullptr;
	MarketTransactionId _transactionId = INVALID_MARKET_TRANSACTION_ID;
	bool _bIsWorldMarket = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

protected:
	void BindMessages();
	void UnbindMessages();
	
public:
	void SetData(bool bIsWorldMarket, const FGsMarketItemData* InData);

protected:
	void SetUISwapInfoPage(int32 InIndex);
	bool CanCompare(const class FGsItem* InItem);

protected:
	void OnReceiveItemPriceInfo(const IGsMessageParam*);
	
protected:
	UFUNCTION()
	void OnClickSwap();
	UFUNCTION()
	void OnClickCompare();	
	UFUNCTION()
	void OnClickPurchase();
	UFUNCTION()
	void OnClickClose();
};
