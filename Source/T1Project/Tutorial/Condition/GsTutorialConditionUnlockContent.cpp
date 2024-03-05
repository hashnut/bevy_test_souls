// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialConditionUnlockContent.h"
#include "DataSchema/Tutorial/GsSchemaTutorial.h"
#include "DataSchema/Tutorial/GsSchemaTutorialTargetId.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Management/ScopeGame/GsUnlockManager.h"


void FGsTutorialConditionUnlockContent::Initialize(const FGsSchemaTutorial* InTable)
{
	if (const FGsSchemaTutorialTargetId* conditionTable = InTable->conditionRow.GetRowByType<FGsSchemaTutorialTargetId>())
	{
		_unlockContentsId = conditionTable->targetId;
	}
	else
	{
		_unlockContentsId = 0;
	}
}

bool FGsTutorialConditionUnlockContent::CheckCondition(const IGsMessageParam* InParam)
{
	if (0 != _unlockContentsId)
	{
		if (const FGsPrimitivePairInt32* param = InParam->Cast<const FGsPrimitivePairInt32>())
		{
			if (param->_second == _unlockContentsId)
			{
				return true;
			}
		}
	}

	return false;
}

bool FGsTutorialConditionUnlockContent::IsValidToStart()
{
	if (0 < _unlockContentsId)
	{
		if (FGsUnlockManager* unlockMgr = GSUnlock())
		{
			if (unlockMgr->IsContentsUnlock(_unlockContentsId))
			{
				return true;
			}
		}
	}

	return false;
}
