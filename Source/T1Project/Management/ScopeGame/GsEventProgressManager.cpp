#include "GsEventProgressManager.h"

#include "EventProgress/GsEventProgressInterface.h"

#include "DataSchema/EventProgress/GsSchemaEventProgress.h"
#include "DataSchema/EventProgress/GsSchemaEventActionEventProgress.h"

#include "UTIL/GsTableUtil.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "T1Project.h"

void FGsEventProgressManager::Update(float In_deltaTime)
{
	// 다음 진행할 이벤트 시작인지
	if (_isDirtyNextEventStart == true)
	{
		_isDirtyNextEventStart = false;

		// 예약 이벤트 실행
		StartReserveEvent();

		// 예약 데이터 초기화
		ClearNextReserveData();

		// backup -> next
		// clear backup 
		ReserveBackupData();
	}
			
	// reserved finish event
	FinishReserveEvent();
}

// 이벤트 시작 요청
void FGsEventProgressManager::OnReqStartEvent(EGsEventProgressType In_type, IGsEventProgressInterface* In_eventWorker, bool In_isTutorial)
{
	const FGsSchemaEventProgress* reqEventProgressTbl =
		UGsTableUtil::FindRowById<UGsTableEventProgress, FGsSchemaEventProgress>(In_type);
	// 요청 데이터 없음
	if (reqEventProgressTbl == nullptr)
	{
		const FString failString = FString::Printf(TEXT("OnReqStartEvent FAIL BY EGsEventProgressType %d table is NONE"), (int)In_type);
		FText errorText = FText::FromString(failString);
		FGsUIHelper::PopupOK(errorText);
		return;
	}

	// 기존께 없다면 바로 시작
	if (_currentEventType == EGsEventProgressType::NONE &&
		In_eventWorker != nullptr)
	{
#ifdef TEST_LOG
		GSLOG(Warning, TEXT("--------------OnReqStartEvent ReserveNextEvent _currentEventType == EGsEventProgressType::NONE"));
#endif

		ReserveNextEvent(reqEventProgressTbl->_isDirect, In_eventWorker, In_type, false);
		return;
	}

	const FGsSchemaEventProgress* currentEventProgressTbl =
		UGsTableUtil::FindRowById<UGsTableEventProgress, FGsSchemaEventProgress>(_currentEventType);

	// 기존께 데이터 없음
	if (currentEventProgressTbl == nullptr)
	{
		return;
	}


	EGsEventProgressWorkType workType = EGsEventProgressWorkType::NOT_DO;
	if (In_isTutorial == true)
	{
		workType = EGsEventProgressWorkType::DO_NOW;
	}
	else
	{
		// 1. 소환 아이템 사용후
		// 2. 이벤트 액션 진입
		// 3. 페어리 획득 패킷 받아서 뽑기 연출 진입
		// 뽑기 연출이 이벤트 액션보다 우선순위가 높아서 화면이 깨지는 현상 나옴
		// 새로운 이벤트가 현재 진행중인 타입이 무조건 대기해야 할 타입이 있다면 대기(ex: 뽑기룸 데이터에 _forcedWaitCurrentTypes 배열에 시네마틱)
		// https://jira.com2us.com/jira/browse/C2URWQ-4957
		if(reqEventProgressTbl->_forcedWaitCurrentTypes.Contains(_currentEventType) == true)
		{
			workType = EGsEventProgressWorkType::WAIT_PRE_EVENT;
		}
		else
		{		
			// 기존꺼와 order 값 비교
			bool isLower = false;
			// 최신꺼가 order가 더크면(크면 낮은것)
			if (currentEventProgressTbl->_order < reqEventProgressTbl->_order)
			{
				isLower = true;
			}

			workType = (isLower == true) ?
				reqEventProgressTbl->_lowerOrder : reqEventProgressTbl->_higherOrder;
		}
	}

	switch (workType)
	{
		// 바로 진행
	case EGsEventProgressWorkType::DO_NOW:		
	{
		// 다음꺼랑 현재꺼가 다를때만 이전꺼 finish 함
		if (In_type != _currentEventType)
		{
			// 정리용 finish 하는도중 해당 컨텐츠의 onfinishevent를 막는다
			_isLockOnFinishEvent = true;
			// 기존꺼 정리
			if (_currentEventWorker != nullptr)
			{
				_currentEventWorker->FinishEvent(_currentEventType, In_type);
			}

			_isLockOnFinishEvent = false;

			// if next event is make backup types
			if (true == currentEventProgressTbl->_makeBackupTypes.Contains(In_type))
			{
				// if current event table use backup
				_backupEventWorker = _currentEventWorker;
				_backupEventType = _currentEventType;
			}
		}
#ifdef TEST_LOG
		GSLOG(Warning, TEXT("--------------OnReqStartEvent ReserveNextEvent DO_NOW"));
#endif
		ReserveNextEvent(reqEventProgressTbl->_isDirect, In_eventWorker, In_type, true);


		break;
	}
	// 이전 이벤트 끝나면 진행
	case EGsEventProgressWorkType::WAIT_PRE_EVENT:
	{
#ifdef TEST_LOG
		GSLOG(Warning, TEXT("--------------OnReqStartEvent ReserveNextEvent WAIT_PRE_EVENT"));
#endif
		// 예약 정보 저장
		AddNextInfo(In_eventWorker, In_type);
		break;
	}
	// 아무것도 안함
	case EGsEventProgressWorkType::NOT_DO:
		In_eventWorker->OnCanceledStartEvent();
		break;
	}
}

