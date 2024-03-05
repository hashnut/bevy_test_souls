#include "GsArenaManager.h"

#include "Input/GsInputEventMsgBase.h"

#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Management/GsMessageHolder.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageContentHud.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "Net/GsNetSendServiceWorld.h"

#include "UTIL/GsTableUtil.h"

#include "Shared/Client/SharedEnums/PD_ResultEnum.h"




void FGsArenaManager::Initialize()
{
	ResetTargetInfo();
	_arenaResult = DuelResult::NONE;
	_isReconnect = false;

	// 메시지 등록
	FGsMessageHolder* msg = GMessage();
	MStageParam& msgStage = msg->GetStageParam();
	_msgStageHandleList.Emplace(msgStage.AddRaw(MessageStage::ENTER_ARENA_CALLBACK,
		this, &FGsArenaManager::OnEnterArena));
	_msgStageHandleList.Emplace(msgStage.AddRaw(MessageStage::LEAVE_ARENA_CALLBACK,
		this, &FGsArenaManager::OnLeaveArena));
	_msgStageHandleList.Emplace(msgStage.AddRaw(MessageStage::ENTER_ARENA_READY_CALLBACK,
		this, &FGsArenaManager::OnEnterArenaReady));
	_msgStageHandleList.Emplace(msgStage.AddRaw(MessageStage::LEAVE_ARENA_READY_CALLBACK,
		this, &FGsArenaManager::OnLeaveArenaReady));

	MGameObject& msgGameObject = msg->GetGameObject();
	_msgGameObjectHandleList.Emplace(msgGameObject.AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, 
		this, &FGsArenaManager::OnLocalSpawnMeComplete));

	MSystem& msgSystem = msg->GetSystem();
	_msgSystemHandleList.Emplace(msgSystem.AddRaw(MessageSystem::RECONNECT_END,
		this, &FGsArenaManager::OnReconnectEnd));
	_msgSystemHandleList.Emplace(msgSystem.AddRaw(MessageSystem::BEFORE_GAME_TO_LOBBY,
		this, &FGsArenaManager::OnBeforeGameToLobby));

	_msgInvasionHandlerList.Empty();
	_msgInvasionHandlerList.Add(msg->GetInvasion().AddRaw(MessageInvasion::INVAION_START,
		this, &FGsArenaManager::OnInvasionStart));

	_arenaHandler.Initialize();
}

void FGsArenaManager::Finalize()
{
	ResetTargetInfo();
	_arenaResult = DuelResult::NONE;

	// 메시지 해제
	FGsMessageHolder* msg = GMessage();

	MStageParam& msgStage = msg->GetStageParam();
	for (MsgStageHandle& msgStageHandle : _msgStageHandleList)
	{
		msgStage.Remove(msgStageHandle);
	}
	_msgStageHandleList.Empty();

	MGameObject& msgGameObject = msg->GetGameObject();
	for (MsgGameObjHandle& msgGameObjectHandle : _msgGameObjectHandleList)
	{
		msgGameObject.Remove(msgGameObjectHandle);
	}
	_msgGameObjectHandleList.Empty();

	MSystem& msgSystem = msg->GetSystem();
	for (MsgSystemHandle& msgSystemHandle : _msgSystemHandleList)
	{
		msgSystem.Remove(msgSystemHandle);
	}
	_msgSystemHandleList.Empty();

	MInvasion& msgInvasion = msg->GetInvasion();
	for (TPair<MessageInvasion, FDelegateHandle>& iter : _msgInvasionHandlerList)
	{
		msgInvasion.Remove(iter);
	}
	_msgInvasionHandlerList.Empty();

	_arenaHandler.Finalize();
}

FGsArenaHandler* FGsArenaManager::GetArenaHandler()
{
	return &_arenaHandler;
}

void FGsArenaManager::SetTargetInfo(UserDBId InUserDBId, Level InLevel, FString InUserName, bool InIsVisible)
{
	_targetUser.SetData(InUserDBId, InLevel, InUserName, InIsVisible);
}

void FGsArenaManager::ResetTargetInfo()
{
	_targetUser.Reset();

	SetArenaOutState(EGsArenaOutState::NONE);
	SetArenaInState(EGsArenaInState::NONE);
}

