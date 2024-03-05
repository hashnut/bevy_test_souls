// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsMarketItemGroupListItem.generated.h"

struct FGsMarketItemGroupData;

/**
 * 거래소 검색탭 리스트뷰의 1, 2뎁스 데이터(아이템 그룹 정보)
 */
UCLASS()
class T1PROJECT_API UGsMarketItemGroupListItem : public UObject
{
	GENERATED_BODY()

public:
	const FGsMarketItemGroupData* _data = nullptr;
	bool _bIsSecondDepth = false; //
	int32 _enchantLevel = 0; // 2뎁스 일때만
	bool _bIsWorldMarket = false;

public:
	void Clear();
	void SetData(bool bIsWorldMarket, const FGsMarketItemGroupData* InData, bool bIsSecondDepth = false, int32 InEnchantLevel = 0);

	MarketPrice GetLowestPrice() const;
	int32 GetSalesCount() const;
	bool IsSalesItemExist() const;
};
