#pragma once

#include "CoreMinimal.h"
#include "GsBattleArenaRankData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"



class T1PROJECT_API FGsBattleArenaRankScheduleHandler
{
protected:

	TArray<FGsBattleArenaScheduler> _scheduleSet;
	TArray<FGsBattleArenaSchedulePendingData> _pendingDataSet;

	float _currentDelay = 0.f;

public:
	void Initialize();
	void Finalize();
	void Update(float inTick);

	void Push(RankSearchType InRankSearchType, GuildWarTierDataId InTierDataId, const TArray<int32>& InIndexSet);
	void ClearAll(RankSearchType InRankSearchType);
	void RemoveAll(RankSearchType InRankSearchType);

protected:
	void UpdateSchedule(float InTick);
	void UpdatePending(float InTick);
	
	void AddPendingData(const FGsBattleArenaScheduler& InData);
};