/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/BattlePass/GsSchemaBattlePassLevelBonusInfo.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaBattlePassLevelBonusInfo::FGsSchemaBattlePassLevelBonusInfo()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaBattlePassLevelBonusInfo)
	
	isPayBonus = true;
	exceedPerExp = 0;
	maxExp = 0;
	bonusReward.DataTable = nullptr;
}
