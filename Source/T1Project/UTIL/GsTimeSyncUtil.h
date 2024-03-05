// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

/*
 * FGsNetTimeSynchronizer의 함수를 편하게 호출하기 위한 클래스
 */
struct T1PROJECT_API FGsTimeSyncUtil
{
	// 서버에서 주는 time_t 값은 서버 기준시이다.
	// 모든 시간 계산은 서버시 기준으로 진행한다.
	// time_t를 FDateTime으로 변환
	static FDateTime ConvertToDateTime(const time_t& InServerTime);

	// 서버 기준시의 time_t를 UTC 기준시의 FDateTime으로 변환
	static FDateTime ConvertServerTimeToUTCDateTime(const time_t& InServerTime);

	// 서버 기준시의 time_t를 클라이언트 기준시의 FDateTime으로 변환
	static FDateTime ConvertServerTimeToClientDateTime(const time_t& InServerTime);

	// 서버의 현재 시간을 구한다(Second)
	static time_t GetServerNowSecond();

	// 서버의 현재 시간을 구한다(FDateTime)
	static FDateTime GetServerNowDateTime();

	// 서버 시간을 UTC 기준시로 변환하여 얻는다
	// 예: 서버가 한국시(UTC+9)이면, 
	//     utc기준시 = 서버시간 - 9시간
	static FDateTime GetServerNowDateTimeUTC();

	// 서버 시간을 클라이언트 기준시로 변환하여 얻는다
	// 예: 서버가 베이징시(UTC+8)이고, 클라이언트가 한국시(UTC+9)이면,
	//     클라이언트 기준시 = 서버시간 - 8시간 + 9시간
	static FDateTime GetClientNowDateTime();

	// 서버 시간에 배속값을 적용한 월드 시간을 구한다(0~24시간. 날짜 없음)
	static FTimespan GetWorldTime();

	// 월드시간 배속값을 받아온다.
	// 예: 1.f : 현실과 동일한 시간
	// 예: 12.f : 현실시간 2시간이 월드의 하루 
	static float GetWorldTimeDilation();

	// time_t를 FDateTime으로 변환. 설정에 따라 FDateTime을 서버시로 줄지, 클라시로 줄지 결정
	//static FDateTime ConvertToDateTimeWrap(const time_t& InTime);

	// FDateTime 의 0은 월요일
	// 서버cron은 0이 일요일
	// Convert 필요
	static uint8 GetServerDayOfWeek(IN EDayOfWeek inDayOfWeek);

	// 2022-09-14 서버팀의 요청으로 서버시간시에서 클라이언트시로 표시해달란 요청이 왔다.
	// 서버시 <-> 클라이언시 수정을 용의하게 하기 위해 외부에서는 GetNowSecond 만 사용하도록 한다.
	// 서버의 현재 시간을 구한다(FDateTime)
	static FDateTime GetNowDateTime();

	// 서버시를 클라이언트시로 변환
	static FDateTime ConvertSeverDateTimeToClientDateTime(FDateTime InDateTime);

	static bool IsExpried(time_t inTime);
};
