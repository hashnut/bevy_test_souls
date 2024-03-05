#include "GsAIReserveHandler.h"
#include "AI/Data/GsAIReserveJob.h"
#include "AI/Data/GsAITaskInfo.h"
#include "AI/GsAIStateManager.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsAIManager.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Target/AutoTarget/GsTargetRule.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsAIMessageParam.h"

#include "T1Project.h"

#include "Core/Public/Misc/DateTime.h"

void FGsAIReserveHandler::MakeJob(EGsAIActionType In_type, int In_reserveCount, 
	TArray<FGsAICondition*> In_actionCondition, TArray<FGsAICondition*> In_clearCondition,
	bool In_isHighPriority)
{
	switch (In_type)
	{
	case EGsAIActionType::MOVE_TO_AUTO_START_POS:
	case EGsAIActionType::AUTO_MOVE_STOP:
	case EGsAIActionType::AUTO_MOVE_STOP_WITH_RESERVE_RETRY:
	case EGsAIActionType::AUTO_MOVE_STOP_WITH_CLEAR_RESERVE_SKILL:
	{
		FGsAIReserveJob* newJob = new FGsAIReserveJob();
		newJob->_actionType = In_type;
		newJob->_actionConditions = In_actionCondition;
		newJob->_clearConditions = In_clearCondition;
		newJob->_isHighPriority = In_isHighPriority;

		if (In_isHighPriority == true)
		{
			_arrayReserveJob.Insert(newJob, 0);
		}
		else
		{
			_arrayReserveJob.Add(newJob);
		}
		// 돌아가는 액션 리스트가 첫번째 있어도 예약 스킬이 등록되어있으면
		// 안돌아가고 사냥을 더함
		// 돌아가는 액션 저장 시점에 예약된 스킬이 있으면 지운다
		// https://jira.com2us.com/jira/browse/CHR-24592
		if (In_type == EGsAIActionType::MOVE_TO_AUTO_START_POS)
		{
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

			FGsSkillHandlerLocalPlayer* localSkill = local->GetCastSkill< FGsSkillHandlerLocalPlayer>();
			if (nullptr == localSkill)
			{
				return;
			}

			if (true == localSkill->IsReserveData())
			{
				localSkill->ClearReserveSkill();
			}
		}
	}
		break;
	case EGsAIActionType::ATTACK_ANYONE:
	case EGsAIActionType::LOOT_ITEM:
	case EGsAIActionType::LOOT_ITEM_AUTO_OFF:
	case EGsAIActionType::INTERACTION:
	{		
		// find target
		TArray<UGsGameObjectBase*> arrayTarget = GetActionTarget(In_type);
		
		int targetCount = arrayTarget.Num();
		// no target -> error(condition not matching)
		if (targetCount == 0)
		{
			return;
		}

		int reserveCount = In_reserveCount;

		// ATTACK_ANYONE 일때  앞에서 이미 현재 타겟인 녀석을 넣어 놓았을수도 있다
		// https://jira.com2us.com/jira/browse/CHR-24592
		if (In_type == EGsAIActionType::ATTACK_ANYONE)
		{
			// 등록할 갯수가 0이면 return
			if (In_reserveCount == 0)
			{
				return;
			}

			if (_arrayReserveJob.Num() != 0 &&
				_arrayReserveJob[0] != nullptr &&
				_arrayReserveJob[0]->_actionType == EGsAIActionType::ATTACK_ANYONE)
			{
				reserveCount = In_reserveCount -1;
			}
		}


		int addCount = 0;
		for (int i=0; addCount < reserveCount; ++i)
		{
			// target not enough count
			if (targetCount - 1 < i)
			{
				return;
			}

			if (true == IsExistTargetObj(arrayTarget[i]))
			{
				continue;
			}

			FGsAIReserveJob* newJob = new FGsAIReserveJob();
			newJob->_actionType = In_type;
			newJob->_targetObj = arrayTarget[i];
			newJob->_actionConditions = In_actionCondition;
			newJob->_clearConditions = In_clearCondition;
			newJob->_isHighPriority = In_isHighPriority;

			if (In_isHighPriority == true)
			{
				_arrayReserveJob.Insert(newJob, 0);
			}
			else
			{
				if (newJob->_targetObj != nullptr &&
					newJob->_targetObj->GetGameId() == _blockTargetGameId)
				{
					_arrayPostAddReserveJob.Add(newJob);
				}
				else
				{
					_arrayReserveJob.Add(newJob);
				}
			}

			++addCount;
		}
	}
	break;
	
	}
}
TArray<UGsGameObjectBase*> FGsAIReserveHandler::GetActionTarget(EGsAIActionType In_action)
{
	TArray<UGsGameObjectBase*> arrayTarget;
	switch (In_action)
	{
	case EGsAIActionType::ATTACK_ANYONE:
	{
		UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
		if (nullptr == player)
		{
			return arrayTarget;
		}

		UGsGameObjectLocalPlayer* local = player->CastGameObject<UGsGameObjectLocalPlayer>();
		if (local == nullptr)
		{
			return arrayTarget;
		}

		FGsTargetHandlerLocalPlayer* targetHandler = local->GetCastTarget<FGsTargetHandlerLocalPlayer>();
		if (targetHandler == nullptr)
		{
			return arrayTarget;
		}
		FGsTargetRule* targetRule = targetHandler->GetTargetRule();
		if (targetRule == nullptr)
		{
			return arrayTarget;
		}
		// find enemy
		targetRule->OnAutoTargetAction(arrayTarget, FGsTargetRule::EGsTargetRuleType::AutoTarget);
	}
	break;
	case EGsAIActionType::LOOT_ITEM:
	case EGsAIActionType::LOOT_ITEM_AUTO_OFF:
	{
		// find drop item
		arrayTarget = GSGameObject()->FindDropObjectsNearest(true);
	}
	break;
	case EGsAIActionType::INTERACTION:
	{
		// find prop interaction 
		arrayTarget = GSGameObject()->FindPropObjectsNearest(true);
	}
	break;
	}

	return arrayTarget;
}
void FGsAIReserveHandler::Initialize(UGsGameObjectBase* In_local)
{
	FGsMessageHolder* msg = GMessage();

	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::AI_CLEAR_RESERVE_JOB,
			this, &FGsAIReserveHandler::OnAIClearReserveJob));

	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::AI_CLEAR_RESERVE_ALL_JOB_BY_TYPE,
			this, &FGsAIReserveHandler::OnAIClearReserveAllJobByType));

	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::AI_ADD_RESERVE_JOB,
			this, &FGsAIReserveHandler::OnAIAddReserveJob));

	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::AI_MODE_ACTIVE,
			this, &FGsAIReserveHandler::OnAIModeActive));

	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE,
			this, &FGsAIReserveHandler::OnLocalSpawnMeComplete));

	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_AUTOMOVE_STOP,
			this, &FGsAIReserveHandler::OnLocalAutoMoveStop));
}
void FGsAIReserveHandler::Finalize()
{
	ClearAllJob();

	FGsMessageHolder* msg = GMessage();

	for (MsgGameObjHandle& el : _gameobjectDelegates)
	{
		msg->GetGameObject().Remove(el);
	}
	_gameobjectDelegates.Empty();
}

