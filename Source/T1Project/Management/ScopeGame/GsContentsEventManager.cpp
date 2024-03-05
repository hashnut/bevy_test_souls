#include "GsContentsEventManager.h"

#include "ContentsEvent/EGsContentsEventState.h"
#include "ContentsEvent/GsContentsEventBase.h"
#include "ContentsEvent/GsContentsEventDungeon.h"
#include "ContentsEvent/GsContentsEventGroup.h"
#include "ContentsEvent/GsContentsEventMapPassivity.h"
#include "ContentsEvent/GsContentsEventNpcPrayer.h"
#include "ContentsEvent/GsContentsEventNpcShop.h"
#include "ContentsEvent/GsContentsEventReward.h"
#include "ContentsEvent/GsContentsEventTeleportCost.h"
#include "DataSchema/ContentsEvent/GsSchemaContentsEventGroup.h"
#include "GsTable.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsContentsEventMessageParam.h"
#include "Net/MessageHandler/GsNetMessageHandlerContentsEvent.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UTIL/GsTimeSyncUtil.h"

void UGsContentsEventManager::Initialize()
{
	if (0 != _listEventDelegate.Num())
	{
		GSLOG(Warning, TEXT("0 != _listEventDelegate.Num(): Finalize() 호출 없이 Initialize() 가 재호출 되었습니다."));
	}

	const UGsTable* Table = FGsSchemaContentsEventGroup::GetStaticTable();
	if (nullptr == Table)
	{
		GSLOG(Error, TEXT("nullptr == Table"));
	}
	else
	{
		TArray<const FGsSchemaContentsEventGroup*> AllRows;
		if (!Table->GetAllRows(AllRows))
		{
			GSLOG(Error, TEXT("!Table->GetAllRows(&AllRows)"));
		}
		else
		{
			for (const FGsSchemaContentsEventGroup* It : AllRows)
			{
				if (nullptr == It)
				{
					GSLOG(Error, TEXT("nullptr == It"));
					continue;
				}

				const ContentsEventGroupId CurrentContentsEventGroupId = It->id;
				TArray<TWeakPtr<const FGsContentsEventBase>> WeakContentsEvents;
				for (const FGsSchemaContentsEventListItem& ListItemIt : It->eventList)
				{
					const ContentsEventType CurrentContentsEventType = ListItemIt.eventType;
					if (ContentsEventType::UNUSED_COMMON == CurrentContentsEventType)
					{
						// 서버 전용 타입이므로 제외함.
						continue;
					}

					const ContentsEventId CurrentContentsEventId = ListItemIt.eventId;
					TMap<ContentsEventId, TSharedRef<FGsContentsEventBase>>& ContentsEventsMap = _contentsEventsTypeMap.FindOrAdd(CurrentContentsEventType);
					switch (CurrentContentsEventType)
					{
					case ContentsEventType::REWARD:
					{
						TSharedRef<FGsContentsEventReward> CurrentContentsEvent = MakeShared<FGsContentsEventReward>(CurrentContentsEventGroupId, CurrentContentsEventId);
						ContentsEventsMap.Emplace(CurrentContentsEventId, StaticCastSharedRef<FGsContentsEventBase>(CurrentContentsEvent));
						WeakContentsEvents.Add(CurrentContentsEvent);
					}
					break;

					case ContentsEventType::MAP_PASSIVITY:
					{
						TSharedRef<FGsContentsEventMapPassivity> CurrentContentsEvent = MakeShared<FGsContentsEventMapPassivity>(CurrentContentsEventGroupId, CurrentContentsEventId);
						ContentsEventsMap.Emplace(CurrentContentsEventId, StaticCastSharedRef<FGsContentsEventBase>(CurrentContentsEvent));
						WeakContentsEvents.Add(CurrentContentsEvent);
					}
					break;

					case ContentsEventType::TELEPORT_COST:
					{
						TSharedRef<FGsContentsEventTeleportCost> CurrentContentsEvent = MakeShared<FGsContentsEventTeleportCost>(CurrentContentsEventGroupId, CurrentContentsEventId);
						ContentsEventsMap.Emplace(CurrentContentsEventId, StaticCastSharedRef<FGsContentsEventBase>(CurrentContentsEvent));
						WeakContentsEvents.Add(CurrentContentsEvent);
					}
					break;

					case ContentsEventType::DUNGEON:
					{
						TSharedRef<FGsContentsEventDungeon> CurrentContentsEvent = MakeShared<FGsContentsEventDungeon>(CurrentContentsEventGroupId, CurrentContentsEventId);
						ContentsEventsMap.Emplace(CurrentContentsEventId, StaticCastSharedRef<FGsContentsEventBase>(CurrentContentsEvent));
						WeakContentsEvents.Add(CurrentContentsEvent);
					}
					break;

					case ContentsEventType::NPC_SHOP:
					{
						TSharedRef<FGsContentsEventNpcShop> CurrentContentsEvent = MakeShared<FGsContentsEventNpcShop>(CurrentContentsEventGroupId, CurrentContentsEventId);
						ContentsEventsMap.Emplace(CurrentContentsEventId, StaticCastSharedRef<FGsContentsEventBase>(CurrentContentsEvent));
						WeakContentsEvents.Add(CurrentContentsEvent);
					}
					break;

					case ContentsEventType::NPC_PRAYER:
					{
						TSharedRef<FGsContentsEventNpcPrayer> CurrentContentsEvent = MakeShared<FGsContentsEventNpcPrayer>(CurrentContentsEventGroupId, CurrentContentsEventId);
						ContentsEventsMap.Emplace(CurrentContentsEventId, StaticCastSharedRef<FGsContentsEventBase>(CurrentContentsEvent));
						WeakContentsEvents.Add(CurrentContentsEvent);
					}
					break;

					case ContentsEventType::UNUSED_COMMON:
					case ContentsEventType::MAX:
					default:
						break;
					}
				}

				TSharedRef<FGsContentsEventGroup> ContentsEventGroup = MakeShared<FGsContentsEventGroup>(It, WeakContentsEvents);
				_contentsEventGroups.Emplace(CurrentContentsEventGroupId, ContentsEventGroup);
			}
		}
	}

	MContentsEvent& ContentsEvent = GMessage()->GetContentsEvent();
	for (const TPair<MessageContentContentsEvent, FDelegateHandle>& It : _listEventDelegate)
	{
		ContentsEvent.Remove(It);
	}
	_listEventDelegate.Empty();

	// _listEventDelegate.Emplace(ContentsEvent.AddUObject(MessageContentContentsEvent::EVENT_ACTION_PLAY, this, &UGsContentsEventManager::EventActionPlayRequest));

	//_listEventDelegate.Emplace(ContentsEvent.AddUObject(MessageContentContentsEvent::ALL_CLEAR_EVENT_ACTION_IMMEDIATE, this, &UGsContentsEventManager::EventActionAllClearImmediate));
}

