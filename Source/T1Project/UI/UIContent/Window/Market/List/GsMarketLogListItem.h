// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsMarketLogListItem.generated.h"

struct FGsMarketSettlementData;
/**
 * 거래소 정상내역 리스트뷰의 데이터
 */
UCLASS()
class T1PROJECT_API UGsMarketLogListItem : public UObject
{
	GENERATED_BODY()

public:
	const FGsMarketSettlementData* _data = nullptr;

public:
	void Clear();
	void SetData(const FGsMarketSettlementData* InData);
};
