/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterElementData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaWorldMapMonsterElementData::FGsSchemaWorldMapMonsterElementData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaWorldMapMonsterElementData)
	
	id.DataTable = nullptr;
	mapId = 0;
	level = 0;
	isOffensive = false;
	rank = CreatureRankType::NONE;
	isInvasion = false;
}
