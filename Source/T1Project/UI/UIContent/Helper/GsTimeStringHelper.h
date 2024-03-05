// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

struct FGsSchemaCultureTimeText;

/*
 * FGsTimeStringHelper
 * 시간 표시 편의를 위한 static 함수 등록
 */
struct T1PROJECT_API FGsTimeStringHelper
{	
public:
	// InTime을 DD일 HH시간 MM분 SS초 형태로 OutString에 담아준다. 
	// 값이 0이면 생략한다.
	static void GetTimeString(const FTimespan& InTime, FString& OutString);

	// 2단어까지만 표현하며, 잘린 부분은 올림처리하여 표기한다.	
	//	- 예: +3.03:03:15 -> 3일 4시간	
	//	- 예: +03:03:15 -> 3시간 4분
	//	- 예: +03:00:00 -> 3시간
	//	- 예: +03:15 -> 3분 15초
	//	- 예: +00:15 -> 15초
	// bUseSecond 가 false인 경우,
	// 	- 예: +03:15 -> 4분
	//	- 예: +00:15 -> 1분 미만
	static void GetTimeStringCeil(const FTimespan& InTime, FString& OutString, bool bUseSecond = true);

	// 2단어까지만 표현하지만 잘린 부분은 버림처리하여 표기한다.
	// - 예: +3.03:03:15 -> 3일 3시간
	// - 예: +03:03:15 -> 3시간 3분
	// - 예: +03:15 -> 3분 15초
	// - 예: +00:15 -> 15초
	static void GetTimeStringFloor(const FTimespan& InTime, FString& OutString);

	// InTime을 HH:MM:SS 형태로 OutString에 담아준다.
	// - bCheckHourZero: True: H 값이 0일 경우 경우 MS로 표시 / False: 무조건 HMS 표기	
	// - 예: 2분 33초
	//	- True일 경우 02:33
	//	- False일 경우 00:02:33
	static void GetTimeStringHMS(const FTimespan& InTime, FString& OutString, bool bCheckHourZero = true);

	static void GetTimeStringHM(const FDateTime& InTime, FString& OutString);

	// InTime이 현재 시간기준으로 얼만큼 경과했는지 OutString에 담아준다.
	// - 예 : 1일전, 1시간전, 1분전, 1분미만
	static void GetElapsedTime(time_t inTime, FString& OutString);

	// n일
	static FString GetDayString(int32 InValue);

	// n시간
	static FString GetHourString(int32 InValue);

	// n분
	static FString GetMinuteString(int32 InValue);
	
	// n초
	static FString GetSecondString(int32 InValue);

	// n초 (소숫점도 표기)
	static FString GetSecondString(float InValue);

	// yyyy.mm.dd hh:mm (2021.02.04 18:55)
	static void GetTimeStringNYMDHM(time_t inTime, FString& OutString);

	// 전 컨텐츠에서 통일된 시간표기를 위하여 추가
	// 각 컨텐츠마다 로컬 시간 계산이 필요하거나 필요하지 않는 경우가 있으니
	// 각자 시간 계산해서 계산 된 시간값을 넣어주면 String table 형식으로 반환 (기획쪽 요청)
	// yyyy.mm.dd. hh:mm:ss
	static void GetTimeStringNYMDHMS(const FDateTime& InTime, FString& OutString);
	static void GetTimeStringNYMDHM(const FDateTime& InTime, FString& OutString);
	static void GetTimeStringNYMD(const FDateTime& InTime, FString& OutString);

	// InIsShowSecond == false 일 경우 30초이하일 때 1분 미만 표기
	// dd일 hh시 mm분 (ss초)
	static void GetTimeStringDungeon(const FTimespan& InTime, bool InIsShowSecond, OUT FString& OutString);

	// Verbosity Level 을 인자로 넘기고 싶었으나, UE_LOG 에 인자를 넣으면 
	// LogMacros 의 UE_LOG_EXPAND_IS_FATAL 에서 컴파일 에러가 난다.
	// 읽어보면 Verbosity 가 static time 에 정해져 있어야 한다는 것을 알 수 있는데
	// 실제로 로그를 찍는 함수들이 전부 constexpr 함수일리가 없으므로, Verbosity Level 마다 함수를 만들었다.
	static void PrintCurrentTimeWithMillisecondsError(const FString& InLogString = TEXT(""));
	static void PrintCurrentTimeWithMillisecondsWarning(const FString& InLogString = TEXT(""));
	static void PrintCurrentTimeWithMillisecondsDisplay(const FString& InLogString = TEXT(""));

protected:
	static FString GetTimeStringByTextKey(const FTextKey& InKey, const int32 InValue);

	// https://jira.com2us.com/jira/browse/CHR-17603?filter=-1
	// 날짜 표기 개선
	static void GetCultureTimeNYMDHM(const FDateTime& InTime, FString& OutString);
	static void GetCultureTimeNYMDHMS(const FDateTime& InTime, FString& OutString);
	static void GetCultureTimeNYMD(const FDateTime& InTime, FString& OutString);
	static const FGsSchemaCultureTimeText* GetSchemaBEGsTimeDisplayText(const EGsTimeDisplayType& InId, const FString& InStrCulture);
};
