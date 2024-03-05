// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Market/GsUIMarketPageBase.h"
#include "Classes/GsPoolUObject.h"
#include "GsUIMarketPageLog.generated.h"

class UTextBlock;
class UListView;
class UGsMarketLogListItem;
class FGsMarketListViewControlLog;

/**
 * 거래소 정상내역 페이지
 */
UCLASS()
class UGsUIMarketPageLog : public UGsUIMarketPageBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, EntryClass = UGsMarketLogListItem))
	UListView* _listView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEmptyList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTotalPrice;

protected:
	UPROPERTY()
	UGsPoolUObject* _poolListItem;

protected:
	FGsMarketListViewControlLog* _listViewControl = nullptr;
	bool _bIsFirst = false;
	bool _bNeedCheckCount = false;
	bool _bIsLocked = false;
	float _prevDistRemaining = 0.f;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void Show(bool bIsWorldMarket) override;
	virtual void Hide() override;
	virtual void OnUpdateItemList() override;
	virtual void OnRcvItemDetailInfo(MarketTransactionId InTransactionId) override;

protected:
	void InvalidateList(bool bResetScroll);

protected:
	UFUNCTION()
	void OnListViewScrolled(float InOffsetInItems, float InDistanceRemaining);
};
