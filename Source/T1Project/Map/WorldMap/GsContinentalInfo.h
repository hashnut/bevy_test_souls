#pragma once
#include "CoreMinimal.h"
#include "GsRegionGroupInfoBase.h"

/*
	대륙 정보
*/
class FGsContinentalInfo : public FGsRegionGroupInfoBase
{
	using Super = FGsRegionGroupInfoBase;

public:
	// array: 던전 지역 id
	TArray<int> _arrayDungeonRegionId;

	// 아지트 지역맵 id
	int _agitRegionId;

	// destruct
public:
	virtual ~FGsContinentalInfo() = default;

	// virtual function
public:
	virtual void Finalize() override
	{
		Super::Finalize();


		if (_arrayDungeonRegionId.Num() != 0)
		{
			_arrayDungeonRegionId.Empty();
		}
	}
};