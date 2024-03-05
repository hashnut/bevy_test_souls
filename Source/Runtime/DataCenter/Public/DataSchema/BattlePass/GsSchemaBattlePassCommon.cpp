/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/BattlePass/GsSchemaBattlePassCommon.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaBattlePassCommon::FGsSchemaBattlePassCommon()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaBattlePassCommon)
	
	id = 0;
	passPeriodStart = FString(TEXT("2022.12.31 00:00:00"));
	passExposureConditionType = BattlePassExposureConditionType::NONE;
	passExposureConditionCountMin = 0;
	passExposureConditionCountMax = 0;
	additionalRewardOpenCurrencyType = CurrencyType::NONE;
	additionalRewardOpenCurrency = 0;
	additionalRewardOpenItemId.DataTable = nullptr;
	additionalRewardOpenItemAmount = 0;
	passLevelGroupId.DataTable = nullptr;
	passMissionGroupId.DataTable = nullptr;
	detail.DataTable = nullptr;
	tabDetailId.DataTable = nullptr;
}
