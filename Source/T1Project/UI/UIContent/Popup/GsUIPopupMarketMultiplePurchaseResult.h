// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Message/GsMessageContents.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupMarketMultiplePurchaseResult.generated.h"

class UScrollBox;
class UPanelWidget;
class UTextBlock;
class UGsButton;
class UGsDynamicPanelSlotHelper;
struct FGsMarketItemData;
struct IGsMessageParam;

/**
 * 거래소에서 여러개 동시 구매한 결과(컬렉션에서 사용)
 * - 아이템 각각 결과가 내려옴에 유의
 */
UCLASS()
class UGsUIPopupMarketMultiplePurchaseResult : public UGsUIPopup
{
	GENERATED_BODY()

public:
	using MsgHandleMarket = TPair<MessageContentMarket, FDelegateHandle>;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scroll;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSuccessCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockFailCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTotalPay;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	TArray<MsgHandleMarket> _msgHandleMarketList;	
	
	TArray<MarketTransactionId> _idList;
	TMap<MarketTransactionId, MarketPrice> _priceMap;
	
	// 구매완료 시 지워질 수 있으므로 SetData시에만 활용되어야 한다
	TArray<const FGsMarketItemData*> _tempDataList;

	bool _bIsWorldMarket = false;
	
public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputCancel() override;

protected:
	void BindMessages();
	void UnbindMessages();

public:
	void SetData(bool bIsWorldMarket, TArray<MarketTransactionId>& InIdList);
	void InvalidatePurchaseResult();

protected:
	void OnRcvPurchaseResult(const IGsMessageParam* InParam);

protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnClickClose();
};
