#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UTIL/GsTableUtil.h"
#include "Attendance/GsAttendanceDefine.h"
#include "T1Project.h"
#include "CommunityGirl/GsSchemaCommunityEvent.h"
#include "Shared/Client/SharedEnums/SharedCommunityEventEnum.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Attendance/GsAttendanceInfo.h"
#include "Option/GsGameUserSettings.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "CommunityEventMission/GsCommunityEventMissionData.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"

class FGsCommunityData
{
private:
	CommunityEventId _id = 0;								// 커뮤니티 event ID	
	const FGsSchemaCommunityEvent* _schemaCommunityEvent;	// 데이터테이블
	bool _isRedDot = true;									// 레드닷?
	bool _isValid = false;									// 이벤트 기간인가?
	bool _isStartWaiting = false;							// 이벤트 오픈 대기중인가?
	bool _isConditionType = false;							// 조건형 이벤트타입인가? (신규, 복귀 시점에 시작되는 이벤트)
	bool _isConditionComplated = false;						// 조건형 이벤트를 모두 보상완료하였는가?

	FDateTime _tmStart;
	FDateTime _tmEnd;

public:
	FGsCommunityData() = default;
	explicit FGsCommunityData(CommunityEventId inId, const FGsSchemaCommunityEvent* inSchemaCommunityEvent)
		:_id(inId),
		_schemaCommunityEvent(inSchemaCommunityEvent)
	{		
		
	}

public:
	const CommunityEventId GetId() const { return _id; }
	const bool IsRedDot() {	return _isRedDot; }
	const FGsSchemaCommunityEvent* GetSchemaCommunityEvent() { return _schemaCommunityEvent; }
	
	void SetRedDot(bool inValue) { _isRedDot = inValue; }
	const FDateTime GetStartTime() { return _tmStart; }
	const FDateTime GetEndTime() { return _tmEnd; }

	const bool GetIsStartWaiting() { return _isStartWaiting; }
	bool GetConditionType() { return _isConditionType; }
	void SetConditionType(bool inValue) { _isConditionType = inValue; }

	bool GetIsConditionComplated() { return _isConditionComplated; }

	CommunityEventType GetType()
	{
		return _schemaCommunityEvent != nullptr ?
			_schemaCommunityEvent->eventType : CommunityEventType::BANNER;
	}

	void ApplyData()
	{
		if (nullptr == _schemaCommunityEvent)
		{
#if WITH_EDITOR
			GSLOG(Log, TEXT("[Community] CommunityData nullptr = mId: %d, "), _id);
#endif
		}
		else
		{
			FDateTime::Parse(_schemaCommunityEvent->eventTimeStart, _tmStart);
			FDateTime::Parse(_schemaCommunityEvent->eventTimeEnd, _tmEnd);
		}

		_isValid = IsValid();
		if (false == _isValid)
		{
			_isStartWaiting = IsStartWaiting();
		}
	}

	bool IsOpen()
	{
		_isValid = IsValid();

		if (true == _isValid)
		{
			_isStartWaiting = false;
			return true;
		}

		return false;
	}

	bool IsValid()
	{
		FDateTime serverTime = FGsTimeSyncUtil::GetServerNowDateTime();

		if (GetType() != CommunityEventType::BANNER)
		{
			if (_schemaCommunityEvent->eventTimeTextType == ECommunityEventTimeType::CONDITION)
			{
				if (true == GetConditionType())
				{
					if (_tmStart <= serverTime &&
						_tmEnd > serverTime)
					{
						return true;
					}

					return false;
				}
				else
				{
					return false;
				}
			}
		}
		
		if (_tmStart <= serverTime &&
			_tmEnd > serverTime)
		{
			return true;
		}

		return false;
	}

	bool IsStartWaiting()
	{
		FDateTime serverTime = FGsTimeSyncUtil::GetServerNowDateTime();

		if (_tmStart > serverTime)
		{
			return true;
		}

		return false;
	}

	bool IsSetRedDot() 
	{
		if(false == _isRedDot)
			return false;

		return IsValid();
	}

	int GetIndex()
	{
		return _schemaCommunityEvent != nullptr ?
			_schemaCommunityEvent->tabIndex : 0;
	}

	FText GetTitleName()
	{
		return _schemaCommunityEvent != nullptr ?
			_schemaCommunityEvent->tabNameText : FText::GetEmpty();
	}

	void SetConditionTime(const time_t& inExpiredTime, const time_t& inComplatedTime)
	{ 
		if (0 >= inExpiredTime)
		{
#if WITH_EDITOR
			GSLOG(Log, TEXT("[Community] ConditionTime ExpiredTime >= 0, value : %f"), inExpiredTime);
#endif
			return;
		}

		FDateTime serverTime = FGsTimeSyncUtil::GetServerNowDateTime();

		FString serverNowDateTimeText;
		FGsTimeStringHelper::GetTimeStringNYMDHM(serverTime, serverNowDateTimeText);

#if WITH_EDITOR
		GSLOG(Log, TEXT("[Community] ServerNowDateTime , value : %s"), *serverNowDateTimeText);
#endif

		if (_schemaCommunityEvent->eventTimeTextType != ECommunityEventTimeType::CONDITION)
			return;

		_isConditionType = true;
		
		FDateTime tmExpired = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(inExpiredTime);
		FDateTime tmDurationHoursReceive((_schemaCommunityEvent->durationHoursReceive * 60 * 60) * ETimespan::TicksPerSecond);
		FTimespan startTime = tmExpired - tmDurationHoursReceive;

		_tmStart = FDateTime(startTime.GetTicks());	

		if (0 < inComplatedTime)
		{
			FDateTime tmComplated = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(inComplatedTime);
			FDateTime tmDurationHoursComplete((_schemaCommunityEvent->durationHoursComplete * 60 * 60) * ETimespan::TicksPerSecond);

			_tmEnd = tmComplated + tmDurationHoursComplete;
			_isConditionComplated = true;
		}
		else
		{
			_tmEnd = tmExpired;
		}

		FString startTimeText;
		FGsTimeStringHelper::GetTimeStringNYMDHM(_tmStart, startTimeText);
		FString endTimeText;
		FGsTimeStringHelper::GetTimeStringNYMDHM(_tmEnd, endTimeText);

#if WITH_EDITOR
		GSLOG(Log, TEXT("[Community] startDateTime , value : %s"), *startTimeText);
		GSLOG(Log, TEXT("[Community] endDateTime , value : %s"), *endTimeText);
#endif
	}

	void SetConditionComplatedTime()
	{
		if (_schemaCommunityEvent->eventTimeTextType != ECommunityEventTimeType::CONDITION)
			return;

		FDateTime serverTime = FGsTimeSyncUtil::GetServerNowDateTime();
		FDateTime tmDurationHoursComplete((_schemaCommunityEvent->durationHoursComplete * 60 * 60) * ETimespan::TicksPerSecond);

		_tmEnd = serverTime + tmDurationHoursComplete;
		_isConditionComplated = true;
	}
};