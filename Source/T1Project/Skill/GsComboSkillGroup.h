// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"

/**
 * 콤보용 스킬 제어 클래스
 * 현재는 체인 스킬에서만 활용하고 추후 Combo(현재 평타 스킬) 스킬도 활용 가능하게 구성
 */
class T1PROJECT_API FGsComboSkillGroup
{	
private:
	// 현재 활성중인 단계
	int _currentIndex = 0;
	TArray<const class FGsSkill*> _listSkillData;
	
	// Update ResetTime
	FTimerHandle _resetTimerHandle;
	
	TFunction<void(FGsComboSkillGroup*)> _callbackChainSkillEnd;
public:
	~FGsComboSkillGroup();

public:
	void Init(TFunction<void(FGsComboSkillGroup*)> CallbackSkillEnd);

	void Add(const class FGsSkill* SkillData);

	// 처음으로 되돌아감
	void Reset();

	// 다음 스텝으로 치환
	const class FGsSkill* Next(bool IgnoreRestoreTime = false);

private:
	void StartTimer(float ResetTime);

public:
	const class FGsSkill* GetCurrentData() const;
	const class FGsSkill* GetFirstSkill() const;
	TArray<const class FGsSkill*> GetAllData() const;
	bool Contain(int32 SkillId) const;
	bool Contain(const class FGsSkill* SkillData) const;

	bool IsFirst() const;
	// SkillChainCondition::PREV_SKILL_REMAIN 스킬 검사
	bool IsRemainSkill() const;
};

