/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemEquipDetail.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Item/GsStatTypeNameSelector.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsNameSelectorBase.h"
FGsSchemaItemEquipDetail::FGsSchemaItemEquipDetail()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaItemEquipDetail)
	
	id = 0;
	tier = 0;
	modelPath = 0;
	materialType = ItemMaterial::FABRIC;
	weaponType = CreatureWeaponType::ALL;
	safeEnchantLevel = 0;
	enchantShieldId = 0;
	enchantRateId = 0;
	enchantBonusId = 0;
	enchantBalanceId = 0;
	enchantCost = 0;
	refineOptionCostCurrencyType = CurrencyType::NONE;
	refineOptionCost = 0;
	goldRecoveryCost = 0;
	diaRecoveryCost = 0;
	setId = 0;
	itemOptionSkillGroupId = 0;
	equipGroupId = 0;
	itemFusionData.DataTable = nullptr;
}
