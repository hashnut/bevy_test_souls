// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContentDungeon.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "DataSchema/Map/GsSchemaMapTerritorySpaceCrack.h"

/**
 *  시공의 틈새 Manager
 */
class T1PROJECT_API FGsSpaceCrackManager final : public IGsManager
{
private:
	// 키값: gameId
	TMap<MapId, TSharedPtr<class FGsSpaceCrackAlarmInfo>> _mapSpaceCrackAlarmInfo;
	TMap<MapId, TSharedPtr<class FGsSpaceCrackActivationInfo>> _mapSpaceCrackActivationInfo;

	// game object 메세지
	MsgGameObjHandleArray _gameObjectMsg;
	MsgSpceCrackHandleArray _spaceCrackMsg;



public:
	//IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;	

public:
	// 시공의 틈새 인포 추가
	void AddSpaceCrackAlarmInfo(const SpaceCrackAlarm& inAlarmInfo);
	void AddSpaceCrackAlarmInfo(MapId inRegionMapId
		, FenceId inFence
		, bool inAlarm
		, const FVector& inPos);
	// 시공의 틈새 인포 삭제
	void DelSpaceCrackPortalInfo(MapId inRegionMapId);

public:
	void AddSpaceCrackActivationInfo(const SpaceCrackActivation& inActivate);
	void DelSpaceCrackActivationInfo(MapId inMapId);

public:
	void UpdateSpaceCrackActivationInfo(const SpaceCrackActivation& inActivate);

public:
	// 미니맵 시공의틈새 아이콘 업데이트
	void UpdateMinimapSpaceCrackIcon();

protected:
	// 메시지
	// 로컬 스폰 끝
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam*);
	void OnLocalPlayerRevive(const struct IGsMessageParam*);	

public:	
	// 시공의 틈새 포탈 정보
	const TMap<MapId, TSharedPtr<class FGsSpaceCrackAlarmInfo>>& GetSpaceCrackAlarmInfoMap();
	// 시공의 틈새가 있니?
	const class FGsSpaceCrackAlarmInfo* FindSpaceCrackAlarmInfoByMapId(MapId inMapId);
	bool IsSpaceCrackSpawn(MapId inMapId);

public:
	const TMap<MapId, TSharedPtr<class FGsSpaceCrackActivationInfo>>& GetSpaceCrackActivationInfoMap();	
	const class FGsSpaceCrackActivationInfo* FindSpaceCrackActivationInfoByMapId(MapId inMapId);
	int64 GetCurrentActivateSpaceCrack();
	bool IsCurrentMapActiveSpaceCrack();

public:
	bool TryGetAllSpaceCrackByMapId(MapId inMapId, OUT TArray<FGsSchemaMapTerritorySpaceCrack>& outSpaceCrackArray);
};

#define GSSpaceCrack() UGsScopeHolder::GetGameManagerFType<FGsSpaceCrackManager>(UGsScopeGame::EManagerType::SpaceCrack)