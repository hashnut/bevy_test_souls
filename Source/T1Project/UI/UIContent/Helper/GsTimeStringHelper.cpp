// Fill out your copyright notice in the Description page of Project Settings.

#include "GsTimeStringHelper.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "T1Project.h"
#include "Internationalization.h"
#include "CulturePointer.h"
#include "Culture.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/DataCenter/Public/DataSchema/Time/GsSchemaBEGsTimeDisplayText.h"
#include "Runtime/DataCenter/Public/DataSchema/Time/GsSchemaCultureTimeText.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"


void FGsTimeStringHelper::GetTimeString(const FTimespan& InTime, FString& OutString)
{
	// 0초 문제되어 추가
	if (0 >= InTime.GetTotalSeconds())
	{
		OutString.Append(GetSecondString(0));
		return;
	}

	if (0 < InTime.GetDays())
	{
		OutString.Append(GetDayString(InTime.GetDays()));
	}

	if (0 < InTime.GetHours())
	{
		// 공백삽입
		if (false == OutString.IsEmpty())
		{
			OutString.Append(TEXT(" "));
		}

		OutString.Append(GetHourString(InTime.GetHours()));
	}

	if (0 < InTime.GetMinutes())
	{
		// 공백삽입
		if (false == OutString.IsEmpty())
		{
			OutString.Append(TEXT(" "));
		}

		OutString.Append(GetMinuteString(InTime.GetMinutes()));
	}

	if (0 < InTime.GetSeconds())
	{
		// 공백삽입
		if (false == OutString.IsEmpty())
		{
			OutString.Append(TEXT(" "));
		}

		OutString.Append(GetSecondString(InTime.GetSeconds()));
	}
}

void FGsTimeStringHelper::GetTimeStringCeil(const FTimespan& InTime, FString& OutString, bool bUseSecond)
{
	// 0초 문제되어 추가
	if (0 >= InTime.GetTotalSeconds())
	{
		OutString.Append(GetSecondString(0));
		return;
	}

	// n일 이상: n일 n시간으로 표기
	if (0 < InTime.GetDays())
	{
		int32 hours = InTime.GetHours();		
		if (0 < (InTime.GetTicks() % ETimespan::TicksPerHour))
		{
			hours += 1; // 넘는 값이 있으면 올림처리
		}

		if (24 == hours)
		{
			OutString.Append(GetDayString(InTime.GetDays() + 1));
		}
		else
		{
			OutString.Append(GetDayString(InTime.GetDays()));
			if (0 < hours)
			{
				OutString.Append(TEXT(" ")); // 공백삽입
				OutString.Append(GetHourString(hours));
			}
		}

		return;
	}
	
	// n시간 이상: n시간 n분으로 표기
	if (0 < InTime.GetHours())
	{
		int32 minutes = InTime.GetMinutes();
		if (0 < (InTime.GetTicks() % ETimespan::TicksPerMinute))
		{
			minutes += 1; // 넘는 값이 있으면 올림처리
		}

		if (60 == minutes)
		{
			OutString.Append(GetHourString(InTime.GetHours() + 1));
		}
		else
		{
			OutString.Append(GetHourString(InTime.GetHours()));
			if (0 < minutes)
			{
				OutString.Append(TEXT(" ")); // 공백삽입
				OutString.Append(GetMinuteString(minutes));
			}
		}

		return;
	}

	// n분 이상: n분 n초로 표기.
	if (0 < InTime.GetMinutes())
	{
		if (bUseSecond)
		{
			// 초단위 이하는 올림처리 하지 않음
			OutString.Append(GetMinuteString(InTime.GetMinutes()));

			if (0 < InTime.GetSeconds())
			{
				OutString.Append(TEXT(" ")); // 공백삽입
				OutString.Append(GetSecondString(InTime.GetSeconds()));
			}
		}
		else
		{
			// n분으로 표기
			int32 seconds = InTime.GetSeconds();
			if (0 < (InTime.GetTicks() % ETimespan::TicksPerSecond))
			{
				seconds += 1; // 넘는 값이 있으면 올림처리
			}

			if (0 < seconds)
			{
				OutString.Append(GetMinuteString(InTime.GetMinutes() + 1));
			}
			else
			{
				OutString.Append(GetMinuteString(InTime.GetMinutes()));
			}
		}

		return;
	}

	if (bUseSecond)
	{
		// n초
		OutString.Append(GetSecondString(InTime.GetSeconds()));
	}
	else
	{
		// 1분 미만
		FText textSec;
		if (FText::FindText(TEXT("FriendText"), TEXT("FriendOffline_Sec"), textSec))
		{
			OutString = textSec.ToString();
		}
	}
}