// 플레이중 이벤트 끝 호출받음
void FGsEventProgressManager::OnFinishEvent(EGsEventProgressType In_type)
{
	// 현재 진행 이벤트가 아님
	// onfinishevent 막은 상태인지(강제 진행으로 인한 정리중인지)
	if (In_type != _currentEventType ||
		_isLockOnFinishEvent == true)
	{
		return;
	}

	FGsEventProgressInfo* nextInfo = GetNextInfo();

	// 다음께 있으면 최근값 저장
	if (nextInfo != nullptr &&
		nextInfo->_eventType != EGsEventProgressType::NONE)
	{
		// 백업
		_preEventType = _currentEventType;
	}
	// 아니면 클리어
	else
	{
		_preEventType = EGsEventProgressType::NONE;
	}

	//  초기화
	_currentEventType = EGsEventProgressType::NONE;
	_currentEventWorker = nullptr;


	// 다음이벤트가 있으면
	if (nextInfo != nullptr &&
		nextInfo->_eventType != EGsEventProgressType::NONE)
	{
		// dirty 처리
		_isDirtyNextEventStart = true;
	}
}
// 예약된 이벤트 시작
void FGsEventProgressManager::StartReserveEvent()
{
	FGsEventProgressInfo* nextInfo = GetNextInfo();
	if (nullptr == nextInfo ||
		nextInfo->_eventWorker == nullptr ||
		nextInfo->_eventType == EGsEventProgressType::NONE)
	{
		return;
	}

	_currentEventWorker = nextInfo->_eventWorker;
	_currentEventType = nextInfo->_eventType;

	nextInfo->_eventWorker->StartEvent(nextInfo->_eventType, _preEventType);
}

// 기존 데이터 클리어
void FGsEventProgressManager::ClearReserveData()
{
	if (_arrayNextEventInfo.Num() == 0)
	{
		return;
	}

	for (FGsEventProgressInfo* iter: _arrayNextEventInfo)
	{
		if (iter == nullptr)
		{
			continue;
		}
		delete iter;
	}
	_arrayNextEventInfo.Empty();
}

// 모든 데이터 클리어
void FGsEventProgressManager::ClearAllData()
{
	ClearReserveData();

	//  초기화
	_currentEventType = EGsEventProgressType::NONE;
	_currentEventWorker = nullptr;

	_preEventType = EGsEventProgressType::NONE;

	// backup data clear
	_backupEventWorker = nullptr;
	_backupEventType = EGsEventProgressType::NONE;
}

