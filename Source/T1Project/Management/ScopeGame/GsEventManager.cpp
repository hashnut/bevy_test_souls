#include "GsEventManager.h"

#include "Runtime/Core/Public/Delegates/IDelegateInstance.h"
#include "Runtime/CoreUObject/Public/UObject/Object.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectBase.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"

#if WITH_DEV_AUTOMATION_TESTS
#include "Runtime/Core/Public/Misc/AutomationTest.h"
#endif

#include "DataSchema/GsSchemaEnums.h"
#include "Event/EventAction/GsSchemaEventAction.h"
#include "Event/EventAction/GsSchemaEventActionGroup.h"
#include "Event/GsEventActionImmediate.h"
#include "Event/GsEventActionSequencer.h"
#include "GsTableManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeGlobal.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageSystem.h"
#include "Message/MessageParam/GsEventMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Sound/GsSoundMixerController.h"

UGsEventManager::FGsInstantSoundPlayer::FGsInstantSoundPlayer()
{
	UGsSoundManager* SoundManager = GSound();
	if (nullptr == SoundManager)
	{
		GSLOG(Error, TEXT("nullptr == SoundManager"));
		return;
	}

	UGsSoundMixerController* SoundMixerController = SoundManager->GetOrCreateSoundMixerController();
	if (nullptr == SoundMixerController)
	{
		GSLOG(Error, TEXT("nullptr == SoundMixerController"));
		return;
	}

	// 사운드 믹서 변경
	SoundMixerController->OnMixChanged(EGsSoundMixType::EventAction);
	_soundMixerChanged = true;
}

UGsEventManager::FGsInstantSoundPlayer::~FGsInstantSoundPlayer()
{
	if (!_soundMixerChanged)
	{
		GSLOG(Warning, TEXT("!_soundMixerChanged, do nothing"));
		return;
	}

	UGsSoundManager* SoundManager = GSound();
	if (nullptr == SoundManager)
	{
		GSLOG(Error, TEXT("nullptr == SoundManager"));
		return;
	}

	UGsSoundMixerController* SoundMixerController = SoundManager->GetOrCreateSoundMixerController();
	if (nullptr == SoundMixerController)
	{
		GSLOG(Error, TEXT("nullptr == SoundMixerController"));
		return;
	}

	// 플레이중인 모든 사운드 정지
	UGsSoundPlayer* SoundPlayer = GSoundPlayer();
	if (nullptr == SoundPlayer)
	{
		GSLOG(Error, TEXT("nullptr == SoundPlayer"));
	}
	else
	{
		for (int32 i = _playingSoundHandles.Num() - 1; 0 <= i; --i)
		{
			SoundPlayer->StopSound(_playingSoundHandles[i].Value);
		}
	}

	// 사운드 믹서 복구
	SoundMixerController->OffMixMode(EGsSoundMixType::EventAction);
}

bool UGsEventManager::FGsInstantSoundPlayer::EnqueuePlayingSound(const FName InSoundResourceRowName, const uint64 InSoundHandle)
{
	if (!_soundMixerChanged)
	{
		GSLOG(Warning, TEXT("!_soundMixerChanged, do nothing"));
		return false;
	}

	_playingSoundHandles.Emplace(InSoundResourceRowName, InSoundHandle);
	return true;
}

uint64 UGsEventManager::FGsInstantSoundPlayer::DequeuePlayingSound(const FName InSoundResourceRowName)
{
	int32 FoundIndex = -1;
	uint64 FoundSoundHandle = 0; // 0 is invalid sound handle
	const int32 Count = _playingSoundHandles.Num();
	for (int32 i = 0; i < Count; ++i)
	{
		const TPair<FName, uint64>& PlayingSoundHandle = _playingSoundHandles[i];
		if (PlayingSoundHandle.Key == InSoundResourceRowName)
		{
			FoundIndex = i;
			FoundSoundHandle = PlayingSoundHandle.Value;
			break;
		}
	}

	if (!_playingSoundHandles.IsValidIndex(FoundIndex))
	{
		GSLOG(Warning, TEXT("!_playingSoundHandles.IsValidIndex(%d)"), FoundIndex);
	}
	else
	{
		_playingSoundHandles.RemoveAt(FoundIndex);
	}

	return FoundSoundHandle;
}


