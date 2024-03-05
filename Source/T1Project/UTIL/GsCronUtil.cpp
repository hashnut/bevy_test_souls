// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCronUtil.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "T1Project.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "Kismet/KismetTextLibrary.h"
#include "AllOf.h"
#include "Copy.h"
#include "ForEach.h"
#include "Transform.h"

const TArray<FString> CronTrait::DAYS = { TEXT("SUN"), TEXT("MON"), TEXT("TUE"),TEXT("WED"),TEXT("THU"),TEXT("FRI"), TEXT("SAT") };
const TArray<FString> CronTrait::Days = { TEXT("Sun"), TEXT("Mon"), TEXT("Tue"),TEXT("Wed"),TEXT("Thu"),TEXT("Fri"), TEXT("Sat") };
const TArray<FString> CronTrait::MONTHS = { TEXT("NIL"), TEXT("JAN"), TEXT("FEB"), TEXT("MAR"), TEXT("APR"),
	TEXT("MAY"), TEXT("JUN"), TEXT("JUL"), TEXT("AUG"), TEXT("SEP"), TEXT("OCT"), TEXT("NOV"), TEXT("DEC") };

FText GetTimeStringByTextKey(DescriptionCategory inCategoryType)
{
	FText textFind;
	switch (inCategoryType)
	{
	case DescriptionCategory::SEC:
		FText::FindText(TEXT("SystemText"), TEXT("Seconds"), textFind);
		break;
	case DescriptionCategory::MIN:
		FText::FindText(TEXT("SystemText"), TEXT("Minutes"), textFind);
		break;
	case DescriptionCategory::HOUR:
		FText::FindText(TEXT("SystemText"), TEXT("Hours"), textFind);
		break;
	case DescriptionCategory::DAY:
		FText::FindText(TEXT("SystemText"), TEXT("Days"), textFind);
		break;
	case DescriptionCategory::MONTH:
		FText::FindText(TEXT("SystemText"), TEXT("Months"), textFind);
		break;
	case DescriptionCategory::WEEKDAY:
		FText::FindText(TEXT("SystemText"), TEXT("Weekday"), textFind);
		break;
	case DescriptionCategory::MAX:
	default:
		break;
	}

	return textFind;
}

TPair<int, int> MakeRange(const FString& inField, const int inMinVal, const int inMaxVal)
{
	int first = 0;
	int last = 0;
	if (inField.Len() == 1 && inField[0] == TEXT('*'))
	{
		first = inMinVal;
		last = inMaxVal;
	}
	else if (!inField.Contains(TEXT("-")))
	{
		first = static_cast<int>(FCString::Atoi(*inField));
		last = first;
	}
	else
	{
		TArray<FString> parts;
		inField.ParseIntoArray(parts, TEXT("-"));
		if (parts.Num() != 2)
		{
			GSLOG(Log, TEXT("[CronException] Specified range requires two fields"));
		}

		first = static_cast<int>(FCString::Atoi(*parts[0]));
		last = static_cast<int>(FCString::Atoi(*parts[1]));
	}

	if (first > inMaxVal || last > inMaxVal)
	{
		GSLOG(Log, TEXT("[CronException] Specified range exceeds maximum"));
	}
	if (first < inMinVal || last < inMinVal)
	{
		GSLOG(Log, TEXT("[CronException] Specified range is less than minimum"));
	}
	if (first > last)
	{
		GSLOG(Log, TEXT("[CronException] Specified range start exceeds range end"));
	}

	return TPair<int, int>(first, last);
}

FText Conv(DescriptionCategory inCategoryType, int inValue, bool inUseEveryText = true)
{
	FText textFind;
	if (inCategoryType == DescriptionCategory::WEEKDAY)
	{
		switch (inValue)
		{
		case 0: // 일 6
			FText::FindText(TEXT("SystemText"), (inUseEveryText == true)?TEXT("Sun"): TEXT("SUN"), textFind);
			return textFind;
		case 1: // 월 0
			FText::FindText(TEXT("SystemText"), (inUseEveryText == true) ? TEXT("Mon"): TEXT("MON"), textFind);
			return textFind;
		case 2: // 화 1
			FText::FindText(TEXT("SystemText"), (inUseEveryText == true) ? TEXT("Tue"): TEXT("TUE"), textFind);
			return textFind;
		case 3: // 수 2
			FText::FindText(TEXT("SystemText"), (inUseEveryText == true) ? TEXT("Wed"): TEXT("WED"), textFind);
			return textFind;
		case 4: // 목 3
			FText::FindText(TEXT("SystemText"), (inUseEveryText == true) ? TEXT("Thu"): TEXT("THU"), textFind);
			return textFind;
		case 5: // 금 4
			FText::FindText(TEXT("SystemText"), (inUseEveryText == true) ? TEXT("Fri"): TEXT("FRI"), textFind);
			return textFind;
		case 6: // 토 5
			FText::FindText(TEXT("SystemText"), (inUseEveryText == true) ? TEXT("Sat"): TEXT("SAT"), textFind);
			return textFind;
		}
	}

	return FText::FromString(FString::FromInt(inValue));
}

FString ReplaceOrdinals(FString text, const TArray<FString>& replacement)
{
	for (size_t i = 0; i < replacement.Num(); ++i)
	{
		const int32 pos = text.Find(replacement[i]);
		if (INDEX_NONE != pos)
		{
			text.RemoveAt(pos, 3);
			text.InsertAt(pos, FString::FromInt(i));
		}
	}

	return text;
}

const Description* SetField(const FString& value, const uint8 minval, const uint8 maxval)
{
	if (value.Len() > 0 && value[value.Len() - 1] == ',')
		return new DescriptionError();

	TArray<FString> fields;
	
	value.ParseIntoArray(fields, TEXT(","));
	if (0 == fields.Num())
	{
		return new DescriptionError();
	}
	else if (1 == fields.Num())
	{
		FString& field = fields[0];

		if (field.Contains(TEXT("*")))
		{
			return new DescriptionAll();
		}
		else if (field.Contains(TEXT("/")))
		{
			TArray<FString> parts;
			field.ParseIntoArray(parts, TEXT("/"));
			if (parts.Num() != 2)
				return new DescriptionError();

			auto start = FCString::Atoi(*parts[0]);
			auto interval = FCString::Atoi(*parts[1]);
			if ((start <= 0) || (interval <= 0))
				return new DescriptionError();
			else
				return new DescriptionRepeat(start, interval);
		}
		else
		{
			auto range = MakeRange(field, minval, maxval);
			auto from = range.Key;
			auto to = range.Value;

			if ((from < 0) || (to < 0))
				return new DescriptionError();

			if (from == to)
				return new DescriptionOne(from);
			else
				return new DescriptionRange(from, to);
		}
	}
	else
	{
		TArray<uint8> list;
		for (const auto& field : fields)
		{
			auto number = FCString::Atoi(*field);
			if (number < 0)
				return new DescriptionError();
			else
				list.Add(static_cast<uint8>(number));
		}
		return new DescriptionList(MoveTemp(list));
	}

	return nullptr;
}

const Description* SetDaysOfWeek(FString value)
{
	auto days = value.ToUpper();
	auto days_replaced = ReplaceOrdinals(days, CronTrait::DAYS);

	if (days_replaced.Len() == 1 && days_replaced[0] == '?')
		days_replaced[0] = '*';

	return SetField(days_replaced, CronTrait::CRON_MIN_DAYS_OF_WEEK, CronTrait::CRON_MAX_DAYS_OF_WEEK);
}

const Description* SetDaysOfMonth(FString value)
{
	if (value.Len() == 1 && value[0] == '?')
		value[0] = '*';

	return SetField(value, CronTrait::CRON_MIN_DAYS_OF_MONTH, CronTrait::CRON_MAX_DAYS_OF_MONTH);
}

const Description* SetMonth(FString value)
{
	auto month = value.ToUpper();
	auto month_replaced = ReplaceOrdinals(month, CronTrait::MONTHS);

	return SetField(month_replaced, CronTrait::CRON_MIN_MONTHS, CronTrait::CRON_MAX_MONTHS);
}

const Description* SetField(const FString& value, const uint8 minval, const uint8 maxval, int addVal, const FieldAffectType previousFieldAffectType, FieldAffectType& nextFieldAffectType)
{
	if (value.Len() > 0 && value[value.Len() - 1] == ',')
		return new DescriptionError();

	TArray<FString> fields;
	
	value.ParseIntoArray(fields, TEXT(","));
	if (0 == fields.Num())
	{
		return new DescriptionError();
	}
	else if (1 == fields.Num())
	{
		FString& field = fields[0];

		if (field.Contains(TEXT("*")))
		{
			return new DescriptionAll();
		}
		else if (field.Contains(TEXT("/")))
		{
			TArray<FString> parts;
			field.ParseIntoArray(parts, TEXT("/"));
			if (parts.Num() != 2)
				return new DescriptionError();

			auto start = FCString::Atoi(*parts[0]);
			
			if (FieldAffectType::INC == previousFieldAffectType ||
				FieldAffectType::RANGE_INC == previousFieldAffectType)
				addVal++;
			else if (FieldAffectType::DEC == previousFieldAffectType ||
				FieldAffectType::RANGE_DEC == previousFieldAffectType)
				addVal--;


			start += addVal;

			if (start > maxval)
			{
				start = start - maxval + minval;
				nextFieldAffectType = FieldAffectType::INC;
			}
			else if (start < minval)
			{
				start += maxval;
				nextFieldAffectType = FieldAffectType::DEC;
			}

			auto interval = FCString::Atoi(*parts[1]);
			if ((start < 0) || (interval <= 0))
				return new DescriptionError();
			else
				return new DescriptionRepeat(start, interval);
		}
		else
		{
			auto range = MakeRange(field, minval, maxval);
			auto from = range.Key;
			auto to = range.Value;
				

			if (FieldAffectType::INC == previousFieldAffectType)
				addVal++;
			else if (FieldAffectType::DEC == previousFieldAffectType)
				addVal--;


			from += addVal;
			to += addVal;


			if (FieldAffectType::RANGE_INC == previousFieldAffectType)
				from--;
			else if (FieldAffectType::RANGE_DEC == previousFieldAffectType)
				to++;
			
			if (from > maxval)
			{
				from -= (maxval + 1);
				nextFieldAffectType = FieldAffectType::INC;
			}

			if (to > maxval)
			{
				to -= (maxval + 1);
				if (FieldAffectType::NONE == nextFieldAffectType)
					nextFieldAffectType = FieldAffectType::RANGE_INC;
			}

			if (to < 0)
			{
				to += (maxval + 1);
				nextFieldAffectType = FieldAffectType::DEC;
			}

			if (from < 0)
			{
				from += (maxval + 1);
				if (FieldAffectType::NONE == nextFieldAffectType)
					nextFieldAffectType = FieldAffectType::RANGE_DEC;
			}

			if ((from < 0) || (to < 0))
				return new DescriptionError();

			if (from == to)
				return new DescriptionOne(from);
			else
				return new DescriptionRange(from, to);
		}
	}
	else
	{
		TArray<uint8> list;
		int32 index = 0;

		if (FieldAffectType::INC == previousFieldAffectType ||
			FieldAffectType::RANGE_INC == previousFieldAffectType)
			addVal++;
		else if (FieldAffectType::DEC == previousFieldAffectType ||
			FieldAffectType::RANGE_DEC == previousFieldAffectType)
			addVal--;

		for (const auto& field : fields)
		{
			index++;
			auto number = FCString::Atoi(*field);
			if (number < 0)
				return new DescriptionError();
			else
			{

				number += addVal;
				if (number > maxval)
				{
					if (1 == index)
						nextFieldAffectType = FieldAffectType::INC;
					else if (FieldAffectType::NONE == nextFieldAffectType)
						nextFieldAffectType = FieldAffectType::RANGE_INC;

					number -= (maxval + 1);
				}

				if (number < 0)
				{
					if (index == fields.Num())
						nextFieldAffectType = FieldAffectType::DEC;
					else if (FieldAffectType::NONE == nextFieldAffectType)
						nextFieldAffectType = FieldAffectType::RANGE_INC;

					number += (maxval + 1);
				}


				list.Add(static_cast<uint8>(number));
			}
		}
		return new DescriptionList(MoveTemp(list));
	}

	return nullptr;
}