void FGsAIReserveHandler::ClearAllJob()
{
	if (_arrayReserveJob.Num() != 0)
	{
		for (FGsAIReserveJob* job : _arrayReserveJob)
		{
			if (job == nullptr)
			{
				continue;
			}

			#ifdef TEST_AI_RETURN
			if (job->_actionType == EGsAIActionType::MOVE_TO_AUTO_START_POS)
			{
				GSLOG(Error, TEXT("MOVE_TO_AUTO_START_POS cleared !!"));
			}
			#endif

			job->ClearData();
			delete job;
		}
		_arrayReserveJob.Empty();
	}

	if (_arrayPostAddReserveJob.Num() != 0)
	{
		for (FGsAIReserveJob* job : _arrayPostAddReserveJob)
		{
			if (job == nullptr)
			{
				continue;
			}
			job->ClearData();
			delete job;
		}
		_arrayPostAddReserveJob.Empty();
	}
}

bool FGsAIReserveHandler::IsReserveJobEmpty()
{
	return (_arrayReserveJob.Num() == 0) ? true : false;
}

FGsAIReserveJob* FGsAIReserveHandler::GetFirstJob()
{
	if (_arrayReserveJob.Num() == 0)
	{
		return nullptr;
	}

	return _arrayReserveJob[0];
}
// get first job
// check clear job -> clear not possible job
FGsAIReserveJob* FGsAIReserveHandler::GetClearConditionCheckedFirstJob()
{
	FGsAIReserveJob* firstJob = nullptr;
	while(1)
	{
		if (true == CheckFirstJob(firstJob))
		{
			break;
		}
	}
	return firstJob;
}

