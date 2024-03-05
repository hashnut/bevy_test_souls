#include "GsAIStateManager.h"

#include "State/GsAIStateIdle.h"
#include "State/GsAIStateMoveToAutoStartPos.h"
#include "State/GsAIStateAttackAnyone.h"
#include "State/GsAIStateLootItem.h"
#include "State/GsAIStateLootItemAutoOff.h"
#include "State/GsAIStatePropInteraction.h"
#include "State/GsAIStateAutoMoveStop.h"
#include "State/GsAIStateAutoMoveStopWithReserveRetry.h"
#include "State/GsAIStateAutoMoveStopWithClearReserveSkill.h"
#include "State/GsAIStateWaitSecond.h"

#include "Data/GsAITaskInfo.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsAIManager.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/AI/GsAIReserveHandler.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"

#include "AI/Data/GsAIReserveJob.h"

#include "T1Project.h"

#include "Core/Public/Misc/DateTime.h"

FGsAIBaseState* FGsAIStateAllocator::Alloc(EGsAIActionType In_type)
{
	if (In_type == EGsAIActionType::WAIT)
	{
		return new FGsAIStateIdle();
	}	
	else if (In_type == EGsAIActionType::MOVE_TO_AUTO_START_POS)
	{
		return new FGsAIStateMoveToAutoStartPos();
	}
	else if (In_type == EGsAIActionType::ATTACK_ANYONE)
	{
		return new FGsAIStateAttackAnyone();
	}
	else if (In_type == EGsAIActionType::LOOT_ITEM)
	{
		return new FGsAIStateLootItem();
	}
	else if (In_type == EGsAIActionType::LOOT_ITEM_AUTO_OFF)
	{
		return new FGsAIStateLootItemAutoOff();
	}
	else if (In_type == EGsAIActionType::INTERACTION)
	{
		return new FGsAIStatePropInteraction();
	}
	else if (In_type == EGsAIActionType::AUTO_MOVE_STOP)
	{
		return new EGsAIStateAutoMoveStop();
	}
	else if (In_type == EGsAIActionType::AUTO_MOVE_STOP_WITH_RESERVE_RETRY)
	{
		return new EGsAIStateAutoMoveStopWithReserveRetry();
	}
	else if (In_type == EGsAIActionType::AUTO_MOVE_STOP_WITH_CLEAR_RESERVE_SKILL)
	{
		return new EGsAIStateAutoMoveStopWithClearReserveSkill();
	}
	else if (In_type == EGsAIActionType::WAIT_SECOND)
	{
		return new FGsAIStateWaitSecond();
	}
	
	return nullptr;
}

// 초기화
void FGsAIStateManager::Initialize(FGsAIManager* In_aiManager)
{
	TArray<EGsAIActionType> arrMode;

	arrMode.Add(EGsAIActionType::WAIT);	
	arrMode.Add(EGsAIActionType::MOVE_TO_AUTO_START_POS);
	arrMode.Add(EGsAIActionType::ATTACK_ANYONE);
	arrMode.Add(EGsAIActionType::LOOT_ITEM);
	arrMode.Add(EGsAIActionType::LOOT_ITEM_AUTO_OFF);
	arrMode.Add(EGsAIActionType::INTERACTION);
	arrMode.Add(EGsAIActionType::AUTO_MOVE_STOP);
	arrMode.Add(EGsAIActionType::AUTO_MOVE_STOP_WITH_RESERVE_RETRY);
	arrMode.Add(EGsAIActionType::AUTO_MOVE_STOP_WITH_CLEAR_RESERVE_SKILL);
	arrMode.Add(EGsAIActionType::WAIT_SECOND);

	for (auto& iter : arrMode)
	{
		auto makeObj = MakeInstance(iter);
		makeObj->SetManager(In_aiManager, this);
	}
	_waitRemakeListTime = GData()->GetGlobalData()->_aiWaitRemakeListTime * ETimespan::TicksPerSecond;
}
// 정리
void FGsAIStateManager::Finalize()
{
}
// 조건 체크 
void FGsAIStateManager::EvaluateConditions()
{
	if (nullptr == _taskResolver)
	{
		return;
	}

	// 기존꺼 돌리면 
	// 3. 루팅, 4. 전투일시
	// 전투하다가 계속 전투함....
	for (int i = 0; i < _taskResolver->_tasks.Num(); ++i)
	{
		if (EvaluateCondition(_taskResolver->_tasks[i]))
		{
			// 그만~
			break;
		}
	}
}

