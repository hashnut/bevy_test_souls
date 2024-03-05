#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedBookmarkEnum.h"

struct FGsReqEventActionParam;

class FGsNetSendServiceBoss
{
public:
	static void SendBossContribution(FieldBossId inBossId);
	static void SendReqCompetitionFieldBossLastDead(FieldBossId inFieldBossId);
	static void SendReqUpdateFieldBossBookmark(const TArray<FieldBossId>& inFieldBossIdArray);

public:
	static void SendReqWorldBossMapWarp(MapId inMapId);
	static void SendReqWorldBossMapLeave();
	static void SendReqWorldBossInfo(WorldBossId inWorldBossId);

};
