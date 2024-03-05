#pragma once

#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

/*
* ai reserve job
*/

class UGsGameObjectBase;
class FGsAICondition;

class FGsAIReserveJob
{
public:
	UGsGameObjectBase* _targetObj;
	EGsAIActionType _actionType;
	TArray<FGsAICondition*> _actionConditions;
	TArray<FGsAICondition*> _clearConditions;

	bool _isHighPriority = false;;

public:
	virtual ~FGsAIReserveJob() = default;
	
public:
	void ClearData();
};