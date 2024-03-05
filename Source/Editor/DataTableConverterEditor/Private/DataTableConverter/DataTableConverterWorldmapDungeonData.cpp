#include "DataTableConverter/GsDataTableConvertBase.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapDungeonData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonData.h"

#include "Slate/Public/Widgets/Notifications/SNotificationList.h"
#include "Slate/Public/Framework/Notifications/NotificationManager.h"

#define DEF_ERR_POPUP_MESSAGE(Message)\
FNotificationInfo Info(Message);\
Info.ExpireDuration = 10.0f;\
Info.bFireAndForget = true;\
Info.Image = FCoreStyle::Get().GetBrush(TEXT("MessageLog.Warning"));\
FSlateNotificationManager::Get().AddNotification(Info);\
return false

IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterWorldmapDungeonlData)
bool FDataTableConverterWorldmapDungeonlData::Convert()
{
	UGsDataTable* DTGsWorldMapDungeonData = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Map/WorldMap/DTGsWorldMapDungeonData.DTGsWorldMapDungeonData"),
		DTGsWorldMapDungeonData))
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(DTGsWorldMapDungeonData)"));
		DEF_ERR_POPUP_MESSAGE(message);
	}

	// empty table
	DTGsWorldMapDungeonData->EmptyTable();

	const UGsTableDungeonGroupData* dungeonGroupData = Cast<UGsTableDungeonGroupData>(FGsSchemaDungeonGroupData::GetStaticTable());
	TArray<const FGsSchemaDungeonGroupData*> dungeonGroupDataAllRow;
	if (false == dungeonGroupData->GetAllRows(dungeonGroupDataAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableDungeonGroupData empty "));
		return false;
	}
	const UGsTableWorldMapDungeonData* gsTable = Cast<UGsTableWorldMapDungeonData>(FGsSchemaWorldMapDungeonData::GetStaticTable());
	for (int i = 0; i < dungeonGroupDataAllRow.Num(); ++i)
	{
		const FGsSchemaDungeonGroupData* dungeonGroupTable = dungeonGroupDataAllRow[i];
		if (nullptr == dungeonGroupTable)
		{
			continue;
		}

		int dungeonId = dungeonGroupTable->id;

		const FGsSchemaWorldMapDungeonData* findWorldmapDungeonData = nullptr;
		gsTable->FindRowById(dungeonId, findWorldmapDungeonData);

		// already exist
		if (findWorldmapDungeonData != nullptr)
		{
			continue;
		}
		
		FGsSchemaWorldMapDungeonData addInfo;
		addInfo.groupId = dungeonId;

		for (const FGsSchemaDungeonDataRow& dungeonIter : dungeonGroupTable->dungeonList)
		{
			if (dungeonIter.IsNull() == true)
			{
				continue;
			}
			const FGsSchemaDungeonData* dungeonData = dungeonIter.GetRow();
			if (dungeonData && dungeonData->mapId.IsNull() == false)
			{
				const FGsSchemaMapData* mapData = dungeonData->mapId.GetRow();
				if (mapData != nullptr)
				{
					if (false == addInfo.regionMapId.Contains(mapData->id))
					{
						addInfo.regionMapId.Add(mapData->id);
					}
				}
			}
		}
		if (addInfo.regionMapId.Num() != 0)
		{
			addInfo.regionMapId.Sort();
		}


		FString keyString = FString::FromInt(dungeonId);
		FName keyName = FName(*keyString);
		DTGsWorldMapDungeonData->AddRow(keyName, addInfo);
	}

	// save table
	DTGsWorldMapDungeonData->SavePackage();

	return true;
};