const Description* SetDaysOfWeek(FString value, const FieldAffectType previousFieldAffectType, FieldAffectType& nextFieldAffectType)
{
	auto days = value.ToUpper();
	auto days_replaced = ReplaceOrdinals(days, CronTrait::DAYS);

	if (days_replaced.Len() == 1 && days_replaced[0] == '?')
		days_replaced[0] = '*';

	return SetField(days_replaced, CronTrait::CRON_MIN_DAYS_OF_WEEK, CronTrait::CRON_MAX_DAYS_OF_WEEK, 0,previousFieldAffectType, nextFieldAffectType);
}
const Description* SetDaysOfMonth(FString value, const FieldAffectType previousFieldAffectType, FieldAffectType& nextFieldAffectType)
{
	if (value.Len() == 1 && value[0] == '?')
		value[0] = '*';

	return SetField(value, CronTrait::CRON_MIN_DAYS_OF_MONTH,  CronTrait::CRON_MAX_DAYS_OF_MONTH, 0,previousFieldAffectType, nextFieldAffectType);
}

const Description* SetMonth(FString value, const FieldAffectType previousFieldAffectType, FieldAffectType& nextFieldAffectType)
{
	auto month = value.ToUpper();
	auto month_replaced = ReplaceOrdinals(month, CronTrait::MONTHS);

	return SetField(month_replaced, CronTrait::CRON_MIN_MONTHS, CronTrait::CRON_MAX_MONTHS, 0, previousFieldAffectType, nextFieldAffectType);
}

void WriteOne(DescriptionCategory inCategoryType, const DescriptionOne* inDescriptions, OUT FString& outValue, bool useEveryText = true)
{
	if (nullptr == inDescriptions)
		return;

		// 일요일 표시가 안되어서 요일도 0이하체크 빼기
	if (inCategoryType != DescriptionCategory::HOUR &&
		inCategoryType != DescriptionCategory::WEEKDAY && 0 >= inDescriptions->mNumber)
		return;

	FText textUnitStr = GetTimeStringByTextKey(inCategoryType);

	FText textFind;
	if (FText::FindText(TEXT("SystemText"), TEXT("DescriptionOne"), textFind))
	{
		outValue += FText::Format(textFind, Conv(inCategoryType, inDescriptions->mNumber, useEveryText), textUnitStr).ToString();
	}
}

void WriteAll(DescriptionCategory inCategoryType, const DescriptionAll* inDescriptions, OUT FString& outValue)
{
	FText textUnitStr = GetTimeStringByTextKey(inCategoryType);

	FText textFind;
	if (FText::FindText(TEXT("SystemText"), TEXT("DescriptionAll"), textFind))
	{
		outValue += FText::Format(textFind, textUnitStr).ToString();
	}
}

void WriteRange(DescriptionCategory inCategoryType, const DescriptionRange* inDescriptions, OUT FString& outValue)
{
	FText textUnitStr = GetTimeStringByTextKey(inCategoryType);

	FText textFind;
	if (FText::FindText(TEXT("SystemText"), TEXT("DescriptionRange"), textFind))
	{
		outValue += FText::Format(textFind, Conv(inCategoryType, inDescriptions->mFrom), textUnitStr,
			Conv(inCategoryType, inDescriptions->mTo), textUnitStr).ToString();
	}
}

void WriteList(DescriptionCategory inCategoryType, const DescriptionList* inDescriptions, OUT FString& outValue, bool useEveryText = true)
{
	FText textUnitStr = GetTimeStringByTextKey(inCategoryType);

	bool first = true;
	for (auto v : inDescriptions->mList)
	{
		if (first) first = false;
		else outValue += TEXT(", ");

		FText textFind;
		if (FText::FindText(TEXT("SystemText"), TEXT("DescriptionOne"), textFind))
		{
			outValue += FText::Format(textFind, Conv(inCategoryType, v, useEveryText), textUnitStr).ToString();
		}
	}
}

void WriteRepeat(DescriptionCategory inCategoryType, const DescriptionRepeat* inDescriptions, OUT FString& outValue)
{
	FText textUnitStr = GetTimeStringByTextKey(inCategoryType);

	FText textFind;
	if (FText::FindText(TEXT("SystemText"), TEXT("DescriptionRepeat"), textFind))
	{
		outValue += FText::Format(textFind, Conv(inCategoryType, inDescriptions->mStart), textUnitStr,
			Conv(inCategoryType, inDescriptions->mInterval), textUnitStr).ToString();
	}
}

void Write(DescriptionCategory inCategoryType, const Description* inDescriptions, OUT FString& outValue, bool useEveryText = true)
{
	switch (inDescriptions->mType)
	{
	case DescriptionType::ONE:
		WriteOne(inCategoryType, static_cast<const DescriptionOne*>(inDescriptions), outValue, useEveryText);
		break;
	case DescriptionType::ALL:
		WriteAll(inCategoryType, static_cast<const DescriptionAll*>(inDescriptions), outValue);
		break;
	case DescriptionType::RANGE:
		WriteRange(inCategoryType, static_cast<const DescriptionRange*>(inDescriptions), outValue);
		break;
	case DescriptionType::LIST:
		WriteList(inCategoryType, static_cast<const DescriptionList*>(inDescriptions), outValue, useEveryText);
		break;
	case DescriptionType::REPEAT:
		WriteRepeat(inCategoryType, static_cast<const DescriptionRepeat*>(inDescriptions), outValue);
		break;
	default:
		break;
	}
	
	if (outValue.IsEmpty())
	{
		return;
	}

	if(DescriptionType::DESCRIPTION_TYPE_ERROR != inDescriptions->mType)
		outValue += TEXT(" ");
}


int GetClientUTCHours()
{
	FTimespan UTCOffset = FDateTime::Now() - FDateTime::UtcNow();
	return UTCOffset.GetHours();
}

int GetClientUTCMinutes()
{
	FTimespan UTCOffset = FDateTime::Now() - FDateTime::UtcNow();
	return UTCOffset.GetMinutes();
}

void GetValue(DescriptionCategory inCategoryType, const Description* inDescriptions, OUT FString& outValue)
{
	switch (inDescriptions->mType)
	{
	case DescriptionType::ONE:
		WriteOne(inCategoryType, static_cast<const DescriptionOne*>(inDescriptions), outValue);
		break;
	case DescriptionType::ALL:
		WriteAll(inCategoryType, static_cast<const DescriptionAll*>(inDescriptions), outValue);
		break;
	case DescriptionType::RANGE:
		WriteRange(inCategoryType, static_cast<const DescriptionRange*>(inDescriptions), outValue);
		break;
	case DescriptionType::LIST:
		WriteList(inCategoryType, static_cast<const DescriptionList*>(inDescriptions), outValue);
		break;
	case DescriptionType::REPEAT:
		WriteRepeat(inCategoryType, static_cast<const DescriptionRepeat*>(inDescriptions), outValue);
		break;
	default:
		break;
	}

	if (DescriptionType::DESCRIPTION_TYPE_ERROR != inDescriptions->mType)
		outValue += TEXT(" ");
}

//void FGsCronUtil::GetCronString(const FString& inCronText, FString& outString)
//{
//	if (inCronText.IsEmpty())
//		return;
//
//	TArray<FString> fields;
//	inCronText.ParseIntoArray(fields, TEXT(" "));
//
//	if (6 != fields.Num())
//		return;
//
//	const Description* mDescriptions[static_cast<size_t>(DescriptionCategory::MAX)];
//		
//	mDescriptions[static_cast<size_t>(DescriptionCategory::SEC)] = SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)] = SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)] = SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)] = SetDaysOfMonth(fields[3]);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::MONTH)] = SetMonth(fields[4]);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)] = SetDaysOfWeek(fields[5]);	
//
//	TFunction<bool(const DescriptionType inDescriptionType)> func = [](const DescriptionType descriptionType)
//	{		
//		return ((DescriptionType::ONE == descriptionType) ||
//			(DescriptionType::RANGE == descriptionType) ||
//			(DescriptionType::LIST == descriptionType) ||
//			(DescriptionType::REPEAT == descriptionType));
//	};
//
//	bool meaningful[static_cast<size_t>(DescriptionCategory::MAX)];
//	for (size_t i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
//	{
//		const DescriptionType descriptionType = mDescriptions[i]->mType;
//
//		meaningful[i] = func(descriptionType);
//	}
//
//	if (meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)] ||
//		meaningful[static_cast<size_t>(DescriptionCategory::DAY)] ||
//		meaningful[static_cast<size_t>(DescriptionCategory::HOUR)])
//	{
//		if (meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)])
//			Write(DescriptionCategory::WEEKDAY, mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)], outString);
//		else
//		{
//			if (mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)]->mType == DescriptionType::ALL)
//			{
//				Write(DescriptionCategory::DAY, mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)], outString);
//			}
//			else
//			{
//				Write(DescriptionCategory::MONTH, mDescriptions[static_cast<size_t>(DescriptionCategory::MONTH)], outString);
//				Write(DescriptionCategory::DAY, mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)], outString);
//			}
//		}
//	}
//
//	if (meaningful[static_cast<size_t>(DescriptionCategory::HOUR)] || meaningful[static_cast<size_t>(DescriptionCategory::MIN)])
//	{
//		Write(DescriptionCategory::HOUR, mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)], outString);
//	}
//
//	if (meaningful[static_cast<size_t>(DescriptionCategory::MIN)] || meaningful[static_cast<size_t>(DescriptionCategory::SEC)])
//	{
//		Write(DescriptionCategory::MIN, mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)], outString);
//	}
//
//	Write(DescriptionCategory::SEC, mDescriptions[static_cast<size_t>(DescriptionCategory::SEC)], outString);
//
//	// 메모리 해제
//	for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
//	{
//		delete mDescriptions[i];
//		mDescriptions[i] = nullptr;
//	}
//}

