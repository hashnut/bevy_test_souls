/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Spot/GsSchemaSpotData.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaDungeonGroupData::FGsSchemaDungeonGroupData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaDungeonGroupData)
	
	id = 0;
	dungeonCategory = DungeonCategory::NORMAL;
	bindingType = ContentsBindingType::USER;
	listOrder = 0;
	visible = true;
	deadTimeCountCheck = true;
	baseTimeLimit = 0;
	maxAddTimeLimit = 0;
	marginTime = 0;
	chargeCostItemId.DataTable = nullptr;
	resetCycling.DataTable = nullptr;
	dungeonGroupType = EGsDungeonGroupType::Level;
	UIContinentalMapSpotTable.mapId.DataTable = nullptr;
	disableTimeCharge = true;
	dungeonMainRewardId.DataTable = nullptr;
	dungeonKey.DataTable = nullptr;
	unLockContentsId.DataTable = nullptr;
	checkQuest = false;
}
