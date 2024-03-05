// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Market/GsUIMarketItemListEntryBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GsUIMarketItemGroupListEntry.generated.h"

struct FGsMarketItemGroupData;
class UTextBlock;
class UGsToggleButton;
class UImage;
class UGsBorder;

/**
 * 거래소 아이템 그룹 리스트표시(즐겨찾기, 아이콘, 이름 및 설명, 최저 판매 단가, 판매 목록)
 */
UCLASS()
class UGsUIMarketItemGroupListEntry : 
	public UGsUIMarketItemListEntryBase, 
	public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _tBtnBookmark;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockStat;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLowestPrice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSalesCount;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgDimmed;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsBorder* _borderLongPress;

protected:
	ItemId _itemId = 0;
	bool _bIsExist = false;
	bool _bIsWorldMarket = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;	

public:
	virtual void RefreshAll() override;
	virtual bool IsSlotLongPressed() const override;

protected:
	void InvalidateAll(class UGsMarketItemGroupListItem* InListItem);

protected:
	// 즐겨찾기 클릭 시
	UFUNCTION()
	void OnClickBookmark();

	UFUNCTION()
	void OnFinishedLongPress();
};
