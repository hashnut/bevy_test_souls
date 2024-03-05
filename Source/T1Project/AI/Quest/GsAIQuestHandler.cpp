#include "GsAIQuestHandler.h"

#include "AI/Data/GsQuestAutoMoveTaskInfo.h"

#include "../Data/GsGameClientVersion.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Looting/GsLootingHandlerLocalPlayer.h"

#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "Message/MessageParam/GsAIMessageParam.h"
#include "Message/MessageParam/GsCameraMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"

#include "Management/ScopeGame/GsAIManager.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "AI/ContentsCondition/GsAIContentsConditionDefault.h"
#include "AI/ContentsCondition/GsAIContentsConditionAutoReturn.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "Engine/Public/UnrealEngine.h"

#include "Engine/Public/DrawDebugHelpers.h"

#include "Core/Public/Misc/DateTime.h"
#include "T1Project.h"


void FGsAIQuestHandler::Initialize()
{

	FGsMessageHolder* msg = GMessage();
	_msgDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::INTERACTION_CONTENTS_BY_NPC, this,
			&FGsAIQuestHandler::OnInteractionContents)
	);
	_msgDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::WAYPOINT_MOVE_END, this,
			&FGsAIQuestHandler::OnWaypointMoveEnd)
	);
	_msgDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::AI_QUEST_AUTO_MOVE_STOP, this,
			&FGsAIQuestHandler::OnQuestAutoMoveStop)
	);

	_msgDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::AUTO_MOVE_FAILED, this,
			&FGsAIQuestHandler::OnAutoMoveFailed)
	);
	
	MakeQuestAutoMoveTaskInfo();

	_killCenterCheckSqVal = GData()->GetGlobalData()->_questKillCenterCheckSqVal;
}

