#include "GsNetTimeSynchronizer.h"

#include "Kismet/KismetSystemLibrary.h"
#include "T1Project/GSGameInstance.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "T1Project.h"

//UWorld::GetRealTimeSeconds가 월드 입장 시 리셋되기 때문에, 흐른시간을 계산할 때 기기시간(FDateTime)을 사용하도록 수정되었다.

FDateTime FGsNetTimeSynchronizer::ConvertToDateTime(const time_t& InTime)
{
	return FDateTime(1970, 1, 1) + FTimespan(InTime * ETimespan::TicksPerSecond);

	// double 로 값이 잘릴까봐 FromSeconds 쓰지 않음
	//return FDateTime(1970, 1, 1) + FTimespan::FromSeconds(InTime);
}

void FGsNetTimeSynchronizer::Initialize(int64 InFrequency, uint64 InTickCount, const time_t& InNowTime, 
	float InTimeDilation, const time_t& InUTCOffsetSecond)
{
	_serverFrequency = InFrequency;
	_serverTickCount = InTickCount;
	_serverNowTime = InNowTime;
	_worldTimeDilation = InTimeDilation;
	_serverUTCOffsetSecond = InUTCOffsetSecond;
	
	// 첫 갱신 시간 저장
	_initSyncAppTime = FApp::GetCurrentTime();
	_lastSyncAppTime = _initSyncAppTime;
}

void FGsNetTimeSynchronizer::SetNowTime(const time_t& inNowTime, const time_t& inUTCOffsetSecond)
{
	_serverNowTime = inNowTime;
	_serverUTCOffsetSecond = inUTCOffsetSecond;

	// 갱신 시간 저장
	_lastSyncAppTime = FApp::GetCurrentTime();
}


uint64 FGsNetTimeSynchronizer::GetServerTickMillisecond() const
{
	// 주의. _serverTickCount는 처음 한번만 받고, 이후 갱신하지 않는다.
	// 따라서 흐른시간을 계산할 때, _initSyncTimeUTC을 사용한다.
	double elapsedTime = FApp::GetCurrentTime() - _initSyncAppTime;
	if (0 > elapsedTime)
	{
		elapsedTime = 0.f;
	}

	// appTime은 초단위고, 밀리세컨드를 리턴해야 해서 1000을 곱함
	return _serverTickCount + FMath::FloorToInt(elapsedTime * 1000);
}

time_t FGsNetTimeSynchronizer::GetServerNowSecond() const
{
	time_t elapsedSec = GetElapsedSecond();
	return _serverNowTime + elapsedSec;
}

FDateTime FGsNetTimeSynchronizer::GetServerNowDateTime() const
{
	return ConvertToDateTime(GetServerNowSecond());
}

FDateTime FGsNetTimeSynchronizer::GetServerNowDateTimeUTC() const
{
	time_t serverUTCSecond = ConvertServerSecondToUTCSecond(GetServerNowSecond());

	return ConvertToDateTime(serverUTCSecond);
}

FDateTime FGsNetTimeSynchronizer::GetClientNowDateTime() const
{
	FTimespan clientUTCOffset = GetClientUTCOffset();

	return GetServerNowDateTimeUTC() + clientUTCOffset;
}

FTimespan FGsNetTimeSynchronizer::GetWorldTime() const
{
	uint64 daySecond = 86400; // 24 * 60 * 60

	// 숫자 허용치를 넘어가 계산 오류가 나지 않게 주의
	uint64 worldTimeSec = GetServerNowSecond() % daySecond; // 월드시간에서 day 값 제거
	worldTimeSec *= _worldTimeDilation; // 시간 배속을 곱함
	worldTimeSec %= daySecond; // 배속 된 시간 day값을 다시 제거

	return FTimespan(worldTimeSec * ETimespan::TicksPerSecond);

	// double 로 값이 잘릴까봐 FromSeconds 쓰지 않음
	//return FTimespan::FromSeconds(worldTimeSec);
}

time_t FGsNetTimeSynchronizer::GetElapsedSecond() const
{
	// 현재 시간에서 마지막 싱크했을 때 받은 시간을 빼서, 흐른 시간을 구한다.
	double elapsedTime = FApp::GetCurrentTime() - _lastSyncAppTime;
	if (0 > elapsedTime)
	{
		elapsedTime = 0.f;
	}

	// 초단위
	return time_t(FMath::FloorToInt(elapsedTime));
}

// 서버 기준시로 받은 time_t를 UTC 기준시로 변환
FDateTime FGsNetTimeSynchronizer::ConvertServerTimeToUTCTime(const time_t& InServerTime) const
{
	time_t serverUTCSecond = ConvertServerSecondToUTCSecond(InServerTime);
	return ConvertToDateTime(serverUTCSecond);
}

// 서버 기준시로 받은 time_t를 클라이언트 기준시로 변환
FDateTime FGsNetTimeSynchronizer::ConvertServerTimeToClientTime(const time_t& InServerTime) const
{
	time_t serverUTCSecond = ConvertServerSecondToUTCSecond(InServerTime);
	FTimespan clientUTCOffset = GetClientUTCOffset();

	return ConvertToDateTime(serverUTCSecond) + clientUTCOffset;
}

time_t FGsNetTimeSynchronizer::ConvertServerSecondToUTCSecond(const time_t& InServerSecond) const
{
	return InServerSecond - _serverUTCOffsetSecond;
}

FTimespan FGsNetTimeSynchronizer::GetClientUTCOffset() const
{
	return FDateTime::Now() - FDateTime::UtcNow();
}

void FGsNetTimeSynchronizer::PrintServerTime()
{
	FDateTime serverNow = GetServerNowDateTime();
	FDateTime serverNowUTC = GetServerNowDateTimeUTC();
	FDateTime clientNow = GetClientNowDateTime();
	FTimespan worldTime = GetWorldTime();
	FDateTime dateTimeUTCNow = FDateTime::UtcNow();
	FDateTime dateTimeNow = FDateTime::Now();

	GSLOG(Log, TEXT("\n- serverNow: %s, UTCOffsetSecond: %d\n- serverNowUTC: %s\n- clientNow: %s\n- dateTimeUTCNow: %s\n- dateTimeNow: %s\n - worldTime: %s, dilation: %f\n"),
		*serverNow.ToString(), _serverUTCOffsetSecond,
		*serverNowUTC.ToString(),
		*clientNow.ToString(),
		*dateTimeUTCNow.ToString(),
		*dateTimeNow.ToString(),
		*worldTime.ToString(),
		_worldTimeDilation);
}

FDateTime FGsNetTimeSynchronizer::ConvertSeverDateTimeToClientDateTime(FDateTime InDateTime) const
{
	FDateTime serverDateTime = InDateTime;
	FTimespan clientUTCOffset = GetClientUTCOffset();

	return serverDateTime + clientUTCOffset;
}