void UGsEventManager::Initialize()
{
	ClearReconnectionStatus();

	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddUObject(this, &UGsEventManager::OnApplicationWillEnterBackground);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddUObject(this, &UGsEventManager::OnApplicationHasEnteredForeground);

	_eventActionSequencer = NewObject<UGsEventActionSequencer>();
	_eventActionSequencer->Initialize();
	_eventActionSequencer->_onEventActionFinished.BindUObject(this, &UGsEventManager::OnEventActionFinished);

	_eventActionImmediate = NewObject<UGsEventActionImmediate>();
	_eventActionImmediate->Initialize();

	// 이벤트 메세지 등록
	{
		if (0 != _listDelegateContentEvent.Num())
		{
			GSLOG(Warning, TEXT("0 != _listDelegateContentEvent.Num(): Finalize() 호출 없이 Initialize() 가 재호출 되었습니다."));
		}

		MEvent& EventHandler = GMessage()->GetEvent();
		for (const TPair<MessageContentEvent, FDelegateHandle>& It : _listDelegateContentEvent)
		{
			EventHandler.Remove(It);
		}
		_listDelegateContentEvent.Empty();
		_listDelegateContentEvent.Emplace(EventHandler.AddUObject(MessageContentEvent::EVENT_ACTION_PLAY, this, &UGsEventManager::OnRequestPlayEventAction));
		_listDelegateContentEvent.Emplace(EventHandler.AddUObject(MessageContentEvent::LAST_EVENT_ACTION_FINISHED, this, &UGsEventManager::OnLastEventActionFinished));
		_listDelegateContentEvent.Emplace(EventHandler.AddUObject(MessageContentEvent::ALL_CLEAR_EVENT_ACTION_IMMEDIATE, this, &UGsEventManager::EventActionAllClearImmediate));
	}

	// 시스템 메세지 등록
	{
		if (0 != _listDelegateSystem.Num())
		{
			GSLOG(Warning, TEXT("0 != _listDelegateContentEvent.Num(): Finalize() 호출 없이 Initialize() 가 재호출 되었습니다."));
		}

		MSystem& SystemHandler = GMessage()->GetSystem();
		for (const TPair<MessageSystem, FDelegateHandle>& It : _listDelegateSystem)
		{
			SystemHandler.Remove(It);
		}
		_listDelegateSystem.Empty();
		_listDelegateSystem.Emplace(SystemHandler.AddUObject(MessageSystem::ENTER_INGAME, this, &UGsEventManager::OnEnterIngame));
		_listDelegateSystem.Emplace(SystemHandler.AddUObject(MessageSystem::RECONNECT_START, this, &UGsEventManager::OnReconnectStart));
		_listDelegateSystem.Emplace(SystemHandler.AddUObject(MessageSystem::RECONNECT_END, this, &UGsEventManager::OnReconnectEnd));
	}
}

void UGsEventManager::Finalize()
{
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.RemoveAll(this);

	FGsMessageHolder* msg = GMessage();

	{
		MEvent& EventHandler = msg->GetEvent();
		for (const TPair<MessageContentEvent, FDelegateHandle>& It : _listDelegateContentEvent)
		{
			EventHandler.Remove(It);
		}
		_listDelegateContentEvent.Empty();
	}

	{
		MSystem& SystemHandler = msg->GetSystem();
		for (const TPair<MessageSystem, FDelegateHandle>& It : _listDelegateSystem)
		{
			SystemHandler.Remove(It);
		}
		_listDelegateSystem.Empty();
	}

	_eventActionSequencer->Finalize();
	_eventActionSequencer = nullptr;

	_eventActionImmediate->Finalize();
	_eventActionImmediate = nullptr;
}

void UGsEventManager::FinalizeEventActionGroup()
{
	// 믹서 복구 등
	_soundPlayerInstant.Reset();
}

UGsEventManager::FGsInstantSoundPlayer& UGsEventManager::GetValidInstantSoundPlayer()
{
	if (!_soundPlayerInstant.IsValid())
	{
		_soundPlayerInstant = MakeShared<UGsEventManager::FGsInstantSoundPlayer>();
	}
	return *_soundPlayerInstant;
}

void UGsEventManager::ClearReconnectionStatus()
{
	_reconnecting = false;
	_justReconnectStarted = false;
}