// 테스트 정보 만들기
void FGsAIQuestHandler::MakeQuestAutoMoveTaskInfo()
{
	// 데이터 생성
	TArray<FGsQuestAutoMoveTaskData> taskDatas;

	// 1. 강제 실행 task
	FGsQuestAutoMoveTaskData task1;

	// 조건 1: 퀘스트 자동 진행중인지
	task1._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_AUTO_PROGRESSING, true });
	// 조건 2: 예약된 이동 데이터 있는지
	task1._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_RESERVE_QUEST_MOVE_DATA, true });
	// 조건 3: 강제 시작 요청이 있었는지
	task1._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_DIRTY_FORCE_START, true });
	// 조건 4: 서버 락 없는지
	task1._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_SERVER_LOCK, false });

	// 행동: 자동 이동 시작
	task1._actionType = EGsQuestAutoMoveActionType::QUEST_MOVE_START;

	taskDatas.Add(task1);

	// 2. retry  task
	FGsQuestAutoMoveTaskData task2;

	// 조건 1: 퀘스트 자동 진행중인지
	task2._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_AUTO_PROGRESSING, true });
	// 조건 2: 예약된 이동 데이터 있는지
	task2._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_RESERVE_QUEST_MOVE_DATA, true });
	// 조건 3: 로컬 상태가 idle 인지
	task2._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_LOCAL_STATE_IDLE, true });
	// 조건 4: 재시도 가능한 실패인지
	task2._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_RETRY_POSIBBLE_FAILED_RESULT, true });
	// 조건 5: 수동 공격 타겟(죽을때 까지 때릴 넘) 없어야 함
	task2._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_MANUAL_AUTO_TARGET_EXIST, false });
	// 조건 6: 루팅가능한게 없어야함
	task2._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_LOOT_POSSIBLE, false });
	// 조건 7: 서버 락 없는지
	task2._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_SERVER_LOCK, false });

	// 행동: 자동 이동 시작
	task2._actionType = EGsQuestAutoMoveActionType::QUEST_MOVE_START;

	taskDatas.Add(task2);

	// 3. 자동 이동 stop  task
	FGsQuestAutoMoveTaskData task3;

	// 조건 1: 퀘스트 자동 진행중이 아닌지
	task3._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_AUTO_PROGRESSING, false });	
	// 조건 2: 자동 진행 데이터 있는지
	task3._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_RESERVE_QUEST_MOVE_DATA, true });

	// 행동: 자동 이동 멈춤
	task3._actionType = EGsQuestAutoMoveActionType::QUEST_MOVE_STOP;

	taskDatas.Add(task3);

	// 4. npc interaction 타겟 없어서 대기 시작  task
	FGsQuestAutoMoveTaskData task4;

	// 조건 1: 퀘스트 자동 진행중인지
	task4._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_AUTO_PROGRESSING, true });
	// 조건 2: 인터랙션 npc 타겟이 없음
	task4._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_NPC_INTERACTION_TARGET_NOT_EXIST, true });
	// 조건 3: 대기 시작 안했는지
	task4._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_NPC_WAIT_START, false });

	// 행동: 자동 이동 멈춤
	task4._actionType = EGsQuestAutoMoveActionType::NPC_SPAWN_WAIT_START;

	taskDatas.Add(task4);

	// 5. npc interaction 타겟 없어서 대기 끝  task
	FGsQuestAutoMoveTaskData task5;

	// 조건 1: 퀘스트 자동 진행중인지
	task5._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_AUTO_PROGRESSING, true });
	// 조건 2: 인터랙션 npc 타겟이 없음
	task5._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_NPC_INTERACTION_TARGET_NOT_EXIST, true });
	// 조건 3: 대기 시작 했는지
	task5._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_NPC_WAIT_START, true });
	// 조건 4: 대기 시간 끝났는지
	task5._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_NPC_WAIT_TIME_END, true });

	// 행동: 에러 팝업 출력(이동 중지)
	task5._actionType = EGsQuestAutoMoveActionType::SHOW_MOVE_FAILED_ERROR_POPUP;

	taskDatas.Add(task5);

	// 6. 이동 실패 에러 팝업 출력
	FGsQuestAutoMoveTaskData task6;

	// 조건 1: 퀘스트 자동 진행중인지
	task6._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_AUTO_PROGRESSING, true });
	// 조건 2: path 에러면서 취소가 아닌가
	task6._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_PATH_AND_NOT_ABORT_RESULT, true });	

	// 행동: 에러 팝업 출력(이동 중지)
	task6._actionType = EGsQuestAutoMoveActionType::SHOW_MOVE_FAILED_ERROR_POPUP;

	taskDatas.Add(task6);


	// 7. retry  task(NPC 없음)
	FGsQuestAutoMoveTaskData task7;

	// 조건 1: 퀘스트 자동 진행중인지
	task7._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_AUTO_PROGRESSING, true });
	// 조건 2: 예약된 이동 데이터 있는지
	task7._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_RESERVE_QUEST_MOVE_DATA, true });
	// 조건 3: 최근 이동 실패가 있었나
	task7._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_DIRTY_MOVE_FAILED, true });
	// 조건 4: NPC 대기 시작했나
	task7._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_NPC_WAIT_START, true });
	// 조건 5: 대기시간이 안끝났나
	task7._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_NPC_WAIT_TIME_END, false });

	// 행동: 자동 이동 시작
	task7._actionType = EGsQuestAutoMoveActionType::QUEST_MOVE_START;

	taskDatas.Add(task7);

	// 8. quest kill center move
	FGsQuestAutoMoveTaskData task8;
	// 조건 1: 퀘스트 자동 진행중인지
	task8._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_AUTO_PROGRESSING, true });
	// 조건 2: kill quest auto start
	task8._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_QUEST_KILL_AUTO_START, true });
	// 조건 3: no monster arround 
	task8._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_ENEMY_EXIST, false });
	// 조건 4: local fsm idle
	task8._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_LOCAL_STATE_IDLE, true });
	// 조건 5: not in center
	task8._conditions.Add(
		FGsQuestAutoMoveConditionData{ EGsQuestAutoMoveConditionType::IS_QUEST_KILL_CENTER_INSIDE, false });

	// 행동: 자동 이동 시작
	task8._actionType = EGsQuestAutoMoveActionType::QUEST_MONSTER_KILL_CENTER_MOVE;

	taskDatas.Add(task8);


	if (nullptr != _resolver)
	{
		_resolver->Finalize();
		delete _resolver;
		_resolver = nullptr;
	}

	_resolver = new FGsQuestAutoMoveResolver();

	for (FGsQuestAutoMoveTaskData& taskDataIter: taskDatas)
	{
		FGsQuestAutoMoveTask* task = new FGsQuestAutoMoveTask();

		for (FGsQuestAutoMoveConditionData& conditionDataIter: taskDataIter._conditions)
		{
			FGsQuestAutoMoveTask::FGsQuestAutoMoveCondition* condition
				= new FGsQuestAutoMoveTask::FGsQuestAutoMoveCondition();
			condition->_conditionType = conditionDataIter._conditionType;
			condition->_boolVal = conditionDataIter._boolVal;

			task->_conditions.Add(condition);
		}
		FGsQuestAutoMoveTask::FGsQuestAutoMoveAction* action
			= new FGsQuestAutoMoveTask::FGsQuestAutoMoveAction();
		action->_actionType = taskDataIter._actionType;

		task->_action = action;

		_resolver->_tasks.Add(task);
	}

	// condition, action 함수 연결
	for (FGsQuestAutoMoveTask* taskIter: _resolver->_tasks)
	{
		for (FGsQuestAutoMoveTask::FGsQuestAutoMoveCondition* conditionIter: taskIter->_conditions)
		{
			if (conditionIter == nullptr)
			{
				continue;
			}

			switch (conditionIter->_conditionType)
			{
			case EGsQuestAutoMoveConditionType::IS_AUTO_PROGRESSING:
				conditionIter->_conditionFunc = [this, conditionIter]()
				{return conditionIter->_boolVal == IsAutoProgressing(); };
				break;
			case EGsQuestAutoMoveConditionType::IS_RESERVE_QUEST_MOVE_DATA:
				conditionIter->_conditionFunc = [this, conditionIter]()
				{return conditionIter->_boolVal == IsReserveQuestMoveData(); };
				break;
			case EGsQuestAutoMoveConditionType::IS_LOCAL_STATE_IDLE:
				conditionIter->_conditionFunc = [this, conditionIter]()
				{return conditionIter->_boolVal == IsLocalStateIdle(); };
				break;
			case EGsQuestAutoMoveConditionType::IS_DIRTY_FORCE_START:
				conditionIter->_conditionFunc = [this, conditionIter]()
				{return conditionIter->_boolVal == IsDirtyForceStart(); };
				break;
			case EGsQuestAutoMoveConditionType::IS_NPC_INTERACTION_TARGET_NOT_EXIST:
				conditionIter->_conditionFunc = [this, conditionIter]()
				{return conditionIter->_boolVal == IsNPCInteractionTargetNotExist(); };
				break;
			case EGsQuestAutoMoveConditionType::IS_NPC_WAIT_START:
				conditionIter->_conditionFunc = [this, conditionIter]()
				{return conditionIter->_boolVal == IsNPCWaitStart(); };
				break;
			case EGsQuestAutoMoveConditionType::IS_NPC_WAIT_TIME_END:
				conditionIter->_conditionFunc = [this, conditionIter]()
				{return conditionIter->_boolVal == IsNPCWaitTimeOver(); };
				break;
			case EGsQuestAutoMoveConditionType::IS_RETRY_POSIBBLE_FAILED_RESULT:
				conditionIter->_conditionFunc = [this, conditionIter]()
				{return conditionIter->_boolVal == IsRetryPossibleFailedResult(); };
				break;
			case EGsQuestAutoMoveConditionType::IS_PATH_AND_NOT_ABORT_RESULT:
				conditionIter->_conditionFunc = [this, conditionIter]()
				{return conditionIter->_boolVal == IsPathAndNotAbortResult(); };
				break;
			case EGsQuestAutoMoveConditionType::IS_DIRTY_MOVE_FAILED:
				conditionIter->_conditionFunc = [this, conditionIter]()
				{return conditionIter->_boolVal == IsDirtyMoveFailed(); };
				break;
			case EGsQuestAutoMoveConditionType::IS_MANUAL_AUTO_TARGET_EXIST:
				conditionIter->_conditionFunc = [this, conditionIter]()
				{return conditionIter->_boolVal == IsManualAutoTargetExist(); };
				break;
			case EGsQuestAutoMoveConditionType::IS_LOOT_POSSIBLE:
				conditionIter->_conditionFunc = [this, conditionIter]()
				{return conditionIter->_boolVal == IsLootPossible(); };
				break;
			case EGsQuestAutoMoveConditionType::IS_QUEST_KILL_AUTO_START:
				conditionIter->_conditionFunc = [this, conditionIter]()
				{return conditionIter->_boolVal == IsQuestKillAutoStart(); };
				break;
			case EGsQuestAutoMoveConditionType::IS_ENEMY_EXIST:
				conditionIter->_conditionFunc = [this, conditionIter]()
				{return conditionIter->_boolVal == IsEnemyExist(); };
				break;
			case EGsQuestAutoMoveConditionType::IS_QUEST_KILL_CENTER_INSIDE:
				conditionIter->_conditionFunc = [this, conditionIter]()
				{return conditionIter->_boolVal == IsQuestKillCenterInside(); };
				break;
			case EGsQuestAutoMoveConditionType::IS_SERVER_LOCK:
				conditionIter->_conditionFunc = [this, conditionIter]()
				{return conditionIter->_boolVal == IsServerLock(); };
				break;
			}
		}

		if (taskIter->_action == nullptr)
		{
			continue;
		}
		switch (taskIter->_action->_actionType)
		{
		case EGsQuestAutoMoveActionType::QUEST_MOVE_START:
			taskIter->_action->_actionFunc = [this]()
			{	DoQuestMoveStart(); };
			break;
		case EGsQuestAutoMoveActionType::QUEST_MOVE_STOP:
			taskIter->_action->_actionFunc = [this]()
			{	DoQueatMoveStop(); };
			break;
		case EGsQuestAutoMoveActionType::NPC_SPAWN_WAIT_START:
			taskIter->_action->_actionFunc = [this]()
			{	DoNPCSpawnWaitStart(); };
			break;
		case EGsQuestAutoMoveActionType::SHOW_MOVE_FAILED_ERROR_POPUP:
			taskIter->_action->_actionFunc = [this]()
			{	DoShowMoveFailedErrorPopup(); };
			break;
		case EGsQuestAutoMoveActionType::QUEST_MONSTER_KILL_CENTER_MOVE:
			taskIter->_action->_actionFunc = [this]()
			{	DoQuestMonsterKillCenterMove(); };
			break;
		}
	}
	
}
// 캐릭터 세팅
void FGsAIQuestHandler::SetCharacter(UGsGameObjectLocalPlayer* In_local)
{
	_local = In_local;
}
// 캐릭터 지움
void FGsAIQuestHandler::RemoveCharacter()
{
	_local = nullptr;
}
void FGsAIQuestHandler::Finalize()
{
	FGsMessageHolder* msg = GMessage();
	
	if (_msgDelegates.Num() != 0)
	{
		for (auto iter : _msgDelegates)
		{
			msg->GetGameObject().Remove(iter);
		}
		_msgDelegates.Empty();
	}
	
	_local = nullptr;

	if (nullptr != _resolver)
	{
		_resolver->Finalize();
		delete _resolver;
		_resolver = nullptr;
	}
}