void UGsContentsEventManager::Finalize()
{
	MContentsEvent& ContentsEvent = GMessage()->GetContentsEvent();
	for (const TPair<MessageContentContentsEvent, FDelegateHandle>& It : _listEventDelegate)
	{
		ContentsEvent.Remove(It);
	}
	_listEventDelegate.Empty();
}

void UGsContentsEventManager::ResetActivatedContentsEventGroupDatas(const TMap<ContentsEventGroupId, FGsNetMessageHandlerContentsEvent::FGsContentsEventGroupData>& InContentsEventGroupDatas)
{
	// 기본적으로 InContentsEventGroups 의 모든 데이터로 덮어씀.
	// 현재 활성화 중인매개변수에서 삭제된 이벤트그룹은 Deactivated 상태로 전환.
	// 매개변수에 존재하지만, Deactivated 였던 이벤트그룹은 Activated 상태로 전환.
	for (const TPair<ContentsEventGroupId, TSharedRef<FGsContentsEventGroup>>& ContentsEventGroupPair : _contentsEventGroups)
	{
		const ContentsEventGroupId CurrentContentsEventGroupId = ContentsEventGroupPair.Key;
		FGsContentsEventGroup& ContentsEventGroup = *ContentsEventGroupPair.Value;
		const FGsNetMessageHandlerContentsEvent::FGsContentsEventGroupData* ContentsEventGroupData = InContentsEventGroupDatas.Find(CurrentContentsEventGroupId);
		if (nullptr == ContentsEventGroupData)
		{
			DeactivateContentsEventGroup(CurrentContentsEventGroupId);
			continue;
		}

		ContentsEventGroup.OverrideData(*ContentsEventGroupData);

		if (ContentsEventGroup.GetState() == EGsContentsEventState::Deactivated)
		{
			ActivateContentsEventGroup(CurrentContentsEventGroupId);
		}
	}
}

