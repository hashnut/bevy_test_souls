// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/Containers/Array.h"
#include "Option/GsGameUserSettings.h"
#include "Function.h"

/**
 * 타겟룰 베이스 클래스 
 */
class T1PROJECT_API FGsTargetModeBase
{
protected:
	// 타겟 선정 검색 함수펑터 정의
	using DefSearchTargetFunc = TFunction< void(OUT TArray<class UGsGameObjectBase*>&, class UGsGameObjectBase*)>;

protected:
	// 타겟 선정 검색함수 및 결과 정보 구조체
	struct SearchTargetInfo
	{
		TArray<class UGsGameObjectBase*> _result;
		DefSearchTargetFunc _func;
		bool _isOnce = false;

		template <typename FuncType>
		void Add(FuncType&& Other, bool IsOnce = false)
		{
			_func = Other;
			_isOnce = IsOnce;
		}
	};

protected:
	class UGsGameObjectLocalPlayer* _local = nullptr;

	// 타겟 선정 검색 함수 등록 선입 선출(FIFO) 구조 이므로 추가하는 순서가 중요하다.
	TArray<SearchTargetInfo> _listSearchType;
	// 검색반경
	float _searchRange = 100.f;
	// 검색 중심 포지션
	FVector2D _searchCenterPos2D;

public:
	FGsTargetModeBase() = default;
	virtual ~FGsTargetModeBase() = default;

public:
	virtual void Initialize(class UGsGameObjectLocalPlayer* Local);

	virtual void ApplyTargetList();

	virtual void Enter() {};
	virtual void Exit() {};

	// 검색시 필요 정보 설정
	virtual void SearchDataInitailize();
	virtual void SearchTarget(OUT TArray<class UGsGameObjectBase*>& OutResult);

	// 타겟 조건이 안되는지 체크 함수 추가
	virtual bool IsNotTarget(class UGsGameObjectBase* In_object);

	// 검색 함수
protected:
	virtual void CallSearchFunction(OUT TArray<class UGsGameObjectBase*>& OutResult);

	// 나를 피격한 대상
	virtual void FuncSearchPcAttacker(OUT TArray<class UGsGameObjectBase*>& OutResult, class UGsGameObjectBase* GameObject);
	virtual void FuncSearchNpcAttacker(OUT TArray<class UGsGameObjectBase*>& OutResult, class UGsGameObjectBase* GameObject);
	
	// 대상이 퀘스트 타입 (사냥형)
	virtual void FuncSearchQuestTarget(OUT TArray<class UGsGameObjectBase*>& OutResult, class UGsGameObjectBase* GameObject);
	// 선공 몬스터
	virtual void FuncSearchFirstAttackNpcTarget(OUT TArray<class UGsGameObjectBase*>& OutResult, class UGsGameObjectBase* GameObject);
	// 거리 우선
	virtual void FuncSearchNearestEnemyTarget(OUT TArray<class UGsGameObjectBase*>& OutResult, class UGsGameObjectBase* GameObject);

};