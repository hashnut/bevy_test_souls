#pragma once

#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
 * 이벤트액션 요청 시 사용할 파라미터
 * 이벤트액션그룹 요청시에는 InEventActionIndex 파라미터 생략 가능
 */
struct FGsReqEventActionParam final : public IGsMessageParam
{

	QuestId				_questId;
	EventActionState	_eventActionState;
	EventActionGroupId	_eventActionGroupId;
	EventActionIndex	_lastEventActionIndex;

	// 매우 특수한 경우에만 true 로 입력할 것. 이 값이 true 인 경우, 이벤트액션을 강제로 재시작함.
	bool				_isReStart;

	FGsReqEventActionParam()
		: FGsReqEventActionParam(INVALID_QUEST_ID, EventActionState::NONE, INVALID_EVENT_ACTION_GROUP_ID, 0, false)
	{
	}

	FGsReqEventActionParam(const QuestId InQuestId, const EventActionState InEventActionState, const EventActionGroupId InEventActionGroupId, const EventActionIndex InLastEventActionIndex, const bool InIsReStart)
		: _questId(InQuestId)
		, _eventActionState(InEventActionState)
		, _eventActionGroupId(InEventActionGroupId)
		, _lastEventActionIndex(InLastEventActionIndex)
		, _isReStart(InIsReStart)
	{
//#if WITH_EDITOR
//		DebugCheckValid();
//#endif
	}

	FGsReqEventActionParam(const FGsReqEventActionParam& Rhs)
		: FGsReqEventActionParam(Rhs._questId, Rhs._eventActionState, Rhs._eventActionGroupId, Rhs._lastEventActionIndex, Rhs._isReStart)
	{
	}

	virtual ~FGsReqEventActionParam()
	{
	}

	void IncreaseLastEventActionIndex()
	{
		++_lastEventActionIndex;
	}

/* 첫번째 이벤트액션그룹ID 가져오기가 보장하기 어려워 주석.
#if WITH_EDITOR
private:
	void DebugCheckValid() const
	{
		static const EventActionGroupId FIRST_EVENT_ACTION_GROUP_ID = FGsReqEventActionParam::GetFirstEventActionGroupId();

		if (_isReStart && GetFirstEventActionGroupId() != FIRST_EVENT_ACTION_GROUP_ID)
		{
			// _isReStart 가 true 인 경우는, 퀘스트의 맨 첫번째 이벤트액션그룹인 경우에 한하여 유효함.
			// 아니라면 서버와 싱크가 실해하여 다른 이슈가 발생할 수 있음.
			UE_LOG(LogTemp, Error, TEXT("Invalid parameter error: _isReStart, _eventActionGroupId:%d"), _eventActionGroupId);
		}
	}

	static EventActionGroupId GetFirstEventActionGroupId()
	{
		EventActionGroupId FirstEventActionGroupId = INVALID_EVENT_ACTION_GROUP_ID;

		const UGsTable* QuestConfigTable = FGsSchemaQuestConfig::GetStaticTable();
		const FGsSchemaQuestConfig* Row = nullptr;
		if (nullptr != QuestConfigTable && QuestConfigTable->FindRow(TEXT("0"), Row))
		{
			const FGsQuestMainData* QuestConfigRow = static_cast<FGsQuestMainData*>(Row->Get());
			if (nullptr != QuestConfigRow)
			{
				const FGsSchemaEventActionGroup* EventActionGroup = QuestConfigRow->initEventActionGroupId.GetRow();
				FirstEventActionGroupId = EventActionGroup->id;
			}
		}

		check(FirstEventActionGroupId != INVALID_EVENT_ACTION_GROUP_ID);
		return FirstEventActionGroupId;
	}
#endif*/
};

/**
 * 이벤트액션이 종료하면 발생하는 이벤트의 파라미터
 */
struct FGsEventActionFinishedParam final : public IGsMessageParam
{
	const QuestId _questId;
	const EventActionGroupId _eventActionGroupId;

	FGsEventActionFinishedParam(const QuestId InQuestId, const EventActionGroupId InEventActionGroupId)
		: _questId(InQuestId)
		, _eventActionGroupId(InEventActionGroupId)
	{
	}

	virtual ~FGsEventActionFinishedParam()
	{
	}
};