bool FGsAIReserveHandler::CheckFirstJob(FGsAIReserveJob*& Out_firstJob)
{
	FGsAIReserveJob* firstJob = GetFirstJob();
	if (firstJob == nullptr)
	{
		return true;
	}

	bool isPass = false;
	for (FGsAICondition* clearCondition : firstJob->_clearConditions)
	{
		if (clearCondition == nullptr ||
			clearCondition->_func == nullptr || clearCondition->_func() == false)
		{
			isPass = false;
			break;
		}
		else
		{
			isPass = true;
		}
	}

	// condition pass(table check value)
	if (isPass == true)
	{
		// remove first job
		ClearReserveJob(firstJob);

		// 예약 데이터 갱신됨
		if (FGsAIManager* aiMgr = GSAI())
		{
			aiMgr->SendDebugReserveListUpdate();
		}
		return false;
	}

	Out_firstJob = firstJob;
	return true;

}
void FGsAIReserveHandler::OnAIClearReserveJob(const IGsMessageParam* In_param)
{
	const FGsAIReserveParam* aiReserveClearParam =
		In_param->Cast<const FGsAIReserveParam>();

	if (aiReserveClearParam == nullptr)
	{
		return;
	}

	ClearJob(aiReserveClearParam->_actionType, aiReserveClearParam->_targetObj);

	// 예약 데이터 갱신됨
	if (FGsAIManager* aiMgr = GSAI())
	{
		aiMgr->SendDebugReserveListUpdate();
	}
}
void FGsAIReserveHandler::OnAIClearReserveAllJobByType(const IGsMessageParam* In_param)
{
	const FGsAIReserveParam* aiReserveClearParam =
		In_param->Cast<const FGsAIReserveParam>();

	if (aiReserveClearParam == nullptr)
	{
		return;
	}

	ClearAllJobByType(aiReserveClearParam->_actionType);

	// 예약 데이터 갱신됨
	if (FGsAIManager* aiMgr = GSAI())
	{
		aiMgr->SendDebugReserveListUpdate();
	}
}

void FGsAIReserveHandler::ClearJob(EGsAIActionType In_type, UGsGameObjectBase* In_target)
{
	TArray<FGsAIReserveJob*> arrayDelIter;
	
	for (int i=0; i < _arrayReserveJob.Num(); ++i)
	{
		FGsAIReserveJob* iter = nullptr;
		iter = _arrayReserveJob[i];
		if (iter == nullptr)
		{
			continue;
		}
		bool isClearJob = false;
		if (iter->_actionType == In_type)
		{
			// 타겟이 없는 타입은 바로 삭제
			if (In_target == nullptr)
			{
				isClearJob = true;
			}
			else if (In_target == iter->_targetObj)
			{
				isClearJob = true;
			}

		}
		if (isClearJob == true)
		{
			arrayDelIter.Add(iter);
		}
	}

	for (FGsAIReserveJob* delIter : arrayDelIter)
	{		
		if (delIter != nullptr)
		{

#ifdef TEST_AI_RETURN
			if (delIter->_actionType == EGsAIActionType::MOVE_TO_AUTO_START_POS)
			{
				GSLOG(Error, TEXT("MOVE_TO_AUTO_START_POS cleared !!"));
			}
#endif

			delIter->ClearData();
			delete delIter;
		}

		_arrayReserveJob.Remove(delIter);
	}
}

void FGsAIReserveHandler::ClearReserveJob(FGsAIReserveJob* In_removeJob)
{
	if (In_removeJob == nullptr)
	{
		return;
	}

#ifdef TEST_AI_RETURN
	if (In_removeJob->_actionType == EGsAIActionType::MOVE_TO_AUTO_START_POS)
	{
		GSLOG(Error, TEXT("MOVE_TO_AUTO_START_POS cleared !!"));
	}
#endif
	
	In_removeJob->ClearData();
	delete In_removeJob;
	
	_arrayReserveJob.Remove(In_removeJob);
}
void FGsAIReserveHandler::ClearAllJobByType(EGsAIActionType In_type)
{
	TArray<FGsAIReserveJob*> arrayDelIter;

	for (int i = 0; i < _arrayReserveJob.Num(); ++i)
	{
		FGsAIReserveJob* iter = nullptr;
		iter = _arrayReserveJob[i];
		if (iter == nullptr)
		{
			continue;
		}
		if (iter->_actionType == In_type)
		{			
			arrayDelIter.Add(iter);
		}
	}

	for (FGsAIReserveJob* delIter : arrayDelIter)
	{
		if (delIter != nullptr)
		{
#ifdef TEST_AI_RETURN
			if (delIter->_actionType == EGsAIActionType::MOVE_TO_AUTO_START_POS)
			{
				GSLOG(Error, TEXT("MOVE_TO_AUTO_START_POS cleared !!"));
			}
#endif
			delIter->ClearData();
			delete delIter;
		}

		_arrayReserveJob.Remove(delIter);
	}
}
void FGsAIReserveHandler::AddJob(EGsAIActionType In_type, UGsGameObjectBase* In_target, int In_addIndex)
{
	int addIndex = In_addIndex;
	if (_arrayReserveJob.Num() == 0)
	{
		addIndex = 0;
	}
	FGsAIStateManager* aiStateManager = GSAI()->GetAIStateManager();
	if (aiStateManager == nullptr)
	{
		return;
	}

	const FGsAITask* task = aiStateManager->FindTaskByActionType(In_type);
	// WAIT_SECOND 란 예외
	// 테이블에는 없고(등록 조건 없음)
	// https://jira.com2us.com/jira/browse/C2URWQ-4432
	//if (task == nullptr)
	//{
	//	return;
	//}

	// clear already exist data
	ClearJob(In_type, In_target);

	FGsAIReserveJob* newJob = new FGsAIReserveJob();
	newJob->_actionType = In_type;
	newJob->_targetObj = In_target;

	// WAIT_SECOND 란 예외
	// 테이블에는 없고(등록 조건 없음)
	// https://jira.com2us.com/jira/browse/C2URWQ-4432
	if(task != nullptr)
	{ 
		newJob->_actionConditions = task->_actionConditions;
		newJob->_clearConditions = task->_clearConditions;
		newJob->_isHighPriority = task->_isHighPriority;	
	}

	_arrayReserveJob.Insert(newJob, addIndex);

}