void UGsEventManager::OnEnterIngame()
{
	GSLOG(Log, TEXT("UGsEventManager::OnEnterIngame()"));

	ClearReconnectionStatus();
}

void UGsEventManager::OnReconnectStart()
{
	GSLOG(Log, TEXT("UGsEventManager::OnReconnectStart()"));

	_reconnecting = true;

	// 특수한 이벤트액션은 리커넥션 발생 시 별도 처리가 필요함.
	if (nullptr == _eventActionSequencer || !_eventActionSequencer->IsValidLowLevel() || !_eventActionSequencer->IsPlaying())
	{
		return;
	}

	// 이벤트액션이 진행중에 재연결이 발생했을 때, 플래그 설정. 재연결 성공 후, 이벤트액션 실행할 때, 이 조건에 따라 분기 처리할 수도 있음.
	_justReconnectStarted = true;

	//if (_eventActionSequencer->IsPlayingPauseOnLostConnection())
	//{
		// 현재 진행중인 이벤트액션이 네트워크 연결 해제 시, 일시 정지되는 이벤트액션이라면, 재연결 후 이벤트액션 플레이를 요청해도 아무것도 하지 않기 위해 저장해 둠.
		//_pausedEventActionForReconnection = MakeShared<TPair<EventActionGroupId, EventActionIndex>>(_eventActionSequencer->GetEventActionGroupId(), _eventActionSequencer->GetEventActionIndex());
	//}
	
	GSLOG(Log, TEXT("UGsEventManager::OnReconnectStart(). _eventActionSequencer->GetEventActionGroupId(): %d, _eventActionSequencer->GetEventActionIndex(): %d"), _eventActionSequencer->GetEventActionGroupId(), _eventActionSequencer->GetEventActionIndex());
}

void UGsEventManager::OnReconnectEnd()
{
	_reconnecting = false;
}

bool UGsEventManager::EnqueuePlayingSound(const FName InSoundResourceRowName, const uint64 InSoundHandle)
{
	return GetValidInstantSoundPlayer().EnqueuePlayingSound(InSoundResourceRowName, InSoundHandle);
}

uint64 UGsEventManager::DequeuePlayingSound(const FName InSoundResourceRowName)
{
	return GetValidInstantSoundPlayer().DequeuePlayingSound(InSoundResourceRowName);
}

bool UGsEventManager::EventActionIsPlaying() const
{
	if (nullptr == _eventActionSequencer || !_eventActionSequencer->IsValidLowLevel())
	{
		return false;
	}

	return _eventActionSequencer->IsPlaying();
}

