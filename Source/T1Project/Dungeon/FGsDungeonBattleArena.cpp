//#include "FGsDungeonBattleArena.h"
//#include "Management/ScopeGlobal/GsGameDataManager.h"
//#include "GsTable.h"
//#include "DataSchema/Map/GsSchemaMapData.h"
//#include "DataSchema/Dungeon/GsSchemaDungeonData.h"
//#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
//#include "DataSchema/BattleArena/GsSchemaGuildWarConfigData.h"
//#include "DataSchema/BattleArena/GsSchemaBattleArenaSeasonData.h"
//#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
//#include "../UTIL/GsCronUtil.h"
//#include "../UI/UIContent/Helper/GsTimeStringHelper.h"
//#include "T1Project.h"
//
//
//FGsDungeonGroupBattleArena::FGsDungeonGroupBattleArena(const struct FGsSchemaDungeonGroupData* InSchemaData)
//	: FGsDungeonGroupBase(InSchemaData)
//{
//	/**/
//}
//
//bool FGsDungeonGroupBattleArena::IsDungeonUnLockContentsDelegate(const FGsDungeonData* InTarget, bool InShowTicker /*= false*/)
//{
//	if (false == FGsDungeonGroupBase::IsDungeonUnLockContentsDelegate(InTarget, InShowTicker))
//	{	// 2023/2/21 PKT - Group 에서 이미 락에 걸려 있다면 던전 검사는 하지 않는다.
//		return false;
//	}
//
//	// 2022/08/18 PKT - 정예 던전을 제외한 나머지 던전은 각 레벨에 따른 세부 언락 조건이 존재 한다.
//	const FGsSchemaDungeonData* schemaData = InTarget->GetSchema();
//	if (DungeonCategory::ARENA != schemaData->dungeonCategory)
//	{
//		const FGsNetUserData* UserData = GGameData()->GetUserData();
//		if (nullptr == UserData)
//		{
//			GSLOG(Error, TEXT("nullptr == UserData"));
//			return false;
//		}
//		// 2023/2/21 PKT - 조건보다 현재 레벨이 같거나 크다면 락 해제
//		return schemaData->unlockLevel <= UserData->mLevel;
//	}
//
//	return false;
//}
//
//bool FGsDungeonGroupBattleArena::IsFreeSeason(const FGsDungeonData* InTarget) const
//{
//	bool outFlag = false;
//
//	do
//	{
//		const FGsSchemaGuildWarConfigData* schemaConfigData = GetConfigData(InTarget);
//		const FGsSchemaBattleArenaSeasonData* shcemaSeasonDate = (schemaConfigData) ? schemaConfigData->seasonDataId.GetRow() : nullptr;
//		if (nullptr == shcemaSeasonDate)
//		{
//			GSLOG(Error, TEXT("nullptr == shcemaSeasonDate"));
//			break;
//		}
//
//		outFlag = shcemaSeasonDate->beginDate.IsEmpty() || shcemaSeasonDate->endDate.IsEmpty();
//
//	} while (0);
//
//	return outFlag;
//}
//
//int32 FGsDungeonGroupBattleArena::SeasonNum(const FGsDungeonData* InTarget) const
//{
//	int32 outNum = -1;
//
//	do
//	{
//		const FGsSchemaGuildWarConfigData* schemaConfigData = GetConfigData(InTarget);
//		const FGsSchemaBattleArenaSeasonData* shcemaSeasonDate = (schemaConfigData) ? schemaConfigData->seasonDataId.GetRow() : nullptr;
//		if (nullptr == shcemaSeasonDate)
//		{
//			GSLOG(Error, TEXT("nullptr == shcemaSeasonDate"));
//			break;
//		}
//
//		outNum = shcemaSeasonDate->seasonNumber;
//
//	} while (0);
//
//	return outNum;
//}
//
//const FText FGsDungeonGroupBattleArena::SeasonDateText(const FGsDungeonData* InTarget) const
//{
//	FText outText = FText::GetEmpty();
//
//	do 
//	{
//		const FGsSchemaGuildWarConfigData* schemaConfigData = GetConfigData(InTarget);
//		const FGsSchemaBattleArenaSeasonData* shcemaSeasonDate = (schemaConfigData) ? schemaConfigData->seasonDataId.GetRow() : nullptr;
//		if (nullptr == shcemaSeasonDate)
//		{
//			GSLOG(Error, TEXT("nullptr == shcemaSeasonDate"));
//			break;
//		}
//
//		/*FDateTime beginDate;
//		if (false == FDateTime::Parse(shcemaSeasonDate->beginDate, beginDate))
//		{
//			GSLOG(Error, TEXT("false == FDateTime::Parse(_schemaData->beginDate, beginDate)[%s]"), *shcemaSeasonDate->beginDate);
//			break;
//		}*/
//
//		FDateTime endDate;
//		if (false == FDateTime::Parse(shcemaSeasonDate->endDate, endDate))
//		{
//			GSLOG(Error, TEXT("false == FDateTime::Parse_schemaData->endDateTime, UpperDateTimeUtc), endDate:%s"), *shcemaSeasonDate->endDate);
//			break;
//		}
//
//		// 2022/12/28 PKT - 서버 -> 클라 시간
//		//FDateTime severToClientBeginDate = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(beginDate);
//		FDateTime severToClientEndDate = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(endDate);
//
//		//FString beginDateText;
//		//FGsTimeStringHelper::GetTimeStringNYMDHM(severToClientBeginDate, beginDateText);
//
//		FString endDateText;		
//		FGsTimeStringHelper::GetTimeStringNYMDHM(severToClientEndDate, endDateText);
//
//		//outText = FText::FromString(FString::Format(TEXT("{0} ~ {1}"), { *beginDateText, *endDateText }));
//		outText = FText::FromString(endDateText);
//
//	} while (0);
//
//	return outText;
//}
//
//const FText FGsDungeonGroupBattleArena::PlayCycleText(const FGsDungeonData* InTarget) const
//{
//	FText outText = FText::GetEmpty();
//
//	do
//	{
//		const FGsSchemaGuildWarConfigData* schemaConfigData = GetConfigData(InTarget);
//		if (nullptr == schemaConfigData)
//		{
//			GSLOG(Error, TEXT("nullptr == schemaConfigData"));
//			break;
//		}
//
//		FString strCycleDate;
//		FGsCronUtil::GetCronString(schemaConfigData->openingCycling, strCycleDate, true);
//
//		outText = FText::FromString(strCycleDate);
//
//	} while (0);
//	
//	return outText;
//}
//
//int32 FGsDungeonGroupBattleArena::ApplyLimitGuildLevel(const FGsDungeonData* InTarget) const
//{
//	int32 outLevel = 0;
//
//	do
//	{
//		const FGsSchemaGuildWarConfigData* schemaConfigData = GetConfigData(InTarget);
//		if (nullptr == schemaConfigData)
//		{
//			GSLOG(Error, TEXT("nullptr == schemaConfigData"));
//			break;
//		}
//
//		outLevel = schemaConfigData->limitGuildLevel;
//
//	} while (0);
//
//	return outLevel;
//}
//
//int32 FGsDungeonGroupBattleArena::ApplyLimitUserLevel(const class FGsDungeonData* InTarget) const
//{
//	int32 outLevel = 0;
//	if (InTarget)
//	{
//		outLevel = (nullptr != InTarget->GetSchema()) ? InTarget->GetSchema()->unlockLevel : 0;
//	}	
//	return outLevel;
//}
//
//GuildMemberGradeType FGsDungeonGroupBattleArena::ApplyLimitGuildGrade(const FGsDungeonData* InTarget) const
//{
//	const FGsSchemaGuildWarConfigData* schemaConfigData = GetConfigData(InTarget);
//	if (nullptr == schemaConfigData)
//	{
//		GSLOG(Error, TEXT("nullptr == schemaConfigData"));
//		return GuildMemberGradeType::UNKNOWN;
//	}
//
//	return schemaConfigData->limitMinApplyGrade;
//}
//
//const FGsSchemaGuildWarConfigData* FGsDungeonGroupBattleArena::GetConfigData(const FGsDungeonData* InTarget) const
//{
//	const FGsSchemaGuildWarConfigData* outData = nullptr;
//	do
//	{
//		if (nullptr == InTarget || nullptr == InTarget->GetSchema())
//		{
//			break;
//		}
//
//		// 2023/11/17 PKT - 찾고자 하는 Map ( Key 가 됨 )
//		const FGsSchemaMapData* targetMapData = InTarget->GetSchema()->mapId.GetRow();
//		if (nullptr == targetMapData)
//		{
//			break;
//		}
//
//		const UGsTable* table = FGsSchemaGuildWarConfigData::GetStaticTable();
//
//		const TMap<FName, uint8*>* OutRowMap;
//		if (nullptr == table || false == table->GetRowMap(OutRowMap))
//		{
//			GSLOG(Error, TEXT("nullptr == table || false == table->GetRowMap(OutRowMap)"));
//			break;
//		}
//
//		for (const auto& pair : (*OutRowMap))
//		{	// 2022/06/09 PKT - 하나만 적용 시키고 빠져 나간다.
//			const FGsSchemaGuildWarConfigData* findData = reinterpret_cast<const FGsSchemaGuildWarConfigData*>(pair.Value);
//			if (findData && findData->mapDataId.GetRow() && findData->mapDataId.GetRow()->id == targetMapData->id)
//			{
//				outData = findData;
//				break;
//			}
//		}
//
//	} while (0);
//
//	if (nullptr == outData)
//	{
//		GSLOG(Error, TEXT("nullptr == outData"));
//	}
//
//	return outData;
//}