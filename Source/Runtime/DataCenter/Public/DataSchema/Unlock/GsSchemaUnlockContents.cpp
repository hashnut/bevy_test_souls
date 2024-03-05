/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Unlock/GsSchemaUnlockContents.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaUnlockContents::FGsSchemaUnlockContents()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaUnlockContents)
	
	id = 0;
	serialNumber = 0;
	contentsType = EGsUnlockContentsType::NONE;
	lockType = UnlockCondition::NONE;
	requireMainStoryId.DataTable = nullptr;
	requireLevel = 0;
	showUnlockTicker = false;
}
