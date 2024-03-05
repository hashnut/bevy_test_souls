#pragma once

#include "CoreMinimal.h"
#include "ETitleEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
* file		GsTitleRewardData.h
* @brief	칭호 시스템 컬렉션 Data
* @author	PKT
* @date		2022/01/14
**/

class T1PROJECT_API FGsTitleRewardData final
{
	/************************************************************************/
	/* Member                                                               */
	/************************************************************************/
private:
	const struct FGsSchemaTitleCollectionReward* _data = nullptr;

	ETitleRewardState _state;

	TArray<PassivityId> _passivitySetIds;

	TArray<TPair<StatType, int32>> _statSet;

public:
	FGsTitleRewardData(const struct FGsSchemaTitleCollectionReward* InData);

	void SetState(ETitleRewardState InState);

	int32 GetId() const;

	ETitleRewardState GetState() const;

	int32 GetAcquisitionCount() const;

	const TArray<PassivityId>& GetPassivityIds() const;

	int32 GetStatCount() const;

	bool FindStatData(int32 InIndex, OUT TPair<StatType, int32>& OutStatData) const;
};