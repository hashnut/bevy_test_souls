#pragma once

#include "Classes/GsMessageHandlerOneParam.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"

template<MessageContentContentsEvent InMessageId>
struct TGsContentsEventMessageParamBase : public IGsMessageParam
{

	static constexpr MessageContentContentsEvent MESSAGE_ID = InMessageId;

protected:
	TGsContentsEventMessageParamBase() = default;
	virtual ~TGsContentsEventMessageParamBase() = default;

public:
	void Send() const
	{
		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr == MessageHolder)
		{
			GSLOG(Error, TEXT("nullptr == MessageHolder"));
			return;
		}

		MessageHolder->GetContentsEvent().SendMessage(MESSAGE_ID, *this);
	}

public:
	static MContentsEvent* GetMessageHandler()
	{
		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr == MessageHolder)
		{
			GSLOG(Error, TEXT("nullptr == MessageHolder"));
			return nullptr;
		}

		return &MessageHolder->GetContentsEvent();
	}
};

/**
 * [이벤트] 컨텐츠이벤트그룹 상태 변경됨
 * 
 * ex)
	MContentsEvent* ContentsEvent = FGsContentsEventMessageParamContentsEventGroupStateChanged::GetMessageHandler();
	_contentsEventHandle = ContentsEvent->AddUObject(MessageContentContentsEvent::CONTENTS_EVENT_GROUP_STATE_CHANGED, this, &UGsUIHUDContentsEventList::OnContentsEventGroupStateChanged);
 */
struct FGsContentsEventMessageParamContentsEventGroupStateChanged final : public TGsContentsEventMessageParamBase<MessageContentContentsEvent::CONTENTS_EVENT_GROUP_STATE_CHANGED>
{

	const ContentsEventGroupId _contentsEventGroupId;
	const bool _isActivated;

	FGsContentsEventMessageParamContentsEventGroupStateChanged(const ContentsEventGroupId InContentsEventGroupId, const bool InIsActivated)
		: _contentsEventGroupId(InContentsEventGroupId)
		, _isActivated(InIsActivated)
	{
	}
	virtual ~FGsContentsEventMessageParamContentsEventGroupStateChanged() = default;

};

/**
 * [이벤트] 컨텐츠이벤트 상태 변경됨
 *
 * ex)
	MContentsEvent* ContentsEvent = FGsContentsEventMessageParamContentsEventStateChanged::GetMessageHandler();
	_contentsEventHandle = ContentsEvent->AddUObject(MessageContentContentsEvent::CONTENTS_EVENT_STATE_CHANGED, this, &UGsUIHUDContentsEventList::OnContentsEventStateChanged);
 */
struct FGsContentsEventMessageParamContentsEventStateChanged final : public TGsContentsEventMessageParamBase<MessageContentContentsEvent::CONTENTS_EVENT_STATE_CHANGED>
{

	const ContentsEventType _contentsEventType;
	const ContentsEventId _contentsEventId;
	const bool _isActivated;

	FGsContentsEventMessageParamContentsEventStateChanged(const ContentsEventType InContentsEventType, const ContentsEventId InContentsEventId, const bool InIsActivated)
		: _contentsEventType(InContentsEventType)
		, _contentsEventId(InContentsEventId)
		, _isActivated(InIsActivated)
	{
	}
	virtual ~FGsContentsEventMessageParamContentsEventStateChanged() = default;
};

/**
 * [이벤트] 컨텐츠이벤트 상태 변경은 여러 개의 컨텐츠이벤트가 변경될 수 있음.
 * 모든 컨텐츠이벤트의 상태를 변경 완료 한 후 발생함.
 *
 * ex)
	MContentsEvent* ContentsEvent = FGsContentsEventMessageParamContentsEventStateChangedCompleted::GetMessageHandler();
	_contentsEventHandle = ContentsEvent->AddUObject(MessageContentContentsEvent::CONTENTS_EVENT_STATE_CHANGED_COMPLETED, this, &UGsUIHUDContentsEventList::OnContentsEventStateChangedCompleted);
 */
struct FGsContentsEventMessageParamContentsEventStateChangedCompleted final : public TGsContentsEventMessageParamBase<MessageContentContentsEvent::CONTENTS_EVENT_STATE_CHANGED_COMPLETED>
{

	/** 상태가 변경된 컨텐츠이벤트들의 ContentsEventType 모음 */
	const TSet<ContentsEventType> _affectedContentsEventTypes;

	explicit FGsContentsEventMessageParamContentsEventStateChangedCompleted(TSet<ContentsEventType> InAffectedContentsEventTypes)
		: _affectedContentsEventTypes(MoveTemp(InAffectedContentsEventTypes))
	{
	}
	virtual ~FGsContentsEventMessageParamContentsEventStateChangedCompleted() = default;

	bool IsAffected(const ContentsEventType InContentsEventType) const
	{
		return _affectedContentsEventTypes.Contains(InContentsEventType);
	}

};
