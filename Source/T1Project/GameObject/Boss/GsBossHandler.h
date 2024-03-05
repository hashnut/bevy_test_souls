// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"
#include "GsWorldBossHandler.h"
#include "GsFieldBossHandler.h"


//=================== field boss manager ====================
class T1PROJECT_API FGsBossHandler
{
public:
	TUniquePtr<class FGsFieldBossHandler> _fieldBossHandler;
	TUniquePtr<class FGsWorldBossHandler> _worldBossHandler;

public:
	void Init(); 
	void Close();

//private:
//	TMap<FieldBossId, FGsFieldBossInfo> _fieldBossMap;
//	TMap<FieldBossId, FGsBossMapContribution> _bossContributionMap;
//	TMap<FieldBossId, FGsFieldBossInfoCooperation*> _mapCooperationBossInfo;
//
//private:
//	MsgUIHandleArray _msgUIHandleList;
//	MsgSystemHandleArray _systemMsgList;
//	MsgBossHandleArray _msgBossHandleList;

public:
	FGsFieldBossHandler* GetFieldBossHandler() { return _fieldBossHandler.Get(); }
	FGsWorldBossHandler* GetWorldBossHandler() { return _worldBossHandler.Get(); }

public:
	static bool IsFieldBossSpawnGroup(MapId inMapId, int64 inSpawnGroup);	

public:
	static FGsBossHandler* GetInstance();
};
