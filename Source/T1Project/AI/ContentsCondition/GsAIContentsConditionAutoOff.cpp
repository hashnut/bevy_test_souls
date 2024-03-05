
#include "GsAIContentsConditionAutoOff.h"

#include "Management/ScopeGame/GsAIManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Input/GsInputEventMsgBase.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageInput.h"

FGsAIContentsConditionAutoOff::~FGsAIContentsConditionAutoOff()
{
}

// 초기화
void FGsAIContentsConditionAutoOff::Initialize(FGsAIManager* In_mng)
{
	_aiManager = In_mng;
	
	FGsMessageHolder* msg = GMessage();

	
}
// 해제
void FGsAIContentsConditionAutoOff::Finalize()
{
	FGsMessageHolder* msg = GMessage();

	
}

void FGsAIContentsConditionAutoOff::SetCharacter(UGsGameObjectLocalPlayer* In_player)
{
	_localPlayer = In_player;
}
void FGsAIContentsConditionAutoOff::RemoveCharacter()
{
	_localPlayer = nullptr;
}

