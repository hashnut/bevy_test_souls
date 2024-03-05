// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Quest/GsQuestInstance.h"
#include "T1Project.h"

class FGsQuest;
class FGsQuestPoolManager;

/**
 * 퀘스트 풀
 */
struct T1PROJECT_API FGsQuestPool
{
	friend class FGsQuestPoolManager;

private:
	TArray<FGsQuestInstance*> _pool;

private:
	// 생성 및 삽입
	template <typename T>
	T* Claim()
	{
		static_assert(TIsDerivedFrom<T, FGsQuestInstance>::IsDerived, "T must be derived from FGsQuestInstance");

		T* quest = nullptr;

		// 없으면 생성하고 반환
		if (0 == _pool.Num())
		{
			quest = new T;
			ensure(quest);
		}
		// 있으면 꺼내서 반환
		else
		{
			quest = static_cast<T*>(_pool.Pop());
		}

		return quest;
	}

	// 부분 반납
	void Release(FGsQuestInstance* inData)
	{
		_pool.Push(inData);
	}

	// 모두 삭제
	void Flush()
	{
		while (0 < _pool.Num())
		{
			FGsQuestInstance* p = _pool.Pop();
			SAFE_DELETE(p);
		}
	}
};

/**
 *  퀘스트 풀 매니저
 */

class T1PROJECT_API FGsQuestPoolManager
{
private:
	TMap<QuestType, FGsQuestPool*> _poolMap;

public:
	void Initialize();
	void Finalize();

public:
	// QuestInstance를 QuestType에 맞는 Pool로 릴리즈
	void ReleaseInstance(const QuestType inType, FGsQuestInstance* inData);

	// QuestType에 맞는 Pool에서 QuestInstance를 Claim
	FGsQuestInstance* ClaimInstance(const QuestType inType);

public:
	// 타입별로 풀 따로 관리하기 때문에 검색 필요
	FGsQuestPool* FindPool(const QuestType inType);
private:
	// Pool 생성 후 PoolMap에 등록
	FGsQuestPool* CreatePoolInternal(const QuestType inType);
};
