#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"

#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"

#include "EventProgress/GsEventProgressDefine.h"

#include "DataCenter/Public/Shared/Client/SharedEnums/SharedEventEnum.h"

/*
	이벤트 진행 매니저
*/

class IGsEventProgressInterface;

// event progress info
class FGsEventProgressInfo
{
public:
	// event requester
	IGsEventProgressInterface* _eventWorker = nullptr;
	// event type
	EGsEventProgressType _eventType = EGsEventProgressType::NONE;
	// 순서
	int _order;
	// 순서보다 더 높은 단계인지
	// 기존에 뭐가 있고(낮은 단계)
	// do now가 한틱에 2개 들어올때
	// 우선순위 높은거 끼리의 정렬(ex: 스킬룸, 대화 연출)
	// https://jira.com2us.com/jira/browse/C2URWQ-4903
	// 1차 정렬 priority 비교: true가 더 높음
	// 2차 정렬 order 숫자 낮을수록 높음
	bool _isPriorityHigh = false;

public:
	FGsEventProgressInfo(IGsEventProgressInterface* In_worker, EGsEventProgressType In_type, int In_order, bool In_isPriorityHigh) :
		_eventWorker(In_worker), _eventType(In_type), _order(In_order), _isPriorityHigh(In_isPriorityHigh)
	{
	}
};

class FGsEventProgressManager : public IGsManager
{
private:
	// 현재 진행 이벤트
	IGsEventProgressInterface* _currentEventWorker = nullptr;
	// 현재 진행 이벤트 타입
	EGsEventProgressType _currentEventType = EGsEventProgressType::NONE;
	
	// next infos
	// single-> multi info reason: multiple requests in one tick
	// ex) save battery and tray area names
	TArray<FGsEventProgressInfo*> _arrayNextEventInfo;

	// backup event
	// 1. if pre event finished by next event
	// 2. pre event table use backup
	IGsEventProgressInterface* _backupEventWorker = nullptr;
	// 다음 이벤트 타입
	EGsEventProgressType _backupEventType = EGsEventProgressType::NONE;

	// 이전 진행 이벤트 타입
	EGsEventProgressType _preEventType = EGsEventProgressType::NONE;

	// 다음 이벤트 시작인지
	bool _isDirtyNextEventStart = false;
	// 외부 finish event 막을지
	bool _isLockOnFinishEvent = false;

	// next finish event type
	// 1. current event setting in update
	// 2. start and next code line finish(ex: QUESG_GOTO_FIELD) -> also finish in update
	EGsEventProgressType _nextFinishEventType = EGsEventProgressType::NONE;

	// 가상 함수
public:
	virtual void Update(float In_deltaTime) override;
	// 해제
	virtual void Finalize() override;
	// 외부 호출
public:
	// 이벤트 시작 요청
	void OnReqStartEvent(EGsEventProgressType In_type, IGsEventProgressInterface* In_eventWorker, bool In_isTutorial = false);
	// 플레이중 이벤트 끝 호출받음
	void OnFinishEvent(EGsEventProgressType In_type);

	void OnReqStartEventByEventAction(EventActionType In_type, IGsEventProgressInterface* In_eventWorker);
	void OnFinishEventByEventAction(EventActionType In_type);

	// 모든 데이터 클리어
	void ClearAllData();
	// add first index
	void AddNextInfoFirstIndex(IGsEventProgressInterface* In_nextEventWorker, EGsEventProgressType In_nextEventType);
	// In_isDirect == true : direct finish
	// In_isDirect == false : next update req finish event
	// 1. current event setting in update
	// 2. start and next code line finish(ex: QUESG_GOTO_FIELD) -> also finish in update
	void OnReqFinishEvent(EGsEventProgressType In_type, bool In_isDirect = true);

	// 내부 로직 함수
protected:
	// 예약된 이벤트 시작
	void StartReserveEvent();
	// 예약 데이터 클리어
	void ClearReserveData();
	// use backup data to next event
	void ReserveBackupData();

	void AddNextInfo(IGsEventProgressInterface* In_nextEventWorker, EGsEventProgressType In_nextEventType);

	FGsEventProgressInfo* GetNextInfo();
	void ClearNextReserveData();

	void RemoveNextReserveDataByType(EGsEventProgressType In_type);
	// reserved event finish
	void FinishReserveEvent();

	void SortByOrderNextEventInfo();

	int GetEventProgressOrder(EGsEventProgressType In_type);

	void ReserveNextEvent(bool In_isDirect, 
		IGsEventProgressInterface* In_eventWorker, EGsEventProgressType In_type,
		bool In_isPreExist);

	// set
public:

	
	// 다음 이벤트를 임의로 지연시키기 위해 사용
	// 절전 모드 <-> 시네마틱 : 두 이벤트는 한쪽이 끝나야만 이후 컨텐츠가 시작된다.
	void SetDirtyNextEventForce(bool InDirty) { _isDirtyNextEventStart = InDirty; }
	// 현재 진행 타입
	EGsEventProgressType GetCurrentProgressType()
	{ 
		return _currentEventType;
	}

	int32 GetReservedEventCount();
};

#define GSEventProgress() UGsScopeHolder::GetGameManagerFType<FGsEventProgressManager>(UGsScopeGame::EManagerType::EventProgress)