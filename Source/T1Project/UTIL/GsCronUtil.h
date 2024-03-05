// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// iOS 패키징 시 GetTypeHash() 오류 발생하여 uint8 로 변경합니다.
enum class DescriptionCategory : uint8
{
	SEC = 0,		// 초
	MIN,			// 분
	HOUR,			// 시
	DAY,			// 일
	MONTH,			// 월
	WEEKDAY,		// 요일
	MAX
};

struct CronTrait
{
	static const int CRON_MIN_SECONDS = 0;
	static const int CRON_MAX_SECONDS = 59;

	static const int CRON_MIN_MINUTES = 0;
	static const int CRON_MAX_MINUTES = 59;

	static const int CRON_MIN_HOURS = 0;
	static const int CRON_MAX_HOURS = 23;

	static const int CRON_MIN_DAYS_OF_WEEK = 0;
	static const int CRON_MAX_DAYS_OF_WEEK = 6;

	static const int CRON_MIN_DAYS_OF_MONTH = 1;
	static const int CRON_MAX_DAYS_OF_MONTH = 31;

	static const int CRON_MIN_MONTHS = 1;
	static const int CRON_MAX_MONTHS = 12;

	static const int CRON_MAX_YEARS_DIFF = 4;

	static const TArray<FString> DAYS;
	static const TArray<FString> Days;
	static const TArray<FString> MONTHS;
};

enum class DescriptionType : size_t
{
	ONE = 0,		// 단일
	ALL,			// 모든
	RANGE,			// 범위
	LIST,			// 나열
	REPEAT,			// 반복
	DESCRIPTION_TYPE_MAX,
	DESCRIPTION_TYPE_ERROR = DESCRIPTION_TYPE_MAX
};

enum class FieldAffectType : size_t
{
	NONE = 0,		//아무 영향 없음
	INC,			// 증가
	DEC,			// 감소
	RANGE_INC,		// 범위 증가
	RANGE_DEC		// 범위 감소
};

struct Description
{
	static constexpr DescriptionType TYPE = DescriptionType::DESCRIPTION_TYPE_ERROR;
	DescriptionType mType = DescriptionType::DESCRIPTION_TYPE_ERROR;
	Description() = default;
	explicit Description(DescriptionType type) : mType(type) {};
};

struct DescriptionOne : Description
{
	static constexpr DescriptionType TYPE = DescriptionType::ONE;
	uint8 mNumber = 0;
	DescriptionOne(int number) : Description(DescriptionType::ONE), mNumber(number) {};
};

struct DescriptionAll : Description
{
	static constexpr DescriptionType TYPE = DescriptionType::ALL;
	DescriptionAll() : Description(DescriptionType::ALL) {};
};

struct DescriptionRange : Description
{
	static constexpr DescriptionType TYPE = DescriptionType::RANGE;
	uint8 mFrom = 0;
	uint8 mTo = 0;
	DescriptionRange(uint8 from, uint8 to) : Description(DescriptionType::RANGE), mFrom(from), mTo(to) {};
};

struct DescriptionList : Description
{
	static constexpr DescriptionType TYPE = DescriptionType::LIST;
	TArray<uint8> mList;
	DescriptionList(TArray<uint8>&& list) : Description(DescriptionType::LIST), mList(MoveTemp(list)) {};
};

struct DescriptionRepeat : Description
{
	static constexpr DescriptionType TYPE = DescriptionType::REPEAT;
	uint8 mStart = 0;
	uint8 mInterval = 0;
	DescriptionRepeat(uint8 start, uint8 interval) : Description(DescriptionType::REPEAT), mStart(start), mInterval(interval) {};
};

struct DescriptionError : Description
{
	static constexpr DescriptionType TYPE = DescriptionType::DESCRIPTION_TYPE_ERROR;
	DescriptionError() : Description(DescriptionType::DESCRIPTION_TYPE_ERROR) {};
};

struct CronException
{
	FString mMessage;
	explicit CronException(const TCHAR* message) : mMessage(message) {}
};

/**
 * Cron 표현식
 */
class T1PROJECT_API FGsCronUtil
{
public:
	// 요일 앞에 매주가 기본으로 붙어서
	// useEveryText == true 이면 붙이고
	// false면 매주가 빠진 버전
	// https://jira.com2us.com/jira/browse/CHR-23655
	static void GetCronString(const FString& inCronText, FString& outString, bool useEveryText = true);
	static void GetCronString(const FString& inCronText, FString& outSecond, FString& outMinuts, FString& outHour, FString& outDay, FString& outWeek);
	static void GetCronStrings(const FString& inCronText, int durationMin, OUT TArray<FString>& outTimes);
	
	static void GetWeek(const FString& inCronText, FString& outString);	
	static void GetHour(const FString& inCronText, FString& outString);
	static void GetDays(IN const FString& inCronText, OUT TArray<int32>& outDayList);
	// 주의: DescriptionType::ONE일 때만 시분초만 읽어서 저장함
	// 주의: 에러상황엔 FTimespan::MinValue() 리턴
	
	// 클라 시간을 리턴함에 유의
	static FTimespan GetTimespan(const FString& inCronText);	
	// UTC 시간을 리턴
	static FTimespan GetTimespanUTC(const FString& inCronText);

	static void GetWeekDays(IN const FString& inCronText, OUT TArray<int32>& outDayList);	

	static FDateTime GetValidDateTime(const FString& InCronText, FDateTime InCheckTime, FTimespan checkRange = FTimespan(0));

private:
	// GetValidDateTime 에서 사용하는 함수
	static int IsValidTime(IN TMap<DescriptionCategory, TSharedPtr<Description>>& descriptions, IN FDateTime inCheckTime, OUT FDateTime& selectTime, IN DescriptionCategory descType = DescriptionCategory::MONTH);
};