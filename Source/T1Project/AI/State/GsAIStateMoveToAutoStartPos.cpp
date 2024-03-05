#include "GsAIStateMoveToAutoStartPos.h"

#include "AI/GsAIStateManager.h"
#include "AI/ContentsCondition/GsAIContentsConditionAutoReturn.h"
#include "AI/ContentsCondition/GsAIContentsConditionAttOrder.h"
#include "AI/ContentsCondition/GsAIContentsConditionDefault.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"

#include "Util/GsClassUtil.h"
#include "Util/GsMove.h"

#include "Message/MessageParam/GsAIMessageParam.h"
#include "Message/GsMessageGameObject.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsAIManager.h"

#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

#include "AIModule/Classes/Navigation/PathFollowingComponent.h"

void FGsAIStateMoveToAutoStartPos::Enter()
{
	// 매니저 없으면 리턴		
	if (nullptr == _aiStateManager || nullptr == _aiManager)
	{
		return;
	}

	// 플레이어 없어도 리턴
	if (nullptr == _targetPlayer)
	{
		// 대기로
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		return;
	}

	// 이동 불가
	if (GetMoveAble() == false)
	{
		// 대기로
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		return;
	}

	FGsGameObjectStateManager* fsm = _targetPlayer->GetBaseFSM();
	// idle이 아니면 안하기
	if (nullptr == fsm || false == fsm->IsState(EGsStateBase::Idle))
	{
		// 대기로
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		return;
	}

	FGsMovementHandlerBase* movementBase = _targetPlayer->GetMovementHandler();
	if (nullptr == movementBase)
	{
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		return;
	}

	FGsMovementHandlerLocalPlayer* movementLocal = FGsClassUtil::FCheckStaticCast<FGsMovementHandlerLocalPlayer, FGsMovementHandlerBase>(movementBase);

	const FVector savePos = _aiManager->GetContentsConditionAutoReturn()->GetAutoStartPos();

	float acceptanceRadius = FGsMove::GetMovePosRange();
	// -값 들어가면 이동 실패 되므로 최소값으로 세팅
	acceptanceRadius = FMath::Max(acceptanceRadius, 0.1f);

	// 도착 안했다면, 목적지가 zero 아니라면
	if (nullptr != movementLocal &&
		false == movementLocal->GetStateAutoMove().HasReached(savePos, acceptanceRadius) &&
		false == savePos.IsNearlyZero())
	{
		// r3
		// https://jira.com2us.com/jira/browse/CHR-15157
		// clear target

		FGsTargetHandlerLocalPlayer* targetHandler = _targetPlayer->GetCastTarget<FGsTargetHandlerLocalPlayer>();
		if (targetHandler == nullptr)
		{
			return;
		}

		targetHandler->ClearTarget(true);


		GSAI()->MoveToPosAutoCurrentMap(EGsMovementAutoContentsType::AutoReturnMove, savePos,
			false);
	}
	// 도착 했다면
	else
	{
		FGsAIReserveParam aiReserveClearParam(EGsAIActionType::MOVE_TO_AUTO_START_POS);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_ALL_JOB_BY_TYPE, &aiReserveClearParam);

		// 대기로
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
	}
}
void FGsAIStateMoveToAutoStartPos::Update(float In_delta)
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
		return;
	}

	// 돌아오는 중에 전투 가능한 몬스터 보이면 이동 중지
	// 1. 내가 영역안에 들어왔는지
	// 2. 주변에 사냥 가능 몬스터 있는지
	// https://jira.com2us.com/jira/browse/CHR-24592
	bool isTooFarFromAutoStartPos = GSAI()->GetContentsConditionAutoReturn()->IsTooFarFromAutoStartPos();
	if (isTooFarFromAutoStartPos == false)
	{
		bool isInSearchRangeAnyoneEnemy =
			GSAI()->GetContentsConditionDefault()->IsInSearchRangeAnyoneEnemy();
		if (isInSearchRangeAnyoneEnemy == true)
		{
			// 이동 예약 데이터 삭제
			_targetPlayer->ClearReserveDataByReserveWork();

			// 이동 중지
			FGsGameObjectStateManager* fsm = _targetPlayer->GetBaseFSM();
			fsm->ProcessEvent(EGsStateBase::AutoMoveStop);

			FGsAIReserveParam aiReserveClearParam(EGsAIActionType::MOVE_TO_AUTO_START_POS);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_ALL_JOB_BY_TYPE, &aiReserveClearParam);

			// 대기로 변경
			_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		}
	}
}
