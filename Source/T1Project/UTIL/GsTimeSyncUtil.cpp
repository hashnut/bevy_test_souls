// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTimeSyncUtil.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Net/GsNetTimeSynchronizer.h"


FDateTime FGsTimeSyncUtil::ConvertToDateTime(const time_t& InServerTime)
{
	return FGsNetTimeSynchronizer::ConvertToDateTime(InServerTime);
}

FDateTime FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(const time_t& InServerTime)
{
	return GNet()->GetTimeSync().ConvertServerTimeToUTCTime(InServerTime);
}

FDateTime FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(const time_t& InServerTime)
{
	return GNet()->GetTimeSync().ConvertServerTimeToClientTime(InServerTime);
}

time_t FGsTimeSyncUtil::GetServerNowSecond()
{
	return GNet()->GetTimeSync().GetServerNowSecond();
}

FDateTime FGsTimeSyncUtil::GetServerNowDateTime()
{
	return GNet()->GetTimeSync().GetServerNowDateTime();
}

FDateTime FGsTimeSyncUtil::GetServerNowDateTimeUTC()
{
	return GNet()->GetTimeSync().GetServerNowDateTimeUTC();
}

FDateTime FGsTimeSyncUtil::GetClientNowDateTime()
{
	return GNet()->GetTimeSync().GetClientNowDateTime();
}

FTimespan FGsTimeSyncUtil::GetWorldTime()
{
	return GNet()->GetTimeSync().GetWorldTime();
}

float FGsTimeSyncUtil::GetWorldTimeDilation()
{
	return GNet()->GetTimeSync().GetWorldTimeDilation();
}

//FDateTime FGsTimeSyncUtil::ConvertToDateTimeWrap(const time_t& InTime)
//{
//#if 1
//	// 서버 시간 그대로 표시
//	return ConvertToDateTime(InTime);
//#else
//	// 서버 시간을 클라이언트 시간으로 변환하여 표시
//	return ConvertServerTimeToClientDateTime(InTime);
//#endif
//}


uint8 FGsTimeSyncUtil::GetServerDayOfWeek(IN EDayOfWeek inDayOfWeek)
{
	uint8 outDayOfWeek = 0;
	switch (inDayOfWeek)
	{
	case EDayOfWeek::Monday:
		outDayOfWeek = 1;
		break;
	case EDayOfWeek::Tuesday:
		outDayOfWeek = 2;
		break;
	case EDayOfWeek::Wednesday:
		outDayOfWeek = 3;
		break;
	case EDayOfWeek::Thursday:
		outDayOfWeek = 4;
		break;
	case EDayOfWeek::Friday:
		outDayOfWeek = 5;
		break;
	case EDayOfWeek::Saturday:
		outDayOfWeek = 6;
		break;
	case EDayOfWeek::Sunday:
		outDayOfWeek = 0;
		break;
	default:
		break;
	}

	return outDayOfWeek;
}

// 2022-09-14 서버팀의 요청으로 서버시간시에서 클라이언트시로 표시해달란 요청이 왔다.
// 서버시 <-> 클라이언시 수정을 용의하게 하기 위해 외부에서는 GetNowSecond 만 사용하도록 한다.
// 서버의 현재 시간을 구한다(FDateTime)
FDateTime FGsTimeSyncUtil::GetNowDateTime()
{
	return GNet()->GetTimeSync().GetClientNowDateTime();
}

// 서버시간을 FDateTime으로 변환
FDateTime FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(FDateTime InDateTime)
{
	return GNet()->GetTimeSync().ConvertSeverDateTimeToClientDateTime(InDateTime);
}

bool FGsTimeSyncUtil::IsExpried(time_t inTime)
{
	FTimespan timespan = FGsTimeSyncUtil::ConvertToDateTime(inTime) - FGsTimeSyncUtil::GetServerNowDateTime();
	if (0 >= timespan.GetTotalSeconds())
	{
		return true;
	}

	return false;
}