// 인터랙션 준비됨 메시지(도착)
void FGsAIQuestHandler::OnInteractionContents(const IGsMessageParam* In_data)
{
	if (_local == nullptr)
	{
		return;
	}

	const FGsInteractionMessageParamContents* interactionParam = 
		In_data->Cast<const FGsInteractionMessageParamContents>();

	FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movement)
	{
		return;
	}

	// 현재 auto 이동중이면서
	// target npc id가 같다면
	if (movement->GetStateAutoMove().GetMovementAutoContentsType() == EGsMovementAutoContentsType::QuestMove &&
		_targetNpcId == interactionParam->_npcTblId)
	{
		movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::None);

		// 퀘스트 이동 끝 노티
		FGsAIQuestMessageParam aiQuestParam(
			_autoActiveQuestId, _questAutoMoveType,
			interactionParam->_spawnId,
			interactionParam->_gameId);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_QUEST_AUTO_MOVE_END, &aiQuestParam);

#ifdef TEST_LOG
		const FString showLog = FString::Printf(TEXT("questId: %llu, questType %i"),
			_autoActiveQuestId, _questAutoMoveType);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, *showLog);
#endif

		ClearData();

	}

}
// 퀘스트 자동 이동 시작
void FGsAIQuestHandler::OnQuestAutoMoveStart(const IGsMessageParam* In_data)
{
	// 기존꺼 클리어 하고 시작
	ClearData();

	const FGsAIQuestAutoMoveMessageParam* autoMoveParam =
		In_data->Cast<const FGsAIQuestAutoMoveMessageParam>();

	_autoActiveQuestId = autoMoveParam->_questId;
	_targetNpcId = autoMoveParam->_npcId;
	_questAutoMoveType = autoMoveParam->_moveType;

	_moveToMapId = autoMoveParam->_mapId;
	_movePos = autoMoveParam->_arrPos;

	_acceptanceRange = autoMoveParam->_range;

	_isDirtyForceMove = true;

	_isQuestKillAutoStart = false;
}
// 퀘스트 이동 시작
void FGsAIQuestHandler::StartQuestAutoMove()
{
	if (_local == nullptr)
	{
		return;
	}

	FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movement)
	{
		return;
	}
	movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::QuestMove);

	// 이동할 거리가 없으면 하지말자
	if (_movePos.Num() == 0)
	{
		return;
	}

	// 강제 이동 dirty 클리어
	_isDirtyForceMove = false;
	// 이동 실패 클리어(이동 다시 시작하니까)
	_isDirtyFailedMove = false;

	FVector singleTargetPos = _movePos[0];
	if (_questAutoMoveType == QuestObjectiveType::INTERACT_NPC)
	{		
		if (FGsInteractionHandlerLocalPlayer* interactionHander =
			_local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>())
		{
			//이전에 인터렉션 하고 있었다면 취소 후 자동 이동
			if (interactionHander->GetInteractionTarget())
			{
				interactionHander->CancelInteration();
			}		
			// npc 대화 이동 개선으로 
			// npc위치로 바로가게 바뀌었는데
			// 현재맵이 다른맵에서는 npc 위치를 클라가 몰라서(client spawn 데이터)
			// 퀘스트 워프 위치로 표시하기(이효민 파트장님과 구두로 협의)
			// https://jira.com2us.com/jira/browse/C2URWQ-5545
			// b1: direct interaction(not use spot pos move)
			interactionHander->TryInteractionNpcTblIdWithMapId(_targetNpcId, _moveToMapId, 
				EGsInteractionMoveType::None, singleTargetPos);
		}
	}
	else if (_questAutoMoveType == QuestObjectiveType::GOTO_POS||
		_questAutoMoveType == QuestObjectiveType::INTERACT_PROP_TOUCH)
	{
		movement->ChangeAutoMoveState().StartMapIdPositions(_local,
			_moveToMapId,
			_movePos, nullptr,
			FGsAutoMoveCompletedOneDelegate::CreateRaw(this, 
				&FGsAIQuestHandler::CallbackQuestMoveFailed));
	}
	else if (_questAutoMoveType == QuestObjectiveType::KILL ||
		_questAutoMoveType == QuestObjectiveType::KILL_CREATURE_TYPE)
	{
		StartQuestKillMove(_moveToMapId, singleTargetPos, _acceptanceRange);
	}


}

