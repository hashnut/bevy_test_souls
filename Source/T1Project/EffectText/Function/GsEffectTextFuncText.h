// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Client/SharedEnums/SharedSkillNotifyEnum.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"

/**
 * 텍스트를 가져오거나 완성해주는 함수 모음
 */
class FGsEffectTextFuncText final
{
public:
	FGsEffectTextFuncText() = default;
	~FGsEffectTextFuncText() {}

public:
	static void GetAbnormalityNameText(AbnormalityEffectType InType, OUT FText& OutData, int32 InOption = 0);
	static void GetDamageStatTypeText(DamageStatType InType, OUT FText& OutText);
	static void GetElementalTypeText(DamageElementalType InType, OUT FText& OutText);
	static void GetSkillTargetTypeText(SkillTargetType InType, OUT FText& OutText);
	static void GetHealRatioValueTypeText(HealRatioValueType InType, OUT FText& OutText);

	static bool GetAbnormalityApplyType(AbnormalityApplyType InType, OUT FText& OutText);
	static bool GetPassivityCauseText(PassivityCauseType InType, OUT FText& OutText);

	static void GetItemCategoryText(ItemCategorySub InType, OUT FText& OutText);
	static void GetCreatureWeaponText(CreatureWeaponType InType, OUT FText& OutText);

	// {0}% 형태로 리턴. bIsBasisPoint: 만분율인지 여부
	static void GetValueRatioText(int32 InValue, bool bIsBasisPoint, OUT FText& OutText);

	static void GetAbnormalityIdNameText(int32 InAbnormalityId, OUT FText& OutText);
	static void GetPassivityIdNameText(int32 InPassivityId, OUT FText& OutText);

	// 살기(SENSITIVE)가 FGsSchemaStatFormatingData 테이블에서 빠져있는데, 빠진 이유가 불명확해서 일단 이렇게 구현
	static void GetStatNameText(StatType InStatType, OUT FText& OutText);
};
