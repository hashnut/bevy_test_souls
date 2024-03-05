#pragma once

#include "DataSchema/GsSchemaEnums.h"

//------------------------------------------
// condition 정보
//------------------------------------------

class FGsAICondition
{
public:
	EGsAIConditionType _conditionType;
	TFunction<bool()> _func;
	bool _boolVal;
};

class FGsAIAction
{
public:
	EGsAIActionType _type;
};
class FGsAITask
{

	// 멤버 변수
public:
	TArray<FGsAICondition*> _reserveConditions;
	TArray<FGsAICondition*> _actionConditions;
	TArray<FGsAICondition*> _clearConditions;
	FGsAIAction* _action;
	// 구별 id
	int id;		
	// 주석
	FString _annotation;

	int _reserveCount;

	bool _isHighPriority;

	// 소멸자
public:
	virtual ~FGsAITask() = default;
	// 로직함수
public:
	// 초기화
	void Initialize();
	// 정리
	void Finalize();
};
//--------------------------------------------
// ai 데이터
//--------------------------------------------
class FGsAITaskResolver
{
	// 멤버 변수
public:
	// 컨디션들
	TArray<FGsAITask*> _tasks;
	// 소멸자
public:
	virtual ~FGsAITaskResolver() = default;

	// 로직 함수
public:
	// 초기화
	void Initialize();
	// 정리
	void Finalize();
	
};