void UGsContentsEventManager::ActivateContentsEventGroup(const FGsNetMessageHandlerContentsEvent::FGsContentsEventGroupData& InContentsEventGroupData)
{
	const ContentsEventGroupId CurrentContentsEventGroupId = InContentsEventGroupData._contentsEventGroupId;
	TSharedRef<FGsContentsEventGroup>* ContentsEventGroup = _contentsEventGroups.Find(CurrentContentsEventGroupId);
	if (nullptr == ContentsEventGroup)
	{
		GSLOG(Error, TEXT("nullptr == ContentsEventGroup, CurrentContentsEventGroupId:%d"), CurrentContentsEventGroupId);
		return;
	}

	(*ContentsEventGroup)->OverrideData(InContentsEventGroupData);

	ActivateContentsEventGroup(CurrentContentsEventGroupId);
}

void UGsContentsEventManager::ActivateContentsEventGroup(const ContentsEventGroupId InContentsEventGroupId)
{
	TSharedRef<FGsContentsEventGroup>* ContentsEventGroup = _contentsEventGroups.Find(InContentsEventGroupId);
	if (nullptr != ContentsEventGroup)
	{
		(*ContentsEventGroup)->SetState(EGsContentsEventState::Activated);
	}
}

void UGsContentsEventManager::DeactivateContentsEventGroup(const ContentsEventGroupId InContentsEventGroupId)
{
	TSharedRef<FGsContentsEventGroup>* ContentsEventGroup = _contentsEventGroups.Find(InContentsEventGroupId);
	if (nullptr != ContentsEventGroup)
	{
		(*ContentsEventGroup)->SetState(EGsContentsEventState::Deactivated);
	}
}

bool CompareByContentsEventGroupAscending(const TWeakPtr<const FGsContentsEventGroup>& InA, const TWeakPtr<const FGsContentsEventGroup>& InB)
{
	if (!InA.IsValid() || !InB.IsValid())
	{
		GSLOG(Error, TEXT("!InA.IsValid() || !InB.IsValid()"));
		return false;
	}

	return InA.Pin()->GetDisplayPriority() < InB.Pin()->GetDisplayPriority();
}

TArray<TWeakPtr<const FGsContentsEventGroup>> UGsContentsEventManager::GetContentsEventGroups(const EGsContentsEventState InContentsEventState) const
{
	TArray<TWeakPtr<const FGsContentsEventGroup>> ContentsEventGroups;
	for (const TPair<ContentsEventGroupId, TSharedRef<FGsContentsEventGroup>>& ContentsEventGroupPair : _contentsEventGroups)
	{
		TSharedRef<FGsContentsEventGroup> ContentsEventGroup = ContentsEventGroupPair.Value;
		if (ContentsEventGroup->GetState() == InContentsEventState)
		{
			ContentsEventGroups.Add(ContentsEventGroup);
		}
	}

	ContentsEventGroups.StableSort(&CompareByContentsEventGroupAscending);

	return ContentsEventGroups;
}

TArray<TWeakPtr<const FGsContentsEventGroup>> UGsContentsEventManager::GetVisibleContentsEventGroups() const
{
	TArray<TWeakPtr<const FGsContentsEventGroup>> ContentsEventGroups;
	for (const TPair<ContentsEventGroupId, TSharedRef<FGsContentsEventGroup>>& ContentsEventGroupPair : _contentsEventGroups)
	{
		TSharedRef<FGsContentsEventGroup> ContentsEventGroup = ContentsEventGroupPair.Value;
		if (EGsContentsEventState::Activated == ContentsEventGroup->GetState() && ContentsEventGroup->CanDisplay())
		{
			ContentsEventGroups.Add(ContentsEventGroup);
		}
	}

	ContentsEventGroups.StableSort(&CompareByContentsEventGroupAscending);

	return ContentsEventGroups;
}