void FGsCronUtil::GetCronString(const FString& inCronText, FString& outSecond, FString& outMinuts, FString& outHour, FString& outDay, FString& outWeek)
{
	if (inCronText.IsEmpty())
		return;

	TArray<FString> fields;
	inCronText.ParseIntoArray(fields, TEXT(" "));

	if (6 != fields.Num())
		return;

	const Description* mDescriptions[static_cast<size_t>(DescriptionCategory::MAX)];

	mDescriptions[static_cast<size_t>(DescriptionCategory::SEC)] = SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS);
	mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)] = SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES);
	mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)] = SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS);
	mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)] = SetDaysOfMonth(fields[3]);
	mDescriptions[static_cast<size_t>(DescriptionCategory::MONTH)] = SetMonth(fields[4]);
	mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)] = SetDaysOfWeek(fields[5]);

	TFunction<bool(const DescriptionType inDescriptionType)> func = [](const DescriptionType descriptionType)
	{
		return ((DescriptionType::ONE == descriptionType) ||
			(DescriptionType::RANGE == descriptionType) ||
			(DescriptionType::LIST == descriptionType) ||
			(DescriptionType::REPEAT == descriptionType));
	};

	bool meaningful[static_cast<size_t>(DescriptionCategory::MAX)];
	for (size_t i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
	{
		const DescriptionType descriptionType = mDescriptions[i]->mType;

		meaningful[i] = func(descriptionType);
	}

	if (meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)] ||
		meaningful[static_cast<size_t>(DescriptionCategory::DAY)] ||
		meaningful[static_cast<size_t>(DescriptionCategory::HOUR)])
	{
		if (meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)])
			Write(DescriptionCategory::WEEKDAY, mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)], outWeek);
		else
			Write(DescriptionCategory::DAY, mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)], outDay);
	}

	if (meaningful[static_cast<size_t>(DescriptionCategory::HOUR)] || meaningful[static_cast<size_t>(DescriptionCategory::MIN)])
	{
		Write(DescriptionCategory::HOUR, mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)], outHour);
	}

	if (meaningful[static_cast<size_t>(DescriptionCategory::MIN)] || meaningful[static_cast<size_t>(DescriptionCategory::SEC)])
	{
		Write(DescriptionCategory::MIN, mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)], outMinuts);
	}

	Write(DescriptionCategory::SEC, mDescriptions[static_cast<size_t>(DescriptionCategory::SEC)], outSecond);

	// 메모리 해제
	for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
	{
		delete mDescriptions[i];
		mDescriptions[i] = nullptr;
	}
}
//
//void FGsCronUtil::GetWeek(const FString& inCronText, FString& outString)
//{
//	if (inCronText.IsEmpty())
//		return;
//
//	TArray<FString> fields;
//	inCronText.ParseIntoArray(fields, TEXT(" "));
//
//	if (6 != fields.Num())
//		return;
//
//	const Description* mDescriptions[static_cast<size_t>(DescriptionCategory::MAX)];
//
//	mDescriptions[static_cast<size_t>(DescriptionCategory::SEC)] = SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)] = SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)] = SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)] = SetDaysOfMonth(fields[3]);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::MONTH)] = SetMonth(fields[4]);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)] = SetDaysOfWeek(fields[5]);
//
//	TFunction<bool(const DescriptionType inDescriptionType)> func = [](const DescriptionType descriptionType)
//	{
//		return ((DescriptionType::ONE == descriptionType) ||
//			(DescriptionType::RANGE == descriptionType) ||
//			(DescriptionType::LIST == descriptionType) ||
//			(DescriptionType::REPEAT == descriptionType));
//	};
//
//	bool meaningful[static_cast<size_t>(DescriptionCategory::MAX)];
//	for (size_t i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
//	{
//		const DescriptionType descriptionType = mDescriptions[i]->mType;
//
//		meaningful[i] = func(descriptionType);
//	}
//
//	if (false == meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)])
//	{			
//		FText textFind;
//		FString text;
//		int32 num = CronTrait::DAYS.Num();
//		int32 last = CronTrait::DAYS.Num() - 1;
//		for (int32 i = 0; i < num; ++i)
//		{
//			FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, FTextKey(CronTrait::DAYS[i]), textFind);
//			outString += textFind.ToString();
//
//			if (i != last)
//			{
//				outString += "/";
//			}
//		}
//
//		// 메모리 해제
//		for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
//		{
//			delete mDescriptions[i];
//			mDescriptions[i] = nullptr;
//		}
//		return;
//	}
//
//	if (meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)] ||
//		meaningful[static_cast<size_t>(DescriptionCategory::DAY)] ||
//		meaningful[static_cast<size_t>(DescriptionCategory::HOUR)])
//	{
//		const Description* desription = static_cast<const Description*>(mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)]);
//		if (desription)
//		{
//			switch (desription->mType)
//			{
//			case DescriptionType::ONE:
//			{
//				const DescriptionOne* desriptionOne = static_cast<const DescriptionOne*>(desription);				
//				outString = Conv(DescriptionCategory::WEEKDAY, desriptionOne->mNumber).ToString();
//			}				
//				break;
//			case DescriptionType::ALL:
//			{
//				const DescriptionAll* desriptionAll = static_cast<const DescriptionAll*>(desription);
//				FText textFind;
//				FString text;				
//				int32 num = CronTrait::DAYS.Num();
//				int32 last = CronTrait::DAYS.Num() - 1;
//				for(int32 i  = 0; i < num; ++i)
//				{
//					FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, FTextKey(CronTrait::DAYS[i]), textFind);
//
//					outString += textFind.ToString();
//
//					if (i != last)
//					{
//						outString += "/";
//					}										
//				}				
//			}				
//				break;
//			case DescriptionType::RANGE:
//			{
//				const DescriptionRange* desriptionRange = static_cast<const DescriptionRange*>(desription);
//				FText textFind;
//				FString text;																
//				int32 week;
//
//				if (desriptionRange->mTo >= CronTrait::DAYS.Num())
//				{
//					GSLOG(Error, TEXT("WeekDate is over than 7"));
//					// 메모리 해제
//					for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
//					{
//						delete mDescriptions[i];
//						mDescriptions[i] = nullptr;
//					}
//					return;
//				}
//
//				for (int32 i = desriptionRange->mFrom; i <= desriptionRange->mTo; ++i)
//				{
//					week = i;
//					FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, FTextKey(CronTrait::DAYS[week]), textFind);
//					outString += textFind.ToString();
//
//					if (week != desriptionRange->mTo)
//					{
//						outString += "/";
//					}
//				}
//			}				
//				break;
//			case DescriptionType::LIST:
//			{
//				const DescriptionList* desriptionList = static_cast<const DescriptionList*>(desription);				
//				FText textFind;
//				FString text;
//				int32 num = desriptionList->mList.Num();
//				int32 last = num - 1;
//				for (int32 i = 0; i < num; ++i)
//				{
//					FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, FTextKey(CronTrait::DAYS[desriptionList->mList[i]]), textFind);
//
//					outString += textFind.ToString();
//
//					if (i != last)
//					{
//						outString += "/";
//					}
//				}
//			}			
//				break;
//				/*case DescriptionType::REPEAT:
//				{
//					const DescriptionRepeat* desriptionRepeat = static_cast<const DescriptionRepeat*>(desription);
//
//					FText textFind;
//					FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, FTextKey(CronTrait::DAYS[desriptionRepeat->mStart]), textFind);
//
//					outString += textFind.ToString();
//				}
//					break;*/
//			default:
//				break;
//			}
//		}
//	}
//
//	// 메모리 해제
//	for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
//	{
//		delete mDescriptions[i];
//		mDescriptions[i] = nullptr;
//	}
//}

//void FGsCronUtil::GetHour(const FString& inCronText, FString& outString)
//{
//	if (inCronText.IsEmpty())
//		return;
//
//	TArray<FString> fields;
//	inCronText.ParseIntoArray(fields, TEXT(" "));
//
//	if (6 != fields.Num())
//		return;
//
//	const Description* mDescriptions[static_cast<size_t>(DescriptionCategory::MAX)];
//
//	mDescriptions[static_cast<size_t>(DescriptionCategory::SEC)] = SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)] = SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)] = SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)] = SetDaysOfMonth(fields[3]);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::MONTH)] = SetMonth(fields[4]);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)] = SetDaysOfWeek(fields[5]);
//
//	TFunction<bool(const DescriptionType inDescriptionType)> func = [](const DescriptionType descriptionType)
//	{
//		return ((DescriptionType::ONE == descriptionType) ||
//			(DescriptionType::RANGE == descriptionType) ||
//			(DescriptionType::LIST == descriptionType) ||
//			(DescriptionType::REPEAT == descriptionType));
//	};
//
//	bool meaningful[static_cast<size_t>(DescriptionCategory::MAX)];
//	for (size_t i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
//	{
//		const DescriptionType descriptionType = mDescriptions[i]->mType;
//
//		meaningful[i] = func(descriptionType);
//	}
//
//	TArray<int32> hourArray;
//	DescriptionType hourType = DescriptionType::MAX;
//	TArray<int32> minutArray;
//	DescriptionType minutType = DescriptionType::MAX;
//
//	if (meaningful[static_cast<size_t>(DescriptionCategory::HOUR)] || meaningful[static_cast<size_t>(DescriptionCategory::MIN)])
//	{
//		const Description* decription = mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)];
//		hourType = decription->mType;
//
//		switch (decription->mType)
//		{
//		case DescriptionType::ONE:
//		{
//			const DescriptionOne* descriptionOne = static_cast<const DescriptionOne*>(decription);	
//			if (descriptionOne)
//			{
//				hourArray.Add(descriptionOne->mNumber);
//			}
//			
//			break;
//		}			
//		case DescriptionType::ALL:
//		{
//			for (int32 i = 0; i < 24; ++i)
//			{
//				hourArray.Add(i);
//			}
//
//			break;
//		}
//
//		/*case DescriptionType::ALL:
//		{
//			const DescriptionAll* descriptionAll = static_cast<const DescriptionAll*>(decription);
//
//			break;
//		}
//		case DescriptionType::RANGE:
//		{
//			const DescriptionRange* descriptionRange = static_cast<const DescriptionRange*>(decription);
//
//			break;
//		}	*/
//		case DescriptionType::LIST:
//		{
//			const DescriptionList* descriptionList = static_cast<const DescriptionList*>(decription);
//			if (descriptionList)
//			{
//				for (uint8 hour : descriptionList->mList)
//				{
//					hourArray.Add(hour);
//				}				
//			}
//			break;
//		}					
//		/*	case DescriptionType::REPEAT:
//			{
//				const DescriptionRepeat* descriptionRepeat = static_cast<const DescriptionRepeat*>(decription);
//				break;
//			}
//				break;*/
//		default:
//			break;
//		}		
//	}
//
//	if (meaningful[static_cast<size_t>(DescriptionCategory::MIN)] || meaningful[static_cast<size_t>(DescriptionCategory::SEC)])
//	{
//		const Description* decription = mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)];
//		minutType = decription->mType;
//
//		switch (decription->mType)
//		{
//		case DescriptionType::ONE:
//		{
//			const DescriptionOne* descriptionOne = static_cast<const DescriptionOne*>(decription);
//			if (descriptionOne)
//			{				
//				minutArray.Add(descriptionOne->mNumber);
//			}
//			break;
//		}
//	/*	case DescriptionType::ALL:
//		{
//			const DescriptionAll* descriptionAll = static_cast<const DescriptionAll*>(decription);
//			break;
//		}
//		case DescriptionType::RANGE:
//		{
//			const DescriptionRange* descriptionAll = static_cast<const DescriptionRange*>(decription);
//			break;
//		}*/
//		case DescriptionType::LIST:
//		{
//			const DescriptionList* descriptionList = static_cast<const DescriptionList*>(decription);
//			if (descriptionList)
//			{
//				for (uint8 minut : descriptionList->mList)
//				{
//					minutArray.Add(minut);
//				}
//			}
//			break;
//		}
//		/*case DescriptionType::REPEAT:
//		{
//			const DescriptionRepeat* descriptionRepeat = static_cast<const DescriptionRepeat*>(decription);
//			break;
//		}
//		break;*/
//		default:
//			break;
//		}
//	}	
//
//	FText hmFormat;	
//	FText hourEveryText;
//	FText hourSharpText;
//	FText timeMinuteN;
//
//	FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, HM_TEXT_KEY, hmFormat);
//	FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, HOUR_EVERY_TEXT_KEY, hourEveryText);
//	FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, HOUR_SHARP_TEXT_KEY, hourSharpText);
//	FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, TIME_MINUTE_N_TEXT_KEY, timeMinuteN);
//
//	if (hourType == DescriptionType::ALL
//		&& minutType == DescriptionType::ONE)
//	{
//		FString timeString;
//
//		if (0 == minutArray[0])
//		{
//			timeString = hourEveryText.ToString() + " " + hourSharpText.ToString();
//		}
//		else
//		{
//			timeString = hourEveryText.ToString() + " " + FText::Format(timeMinuteN, UKismetTextLibrary::Conv_IntToText(minutArray[0], false, true, 2)).ToString();
//		}
//	
//		outString = timeString;
//	}
//
//	if (hourType == DescriptionType::ONE
//		&& minutType == DescriptionType::MAX)
//	{
//		outString = FText::Format(hmFormat, hourArray[0], DOUBLE_ZERO).ToString();
//	}
//
//	if (hourType == DescriptionType::ONE
//		&& minutType == DescriptionType::ONE)
//	{
//		outString = FText::Format(hmFormat, hourArray[0], UKismetTextLibrary::Conv_IntToText(minutArray[0], false, true, 2)).ToString();
//	}
//
//	if (hourType == DescriptionType::ONE
//		&& minutType == DescriptionType::LIST)
//	{
//		int32 num = minutArray.Num();
//		int32 last = num - 1;
//		for (int32 i = 0; i < minutArray.Num(); ++i)
//		{
//			outString += FText::Format(hmFormat, hourArray[0], UKismetTextLibrary::Conv_IntToText(minutArray[i], false, true, 2)).ToString();
//			if (i != last)
//			{
//				outString += TEXT(" & ");
//			}
//		}
//	}	
//
//	if (hourType == DescriptionType::LIST
//		&& minutType == DescriptionType::MAX)
//	{
//		int32 num = hourArray.Num();
//		int32 last = num - 1;
//		for (int32 i = 0; i < num; ++i)
//		{
//			outString += FText::Format(hmFormat, hourArray[i], DOUBLE_ZERO).ToString();
//			if (i != last)
//			{
//				outString += TEXT(" & ");
//			}
//		}
//	}
//
//	/*if (hourType == DescriptionType::LIST
//		&& minutType == DescriptionType::ONE)
//	{
//		int32 hourNum = hourArray.Num();
//		int32 hourLast = hourNum - 1;
//		int32 minutNum = minutArray.Num();
//		int32 minutLast = minutNum - 1;
//		for (int32 i = 0; i < hourNum; ++i)
//		{
//			for (int32 j = 0; j < minutNum; ++j)
//			{
//				outString += FText::Format(hmFormat, hourArray[i], UKismetTextLibrary::Conv_IntToText(minutArray[j], false, true, 2)).ToString();
//
//				if (i == hourLast && j == minutLast)
//				{
//					break;
//				}
//
//				outString += TEXT(" & ");
//			}
//		}
//	}*/
//
//	if ((hourType == DescriptionType::ALL && minutType == DescriptionType::LIST)
//	|| (hourType == DescriptionType::LIST && minutType == DescriptionType::ONE)
//	|| (hourType == DescriptionType::LIST && minutType == DescriptionType::LIST))
//	{
//		int32 hourNum = hourArray.Num();
//		int32 hourLast = hourNum - 1;
//		int32 minutNum = minutArray.Num();
//		int32 minutLast = minutNum - 1;
//		for (int32 i = 0; i < hourNum; ++i)
//		{
//			for (int32 j = 0; j < minutNum; ++j)
//			{
//				outString += FText::Format(hmFormat, hourArray[i], UKismetTextLibrary::Conv_IntToText(minutArray[j], false, true, 2)).ToString();
//
//				if (i == hourLast && j == minutLast)
//				{
//					break;
//				}
//
//				outString += TEXT(" & ");
//			}							
//		}
//	}
//
//	// 메모리 해제
//	for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
//	{
//		delete mDescriptions[i];
//		mDescriptions[i] = nullptr;
//	}
//}

