#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsTimeSyncUtil.h"

#include "Attendance/GsAttendanceDefine.h"
#include "Attendance/GsSchemaCommunityEventAttendance.h"

#include "T1Project.h"
#include "DataSchema/CommunityGirl/GsSchemaCommunityEvent.h"
#include "Shared/Client/SharedEnums/SharedCommunityEventEnum.h"

class FGsAttendanceData
{
private:
	AttendanceId	_id = 0;										// 출석부 ID
	AttendanceCount	_count = 0;										// 출석체크 횟수
	bool _isCheckInToday = false;									// 체크여부
	bool _isUsable = false;											// 이용가능한가?
	const FGsSchemaCommunityEventAttendance* _schemaAttendance;		// 데이터테이블
	const FGsSchemaCommunityEvent* _schemaCommunityEvent;			// 커뮤니티 이벤트 테이블
	bool _isCompleted = false;										// 출석판을 모두 완성하였는가?

	FDateTime _tmStart;
	FDateTime _tmEnd;

public:
	FGsAttendanceData() = default;
	explicit FGsAttendanceData(AttendanceId inId, AttendanceCount inCount, bool inIsCheckInToday, bool inIsUsable)
		:_id(inId),
		_count(inCount),
		_isCheckInToday(inIsCheckInToday),
		_isUsable(inIsUsable)
	{
	}

	explicit FGsAttendanceData(AttendanceId inId, AttendanceCount inCount, bool inIsCheckInToday, bool inIsUsable, bool inIsCompleted)
		:_id(inId),
		_count(inCount),
		_isCheckInToday(inIsCheckInToday),
		_isUsable(inIsUsable),
		_isCompleted(inIsCompleted)
	{
	}

public:
	// getter
	const AttendanceId GetId() const { return _id; }
	const AttendanceCount GetCount() const { return _count; }
	const bool IsCheckInToday() const { return _isCheckInToday; }
	const bool IsUsable() const { return _isUsable; }
	const bool IsCompleted() const { return _isCompleted; }
	const FGsSchemaCommunityEventAttendance* GetSchemaAttendance() { return _schemaAttendance; }

	const FDateTime GetStartTime() { return _tmStart; }
	const FDateTime GetEndTime() { return _tmEnd; }
	const CommunityEventConditionType GetCommunityEventConditionType()
	{
		return(nullptr == _schemaCommunityEvent) ? 
			CommunityEventConditionType::DEFAULT_USER : _schemaCommunityEvent->communityEventConditionType;
	}

	void ApplyData()
	{
		_schemaAttendance = UGsTableUtil::FindRowById<UGsTableCommunityEventAttendance, FGsSchemaCommunityEventAttendance>(_id);
		if (nullptr == _schemaAttendance)
		{
#if WITH_EDITOR
			GSLOG(Log, TEXT("[Attendance] AttendanceData nullptr = mId: %d, "), _id);
#endif
		}
	}

	// setter
	void SetCount(AttendanceCount inValue) { _count = inValue; }
	void SetCheckInToday(bool inValue) { _isCheckInToday = inValue; }
	void SetUsable(bool inValue) { _isUsable = inValue; }
	void SetCompleted(bool inValue) { _isCompleted = inValue; }
	void SetCommunityEventData(const FGsSchemaCommunityEvent* inSchemaCommunityEvent) 
	{ 
		_schemaCommunityEvent = inSchemaCommunityEvent;

		SetStartTime(inSchemaCommunityEvent->eventTimeStart);
		SetEndTime(inSchemaCommunityEvent->eventTimeEnd);
	}

	void SetStartTime(IN const FString& inStartTime) 
	{ 
		if (!inStartTime.IsEmpty())
		{
			FDateTime::Parse(inStartTime, _tmStart);
		}		
	}
	void SetEndTime(IN const FString& inEndTime) 
	{
		if (!inEndTime.IsEmpty())
		{
			FDateTime::Parse(inEndTime, _tmEnd);
		}
	}

	bool IsSetRedDot()
	{
		FDateTime currentDateTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC();

		if (currentDateTime < _tmStart || currentDateTime > _tmEnd)
		{
			return false;
		}

		return IsUsable() && !IsCompleted() ? !IsCheckInToday() : false;
	}
	
	bool IsValidDay()
	{
		if (_schemaAttendance)
		{
			int days =_schemaAttendance->rewardList.Num();
			if (days <= GetCount())
			{
				return false;
			}
		}

		return true;
	}

	void CheckCompleted(AttendanceCount InCount)
	{
		if (InCount >= _count)
		{
			_isCompleted = true;
		}
	}

	bool IsAllCompleted()
	{
		if (_schemaAttendance)
		{
			int days = _schemaAttendance->rewardList.Num();
			if (days <= GetCount())
			{
				return true;
			}
		}

		return false;
	}
};