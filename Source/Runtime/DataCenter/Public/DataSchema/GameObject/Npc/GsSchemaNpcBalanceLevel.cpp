/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcBalanceLevel.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Item/GsStatTypeNameSelector.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsNameSelectorBase.h"
FGsSchemaNpcBalanceLevel::FGsSchemaNpcBalanceLevel()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaNpcBalanceLevel)
	
	level = 0;
	rewardId.DataTable = nullptr;
}
