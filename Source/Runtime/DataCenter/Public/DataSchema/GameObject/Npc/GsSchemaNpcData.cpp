/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaNpcData::FGsSchemaNpcData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaNpcData)
	
	id = 0;
	balanceId.DataTable = nullptr;
	fixedLevel = 0;
	moveType = CreatureMoveType::NORMAL;
	iffFactionType = IffFactionType::MONSTER;
	npcFunctionType = NpcFunctionType::NONE;
	npcFactionId = 0;
	commonInfoId.DataTable = nullptr;
	shapeId.DataTable = nullptr;
	scale = 1.000000f;
	balloonMessageId = 0;
	npcAIId = 0;
	interactionRange = 0;
	race = CreatureRaceType::NONE;
	attackType = AttackType::MELEE;
	defenseMaterial = ItemMaterial::FABRIC;
	element = MonsterElement::NONE;
	commonActionListId.DataTable = nullptr;
	reactionListId.DataTable = nullptr;
	spawnActionTime = 0.000000f;
	contributionDistance = 0;
	contributionDelete = false;
	canItemDropOverride = CanItemDrop::DEFAULT;
	rotateSpeed = 0;
	isOffensive = false;
	sightRadius = 0.000000f;
	obsoleted = false;
	questBoardGroupIdList = 0;
	isPalyBalloon = false;
	balloonData = 0;
	grade = CreatureRankType::NONE;
	increaseArcasPoint = 0;
	decreaseArcasPoint = 0;
	pvpAlignmentPoint = 0;
	monsterKnowledgeLevelSetId.DataTable = nullptr;
	polymorpShapeId.DataTable = nullptr;
	polymorpShapeId.RowName = FName(TEXT("PC_변이"));
	spawnMove = 0.000000f;
	attackCognition = 0.000000f;
}
