// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIMarketPageBase.generated.h"

/**
 * 거래소 페이지 기본 클래스
 */
UCLASS(Abstract)
class UGsUIMarketPageBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	bool _bIsWorldMarket = false;
	bool IsWorldMarket() const { return _bIsWorldMarket; }

public:
	virtual void Show(bool bIsWorldMarket);
	virtual void Hide() {}

public:
	virtual void OnUpdateItemList() {}
	virtual void OnUpdateBookmark(ItemId InItemId) {}
	virtual void OnRcvSellRegisterdItem(MarketTransactionId InTransactionId) {}
	virtual void OnRcvSellCancelItem(MarketTransactionId InTransactionId) {}
	virtual void OnRcvBuySuccess(MarketTransactionId InTransactionId) {}
	virtual void OnRcvBuyFail() {}
	virtual void OnUpdateSearch(const FString& InStr) {}
	virtual void OnUpdateFilter(bool bIsGradeChanged) {}
	virtual void OnRcvItemDetailInfo(MarketTransactionId InTransactionId) {}
	virtual void OnRcvSearchResult() {}
	virtual void OnReqRefresh() {}
	virtual void OnRcvBookmarkList() {}
};
