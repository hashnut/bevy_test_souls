// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Market/GsUIMarketItemListEntryBase.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GsUIMarketLogListEntry.generated.h"

class UTextBlock;
struct FGsMarketSettlementData;

/**
 * 정산 내역 리스트 UI
 */
UCLASS()
class UGsUIMarketLogListEntry : 
	public UGsUIMarketItemListEntryBase,
	public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPrice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockEarnings;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSaleDate;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSaleTime;

protected:
	MarketTransactionId _transactionId;
	bool _bIsEquipType = false;

protected:
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	virtual void OnLongPressIcon(class UGsUIIconItem& InIcon) override;

public:
	virtual void RefreshAll() override;

protected:
	void InvalidateAll(class UGsMarketLogListItem* InListItem);
};