// 단일 컨디션 체크
bool FGsAIStateManager::EvaluateCondition(const FGsAITask* In_info)
{
	if (nullptr == In_info)
	{
		return false;
	}

	bool isPassed = true;

	for (auto iter : In_info->_reserveConditions)
	{
		// 값이 없거나 조건 만족 못하면 구만~
		if (nullptr == iter || nullptr == iter->_func ||
			false == iter->_func())
		{
			isPassed = false;
			break;
		}
	}
	// 모든 조건 만족하면
	// job 에 값이 있다면
	if (true == isPassed &&
		nullptr != In_info->_action)
	{
		if (true == _isShowLog)
		{
			GSLOG(Warning, TEXT("pass condition id: %d, annotation: %s"), 
				In_info->id, *In_info->_annotation);
		}

		FGsAIAction* action = In_info->_action;
		// 상태 변환
		ChangeState(action->_type);
		return true;
	}

	return false;
}



// 캐릭터 세팅
void FGsAIStateManager::SetCharacter(UGsGameObjectLocalPlayer* In_char)
{
	TMap<EGsAIActionType, TSharedPtr<FGsAIBaseState>> container = GetContainer();

	for (auto& iter : container)
	{
		iter.Value->SetTarget(In_char);
	}

	ClearState();

	// set ai reserver
	if (In_char != nullptr)
	{
		_aiReserveHandler = In_char->GetAIReserve();
	}
}
// 캐릭터 삭제
void FGsAIStateManager::RemoveCharacter()
{
	TMap<EGsAIActionType, TSharedPtr<FGsAIBaseState>> container = GetContainer();

	for (auto& iter : container)
	{
		iter.Value->RemoveCharacter();
	}
	
	_aiReserveHandler = nullptr;
}
// 상태 초기화
void FGsAIStateManager::ClearState()
{
	// 대기로 변경
	// 처음이거나
	// 대기가 아니면 변경
	if (false == CurrentState.IsValid() ||
		CurrentState.Pin()->GetType() != EGsAIActionType::WAIT)
	{
		ChangeState(EGsAIActionType::WAIT);
	}
}
// 갱신
void FGsAIStateManager::Update(float In_deltaTime)
{
	if (auto state = CurrentState.Pin())
	{
		state->Update(In_deltaTime);
	}

	int64 nowTick = FDateTime::UtcNow().GetTicks();

	if (_isWaitChangeState == true)
	{		
		int64 diffTick = nowTick - _delayStartTime;

		if (diffTick >= _delayTime)
		{
			_isWaitChangeState = false;
			ChangeStateByCurrentReserveJob();
		}
	}

	if (_isWaitRemakeList == true)
	{
		int64 remakeDiffTick = nowTick - _waitStartRemakeListTime;

		if (remakeDiffTick >= _waitRemakeListTime)
		{
			_isWaitRemakeList = false;
			_isDirtyRemakeList = true;
		}
	}

	if (_aiReserveHandler != nullptr)
	{
		// make job high priority
		if (true == MakeJobsByEvaluateConditions(_aiReserveHandler, true))
		{
			// 예약데이터 추가됨
			if (FGsAIManager* aiMgr = GSAI())
			{
				aiMgr->SendDebugReserveListUpdate();
			}

			FGsAIReserveJob* firstJob = _aiReserveHandler->GetFirstJob();
			if (firstJob != nullptr &&
				true == firstJob->_isHighPriority && CurrentState.IsValid() &&
				CurrentState.Pin()->GetType() != EGsAIActionType::WAIT &&
				false == CurrentState.Pin()->IsHighPriority() )
			{
				ClearState();
			}
		}
	}
}
void FGsAIStateManager::ChangeState(TStateType In_state)
{
	if (true == _isShowLog)
	{
		EGsAIActionType behaviorType = (EGsAIActionType)In_state;
		GSLOG(Warning, TEXT("ChangeState: %d, enum: %s"),
			In_state,
			*GetEnumToString(EGsAIActionType, behaviorType));
	}
	Super::ChangeState(In_state);
}

