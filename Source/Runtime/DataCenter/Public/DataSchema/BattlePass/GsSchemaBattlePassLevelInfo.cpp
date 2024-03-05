/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/BattlePass/GsSchemaBattlePassLevelInfo.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaBattlePassLevelInfo::FGsSchemaBattlePassLevelInfo()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaBattlePassLevelInfo)
	
	exp = 0;
	levelPurchaseCurrencyType = CurrencyType::NONE;
	levelPurchaseCurrency = 0;
	levelPurchaseItemId.DataTable = nullptr;
	levelPurchaseItemCount = 1;
	levelBasicReward.DataTable = nullptr;
	levelAdditionalReward.DataTable = nullptr;
	displayLevelFlag = false;
}
