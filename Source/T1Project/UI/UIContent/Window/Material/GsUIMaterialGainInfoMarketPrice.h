// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIMaterialGainInfoMarketPrice.generated.h"

class UTextBlock;
class UGsUIMarketPriceInfo;
class UGsSwitcherButton;
class UWidgetSwitcher;
struct FGsMarketCollectionData;
struct IGsMessageParam;

/**
 * 컬렉션 아이템 거래소 시세정보
 */
UCLASS()
class UGsUIMaterialGainInfoMarketPrice : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherTradable;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMarketPriceInfo* _uiPriceInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnPurchase;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockState;

protected:
	ItemId _itemId = INVALID_ITEM_ID;
	Level _enchantLevel = INVALID_LEVEL;
	CollectionId _collectionId = INVALID_COLLECTION_ID;

	bool _isRequestLowestPrice = false;
	bool _isRequestWorldMarket = false;

protected:
	virtual void NativeOnInitialized() override;

public:	
	void SetData(CollectionId InCollectionId, ItemId InMaterialId, Level InLevel);

protected:
	void SendRequestLowestPrice();
	void SendRequestPrice(bool bIsWorldMarket);

protected:
	void CheckAndSendRequestPrice(FGsMarketCollectionData* InCollectionDatas);

	void SetUITradable(bool bIsTradable);
	bool IsTradable(ItemId InItemId) const;
	void OpenPurchasePopup();	

protected:
	UFUNCTION()
	void OnClickPurchase();

public:
	void OnReceiveCollectionPriceList(const IGsMessageParam* InParam);
	void OnReceiveItemPriceInfo(const IGsMessageParam*);	
	void OnRequestRefreshPrice(const IGsMessageParam*);
};
