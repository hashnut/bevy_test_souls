#pragma once

#include "Map/GsMapIconType.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/*
	월드맵 스태틱 함수
*/

class FGsWorldMapFunc
{
public:
	// 파티 아이콘 타입 얻어오기
	static bool GetPartyIconType(int64 In_gameId, EGsRegionMapIconType& Out_iconType);
	// 지역 던전맵인가(둘중 하나라도 던전이면 던전)
	// 1. 현재맵
	// 2. 보고 있는 지역맵
	static bool IsRegionDungeon();
	// get visible by region icon type(check server option)
	static bool IsRegionMapIconTypeVisible(EGsRegionMapIconType In_iconType);
	// 필터링 값 체크
	static bool IsRegionMapIconFilteringVisible(EGsRegionMapIconFiltering In_iconFiltering);

	// 몬스터 드랍 켜져있는지(체크할 타입들이 있음)
	static bool IsMonsterDropOn(EGsRegionMapIconType In_iconType);

	static void ClickAutoMoveRegionMap(bool In_isPortal, int In_realSelUseIndex);
	// 위치 즐겨 찾기 자동 이동
	// https://jira.com2us.com/jira/browse/CHR-23756
	static void ClickAutoMoveBookMarkPopup(int In_bookmarkIndex);

	static void ClickTeleportRegionMap(bool In_isPortal, int In_realSelUseIndex, FString In_targetName);

	// 침공전 포탈: 자동 이동
	// 몬스터 드랍 위치: 자동 이동
	// 성소 위치: 자동 이동
	// 공용 함수명으로 변경
	static void ClickAutoMoveRegionMapPortalPos(FVector In_portalPos, float In_useAcceptanceRadius = 0.0f);
	// 침공전 포탈: 워프 
	static void ClickTeleportRegionMapInvasionPortal(int64 In_spotId, FString In_targetName);

	static void ClickTeleportRegionMapMonster(MapId In_mapId, CreatureId In_npcId, SpawnGroupId In_spawnGroupId);
	// 지역맵 열기 가능한지
	// 1. 언락 컨텐츠 체크
	// 2. 불가능한 레벨인지 체크(MapType::INSTANCE_SINGLE || MapType::SINGLE_FIELD || MapType::GUILD_DUNGEON)
	// 3. 관전모드일때 막기
	static bool IsRegionMapOpenPossible();
};