void UGsEventManager::OnRequestPlayEventAction(const IGsMessageParam& InParam)
{
	if (_reconnecting)
	{
		// 재연결 중에는 아무것도 진행시키지 않음
		GSLOG(Warning, TEXT("재연결 중에는 모든 이벤트액션 시작 요청을 무시함."));
		return;
	}

	_lastReqEventActionParam = *InParam.Cast<const FGsReqEventActionParam>();

	GSLOG(Log, TEXT("UGsEventManager::OnRequestPlayEventAction(), _eventActionGroupId: %d, _lastEventActionIndex: %d, IsClientOnlyEventAction(): %d, _isReStart: %d"), _lastReqEventActionParam._eventActionGroupId, _lastReqEventActionParam._lastEventActionIndex, StaticCast<int32>(IsClientOnlyEventAction(_lastReqEventActionParam._eventActionGroupId, _lastReqEventActionParam._lastEventActionIndex)), StaticCast<int32>(_lastReqEventActionParam._isReStart));

	// reconnection 인 경우에는 이전에 이미 보냈으므로, 다시 보내지 않음.
	const bool SkipSendEventActionPacket = _justReconnectStarted;
	_justReconnectStarted = false;

	/*
	if (_justReconnectStarted)
	{
		_justReconnectStarted = false;

		// 이벤트액션인덱스가 INVALID_EVENT_ACTION_INDEX 인 경우는 이벤트액션그룹의 첫번째 이벤트액션을 실행할 때만 수신됨.
		// _justReconnectStarted 플래그가 정상적이지 않게 true 로 설정된 경우에 발생할 수 있는 이슈를 대응하기 위해 이 조건인 경우에는 _justReconnectStarted 를 무시하도록 설정함.
		if (_lastReqEventActionParam._lastEventActionIndex != INVALID_EVENT_ACTION_INDEX)
		{
			// 특정 이벤트액션은 재연결 후 플레이 시, 서버 패킷 송신 없이 클라에서 알아서 리플레이함.
			if (EventActionIsPlaying() && _eventActionSequencer->ShouldReplayOnReconnection(_lastReqEventActionParam._lastEventActionIndex))
			{
				_eventActionSequencer->RewindEventAction();
				RequestPlayEventActionToEventProgress();
				return;
			}
		}
	}

	if (_pausedEventActionForReconnection.IsValid() && (_lastReqEventActionParam._eventActionGroupId == _pausedEventActionForReconnection->Key) && (_lastReqEventActionParam._lastEventActionIndex == _pausedEventActionForReconnection->Value))
	{	
		// 플레이를 요청했어도, 재연결 중 일시 정지되는 이벤트액션이라면, 재연결 후 자동으로 재진행하므로 아무것도 하지 않음.
		// 본 블럭에서 아무것도 하지 않고 return 하므로, 현재 진행중인 이벤트액션이 반드시 종료 콜백을 호출하는 것을 보장해야만 함.
		//GSLOG(Log, TEXT("_pausedEventActionForReconnection->Key: %d, _pausedEventActionForReconnection->Value: %d"), _pausedEventActionForReconnection->Key, _pausedEventActionForReconnection->Value);
		//_pausedEventActionForReconnection.Reset();

		if (nullptr != _eventActionSequencer && _eventActionSequencer->IsValidLowLevel() && _eventActionSequencer->IsPlaying())
		{
			_eventActionSequencer->RewindEventAction();
			RequestPlayEventActionToEventProgress();
		}
		return;
	}
	*/

/*
#if WITH_DEV_AUTOMATION_TESTS
	// 자동테스트 중 이라면, 자동테스트에서 응답 패킷을 송신하므로 여기서는 아무것도 하지 않음.
	GSLOG(Log, TEXT("InQuestId:%d, InEventActionState:%d, InEventActionGroupId:%d, InEventActionIndex:%d"), _lastReqEventActionParam._questId, _lastReqEventActionParam._eventActionState, _lastReqEventActionParam._eventActionGroupId, _lastReqEventActionParam._lastEventActionIndex);
	if (nullptr == FAutomationTestFramework::Get().GetCurrentTest())
#endif
*/

	if (_lastReqEventActionParam._isReStart)
	{
		// 매우 특수한 조건으로, _isReStart 가 참인 경우, 이전의 모든 상태를 무시하고 강제로 새로 시작함.
		_lastReqEventActionParam.IncreaseLastEventActionIndex();
		RequestPlayEventActionToEventProgress();
	}
	else if (!SkipSendEventActionPacket)
	{
		// next 이벤트액션이 유효하지 않으면, 완료로 즉시 처리.
		if (!UGsEventManager::IsValidPlayEventActionData(_lastReqEventActionParam._eventActionGroupId, _lastReqEventActionParam._lastEventActionIndex + 1))
		{
			// 이 블럭은 일반적인 상황에서 실행되지 않고, 재연결 등 서버로부터 특정한 EventActionIndex 를 수신받았을 때, 정상 진행 가능하도록 하기 위해 실행함.
			// 일반적인 상황에서는 TryNextEventActionPlay() 에서 이벤트액션그룹 종료를 송신함.
			GSLOG(Log, TEXT("!UGsEventManager::IsValidPlayEventActionData(%d, %d), _questId:%llu"), _lastReqEventActionParam._eventActionGroupId, _lastReqEventActionParam._lastEventActionIndex, _lastReqEventActionParam._questId);
			FGsEventActionFinishedParam Param(_lastReqEventActionParam._questId, _lastReqEventActionParam._eventActionGroupId);
			GMessage()->GetEvent().SendMessage(MessageContentEvent::LAST_EVENT_ACTION_FINISHED, Param);
			return;
		}

		// 이 함수로 전달된 eventActionIndex 는 서버의 마지막 값임. 이 다음 값부터 시작해야 하므로 인덱스를 증가시킴.
		_lastReqEventActionParam.IncreaseLastEventActionIndex();
		FGsNetSendServiceWorld::SendEventAction(_lastReqEventActionParam);
		// 이후, 응답패킷에서 UGsEventManager::EventActionPlayResponse() 을 콜백

		// Client Only 이벤트액션은 패킷을 Request 는 하지만, Response 에서 처리하지 않고 즉시 실행함.
		if (IsClientOnlyEventAction(_lastReqEventActionParam._eventActionGroupId, _lastReqEventActionParam._lastEventActionIndex))
		{
			RequestPlayEventActionToEventProgress();
		}
	}
	else
	{
		// 일반적인 상황에서 실행하게 될 블럭
		RequestPlayEventActionToEventProgress();
	}
}

