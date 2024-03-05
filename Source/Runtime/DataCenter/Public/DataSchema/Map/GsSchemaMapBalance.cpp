/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapBalance.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaMapBalance::FGsSchemaMapBalance()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaMapBalance)
	
	id = 0;
	EnablePK = false;
	EnablePlaceMem = false;
	passivityIdListIndexEnv = -1;
	canItemDrop = true;
	resurrectionDataIdx.DataTable = nullptr;
	expMapBonusRate = 0;
	goldMapBonusRate = 0;
	npcIdleTimeRate = 1.000000f;
	npcRoamingRangeRate = 1.000000f;
}
