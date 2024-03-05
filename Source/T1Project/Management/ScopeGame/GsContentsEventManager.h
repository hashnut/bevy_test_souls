#pragma once

#include "Classes/GsManager.h"
#include "ContentsEvent/EGsContentsEventState.h"
#include "ContentsEvent/GsContentsEventBase.h"
#include "ContentsEvent/GsContentsEventGroup.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Message/GsMessageContents.h"
#include "Net/MessageHandler/GsNetMessageHandlerContentsEvent.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsContentsEventManager.generated.h"

class FGsContentsEventBase;
struct IGsMessageParam;

/**
 * 컨텐츠이벤트 통합 매니저
 */
UCLASS()
class UGsContentsEventManager final :
	public UObject,
	public IGsManager
{

	GENERATED_BODY()

	friend class FGsNetMessageHandlerContentsEvent;

private:
	/** 이벤트딜리게이트 목록 */
	TArray<TPair<MessageContentContentsEvent, FDelegateHandle>> _listEventDelegate;

	TMap<ContentsEventGroupId, TSharedRef<FGsContentsEventGroup>> _contentsEventGroups;

	TMap<ContentsEventType, TMap<ContentsEventId, TSharedRef<FGsContentsEventBase>>> _contentsEventsTypeMap;

public:
	void Initialize() final;

	void Finalize() final;
	
	/** 컨텐츠이벤트그룹 */

private:
	/** 현재 활성화 컨텐츠이벤트그룹을 매개변수의 모든 값으로 덮어씀.
	 *  - 기존에 비활성화중인데, 매개변수에 있다면 활성화 시킴.
	 *  - 기존에 활성화 중인데, 매개변수 목록에 없다면 비활성화 시킴.
	 *  - 이미 활성화중인 컨텐츠이벤트그룹은 그대로 데이터만 덮어씀
	 */
	void ResetActivatedContentsEventGroupDatas(const TMap<ContentsEventGroupId, FGsNetMessageHandlerContentsEvent::FGsContentsEventGroupData>& InContentsEventGroupDatas);

	/** 컨텐츠이벤트그룹 활성화 */
	void ActivateContentsEventGroup(const FGsNetMessageHandlerContentsEvent::FGsContentsEventGroupData& InContentsEventGroupData);

	/** 컨텐츠이벤트그룹 활성화 */
	void ActivateContentsEventGroup(const ContentsEventGroupId InContentsEventGroupId);

	/** 컨텐츠이벤트그룹 비활성화 */
	void DeactivateContentsEventGroup(const ContentsEventGroupId InContentsEventGroupId);

	/** 상태가 같은 모든 컨텐츠이벤트그룹 목록 */
	TArray<TWeakPtr<const FGsContentsEventGroup>> GetContentsEventGroups(const EGsContentsEventState InContentsEventState) const;

public:
	/** Hud 등에 출력해야 할 이벤트그룹 목록을 반환 */
	TArray<TWeakPtr<const FGsContentsEventGroup>> GetVisibleContentsEventGroups() const;

	/** 컨텐츠이벤트그룹 반환 */
	TWeakPtr<const FGsContentsEventGroup> GetContentsEventGroup(const ContentsEventGroupId InContentsEventGroupId) const;

	/** 컨텐츠이벤트 */

private:
	/**
	 * 매개변수로 전달되는 모든 컨텐츠이벤트를 활성화 상태로 설정한다.
	 * 그리고, 이 목록에 없는 컨텐츠이벤트는 모두 비활성 상태로 설정한다.
	 */
	void ResetActivatedContentsEventDatas(const TMap<ContentsEventType, TMap<ContentsEventId, FGsNetMessageHandlerContentsEvent::FGsContentsEventData>>& InContentsEventDatasMap);

	/** 컨텐츠이벤트 활성화 */
	void ActivateContentsEvent(const ContentsEventType InContentsEventType, const ContentsEventId InContentsEventId);

	/** 컨텐츠이벤트 비활성화 */
	void DeactivateContentsEvent(const ContentsEventType InContentsEventType, const ContentsEventId InContentsEventId);

	/** 컨텐츠이벤트ID 로 컨텐츠이벤트 조회 */
	TSharedPtr<FGsContentsEventBase> FindContentsEvent(const ContentsEventId InContentsEventId, const ContentsEventType InContentsEventType = ContentsEventType::MAX);

	/**
	 * 특정 컨텐츠이벤트그룹 내 지정한 상태를 가진 컨텐츠이벤트 목록을 반환.
	 */
	TArray<TWeakPtr<const FGsContentsEventBase>> FindContentsEvents(const ContentsEventGroupId InContentsEventGroupId, const EGsContentsEventState InContentsEventState = EGsContentsEventState::Any) const;

public:
	/**
	 * ID 로 컨텐츠이벤트 조회
	 * ex) TWeakPtr<const FGsContentsEventNpcPrayer> a = UGsContentsEventManager::GetContentsEvent<const FGsContentsEventNpcPrayer>(0);
	 */
	template<typename T = class FGsContentsEventBase>
	TWeakPtr<const T> GetContentsEvent(const ContentsEventId InContentsEventId) const
	{
		const TMap<ContentsEventId, TSharedRef<FGsContentsEventBase>>* ContentsEventMap = _contentsEventsTypeMap.Find(T::CONTENTS_EVENT_TYPE);
		if (nullptr == ContentsEventMap)
		{
			return nullptr;
		}

		const TSharedRef<FGsContentsEventBase>* ContentsEvent = ContentsEventMap->Find(InContentsEventId);
		if (nullptr == ContentsEvent)
		{
			return nullptr;
		}

		return StaticCastSharedRef<const T>(*ContentsEvent);
	}

	/**
	 * 특정 이벤트타입 중, 활성화된 컨텐츠이벤트를 조회하여 반환.
	 * 단, 동일한 타입의 컨텐츠이벤트가 2개 이상 활성화 되어 있는 경우, 에러 로그를 출력하고 nullptr 을 반홤함.
	 * 동시간 내에 반드시 하나의 컨텐츠이벤트만 활성화되어야 하는 경우 사용함.
	 * ex) TWeakPtr<const FGsContentsEventNpcPrayer> b = UGsContentsEventManager::GetUniqueActivatedContentsEvent<const FGsContentsEventNpcPrayer>();
	 */
	template<typename T = class FGsContentsEventBase>
	TWeakPtr<const T> GetUniqueActivatedContentsEvent() const
	{
		TArray<TWeakPtr<const T>> ContentsEvents = UGsContentsEventManager::GetContentsEvents<const T>(EGsContentsEventState::Activated);
		if (1 == ContentsEvents.Num())
		{
			return ContentsEvents[0];
		}

		if (1 < ContentsEvents.Num())
		{
			GSLOG(Error, TEXT("1 < ContentsEvents.Num()"));
		}

		return nullptr;
	}

	/**
	 * T 타입의 모든 컨텐츠이벤트맵 반환
	 * ex) TMap<ContentsEventId, TWeakPtr<const FGsContentsEventNpcPrayer>> c = UGsContentsEventManager::GetContentsEventMap<const FGsContentsEventNpcPrayer>();
	 */
	template<typename T = class FGsContentsEventBase>
	TMap<ContentsEventId, TWeakPtr<const T>> GetContentsEventMap() const
	{
		TMap<ContentsEventId, TWeakPtr<const T>> ContentsEventWeakMap;
		const TMap<ContentsEventId, TSharedRef<FGsContentsEventBase>>* ContentsEventMap = _contentsEventsTypeMap.Find(T::CONTENTS_EVENT_TYPE);
		if (nullptr != ContentsEventMap)
		{
			for (const TPair<ContentsEventId, TSharedRef<FGsContentsEventBase>> ContentsEventPair : *ContentsEventMap)
			{
				ContentsEventWeakMap.Emplace(ContentsEventPair.Key, StaticCastSharedRef<const T>(ContentsEventPair.Value));
			}	
		}

		return ContentsEventWeakMap;
	}

	/**
	 * 특정 상태의 모든 컨텐츠이벤트 목록을 반환
	 * ex) * TArray<TWeakPtr<const FGsContentsEventNpcPrayer>> d = UGsContentsEventManager::GetContentsEvents<const FGsContentsEventNpcPrayer>();
	 */
	template<typename T = class FGsContentsEventBase>
	TArray<TWeakPtr<const T>> GetContentsEvents(const EGsContentsEventState InContentsEventState = EGsContentsEventState::Activated) const
	{
		TArray<TWeakPtr<const T>> ContentsEvents;

		const TMap<ContentsEventId, TSharedRef<FGsContentsEventBase>>* ContentsEventMap = _contentsEventsTypeMap.Find(T::CONTENTS_EVENT_TYPE);
		if (nullptr == ContentsEventMap)
		{
			return ContentsEvents;
		}

		for (const TPair<ContentsEventId, TSharedRef<FGsContentsEventBase>>& ContentsEventPair : *ContentsEventMap)
		{
			TSharedRef<const T> ContentsEvent = StaticCastSharedRef<const T>(ContentsEventPair.Value);
			switch (InContentsEventState)
			{
			case EGsContentsEventState::Deactivated:
				if (!ContentsEvent->IsActivated())
				{
					ContentsEvents.Add(ContentsEvent);
				}
				break;

			case EGsContentsEventState::Activated:
				if (ContentsEvent->IsActivated())
				{
					ContentsEvents.Add(ContentsEvent);
				}
				break;

			case EGsContentsEventState::Any:
			default:
				ContentsEvents.Add(ContentsEvent);
				break;
			}
		}

		return ContentsEvents;
	}

public:
	/**
	 * 매개변수의 컨텐츠이벤트 중 하나라도 활성화되어 있으면 true 반환. 모두 비활성화면 false 반환.
	 */
	bool IsAnyContentsEventActivated(const ContentsEventType InContentsEventType, const TSet<int32>& InContentsEventIdSet) const;

};

#define GSContentsEvent() UGsScopeHolder::GetGameManager<UGsContentsEventManager>(UGsScopeGame::EManagerType::ContentsEvent)