//FTimespan FGsCronUtil::GetTimespan(const FString& inCronText)
//{
//	if (inCronText.IsEmpty())
//	{
//		return FTimespan::MinValue();
//	}
//
//	TArray<FString> fields;
//	inCronText.ParseIntoArray(fields, TEXT(" "));
//
//	if (6 != fields.Num())
//	{
//		return FTimespan::MinValue();
//	}
//
//	// 초 분 시 3개만 본다
//	const Description* desc[3];
//	desc[0] = SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS);
//	desc[1] = SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES);
//	desc[2] = SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS);
//
//	bool bIsValid = true;
//	int32 value[3];
//	for (int32 i = 0; i < 3; ++i)
//	{
//		if (DescriptionType::ONE != desc[i]->mType)
//		{
//			bIsValid = false;
//			break;
//		}
//
//		const DescriptionOne* descOne = static_cast<const DescriptionOne*>(desc[i]);
//		value[i] = descOne->mNumber;
//	}
//
//	// 메모리 해제
//	for (int32 i = 0; i < 3; ++i)
//	{
//		delete desc[i];
//		desc[i] = nullptr;
//	}
//	  
//	return (bIsValid) ? FTimespan(value[2], value[1], value[0]) : FTimespan::MinValue();
//}


//FDateTime FGsCronUtil::GetValidDateTime(const FString& inCronText, FDateTime baseTime)
//{
//	if (inCronText.IsEmpty())
//	{
//		return FDateTime::MinValue();
//	}
//
//	TArray<FString> fields;
//	inCronText.ParseIntoArray(fields, TEXT(" "));
//
//	if (6 != fields.Num())
//		return FDateTime::MinValue();
//
//	//DescriptionCategory Key로 indexing 할 때 GetTypeHash(enumType) 에 모호성이 생긴다
//	//어떤 헤더의 문제인지는 모르겠음. key를 size_t로 바꾼다.
//
//	TMap<DescriptionCategory, TSharedPtr<Description>> descriptions = {
//		TPair<DescriptionCategory, TSharedPtr<Description>>{DescriptionCategory::SEC, nullptr},
//		TPair<DescriptionCategory, TSharedPtr<Description>>{DescriptionCategory::MIN, nullptr},
//		TPair<DescriptionCategory, TSharedPtr<Description>>{DescriptionCategory::HOUR, nullptr},
//		TPair<DescriptionCategory, TSharedPtr<Description>>{DescriptionCategory::DAY, nullptr},
//		TPair<DescriptionCategory, TSharedPtr<Description>>{DescriptionCategory::MONTH, nullptr},
//		TPair<DescriptionCategory, TSharedPtr<Description>>{DescriptionCategory::WEEKDAY, nullptr}, //year
//	};
//
//	descriptions[DescriptionCategory::SEC] = MakeShareable<Description>(const_cast<Description*>(SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS)));
//	descriptions[DescriptionCategory::MIN] = MakeShareable<Description>(const_cast<Description*>(SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES)));
//	descriptions[DescriptionCategory::HOUR] = MakeShareable<Description>(const_cast<Description*>(SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS)));
//	descriptions[DescriptionCategory::DAY] = MakeShareable<Description>(const_cast<Description*>(SetDaysOfMonth(fields[3])));
//	descriptions[DescriptionCategory::MONTH] = MakeShareable<Description>(const_cast<Description*>(SetMonth(fields[4])));
//	descriptions[DescriptionCategory::WEEKDAY] = MakeShareable<Description>(const_cast<Description*>(SetDaysOfWeek(fields[5])));
//
//
//	TArray<size_t> date; date.AddZeroed(static_cast<size_t>(DescriptionCategory::MAX));
//
//	int32 min = baseTime.GetMinute();
//	int32 hour = baseTime.GetHour();
//	int32 day = baseTime.GetDay();
//	int32 month = baseTime.GetMonth();
//	int32 dayOfWeek = (int32)(baseTime.GetDayOfWeek()) + 1;
//
//	using cmpKey = TPair<int32, int32>;
//	using triple = TPair<DescriptionCategory, cmpKey>;
//
//	for (auto e : { triple{DescriptionCategory::SEC,		cmpKey{0, 0}},
//					triple{DescriptionCategory::MIN,		cmpKey{min, min}},
//					triple{DescriptionCategory::HOUR,		cmpKey{hour, hour}},
//					triple{DescriptionCategory::DAY,		cmpKey{day, day}},
//					triple{DescriptionCategory::MONTH,		cmpKey{month, month}},
//					triple{DescriptionCategory::WEEKDAY,	cmpKey{dayOfWeek, day}} })
//	{
//		auto cmpVal = e.Value.Key;
//		auto setVal = e.Value.Value;
//		if (descriptions[e.Key] && descriptions[e.Key]->mType == DescriptionType::ONE)
//		{
//			auto oneVal = date[StaticCast<size_t>(e.Key)] = static_cast<DescriptionOne*>(descriptions[e.Key].Get())->mNumber;
//			if (e.Key == DescriptionCategory::WEEKDAY)
//			{
//				date[StaticCast<size_t>(e.Key)] = setVal + (oneVal - cmpVal);
//			}
//		}
//		else if (descriptions[e.Key]->mType == DescriptionType::ALL)
//		{
//			date[StaticCast<size_t>(e.Key)] = setVal;
//		}
//		else if (descriptions[e.Key]->mType == DescriptionType::LIST)
//		{
//			if (const auto desriptionList = static_cast<const DescriptionList*>(descriptions[e.Key].Get()))
//			{
//				if (desriptionList && desriptionList->mList.Find(cmpVal))
//				{
//					date[StaticCast<size_t>(e.Key)] = setVal;
//				}
//			}
//		}
//		else if (descriptions[e.Key]->mType == DescriptionType::RANGE)
//		{
//			if (const auto desriptionRange = static_cast<const DescriptionRange*>(descriptions[e.Key].Get()))
//			{
//				if (cmpVal <= desriptionRange->mTo && cmpVal >= desriptionRange->mFrom)
//				{
//					date[StaticCast<size_t>(e.Key)] = setVal;
//				}
//			}
//		}
//	}
//
//	//day weekday 불일치시, validation check 이므로 base day와 일치하는 쪽으로 세팅. 아니면 day를 우선
//	auto validDay = date[static_cast<size_t>(DescriptionCategory::DAY)];
//	auto validWeekDay = date[static_cast<size_t>(DescriptionCategory::WEEKDAY)];
//
//	if (validDay != validWeekDay)
//	{
//		date[static_cast<size_t>(DescriptionCategory::DAY)] = 
//			(validWeekDay == day || validDay == day) ? day : date[static_cast<size_t>(DescriptionCategory::DAY)];
//	}
//
//	FDateTime retDateTime(baseTime.GetYear(), 
//		date[static_cast<size_t>(DescriptionCategory::MONTH)],
//		date[static_cast<size_t>(DescriptionCategory::DAY)],
//		date[static_cast<size_t>(DescriptionCategory::HOUR)],
//		date[static_cast<size_t>(DescriptionCategory::MIN)],
//		date[static_cast<size_t>(DescriptionCategory::SEC)]
//	);
//
//	return retDateTime;
//}




//void FGsCronUtil::GetWeekDays(IN const FString& inCronText, OUT TArray<int32>& outDayList)
//{
//	if (inCronText.IsEmpty())
//		return;
//
//	TArray<FString> fields;
//	inCronText.ParseIntoArray(fields, TEXT(" "));
//
//	if (6 != fields.Num())
//		return;
//
//	const Description* mDescriptions[static_cast<size_t>(DescriptionCategory::MAX)];
//
//	mDescriptions[static_cast<size_t>(DescriptionCategory::SEC)] = SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)] = SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)] = SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)] = SetDaysOfMonth(fields[3]);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::MONTH)] = SetMonth(fields[4]);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)] = SetDaysOfWeek(fields[5]);
//
//	TFunction<bool(const DescriptionType inDescriptionType)> func = [](const DescriptionType descriptionType)
//	{
//		return ((DescriptionType::ONE == descriptionType) ||
//			(DescriptionType::RANGE == descriptionType) ||
//			(DescriptionType::LIST == descriptionType) ||
//			(DescriptionType::REPEAT == descriptionType));
//	};
//
//	bool meaningful[static_cast<size_t>(DescriptionCategory::MAX)];
//	for (size_t i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
//	{
//		const DescriptionType descriptionType = mDescriptions[i]->mType;
//
//		meaningful[i] = func(descriptionType);
//	}
//
//	if (false == meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)])
//	{
//		int32 num = CronTrait::DAYS.Num();
//		for (int32 i = 0; i < num; ++i)
//		{
//			outDayList.Emplace(i);
//		}
//
//		// 메모리 해제
//		for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
//		{
//			delete mDescriptions[i];
//			mDescriptions[i] = nullptr;
//		}
//		return;
//	}
//
//	if (meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)] ||
//		meaningful[static_cast<size_t>(DescriptionCategory::DAY)] ||
//		meaningful[static_cast<size_t>(DescriptionCategory::HOUR)])
//	{
//		const Description* desription = static_cast<const Description*>(mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)]);
//		if (desription)
//		{
//			switch (desription->mType)
//			{
//			case DescriptionType::ONE:
//			{
//				const DescriptionOne* desriptionOne = static_cast<const DescriptionOne*>(desription);
//				outDayList.Emplace(desriptionOne->mNumber);
//			}
//			break;
//			case DescriptionType::ALL:
//			{
//				const DescriptionAll* desriptionAll = static_cast<const DescriptionAll*>(desription);				
//				int32 num = CronTrait::DAYS.Num();
//				for (int32 i = 0; i < num; ++i)
//				{
//					outDayList.Emplace(i);
//				}
//			}
//			break;
//			case DescriptionType::RANGE:
//			{
//				const DescriptionRange* desriptionRange = static_cast<const DescriptionRange*>(desription);
//				FText textFind;
//				FString text;
//
//				if (desriptionRange->mTo >= CronTrait::DAYS.Num())
//				{
//					GSLOG(Error, TEXT("WeekDate is over than 7"));
//					// 메모리 해제
//					for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
//					{
//						delete mDescriptions[i];
//						mDescriptions[i] = nullptr;
//					}
//					return;
//				}
//
//				for (int32 i = desriptionRange->mFrom; i <= desriptionRange->mTo; ++i)
//				{
//					outDayList.Emplace(i);
//				}
//			}
//			break;
//			case DescriptionType::LIST:
//			{
//				const DescriptionList* desriptionList = static_cast<const DescriptionList*>(desription);				
//				int32 num = desriptionList->mList.Num();				
//				for (int32 i = 0; i < num; ++i)
//				{		
//					outDayList.Emplace(desriptionList->mList[i]);
//				}
//			}
//			break;
//			default:
//				break;
//			}
//		}
//	}
//
//	// 메모리 해제
//	for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
//	{
//		delete mDescriptions[i];
//		mDescriptions[i] = nullptr;
//	}
//}

