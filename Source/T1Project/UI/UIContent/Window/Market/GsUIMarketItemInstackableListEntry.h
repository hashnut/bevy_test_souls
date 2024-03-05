// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Market/GsUIMarketItemListEntryBase.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/UIContent/Window/Market/List/GsMarketItemListItem.h"
#include "GsUIMarketItemInstackableListEntry.generated.h"

struct FGsMarketItemData;
class UTextBlock;
class FGsItemEquipment;
class UGsBorder;
class UGsCheckBox;

/**
 * 거래소 쌓을 수 없는 아이템 표시. 대표적으로 장비.(아이콘, 이름 및 스탯, 제련옵션, 특성, 판매금액)
 */
UCLASS()
class UGsUIMarketItemInstackableListEntry : 
	public UGsUIMarketItemListEntryBase, 
	public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockStat;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRefineOption;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockShieldCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockProp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPrice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsBorder* _borderLongPress;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsCheckBox* _checkBoxSelect;

protected:
	MarketTransactionId _transactionId = INVALID_MARKET_TRANSACTION_ID;
	bool _bIsWorldMarket = false;
	TWeakObjectPtr<UGsMarketItemListItem> _listItemPtr;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual class FGsItem* GetItem() override;

public:
	virtual void RefreshAll() override;
	virtual bool IsSlotLongPressed() const override;

protected:
	void InvalidateAll(class UGsMarketItemListItem* InListItem);

	void SetUIEnchantProperty(FGsItemEquipment* InEquipItem);
	void SetUIRefineOption(FGsItemEquipment* InEquipItem);

protected:
	UFUNCTION()
	void OnFinishedLongPress();

	UFUNCTION()
	void OnClickCheckBox(int32 IndexInGroup, bool bIsChecked);
};
