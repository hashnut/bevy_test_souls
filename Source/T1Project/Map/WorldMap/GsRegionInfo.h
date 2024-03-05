#pragma once
#include "CoreMinimal.h"

class FGsRegionMonsterInfo;
class FGsRegionMonsterSameIDInfo;

// 포털 정보
class FGsPortalInfo
{
public:
	FText _portalName;
	int64 _portalSpotId;
	FVector _portalPos;
	bool _isInTown;
};
// 텔레포트 정보
class FGsTeleportInfo
{
public:
	FText _teleportName;
	FVector _teleportPos;
	int64 _teleportSpotId;
};
// 성소 정보(지도용)
class FGsRegionMapSanctumInfo
{
public:
	// 성소 이름
	FText _sanctumName;
	// 성소 테이블 id(FGsSchemaSanctumData)
	int32 _sanctumAreaId;
	// 성소 위치
	FVector _sanctumPos;
	// 접근 거리(지도 이동시 중간에 파고들게 안할려고 npc table 값 참조)
	float _sanctumAcceptanceRange;
};


/*
	지역정보
*/
class FGsRegionInfo
{
public:
	bool _isThereATown;
	// 포털 정보
	TArray<FGsPortalInfo*> _arrayPortalInfo;
	// 침공전 입장용 포털 정보(마을 제외)
	TArray<FGsPortalInfo*> _arrayInvasionEnterPortalInfo;
	TArray<FGsTeleportInfo*> _arrayTeleportInfo;	

	// 몬스터 정보(정렬)
	// 1. 등급(보스 이상만 최상위)
	// 2. 이름순
	TArray<FGsRegionMonsterInfo*> _arrayMonsterInfo;
	// 지역맵용 성소 정보
	TArray<FGsRegionMapSanctumInfo*> _arrayRegionMapSanctumInfo;
	// 몬스터 인포 팝업에서 사용할 몬스터 레벨 리스트 정보
	// 키값: 몬스터 id, 내용: 몬스터 레벨들
	// https://jira.com2us.com/jira/browse/CHR-24415
	TMap<int, FGsRegionMonsterSameIDInfo*> _mapMonsterSameIdInfo;
	// 맵 id
	int _mapId;

public:
	void Finalize();
};