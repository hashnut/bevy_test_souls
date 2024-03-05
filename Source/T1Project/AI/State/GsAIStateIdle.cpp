#include "GsAIStateIdle.h"
#include "AI/GsAIStateManager.h"

#include "GameObject/AI/GsAIReserveHandler.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Core/Public/Misc/DateTime.h"

void FGsAIStateIdle::Update(float In_deltaTime)
{
	if (nullptr == _aiStateManager)
	{
		return;
	}

	// do reserve job process
	_aiStateManager->ProcessReserveJob();
}
