#include "GsAttackInfoScan.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"


float FGsAttackInfoScan::GetWaitTime()
{
	return GData()->GetGlobalData()->_scanAttackStateWaitTime;
}

void FGsAttackInfoScan::EndTime()
{
	if (CallbackTimeEnd != nullptr)
	{
		CallbackTimeEnd(_gameId);
	}
}

