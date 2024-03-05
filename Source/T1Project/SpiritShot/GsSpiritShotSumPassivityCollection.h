#pragma once
#pragma once

#include "Classes/Collection/GsCollectionData.h"

#include "T1Project.h"


class FGsSpiritShotSumPassivityCollection : public IGsCollectionData
{
	/*
	* Passivity Collection System 사용을 위한 구현
	*/

	TArray<PassivityId> _collectionList;

public:
	FGsSpiritShotSumPassivityCollection() = default;
	virtual ~FGsSpiritShotSumPassivityCollection() = default;

	/*
	* Interface 함수 정의
	*/
public:
	virtual PassivityOwnerType GetPassivityOwnerType() const final
	{
		return PassivityOwnerType::SPIRIT_SHOT_ENCHANT_SUM_LEVEL;
	}

	virtual int32 GetPassivityCount() const final
	{
		return _collectionList.Num();
	}

	virtual PassivityId GetPassivityId(int32 InIndex) const final
	{
		if (_collectionList.IsValidIndex(InIndex))
		{
			return _collectionList[InIndex];
		}
		return INVALID_PASSIVITY_ID;
	}


	/*
	* Collection Data Add / Empty
	*/
public:
	void Add(PassivityId InId)
	{
		if (INVALID_PASSIVITY_ID != InId)
		{
			_collectionList.Emplace(InId);
		}
		else
		{
			// 올바르지 않은 Id 로그
			GSLOG(Error, TEXT("FGsCollectionSpiritShotPassivity::Add Invaild InId %d"), InId);
		}
	}

	void Empty()
	{
		_collectionList.Empty();
	}
};
