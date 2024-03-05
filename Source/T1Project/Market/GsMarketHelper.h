// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "DataSchema/GsSchemaEnums.h"

struct FGsSchemaMarketConfig;
struct FGsSchemaMarketFilterSetting;
struct FGsSchemaMarketCategoryData;

/**
 * 거래소 편의 함수 모음
 */
struct T1PROJECT_API FGsMarketHelper final
{
public:
	static const FGsSchemaMarketConfig* GetMarketConfig();
	static const FGsSchemaMarketFilterSetting* GetMarketFilterSetting(); // 필요없으면 다른곳으로 옮겨야지..
	static const FGsSchemaMarketCategoryData* GetMarketCategoryData(EGsMarketUICategory InCategory);
	static bool GetSellRemainTime(bool bIsWorldMarket, const time_t& InSellStartTime, OUT FTimespan& OutRemainTime);

	// bIsFractionalDigits: T: 소수점 표기, F: 소수점 없음 표기. 소수점 있을 경우 셋째자리에서 올림해서 둘째자리까지 표기
	static void GetMarketPriceText(double InPrice, bool bIsFractionalDigits, OUT FText& OutText);

	// 구매나 수령 전 인벤토리 상태 체크 로직
	static bool CheckInventoryState(int32 InWeight, bool bUseMessage);

	// U1: 갯수 하드코딩. 추후 필요하다고 하면 테이블로 뺄 것
	static int32 GetMaxMultiplePurchaseCount() { return 5; }

	static ItemId GetWorldMarketRegistrationFeeItemId();
	static ItemGrade GetWorldMarketMinimumItemGrade();
	static ItemGrade GetWorldMarketShortcutItemGrade();

	// 콜렉션에서 아이템 바로가기(다른 강화도 조회 결과에 따라 서버/월드 마켓 중 어디로 갈지 결정)
	static void OpenMarketFromCollection(CollectionId InCollectionId, ItemId InItemId);
};