// 웨이 포인트 이동 끝 메시지(도착)
void FGsAIQuestHandler::OnWaypointMoveEnd(const IGsMessageParam*)
{
	if (_local == nullptr)
	{
		return;
	}

	FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movement)
	{
		return;
	}

	// 현재 auto 이동중이면서
	// target npc id가 같다면
	if (movement->GetStateAutoMove().GetMovementAutoContentsType() == EGsMovementAutoContentsType::QuestMove)
	{
		movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::None);
		// 퀘스트 이동 끝 노티
		FGsAIQuestMessageParam aiQuestParam(_autoActiveQuestId, _questAutoMoveType);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_QUEST_AUTO_MOVE_END, &aiQuestParam);

#ifdef TEST_LOG
		const FString showLog = FString::Printf(TEXT("questId: %llu, questType: %i"),
			_autoActiveQuestId, _questAutoMoveType);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, *showLog);
#endif

		// 프랍도
		if (_questAutoMoveType == QuestObjectiveType::INTERACT_PROP_TOUCH)
		{
			GSAI()->SetActiveAI(true);
#if WITH_EDITOR
			GSLOG(Log, TEXT("FGsAIQuestHandler::OnWaypointMoveEnd _autoActiveQuestId : %llu"), _autoActiveQuestId);
#endif
		}

		ClearData();
	}
}

