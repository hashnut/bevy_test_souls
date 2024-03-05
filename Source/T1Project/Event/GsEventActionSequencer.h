#pragma once

#include "Runtime/Core/Public/Delegates/Delegate.h"
#include "Runtime/Core/Public/Delegates/IDelegateInstance.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"

#include "Management/ScopeGame/GsEventManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsEventActionSequencer.generated.h"

class UGsEventActionBase;
class UGsEventActionList;
class UObject;
struct FGsSchemaEventAction;

/**
 * 이벤트액션을 순서대로 진행시키는 시퀀서
 */
UCLASS()
class UGsEventActionSequencer final : public UObject
{
	GENERATED_BODY()

	friend void UGsEventManager::Initialize();

private:
	/** 현재 실행중인 이벤트액션 리스트 */
	UPROPERTY()
	const UGsEventActionList* _eventActionList = nullptr;

	/** 이벤트액션그룹ID */
	EventActionGroupId _eventActionGroupId = INVALID_EVENT_ACTION_GROUP_ID;

	/** 현재 실행중인 이벤트 액션 인덱스. -1 이 기본값 */
	EventActionIndex _currentEventActionIndex = INVALID_EVENT_ACTION_INDEX;

	/** 이벤트액션이 완료되면 발생 */
	FSimpleDelegate _onEventActionFinished;

public:
	void Initialize();
	void Finalize();

public:
	bool IsPlaying() const;
	bool ShouldReplayOnReconnection(const EventActionIndex InEventActionIndex) const;
	void Play(const EventActionIndex InEventActionIndex);
	bool RewindEventAction();

private:
	void Pause() const;

public:
	void SetEventActionGroupId(const EventActionGroupId InEventActionGroupId);
	EventActionGroupId GetEventActionGroupId() const;
	EventActionIndex GetEventActionIndex() const;
	bool HasNextEventAction() const;
	void OnApplicationWillEnterBackground();
	void OnApplicationHasEnteredForeground();

private:
	static const UGsEventActionList* MakeEventActionList(UObject* Outer, const EventActionGroupId InEventActionGroupId);
	static UGsEventActionBase* CreateEventAction(UObject* Outer, const FGsSchemaEventAction& InEventActionData);
	bool IsValidEventActionList() const;
	UGsEventActionBase* GetEventAction(const EventActionIndex InEventActionIndex) const;
	UGsEventActionBase* GetPlayingEventAction() const;
	void OnEventActionFinished();
};
