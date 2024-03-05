#pragma once

#include "CoreMinimal.h"

// 서버에서 내려주는 시간과 동기화하기위해 존재하는 클래스이다.
// 서버에서 시작시간을 내려주면 그시간을 기준으로 잡고 경과시간은 클라에서 처리, 합산된 시간을 쓴다

class UWorld;

class T1PROJECT_API FGsNetTimeSynchronizer
{
private:
	const double _tickMultiply = 0.001;	

private:
	int64 _serverFrequency = 0; // ?
	uint64 _serverTickCount = 0; // 서버 구동 후 흐른 시간(Millisecond)
	time_t _serverNowTime = 0; // 기준 시간(1970년1월1일) 부터 흐른 시간(Second). 서버 기준시.
	float _worldTimeDilation = 1.f; // 월드 시간 배속	
	time_t _serverUTCOffsetSecond = 0; // 서버 시간 UTCOffset 값을 Second로 저장. (예: 서버 기준시가 한국이면 9*3600)	
	double _initSyncAppTime = 0.f; // Initialize 시 받은 시간. 처음 한번만 받는 값의 연산에 사용(_serverTickCount)
	double _lastSyncAppTime = 0.f; // 마지막 갱신 시 받은 시간. 서버에 가끔 요청보내서 싱크 맞출때 갱신됨

public:
	// time_t를 FDateTime으로 변환
	static FDateTime ConvertToDateTime(const time_t& InTime);

public:
	// 서버 기준시, 서버 기준시의 UTCOffset 값을 받는다
	void Initialize(int64 InFrequency, uint64 InTickCount, const time_t& InNowTime, float InTimeDilation, 
		const time_t& InUTCOffsetSecond);

	// 서버에 종종 요청보내서 시간을 받아와 시간이 벌어지지 않게 동기화 맞춤
	void SetNowTime(const time_t& InNowTime, const time_t& InUTCOffsetSecond);

public:
	// 서버 기동 후 지난 시간을 구한다(Millisecond)
	uint64 GetServerTickMillisecond() const;
	
	// 서버의 현재 시간을 구한다(Second)
	time_t GetServerNowSecond() const;

	// 서버의 현재 시간을 구한다(FDateTime)
	FDateTime GetServerNowDateTime() const;

	// 서버 시간을 UTC 기준시로 변환하여 얻는다
	// 예: 서버가 한국시(UTC+9)이면, 
	// utc기준시 = 서버시간 - 9
	FDateTime GetServerNowDateTimeUTC() const;

	// 서버 시간을 클라이언트 기준시로 변환하여 얻는다
	// 예: 서버가 베이징시(UTC+8)이고, 클라이언트가 한국시(UTC+9)이면,
	// 클라이언트 기준시 = 서버시간 - 8 + 9
	FDateTime GetClientNowDateTime() const;

	// 서버 시간에 배속값을 적용한 월드 시간을 구한다(0~24시간. 날짜 없음)
	FTimespan GetWorldTime() const;

	// 마지막 싱크 시점부터의 경과시간을 얻는다
	time_t GetElapsedSecond() const;
	
	// 서버 기준시로 받은 time_t를 UTC 기준시로 변환
	FDateTime ConvertServerTimeToUTCTime(const time_t& InServerTime) const;

	// 서버 기준시로 받은 time_t를 클라이언트 기준시로 변환
	FDateTime ConvertServerTimeToClientTime(const time_t& InServerTime) const;

	// 서버 기준시를 클라이언트 기준시로 변환
	FDateTime ConvertSeverDateTimeToClientDateTime(FDateTime InDateTime) const;

private:
	time_t ConvertServerSecondToUTCSecond(const time_t& InServerSecond) const;
	FTimespan GetClientUTCOffset() const;

public:
	double GetTickMultiply() const { return  _tickMultiply; }
	float GetWorldTimeDilation() const { return _worldTimeDilation; }
	void PrintServerTime();
};