// 퀘스트 자동 이동 중지(퀘스트 hud toggle로 끔)
void FGsAIQuestHandler::OnQuestAutoMoveStop(const IGsMessageParam*)
{
	if (_local == nullptr)
	{
		return;
	}

	_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);

	// 예약된 행동들 클리어 해준다
	if (_questAutoMoveType == QuestObjectiveType::INTERACT_NPC)
	{
		if (FGsInteractionHandlerLocalPlayer* interactionHander =
			_local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>())
		{
			interactionHander->ClearReserveInteraction();
		}
	}

	ClearData();
}

// 업데이트
void FGsAIQuestHandler::Update(float In_delta)
{	
	EvaluateConditions();
}

// 데이터 클리어
void FGsAIQuestHandler::ClearData()
{
	_autoActiveQuestId = 0;
	_targetNpcId = 0;
	_moveToMapId = 0;
	_movePos.Empty();

	_moveFailedReason = EGsMoveFailedReason::None;
	_isNPCWaitStart = false;
	_isNPCWaitTimeOver = false;

	_isDirtyForceMove = false;
	_isDirtyFailedMove = false;

	_isQuestKillCenterMove = false;

	_isAutoOffLooting = false;
}


// 퀘스트 이동 실패
void FGsAIQuestHandler::CallbackQuestMoveFailed(const FPathFollowingResult& Result)
{
	FailedAutoMove(EGsMoveFailedReason::ControllerAutoMoveFailed, Result.Code);
}

// 퀘스트 kill 이동 예약된게 있나
bool FGsAIQuestHandler::IsReserveQuestKillMoveData()
{
	return _reserveQuestKillMoveInfo._isReserved;
}

// 예약된 quest kill 이동 진행
void FGsAIQuestHandler::ProcessReserveQuestKillMove()
{
	if (_local == nullptr)
	{
		return;
	}

	if (true == IsInsideQuestKillMovePosRange(_reserveQuestKillMoveInfo._targetMapId,
		_reserveQuestKillMoveInfo._targetPos, _reserveQuestKillMoveInfo._acceptanceRadius))
	{
		ArriveQuestKillMove();
	}
	else
	{
		FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();

		// 인터랙션 이동(pos) 시작때 나머지 reserve 데이터 삭제(현재는 waypoint, input handler (터치 이동), interaciton handler 이동만 처리)
		// https://jira.com2us.com/jira/browse/C2URWQ-6334
		_local->ClearReserveDataByReserveWork(nullptr);
		
		movement->ChangeAutoMoveState().StartMapIdPos(_local,
			_reserveQuestKillMoveInfo._targetMapId,
			_reserveQuestKillMoveInfo._targetPos, 
			_reserveQuestKillMoveInfo._acceptanceRadius,
			FGsAutoMoveCompletedDelegate::CreateRaw(this,
				&FGsAIQuestHandler::CallbackQuestKillMoveEnd),
			FGsAutoMoveCompletedOneDelegate::CreateRaw(this,
				&FGsAIQuestHandler::CallbackQuestKillMoveFailed));
	}
}

void FGsAIQuestHandler::ProcessReserveQuestKillCenterMove()
{
	if (_local == nullptr)
	{
		return;
	}

	if (true == IsInsideQuestKillCenterMovePosRange(
		_reserveQuestKillCenterMoveInfo._targetPos, _reserveQuestKillCenterMoveInfo._acceptanceRadius))
	{
		ArriveQuestKillCenterMove();
	}
	else
	{
		FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();

		int currMapId = GLevel()->GetCurrentLevelId();

		movement->ChangeAutoMoveState().StartMapIdPos(_local,
			currMapId,
			_reserveQuestKillCenterMoveInfo._targetPos,
			_reserveQuestKillCenterMoveInfo._acceptanceRadius,
			FGsAutoMoveCompletedDelegate::CreateRaw(this,
				&FGsAIQuestHandler::CallbackQuestKillCenterMoveEnd),
			FGsAutoMoveCompletedOneDelegate::CreateRaw(this,
				&FGsAIQuestHandler::CallbackQuestKillCenterMoveFailed));
	}
}

// 퀘스트 kill 이동 범위 안인가
bool FGsAIQuestHandler::IsInsideQuestKillMovePosRange(int In_mapId, 
	const FVector& In_targetPos, float In_acceptanceRadius)
{
	if (_local == nullptr)
	{
		return false;
	}

	// 현재위치 맵id랑 목표랑 다르면
	int currMapId = GLevel()->GetCurrentLevelId();
	if (In_mapId != currMapId)
	{
		// 범위 밖
		return false;
	}

	float distanceSq = FVector::DistSquared2D(In_targetPos, _local->GetLocation());
	if (distanceSq <= In_acceptanceRadius * In_acceptanceRadius)
	{
		return true;
	}
	return false;
}

bool FGsAIQuestHandler::IsInsideQuestKillCenterMovePosRange(const FVector& In_targetPos,
	float In_acceptanceRadius)
{
	if (_local == nullptr)
	{
		return false;
	}

	float distanceSq = FVector::DistSquared2D(In_targetPos, _local->GetLocation());
	float totalRadius = In_acceptanceRadius + _local->GetData()->GetScaledCapsuleRadius();
	if (distanceSq <= totalRadius * totalRadius)
	{
		return true;
	}
	return false;
}

