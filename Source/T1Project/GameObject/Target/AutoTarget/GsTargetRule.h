// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/Templates/Function.h"
#include "Option/GsGameUserSettings.h"

/**
 * 타겟 선정 룰을 판단하는 클래스
 */
class T1PROJECT_API FGsTargetRule
{
public:
	// 타겟 선정 룰 타입
	enum EGsTargetRuleType
	{
		ManualTarget,	// 수동
		SemiAutoTarget, // 스킬 버튼만 수동 입력
		AutoTarget,		// 유저 조작이 전혀 없는 자동 사냥 활성 모드

		TargetRuleType_MAX,
	};

protected:
	//UGsGameObjectLocalPlayer* _local = nullptr;
	class FGsTargetModeBase* _targetRuleMode[(int)EGsTargetRuleType::TargetRuleType_MAX];
	// 위 Mode중 현재 활성화중인 Mode
	class FGsTargetModeBase* _currentMode = nullptr;

	// 특정 환경때문에 타겟에서 제외된 특수 케이스 정보
	uint64 _ignoreTargetGuid = 0;

public:
	FGsTargetRule() = default;
	~FGsTargetRule() = default;

public:
	void Initialize(class UGsGameObjectLocalPlayer* Local);
	void Finalize();

public:
	void ApplyTargetList();
	class UGsGameObjectBase* OnAutoTargetAction(EGsTargetRuleType TargetRuleType);
	void OnAutoTargetAction(OUT TArray<class UGsGameObjectBase*>& OutTargetList, EGsTargetRuleType TargetRuleType);

private:
	void ChangeMode(class FGsTargetModeBase* Mode);

public:
	void SetIgnoreTargetId(uint64 In_IgnoreObjectGuid);

	class FGsTargetModeBase* GetCurrentMode() const { return _currentMode; }
	class FGsAutoTargetMode* GetAutoMode() const;
	bool IsAutoMode() const;
};
