#include "GsLockOnHandler.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsObserverManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsArenaManager.h"

#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/Data/GsGameObjectDataRemotePlayer.h"

#include "DataSchema/Skill/GsSchemaSkillSet.h"

#include "UTIL/GsGameObjectUtil.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

// 초기화
void FGsLockOnHandler::Init()
{
}
// 종료
void FGsLockOnHandler::Close()
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

		if (_systemMsg.Num() != 0)
		{
			for (auto iter : _systemMsg)
			{
				msg->GetSystem().Remove(iter);
			}
			_systemMsg.Empty();
		}
	}
}

// 락온 타게팅
void FGsLockOnHandler::SetLockOnTarget(UGsGameObjectBase* In_target, bool In_isCallByScanList)
{
	// 관전 모드에서는 안함
	if (GSObserver()->GetIsObserveStart() == true)
	{
		return;
	}

	if (In_target == nullptr)
	{
		return;
	}

	bool isInvalidTarget = false;
	if (UGsGameObjectCreature* creatureObj = Cast<UGsGameObjectCreature>(In_target))
	{
		isInvalidTarget = creatureObj->IsTargetedAvailableState() == false;
	}
	// hp 0이거나 dead 상태, hiding, bury
	if (true == isInvalidTarget)
	{
		return;
	}

	// rule changed(22/03/04)
	// 1. 1step -> 2step
	// 2. target -> lock on
	// 3. lock on -> other click -> other target and old lock on release

	// check targeting
	UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (localBase == nullptr)
	{
		return;
	}

	UGsGameObjectLocalPlayer* local = localBase->CastGameObject<UGsGameObjectLocalPlayer>();
	if (local == nullptr)
	{
		return;
	}

	// 타겟 가능한 상대인가
	if (false == local->CanTarget(In_target))
	{
		return;
	}

	FGsTargetHandlerLocalPlayer* targetHandler = local->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	if (targetHandler == nullptr)
	{
		return;
	}

	UGsGameObjectBase* oldTarget = targetHandler->GetLastTarget();
	bool isNewTargetSelected = false;
	if (oldTarget == nullptr ||
		oldTarget != In_target)
	{
		isNewTargetSelected = true;
	}

	bool isAction = false;
	if (true == isNewTargetSelected)
	{
		// set new target 
		if (In_isCallByScanList)
		{
			targetHandler->SetTarget(In_target, false, FGsTargetHandlerBase::SelectTargetFilter::ScanList);
			// 타겟이 1차 타겟으로 선정된 경우
			if (In_target == targetHandler->GetTarget())
			{
				isAction = true;
			}
		}
		else
		{
			targetHandler->SetTarget(In_target, false, FGsTargetHandlerBase::SelectTargetFilter::SelectTarget);
		}
	}
	else
	{
		// 타겟설정에 성공 했을때만
		if (targetHandler->SetTarget(In_target, false, FGsTargetHandlerBase::SelectTargetFilter::LockOn))
		{
			isAction = true;
		}
	}

	if (isAction)
	{
		// 스킬실행
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
	}
}
