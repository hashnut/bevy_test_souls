#pragma once
#include "GsStateNonPlayerBase.h"

/**
 * LifeTime 0 - Dead : Treasure Monster
 */
class T1PROJECT_API FGsStateNonPlayerRunAway :
	public FGsStateBaseSingleNonPlayer,
	public TGsStateSingleton<FGsStateNonPlayerRunAway>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleNonPlayer);
};

