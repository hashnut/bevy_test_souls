#include "GsContentsEventGroup.h"

#include "DataSchema/ContentsEvent/GsSchemaContentsEventGroup.h"
#include "DataSchema/ContentsEvent/GsSchemaContentsEventListItem.h"
#include "EGsContentsEventState.h"
#include "GsContentsEventBase.h"
#include "GsTable.h"
#include "Management/ScopeGame/GsContentsEventManager.h"
#include "Message/MessageParam/GsContentsEventMessageParam.h"
#include "Net/MessageHandler/GsNetMessageHandlerContentsEvent.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UTIL/GsTimeSyncUtil.h"

FGsContentsEventGroup::FGsContentsEventGroup(const FGsSchemaContentsEventGroup* InContentsEventGroup, TArray<TWeakPtr<const FGsContentsEventBase>>& InContentsEvents)
	: _contentsEventGroupData(InContentsEventGroup)
	, _contentsEvents(MoveTemp(InContentsEvents))
	, _overridedStartUtc((nullptr != _contentsEventGroupData) ? _contentsEventGroupData->startTime : 0)
	, _overridedEndUtc((nullptr != _contentsEventGroupData) ? _contentsEventGroupData->endTime : 0)
{
	// GSLOG(Log, TEXT("Creating ContentsEventGroup(%d, (num:%d)), startUtc:%s, endUtc:%s"), GetContentsEventGroupId(), InContentsEvents.Num(), *_overridedStartUtc.ToString(), *_overridedEndUtc.ToString());

	Initialize();
}

void FGsContentsEventGroup::Initialize()
{
	if (nullptr == _contentsEventGroupData)
	{
		GSLOG(Error, TEXT("nullptr == _contentsEventGroupData"));
	}

	if (0 == _contentsEvents.Num())
	{
		GSLOG(Error, TEXT("0 == _contentsEvents.Num()"));
	}
}

void FGsContentsEventGroup::OverrideData(const FGsNetMessageHandlerContentsEvent::FGsContentsEventGroupData& InContentsEventGroupData)
{
	if (GetContentsEventGroupId() != InContentsEventGroupData._contentsEventGroupId)
	{
		GSLOG(Error, TEXT("GetContentsEventGroupId() != InContentsEventGroupData._contentsEventGroupId"));
		return;
	}

	_overridedStartUtc = InContentsEventGroupData._startUtc;
	_overridedEndUtc = InContentsEventGroupData._endUtc;

	// GSLOG(Log, TEXT("OverrideData() startUtc:%s, endUtc:%s"), *_overridedStartUtc.ToString(), *_overridedEndUtc.ToString());
}

void FGsContentsEventGroup::SetState(const EGsContentsEventState InContentsEventState)
{
	// GSLOG(Log, TEXT("FGsContentsEventGroup(%d) SetState(%d)"), GetContentsEventGroupId(), InContentsEventState);
	if (InContentsEventState != GetState())
	{
		_state = InContentsEventState;
		FGsContentsEventMessageParamContentsEventGroupStateChanged(GetContentsEventGroupId(), IsActivated()).Send();
	}
}

EGsContentsEventState FGsContentsEventGroup::GetState() const
{
	return _state;
}

ContentsEventGroupId FGsContentsEventGroup::GetContentsEventGroupId() const
{
	if (nullptr == _contentsEventGroupData)
	{
		GSLOG(Error, TEXT("nullptr == _contentsEventGroupData"));
		return -1;
	}

	return _contentsEventGroupData->id;
}

const TArray<TWeakPtr<const FGsContentsEventBase>>& FGsContentsEventGroup::GetContentsEventAll() const
{
	return _contentsEvents;
}

FDateTime FGsContentsEventGroup::GetStartUtc() const
{
	return _overridedStartUtc;
}

FDateTime FGsContentsEventGroup::GetEndUtc() const
{
	return _overridedEndUtc;
}

bool FGsContentsEventGroup::IsActivated() const
{
	return EGsContentsEventState::Activated == GetState();
}

bool FGsContentsEventGroup::CanActivate() const
{
	return !IsActivated();
}

bool FGsContentsEventGroup::CanDisplay() const
{
	return (nullptr != _contentsEventGroupData) ? _contentsEventGroupData->visible : false;
}

int32 FGsContentsEventGroup::GetDisplayPriority() const
{
	return (nullptr != _contentsEventGroupData) ? _contentsEventGroupData->displayPriority : TNumericLimits<int32>::Max();
}

FSoftObjectPath FGsContentsEventGroup::GetIcon() const
{
	return (nullptr != _contentsEventGroupData) ? _contentsEventGroupData->icon : FSoftObjectPath();
}

FText FGsContentsEventGroup::GetTitle() const
{
	return (nullptr != _contentsEventGroupData) ? _contentsEventGroupData->title : FText::GetEmpty();
}

FText FGsContentsEventGroup::GetDescription() const
{
	return (nullptr != _contentsEventGroupData) ? _contentsEventGroupData->desc : FText::GetEmpty();
}

FText FGsContentsEventGroup::GetAltDescription() const
{
	switch (_altDescDisplayType)
	{
	case EGsContentsEventGroupAltDescDisplayType::EndDate:
	{
		FDateTime LocalDateTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(GetEndUtc());
		FString LocalDateTimeStr;
		FGsTimeStringHelper::GetTimeStringNYMDHMS(LocalDateTime, LocalDateTimeStr);

		FText FormatText;
		if (FText::FindText(TEXT("ContentsEventText"), TEXT("UI_Notice_EventTimeEnd"), FormatText))
		{
			return FText::Format(FormatText, FText::FromString(LocalDateTimeStr));
		}

		return FText::FromString(LocalDateTimeStr);
	}

	case EGsContentsEventGroupAltDescDisplayType::None:
	default:
		return FText::GetEmpty();
	}
}
