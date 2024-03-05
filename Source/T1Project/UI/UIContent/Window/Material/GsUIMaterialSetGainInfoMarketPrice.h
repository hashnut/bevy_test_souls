// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UTIL/GsIntervalChecker.h"
#include "Item/Collection/EItemCollectionEnumData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIMaterialSetGainInfoMarketPrice.generated.h"

class UScrollBox;
class UPanelWidget;
class UTextBlock;
class UGsButton;
class UGsDynamicPanelSlotHelper;
class FGsItemCollectionField;
struct IGsMessageParam;
struct FGsMarketCollectionData;
struct FGsMarketCollectionItemData;
struct FGsMarketLowestPriceRequestInfo;

/**
 * 컬렉션 거래소 페이지
 */
UCLASS()
class UGsUIMaterialSetGainInfoMarketPrice : public UUserWidget
{
	GENERATED_BODY()

public:
	using PriceKeyPair = TPair<ItemId, Level>;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scroll;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootList;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	const FGsItemCollectionField* _collectionData = nullptr;
	FGsMarketCollectionData* _marketData = nullptr;

	FGsIntervalChecker _checkerRefresh;	

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	// 컬렉션 관련 데이터를 넘길 것
	void SetData(const FGsItemCollectionField* InData);	
	void RefreshPrice();

	void OnClickRefresh();
	void OnClickPurchase();

	bool IsExistMarketData() const;

public:
	// 가격 리스트 받은 후 메시지 연결할 것
	void OnReceiveMarketPriceList(const IGsMessageParam* InParam);
	void OnRequestRefreshPrice(const IGsMessageParam*);

protected:
	UFUNCTION()
	void OnCreateEntry(UWidget* InEntry);
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);

	void OnClickPurchaseListEntry(int32 InIndex);

protected:
	void InvalidateList();
	void SendReqestLowestPrice();
	void OpenPurchasePopup(TArray<TPair<ItemId, Level>>& InData);
	bool IsActiveCollect(ECollectionMaterialActionType InType) const;
};
