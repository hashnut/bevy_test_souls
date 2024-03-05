// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Currency/GsCostPackage.h"

#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Item/GsSchemaItemExpiredTime.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "Management/ScopeGame/GsValidatorChainManager.h"
#include "Management/ScopeGame/GsValidatorChainManager.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"


struct FGsSchemaPassivitySet;
/**
 * 아이템 관련 편의 함수 모음
 */
struct T1PROJECT_API FGsItemHelper final
{
private:
	/** 아이템 상세 팝업의 '지속' 부분 텍스트 파싱 */
	static TArray<TPair<int32, int32>> GetEffectIdTimePairList(const FGsItem* InItem);

public:
	static bool GetItemGradeText(ItemGrade InGrade, OUT FText& OutText);
	static bool GetEnchantBonusText(EGsItemEnchantBonusType InType, OUT FText& OutText);

	static int32 GetLocalPlayerMaxWeight();
	static float GetWeightPercent(int32 InWeight);
	static bool GetWeightPercentText(float InWeightPercent, OUT FText& OutPercentText, bool InShowPoint = true);

	static bool IsPickOneTypeRewardItem(const FGsItem* InItem);

	/** 아이템 상세 팝업의 '지속' 부분 텍스트 파싱 */
	static FText GetDurationText(const FGsItem* InItem);

	/** 아이템 상세 팝업의 '효과' 부분 텍스트 파싱 */
	static FText GetSupplyEffectText(const FGsItem* InItem);

	/* 아이템 쿨타임 설정 */
	static void SetPeriodItemCoolTime(const FGsItem* InItem);

	/* 마법 부여 효과 정보 */
	static const FGsSchemaPassivitySet* GetMagicalForgePassivitySetAndGrade(MagicalForgeData InMagicalForgeData, OUT ItemGrade& OutGrade);

	static bool IsMatchedMagicalForge(const ItemDBId InTargetItemDBId, const ItemDBId InIngredientItemDBId);
	static bool CanMagicalForge(const ItemDBId InTargetItemDBId, const ItemDBId InIngredientItemDBId, bool InIsShowTicker, OUT bool& OutIsSameEffect);
};
