// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GsTargetModeBase.h"
#include "AI/Data/GsAIDefine.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

class UGsGameObjectBase;

// 자동 사냥 타겟 모드
class T1PROJECT_API FGsAutoTargetMode : public FGsTargetModeBase
{
	using Super = FGsTargetModeBase;

protected:
	// 검색 반경 타입
	EGsAIAutoReturnStartPosType _currRetunType;

	// 퀘스트 자동 활성화 여부 확인
	bool _isQuestAutoProgressing = false;
	// 아레나 활성 여부 확인
	bool _isArena = false;

public:
	FGsAutoTargetMode() = default;
	virtual ~FGsAutoTargetMode() = default;

public:
	virtual void ApplyTargetList() override;

	virtual void SearchDataInitailize() override;
	virtual void SearchTarget(OUT TArray<class UGsGameObjectBase*>& OutResult);

	// 타겟 조건이 안되는지 체크 함수 추가
	virtual bool IsNotTarget(UGsGameObjectBase* In_object) override;

protected:
	virtual void CallSearchFunction(OUT TArray<class UGsGameObjectBase*>& OutResult) override;

	// 대상이 퀘스트 타입 (사냥형)
	virtual void FuncSearchQuestTarget(OUT TArray<class UGsGameObjectBase*>& OutResult,
		class UGsGameObjectBase* GameObject) override;

	// 특정 NpcFactionType을 가진 Npc 스킵 처리
	bool IsIgnoreNpcFactionType(IffFactionType IffType);

	// 특정 NpcFunctionType을 가진 Npc 스킵 처리
	bool IsIgnoreNpcFunctionType(NpcFunctionType functionType);
};