// 컨텐츠 변경에 따른 데이터 변경
void FGsAIStateManager::ChangeContentsType(const FGsAITaskResolver* In_data)
{
	// 해당 컨텐츠 데이터로 참조 변경
	_taskResolver = In_data;
	// 상태 초기화
	ClearState();
}

// 상태 멈춤
void FGsAIStateManager::StopState()
{
	// 중간에 끊음, 초기화
	if (auto state = CurrentState.Pin())
	{
		state->Stop();
	}
}

// 해당 상태인가
bool FGsAIStateManager::IsState(EGsAIActionType In_type)
{
	if (CurrentState.IsValid() == false)
	{
		return false;
	}

	return (CurrentState.Pin()->GetType() == In_type) ? true : false;
}

void FGsAIStateManager::ProcessReserveJob()
{
	if (_isWaitChangeState == true)
	{
		return;
	}


	// check remain reserve job
	if (_aiReserveHandler == nullptr)
	{
		return;
	}

	EGsAIDelayType delayType = EGsAIDelayType::NONE;

	bool isDebugReserveListUpdate = false;
	// 시작 지역으로 돌아가는게 첫번째인지
	bool isFirstJobReturnPos = _aiReserveHandler->IsFirstJobAction(EGsAIActionType::MOVE_TO_AUTO_START_POS);
	// 대기가 첫번째인지
	bool isFirstJobWaitSecond = _aiReserveHandler->IsFirstJobAction(EGsAIActionType::WAIT_SECOND);

	// empty
	if (_aiReserveHandler->IsReserveJobEmpty() == true ||
		(_isDirtyRemakeList == true &&
			// 첫번째가 원점 돌아가는 모드면 clear를 하지 않는다
			// 타이머 때문에 돌아가는 job이 삭제되어서 돌아가지 못하고 더 전투하는 현상 때문
			// https://jira.com2us.com/jira/browse/CHR-24592
			// 대기 시간도 추가로 체크
			(isFirstJobReturnPos == false && isFirstJobWaitSecond == false)
		))
	{	
		bool isByRemakeListTimmer = false;
		// only use clear retry by empty job
		// dirty(over time remake list) clear causes same target retry
		if (_isDirtyRemakeList == true)
		{
			_isDirtyRemakeList = false;
			isByRemakeListTimmer = true;
			// clear old list data
			_aiReserveHandler->ClearAllJob();

			isDebugReserveListUpdate = true;
		}
		else
		{
			_aiReserveHandler->ClearRetry();
		}
		// 예약 데이터 추가됨
		if (true == MakeJobsByEvaluateConditions(_aiReserveHandler, false))
		{			
			isDebugReserveListUpdate = true;
		}

		if (isByRemakeListTimmer == true)
		{
			delayType = EGsAIDelayType::NONE;
		}
		else
		{ 
			delayType = EGsAIDelayType::MAKE_JOB_DELAY;		

			//https://jira.com2us.com/jira/browse/C2URWQ-2738
			// list remake wait time start			
		}
		_waitStartRemakeListTime = FDateTime::UtcNow().GetTicks();
		_isWaitRemakeList = true;
	}
	else if(_aiReserveHandler->GetIsRetry() == true)
	{
		delayType = EGsAIDelayType::RETRY_DELAY;
	}

	if (delayType == EGsAIDelayType::NONE)
	{
		_aiReserveHandler->ClearRetry();
		ChangeStateByCurrentReserveJob();		
	}
	else
	{
		_delayStartTime = FDateTime::UtcNow().GetTicks();
		_isWaitChangeState = true;
		_delayTime = ((delayType == EGsAIDelayType::RETRY_DELAY) ?
			GData()->GetGlobalData()->_aiTaskRetryDelayTime : GData()->GetGlobalData()->_aiIdleMakeJobDelayTime) *
			ETimespan::TicksPerSecond;
	}

	if(isDebugReserveListUpdate == true)
	{ 
		if (FGsAIManager* aiMgr = GSAI())
		{
			aiMgr->SendDebugReserveListUpdate();
		}
	}
}