void UGsEventManager::OnLastEventActionFinished(const IGsMessageParam& Param)
{
	FinalizeEventActionGroup();
}

void UGsEventManager::EventActionPlayResponse(const bool InSucceeded, const QuestId InQuestId, const EventActionState InEventActionState, const EventActionGroupId InEventActionGroupId, const EventActionIndex InLastEventActionIndex)
{
	GSLOG(Log, TEXT("InSucceeded:%d, InQuestId:%llu, InEventActionState:%d, InEventActionGroupId:%d, InLastEventActionIndex:%d"), InSucceeded, InQuestId, InEventActionState, InEventActionGroupId, InLastEventActionIndex);

	if (!InSucceeded)
	{
		return;
	}

	_lastReqEventActionParam._eventActionGroupId = InEventActionGroupId;
	_lastReqEventActionParam._lastEventActionIndex = InLastEventActionIndex;

	// Client Only 이벤트액션은 패킷을 Request 는 하지만, Response 에서 처리하지 않고 즉시 실행함.
	if (!IsClientOnlyEventAction(_lastReqEventActionParam._eventActionGroupId, _lastReqEventActionParam._lastEventActionIndex))
	{
		RequestPlayEventActionToEventProgress(); // PlayEventAction(InEventActionGroupId, InLastEventActionIndex);
	}
}

void UGsEventManager::EventActionPlayImmediate(EventSubDialogId InId, bool InIsActive, const TArray<EvenParameterValue>& InParamSet)
{
	GSLOG(Log, TEXT("EventSubDialogId : [%d], Active : [%d], Param Count : [%d]"), InId, InIsActive, InParamSet.Num());

	PlayEventActionImmediate(InId, InIsActive, InParamSet);
}

void UGsEventManager::EventActionAllClearImmediate(const IGsMessageParam& Param)
{
	AllClearEventActionImmediate();
}

EventActionType UGsEventManager::GetCurrentEventActionType() const
{
	if (nullptr == _eventActionSequencer)
	{
		GSLOG(Log, TEXT("nullptr != _eventActionSequencer"));
		return EventActionType::MAX;
	}

	const UGsTableEventActionGroup* Table = Cast<UGsTableEventActionGroup>(UGsTableManager::GetInstance().GetTable(FGsSchemaEventActionGroup::StaticStruct()));
	if (nullptr == Table)
	{
		GSLOG(Error, TEXT("nullptr == Table"));
		return EventActionType::MAX;
	}

	const EventActionGroupId CurrentEventActionGroupId = _lastReqEventActionParam._eventActionGroupId;
	const EventActionIndex CurrentLastEventActionIndex = _lastReqEventActionParam._lastEventActionIndex;
	const FGsSchemaEventActionGroup* EventActionGroup;
	if (!Table->FindRowById(CurrentEventActionGroupId, EventActionGroup))
	{
		GSLOG(Error, TEXT("!Table->FindRowById(%d, EventActionGroup)"), CurrentEventActionGroupId);
		return EventActionType::MAX;
	}

	if (nullptr == EventActionGroup)
	{
		GSLOG(Error, TEXT("nullptr == EventActionGroup, CurrentEventActionGroupId:%d"), CurrentEventActionGroupId);
		return EventActionType::MAX;
	}

	if (!EventActionGroup->eventActionList.IsValidIndex(CurrentLastEventActionIndex))
	{
		GSLOG(Error, TEXT("!EventActionGroup->eventActionList.IsValidIndex(%d), InEventActionGroupId:%d"), CurrentLastEventActionIndex, CurrentEventActionGroupId);
		return EventActionType::MAX;
	}

	return EventActionGroup->eventActionList[CurrentLastEventActionIndex].type;
}

