// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageContents.h"
#include "GsUIPopupCollectionMarketPurchase.generated.h"

class UScrollBox;
class UPanelWidget;
class UTextBlock;
class UGsButton;
class UGsSwitcherButton;
class UGsDynamicPanelSlotHelper;
class UGsUIMarketPriceInfo;
struct FGsMarketCollectionData;
struct FGsMarketCollectionItemData;
struct IGsMessageParam;

/**
 * 거래소에서 여러개 동시 구매 시 사용(컬렉션에서 사용)
 */
UCLASS()
class UGsUIPopupCollectionMarketPurchase : public UGsUIPopup
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
	UGsSwitcherButton* _sBtnPricePerUnit;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnPrice;

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
	FGsToggleGroup _toggleGroupPriceType;

	CollectionId _collectionId = INVALID_COLLECTION_ID;
	TArray<TPair<ItemId, Level>> _keyList;


	TArray<MsgHandleMarket> _msgHandleMarketList;
	TArray<const FGsMarketCollectionItemData*> _dataList;
	TArray<TPair<MarketTransactionId, bool>> _selectedList;

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
	void SetData(CollectionId InCollectionId, TArray<TPair<ItemId, Level>>& InKeyList);

protected:
	void InvalidateList();
	void InvalidateTotalPrice();

protected:
	void OnReceiveMarketPriceList(const IGsMessageParam* InParam);
	void OnReceiveItemPriceInfo(const IGsMessageParam*);

protected:
	void OnSelectItem(int32 InIndex);
	void OnClickPriceType(int32 InIndex, bool bIsSelected);

	UFUNCTION()
	void OnCreateEntry(UWidget* InEntry);
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);

	void OnClickCheckBox(MarketTransactionId InTransactionId, bool bIsWorldMarket, bool bIsChecked);

	UFUNCTION()
	void OnClickPurchase();
	UFUNCTION()
	void OnClickClose();

protected:
	const FGsMarketCollectionItemData* GetSelectedItem() const;
	bool IsPricePerUnit() const;
	bool IsSelectedItem(const FGsMarketCollectionItemData* InData);
};
