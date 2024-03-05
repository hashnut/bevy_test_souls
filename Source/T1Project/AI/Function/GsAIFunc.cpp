#include "GsAIFunc.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataSchema/Skill/GsSchemaSkillSet.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsGameObjectUtil.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Interaction/GsInteractionMoveType.h"
#include "GameObject/Interaction/GsReserveInteractionInfo.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Define/GsGameObjectDefine.h"

#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsSkillManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsAIManager.h"

#include "Message/GsMessageContentHud.h"

#include "Skill/GsSkill.h"
#include "AI/ContentsCondition/GsAIContentsConditionDefault.h"


#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

// 현재 사용할 skill id 얻어오기
// 쿨타임 없는 녀석들중
// 제일 앞에꺼
// 1번째껀 평타
const FGsSchemaSkillSet* FGsAIFunc::FindNowUseSkillID(UGsGameObjectLocalPlayer* In_player)
{
	check(In_player);

	FGsSkillHandlerLocalPlayer* skillHandler = In_player->GetCastSkill<FGsSkillHandlerLocalPlayer>();
	check(skillHandler);

	return skillHandler->GetNextAutoSkill(SkillCategorySet::ACTIVE);
}

// 지금 사용가능한 스킬있는가(평타뺴고)
bool FGsAIFunc::HasNowUseSkill(UGsGameObjectLocalPlayer* In_player)
{
	check(In_player);

	FGsSkillHandlerLocalPlayer* skillHandler = In_player->GetCastSkill<FGsSkillHandlerLocalPlayer>();
	check(skillHandler);

	const FGsSchemaSkillSet* skillSet = skillHandler->GetNextAutoSkill();
	if (skillSet == nullptr || skillSet->isNormal)
	{
		return false;
	}
	return true;
}

// 일반 스킬(평타) 가져오기
int FGsAIFunc::GetNormalSkillId()
{
	int normalSkillID = 0;
	if (const FGsSkill* findSkill = GSkill()->FindSkillSlot(0))
	{
		normalSkillID = findSkill->_tableData->id;
	}
	return normalSkillID;
}

// 다음 자동 스킬 범위
float FGsAIFunc::GetNextAutoSkillRange(UGsGameObjectLocalPlayer* In_player)
{
	float findSkillRange = 0.0f;
	FGsSkillHandlerLocalPlayer* skillHandler = In_player->GetCastSkill<FGsSkillHandlerLocalPlayer>();
	if (nullptr == skillHandler)
	{
		return findSkillRange;
	}
	const FGsSchemaSkillSet* skillSet = skillHandler->GetNextAutoSkill();
	if (skillSet == nullptr)
	{
		// 평타꺼 찾으면 됨
		if (const FGsSkill* findSkill = GSkill()->FindSkillSlot(0))
		{
			if (const FGsSchemaSkillSet* normalSkillSet = findSkill->_tableData)
			{
				findSkillRange = (float)FGsSkillHandlerBase::GetSkillAttackRange(In_player, normalSkillSet);
			}
		}
		return findSkillRange;
	}
	
	findSkillRange = (float)skillSet->approachRange;
	return findSkillRange;
}

