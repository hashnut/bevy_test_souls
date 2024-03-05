/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/User/GsSchemaUserReactionInfo.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaUserReactionInfo::FGsSchemaUserReactionInfo()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaUserReactionInfo)
	
	type = EReactionType::NONE;
	sword.DataTable = nullptr;
	dagger.DataTable = nullptr;
	bow.DataTable = nullptr;
	wand.DataTable = nullptr;
	orb.DataTable = nullptr;
	shotgun.DataTable = nullptr;
	hand.DataTable = nullptr;
}
