/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Prop/GsSchemaPropData.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaPropData::FGsSchemaPropData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaPropData)
	
	id = 0;
	propType = PropType::QUEST;
	shapeId.DataTable = nullptr;
	interactionType = PropInteractionType::NONE;
	touchInteractionTime = 0.000000f;
	touchInteractionRange = 0.000000f;
	multipleInteractionUserCount = 0;
	spawnMaintainTimeS = 0;
	hitCountMax = 0;
	rewardId.DataTable = nullptr;
	multipleInteractionCountMax = 0;
	despawnInterval = 0.000000f;
	paramValue = 0;
	paramValue2 = 0;
	playEffectOffset = FVector(0.000000, 0.000000, 0.000000);
	playEffectScale = 1.000000f;
	abnormalityId.DataTable = nullptr;
}