// 예약된 정보 클리어
void FGsAIQuestHandler::ClearReserveQuestKillMoveData()
{
	_reserveQuestKillMoveInfo._isReserved = false;
}
void FGsAIQuestHandler::ClearReserveQuestKillCenterMoveData()
{
	_reserveQuestKillCenterMoveInfo._isReserved = false;
}
// 퀘스트 kill 이동 시작
void FGsAIQuestHandler::StartQuestKillMove(
	int In_mapId, const FVector& In_pos, float In_acceptanceRadius)
{
	ClearReserveQuestKillMoveData();

	_reserveQuestKillMoveInfo._isReserved = true;
	_reserveQuestKillMoveInfo._targetMapId = In_mapId;
	_reserveQuestKillMoveInfo._targetPos = In_pos;
	_reserveQuestKillMoveInfo._acceptanceRadius = In_acceptanceRadius;

	ProcessReserveQuestKillMove();
}

void FGsAIQuestHandler::StartQuestKillCenterMove(const FVector& In_pos, float In_acceptanceRadius)
{
	ClearReserveQuestKillCenterMoveData();

	_reserveQuestKillCenterMoveInfo._isReserved = true;
	_reserveQuestKillCenterMoveInfo._targetPos = In_pos;
	_reserveQuestKillCenterMoveInfo._acceptanceRadius = In_acceptanceRadius;

	ProcessReserveQuestKillCenterMove();
}

// 퀘스트 kill 이동 성공
void FGsAIQuestHandler::CallbackQuestKillMoveEnd()
{
	ArriveQuestKillMove();
	if (_local == nullptr)
	{
		return;
	}
	FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movement)
	{
		return;
	}
	movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::None);
}

void FGsAIQuestHandler::CallbackQuestKillCenterMoveEnd()
{
	ArriveQuestKillCenterMove();
	if (_local == nullptr)
	{
		return;
	}
	FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movement)
	{
		return;
	}
	movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::None);
}

// 퀘스트 kill 이동 도착 처리
void FGsAIQuestHandler::ArriveQuestKillMove()
{
	ClearReserveQuestKillMoveData();

	if (_local == nullptr)
	{
		return;
	}

	// auto move 멈춤
	_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);

	// https://jira.com2us.com/jira/browse/C2URWQ-3757
	FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movement)
	{
		return;
	}

	// 현재 auto 이동중이면서
	if (movement->GetStateAutoMove().GetMovementAutoContentsType() == EGsMovementAutoContentsType::QuestMove)
	{
		movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::None);
	}


	// 퀘스트 이동 끝 노티
	FGsAIQuestMessageParam aiQuestParam(_autoActiveQuestId, _questAutoMoveType);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_QUEST_AUTO_MOVE_END, &aiQuestParam);


	GSAI()->SetActiveAI(true);
#if WITH_EDITOR
	GSLOG(Log, TEXT("FGsAIQuestHandler::CallbackQuestKillMoveEnd _autoActiveQuestId : %llu"), _autoActiveQuestId);
#endif

	_isQuestKillAutoStart = true;
	if (_movePos.Num() != 0)
	{
		_questKillCenterPos = _movePos[0];
	}

	ClearData();
}

void FGsAIQuestHandler::ArriveQuestKillCenterMove()
{
	ClearReserveQuestKillCenterMoveData();

	if (_local == nullptr)
	{
		return;
	}

	// auto move 멈춤
	_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);


	GSAI()->SetActiveAI(true);
	

	_isQuestKillCenterMove = false;

}
// 퀘스트 kill 이동 실패
void FGsAIQuestHandler::CallbackQuestKillMoveFailed(const FPathFollowingResult& Result)
{
	ClearReserveQuestKillMoveData();
	if (_local == nullptr)
	{
		return;
	}
	// auto move 멈춤
	_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);

	FailedAutoMove(EGsMoveFailedReason::ControllerAutoMoveFailed, Result.Code);
	
}
void FGsAIQuestHandler::CallbackQuestKillCenterMoveFailed(const FPathFollowingResult& Result)
{
	ClearReserveQuestKillCenterMoveData();
	if (_local == nullptr)
	{
		return;
	}
	// auto move 멈춤
	_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);

	GSAI()->SetActiveAI(true);

	_isQuestKillCenterMove = false;
}

// 조건 함수
// 퀘스트 자동 진행중인가
bool FGsAIQuestHandler::IsAutoProgressing()
{
	return GSQuest()->IsAutoProgressing();
}
// 예약된 퀘스트 이동 데이터 있는가
bool FGsAIQuestHandler::IsReserveQuestMoveData()
{
	return (_autoActiveQuestId != 0) ? true : false;
}
// 로컬 상태 idle 인가
bool FGsAIQuestHandler::IsLocalStateIdle()
{
	return _local->GetBaseFSM()->IsCompleteIdleState();
}
// 강제 이동 요청 있었는가
bool FGsAIQuestHandler::IsDirtyForceStart()
{
	return _isDirtyForceMove;
}
// 인터랙션 대상 npc 없는지
bool FGsAIQuestHandler::IsNPCInteractionTargetNotExist()
{
	return (_moveFailedReason == EGsMoveFailedReason::InteractionNPCNotExist) ? true : false;
}

