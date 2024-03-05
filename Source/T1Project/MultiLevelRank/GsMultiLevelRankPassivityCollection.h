#pragma once

#include "Classes/Collection/GsCollectionData.h"

#include "T1Project.h"


class FGsMultiLevelRankPassivityCollection : public IGsCollectionData
{
	/*
	* Passivity Collection System 사용을 위한 구현
	*/

	TArray<PassivityId> _collectionList;

public:
	FGsMultiLevelRankPassivityCollection() = default;
	virtual ~FGsMultiLevelRankPassivityCollection() = default;

	/*
	* Interface 함수 정의
	*/
public:
	virtual PassivityOwnerType GetPassivityOwnerType() const final
	{
		return PassivityOwnerType::MULTI_LEVEL_RANK;
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
		if (INDEX_NONE == _collectionList.Find(InId) && InId != INVALID_PASSIVITY_ID)
		{
			_collectionList.Emplace(InId);
		}
		else
		{
			// 중복 or 올바르지 않은 Id 로그
			GSLOG(Error, TEXT("FGsCollectionMultiLevelRankPassivity::Add Invaild InId %d"), InId);
		}
	}

	void Empty()
	{
		_collectionList.Empty();
	}
};