TWeakPtr<const FGsContentsEventGroup> UGsContentsEventManager::GetContentsEventGroup(const ContentsEventGroupId InContentsEventGroupId) const
{
	const TSharedRef<FGsContentsEventGroup>* ContentsEventGroup = _contentsEventGroups.Find(InContentsEventGroupId);
	if (nullptr != ContentsEventGroup)
	{
		return *ContentsEventGroup;
	}
	else
	{
		return nullptr;
	}
}

void UGsContentsEventManager::ResetActivatedContentsEventDatas(const TMap<ContentsEventType, TMap<ContentsEventId, FGsNetMessageHandlerContentsEvent::FGsContentsEventData>>& InContentsEventDatasMap)
{
	TSet<ContentsEventType> AffectedContentsEventTypes;

	for (TPair<ContentsEventType, TMap<ContentsEventId, TSharedRef<FGsContentsEventBase>>>& ContentsEventMapPair : _contentsEventsTypeMap)
	{
		const ContentsEventType CurrentContentsEventType = ContentsEventMapPair.Key;

		const TMap<ContentsEventId, FGsNetMessageHandlerContentsEvent::FGsContentsEventData>* ContentsEventDataMap = InContentsEventDatasMap.Find(CurrentContentsEventType);
		const bool HasAnyContentsEventType = (nullptr != ContentsEventDataMap);

		TMap<ContentsEventId, TSharedRef<FGsContentsEventBase>>& ContentsEventMap = ContentsEventMapPair.Value;
		for (TPair<ContentsEventId, TSharedRef<FGsContentsEventBase>>& ContentsEventPair : ContentsEventMap)
		{
			const bool ContentsEventActivated = HasAnyContentsEventType && ContentsEventDataMap->Contains(ContentsEventPair.Key);
			TSharedRef<FGsContentsEventBase>& ContentsEvent = ContentsEventPair.Value;
			if (ContentsEvent->ChangeActivate(ContentsEventActivated))
			{
				// 활성화 상태 변경 시, 실제 상태가 변경된 타입을 수집
				AffectedContentsEventTypes.Add(CurrentContentsEventType);
			}
		}
	}

	FGsContentsEventMessageParamContentsEventStateChangedCompleted(MoveTemp(AffectedContentsEventTypes)).Send();
}

void UGsContentsEventManager::ActivateContentsEvent(const ContentsEventType InContentsEventType, const ContentsEventId InContentsEventId)
{
	TSharedPtr<FGsContentsEventBase> ContentsEvent = FindContentsEvent(InContentsEventId, InContentsEventType);
	if (!ContentsEvent.IsValid())
	{
		GSLOG(Error, TEXT("!ContentsEvent.IsValid(), InContentsEventType:%d, InContentsEventId:%d"), InContentsEventType, InContentsEventId);
	}
	else
	{
		if (ContentsEvent->ChangeActivate(true))
		{
			FGsContentsEventMessageParamContentsEventStateChangedCompleted({ InContentsEventType }).Send();
		}
	}
}

void UGsContentsEventManager::DeactivateContentsEvent(const ContentsEventType InContentsEventType, const ContentsEventId InContentsEventId)
{
	TSharedPtr<FGsContentsEventBase> ContentsEvent = FindContentsEvent(InContentsEventId, InContentsEventType);
	if (!ContentsEvent.IsValid())
	{
		GSLOG(Error, TEXT("!ContentsEvent.IsValid(), InContentsEventType:%d, InContentsEventId:%d"), InContentsEventType, InContentsEventId);
	}
	else
	{
		if (ContentsEvent->ChangeActivate(false))
		{
			FGsContentsEventMessageParamContentsEventStateChangedCompleted({ InContentsEventType }).Send();
		}
	}
}