//void FGsCronUtil::GetDays(IN const FString& inCronText, OUT TArray<int32>& outDayList)
//{
//	if (inCronText.IsEmpty())
//		return;
//
//	TArray<FString> fields;
//	inCronText.ParseIntoArray(fields, TEXT(" "));
//
//	if (6 != fields.Num())
//		return;
//
//	const Description* mDescriptions[static_cast<size_t>(DescriptionCategory::MAX)];
//
//	mDescriptions[static_cast<size_t>(DescriptionCategory::SEC)] = SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)] = SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)] = SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)] = SetDaysOfMonth(fields[3]);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::MONTH)] = SetMonth(fields[4]);
//	mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)] = SetDaysOfWeek(fields[5]);
//
//	TFunction<bool(const DescriptionType inDescriptionType)> func = [](const DescriptionType descriptionType)
//	{
//		return ((DescriptionType::ONE == descriptionType) ||
//			(DescriptionType::RANGE == descriptionType) ||
//			(DescriptionType::LIST == descriptionType) ||
//			(DescriptionType::REPEAT == descriptionType));
//	};
//
//	bool meaningful[static_cast<size_t>(DescriptionCategory::MAX)];
//	for (size_t i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
//	{
//		const DescriptionType descriptionType = mDescriptions[i]->mType;
//
//		meaningful[i] = func(descriptionType);
//	}
//
//	if (!meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)] &&
//		meaningful[static_cast<size_t>(DescriptionCategory::DAY)])
//	{
//		const Description* desription = static_cast<const Description*>(mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)]);
//		if (desription)
//		{
//			switch (desription->mType)
//			{
//			case DescriptionType::ONE:
//			{
//				const DescriptionOne* desriptionOne = static_cast<const DescriptionOne*>(desription);
//				outDayList.Emplace(desriptionOne->mNumber);
//			}
//			break;
//			case DescriptionType::LIST:
//			{
//				const DescriptionList* desriptionList = static_cast<const DescriptionList*>(desription);
//				int32 num = desriptionList->mList.Num();
//				for (int32 i = 0; i < num; ++i)
//				{
//					outDayList.Emplace(desriptionList->mList[i]);
//				}
//			}
//			break;
//			default:
//				break;
//			}
//		}
//	}
//
//	// 메모리 해제
//	for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
//	{
//		delete mDescriptions[i];
//		mDescriptions[i] = nullptr;
//	}
//}


void FGsCronUtil::GetCronString(const FString& inCronText, FString& outString, bool useEveryText)
{
	if (inCronText.IsEmpty())
		return;

	TArray<FString> fields;
	inCronText.ParseIntoArray(fields, TEXT(" "));

	if (6 != fields.Num())
		return;

	int addHours = GetClientUTCHours();
	int addMin = GetClientUTCMinutes();

	FieldAffectType previousFieldAffectType = FieldAffectType::NONE;
	FieldAffectType nextFieldAffectType = FieldAffectType::NONE;
	const Description* mDescriptions[static_cast<size_t>(DescriptionCategory::MAX)];

	mDescriptions[static_cast<size_t>(DescriptionCategory::SEC)] = SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS);

	mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)] = SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES, addMin, previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)] = SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS, addHours, previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)] = SetDaysOfWeek(fields[5], previousFieldAffectType, nextFieldAffectType); //요일은 달 변화하고는 관계 없어 먼저함	
	
	mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)] = SetDaysOfMonth(fields[3], previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::MONTH)] = SetMonth(fields[4], previousFieldAffectType, nextFieldAffectType);


	TFunction<bool(const DescriptionType inDescriptionType)> func = [](const DescriptionType descriptionType)
	{
		return ((DescriptionType::ONE == descriptionType) ||
			(DescriptionType::RANGE == descriptionType) ||
			(DescriptionType::LIST == descriptionType) ||
			(DescriptionType::REPEAT == descriptionType));
	};

	bool meaningful[static_cast<size_t>(DescriptionCategory::MAX)];
	for (size_t i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
	{
		const DescriptionType descriptionType = mDescriptions[i]->mType;

		meaningful[i] = func(descriptionType);
	}

	if (meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)] ||
		meaningful[static_cast<size_t>(DescriptionCategory::DAY)] ||
		meaningful[static_cast<size_t>(DescriptionCategory::HOUR)])
	{
		if (meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)])
			Write(DescriptionCategory::WEEKDAY, mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)], outString, useEveryText);
		else
		{
			if (mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)]->mType == DescriptionType::ALL)
			{
				Write(DescriptionCategory::DAY, mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)], outString);
			}
			else
			{
				Write(DescriptionCategory::MONTH, mDescriptions[static_cast<size_t>(DescriptionCategory::MONTH)], outString);
				Write(DescriptionCategory::DAY, mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)], outString);
			}
		}
	}

	if (meaningful[static_cast<size_t>(DescriptionCategory::HOUR)] || meaningful[static_cast<size_t>(DescriptionCategory::MIN)])
	{
		Write(DescriptionCategory::HOUR, mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)], outString);
	}

	if (meaningful[static_cast<size_t>(DescriptionCategory::MIN)] || meaningful[static_cast<size_t>(DescriptionCategory::SEC)])
	{
		Write(DescriptionCategory::MIN, mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)], outString);
	}

	Write(DescriptionCategory::SEC, mDescriptions[static_cast<size_t>(DescriptionCategory::SEC)], outString);

	// 메모리 해제
	for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
	{
		delete mDescriptions[i];
		mDescriptions[i] = nullptr;
	}
}

void FGsCronUtil::GetCronStrings(const FString& inCronText, int durationMin, TArray<FString>& outTimes)
{
	if (inCronText.IsEmpty())
		return;

	TArray<FString> fields;
	inCronText.ParseIntoArray(fields, TEXT(" "));

	if (6 != fields.Num())
		return;

	int addHours = GetClientUTCHours();
	int addMin = GetClientUTCMinutes();

	FieldAffectType previousFieldAffectType = FieldAffectType::NONE;
	FieldAffectType nextFieldAffectType = FieldAffectType::NONE;
	const Description* mDescriptions[static_cast<size_t>(DescriptionCategory::MAX)];

	mDescriptions[static_cast<size_t>(DescriptionCategory::SEC)] = SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS);

	mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)] = SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES, addMin, previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)] = SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS, addHours, previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)] = SetDaysOfWeek(fields[5], previousFieldAffectType, nextFieldAffectType); //요일은 달 변화하고는 관계 없어 먼저함
	mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)] = SetDaysOfMonth(fields[3], previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::MONTH)] = SetMonth(fields[4], previousFieldAffectType, nextFieldAffectType);


	TFunction<bool(const DescriptionType inDescriptionType)> func = [](const DescriptionType descriptionType)
	{
		return ((DescriptionType::ONE == descriptionType) ||
				(DescriptionType::RANGE == descriptionType) ||
				(DescriptionType::LIST == descriptionType) ||
				(DescriptionType::REPEAT == descriptionType));
	};

	bool meaningful[static_cast<size_t>(DescriptionCategory::MAX)];
	for (size_t i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
	{
		const DescriptionType descriptionType = mDescriptions[i]->mType;

		meaningful[i] = func(descriptionType);
	}

	FString dayString;
	if (meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)])
	{
		if (mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)]->mType == DescriptionType::LIST)
		{
			static TArray<FString> weekText = { TEXT("SUN"), TEXT("MON"), TEXT("TUE"), TEXT("WED"), TEXT("THU"), TEXT("FRI"), TEXT("SAT") };

			TArray<uint8> weekNums;
			weekNums.Append(static_cast<const DescriptionList*>(mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)])->mList);
			weekNums.Sort();

			bool first = true;
			for (auto& e : weekNums)
			{
				if (dayString.IsEmpty()) dayString = Conv(DescriptionCategory::WEEKDAY, e).ToString();
				else
				{
					dayString += TEXT(",");

					FText textFind;
					if (FText::FindText(TEXT("SystemText"), weekText[e], textFind))
						dayString += textFind.ToString();
				}
			}
			if (dayString.IsEmpty() == false)
			{
				dayString += TEXT(" ");
				dayString += GetTimeStringByTextKey(DescriptionCategory::WEEKDAY).ToString();
			}
		}
		else
		{
			Write(DescriptionCategory::WEEKDAY, mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)], dayString);
		}
	}
	else
	{
		if (meaningful[static_cast<size_t>(DescriptionCategory::DAY)] || meaningful[static_cast<size_t>(DescriptionCategory::MONTH)])
			 Write(DescriptionCategory::MONTH, mDescriptions[static_cast<size_t>(DescriptionCategory::MONTH)], dayString);
		Write(DescriptionCategory::DAY, mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)], dayString);
	}

	FString endDayString;

	FText formatText;
	FText::FindText(TEXT("SystemText"), TEXT("DescriptionOne"), formatText);

	TArray<FString> startTimes;
	TArray<FString> endTimes;

	if (meaningful[static_cast<size_t>(DescriptionCategory::HOUR)] || meaningful[static_cast<size_t>(DescriptionCategory::MIN)])
	{
		using CatPairArray = TPair<DescriptionCategory, TArray<uint8>>;

		TArray<CatPairArray> useCategory = {
			CatPairArray{ DescriptionCategory::HOUR, TArray<uint8>() },
			CatPairArray{ DescriptionCategory::MIN, TArray<uint8>() } };

		for (auto& pair : useCategory)
		{
			auto category = pair.Key;
			auto& units = pair.Value;

			if (mDescriptions[static_cast<size_t>(category)]->mType == DescriptionType::ONE)
			{
				units.Add(static_cast<const DescriptionOne*>(mDescriptions[static_cast<size_t>(category)])->mNumber);
			}
			else if (mDescriptions[static_cast<size_t>(category)]->mType == DescriptionType::LIST)
			{
				units.Append(static_cast<const DescriptionList*>(mDescriptions[static_cast<size_t>(category)])->mList);
			}
			else if (mDescriptions[static_cast<size_t>(category)]->mType == DescriptionType::RANGE)
			{
				Write(category, mDescriptions[static_cast<size_t>(category)], dayString); // 시간이면 분처리 안함
				break;
			}
			else if (mDescriptions[static_cast<size_t>(category)]->mType == DescriptionType::REPEAT)
			{
				Write(category, mDescriptions[static_cast<size_t>(category)], dayString); // 시간이면 분처리 안함
				break;
			}
			else
			{
				Write(category, mDescriptions[static_cast<size_t>(category)], dayString);
			}
			if(meaningful[static_cast<size_t>(DescriptionCategory::MIN)] == false)
				break;
		}

		FText textUnitHourStr = GetTimeStringByTextKey(DescriptionCategory::HOUR);
		FText textUnitMinStr = GetTimeStringByTextKey(DescriptionCategory::MIN);

		FText HourMinForm;
		FText::FindText(TEXT("SystemText"), TEXT("HourMinForm"), HourMinForm);

		FTimespan duration(0, durationMin, 0);
		auto durHour = duration.GetHours();
		auto durMin = duration.GetMinutes();

		if (useCategory[0].Value.Num() > 0)
		{
			for (auto unit : (useCategory[0].Value))
			{
				durMin = duration.GetMinutes();

				FString startHourString = FText::Format(formatText, Conv(DescriptionCategory::HOUR, unit), textUnitHourStr).ToString();
				FString endHourString = FText::Format(formatText, Conv(DescriptionCategory::HOUR, unit + durHour), textUnitHourStr).ToString();

				if (useCategory[1].Value.Num() > 0)
				{
					for (auto min : (useCategory[1].Value))
					{
						FTimespan checkDuration = duration + FTimespan(0, min, 0);
						durMin = checkDuration.GetMinutes();

						FString startMinString = min > 0 ? FString::Format(*HourMinForm.ToString(), { startHourString,  FText::Format(formatText, Conv(DescriptionCategory::MIN, min), textUnitMinStr).ToString() }) : startHourString;
						FString endMinString = (durMin > 0) ? endHourString + FText::Format(formatText, Conv(DescriptionCategory::MIN, durMin), textUnitMinStr).ToString() : endHourString;

						startTimes.Add(startMinString);
						endTimes.Add(endMinString);
					}
				}
				else
				{
					endHourString = (durMin > 0) ? endHourString + FText::Format(formatText, Conv(DescriptionCategory::MIN, durMin), textUnitMinStr).ToString() : endHourString;

					startTimes.Add(startHourString);
					endTimes.Add(endHourString);
				}
			}
		}
		else
		{
			durMin = duration.GetTotalMinutes();
			if (useCategory[1].Value.Num() > 0)
			{
				for (auto unit : (useCategory[1].Value))
				{
					FString startMinString = FText::Format(formatText, Conv(DescriptionCategory::MIN, unit), textUnitMinStr).ToString();
					FString endMinString = FText::Format(formatText, Conv(DescriptionCategory::MIN, unit + durMin), textUnitMinStr).ToString();

					startTimes.Add(startMinString);
					endTimes.Add(endMinString);
				}
			}
			else
			{
				FText RepeatRange;
				FText::FindText(TEXT("SystemText"), TEXT("RepeatRange"), RepeatRange);

				FText endHourText = durHour > 0 ? FText::Format(formatText, Conv(DescriptionCategory::HOUR, durHour), textUnitHourStr) : FText::GetEmpty();
				endHourText = (durMin > 0) ? FText::Join(endHourText, FText::Format(formatText, Conv(DescriptionCategory::MIN, durMin), textUnitMinStr)) : FText::GetEmpty();

				endTimes.Add(FText::Format(RepeatRange, endHourText).ToString());
			}
		}
	}

	if (startTimes.Num() <= 0 && endTimes.IsValidIndex(0))
	{
		outTimes.Add(FString::Format(TEXT("{0} {1}"), { dayString, endTimes[0] }));
		endTimes.Reset();
	}
	else if (startTimes.Num() == 1 && endTimes.IsValidIndex(0))
	{
		outTimes.Add(FString::Format(TEXT("{0} {1} ~ {2}"), { dayString, startTimes[0], endTimes[0] }));
		startTimes.Reset();
		endTimes.Reset();
	}
	else
	{
		outTimes.Add(dayString);
	}

	auto startTimeIter = startTimes.CreateIterator();
	auto endTimeIter = endTimes.CreateIterator();
	for (; startTimeIter && endTimeIter; ++startTimeIter, ++endTimeIter)
	{
		if(durationMin > 0)
			 outTimes.Add(FString::Format(TEXT("{0} ~ {1}"), { (*startTimeIter),(*endTimeIter) }));
		else outTimes.Add(*startTimeIter);
	}

	// 메모리 해제
	for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
	{
		delete mDescriptions[i];
		mDescriptions[i] = nullptr;
	}
}