void FGsTimeStringHelper::GetTimeStringFloor(const FTimespan& InTime, FString& OutString)
{
	// 0초 문제되어 추가
	if (0 >= InTime.GetTotalSeconds())
	{
		OutString.Append(GetSecondString(0));
		return;
	}

	// n일 이상: n일 n시간으로 표기
	if (0 < InTime.GetDays())
	{
		int32 hours = InTime.GetHours();
		OutString.Append(GetDayString(InTime.GetDays()));

		if (0 < hours)
		{
			OutString.Append(TEXT(" ")); // 공백삽입
			OutString.Append(GetHourString(hours));
		}
		return;
	}

	// n시간 이상: n시간 n분으로 표기
	if (0 < InTime.GetHours())
	{
		int32 minutes = InTime.GetMinutes();
		OutString.Append(GetHourString(InTime.GetHours()));

		if (0 < minutes)
		{
			OutString.Append(TEXT(" ")); // 공백삽입
			OutString.Append(GetMinuteString(minutes));
		}
		return;
	}

	// n분 이상: n분 n초로 표기.
	if (0 < InTime.GetMinutes())
	{
		int32 seconds = InTime.GetSeconds();
		OutString.Append(GetMinuteString(InTime.GetMinutes()));

		if (0 < seconds)
		{
			OutString.Append(TEXT(" ")); // 공백삽입
			OutString.Append(GetSecondString(InTime.GetSeconds()));
		}
		return;
	}

	// n초
	OutString.Append(GetSecondString(InTime.GetSeconds()));
}

void FGsTimeStringHelper::GetTimeStringHMS(const FTimespan& InTime, FString& OutString, bool bCheckHourZero)
{
	// 일단위 넘어갈 경우 H에 더해서 찍어줌
	if (0 < InTime.GetDays())
	{
		int32 hours = InTime.GetDays() * 24 + InTime.GetHours();
		if (100 > hours)
		{
			OutString = FString::Printf(TEXT("%02i:%02i:%02i"), hours, InTime.GetMinutes(), InTime.GetSeconds());
		}
		else
		{
			OutString = FString::Printf(TEXT("%d:%02i:%02i"), hours, InTime.GetMinutes(), InTime.GetSeconds());
		}

		return;
	}

	// InTime.ToString(TEXT("%h:%m:%s") 이런 형태는, 항상 +- 부호가 붙기 때문에 지워준다.
	if (false == bCheckHourZero ||
		0 < InTime.GetHours())
	{
		OutString = InTime.ToString(TEXT("%h:%m:%s"));
		OutString.RemoveAt(0);
		return;
	}

	OutString = InTime.ToString(TEXT("%m:%s"));
	OutString.RemoveAt(0);
}

void FGsTimeStringHelper::GetTimeStringHM(const FDateTime& InTime, FString& OutString)
{
	OutString = FString::Printf(TEXT("%02i:%02i"), InTime.GetHour(), InTime.GetSecond());
}

FString FGsTimeStringHelper::GetDayString(int32 InValue)
{
	// TEXT: {0}일
	return GetTimeStringByTextKey(TEXT("NDays"), InValue);
}

FString FGsTimeStringHelper::GetHourString(int32 InValue)
{
	// TEXT: {0}시간
	return GetTimeStringByTextKey(TEXT("NHours"), InValue);
}

FString FGsTimeStringHelper::GetMinuteString(int32 InValue)
{
	// TEXT: {0}분
	return GetTimeStringByTextKey(TEXT("NMinutes"), InValue);
}

FString FGsTimeStringHelper::GetSecondString(int32 InValue)
{
	// TEXT: {0}초
	return GetTimeStringByTextKey(TEXT("NSeconds"), InValue);
}

FString FGsTimeStringHelper::GetSecondString(float InValue)
{
	FText textFormat;
	if (FText::FindText(TEXT("SystemText"), TEXT("NSeconds"), textFormat))
	{
		return FText::Format(textFormat, InValue).ToString();
	}

	return TEXT("");
}

FString FGsTimeStringHelper::GetTimeStringByTextKey(const FTextKey& InKey, int32 InValue)
{
	FText textFormat;
	if (FText::FindText(TEXT("SystemText"), InKey, textFormat))
	{
		return FText::Format(textFormat, InValue).ToString();
	}

	return TEXT("");
}

