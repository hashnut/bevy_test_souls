#pragma once
#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "Runtime/DataCenter/Public/Map/GsMapIconType.h"

#include "Map/WorldMap/GsWorldMapDefine.h"

#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"


/*
	맵 함수
*/
class FGsMapFunc
{
public:
	// 로컬 상태 이상인지(상태 이상이면 포털 못탐)
	static bool IsLocalAbnormality(CreatureActionType In_actionType,const FTextKey& In_fileName, const FTextKey& In_textName);

	// 로컬 이동 불가능한지
	static bool IsLocalMoveImpossible();

	// warp possible check
	static bool IsLocalWarpImpossible();

	// 로컬 특정 업노말리티 상태인지
	static bool IsLocalAbnormalityType(AbnormalityEffectType In_type);
	// get invasion npc icon type(region map icon)
	static EGsRegionMapIconType GetInvasionNPCRegionMapIconType(EGsRegionInvadeNPCType In_NPCType);
	// get invasion npc icon type(mini map icon)
	static EGsMapIconType GetInvasionNPCMiniMapIconType(EGsRegionInvadeNPCType In_NPCType);
	// 던전 스팟 정보 구하기
	static bool GetDungeonSpotInfo(int In_dungeonId, FGsSchemaSpotInfo& Out_spotInfo, int& Out_mapId);
};