void FGsCronUtil::GetWeek(const FString& inCronText, FString& outString)
{
	if (inCronText.IsEmpty())
		return;

	TArray<FString> fields;
	inCronText.ParseIntoArray(fields, TEXT(" "));

	if (6 != fields.Num())
		return;

	int addHours = GetClientUTCHours();
	int addMin = GetClientUTCMinutes();

	FieldAffectType previousFieldAffectType = FieldAffectType::NONE;
	FieldAffectType nextFieldAffectType = FieldAffectType::NONE;
	const Description* mDescriptions[static_cast<size_t>(DescriptionCategory::MAX)];
	
	mDescriptions[static_cast<size_t>(DescriptionCategory::SEC)] = SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS);

	mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)] = SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES, addMin, previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)] = SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS, addHours, previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)] = SetDaysOfWeek(fields[5], previousFieldAffectType, nextFieldAffectType); //요일은 달 변화하고는 관계 없어 먼저함
	mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)] = SetDaysOfMonth(fields[3], previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::MONTH)] = SetMonth(fields[4], previousFieldAffectType, nextFieldAffectType);

	TFunction<bool(const DescriptionType inDescriptionType)> func = [](const DescriptionType descriptionType)
	{
		return ((DescriptionType::ONE == descriptionType) ||
			(DescriptionType::RANGE == descriptionType) ||
			(DescriptionType::LIST == descriptionType) ||
			(DescriptionType::REPEAT == descriptionType));
	};

	bool meaningful[static_cast<size_t>(DescriptionCategory::MAX)];
	for (size_t i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
	{
		const DescriptionType descriptionType = mDescriptions[i]->mType;

		meaningful[i] = func(descriptionType);
	}

	if (false == meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)])
	{
		FText textFind;
		FString text;
		int32 num = CronTrait::DAYS.Num();
		int32 last = CronTrait::DAYS.Num() - 1;
		//매일 마다 출력된다면 월/화..일로 출력되지 않고 매일마다 출력되도록 수정 
		//https://jira.com2us.com/jira/browse/C2URWQ-2846
		if (7 == num)
		{			
			FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, FTextKey(TEXT("Daily")), textFind);
			outString += textFind.ToString();
		}
		else
		{
			for (int32 i = 0; i < num; ++i)
			{
				FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, FTextKey(CronTrait::DAYS[i]), textFind);
				outString += textFind.ToString();

				if (i != last)
				{
					outString += "/";
				}
			}
		}	

		// 메모리 해제
		for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
		{
			delete mDescriptions[i];
			mDescriptions[i] = nullptr;
		}
		return;
	}

	if (meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)] ||
		meaningful[static_cast<size_t>(DescriptionCategory::DAY)] ||
		meaningful[static_cast<size_t>(DescriptionCategory::HOUR)])
	{
		const Description* desription = static_cast<const Description*>(mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)]);
		if (desription)
		{
			switch (desription->mType)
			{
			case DescriptionType::ONE:
			{
				const DescriptionOne* desriptionOne = static_cast<const DescriptionOne*>(desription);
				outString = Conv(DescriptionCategory::WEEKDAY, desriptionOne->mNumber).ToString();
			}
			break;
			case DescriptionType::ALL:
			{			
				const DescriptionAll* desriptionAll = static_cast<const DescriptionAll*>(desription);
				FText textFind;
				FString text;
				int32 num = CronTrait::DAYS.Num();
				int32 last = CronTrait::DAYS.Num() - 1;
				for (int32 i = 0; i < num; ++i)
				{
					FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, FTextKey(CronTrait::DAYS[i]), textFind);

					outString += textFind.ToString();

					if (i != last)
					{
						outString += "/";
					}
				}
			}
			break;
			case DescriptionType::RANGE:
			{
				const DescriptionRange* desriptionRange = static_cast<const DescriptionRange*>(desription);
				FText textFind;
				FString text;
				int32 week;

				if (desriptionRange->mTo >= CronTrait::DAYS.Num())
				{
					GSLOG(Error, TEXT("WeekDate is over than 7"));
					// 메모리 해제
					for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
					{
						delete mDescriptions[i];
						mDescriptions[i] = nullptr;
					}
					return;
				}

				for (int32 i = desriptionRange->mFrom; i <= desriptionRange->mTo; ++i)
				{
					week = i;
					FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, FTextKey(CronTrait::DAYS[week]), textFind);
					outString += textFind.ToString();

					if (week != desriptionRange->mTo)
					{
						outString += "/";
					}
				}
			}
			break;
			case DescriptionType::LIST:
			{
				const DescriptionList* desriptionList = static_cast<const DescriptionList*>(desription);
				FText textFind;
				FString text;
				int32 num = desriptionList->mList.Num();
				int32 last = num - 1;
				for (int32 i = 0; i < num; ++i)
				{
					FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, FTextKey(CronTrait::DAYS[desriptionList->mList[i]]), textFind);

					outString += textFind.ToString();

					if (i != last)
					{
						outString += "/";
					}
				}
			}
			break;
			/*case DescriptionType::REPEAT:
			{
				const DescriptionRepeat* desriptionRepeat = static_cast<const DescriptionRepeat*>(desription);

				FText textFind;
				FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, FTextKey(CronTrait::DAYS[desriptionRepeat->mStart]), textFind);

				outString += textFind.ToString();
			}
				break;*/
			default:
				break;
			}
		}
	}

	// 메모리 해제
	for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
	{
		delete mDescriptions[i];
		mDescriptions[i] = nullptr;
	}
}

void FGsCronUtil::GetHour(const FString& inCronText, FString& outString)
{
	if (inCronText.IsEmpty())
		return;

	TArray<FString> fields;
	inCronText.ParseIntoArray(fields, TEXT(" "));

	if (6 != fields.Num())
		return;

	int addHours = GetClientUTCHours();
	int addMin = GetClientUTCMinutes();

	FieldAffectType previousFieldAffectType = FieldAffectType::NONE;
	FieldAffectType nextFieldAffectType = FieldAffectType::NONE;
	const Description* mDescriptions[static_cast<size_t>(DescriptionCategory::MAX)];

	mDescriptions[static_cast<size_t>(DescriptionCategory::SEC)] = SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS);

	mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)] = SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES, addMin, previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)] = SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS, addHours, previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)] = SetDaysOfWeek(fields[5], previousFieldAffectType, nextFieldAffectType); //요일은 달 변화하고는 관계 없어 먼저함
	mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)] = SetDaysOfMonth(fields[3], previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::MONTH)] = SetMonth(fields[4], previousFieldAffectType, nextFieldAffectType);


	TFunction<bool(const DescriptionType inDescriptionType)> func = [](const DescriptionType descriptionType)
	{
		return ((DescriptionType::ONE == descriptionType) ||
			(DescriptionType::RANGE == descriptionType) ||
			(DescriptionType::LIST == descriptionType) ||
			(DescriptionType::REPEAT == descriptionType));
	};

	bool meaningful[static_cast<size_t>(DescriptionCategory::MAX)];
	for (size_t i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
	{
		const DescriptionType descriptionType = mDescriptions[i]->mType;

		meaningful[i] = func(descriptionType);
	}

	TArray<int32> hourArray;
	DescriptionType hourType = DescriptionType::DESCRIPTION_TYPE_MAX;
	TArray<int32> minutArray;
	DescriptionType minutType = DescriptionType::DESCRIPTION_TYPE_MAX;

	if (meaningful[static_cast<size_t>(DescriptionCategory::HOUR)] || meaningful[static_cast<size_t>(DescriptionCategory::MIN)])
	{
		const Description* decription = mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)];
		hourType = decription->mType;

		switch (decription->mType)
		{
		case DescriptionType::ONE:
		{
			const DescriptionOne* descriptionOne = static_cast<const DescriptionOne*>(decription);
			if (descriptionOne)
			{
				hourArray.Add(descriptionOne->mNumber);
			}

			break;
		}
		case DescriptionType::ALL:
		{
			for (int32 i = 0; i < 24; ++i)
			{
				hourArray.Add(i);
			}

			break;
		}

		/*case DescriptionType::ALL:
		{
			const DescriptionAll* descriptionAll = static_cast<const DescriptionAll*>(decription);

			break;
		}
		case DescriptionType::RANGE:
		{
			const DescriptionRange* descriptionRange = static_cast<const DescriptionRange*>(decription);

			break;
		}	*/
		case DescriptionType::LIST:
		{
			const DescriptionList* descriptionList = static_cast<const DescriptionList*>(decription);
			if (descriptionList)
			{
				for (uint8 hour : descriptionList->mList)
				{
					hourArray.Add(hour);
				}
			}
			break;
		}
		/*	case DescriptionType::REPEAT:
			{
				const DescriptionRepeat* descriptionRepeat = static_cast<const DescriptionRepeat*>(decription);
				break;
			}
				break;*/
		default:
			break;
		}
	}

	if (meaningful[static_cast<size_t>(DescriptionCategory::MIN)] || meaningful[static_cast<size_t>(DescriptionCategory::SEC)])
	{
		const Description* decription = mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)];
		minutType = decription->mType;

		switch (decription->mType)
		{
		case DescriptionType::ONE:
		{
			const DescriptionOne* descriptionOne = static_cast<const DescriptionOne*>(decription);
			if (descriptionOne)
			{
				minutArray.Add(descriptionOne->mNumber);
			}
			break;
		}
		/*	case DescriptionType::ALL:
			{
				const DescriptionAll* descriptionAll = static_cast<const DescriptionAll*>(decription);
				break;
			}
			case DescriptionType::RANGE:
			{
				const DescriptionRange* descriptionAll = static_cast<const DescriptionRange*>(decription);
				break;
			}*/
		case DescriptionType::LIST:
		{
			const DescriptionList* descriptionList = static_cast<const DescriptionList*>(decription);
			if (descriptionList)
			{
				for (uint8 minut : descriptionList->mList)
				{
					minutArray.Add(minut);
				}
			}
			break;
		}
		/*case DescriptionType::REPEAT:
		{
			const DescriptionRepeat* descriptionRepeat = static_cast<const DescriptionRepeat*>(decription);
			break;
		}
		break;*/
		default:
			break;
		}
	}

	FText hmFormat;
	FText hourEveryText;
	FText hourSharpText;
	FText timeMinuteN;

	FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, HM_TEXT_KEY, hmFormat);
	FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, HOUR_EVERY_TEXT_KEY, hourEveryText);
	FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, HOUR_SHARP_TEXT_KEY, hourSharpText);
	FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, TIME_MINUTE_N_TEXT_KEY, timeMinuteN);

	if (hourType == DescriptionType::ALL
		&& minutType == DescriptionType::ONE)
	{
		FString timeString;

		if (0 == minutArray[0])
		{
			timeString = hourEveryText.ToString() + " " + hourSharpText.ToString();
		}
		else
		{
			timeString = hourEveryText.ToString() + " " + FText::Format(timeMinuteN, UKismetTextLibrary::Conv_IntToText(minutArray[0], false, true, 2)).ToString();
		}

		outString = timeString;
	}

	if (hourType == DescriptionType::ONE
		&& minutType == DescriptionType::DESCRIPTION_TYPE_MAX)
	{
		outString = FText::Format(hmFormat, hourArray[0], DOUBLE_ZERO).ToString();
	}

	if (hourType == DescriptionType::ONE
		&& minutType == DescriptionType::ONE)
	{
		outString = FText::Format(hmFormat, hourArray[0], UKismetTextLibrary::Conv_IntToText(minutArray[0], false, true, 2)).ToString();
	}

	if (hourType == DescriptionType::ONE
		&& minutType == DescriptionType::LIST)
	{
		int32 num = minutArray.Num();
		int32 last = num - 1;
		for (int32 i = 0; i < minutArray.Num(); ++i)
		{
			outString += FText::Format(hmFormat, hourArray[0], UKismetTextLibrary::Conv_IntToText(minutArray[i], false, true, 2)).ToString();
			if (i != last)
			{
				outString += TEXT(" & ");
			}
		}
	}

	if (hourType == DescriptionType::LIST
		&& minutType == DescriptionType::DESCRIPTION_TYPE_MAX)
	{
		int32 num = hourArray.Num();
		int32 last = num - 1;
		for (int32 i = 0; i < num; ++i)
		{
			outString += FText::Format(hmFormat, hourArray[i], DOUBLE_ZERO).ToString();
			if (i != last)
			{
				outString += TEXT(" & ");
			}
		}
	}

	/*if (hourType == DescriptionType::LIST
		&& minutType == DescriptionType::ONE)
	{
		int32 hourNum = hourArray.Num();
		int32 hourLast = hourNum - 1;
		int32 minutNum = minutArray.Num();
		int32 minutLast = minutNum - 1;
		for (int32 i = 0; i < hourNum; ++i)
		{
			for (int32 j = 0; j < minutNum; ++j)
			{
				outString += FText::Format(hmFormat, hourArray[i], UKismetTextLibrary::Conv_IntToText(minutArray[j], false, true, 2)).ToString();

				if (i == hourLast && j == minutLast)
				{
					break;
				}

				outString += TEXT(" & ");
			}
		}
	}*/

	if ((hourType == DescriptionType::ALL && minutType == DescriptionType::LIST)
		|| (hourType == DescriptionType::LIST && minutType == DescriptionType::ONE)
		|| (hourType == DescriptionType::LIST && minutType == DescriptionType::LIST))
	{
		int32 hourNum = hourArray.Num();
		int32 hourLast = hourNum - 1;
		int32 minutNum = minutArray.Num();
		int32 minutLast = minutNum - 1;
		for (int32 i = 0; i < hourNum; ++i)
		{
			for (int32 j = 0; j < minutNum; ++j)
			{
				outString += FText::Format(hmFormat, hourArray[i], UKismetTextLibrary::Conv_IntToText(minutArray[j], false, true, 2)).ToString();

				if (i == hourLast && j == minutLast)
				{
					break;
				}

				outString += TEXT(" & ");
			}
		}
	}

	// 메모리 해제
	for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
	{
		delete mDescriptions[i];
		mDescriptions[i] = nullptr;
	}
}

