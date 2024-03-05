#pragma once

#include "Runtime/Core/Public/Containers/Array.h"
#include "Runtime/Core/Public/Containers/Map.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"

#include "EventProgress/GsEventProgressDefine.h"
#include "EventProgress/GsEventProgressInterface.h"
#include "GsManager.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageSystem.h"
#include "Message/MessageParam/GsEventMessageParam.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsEventManager.generated.h"

class FDelegateHandle;
class UGsEventActionSequencer;
class UGsEventActionImmediate;
struct IGsMessageParam;

/**
 * 이벤트 매니저
 */
UCLASS()
class T1PROJECT_API UGsEventManager final :
	public UObject,
	public IGsManager,
	public IGsEventProgressInterface
{
	GENERATED_BODY()

private:
	struct FGsInstantSoundPlayer final
	{

		bool _soundMixerChanged;

		/**
		 * 플레이 사운드는 stop 을 위해 핸들을 보유하고 있어야 하는데, Event Action 은 계속 유지되지 않으므로 이곳에 저장함.
		 */
		TArray<TPair<FName, uint64>> _playingSoundHandles;

		FGsInstantSoundPlayer();

		~FGsInstantSoundPlayer();

		/** 사운드 핸들을 큐에 저장 */
		bool EnqueuePlayingSound(const FName InSoundResourceRowName, const uint64 InSoundHandle);

		/** 검색하여 발견한 사운드 핸들을 반환. 없을 경우 0 반환. */
		uint64 DequeuePlayingSound(const FName InSoundResourceRowName);

	};

private:
	/** 이벤트액션 전용 시퀀서 */
	UPROPERTY(Transient)
	UGsEventActionSequencer* _eventActionSequencer;

	/** 이벤트딜리게이트 목록 */
	TArray<TPair<MessageContentEvent, FDelegateHandle>> _listDelegateContentEvent;

	TArray<TPair<MessageSystem, FDelegateHandle>> _listDelegateSystem;

	/** 마지막 사용한 이벤트액션 파라미터 */
	FGsReqEventActionParam _lastReqEventActionParam;

	UPROPERTY(Transient)
	UGsEventActionImmediate* _eventActionImmediate;

	TSharedPtr<FGsInstantSoundPlayer> _soundPlayerInstant;

	/**
	 * 특정한 이벤트액션은 진행 도중에 네트워크 연결 해제 시, 진행이 정지됨. 재연결이 성공하면 진행을 계속함.
	 * 거의 대부분의 이벤트액션은 플레이 도중에 pause/play 를 하지 않으나, 일부 특수한 이벤트액션은 리커넥션 후 play 할 때,
	 * 서버로 이벤트액션 시작을 다시 보내지 않도록, 재연결 당시의 이벤트액션을 저장해 둠.
	 */
	//TSharedPtr<TPair<EventActionGroupId, EventActionIndex>> _pausedEventActionForReconnection;

	/** 최근에 재접속이 실행되었는지 여부를 저장함. */
	bool _justReconnectStarted = false;

	/** 재연결 상태 여부 */
	bool _reconnecting = false;

public:
	void Initialize() final;

	void Finalize() final;

	/** 하나의 이벤트액션그룹의 플레이가 종료되면, 관련 데이터 리셋을 위해 호출함. */
	void FinalizeEventActionGroup();

private:
	FGsInstantSoundPlayer& GetValidInstantSoundPlayer();

	void ClearReconnectionStatus();

	void OnEnterIngame();

	void OnReconnectStart();

	void OnReconnectEnd();

public:
	/** 사운드 핸들을 큐에 저장 */
	bool EnqueuePlayingSound(const FName InSoundResourceRowName, const uint64 InSoundHandle);

	/** 검색하여 발견한 사운드 핸들을 반환. 없을 경우 0 반환. */
	uint64 DequeuePlayingSound(const FName InSoundResourceRowName);

public:
	bool EventActionIsPlaying() const;

	/**
	 * 서버로 이벤트액션플레이를 요청
	 */
	void OnRequestPlayEventAction(const IGsMessageParam& Param);

	/**
	 * 현재 이벤트액션그룹 내의 마지막 이벤트액션이 종료됨
	 */
	void OnLastEventActionFinished(const IGsMessageParam& Param);

	/**
	 * 서버로부터 이벤트액션플레이 요청에 대한 응답을 받음. 실제로 이벤트 플레이는 여기서부터 시작
	 */
	void EventActionPlayResponse(const bool InSucceeded, const QuestId InQuestId, const EventActionState InEventActionState, const EventActionGroupId InEventActionGroupId, const EventActionIndex InEventActionIndex);

	/**
	 * 2022/08/09 PKT - 서버로 부터 즉시 실행할 Event 받음.
	 */
	void EventActionPlayImmediate(EventSubDialogId InId, bool InIsActive = true, const TArray<EvenParameterValue>& InParamSet = TArray<EvenParameterValue>());
	
	/**
	 * 2022/08/12 PKT - 현재 실행 되고 있는 Event Clear
	 */
	void EventActionAllClearImmediate(const IGsMessageParam& Param);

private:
	EventActionType GetCurrentEventActionType() const;

	/**
	 * @brief 특정 이벤트액션이 ClientOnly 여부를 반환.
	 * @param InEventActionGroupId 
	 * @param InEventActionIndex 
	 * @return 
	*/
	bool IsClientOnlyEventAction(const EventActionGroupId InEventActionGroupId, const EventActionIndex InEventActionIndex) const;

	/**
	 * UGsEventActionSequencer 로부터 각 이벤트액션이 종료될 때마다 호출됨
	 */
	void OnEventActionFinished();

	void TryNextEventActionPlay();

	void PlayEventAction(const EventActionGroupId InEventActionGroupId, const EventActionIndex InEventActionIndex);

	/** 재연결 등 특수한 상황에서 현재 진행중이었던 이벤트액션이 무엇이 있던지 완전히 클리어 하는 용도. */
	void ForceClearEventActionSequencer();



	void PlayEventActionImmediate(EventSubDialogId InId, bool InIsActive, const TArray<EvenParameterValue>& InParamSet);

	void AllClearEventActionImmediate();

	UFUNCTION()
	void OnApplicationWillEnterBackground();

	UFUNCTION()
	void OnApplicationHasEnteredForeground();

private:
	/**
	 * EventProgress 에게 플레이를 요청함.
	 * 이 후, EventProgress 의 제어에 의해 StartEvent 가 호출되면 실제 이벤트액션을 플레이함.
	 */
	void RequestPlayEventActionToEventProgress();

public:
	/** Implements IGsEventProgressInterface */
	void StartEvent(EGsEventProgressType /*In_startType*/, EGsEventProgressType /*In_preType*/) final;

private:
	/** 이벤트액션 데이터가 유효한 데이터인지 여부 반환 */
	static bool IsValidPlayEventActionData(const EventActionGroupId InEventActionGroupId, const EventActionIndex InEventActionIndex);
};

#define GSEvent() UGsScopeHolder::GetGameManager<UGsEventManager>(UGsScopeGame::EManagerType::Event)
