#pragma once

#include "GsStateRemotePlayerBase.h"

/*
	remote 스폰 상태
*/
class T1PROJECT_API FGsStateRemotePlayerSpawn :
	public FGsStateBaseRemotePlayer,
	public TGsStateSingleton<FGsStateRemotePlayerSpawn>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseRemotePlayer);
};