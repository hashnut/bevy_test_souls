/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/MultiLevelRank/GsSchemaMultiLevelRank.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaMultiLevelRank::FGsSchemaMultiLevelRank()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaMultiLevelRank)
	
	rank = 1;
	exp = 0;
	costType = CostType::CURRENCY;
	costItemId = 0;
	costItemAmount = 0;
	currencyType = CurrencyType::DIA;
	currencyCost = 0;
	effectType = ItemGrade::NONE;
	rewardId.DataTable = nullptr;
	rewardType = EGsMultiLevelRewardType::NONE;
	linkType = ERankLinkType::SingleOnly;
}