void FGsTimeStringHelper::GetElapsedTime(time_t inTime, FString& OutString)
{
	FDateTime time = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(inTime);
	FDateTime nowTime = FGsTimeSyncUtil::GetClientNowDateTime();

	FTimespan span = nowTime - time;

	// n일 이상: n일 전으로 표기
	if (0 < span.GetDays())
	{
		FText textFormat;
		if (FText::FindText(TEXT("FriendText"), TEXT("FriendOffline_Day"), textFormat))
		{
			OutString = FText::Format(textFormat, span.GetDays()).ToString();
		}

		return;
	}

	// n시간 이상: n시간 전으로 표기
	if (0 < span.GetHours())
	{
		FText textFormat;
		if (FText::FindText(TEXT("FriendText"), TEXT("FriendOffline_Hour"), textFormat))
		{
			OutString = FText::Format(textFormat, span.GetHours()).ToString();
		}

		return;
	}

	// n분 이상: n분 전으로 표기.
	if (0 < span.GetMinutes())
	{
		FText textFormat;
		if (FText::FindText(TEXT("FriendText"), TEXT("FriendOffline_Min"), textFormat))
		{
			OutString = FText::Format(textFormat, span.GetMinutes()).ToString();
		}

		return;
	}

	// 1분 이하
	FText textFind;
	if (FText::FindText(TEXT("FriendText"), TEXT("FriendOffline_Sec"), textFind))
	{
		OutString = textFind.ToString();
	}
}

void FGsTimeStringHelper::GetTimeStringNYMDHM(time_t inTime, FString& OutString)
{
	FDateTime dateTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(inTime);
	GetCultureTimeNYMDHM(dateTime, OutString);
}

void FGsTimeStringHelper::GetTimeStringNYMDHMS(const FDateTime& InTime, FString& OutString)
{
	GetCultureTimeNYMDHMS(InTime, OutString);
}

void FGsTimeStringHelper::GetTimeStringNYMDHM(const FDateTime& InTime, FString& OutString)
{
	GetCultureTimeNYMDHM(InTime, OutString);
}

void FGsTimeStringHelper::GetTimeStringNYMD(const FDateTime& InTime, FString& OutString)
{
	GetCultureTimeNYMD(InTime, OutString);
}

void FGsTimeStringHelper::GetTimeStringDungeon(const FTimespan& InTime, bool InIsShowSecond, OUT FString& OutString)
{
	const static double ZERO_SEC = 0;
	double totalSeconds = InTime.GetTotalSeconds();
	if (ZERO_SEC <= totalSeconds)
	{
		const static int32 MIN_SEC = 60;
		if (InIsShowSecond || MIN_SEC < InTime.GetTotalSeconds())
		{
			OutString.Empty(0);
			if (0 < InTime.GetDays())
			{	// Day
				OutString.Append(FGsTimeStringHelper::GetDayString(InTime.GetDays()));
			}

			if (0 < InTime.GetHours())
			{	// Hours
				if (false == OutString.IsEmpty())
				{
					OutString.Append(TEXT(" "));
				}
				OutString.Append(FGsTimeStringHelper::GetHourString(InTime.GetHours()));
			}

			if (0 < InTime.GetMinutes())
			{	// Minutes
				if (false == OutString.IsEmpty())
				{
					OutString.Append(TEXT(" "));
				}
				OutString.Append(FGsTimeStringHelper::GetMinuteString(InTime.GetMinutes()));
			}
		}

		if (InIsShowSecond)
		{
			if (0 < InTime.GetSeconds())
			{	// Seconds
				if (false == OutString.IsEmpty())
				{
					OutString.Append(TEXT(" "));
				}
				OutString.Append(FGsTimeStringHelper::GetSecondString(InTime.GetSeconds()));
			}
		}
	}
}

void FGsTimeStringHelper::PrintCurrentTimeWithMillisecondsError(const FString& InLogString)
{
#if WITH_EDITOR

	FDateTime Now = FDateTime::UtcNow();
	FString TimeString = Now.ToString(TEXT("%H:%M:%S.%f"));

	if (InLogString.IsEmpty())
	{
		GSLOG(Error, TEXT("Current time : %s"), *TimeString);
	}
	else
	{
		GSLOG(Error, TEXT("Log : %s, Current time : %s"), *InLogString, *TimeString);
	}
#endif
}

void FGsTimeStringHelper::PrintCurrentTimeWithMillisecondsWarning(const FString& InLogString)
{
#if WITH_EDITOR

	FDateTime Now = FDateTime::UtcNow();
	FString TimeString = Now.ToString(TEXT("%H:%M:%S.%f"));
	if (InLogString.IsEmpty())
	{
		GSLOG(Warning, TEXT("Current time : %s"), *TimeString);
	}
	else
	{
		GSLOG(Warning, TEXT("Log : %s, Current time : %s"), *InLogString, *TimeString);
	}

#endif
}