FTimespan FGsCronUtil::GetTimespan(const FString& inCronText)
{
	if (inCronText.IsEmpty())
	{
		return FTimespan::MinValue();
	}

	TArray<FString> fields;
	inCronText.ParseIntoArray(fields, TEXT(" "));

	if (6 != fields.Num())
	{
		return FTimespan::MinValue();
	}

	int addHours = GetClientUTCHours();
	int addMin = GetClientUTCMinutes();

	// 초 분 시 3개만 본다
	FieldAffectType previousFieldAffectType = FieldAffectType::NONE;
	FieldAffectType nextFieldAffectType = FieldAffectType::NONE;
	const Description* desc[3];

	desc[0] = SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS);
	desc[1] = SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES, addMin, previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	desc[2] = SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS, addHours, previousFieldAffectType, nextFieldAffectType);
	

	bool bIsValid = true;
	int32 value[3];
	for (int32 i = 0; i < 3; ++i)
	{
		if (DescriptionType::ONE != desc[i]->mType)
		{
			bIsValid = false;
			break;
		}

		const DescriptionOne* descOne = static_cast<const DescriptionOne*>(desc[i]);
		value[i] = descOne->mNumber;
	}

	// 메모리 해제
	for (int32 i = 0; i < 3; ++i)
	{
		delete desc[i];
		desc[i] = nullptr;
	}

	return (bIsValid) ? FTimespan(value[2], value[1], value[0]) : FTimespan::MinValue();
}

FTimespan FGsCronUtil::GetTimespanUTC(const FString& inCronText)
{
	if (inCronText.IsEmpty())
	{
		return FTimespan::MinValue();
	}

	TArray<FString> fields;
	inCronText.ParseIntoArray(fields, TEXT(" "));

	if (6 != fields.Num())
	{
		return FTimespan::MinValue();
	}

	// 초 분 시 3개만 본다
	const Description* desc[3];
	desc[0] = SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS);
	desc[1] = SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES);
	desc[2] = SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS);

	bool bIsValid = true;
	int32 value[3];
	for (int32 i = 0; i < 3; ++i)
	{
		if (DescriptionType::ONE != desc[i]->mType)
		{
			bIsValid = false;
			break;
		}

		const DescriptionOne* descOne = static_cast<const DescriptionOne*>(desc[i]);
		value[i] = descOne->mNumber;
	}

	// 메모리 해제
	for (int32 i = 0; i < 3; ++i)
	{
		delete desc[i];
		desc[i] = nullptr;
	}

	return (bIsValid) ? FTimespan(value[2], value[1], value[0]) : FTimespan::MinValue();
}

void FGsCronUtil::GetWeekDays(IN const FString& inCronText, OUT TArray<int32>& outDayList)
{
	if (inCronText.IsEmpty())
		return;

	TArray<FString> fields;
	inCronText.ParseIntoArray(fields, TEXT(" "));

	if (6 != fields.Num())
		return;

	int addHours = GetClientUTCHours();
	int addMin = GetClientUTCMinutes();

	FieldAffectType previousFieldAffectType = FieldAffectType::NONE;
	FieldAffectType nextFieldAffectType = FieldAffectType::NONE;
	const Description* mDescriptions[static_cast<size_t>(DescriptionCategory::MAX)];

	mDescriptions[static_cast<size_t>(DescriptionCategory::SEC)] = SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS);

	mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)] = SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES, addMin, previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)] = SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS, addHours, previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)] = SetDaysOfWeek(fields[5], previousFieldAffectType, nextFieldAffectType); //요일은 달 변화하고는 관계 없어 먼저함
	mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)] = SetDaysOfMonth(fields[3], previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::MONTH)] = SetMonth(fields[4], previousFieldAffectType, nextFieldAffectType);


	TFunction<bool(const DescriptionType inDescriptionType)> func = [](const DescriptionType descriptionType)
	{
		return ((DescriptionType::ONE == descriptionType) ||
			(DescriptionType::RANGE == descriptionType) ||
			(DescriptionType::LIST == descriptionType) ||
			(DescriptionType::REPEAT == descriptionType));
	};

	bool meaningful[static_cast<size_t>(DescriptionCategory::MAX)];
	for (size_t i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
	{
		const DescriptionType descriptionType = mDescriptions[i]->mType;

		meaningful[i] = func(descriptionType);
	}

	if (false == meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)])
	{
		int32 num = CronTrait::DAYS.Num();
		for (int32 i = 0; i < num; ++i)
		{
			outDayList.Emplace(i);
		}

		// 메모리 해제
		for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
		{
			delete mDescriptions[i];
			mDescriptions[i] = nullptr;
		}
		return;
	}

	if (meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)] ||
		meaningful[static_cast<size_t>(DescriptionCategory::DAY)] ||
		meaningful[static_cast<size_t>(DescriptionCategory::HOUR)])
	{
		const Description* desription = static_cast<const Description*>(mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)]);
		if (desription)
		{
			switch (desription->mType)
			{
			case DescriptionType::ONE:
			{
				const DescriptionOne* desriptionOne = static_cast<const DescriptionOne*>(desription);
				outDayList.Emplace(desriptionOne->mNumber);
			}
			break;
			case DescriptionType::ALL:
			{
				const DescriptionAll* desriptionAll = static_cast<const DescriptionAll*>(desription);
				int32 num = CronTrait::DAYS.Num();
				for (int32 i = 0; i < num; ++i)
				{
					outDayList.Emplace(i);
				}
			}
			break;
			case DescriptionType::RANGE:
			{
				const DescriptionRange* desriptionRange = static_cast<const DescriptionRange*>(desription);
				FText textFind;
				FString text;

				if (desriptionRange->mTo >= CronTrait::DAYS.Num())
				{
					GSLOG(Error, TEXT("WeekDate is over than 7"));
					// 메모리 해제
					for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
					{
						delete mDescriptions[i];
						mDescriptions[i] = nullptr;
					}
					return;
				}

				for (int32 i = desriptionRange->mFrom; i <= desriptionRange->mTo; ++i)
				{
					outDayList.Emplace(i);
				}
			}
			break;
			case DescriptionType::LIST:
			{
				const DescriptionList* desriptionList = static_cast<const DescriptionList*>(desription);
				int32 num = desriptionList->mList.Num();
				for (int32 i = 0; i < num; ++i)
				{
					outDayList.Emplace(desriptionList->mList[i]);
				}
			}
			break;
			default:
				break;
			}
		}
	}

	// 메모리 해제
	for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
	{
		delete mDescriptions[i];
		mDescriptions[i] = nullptr;
	}
}

void FGsCronUtil::GetDays(IN const FString& inCronText, OUT TArray<int32>& outDayList)
{
	if (inCronText.IsEmpty())
		return;

	TArray<FString> fields;
	inCronText.ParseIntoArray(fields, TEXT(" "));

	if (6 != fields.Num())
		return;

	int addHours = GetClientUTCHours();
	int addMin = GetClientUTCMinutes();

	FieldAffectType previousFieldAffectType = FieldAffectType::NONE;
	FieldAffectType nextFieldAffectType = FieldAffectType::NONE;
	const Description* mDescriptions[static_cast<size_t>(DescriptionCategory::MAX)];

	mDescriptions[static_cast<size_t>(DescriptionCategory::SEC)] = SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS);

	mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)] = SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES, addMin, previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)] = SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS, addHours, previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)] = SetDaysOfWeek(fields[5], previousFieldAffectType, nextFieldAffectType); //요일은 달 변화하고는 관계 없어 먼저함
	mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)] = SetDaysOfMonth(fields[3], previousFieldAffectType, nextFieldAffectType);
	previousFieldAffectType = nextFieldAffectType;

	mDescriptions[static_cast<size_t>(DescriptionCategory::MONTH)] = SetMonth(fields[4], previousFieldAffectType, nextFieldAffectType);


	TFunction<bool(const DescriptionType inDescriptionType)> func = [](const DescriptionType descriptionType)
	{
		return ((DescriptionType::ONE == descriptionType) ||
			(DescriptionType::RANGE == descriptionType) ||
			(DescriptionType::LIST == descriptionType) ||
			(DescriptionType::REPEAT == descriptionType));
	};

	bool meaningful[static_cast<size_t>(DescriptionCategory::MAX)];
	for (size_t i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
	{
		const DescriptionType descriptionType = mDescriptions[i]->mType;

		meaningful[i] = func(descriptionType);
	}

	if (!meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)] &&
		meaningful[static_cast<size_t>(DescriptionCategory::DAY)])
	{
		const Description* desription = static_cast<const Description*>(mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)]);
		if (desription)
		{
			switch (desription->mType)
			{
			case DescriptionType::ONE:
			{
				const DescriptionOne* desriptionOne = static_cast<const DescriptionOne*>(desription);
				outDayList.Emplace(desriptionOne->mNumber);
			}
			break;
			case DescriptionType::LIST:
			{
				const DescriptionList* desriptionList = static_cast<const DescriptionList*>(desription);
				int32 num = desriptionList->mList.Num();
				for (int32 i = 0; i < num; ++i)
				{
					outDayList.Emplace(desriptionList->mList[i]);
				}
			}
			break;
			default:
				break;
			}
		}
	}

	// 메모리 해제
	for (int32 i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
	{
		delete mDescriptions[i];
		mDescriptions[i] = nullptr;
	}
}