void FGsAIReserveHandler::OnAIModeActive(const IGsMessageParam*)
{
	// clear all job
	ClearAllJob();

	if (FGsAIManager* aiMgr = GSAI())
	{
		aiMgr->SendDebugReserveListUpdate();
	}
}

void FGsAIReserveHandler::OnLocalSpawnMeComplete(const IGsMessageParam*)
{
	// clear all job
	ClearAllJob();

	if (FGsAIManager* aiMgr = GSAI())
	{
		aiMgr->SendDebugReserveListUpdate();
	}
}

void FGsAIReserveHandler::OnAIAddReserveJob(const IGsMessageParam* In_param)
{
	const FGsAIReserveParam* aiReserveClearParam =
		In_param->Cast<const FGsAIReserveParam>();

	if (aiReserveClearParam == nullptr)
	{
		return;
	}

	AddJob(aiReserveClearParam->_actionType,
		aiReserveClearParam->_targetObj,
		aiReserveClearParam->_addIndex);

	if (FGsAIManager* aiMgr = GSAI())
	{
		aiMgr->SendDebugReserveListUpdate();
	}
}

void FGsAIReserveHandler::StartPauseTime(const FVector& In_currentLocalPos)
{
	_pauseStartTime = FDateTime::UtcNow().GetTicks();
	_pauseTime = GData()->GetGlobalData()->_aiAutoMoveFailBlockTime * ETimespan::TicksPerSecond;

	_localPausePose = In_currentLocalPos;

	_isDirtyReserveRetry = false;
}

void FGsAIReserveHandler::UpdatePauseTime(const FVector& In_currentLocalPos)
{
	bool isAutoMode = false;
	if (FGsAIManager* aiMgr = GSAI())
	{
		isAutoMode = aiMgr->IsAIOn();
	}

	if (isAutoMode == false)
	{
		ClearRetry();
		return;
	}

	if (_localPausePose.Equals(In_currentLocalPos) == false)
	{
		_pauseStartTime = FDateTime::UtcNow().GetTicks();
		_localPausePose = In_currentLocalPos;

		ClearRetry();
		return;
	}

	int64 nowTick = FDateTime::UtcNow().GetTicks();
	int64 diffTick = nowTick - _pauseStartTime;

	if (diffTick >= _pauseTime)
	{
		_isDirtyReserveRetry = true;
	}
}

