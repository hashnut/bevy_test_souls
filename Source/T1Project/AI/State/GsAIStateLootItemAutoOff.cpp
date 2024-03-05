#include "GsAIStateLootItemAutoOff.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsAIManager.h"

#include "AI/Quest/GsAIQuestHandler.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectDropObject.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Reserve/GsReserveWork.h"
#include "GameObject/AI/GsAIReserveHandler.h"

#include "Message/GsMessageGAmeObject.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"

#include "AI/GsAIStateManager.h"
#include "AI/Data/GsAIReserveJob.h"

#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

void FGsAIStateLootItemAutoOff::Enter()
{
	// 자동 진행 아이템 획득일경우 스킬 예약 클리어
	FGsSkillHandlerLocalPlayer* localSkill = _targetPlayer->GetCastSkill< FGsSkillHandlerLocalPlayer>();
	if (localSkill)
	{
		localSkill->ClearReserveSkill();
	}

	TArray<IGsReserveWork*> resrveWorks = _targetPlayer->GetReserveWorks();
	for (IGsReserveWork* iter : resrveWorks)
	{
		if (iter == nullptr)
		{
			continue;
		}

		if (iter->IsReserveData() == true)
		{
			iter->DoRetry();
			return;
		}
	}

	FGsAIReserveHandler* aiReserveHandler = _targetPlayer->GetAIReserve();
	if (aiReserveHandler == nullptr)
	{
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		return;
	}

	FGsAIReserveJob* reserveJob = aiReserveHandler->GetFirstJob();
	if (reserveJob == nullptr || reserveJob->_targetObj == nullptr ||
		reserveJob->_actionType != EGsAIActionType::LOOT_ITEM_AUTO_OFF)
	{
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		return;
	}
	UGsGameObjectDropObject* findDrop = Cast<UGsGameObjectDropObject>(reserveJob->_targetObj);
	if (findDrop == nullptr)
	{
		return;
	}

	// 메세지 Send
	FGsInteractionMessageParam messageParam(findDrop->GetGameId()
		, findDrop->GetObjectType()
		, findDrop->GetItemAmount()
		, true);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::DROPOBJECT_PICK, &messageParam);

	FGsAIManager* aiMgr = GSAI();
	if (nullptr == aiMgr)
	{
		return;
	}
	FGsAIQuestHandler* aiQuestHandler = aiMgr->GetAIQuestHandler();
	if (nullptr == aiQuestHandler)
	{
		return;
	}

	aiQuestHandler->SetIsAutoOffLooting(true);
}