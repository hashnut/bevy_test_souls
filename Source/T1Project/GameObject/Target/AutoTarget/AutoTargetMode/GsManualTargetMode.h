// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GsTargetModeBase.h"

// 수동 타겟 모드(수동 이동 + 수동 스킬버튼)
class T1PROJECT_API FGsManualTargetMode : public FGsTargetModeBase
{
	using Super = FGsTargetModeBase;

protected:
	class FGsGameObjectData* _localData = nullptr;

	// 우선 탐색 반경
	float _firstSearchRange = 0.f;

public:
	FGsManualTargetMode() = default;
	virtual ~FGsManualTargetMode() = default;

public:
	virtual void ApplyTargetList() override;
	virtual void SearchTarget(OUT TArray<class UGsGameObjectBase*>& OutResult) override;

	virtual void SearchDataInitailize() override;

	virtual bool IsNotTarget(UGsGameObjectBase* In_object) override;

	// 검색 함수 처리
protected:
	virtual void CallSearchFunction(OUT TArray<class UGsGameObjectBase*>& OutResult) override;

	// 경계 대상(원수) 찾기
	void FuncSearchEnemyUserTarget(OUT TArray<UGsGameObjectBase*>& OutResult, UGsGameObjectBase* GameObject);
};
