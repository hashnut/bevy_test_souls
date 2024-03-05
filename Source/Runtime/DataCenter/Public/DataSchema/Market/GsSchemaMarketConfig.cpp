/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Market/GsSchemaMarketConfig.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaMarketConfig::FGsSchemaMarketConfig()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaMarketConfig)
	
	registrationFee = 0;
	minPrice = 0;
	maxPrice = 0;
	maxRegistrationCount = 0;
	minRegistrationWaitingTimeM = 0;
	maxRegistrationWaitingTimeM = 0;
	sellPeriodH = 0;
	maxSettlementCount = 0;
	logStorageDurationD = 0;
	maxBookmarkCount = 0;
	listItemCountPerPage = 0;
	gmMarketWithdrawMailTypeId = 0;
	wdRegistrationFee = 0;
	wdMinPrice = 0;
	wdMaxPrice = 0;
	wdMaxRegistrationCount = 0;
	wdMinRegistrationWaitingTimeM = 0;
	wdMaxRegistrationWaitingTimeM = 0;
	wdSellPeriodH = 0;
	wdMaxSettlementCount = 0;
	wdLogStorageDurationD = 0;
	wdMaxBookmarkCount = 0;
	wdListItemCountPerPage = 0;
	wdMinimumItemGrade = ItemGrade::NONE;
	wdRegistrationFeeItemId.DataTable = nullptr;
	WorldMarketShortcutGrade = ItemGrade::NONE;
}