// use backup data to next event
void FGsEventProgressManager::ReserveBackupData()
{
	if (_backupEventWorker == nullptr ||
		_backupEventType == EGsEventProgressType::NONE)
	{
		return;
	}

	// set next data
	AddNextInfo(_backupEventWorker, _backupEventType);

	// clear backup data
	_backupEventWorker = nullptr;
	_backupEventType = EGsEventProgressType::NONE;
}
int FGsEventProgressManager::GetEventProgressOrder(EGsEventProgressType In_type)
{
	const FGsSchemaEventProgress* eventProgressTbl =
		UGsTableUtil::FindRowById<UGsTableEventProgress, FGsSchemaEventProgress>(In_type);

	if (eventProgressTbl == nullptr)
	{
		return -1;
	}
	return eventProgressTbl->_order;
}

void FGsEventProgressManager::AddNextInfo(IGsEventProgressInterface* In_nextEventWorker, EGsEventProgressType In_nextEventType)
{
	// already exist -> del
	RemoveNextReserveDataByType(In_nextEventType);

	int order = GetEventProgressOrder(In_nextEventType);

	FGsEventProgressInfo* newInfo = new FGsEventProgressInfo(In_nextEventWorker, In_nextEventType, order, false);
	_arrayNextEventInfo.Add(newInfo);

	SortByOrderNextEventInfo();
}
void FGsEventProgressManager::SortByOrderNextEventInfo()
{
	_arrayNextEventInfo.Sort([](FGsEventProgressInfo& lhs, FGsEventProgressInfo& rhs)
		{
			// 1. priority 체크
			if (lhs._isPriorityHigh != rhs._isPriorityHigh)
			{
				// 왼쪽이 high면 앞으로
				if (lhs._isPriorityHigh == true)
				{
					return true;
				}
				// 아니면 뒤로
				else
				{
					return false;
				}
			}
			// 2. order 높이 체크(숫자가 작을수록 높은거임)

			return lhs._order < rhs._order;
		});

#ifdef TEST_LOG
	for (int i = 0; i < _arrayNextEventInfo.Num(); ++i)
	{
		FGsEventProgressInfo* info = _arrayNextEventInfo[i];
		if (info == nullptr)
		{
			continue;
		}

		GSLOG(Warning, TEXT("--------------event progress info type : %d, order: %d"), (int)info->_eventType, info->_order);
		
	}
		
#endif
}
FGsEventProgressInfo* FGsEventProgressManager::GetNextInfo()
{
	if (_arrayNextEventInfo.Num() == 0)
	{
		return nullptr;
	}

	return _arrayNextEventInfo[0];
}

void FGsEventProgressManager::ClearNextReserveData()
{
	if (_arrayNextEventInfo.Num() == 0)
	{
		return;
	}

	FGsEventProgressInfo* findData = _arrayNextEventInfo[0];
	if (findData != nullptr)
	{
		delete findData;
	}
	// remove first index
	_arrayNextEventInfo.RemoveAt(0);
}

// 해제
void FGsEventProgressManager::Finalize()
{
	ClearAllData();
}

// 다음 이벤트 강제로 세팅
void FGsEventProgressManager::AddNextInfoFirstIndex(IGsEventProgressInterface* In_nextEventWorker, EGsEventProgressType In_nextEventType)
{
	// already exist -> del
	RemoveNextReserveDataByType(In_nextEventType);

	int order = GetEventProgressOrder(In_nextEventType);

	FGsEventProgressInfo* newInfo = new FGsEventProgressInfo(In_nextEventWorker, In_nextEventType, order, true);
	_arrayNextEventInfo.Add(newInfo);

	SortByOrderNextEventInfo();
}