// 취소에 의한 실패인가
bool FGsAIQuestHandler::IsAbortFaield()
{
	if (_moveFailedReason == EGsMoveFailedReason::ControllerAutoMoveFailed)
	{
		if (_moveFailedPathResult == EPathFollowingResult::Aborted)
		{
			return true;
		}
	}
	return false;
}

// 재시도 가능한 실패 결과인지
bool FGsAIQuestHandler::IsRetryPossibleFailedResult()
{
	// 1. 취소에 의한건가(상태 이상)	
	bool isAbort = IsAbortFaield();
	// 2. npc 대기에 의한건가(대기시작과 시간끝전)
	// 3. auto off 루팅이 있었나
	return isAbort || (_isNPCWaitStart&& !_isNPCWaitTimeOver) || _isAutoOffLooting;
}

// npc 대기 시작 했는가
bool FGsAIQuestHandler::IsNPCWaitStart()
{
	return _isNPCWaitStart;
}

// npc 대기시간 끝났는지
bool FGsAIQuestHandler::IsNPCWaitTimeOver()
{
	return _isNPCWaitTimeOver;
}

// path 에러고 취소가 아닌지
bool FGsAIQuestHandler::IsPathAndNotAbortResult()
{
	if (_moveFailedReason == EGsMoveFailedReason::ControllerAutoMoveFailed)
	{
		if (_moveFailedPathResult != EPathFollowingResult::Aborted)
		{
			return true;
		}
	}
	return false;
}
// 최근 이동 실패 한적이 있나
bool FGsAIQuestHandler::IsDirtyMoveFailed()
{
	return _isDirtyFailedMove;
}

// 수동 공격 타겟(죽을때 까지 때릴)이 있는지
bool FGsAIQuestHandler::IsManualAutoTargetExist()
{
	if (nullptr == _local)
	{
		return false;
	}
	FGsTargetHandlerLocalPlayer* targetHandler = _local->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	if (targetHandler && targetHandler->GetManualModeAutoTarget())
	{
		return true;
	}
	return false;
}
// check loot possible
bool FGsAIQuestHandler::IsLootPossible()
{
	if (_local == nullptr)
	{
		return false;
	}

	bool isAutoMode = false;
	if (FGsAIManager* aiMgr = GSAI())
	{
		isAutoMode = aiMgr->IsAIOn();
	}
	bool isQuestAutoProgressing = false;
	if (UGsQuestManager* questManager = GSQuest())
	{
		isQuestAutoProgressing = questManager->IsAutoProgressing();
	}
	// 이전 루팅행동이 있으면 안함(루팅 이동 데이터 있는지 체크)
	bool isReserveLooting = false;
	if (FGsLootingHandlerLocalPlayer* lootingHandler = _local->GetLootingHandler())
	{
		if (lootingHandler->IsReserveLootng())
		{
			isReserveLooting = true;
		}
	}


	// 퀘스트 완료후 이동할때 못주운 drop item 있는지
	// 퀘스트 진행중일때임
	bool isMissingMyItemExist = false;
	if (false == isAutoMode &&
		true == isQuestAutoProgressing)
	{
		isMissingMyItemExist = false;
		if (FGsAIManager* aiMgr = GSAI())
		{
			isMissingMyItemExist = GSAI()->GetContentsConditionDefault()->GetIsMyMissingItemExist();
		}
	}
	// check target have
	bool isHaveTarget = false;
	if (FGsTargetHandlerLocalPlayer* targetHandler = _local->GetCastTarget<FGsTargetHandlerLocalPlayer>())
	{
		if (UGsGameObjectBase* targetObj = targetHandler->GetTarget())
		{
			isHaveTarget = true;
		}
	}

	if ( true == isMissingMyItemExist &&
		true == GSAI()->GetIsLootTargetExistAndPossible() &&
		false == isReserveLooting &&
		false == GSAI()->GetIsAutoLootCoolTime() 
		//&&isHaveTarget == false
		)
	{
		return true;
	}

	return false;
}

// 행동 함수
// 퀘스트 이동 시작
void FGsAIQuestHandler::DoQuestMoveStart()
{
	StartQuestAutoMove();
}
// 퀘스트 이동 정지
void FGsAIQuestHandler::DoQueatMoveStop()
{
	if (_local == nullptr)
	{
		return;
	}

	_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);

	ClearData();
}
// npc 스폰 대기 시작
void FGsAIQuestHandler::DoNPCSpawnWaitStart()
{
	_isNPCWaitStart = true;
	_npcWaitStartTime = FDateTime::UtcNow().GetTicks();

	// 재시도 시작
	StartQuestAutoMove();
}
// 이동 실패고 에러 팝업 출력(이동 중지)
void FGsAIQuestHandler::DoShowMoveFailedErrorPopup()
{
	if (_local == nullptr)
	{
		return;
	}

	_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);
	
	// 실패 정보 팝업 출력
	// 에러팝업
	FString failTickerString;
	switch (_moveFailedReason)
	{
	case EGsMoveFailedReason::ControllerAutoMoveFailed:
	{
		switch (_moveFailedPathResult)
		{
		case EPathFollowingResult::Type::Invalid:
		{
			failTickerString =
				FString::Printf(TEXT("AutoMove fail from pathResult: %d"), _moveFailedPathResult);
		}
		break;
		default:
			break;
		}
	}
	break;
	case EGsMoveFailedReason::InteractionNPCNotExist:
	{
		failTickerString =
			FString::Printf(TEXT("find fail from npc tbl id: %d"), _targetNpcId);
	}
	break;
	default:
		break;
	}
	// 인터랙션 npc 타겟 없을때 나오는 팝업 에디터에서만 나오게 처리
	// https://jira.com2us.com/jira/browse/C2URWQ-2208
