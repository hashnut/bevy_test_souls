/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityEffectUtil.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaAbnormalityEffectUtil::FGsSchemaAbnormalityEffectUtil()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaAbnormalityEffectUtil)
	
	SkillNoityId.DataTable = nullptr;
	minHp = 0;
	damageRate = 0;
	abnormalityId.DataTable = nullptr;
	hitType = AbnormalityEffectBuffCountHitType::HIT_SELF_NORMAL;
	skillGrade = AbnormalityEffectBuffCountSkillGrade::NONE;
	rate = 0;
	cooldown = 0.000000f;
	damage = 0;
	damage_reduction_rate = 0;
	hpAbsorptionRate = 0;
	hpLimitAbsorptionRate = 0;
	hpAbsorption = 0;
	hpAbsorptionAffectedByBonusRate = false;
	damageShareRate = 0;
	damageShareApplicationRate = 0;
	damageShareLimitHpRate = 0;
}
