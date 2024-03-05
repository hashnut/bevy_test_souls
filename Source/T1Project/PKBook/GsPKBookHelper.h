#pragma once

#include "CoreMinimal.h"
#include "../T1Project.h"
#include "../UTIL/GsTimeSyncUtil.h"
#include "../Guild/GsGuildHelper.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"





class T1PROJECT_API FGsPKBookHelper
{
public:
	static bool FindText(const FTextKey& InKey, OUT FText& OutText)
	{
		if (false == FText::FindText(TEXT("PKBookText"), InKey, OutText))
		{
			OutText = FText::FromString(FString::Format(TEXT("UnKnow Message : {0}"), { InKey.GetChars() }));

			return false;
		}
		return true;
	}

	static void TextDate(const time_t& InTime, FText& OutText)
	{
		FDateTime localClientTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(InTime);

		FString textDate;
		FGsTimeStringHelper::GetTimeStringNYMDHM(localClientTime, textDate);

		OutText = FText::FromString(textDate);
	}

	static void TextReason(PKBookResult InResult, FText& OutText)
	{
		OutText = FText::GetEmpty();

		switch (InResult)
		{
		case PKBookResult::KILL:
			FindText(TEXT("Record_Result_Kill"), OutText);
			break;
		case PKBookResult::DIE:
			FindText(TEXT("Record_Result_Death"), OutText);
			break;
		case PKBookResult::ASSIST:
			FindText(TEXT("Record_Result_Assist"), OutText);
			break;
		}
	}

	static void TextMapName(MapId InId, FText& OutText)
	{
		const UGsTableMapData* mapDataTable = Cast<UGsTableMapData>(FGsSchemaMapData::GetStaticTable());
		if (nullptr == mapDataTable)
		{
			GSLOG(Error, TEXT("nullptr == mapDataTable"));
			return;
		}

		const FGsSchemaMapData* findMapData = nullptr;
		if (false == mapDataTable->FindRowById(StaticCast<int32>(InId), findMapData))
		{
			GSLOG(Error, TEXT("Not Find MapData : [%d]"), InId);
			return;
		}

		OutText = findMapData->MapStringId;
	}

	static void TextPeriodTime(const time_t InDate, FText& OutText)
	{
		OutText = FText::GetEmpty();

		FTimespan spawnDate = FTimespan::FromSeconds(FGsTimeSyncUtil::GetServerNowSecond() - InDate);

		double totalMin = spawnDate.GetTotalMinutes();
		double totalHours = spawnDate.GetTotalHours();
		double totalDays = spawnDate.GetTotalDays();

		if (1 > totalHours)
		{	// 2022/09/23 PKT - {0}분 전
			FText format;
			FindText(TEXT("State_DetectionTarget_LogOut_Min"), format);
			// 2022/09/28 PKT - {0}분 전(1분 보다 작을 경우 1분 전으로 표기)
			OutText = FText::Format(format, (1 > totalMin) ? 1 : StaticCast<int32>(totalMin));
		}
		else if ( 1 > totalDays )
		{	// 2022/09/23 PKT - {0}시간 전
			FText format;
			FindText(TEXT("State_DetectionTarget_LogOut_Hour"), format);
			OutText = FText::Format(format, StaticCast<int32>(totalHours));
		}
		else if (365 > totalDays)
		{	// 2022/09/23 PKT - {0}일 전
			FText format;
			FindText(TEXT("State_DetectionTarget_LogOut_Day"), format);
			OutText = FText::Format(format, StaticCast<int32>(totalDays));
		}
		else
		{
			const int32 totalYear = totalDays / 365;
			// 2022/09/23 PKT - {0}년 전
			FText format;
			FindText(TEXT("State_DetectionTarget_LogOut_Year"), format);
			OutText = FText::Format(format, totalYear);
		}
	}

	static bool FindPathGuildIcon(GuildEmblemId InGuildEmblemId, bool InIsSmall, FSoftObjectPath& OutPath)
	{
		const FGsSchemaGuildEmblem* schemaGuildEmblem = FGsGuildHelper::GetGuildEmblem(InGuildEmblemId);
		if (nullptr == schemaGuildEmblem)
		{
			return false;
		}

		if (InIsSmall)
		{
			OutPath = schemaGuildEmblem->iconPathSmall;
		}
		else
		{
			OutPath = schemaGuildEmblem->iconPathBig;
		}

		return true;
	}
};
