#include "GsEventActionSequencer.h"

#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"

#include "Event/EventAction/GsSchemaEventAction.h"
#include "Event/EventAction/GsSchemaEventActionGroup.h"
#include "EventAction/GsEventActionBase.h"
#include "EventAction/GsEventActionCinematic.h"
#include "EventAction/GsEventActionDelay.h"
#include "EventAction/GsEventActionDialog.h"
#include "EventAction/GsEventActionDimmedOff.h"
#include "EventAction/GsEventActionDimmedOn.h"
#include "EventAction/GsEventActionImmediatelyFinish.h"
#include "EventAction/GsEventActionList.h"
#include "EventAction/GsEventActionPlayMovie.h"
#include "EventAction/GsEventActionPlaySound.h"
#include "EventAction/GsEventActionTutorial.h"
#include "GsSchemaBase.h"
#include "GsSchemaRowBase.h"
#include "GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"

void UGsEventActionSequencer::Initialize()
{
}

void UGsEventActionSequencer::Finalize()
{
	_eventActionList = nullptr;
	_currentEventActionIndex = INVALID_EVENT_ACTION_INDEX;
	_onEventActionFinished.Unbind();
}

bool UGsEventActionSequencer::IsPlaying() const
{
	const UGsEventActionBase* EventAction = GetPlayingEventAction();
	if (nullptr == EventAction)
	{
		return false;
	}

	return EventAction->IsPlaying();
}

bool UGsEventActionSequencer::ShouldReplayOnReconnection(const EventActionIndex InEventActionIndex) const
{
	const UGsEventActionBase* EventAction = GetEventAction(InEventActionIndex);
	if (nullptr == EventAction)
	{
		return false;
	}

	return EventAction->ShouldReplayOnReconnection();
}

void UGsEventActionSequencer::Play(const EventActionIndex InEventActionIndex)
{
	if (!IsValidEventActionList())
	{
		GSLOG(Error, TEXT("!IsValidEventActionList()"));
		return;
	}

	_currentEventActionIndex = InEventActionIndex;

	UGsEventActionBase* EventAction = GetEventAction(_currentEventActionIndex);
	if (nullptr == EventAction)
	{
		GSLOG(Error, TEXT("nullptr == EventAction, _eventActionGroupId:%d, _currentEventActionIndex:%d"), _eventActionGroupId, _currentEventActionIndex);
		return;
	}

	EventAction->_onFinished.BindUObject(this, &UGsEventActionSequencer::OnEventActionFinished);
	const bool IsPlaySucceeded = EventAction->Play() && EventAction->OnAfterPlay();
	if (!IsPlaySucceeded || EventAction->IsA<UGsEventActionImmediatelyFinish>())
	{
		// 플레이가 성공하지 못한 경우라도 Finish() 를 정상 호출하여 종료 콜백이 정상 수행되도록 함.
		// 즉시 종료할 이벤트 액션인 경우 처리
		EventAction->Finish();
	}
}

bool UGsEventActionSequencer::RewindEventAction()
{
	UGsEventActionBase* EventAction = GetPlayingEventAction();
	GSLOG(Log, TEXT("UGsEventActionSequencer::RewindEventAction() and try to Rewind() at: %p"), EventAction);
	return (nullptr != EventAction) && EventAction->Rewind();
}

void UGsEventActionSequencer::Pause() const
{
	if (!IsValidEventActionList())
	{
		return;
	}

	if (!IsPlaying())
	{
		return;
	}
}

void UGsEventActionSequencer::SetEventActionGroupId(const EventActionGroupId InEventActionGroupId)
{
	if (_eventActionGroupId == InEventActionGroupId)
	{
		// 이미 같은 이벤트그룹이라면 재설정 필요 없음.
		return;
	}

	if (IsPlaying())
	{
		GSLOG(Warning, TEXT("IsPlaying(), GetEventActionGroupId():%d, InEventActionGroupId:%d"), GetEventActionGroupId(), InEventActionGroupId);
	}

	_eventActionGroupId = InEventActionGroupId;
	if (INVALID_EVENT_ACTION_GROUP_ID == _eventActionGroupId)
	{
		_eventActionList = nullptr;
	}
	else
	{
		_eventActionList = UGsEventActionSequencer::MakeEventActionList(this, _eventActionGroupId);
	}
}

EventActionGroupId UGsEventActionSequencer::GetEventActionGroupId() const
{
	return _eventActionGroupId;
}

EventActionIndex UGsEventActionSequencer::GetEventActionIndex() const
{
	return _currentEventActionIndex;
}

bool UGsEventActionSequencer::HasNextEventAction() const
{
	if (!IsValidEventActionList())
	{
		return false;
	}

	return nullptr != GetEventAction(_currentEventActionIndex + 1);
}

void UGsEventActionSequencer::OnApplicationWillEnterBackground()
{
	if (!IsPlaying())
	{
		return;
	}

	UGsEventActionBase* EventAction = GetPlayingEventAction();
	if (nullptr == EventAction || !EventAction->IsValidLowLevel())
	{
		return;
	}

	EventAction->OnApplicationWillEnterBackground();
}

void UGsEventActionSequencer::OnApplicationHasEnteredForeground()
{
	if (!IsPlaying())
	{
		return;
	}

	UGsEventActionBase* EventAction = GetPlayingEventAction();
	if (nullptr == EventAction || !EventAction->IsValidLowLevel())
	{
		return;
	}

	EventAction->OnApplicationHasEnteredForeground();
}

