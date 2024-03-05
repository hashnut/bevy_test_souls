//#include "GsDungeonGuildWar.h"
//#include "Management/ScopeGlobal/GsGameDataManager.h"
//#include "GsTable.h"
//#include "DataSchema/Map/GsSchemaMapData.h"
//#include "DataSchema/Dungeon/GsSchemaDungeonData.h"
//#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
//#include "DataSchema/BattleArena/GsSchemaGuildWarConfigData.h"
//#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
//#include "T1Project.h"
//
//
//FGsDungeonGuildWar::FGsDungeonGuildWar(const struct FGsSchemaDungeonGroupData* InSchemaData)
//	: FGsDungeonGroupBase(InSchemaData)
//{
//
//}
//
//bool FGsDungeonGuildWar::IsDungeonUnLockContentsDelegate(const FGsDungeonData* InTarget, bool InShowTicker /*= false*/)
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
//
//const FGsSchemaGuildWarConfigData* FGsDungeonGuildWar::GetConfigData(const FGsDungeonData* InTarget)
//{
//	const FGsSchemaGuildWarConfigData* outData = nullptr;
//	do
//	{
//		if (nullptr == InTarget || InTarget->GetSchema())
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