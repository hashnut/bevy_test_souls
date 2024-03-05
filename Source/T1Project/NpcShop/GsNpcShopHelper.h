// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NpcShop/GsNpcShopEnum.h"
#include "NpcShop/GsNpcShopData.h"

#include "Item/GsItem.h"

#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


/**
 * 상점 관련 편의 함수 모음
 */
struct T1PROJECT_API FGsNpcShopHelper final
{
public:
	// 매매 제한 조건 확인
	static bool IsAllowedSellItem(const FGsItem* InItem, OUT EGsNotAllowedSellReaon& OutReaon);
	static bool IsAllowedBuyItem(const FGsNpcShopItemInfo* InItemInfo, OUT EGsNotAllowedBuyReason& OutReaon);

	static bool IsOverLimitLevel(const int32 InLimitLevel, OUT EGsNotAllowedBuyReason& OutReaon);
	static bool IsOverLimitGuildLevel(const GuildLevel InLimitGuildLevel, OUT EGsNotAllowedBuyReason& OutReaon);
	static bool IsOverLimitGuildMemberGrade(const GuildMemberGradeType InLimitGuildMemberGrade, OUT EGsNotAllowedBuyReason& OutReaon);
	static bool IsOverLimitPurchase(const FGsNpcShopItemInfo* InItemInfo, OUT EGsNotAllowedBuyReason& OutReaon);

	static void ShowTickerNotAllowedBuy(EGsNotAllowedBuyReason InReason);
	static void ShowTickerNotAllowedSell(EGsNotAllowedSellReaon InReason);

	// 자동 매매 옵션 저장
	static void SaveAutoBuySettingData(TMap<int32, ItemAmount>& InSettingDataMap);
	static void SaveAutoSellConsumeSettingData(TMap<int32, bool>& InSettingDataMap);
	static void SaveAutoSellEquipSettingData(TArray<TPair<bool, TPair<ItemCategoryMain, ItemGrade>>>& InSettingDataList);

	static void GetTextByRTT(ResetTimeTicket InRTT, OUT FText& OutText);
};