const UGsEventActionList* UGsEventActionSequencer::MakeEventActionList(UObject* Outer, const EventActionGroupId InEventActionGroupId)
{
	const UGsTableEventActionGroup* Table = Cast<UGsTableEventActionGroup>(UGsTableManager::GetInstance().GetTable(FGsSchemaEventActionGroup::StaticStruct()));
	if (nullptr == Table)
	{
		GSLOG(Error, TEXT("nullptr == Table, InEventActionGroupId:%d"), InEventActionGroupId);
		return nullptr;
	}

	const FGsSchemaEventActionGroup* EventActionGroup;
	if (!Table->FindRowById(InEventActionGroupId, EventActionGroup))
	{
		GSLOG(Error, TEXT("!Table->FindRowById(%d, EventActionGroup)"), InEventActionGroupId);
		return nullptr;
	}

	if (nullptr == EventActionGroup)
	{
		GSLOG(Error, TEXT("nullptr == EventActionGroup, InEventActionGroupId:%d"), InEventActionGroupId);
		return nullptr;
	}

	if (0 == EventActionGroup->eventActionList.Num())
	{
		GSLOG(Error, TEXT("0 == EventActionGroup->eventActionList.Num(), InEventActionGroupId:%d"), InEventActionGroupId);
		return nullptr;
	}

	UGsEventActionList* EventActionList = NewObject<UGsEventActionList>(Outer);
	for (const FGsSchemaEventAction& EventActionData : EventActionGroup->eventActionList)
	{
		UGsEventActionBase* EventAction = UGsEventActionSequencer::CreateEventAction(EventActionList, EventActionData);
		EventActionList->AddEventAction(EventAction);
	}
	return EventActionList;
}

UGsEventActionBase* UGsEventActionSequencer::CreateEventAction(UObject* Outer, const FGsSchemaEventAction& InEventActionData)
{
	const EventActionType CurrentEventActionType = InEventActionData.type;
	UGsEventActionBase* EventAction = nullptr;
	switch (CurrentEventActionType)
	{
	case EventActionType::PLAY_CINEMATIC: EventAction = NewObject<UGsEventActionCinematic>(Outer); break;
	case EventActionType::DIMMED_ON: EventAction = NewObject<UGsEventActionDimmedOn>(Outer); break;
	case EventActionType::DIMMED_OFF: EventAction = NewObject<UGsEventActionDimmedOff>(Outer); break;
	case EventActionType::TUTORIAL: EventAction = NewObject<UGsEventActionTutorial>(Outer); break;
	case EventActionType::DIALOG: EventAction = NewObject<UGsEventActionDialog>(Outer); break;
	case EventActionType::PLAY_MOVIE: EventAction = NewObject<UGsEventActionPlayMovie>(Outer); break;
	//case EventActionType::PLAY_SOUND: EventAction = NewObject<UGsEventActionPlaySound>(Outer); break;

		// 클라에서 아무것도 하지 않는 이벤트액션 모음.
	case EventActionType::TELEPORT:
	case EventActionType::SPAWN:
	case EventActionType::DESPAWN:
	case EventActionType::MONSTER_KILL:
		// 이벤트 액션에서는 아무것도 하지 않는다.
	case EventActionType::TRANSFORM_LOOK_INFO:
	case EventActionType::TRANSFORM_STAT:
	case EventActionType::TRANSFORM_CLEAR:
		EventAction = NewObject<UGsEventActionImmediatelyFinish>(Outer);
		break;

	case EventActionType::MAX:
	default:
		GSLOG(Error, TEXT("Invalid EventActionType:%d"), CurrentEventActionType);
		return nullptr;
	}
	
	if (nullptr == EventAction)
	{
		GSLOG(Warning, TEXT("nullptr == EventAction"));
		return nullptr;
	}

	// InEventActionData.typeInfo can be null.
	const FGsSchemaBase* EventActionData = (InEventActionData.typeInfo.IsNull()) ? nullptr : InEventActionData.typeInfo.GetRowByType<const FGsSchemaBase>();
	EventAction->Initialize(EventActionData);
	return EventAction;
}

bool UGsEventActionSequencer::IsValidEventActionList() const
{
	return (_eventActionGroupId != INVALID_EVENT_ACTION_GROUP_ID) && (nullptr != _eventActionList) && _eventActionList->IsValidLowLevel();
}

UGsEventActionBase* UGsEventActionSequencer::GetEventAction(const EventActionIndex InEventActionIndex) const
{
	if (!IsValidEventActionList())
	{
		GSLOG(Log, TEXT("!IsValidEventActionList()"));
		return nullptr;
	}

	UGsEventActionBase* EventAction = nullptr;
	if (!_eventActionList->GetValidEventActionAt(InEventActionIndex, EventAction))
	{
		GSLOG(Log, TEXT("EventActionList->GetValidEventActionAt(%d, EventAction)"), InEventActionIndex);
		return nullptr;
	}

	return EventAction;
}

UGsEventActionBase* UGsEventActionSequencer::GetPlayingEventAction() const
{
	if (!IsValidEventActionList() || (INVALID_EVENT_ACTION_INDEX == _currentEventActionIndex))
	{
		return nullptr;
	}

	return GetEventAction(_currentEventActionIndex);
}

void UGsEventActionSequencer::OnEventActionFinished()
{
	_onEventActionFinished.ExecuteIfBound();
}
