// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Market/GsMarketItemData.h"
#include "GsMarketItemListItem.generated.h"


/**
 * 거래소 검색탭 리스트뷰의 3뎁스 데이터(아이템 정보)
 */
UCLASS()
class T1PROJECT_API UGsMarketItemListItem : public UObject
{
	GENERATED_BODY()

public:
	const FGsMarketItemData* _data;
	bool _bIsWorldMarket = false;

public:
	void Clear();
	void SetData(bool bIsWorldMarket, const FGsMarketItemData* InData);

};
