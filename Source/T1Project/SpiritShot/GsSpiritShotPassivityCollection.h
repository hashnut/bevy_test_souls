#pragma once
#pragma once

#include "Classes/Collection/GsCollectionData.h"

#include "T1Project.h"


class FGsSpiritShotPassivityCollection : public IGsCollectionData
{
	/*
	* Passivity Collection System 사용을 위한 구현
	*/

	TArray<PassivityId> _collectionList;

public:
	FGsSpiritShotPassivityCollection() = default;

	FGsSpiritShotPassivityCollection(const FGsSpiritShotPassivityCollection* InData)
	{
		_collectionList = InData->GetCollectionList();
	}

	FGsSpiritShotPassivityCollection(const FGsSpiritShotPassivityCollection& InData)
	{
		_collectionList = InData.GetCollectionList();
	}

	FGsSpiritShotPassivityCollection(TArray<PassivityId> InCollectionList)
	{
		_collectionList = InCollectionList;
	}

	virtual ~FGsSpiritShotPassivityCollection() = default;

	/*
	* Interface 함수 정의
	*/
public:
	virtual PassivityOwnerType GetPassivityOwnerType() const final
	{
		return PassivityOwnerType::SPIRIT_SHOT_ENCHANT;
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
	
	TArray<PassivityId> GetCollectionList() const
	{
		return _collectionList;
	}


	/*
	* Collection Data Add / Append / Empty
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

	void Append(TArray<PassivityId> InList)
	{
		for (PassivityId id : InList)
		{
			Add(id);
		}
	}

	void Empty()
	{
		_collectionList.Empty();
	}
};
