#include "GsAITaskInfo.h"

void FGsAITask::Initialize()
{
}
void FGsAITask::Finalize()
{
	if (_reserveConditions.Num() > 0)
	{
		for (auto iter: _reserveConditions)
		{
			if (nullptr != iter)
			{
				iter->_func = nullptr;
				delete iter;
			}
		}
		_reserveConditions.Empty();
	}

	if (_actionConditions.Num() > 0)
	{
		for (auto iter : _actionConditions)
		{
			if (nullptr != iter)
			{
				iter->_func = nullptr;
				delete iter;
			}
		}
		_actionConditions.Empty();
	}

	if (_clearConditions.Num() > 0)
	{
		for (auto iter : _clearConditions)
		{
			if (nullptr != iter)
			{
				iter->_func = nullptr;
				delete iter;
			}
		}
		_clearConditions.Empty();
	}

	if (nullptr != _action)
	{
		delete _action;
	}
}
void FGsAITaskResolver::Initialize()
{
}
void FGsAITaskResolver::Finalize()
{
	if (_tasks.Num() > 0)
	{
		for (auto iter: _tasks)
		{
			if (nullptr != iter)
			{
				iter->Finalize();
				delete iter;
			}
		}
		_tasks.Empty();
	}
}
