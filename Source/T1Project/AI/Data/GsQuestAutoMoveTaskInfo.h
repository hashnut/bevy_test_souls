#pragma once
#include "CoreMinimal.h"
#include "AI/Data/GsQuestAutoMoveDefine.h"

/*
	task 데이터(테이블로 변경시 변경될 정보)
*/

struct FGsQuestAutoMoveConditionData
{
	EGsQuestAutoMoveConditionType _conditionType;
	bool _boolVal;
};

struct FGsQuestAutoMoveTaskData
{
	TArray<FGsQuestAutoMoveConditionData> _conditions;
	EGsQuestAutoMoveActionType _actionType;
};

/*
	퀘스트 자동 이동 테스크 정보
*/

class FGsQuestAutoMoveTask
{
public:
	// 조건
	class FGsQuestAutoMoveCondition
	{
	public:
		EGsQuestAutoMoveConditionType _conditionType;
		TFunction<bool()> _conditionFunc;
		bool _boolVal;
	};

	class FGsQuestAutoMoveAction
	{
	public:
		EGsQuestAutoMoveActionType _actionType;
		TFunction<void()> _actionFunc;
	};

	// 멤버 변수
public:
	TArray<FGsQuestAutoMoveCondition*> _conditions;
	FGsQuestAutoMoveAction* _action;

	// 소멸자
public:
	virtual ~FGsQuestAutoMoveTask() = default;

	// 로직 함수
public:
	// 초기화
	void Initialize();
	// 정리
	void Finalize();
};

/*
 테스크 모음
*/

class FGsQuestAutoMoveResolver
{
	// 멤버 변수
public:
	TArray<FGsQuestAutoMoveTask*> _tasks;

	// 소멸자
public:
	virtual ~FGsQuestAutoMoveResolver() = default;

	// 로직 함수
public:
	// 초기화
	void Initialize();
	// 정리
	void Finalize();
};