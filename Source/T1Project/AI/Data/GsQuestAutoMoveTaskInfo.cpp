#include "GsQuestAutoMoveTaskInfo.h"

void FGsQuestAutoMoveTask::Initialize()
{

}

void FGsQuestAutoMoveTask::Finalize()
{
	if (_conditions.Num() > 0)
	{
		for (FGsQuestAutoMoveCondition* iter: _conditions)
		{
			if (nullptr != iter)
			{
				iter->_conditionFunc = nullptr;
				delete iter;
			}
		}
		_conditions.Empty();
	}

	if (nullptr != _action)
	{
		delete _action;
		_action = nullptr;
	}
}

// 초기화
void FGsQuestAutoMoveResolver::Initialize()
{

}
// 정리
void FGsQuestAutoMoveResolver::Finalize()
{
	if (_tasks.Num() > 0)
	{
		for (FGsQuestAutoMoveTask* iter :_tasks)
		{
			if (nullptr != iter )
			{
				iter->Finalize();
				delete iter;
			}
		}
		_tasks.Empty();
	}
}