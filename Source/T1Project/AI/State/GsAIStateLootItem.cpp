#include "GsAIStateLootItem.h"

#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectDropObject.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Looting/GsLootingHandlerLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Reserve/GsReserveWork.h"
#include "GameObject/AI/GsAIReserveHandler.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Message/GsMessageGAmeObject.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "AI/GsAIStateManager.h"
#include "AI/Data/GsAIReserveJob.h"

#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"


void FGsAIStateLootItem::Enter()
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
	if (reserveJob == nullptr|| reserveJob->_targetObj == nullptr ||
		reserveJob->_actionType != EGsAIActionType::LOOT_ITEM)
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
}

void FGsAIStateLootItem::Update(float In_deltaTime)
{
	if (nullptr == _targetPlayer ||
		nullptr == _targetPlayer->GetBaseFSM())
	{
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		return;
	}

	// 아이들이면 
	if (true == _targetPlayer->GetBaseFSM()->IsState(EGsStateBase::Idle))
	{
		// 대기로 변경
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
	}
}