#pragma once
#include "GsStateRemotePlayerBase.h"

#include "Classes/Engine/EngineTypes.h"
/*
	remote 워프 상태
*/
class T1PROJECT_API FGsStateRemotePlayerWarp :
	public FGsStateBaseRemotePlayer,
	public TGsStateSingleton<FGsStateRemotePlayerWarp>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseRemotePlayer);

};