/*
* 2022-12-08 Geena
* GetValidDateTime(const FString& InCronText, FDateTime InCheckTime, FTimespan checkRange, bool useLocalTime)
* 
* in 시간 - duration < 설정값으로 checkTime - duration(시작 가능 시간)중 가장 가까운 시간을 찾는다.
* 
* 우선 dayOfWeek가 설정되어 있으면 day는 all 로 제한, 그렇지 않으면 day 값을 체크한다
* 12월 31일 23시부터 3시간 등의 day, month, year 가 넘어가는 경우에 대한 체크 및 비교 처리
* hour, day, month : list, one, range 적용
* CheckRange Time이 1 day 이하로 가정. 24시간 이상일 경우는 검증 안함
*/

FDateTime FGsCronUtil::GetValidDateTime(const FString& InCronText, FDateTime InCheckTime, FTimespan checkRange)
{
	//InCheckTime = FDateTime(2023, 11, 25, 0, 0, 0); //test
	if (InCronText.IsEmpty())
	{
		return FDateTime::MinValue();
	}

	TArray<FString> fields;
	InCronText.ParseIntoArray(fields, TEXT(" "));

	if (6 != fields.Num())
	{
		GSLOG(Error, TEXT("Cron field is not 6"));
		return FDateTime::MinValue();
	}	

	TMap<DescriptionCategory, TSharedPtr<Description>> descriptions = {
		TPair<DescriptionCategory, TSharedPtr<Description>>{DescriptionCategory::SEC, nullptr},
		TPair<DescriptionCategory, TSharedPtr<Description>>{DescriptionCategory::MIN, nullptr},
		TPair<DescriptionCategory, TSharedPtr<Description>>{DescriptionCategory::HOUR, nullptr},
		TPair<DescriptionCategory, TSharedPtr<Description>>{DescriptionCategory::DAY, nullptr},
		TPair<DescriptionCategory, TSharedPtr<Description>>{DescriptionCategory::MONTH, nullptr},
		TPair<DescriptionCategory, TSharedPtr<Description>>{DescriptionCategory::WEEKDAY, nullptr}, //year
	};

	FieldAffectType fieldAffectType = FieldAffectType::NONE;
	descriptions[DescriptionCategory::SEC] = MakeShareable<Description>
		(const_cast<Description*>(SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS)));

	descriptions[DescriptionCategory::MIN] = MakeShareable<Description>
		(const_cast<Description*>(SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES, 0, fieldAffectType, fieldAffectType)));

	descriptions[DescriptionCategory::HOUR] = MakeShareable<Description>
		(const_cast<Description*>(SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS, 0, fieldAffectType, fieldAffectType)));

	auto previousFieldAffectType = fieldAffectType;
	descriptions[DescriptionCategory::WEEKDAY] = MakeShareable<Description>
		(const_cast<Description*>(SetDaysOfWeek(fields[5], previousFieldAffectType, fieldAffectType)));

	descriptions[DescriptionCategory::DAY] = MakeShareable<Description>
		(const_cast<Description*>(SetDaysOfMonth(fields[3], previousFieldAffectType, fieldAffectType)));

	descriptions[DescriptionCategory::MONTH] = MakeShareable<Description>
		(const_cast<Description*>(SetMonth(fields[4], fieldAffectType, fieldAffectType)));

	FDateTime resTime = InCheckTime -FTimespan(0, 0, InCheckTime.GetSecond());
	FDateTime beginTime = resTime - checkRange;
	FDateTime selectTime = beginTime;

	while (IsValidTime(descriptions, beginTime, selectTime) <= 0)
	{
		GSLOG(Log, TEXT("Failure Time Compare %s - %s"), *beginTime.ToIso8601(), *selectTime.ToIso8601());
		if (selectTime > beginTime)
		{
			beginTime = selectTime;
		}
		else break;
	}

	GSLOG(Log, TEXT("Time Compare %s - %s"), *InCheckTime.ToIso8601(), *selectTime.ToIso8601());
	return selectTime;
}

int FGsCronUtil::IsValidTime(IN TMap<DescriptionCategory, TSharedPtr<Description>>& descriptions, 
	IN FDateTime inCheckTime, OUT FDateTime& selectTime, IN DescriptionCategory descType /*= DescriptionCategory::MONTH*/)
{
	using cmpPair = TPair<DescriptionCategory, int32>;

	static TMap<DescriptionCategory, int32> limits = { cmpPair{DescriptionCategory::MONTH, 24 * 60 * 60}, cmpPair{DescriptionCategory::WEEKDAY, 24 * 60 * 60},
		cmpPair{DescriptionCategory::DAY, 24 * 60 * 60}, cmpPair{DescriptionCategory::HOUR, 60 * 60}, cmpPair{DescriptionCategory::MIN, 60 } };
	static TMap<DescriptionCategory, int32> maxVals = { cmpPair{DescriptionCategory::MONTH, 12}, cmpPair{DescriptionCategory::WEEKDAY, 7},
		cmpPair{DescriptionCategory::HOUR, 24}, cmpPair{DescriptionCategory::MIN, 60} };

	static TArray<DescriptionCategory> checkCategory = { DescriptionCategory::MONTH, DescriptionCategory::DAY, DescriptionCategory::WEEKDAY, DescriptionCategory::HOUR, DescriptionCategory::MIN };

	static auto GetDaysInDiffMonth = [](int32 fromMonth, int32 toMonth, int32 fromYear) {

		if (fromMonth > toMonth) toMonth += 12;

		int32 days = 0;
		for (; fromMonth < toMonth; fromMonth++)
		{
			if (fromMonth > 12)
				 days += FDateTime::DaysInMonth(fromYear + 1, fromMonth % 12);
			else days += FDateTime::DaysInMonth(fromYear, fromMonth);
		}
		return days;
	};

	static auto GetUseList = [](DescriptionCategory descType, int32 baseValue,
		TMap<DescriptionCategory, TSharedPtr<Description>>& inDescriptions, TArray<uint32>& outList)
		{
			auto max = maxVals[descType];

			if (auto timeDescriptor = inDescriptions[descType])
			{
				if (timeDescriptor->mType == DescriptionType::ONE)
				{
					auto elem = StaticCast<DescriptionOne*>(timeDescriptor.Get())->mNumber;
					outList.Add(elem);
				}
				else if (timeDescriptor->mType == DescriptionType::ALL)
				{
					auto from = baseValue;
					auto to = max;
					for (; from <= to; from++)
						outList.Add(from);
				}
				else if (timeDescriptor->mType == DescriptionType::REPEAT)
				{
					if (const auto descriptionRepeat = static_cast<const DescriptionRepeat*>(timeDescriptor.Get()))
					{
						auto from = descriptionRepeat->mStart;
						auto to = from;

						while (to <= max)
						{
							if (to >= baseValue) outList.Emplace(to);
							to += descriptionRepeat->mInterval;
						}
					}
				}
				else if (timeDescriptor->mType == DescriptionType::LIST)
				{
					if (const auto descriptionList = static_cast<const DescriptionList*>(timeDescriptor.Get()))
					{
						for (auto elem : descriptionList->mList)
						{
							if (elem >= baseValue) outList.Emplace(elem);
						}
					}
				}
				else if (timeDescriptor->mType == DescriptionType::RANGE)
				{
					if (const auto descriptionRange = static_cast<const DescriptionRange*>(timeDescriptor.Get()))
					{
						auto to = descriptionRange->mTo;
						auto from = descriptionRange->mFrom;
						if (to < from) to += max;
						for (; from <= to; from++)
						{
							if (from >= baseValue) outList.Emplace(from);
						}
					}
				}
			}
		};

	int32 year  = selectTime.GetYear();
	int32 month = selectTime.GetMonth();

	auto dayLimit = FDateTime::DaysInMonth(year, month);
	maxVals.Emplace(DescriptionCategory::DAY, dayLimit);

	auto index = checkCategory.IndexOfByKey(descType);

	TMap<DescriptionCategory, int32> offsetList = {
		cmpPair{ DescriptionCategory::MONTH,  (GetDaysInDiffMonth(1, month, year) + selectTime.GetDay() - 1) * limits[DescriptionCategory::DAY] +
				selectTime.GetHour() * limits[DescriptionCategory::HOUR] +
				selectTime.GetMinute() * limits[DescriptionCategory::MIN] + selectTime.GetSecond() },
		cmpPair{ DescriptionCategory::WEEKDAY, (static_cast<int32>(selectTime.GetDayOfWeek()) * limits[DescriptionCategory::DAY] +
				selectTime.GetHour() * limits[DescriptionCategory::HOUR] +
				selectTime.GetMinute() * limits[DescriptionCategory::MIN] + selectTime.GetSecond()) },
		cmpPair{ DescriptionCategory::DAY, (selectTime.GetDay() - 1) * limits[DescriptionCategory::DAY] +
				selectTime.GetHour() * limits[DescriptionCategory::HOUR] +
				selectTime.GetMinute() * limits[DescriptionCategory::MIN] + selectTime.GetSecond() },
		cmpPair{ DescriptionCategory::HOUR, (selectTime.GetHour() * limits[DescriptionCategory::HOUR] +
				selectTime.GetMinute() * limits[DescriptionCategory::MIN] + selectTime.GetSecond()) },
		cmpPair{ DescriptionCategory::MIN, selectTime.GetMinute() * limits[DescriptionCategory::MIN] + selectTime.GetSecond() } };

	auto max = maxVals[descType];
	auto limit = limits[descType];
	auto offset = offsetList[descType];

	FDateTime beginTime = inCheckTime;
	FDateTime baseTime = selectTime - FTimespan(0, 0, offset);
	FDateTime retTime = selectTime;

	TMap<DescriptionCategory, int32> bases = { cmpPair{DescriptionCategory::MONTH, selectTime.GetMonth()},
		cmpPair{DescriptionCategory::WEEKDAY, (static_cast<int32>(selectTime.GetDayOfWeek()) + 1) % 7},
		cmpPair{DescriptionCategory::DAY, selectTime.GetDay()}, cmpPair{DescriptionCategory::HOUR, selectTime.GetHour()},
		cmpPair{DescriptionCategory::MIN, selectTime.GetMinute()} };

	auto base = bases[descType];
	
	TArray<uint32> useList;
	GetUseList(descType, base, descriptions, useList);
	useList.StableSort();

	if (descType == DescriptionCategory::WEEKDAY)
	{
		auto tempMax = maxVals[DescriptionCategory::DAY];

		TArray<uint32> tempList;
		Swap(useList, tempList);
		
		for (auto e : tempList)
		{
			FDateTime tempTIme = baseTime + FTimespan(0, 0, e * limit);
			if (tempTIme.GetDay() <= tempMax) useList.Add(e);
		}
	}

	int findTime = 0;
	bool isRecursion = index != INDEX_NONE && checkCategory.IsValidIndex(index + 1);
	
	//dayOfWeek의 Cron 표현식과 FDateTime의 표현이 서로 다르다
	for (auto e : useList)
	{
		e = descType == DescriptionCategory::MONTH ? GetDaysInDiffMonth(1, e, year) : 
			descType == DescriptionCategory::WEEKDAY ? (e + 6) % 7 : 
			descType == DescriptionCategory::DAY ? e - 1 : e;

		retTime = baseTime + FTimespan(0, 0, e * limit);
		if (isRecursion)
		{
			findTime = IsValidTime(descriptions, beginTime, retTime, checkCategory[index + 1]);
		}
		else
		{
			findTime = retTime > beginTime ? 1 : 0;
		}
		if (findTime == -1) break;
		if (findTime == 1) break;
	}

	if(useList.Num() == 0)
	{
		retTime = baseTime + FTimespan(0, 0, 
			(descType == DescriptionCategory::MONTH ? GetDaysInDiffMonth(1, 1 + max, year) : 
			 descType == DescriptionCategory::WEEKDAY ? (max + 6) % 7 : max) * limit);
		findTime = -1;
	}

	if (findTime == 0)
	{
		retTime = baseTime;
	}
	
	selectTime = retTime;
	return findTime;
};