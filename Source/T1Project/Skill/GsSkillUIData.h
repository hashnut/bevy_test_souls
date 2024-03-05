// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

class FGsSkill;
struct FGsSchemaCostumeCommon;

/**
 * 스킬 편집 창의 스킬 데이터
 */
class T1PROJECT_API FGsSkillUIData final
{
private:
	TWeakPtr<const FGsSkill> _skillPtr;

public:
	SkillId _skillId = INVALID_SKILL_ID;
	CreatureWeaponType _weaponType = CreatureWeaponType::NONE;

	TArray<TWeakPtr<const FGsSkill>> _upgradeList; // 뿌리 스킬부터 전부 저장
	TArray<FGsSkillUIData> _chainList; // 체인 2부터 저장

	SkillId _firstChainSkillId = INVALID_SKILL_ID; // 체인 스킬의 첫 스킬 저장
	int32 _upgradeStepIndex = 0;
	
	// 코스튬 스킬일 경우
	const FGsSchemaCostumeCommon* _costume = nullptr;	

private:
	bool _bIsRedDot = false; // 습득가능 / 새로 얻음
	bool _bIsReinforce = false; // 강화가능

public:
	FGsSkillUIData() = default;

public:	
	void SetSkill(TWeakPtr<const FGsSkill> InSkill);
	void SetFirstChainSkill(SkillId InSkillId) { _firstChainSkillId = InSkillId; }
	void SetSkillCurrUpgradeStep();
	
	void UpdateRedDot();

	SkillId GetSkillId() const { return _skillId; }
	const FGsSkill* GetSkill();
	const FGsSkill* GetFirstChainSkill() const;
	const FGsSkill* GetUpgradeSkillByStep(int32 InStep) const;
	const FGsSkill* GetUpgradeRootSkill() const;
	
	bool IsRedDot() const { return _bIsRedDot; }
	bool IsReinforce() const { return _bIsReinforce; }
	bool IsUpgradeSkill() const;
	int32 GetUpgradeStep() const;

	// 코스튬 스킬이 고려된 항목
	bool IsCollectedCostume() const;
	bool IsCostumeSkill() const { return (_costume) ? true : false; }

	bool IsLearn() const;
};
