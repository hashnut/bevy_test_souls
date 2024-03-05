#include "GsAIReserveJob.h"
#include "AI/Data/GsAITaskInfo.h"

void FGsAIReserveJob::ClearData()
{
	if (_targetObj != nullptr)
	{
		_targetObj = nullptr;
	}

	if (_actionConditions.Num() != 0)
	{
		_actionConditions.Empty();
	}
	if (_clearConditions.Num() != 0)
	{
		_clearConditions.Empty();
	}
}