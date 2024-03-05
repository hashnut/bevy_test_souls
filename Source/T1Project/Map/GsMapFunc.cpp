#include "GsMapFunc.h"

#include "UTIL/GsText.h"
#include "UTIL/GsTableUtil.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"

#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"

// 로컬 상태 이상인지(상태 이상이면 포털 못탐)
bool FGsMapFunc::IsLocalAbnormality(CreatureActionType In_actionType, const FTextKey& In_fileName, const FTextKey& In_textName)
{
	// local이  상태이상이면 못보냄
	bool isLocalAbnormality = false;

	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (local->IsCrowdControlLock(In_actionType))
		{
			isLocalAbnormality = true;
		}
	}

	if (isLocalAbnormality == true)
	{
		FText findText;
		FText::FindText(In_fileName, In_textName, findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return true;
	}

	return false;
}

// 로컬 이동 불가능한지
bool FGsMapFunc::IsLocalMoveImpossible()
{
	return IsLocalAbnormality(CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT, TEXT("WorldMapText"), TEXT("ErrorCannotmove"));
}
// warp possible check
bool FGsMapFunc::IsLocalWarpImpossible()
{
	return IsLocalAbnormality(CreatureActionType::CREATURE_ACTION_TYPE_WARP, TEXT("WorldMapText"), TEXT("ErrorCannotmove"));
}
// 로컬 특정 업노말리티 상태인지
bool FGsMapFunc::IsLocalAbnormalityType(AbnormalityEffectType In_type)
{
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return false;
	}
	FGsAbnormalityHandlerBase* abnormality = local->GetAbnormalityHandler();

	if (abnormality == nullptr)
	{
		return false;
	}
	return abnormality->IsAbnormalityEffectType(In_type);
}

// get invasion npc icon type
EGsRegionMapIconType FGsMapFunc::GetInvasionNPCRegionMapIconType(EGsRegionInvadeNPCType In_NPCType)
{
	EGsRegionMapIconType iconType = EGsRegionMapIconType::None;
	switch (In_NPCType)
	{
	case EGsRegionInvadeNPCType::Guardian:
		iconType = EGsRegionMapIconType::InvasionGuardian;
		break;
	case EGsRegionInvadeNPCType::Monster:
		iconType = EGsRegionMapIconType::InvasionMonster;
		break;
	case EGsRegionInvadeNPCType::Wagon:
		iconType = EGsRegionMapIconType::InvasionWagon;
		break;
	}

	return iconType;
}

// get invasion npc icon type(mini map icon)
EGsMapIconType FGsMapFunc::GetInvasionNPCMiniMapIconType(EGsRegionInvadeNPCType In_NPCType)
{
	EGsMapIconType iconType = EGsMapIconType::None;
	switch (In_NPCType)
	{
	case EGsRegionInvadeNPCType::Guardian:
		iconType = EGsMapIconType::InvasionGuardian;
		break;
	case EGsRegionInvadeNPCType::Monster:
		iconType = EGsMapIconType::InvasionMonster;
		break;
	case EGsRegionInvadeNPCType::Wagon:
		iconType = EGsMapIconType::InvasionWagon;
		break;
	}

	return iconType;
}

bool FGsMapFunc::GetDungeonSpotInfo(int In_dungeonId, FGsSchemaSpotInfo& Out_spotInfo, int& Out_mapId)
{
	const FGsSchemaDungeonGroupData* dungeonGroupTbl =
		UGsTableUtil::FindRowById<UGsTableDungeonGroupData, FGsSchemaDungeonGroupData>(In_dungeonId);

	if (dungeonGroupTbl == nullptr)
	{
		return false;
	}
	int mapId = dungeonGroupTbl->UIContinentalMapSpotTable.mapId.GetRow()->mapId;
	Out_mapId = mapId;

	FGsClientSpawnManager* spawnManager = GSClientSpawn();
	if (spawnManager == nullptr)
	{
		return false;
	}

	if (false == spawnManager->TryGetSpot(
		mapId,
		dungeonGroupTbl->UIContinentalMapSpotTable.spotIndex, Out_spotInfo))
	{
		return false;
	}

	return true;
}