#if WITH_EDITOR
	if (!failTickerString.IsEmpty())
	{
		FText failTickerText = FText::FromString(failTickerString);
		FGsUIHelper::TrayMessageTicker(failTickerText);
		FGsUIHelper::PopupOK(failTickerText);
	}
#endif

	// 데이터 클리어
	ClearData();

	// 퀘스트 쪽에 stop 요청
	if (FGsQuestHandler* questHandler = _local->GetQuestHandler())
	{
		questHandler->SetAutoMoveStop();
	}
}

// 자동이동 실패(인터랙션에서 쏘는 메시지)
void FGsAIQuestHandler::OnAutoMoveFailed(const IGsMessageParam* In_data)
{
	// 퀘스트 자동 진행에 의한게 아니면 안함
	if (GSQuest()->IsAutoProgressing() == false)
	{
		return;
	}

	const FGsMoveFailedMessageParam* failedParam = In_data->Cast<const FGsMoveFailedMessageParam>();
	if (failedParam == nullptr)
	{
		return;
	}
	

	FailedAutoMove(failedParam->_moveFailedReason, failedParam->_pathResult);
}

// 이동 실패
void FGsAIQuestHandler::FailedAutoMove(EGsMoveFailedReason In_reason, EPathFollowingResult::Type In_pathResult)
{
	_isDirtyFailedMove = true;

	_moveFailedReason = In_reason;
	_moveFailedPathResult = In_pathResult;

	_moveFailedTime = FDateTime::UtcNow().GetTicks();

	if (_isNPCWaitStart == true)
	{
		int64 timeDiff =
			_moveFailedTime - _npcWaitStartTime;

		static float maxWaitTime = 
			GData()->GetGlobalData()->_questAutoMoveNPCWaitRetryTime * ETimespan::TicksPerSecond;

		if (timeDiff >= maxWaitTime)
		{
			_isNPCWaitTimeOver = true;
		}
	}
}

// task 체크
void FGsAIQuestHandler::EvaluateConditions()
{
	if (nullptr == _resolver)
	{
		return;
	}

	for (FGsQuestAutoMoveTask* task: _resolver->_tasks)
	{
		if (true == EvaluateCondition(task))
		{
			// 고만혀
			break;
		}
	}
}

// task 체크
bool FGsAIQuestHandler::EvaluateCondition(const FGsQuestAutoMoveTask* In_task)
{
	if (In_task == nullptr)
	{
		return false;
	}

	bool isPassed = true;

	for (FGsQuestAutoMoveTask::FGsQuestAutoMoveCondition* iter: In_task->_conditions)
	{
		// 하나라도 만족 못하면 통과 못함
		if (nullptr == iter || nullptr == iter->_conditionFunc ||
			false == iter->_conditionFunc())
		{
			isPassed = false;
			break;
		} 
	}

	if (true == isPassed &&
		nullptr != In_task->_action &&
		nullptr != In_task->_action->_actionFunc)
	{
		In_task->_action->_actionFunc();
		return true;
	}
	return false;
}

bool FGsAIQuestHandler::IsQuestKillAutoStart()
{
	return _isQuestKillAutoStart;
}
bool FGsAIQuestHandler::IsEnemyExist()
{
	bool isInSearchRangeAnyoneEnemy =
		GSAI()->GetContentsConditionDefault()->IsInSearchRangeAnyoneEnemy();
	return isInSearchRangeAnyoneEnemy;
}

// quest monster kill center move
void FGsAIQuestHandler::DoQuestMonsterKillCenterMove()
{

	GSAI()->SetActiveAI(false);

	if (_local == nullptr)
	{
		return;
	}

	FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movement)
	{
		return;
	}
	movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::QuestKillCenterMove);

	_isQuestKillCenterMove = true;

	float newDist = 10.0f;
	StartQuestKillCenterMove( _questKillCenterPos, newDist);
	
	if (_isDrawKillCenterMove == true)
	{
		// quest kill center pos
		DrawDebugSphere(_local->GetWorld(), _questKillCenterPos, 10.0f, 8, FColor::Blue, false, 10.0f);
		DrawDebugSphere(_local->GetWorld(), _questKillCenterPos, _acceptanceRange, 8, FColor::Magenta, false, 10.0f);
	}
}
bool FGsAIQuestHandler::IsQuestKillCenterInside()
{
	if (_local == nullptr)
	{
		return false;
	}
	

	FVector autoStartPos =
		GSAI()->GetContentsConditionAutoReturn()->GetAutoStartPos();

	float distanceSq = FVector::DistSquared2D(_questKillCenterPos, autoStartPos);
	if (distanceSq <= _killCenterCheckSqVal)
	{
		return true;
	}
	return false;
}

bool FGsAIQuestHandler::IsServerLock()
{
	if (_local == nullptr)
	{
		return true;
	}

	// check sever move lock
	bool isServerLock =
		_local->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT);
	
	return isServerLock;
}