void FGsEventProgressManager::RemoveNextReserveDataByType(EGsEventProgressType In_type)
{

	int findIndex = -1;
	for (int i =0; i< _arrayNextEventInfo.Num() ; ++i)
	{
		FGsEventProgressInfo* info = _arrayNextEventInfo[i];
		if (info == nullptr)
		{
			continue;
		}

		if (info->_eventType == In_type)
		{
			findIndex = i;
			break;
		}
	}
	if (findIndex == -1)
	{
		return;
	}

	delete _arrayNextEventInfo[findIndex];

	_arrayNextEventInfo.RemoveAt(findIndex);
}

// reserved event finish
void FGsEventProgressManager::FinishReserveEvent()
{
	if (_nextFinishEventType == EGsEventProgressType::NONE)
	{
		return;
	}

	OnFinishEvent(_nextFinishEventType);

	_nextFinishEventType = EGsEventProgressType::NONE;
}

// next update req finish event
void FGsEventProgressManager::OnReqFinishEvent(EGsEventProgressType In_type, bool In_isDirect)
{
	if (In_isDirect == false)
	{
		_nextFinishEventType = In_type;
	}
	else
	{
		OnFinishEvent(In_type);
	}
}

void FGsEventProgressManager::OnReqStartEventByEventAction(EventActionType In_type, IGsEventProgressInterface* In_eventWorker)
{
	const FGsSchemaEventActionEventProgress* eventActionEventProgressTbl =
		UGsTableUtil::FindRowById<UGsTableEventActionEventProgress, FGsSchemaEventActionEventProgress>(In_type);

	if (eventActionEventProgressTbl == nullptr &&
		In_eventWorker != nullptr)
	{
		// not exist type, run direct
		In_eventWorker->StartEvent(EGsEventProgressType::NONE, EGsEventProgressType::NONE);
		return;
	}

	OnReqStartEvent(eventActionEventProgressTbl->eventProgressType, In_eventWorker);
}
void FGsEventProgressManager::OnFinishEventByEventAction(EventActionType In_type)
{
	const FGsSchemaEventActionEventProgress* eventActionEventProgressTbl =
		UGsTableUtil::FindRowById<UGsTableEventActionEventProgress, FGsSchemaEventActionEventProgress>(In_type);

	if (eventActionEventProgressTbl == nullptr)
	{
		return;
	}

	OnFinishEvent(eventActionEventProgressTbl->eventProgressType);
}

void FGsEventProgressManager::ReserveNextEvent(bool In_isDirect, 
	IGsEventProgressInterface* In_eventWorker, EGsEventProgressType In_type,
	bool In_isPreExist)
{
	if (In_eventWorker == nullptr)
	{
		return;
	}

	if (In_isDirect == true)
	{
		if (In_isPreExist == true)
		{
			// 백업
			_preEventType = _currentEventType;
		}

		_currentEventWorker = In_eventWorker;
		_currentEventType = In_type;
#ifdef TEST_LOG
		GSLOG(Warning, TEXT("--------------ReserveNextEvent In_isDirect true StartEvent"));
#endif

		In_eventWorker->StartEvent(In_type, _preEventType);
	}
	else
	{
		// dirty 처리
		_isDirtyNextEventStart = true;

		if (In_isPreExist == true)
		{
			// 백업
			_preEventType = _currentEventType;

#ifdef TEST_LOG
			GSLOG(Warning, TEXT("--------------ReserveNextEvent In_isDirect false AddNextInfoFirstIndex"));
#endif

			// 예약 정보 저장(first index)
			AddNextInfoFirstIndex(In_eventWorker, In_type);
		}
		else
		{
#ifdef TEST_LOG
			GSLOG(Warning, TEXT("--------------ReserveNextEvent In_isDirect false AddNextInfo"));
#endif
			// 예약 정보 저장
			AddNextInfo(In_eventWorker, In_type);
		}
	}
}

int32 FGsEventProgressManager::GetReservedEventCount()
{
	return _arrayNextEventInfo.Num();
}
