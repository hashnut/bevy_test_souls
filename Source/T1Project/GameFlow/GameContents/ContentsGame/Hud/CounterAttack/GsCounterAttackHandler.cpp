#include "GsCounterAttackHandler.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageContents.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"

#include "DataSchema/Skill/GsSchemaSkillSet.h"

#include "GameObject/CounterAttack/GsAttackInfoCounterAttack.h"

#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDCounterAttackButton.h"

// initialize
void FGsCounterAttackHandler::Init()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_HIT, this,
				&FGsCounterAttackHandler::OnLocalHit));

		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::ABNORMALITY_ALL_UPDATE, this,
				&FGsCounterAttackHandler::OnAbnormalityAllUpdate));
	}
}
// close
void FGsCounterAttackHandler::Close()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_gameObjectMsg.Num() != 0)
		{
			for (auto iter : _gameObjectMsg)
			{
				msg->GetGameObject().Remove(iter);
			}
			_gameObjectMsg.Empty();
		}
	}
}

// 로컬 얻어 맞음
void FGsCounterAttackHandler::OnLocalHit(const IGsMessageParam* In_param)
{
	const FGsGameObjectMessageParamTarget* paramTarget =
		static_cast<const FGsGameObjectMessageParamTarget*>(In_param);

	if (nullptr == paramTarget->_paramTarget)
	{
		return;
	}

	// only set remote player
	if (paramTarget->_paramTarget->IsObjectType(EGsGameObjectType::RemotePlayer) == false)
	{
		return;
	}

	int64 attackerGameId = paramTarget->_paramTarget->GetGameId();
	GSGameObject()->StartCounterAttackShow(attackerGameId);

	// show ui
	UpdateCounterAttackButtonHUDMode();
}

void FGsCounterAttackHandler::OnClickCounterAttackButton()
{
	UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (localBase == nullptr)
	{
		return;
	}
	FGsTargetHandlerLocalPlayer* targetHandler = localBase->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	if (targetHandler == nullptr)
	{
		return;
	}

	FGsAttackInfoCounterAttack* attackInfoCounterAttack =  GSGameObject()->GetAttackInfoCounterAttack();
	if (attackInfoCounterAttack == nullptr)
	{
		return;
	}
	int64 targetGameId = attackInfoCounterAttack->GetTargetGameId();

	if (targetGameId == -1)
	{
		return;
	}
	UGsGameObjectBase* findTarget =
		GSGameObject()->FindObject(EGsGameObjectType::Creature, targetGameId);

	if (findTarget == nullptr)
	{
		return;
	}

	targetHandler->SetTarget(findTarget, false, FGsTargetHandlerBase::SelectTargetFilter::LockOn);
	UGsGameObjectLocalPlayer* local = localBase->CastGameObject<UGsGameObjectLocalPlayer>();
	if (local == nullptr)
	{
		return;
	}
	// 락온일경우 스킬실행
	FGsSkillHandlerLocalPlayer* skillHandler = local->GetCastSkill<FGsSkillHandlerLocalPlayer>();
	if (skillHandler == nullptr)
	{
		return;
	}

	const FGsSchemaSkillSet* firstSkillData = skillHandler->GetFirstComboData();
	if (firstSkillData == nullptr)
	{
		return;
	}

	skillHandler->OnSkill(firstSkillData->id);

	GMessage()->GetSaveBattery().SendMessage(MessageContentSaveBattery::CLOSE, nullptr);
}

void FGsCounterAttackHandler::UpdateCounterAttackButtonHUDMode()
{
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_COUNTERATTACK);
}

void FGsCounterAttackHandler::OnAbnormalityAllUpdate(const IGsMessageParam* In_param)
{
	const FGsGameObjectMessageParamAbnormality* param = In_param->Cast<const FGsGameObjectMessageParamAbnormality>();
	if (param && param->_paramOwner)
	{
		if (FGsAbnormalityHandlerBase* abnormalityHandler = param->_paramOwner->GetAbnormalityHandler())
		{
			// state changed
			if (abnormalityHandler->IsTargetedAvailableState() == false)
			{
				// stop show
				int64 targetGameId = param->_paramOwner->GetGameId();
				GSGameObject()->StopCounterAttackShow(targetGameId);
			}
		}
	}
}