void FGsAIReserveHandler::RetryByAutoPause()
{
	if (_isRetry == false)
	{
		_isRetry = true;
	}

	--_reamainRetryCount;

	// retry end
	if (_reamainRetryCount <= 0)
	{
		ClearRetry();

		// job move to last index
		if (_arrayReserveJob.Num() == 0)
		{
			return;
		}

		FGsAIReserveJob* firstJob = _arrayReserveJob[0];

		// only pause target exist
		if (_arrayReserveJob.Num() == 1)
		{
			ClearRetrySingleJob(firstJob);

			SetBlockTarget(firstJob);
		}
		else
		{		
			bool isSingleJob = false;
			// first job: AUTO_MOVE_STOP_WITH_RESERVE_RETRY-> next job
			if (firstJob->_actionType == EGsAIActionType::AUTO_MOVE_STOP_WITH_RESERVE_RETRY)
			{
				firstJob = _arrayReserveJob[1];
				if (_arrayReserveJob.Num() == 2)
				{
					isSingleJob = true;
				}
			}

			if (isSingleJob == true)
			{
				ClearRetrySingleJob(firstJob);
			}
			else
			{
				_arrayReserveJob.Remove(firstJob);
				_arrayReserveJob.Insert(firstJob, _arrayReserveJob.Num());
			}

			SetBlockTarget(firstJob);
		}

		// 예약리스트 갱신됨
		if (FGsAIManager* aiMgr = GSAI())
		{
			aiMgr->SendDebugReserveListUpdate();
		}
		
	}	
}
void FGsAIReserveHandler::ClearRetrySingleJob(FGsAIReserveJob* In_job)
{
	if (In_job == nullptr)
	{
		return;
	}

	if (In_job->_actionType == EGsAIActionType::ATTACK_ANYONE)
	{
		// clear target
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

		FGsTargetHandlerLocalPlayer* targetHandler = local->GetCastTarget<FGsTargetHandlerLocalPlayer>();
		if (targetHandler == nullptr)
		{
			return;
		}

		targetHandler->ClearTarget();
	}

	ClearJob(In_job->_actionType, In_job->_targetObj);
}
void FGsAIReserveHandler::SetBlockTarget(FGsAIReserveJob* In_job)
{
	if (In_job == nullptr)
	{
		return;
	}

	if (In_job->_actionType == EGsAIActionType::ATTACK_ANYONE ||
		In_job->_actionType == EGsAIActionType::LOOT_ITEM ||
		In_job->_actionType == EGsAIActionType::LOOT_ITEM_AUTO_OFF ||
		In_job->_actionType == EGsAIActionType::INTERACTION)
	{
		if (In_job->_targetObj != nullptr)
		{
			_blockTargetGameId = In_job->_targetObj->GetGameId();
		}
	}
}

void FGsAIReserveHandler::ClearRetry()
{
	_reamainRetryCount = GData()->GetGlobalData()->_aiTaskRetryCount;
	_isRetry = false;

	_blockTargetGameId = 0;
	_isDirtyReserveRetry = false;
}

// check already exist
bool FGsAIReserveHandler::IsExistTargetObj(UGsGameObjectBase* In_targetObj)
{
	if (_arrayReserveJob.Num() == 0)
	{
		return false;
	}

	for (int i =0 ; i<  _arrayReserveJob.Num(); ++i)
	{
		FGsAIReserveJob* findInfo = _arrayReserveJob[i];
		if (nullptr == findInfo ||
			nullptr == findInfo->_targetObj)
		{
			continue;
		}

		if (findInfo->_targetObj == In_targetObj)
		{
			return true;
		}		
	}
	return false;
}

void FGsAIReserveHandler::OnLocalAutoMoveStop(const IGsMessageParam*)
{
	ClearJob(EGsAIActionType::AUTO_MOVE_STOP, nullptr);
	ClearJob(EGsAIActionType::AUTO_MOVE_STOP_WITH_RESERVE_RETRY, nullptr);
	ClearJob(EGsAIActionType::AUTO_MOVE_STOP_WITH_CLEAR_RESERVE_SKILL, nullptr);

	// 예약리스트 갱신됨
	if (FGsAIManager* aiMgr = GSAI())
	{
		aiMgr->SendDebugReserveListUpdate();
	}

	_isDirtyReserveRetry = false;
}

void FGsAIReserveHandler::PostMakeJobsByEvaluateConditions()
{
	if (_arrayPostAddReserveJob.Num() != 0)
	{
		_arrayReserveJob.Insert(_arrayPostAddReserveJob, _arrayReserveJob.Num());

		_arrayPostAddReserveJob.Empty();
	}

}

// 액션 타입 이미 있는지 체크
bool FGsAIReserveHandler::IsExistAction(EGsAIActionType In_actionType)
{
	for (int i = 0; i < _arrayReserveJob.Num(); ++i)
	{
		FGsAIReserveJob* iter = nullptr;
		iter = _arrayReserveJob[i];
		if (iter == nullptr)
		{
			continue;
		}
		if (iter->_actionType == In_actionType)
		{
			return true;
		}
	}
	return false;
}

// 첫번재 잡이 해당 액션인가
// 없으면 false
bool FGsAIReserveHandler::IsFirstJobAction(EGsAIActionType In_action)
{
	FGsAIReserveJob* firstJob = GetFirstJob();
	if (firstJob == nullptr)
	{
		return false;
	}

	return firstJob->_actionType == In_action;
}