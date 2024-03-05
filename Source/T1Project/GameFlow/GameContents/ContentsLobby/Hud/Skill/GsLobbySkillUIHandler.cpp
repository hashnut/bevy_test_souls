#include "GsLobbySkillUIHandler.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeLobby/GsLobbyCharacterManager.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectPlayableEventLocalPlayer.h"


// 초기화(최초 한번)
void FGsLobbySkillUIHandler::Init()
{
	FGsMessageHolder* msgHolder = GMessage();

	MReserveSkillAction& msgReserveSkill = msgHolder->GetSkillRserveActiveState();
	_msgContentHudHandleList.Emplace(msgReserveSkill.AddRaw(MessageContentHud::SKILL_ACTIVE,
		this, &FGsLobbySkillUIHandler::OnActiveSkill));
}
// 정리(마지막 한번)
void FGsLobbySkillUIHandler::Close()
{
	FGsMessageHolder* msgHolder = GMessage();

	MReserveSkillAction& msgReserveSkill = msgHolder->GetSkillRserveActiveState();
	for (MsgHudHandle& msgReserveSkillHandle : _msgContentHudHandleList)
	{
		msgReserveSkill.Remove(msgReserveSkillHandle);
	}
	_msgContentHudHandleList.Empty();
}

void FGsLobbySkillUIHandler::OnActiveSkill(int32 InSlotId)
{
	const TArray<int32>& skillList = GData()->GetGlobalData()->_playableEventLocalSkill;
	if (skillList.Num() == 0)
	{
		return;
	}

	if (_slotIndex >= skillList.Num())
	{
		_slotIndex = 0;
	}

	int skillId = skillList[_slotIndex];

	UGsGameObjectPlayableEventLocalPlayer* targetObj =
		LSLobbyCharacter()->GetPlayableEventLocalPlayer();

	if (nullptr == targetObj)
	{
		return;
	}

	if (FGsSkillHandlerBase* skillHandler = targetObj->GetSkillHandler())
	{
		// 성공시 슬롯 인덱스를 증가해준다.
		if (skillHandler->OnSkill(skillId))
		{
			// 1증가
			++_slotIndex;
		}
	}
}