void FGsArenaManager::SetArenaInState(EGsArenaInState InArenaInState)
{
	if (static_cast<uint8>(InArenaInState) >= static_cast<uint8>(_ArenaInState) ||
		static_cast<uint8>(InArenaInState) == static_cast<uint8>(EGsArenaInState::NONE))
	{
		_ArenaInState = InArenaInState;

		_arenaHandler.SetArenaInState(InArenaInState);
	}
}

void FGsArenaManager::SetArenaOutState(EGsArenaOutState InArenaOutState)
{
	_arenaOutState = InArenaOutState;

	_arenaHandler.SetArenaOutState(InArenaOutState);
}

void FGsArenaManager::SetArenaResult(DuelResult InResult)
{
	_arenaResult = InResult;
}

void FGsArenaManager::SetArenaStartTime(time_t InStartTime)
{
	_arenaStartTime = InStartTime;
}

void FGsArenaManager::SetCountDownStartTime(time_t InStartTime)
{
	_arenaCountDownStartTime = InStartTime;
}

void FGsArenaManager::SetEnteredArenaData(const FGsSchemaArenaData* InArenaData)
{
	_enteredArenaData = InArenaData;
}

const FGsSchemaArenaData* FGsArenaManager::GetArenaData()
{
	int32 mapId = GLevel()->GetCurrentLevelId();
	return UGsTableUtil::FindRowById<UGsTableArenaData, FGsSchemaArenaData>(mapId);
}

bool FGsArenaManager::IsArenaStarted()
{
	return (_ArenaInState == EGsArenaInState::ARENA_START);
}

bool FGsArenaManager::IsArenaEnded()
{
	return (_ArenaInState == EGsArenaInState::ARENA_END);
}

bool FGsArenaManager::IsInArena()
{
	return (_ArenaInState == EGsArenaInState::COUNT_DOWN) ||
		(_ArenaInState == EGsArenaInState::ARENA_START) ||
		(_ArenaInState == EGsArenaInState::ARENA_END);
}

bool FGsArenaManager::IsTargetUser(const FString& InTargetUserName)
{
	return (InTargetUserName == _targetUser._targetUserName);
}

void FGsArenaManager::ClearArenaRequest()
{
	if (_arenaOutState != EGsArenaOutState::ENTERED)
	{
		_arenaHandler.CancelArenaRequest(false);
	}
}

void FGsArenaManager::OnEnterArena(const IGsMessageParam* InParam)
{

}

void FGsArenaManager::OnLeaveArena(const IGsMessageParam* InParam)
{
	if (_isReconnect)
		return;

	// 대결 중(NONE이 아닌)에 결과를 받지 않았을 때 나오면 패배처리
	if (_ArenaInState != EGsArenaInState::NONE && false == IsArenaEnded())
	{
		_arenaResult = DuelResult::LOSE;
	}

	ResetTargetInfo();
}

void FGsArenaManager::OnEnterArenaReady(const IGsMessageParam* InParam)
{

}

void FGsArenaManager::OnLeaveArenaReady(const IGsMessageParam* InParam)
{
	if (_isReconnect)
		return;

	_arenaHandler.CancelArenaRequest(true);
}

void FGsArenaManager::OnReconnectEnd()
{
	_isReconnect = true;
}

void FGsArenaManager::OnLocalSpawnMeComplete(const IGsMessageParam* InParam)
{
	// 부활 후 졌다는 기록이 있으면 출력
	_arenaHandler.PlayArenaResultIfLose();

	if (_arenaOutState != EGsArenaOutState::ENTERED)
	{
		_arenaHandler.CancelArenaRequest(false);
	}

	_isReconnect = false;
}

void FGsArenaManager::OnBeforeGameToLobby()
{
	ClearArenaRequest();
}

void FGsArenaManager::OnInvasionStart(const IGsMessageParam* InParam)
{
	// 침공 시작 시 요청한 대결을 취소한다
	if (GetArenaOutState() == EGsArenaOutState::REQUEST)
	{
		FGsNetSendServiceWorld::SendReqDuelInviteCancel();
		ResetTargetInfo();
		GMessage()->GetArena().SendMessage(MessageContentArena::CLOSE_ARENA_UI, nullptr);
	}
}