#pragma once

#include "Shared/Client/SharedEnums/SharedEventEnum.h"


struct T1PROJECT_API FGsEventActionParamData
{
	MissionEventParam type;
	TArray<int64> paramSet;
};
