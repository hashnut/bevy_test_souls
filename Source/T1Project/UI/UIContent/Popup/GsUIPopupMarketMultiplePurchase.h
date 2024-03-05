// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageContents.h"
#include "GsUIPopupMarketMultiplePurchase.generated.h"

class UScrollBox;
class UPanelWidget;
class UTextBlock;
class UGsButton;
class UGsSwitcherButton;
class UGsDynamicPanelSlotHelper;
class UGsUIMarketPriceInfo;
struct FGsMarketItemData;
struct IGsMessageParam;

/**
 * 거래소에서 여러개 동시 구매 시 사용(거래소에서 사용)
 */
UCLASS()
class UGsUIPopupMarketMultiplePurchase : public UGsUIPopup
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
	UGsUIMarketPriceInfo* _priceInfoUI;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTotalPrice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMyAmount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLowCurrency;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPurchase;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	FGsToggleGroup _toggleGroupItem;

	TArray<MsgHandleMarket> _msgHandleMarketList;
	TArray<const FGsMarketItemData*> _dataList;
	TArray<MarketTransactionId> _selectedList;
	bool _bIsWorldMarket = false;
	
public:
	virtual void BeginDestroy() override;

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
	void SetData(bool bIsWorldMarket, TArray<MarketTransactionId>& InIdList);

protected:
	void InvalidateList();
	void InvalidateTotalPrice();

protected:	
	void OnReceiveItemPriceInfo(const IGsMessageParam*);

protected:
	void OnSelectItem(int32 InIndex);	

	UFUNCTION()
	void OnCreateEntry(UWidget* InEntry);
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);

	void OnClickCheckBox(MarketTransactionId InTransactionId, bool bIsChecked);

	UFUNCTION()
	void OnClickPurchase();
	UFUNCTION()
	void OnClickClose();

protected:
	const FGsMarketItemData* GetSelectedItem() const;
};