void FGsTimeStringHelper::PrintCurrentTimeWithMillisecondsDisplay(const FString& InLogString)
{
#if WITH_EDITOR

	FDateTime Now = FDateTime::UtcNow();
	FString TimeString = Now.ToString(TEXT("%H:%M:%S.%f"));
	if (InLogString.IsEmpty())
	{
		GSLOG(Display, TEXT("Current time : %s"), *TimeString);
	}
	else
	{
		GSLOG(Display, TEXT("Log : %s, Current time : %s"), *InLogString, *TimeString);
	}

#endif
}

void FGsTimeStringHelper::GetCultureTimeNYMDHM(const FDateTime& InTime, FString& OutString)
{
	FString strTempCulture = UGsLocalizationManager::GetCurrCultureName();

	FText textFormat;
	const FGsSchemaCultureTimeText* schemaCultureTimeText = FGsTimeStringHelper::GetSchemaBEGsTimeDisplayText(EGsTimeDisplayType::YMD_HM, strTempCulture);
	if (nullptr != schemaCultureTimeText)
	{
		textFormat = schemaCultureTimeText->text;
		OutString = FText::Format(textFormat, FText::AsNumber(InTime.GetYear(), &FNumberFormattingOptions::DefaultNoGrouping()),
			InTime.GetMonth(), InTime.GetDay(),
			FText::FromString(FString::Printf(TEXT("%02i"), InTime.GetHour())),
			FText::FromString(FString::Printf(TEXT("%02i"), InTime.GetMinute()))).ToString();
	}
}

void FGsTimeStringHelper::GetCultureTimeNYMDHMS(const FDateTime& InTime, FString& OutString)
{
	FString strTempCulture = UGsLocalizationManager::GetCurrCultureName();
	
	FText textFormat;
	const FGsSchemaCultureTimeText* schemaCultureTimeText = FGsTimeStringHelper::GetSchemaBEGsTimeDisplayText(EGsTimeDisplayType::YMD_HMS, strTempCulture);
	if (nullptr != schemaCultureTimeText)
	{
		textFormat = schemaCultureTimeText->text;
		OutString = FText::Format(textFormat, FText::AsNumber(InTime.GetYear(), &FNumberFormattingOptions::DefaultNoGrouping()),
			InTime.GetMonth(), InTime.GetDay(),
			FText::FromString(FString::Printf(TEXT("%02i"), InTime.GetHour())),
			FText::FromString(FString::Printf(TEXT("%02i"), InTime.GetMinute())),
			FText::FromString(FString::Printf(TEXT("%02i"), InTime.GetSecond()))).ToString();
	}
}

void FGsTimeStringHelper::GetCultureTimeNYMD(const FDateTime& InTime, FString& OutString)
{
	FString strTempCulture = UGsLocalizationManager::GetCurrCultureName();

	FText textFormat;
	const FGsSchemaCultureTimeText* schemaCultureTimeText = FGsTimeStringHelper::GetSchemaBEGsTimeDisplayText(EGsTimeDisplayType::YMD, strTempCulture);
	if (nullptr != schemaCultureTimeText)
	{
		textFormat = schemaCultureTimeText->text;
		OutString = FText::Format(textFormat, FText::AsNumber(InTime.GetYear(), &FNumberFormattingOptions::DefaultNoGrouping()),
			InTime.GetMonth(), InTime.GetDay()).ToString();
	}
}

const FGsSchemaCultureTimeText* FGsTimeStringHelper::GetSchemaBEGsTimeDisplayText(const EGsTimeDisplayType& InId, const FString& InStrCulture)
{
	const UGsTableBEGsTimeDisplayText* table = Cast<UGsTableBEGsTimeDisplayText>(FGsSchemaBEGsTimeDisplayText::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaBEGsTimeDisplayText* schemaTimeDisplayText = nullptr;
	if (false == table->FindRowById(InId, schemaTimeDisplayText))
	{
		return nullptr;
	}

	const FGsSchemaCultureTimeText* findCultureTimeText = schemaTimeDisplayText->cultureTimeText.FindByPredicate([InStrCulture](
		const FGsSchemaCultureTimeText& el)
		{
			return el.culture == InStrCulture;
		});

	if (nullptr == findCultureTimeText)
	{
		FString defaultCultureName = TEXT("en");
		findCultureTimeText = schemaTimeDisplayText->cultureTimeText.FindByPredicate([defaultCultureName](
			const FGsSchemaCultureTimeText& el)
			{
				return el.culture == defaultCultureName;
			});
	}

	return findCultureTimeText;
}