TSharedPtr<FGsContentsEventBase> UGsContentsEventManager::FindContentsEvent(const ContentsEventId InContentsEventId, const ContentsEventType InContentsEventType /*= ContentsEventType::MAX*/)
{
	if (ContentsEventType::MAX == InContentsEventType)
	{
		// 전체 검색
		for (TPair<ContentsEventType, TMap<ContentsEventId, TSharedRef<FGsContentsEventBase>>>& ContentsEventsTypeMapPair : _contentsEventsTypeMap)
		{
			TMap<ContentsEventId, TSharedRef<FGsContentsEventBase>>& ContentsEvents = ContentsEventsTypeMapPair.Value;
			TSharedRef<FGsContentsEventBase>* ContentsEvent = ContentsEvents.Find(InContentsEventId);
			if (nullptr == ContentsEvent)
			{
				continue;
			}

			return *ContentsEvent;
		}

		GSLOG(Error, TEXT("Not found, InContentsEventType:%d, InContentsEventId:%d"), InContentsEventType, InContentsEventId);
		return nullptr;
	}
	else
	{
		// 특정 컨텐츠이벤트타입에서만 검색
		TMap<ContentsEventId, TSharedRef<FGsContentsEventBase>>* ContentsEventsMap = _contentsEventsTypeMap.Find(InContentsEventType);
		if (nullptr == ContentsEventsMap)
		{
			GSLOG(Error, TEXT("nullptr == ContentsEventsMap, InContentsEventType:%d"), InContentsEventType);
			return nullptr;
		}

		TSharedRef<FGsContentsEventBase>* ContentsEvent = ContentsEventsMap->Find(InContentsEventId);
		if (nullptr == ContentsEvent)
		{
			GSLOG(Warning, TEXT("nullptr == ContentsEvent, InContentsEventId:%d"), InContentsEventId);
			return nullptr;
		}

		return *ContentsEvent;
	}
}

TArray<TWeakPtr<const FGsContentsEventBase>> UGsContentsEventManager::FindContentsEvents(const ContentsEventGroupId InContentsEventGroupId, const EGsContentsEventState InContentsEventState /*= EGsContentsEventState::Any*/) const
{
	TArray<TWeakPtr<const FGsContentsEventBase>> ContentsEvents;
	const TSharedRef<FGsContentsEventGroup>* ContentsEventGroup = _contentsEventGroups.Find(InContentsEventGroupId);
	if (nullptr == ContentsEventGroup)
	{
		GSLOG(Error, TEXT("nullptr == ContentsEventGroup, InContentsEventGroupId:%d"), InContentsEventGroupId);
	}
	else
	{
		const TArray<TWeakPtr<const FGsContentsEventBase>>& ContentsEventsInGroup = (*ContentsEventGroup)->GetContentsEventAll();
		for (const TWeakPtr<const FGsContentsEventBase>& ContentsEvent : ContentsEventsInGroup)
		{
			switch (InContentsEventState)
			{
			case EGsContentsEventState::Deactivated:
				if (!ContentsEvent.Pin()->IsActivatedSelf())
				{
					ContentsEvents.Emplace(ContentsEvent);
				}
				break;

			case EGsContentsEventState::Activated:
				if (ContentsEvent.Pin()->IsActivatedSelf())
				{
					ContentsEvents.Emplace(ContentsEvent);
				}
				break;

			case EGsContentsEventState::Any:
			default:
				ContentsEvents.Emplace(ContentsEvent);
				break;
			}
		}
	}

	return ContentsEvents;
}

bool UGsContentsEventManager::IsAnyContentsEventActivated(const ContentsEventType InContentsEventType, const TSet<int32>& InContentsEventIdSet) const
{
	const TMap<ContentsEventId, TSharedRef<FGsContentsEventBase>>* ContentsEventMap = _contentsEventsTypeMap.Find(InContentsEventType);
	if (nullptr == ContentsEventMap)
	{
		return false;
	}

	for (const ContentsEventId CurrentContentsEventId : InContentsEventIdSet)
	{
		const TSharedRef<FGsContentsEventBase>* ContentsEvent = ContentsEventMap->Find(CurrentContentsEventId);
		if (nullptr != ContentsEvent && (*ContentsEvent)->IsActivated())
		{
			return true;
		}
	}

	return false;
}