void FGsAIStateManager::ChangeStateByCurrentReserveJob()
{
	// check remain reserve job
	UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == player)
	{
		return;
	}

	UGsGameObjectLocalPlayer* local = player->CastGameObject<UGsGameObjectLocalPlayer>();
	if (local == nullptr)
	{
		return;
	}

	FGsAIReserveHandler* aiReserveHandler = local->GetAIReserve();
	if (aiReserveHandler == nullptr)
	{
		return;
	}

	FGsAIReserveJob* reserveJob = aiReserveHandler->GetClearConditionCheckedFirstJob();
	if (reserveJob == nullptr)
	{
		return;
	}

	bool isPass = true;
	for (FGsAICondition* actionCondition : reserveJob->_actionConditions)
	{
		if (actionCondition == nullptr ||
			actionCondition->_func == nullptr || actionCondition->_func() == false)
		{
			isPass = false;
			break;
		}
	}
	if (isPass == true)
	{
		ChangeState(reserveJob->_actionType);
	}
}

bool FGsAIStateManager::MakeJobsByEvaluateConditions(FGsAIReserveHandler* In_aiReserveHandler, 
	bool In_isHighPriority)
{
	if (nullptr == _taskResolver)
	{
		return false;
	}
	bool isMakeJob = false;
	// high priority(always add check)
	if (In_isHighPriority == false)
	{
		// if target exist 
		// add first
		UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
		if (nullptr == player)
		{
			return false;
		}

		UGsGameObjectLocalPlayer* local = player->CastGameObject<UGsGameObjectLocalPlayer>();
		if (local == nullptr)
		{
			return false;
		}

		FGsTargetHandlerLocalPlayer* targetHandler = local->GetCastTarget<FGsTargetHandlerLocalPlayer>();

		if (targetHandler == nullptr)
		{
			return false;
		}

		// now target add first( only ai on)
		if (true == GSAI()->IsAIOn())
		{
			UGsGameObjectBase* nowTarget = targetHandler->GetTarget();
			if (nowTarget != nullptr &&
				In_aiReserveHandler != nullptr)
			{
				In_aiReserveHandler->AddJob(EGsAIActionType::ATTACK_ANYONE, nowTarget, 0);
				isMakeJob = true;
			}
		}
	}
	
	for (int i = 0; i < _taskResolver->_tasks.Num(); ++i)
	{
		FGsAITask* task = _taskResolver->_tasks[i];
		if (task == nullptr)
		{
			continue;
		}
		if (task->_isHighPriority != In_isHighPriority)
		{
			continue;
		}
		if (true == MakeJobByEvaluateCondition(task, In_aiReserveHandler, In_isHighPriority))
		{
			isMakeJob = true;
		}
	}

	In_aiReserveHandler->PostMakeJobsByEvaluateConditions();

	return isMakeJob;
}

bool FGsAIStateManager::MakeJobByEvaluateCondition(const FGsAITask* In_info, FGsAIReserveHandler* In_aiReserveHandler,
	bool In_isHighPriority)
{
	if (nullptr == In_info)
	{
		return false;
	}

	bool isPassed = true;

	for (auto iter : In_info->_reserveConditions)
	{
		// 값이 없거나 조건 만족 못하면 구만~
		if (nullptr == iter || nullptr == iter->_func ||
			false == iter->_func())
		{
			isPassed = false;
			break;
		}
	}
	// 모든 조건 만족하면
	// job 에 값이 있다면
	if (true == isPassed &&
		nullptr != In_info->_action)
	{
		if (true == _isShowLog)
		{
			GSLOG(Warning, TEXT("pass condition id: %d, annotation: %s"),
				In_info->id, *In_info->_annotation);
		}

		FGsAIAction* action = In_info->_action;
		In_aiReserveHandler->MakeJob(action->_type, In_info->_reserveCount, 
			In_info->_actionConditions, In_info->_clearConditions,
			In_isHighPriority);
		return true;
	}

	return false;
}

const FGsAITask* FGsAIStateManager::FindTaskByActionType(EGsAIActionType In_actionType)
{
	if (nullptr == _taskResolver)
	{
		return nullptr;
	}

	for (int i = 0; i < _taskResolver->_tasks.Num(); ++i)
	{
		const FGsAITask* task = _taskResolver->_tasks[i];
		if (task == nullptr ||
			task->_action == nullptr)
		{
			continue;
		}

		if (task->_action->_type == In_actionType)
		{
			return task;
		}
	}

	return nullptr;
}