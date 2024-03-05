// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GsAutoTargetMode.h"

// 스킬 버튼 사용으로 인한 타겟 선정
// 기본 타겟룰은 AutoMode 속성을 상속 받음
class T1PROJECT_API FGsSemiAutoTargetMode : public FGsAutoTargetMode
{
	using Super = FGsAutoTargetMode;

public:
	FGsSemiAutoTargetMode() = default;
	virtual ~FGsSemiAutoTargetMode() = default;

public:
	virtual void SearchDataInitailize() override;
	virtual void SearchTarget(OUT TArray<class UGsGameObjectBase*>& OutResult) override;
};