bool UGsEventManager::IsClientOnlyEventAction(const EventActionGroupId InEventActionGroupId, const EventActionIndex InEventActionIndex) const
{
	const UGsTableEventActionGroup* Table = Cast<UGsTableEventActionGroup>(UGsTableManager::GetInstance().GetTable(FGsSchemaEventActionGroup::StaticStruct()));
	if (nullptr == Table)
	{
		GSLOG(Error, TEXT("nullptr == Table, InEventActionGroupId:%d"), InEventActionGroupId);
		return false;
	}

	const FGsSchemaEventActionGroup* EventActionGroup;
	if (!Table->FindRowById(InEventActionGroupId, EventActionGroup))
	{
		GSLOG(Error, TEXT("!Table->FindRowById(%d, EventActionGroup)"), InEventActionGroupId);
		return false;
	}

	if (nullptr == EventActionGroup)
	{
		GSLOG(Error, TEXT("nullptr == EventActionGroup, InEventActionGroupId:%d"), InEventActionGroupId);
		return false;
	}

	if (0 == EventActionGroup->eventActionList.Num())
	{
		GSLOG(Error, TEXT("0 == EventActionGroup->eventActionList.Num(), InEventActionGroupId:%d"), InEventActionGroupId);
		return false;
	}

	if (!EventActionGroup->eventActionList.IsValidIndex(InEventActionIndex))
	{
		GSLOG(Error, TEXT("!EventActionGroup->eventActionList.IsValidIndex(%d):%d, InEventActionGroupId:%d"), InEventActionIndex, EventActionGroup->eventActionList.Num(), InEventActionGroupId);
		return false;
	}

	return EventActionGroup->eventActionList[InEventActionIndex].clientOnly;
}

void UGsEventManager::OnEventActionFinished()
{
	if (_reconnecting)
	{
		// 재연결 중에는 아무것도 진행시키지 않음
		GSLOG(Log, TEXT("Stop eventaction on reconnecting."));
		return;
	}

	FGsEventProgressManager* EventProgressManager = GSEventProgress();
	if (nullptr == EventProgressManager)
	{
		GSLOG(Error, TEXT("nullptr == EventProgressManager"));
	}
	else
	{
		const EventActionType CurrentEventActionType = GetCurrentEventActionType();

		// 이 후, 반드시 UGsEventManager::StartEvent() 가 호출되어야만 한다. 호출되지 않을 경우, 이벤트가 더이상 실행되지 않음.
		GSLOG(Log, TEXT("EventProgressManager->OnFinishEventByEventAction(%d);"), CurrentEventActionType);
		EventProgressManager->OnFinishEventByEventAction(CurrentEventActionType);
	}

	TryNextEventActionPlay();
}

void UGsEventManager::TryNextEventActionPlay()
{
	if (nullptr == _eventActionSequencer)
	{
		GSLOG(Error, TEXT("nullptr == _eventActionSequencer"));
		return;
	}

	if (_eventActionSequencer->HasNextEventAction())
	{
		GMessage()->GetEvent().SendMessage(MessageContentEvent::EVENT_ACTION_PLAY, _lastReqEventActionParam);
	}
	else
	{
		GSLOG(Log, TEXT("LastEventActionFinished _lastReqEventActionParam._questId:%llu, _lastReqEventActionParam._eventActionGroupId:%d"), _lastReqEventActionParam._questId, _lastReqEventActionParam._eventActionGroupId);
		FGsEventActionFinishedParam Param(_lastReqEventActionParam._questId, _lastReqEventActionParam._eventActionGroupId);
		GMessage()->GetEvent().SendMessage(MessageContentEvent::LAST_EVENT_ACTION_FINISHED, Param);
	}
}

void UGsEventManager::PlayEventAction(const EventActionGroupId InEventActionGroupId, const EventActionIndex InEventActionIndex)
{
	if (nullptr == _eventActionSequencer)
	{
		GSLOG(Error, TEXT("nullptr == _eventActionSequencer, InEventActionGroupId:%d, InEventActionIndex:%d"), InEventActionGroupId, InEventActionIndex);
		return;
	}

	_eventActionSequencer->SetEventActionGroupId(InEventActionGroupId);
	_eventActionSequencer->Play(InEventActionIndex);
}