// npc function slot move
void FGsAIFunc::StartMoveNPCFunctionSlot(int In_tblId)
{
	UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

	if (nullptr == localBase)
	{
		return;
	}
	UGsGameObjectLocalPlayer* local = localBase->CastGameObject<UGsGameObjectLocalPlayer>();

	if (nullptr == local)
	{
		return;
	}
	// https://jira.com2us.com/jira/browse/C2URWQ-4034
	// 서버 lock 상태이거나
	// game object fsm spawn 상태이면 막기
	bool isServerLock =
		local->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT);
	bool isStateSpawn = local->GetBaseFSM()->IsState(EGsStateBase::Spawn);
	if (isServerLock == true ||
	isStateSpawn == true)
	{
		return;
	}

	// 인터랙션 서버 응답 대기중에는 처리 안하기
	// 상점 인터랙션 도착후 서버 전송하고 대기중 다른 npc 슬롯으로 이동시
	// 상점 열린상태에서 이동
	// https://jira.com2us.com/jira/browse/C2URWQ-7478
	if (true == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE))
	{
		return;
	}



	//// https://jira.com2us.com/jira/browse/C2URWQ-1516

	FGsInteractionHandlerLocalPlayer* localInteraction = local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();
	if (localInteraction == nullptr)
	{
		return;
	}


	FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (movement == nullptr)
	{
		return;
	}
	// now intput mode, not move
	if (movement->IsInputMove() == true)
	{
		return;
	}

	TArray<int> checkNpcId{ In_tblId };	
	if (IsNPCMoveTarget(localBase, checkNpcId) == true)
	{
		// 이동 예약 데이터 삭제
		local->ClearReserveDataByReserveWork();

		local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);
		return;
	}


	if (FGsQuestHandler* questHandler = local->GetQuestHandler())
	{
		questHandler->OnUserAutoMoveStop();
	}

	localInteraction->TryInteractionNpcTblId(In_tblId);
	
	localInteraction->BindAutoMoveMessageByNPCFunctionSlot();
	
}
void FGsAIFunc::PostAutoMoveRequetedNPCFunction()
{
	UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

	if (nullptr == localBase)
	{
		return;
	}
	UGsGameObjectLocalPlayer* local = localBase->CastGameObject<UGsGameObjectLocalPlayer>();

	if (nullptr == local)
	{
		return;
	}


	FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (movement == nullptr)
	{
		return;
	}

	movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::NpcListMove);

	// 저장
	GSAI()->SetNpcListMoveStart(true);

	// update slot ui
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_NPCFUNCTION);
}
bool FGsAIFunc::IsNPCMoveTarget(
	UGsGameObjectBase* In_localBase, TArray<int> In_arrayNpcid, bool In_isCheckFSM)
{
	if (In_localBase == nullptr || In_arrayNpcid.Num() == 0)
	{
		return false;
	}

	UGsGameObjectLocalPlayer* local = In_localBase->CastGameObject<UGsGameObjectLocalPlayer>();

	if (nullptr == local)
	{
		return false;
	}

	// 1. check state auto move
	bool isAutoMove =(In_isCheckFSM == false)? true:  local->GetBaseFSM()->IsState(EGsStateBase::AutoMove);

	FGsInteractionHandlerLocalPlayer* localInteraction = local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();
	if (localInteraction == nullptr)
	{
		return false;
	}
	// 2. check same interaction target id 
	bool isSameInteractionTargetId = false;
	if (localInteraction->IsReserveInteraction() == true)
	{
		const FGsReserveInteractionInfo& reserveInteractionInfo = localInteraction->GetReserveInfo();

		if (reserveInteractionInfo._moveType == EGsInteractionMoveType::PosMove)
		{
			if (In_arrayNpcid.Contains (reserveInteractionInfo._target._tblId) == true)
			{
				isSameInteractionTargetId = true;
			}
		}
		else if (reserveInteractionInfo._moveType == EGsInteractionMoveType::TargetMove)
		{
			for (int iterTblId : In_arrayNpcid)
			{
				UGsGameObjectBase* nonPlayer = GSGameObject()->FindObjectNearestNonPlayerByNPCId(
					In_localBase->GetActor(), iterTblId);
				if (nonPlayer != nullptr)
				{
					if (localInteraction->GetInteractionTarget() == nonPlayer)
					{
						isSameInteractionTargetId = true;
						break;
					}
				}
			}
		}
	}

	FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (movement == nullptr)
	{
		return false;
	}

	// 3. check movement auto contents type
	bool isMovementAutoContentsNpcListMove = false;
	if (movement->GetStateAutoMove().GetMovementAutoContentsType() == EGsMovementAutoContentsType::NpcListMove)
	{
		isMovementAutoContentsNpcListMove = true;
	}

	if (isAutoMove == true &&
		isSameInteractionTargetId == true &&
		isMovementAutoContentsNpcListMove == true)
	{
		return true;
	}

	return false;
}