/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
FGsSchemaAbnormalitySet::FGsSchemaAbnormalitySet()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaAbnormalitySet)
	
	id = 0;
	name = FName();
	categoryId = 0;
	abnormalityStackType = AbnormalityStackType::NONE;
	groupId = 0;
	stackGroupId = 0;
	priority = 0;
	maxStackCount = 0;
	changeAbnormalityId = 0;
	isBuff = false;
	requireWeaponType = CreatureWeaponType::ALL;
	removeConditionLogout = false;
	removeConditionDeath = false;
	removeConditionRequireWeaponCheck = false;
	removeConditionDespawn = false;
	removeConditionDurationTimeCheck = false;
	removeConditionSystemForceRemovable = false;
	removeConditionUseSkill = false;
	removeConditionUseActiveSkill = false;
	removeConditionSkillEnd = false;
	removeConditionChangeFairy = false;
	removeConditionInTown = false;
	removeAbnormalityId = 0;
	removeConditionChangePreset = false;
	stableConditionOnReturnState = false;
	durationMin = 0.000000f;
	durationMax = 0.000000f;
	useProbabilityDuartion = false;
	tickTime = 0.000000f;
	hideUI = false;
}
