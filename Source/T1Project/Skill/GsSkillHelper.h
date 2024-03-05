// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"

class FGsSkill;
struct FGsSchemaSkillUpCostSet;
struct FGsSchemaUserSkillSet;
struct FGsSchemaSkillLevelSet;
struct FGsSchemaPassivitySet;

/**
 * 스킬 관련 도움 함수 추가
 */
class T1PROJECT_API FGsSkillHelper
{
public:
	static TWeakPtr<const FGsSkill> GetSkillByUserSkillSetId(int32 InUserSkillSetId, CreatureGenderType InGender);
	static TWeakPtr<const FGsSkill> GetSkillByUserSkillSet(const FGsSchemaUserSkillSet* InRow, CreatureGenderType InGender);

	static SkillId GetSkillIdByUserSkillSetId(int32 InUserSkillSetId, CreatureGenderType InGender);

	static bool IsLearnedSkill(int32 InUserSkillSetId);
	static bool IsLearnedSkill(const FGsSkill* InSkill);

	static const FGsSchemaSkillUpCostSet* GetSkillUpCostSet(int InSkillUpCostId);

	// 레드닷 체크 시 재화 변화까지 체크하긴 어려우므로 재화 체크여부를 따로 설정한다
	static bool CheckSkillUpCost(int32 InSkillUpCostId, bool bCheckCurrency, bool bShowMsg = false);
	static bool CanLearn(const FGsSkill* InSkill, bool bShowMsg = false);
	static bool CanReinforce(const FGsSkill* InSkill, bool bCheckCurrency);
	static bool CheckSkillRedDot(const FGsSkill* InSkill);
	static bool CanUseSkillBook(int32 InUserSkillSetId, bool bShowMsg = false);

	// R3 스킬 업그레이드 개선
	static void GetSkillListByUpgradeStep(const FGsSkill* InSkill, OUT TArray<TWeakPtr<const FGsSkill>>& OutList);
	static const FGsSchemaPassivitySet* GetUpgradeSkillRequirePassivity(const FGsSchemaSkillLevelSet* InLevelSet);

	// 업그레이드 스킬이 아닐경우 null 리턴
	static TWeakPtr<const FGsSkill> GetUpgradeRootSkill(const FGsSkill* InSkill);
	static SkillId GetUpgradeRootSkillId(const FGsSkill* InSkill);

	static bool GetSkillSimpleInfoByUserSkillSetId(int32 InUserSkillSetId, 
		OUT FText& OutSkillName, OUT FText& OutSkillDesc, OUT ItemGrade& OutGrade);

	static CostumeId GetCostumeIdBySkillId(SkillId InSkillId);

	// 업그레이드 3단계의 경우 정보가 없으므로 이전 업그레이드 단계에 있는 스킬 중 배운게 있나 확인
	static bool HasRootSkill(const FGsSkill* InRootSkill, int32 InCurrStep);

	// 패시비티에 연결된 업그레이드 스킬 찾기
	static TWeakPtr<const FGsSkill> GetUpgradeSkillByPassivitySkill(const FGsSkill* InSkill);

private:
	// 패시비티에 연결된 업그레이드 스킬의 루트 스킬 찾기
	static TWeakPtr<const FGsSkill> GetUpgradeRootSkillByPassivitySkill(const FGsSkill* InSkill);
	
	// 패시비티에 연결된 업그레이드 스킬의 단계 찾기
	static int32 GetUpgradeSkillStepByPassivitySkill(const FGsSkill* InSkill);
};
