/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaDungeonData::FGsSchemaDungeonData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaDungeonData)
	
	dungeonCategory = DungeonCategory::NORMAL;
	mapId.DataTable = nullptr;
	indexValue = 0;
	visible = true;
	spotId = 0;
	unlockLevel = 0;
	addNpcLevel = 0;
	levelOfDifficulty = DungeonDifficulty::NONE;
	bossId.DataTable = nullptr;
	bossId.RowName = FName(TEXT("10"));
	timeLimitM = 0;
	isRandomRewardNormal = false;
	isRandomRewardElite = false;
	isRandomRewardNamed = false;
}