void UGsEventManager::ForceClearEventActionSequencer()
{
	GSLOG(Log, TEXT("UGsEventManager::ForceClearEventActionSequencer()"));

	if (nullptr == _eventActionSequencer || !_eventActionSequencer->IsValidLowLevel() || !_eventActionSequencer->IsPlaying())
	{
		return;
	}

	//_eventActionSequencer->RewindEventAction();
}

void UGsEventManager::PlayEventActionImmediate(EventSubDialogId InId, bool InIsActive, const TArray<EvenParameterValue>& InParamSet)
{
	if (nullptr == _eventActionImmediate)
	{
		GSLOG(Error, TEXT("nullptr == _eventActionImmediate"));
		return;
	}

	_eventActionImmediate->Play(InId, InIsActive, InParamSet);
}

void UGsEventManager::AllClearEventActionImmediate()
{
	if (nullptr == _eventActionImmediate)
	{
		GSLOG(Error, TEXT("nullptr == _eventActionImmediate"));
		return;
	}

	_eventActionImmediate->ClearAll();
}

void UGsEventManager::OnApplicationWillEnterBackground()
{
	GSLOG(Log, TEXT("UGsEventManager::OnApplicationWillEnterBackground()"));

	if (nullptr != _eventActionSequencer && _eventActionSequencer->IsValidLowLevel())
	{
		_eventActionSequencer->OnApplicationWillEnterBackground();
	}
}

void UGsEventManager::OnApplicationHasEnteredForeground()
{
	GSLOG(Log, TEXT("UGsEventManager::OnApplicationHasEnteredForeground()"));

	if (nullptr != _eventActionSequencer && _eventActionSequencer->IsValidLowLevel())
	{
		_eventActionSequencer->OnApplicationHasEnteredForeground();
	}
}

void UGsEventManager::RequestPlayEventActionToEventProgress()
{
	FGsEventProgressManager* EventProgressManager = GSEventProgress();
	if (nullptr == EventProgressManager)
	{
		GSLOG(Error, TEXT("nullptr == EventProgressManager"));
		return;
	}

	const EventActionType CurrentEventActionType = GetCurrentEventActionType();

	// 이 후, 반드시 UGsEventManager::StartEvent() 가 호출되어야만 한다. 호출되지 않을 경우, 이벤트가 더이상 실행되지 않음.
	GSLOG(Log, TEXT("EventProgressManager->OnReqStartEventByEventAction(%d, this);"), CurrentEventActionType);
	EventProgressManager->OnReqStartEventByEventAction(CurrentEventActionType, this);
}

void UGsEventManager::StartEvent(EGsEventProgressType /*In_startType*/, EGsEventProgressType /*In_preType*/)
{
	if (_reconnecting)
	{
		// 재연결 중에는 아무것도 진행시키지 않음
		GSLOG(Warning, TEXT("재연결 중이므로 이벤트액션 시작 안함."));
		return;
	}

	GSLOG(Log, TEXT("UGsEventManager::StartEvent()"));
	PlayEventAction(_lastReqEventActionParam._eventActionGroupId, _lastReqEventActionParam._lastEventActionIndex);
}

bool UGsEventManager::IsValidPlayEventActionData(const EventActionGroupId InEventActionGroupId, const EventActionIndex InEventActionIndex)
{
	const UGsTableEventActionGroup* Table = Cast<UGsTableEventActionGroup>(UGsTableManager::GetInstance().GetTable(FGsSchemaEventActionGroup::StaticStruct()));
	if (nullptr == Table)
	{
		GSLOG(Error, TEXT("nullptr == Table, InEventActionGroupId:%d"), InEventActionGroupId);
		return false;
	}

	const FGsSchemaEventActionGroup* EventActionGroup;
	if (!Table->FindRowById(InEventActionGroupId, EventActionGroup))
	{
		return false;
	}

	if (nullptr == EventActionGroup)
	{
		GSLOG(Error, TEXT("nullptr == EventActionGroup, InEventActionGroupId:%d"), InEventActionGroupId);
		return false;
	}

	if (0 > InEventActionIndex || InEventActionIndex >= EventActionGroup->eventActionList.Num())
	{
		GSLOG(Log, TEXT("0 > InEventActionIndex || InEventActionIndex >= EventActionGroup->eventActionList.Num(), InEventActionIndex: %d, EventActionGroup->eventActionList.Num(): %d"), InEventActionIndex, EventActionGroup->eventActionList.Num());
		return false;
	}

	return true;
}
