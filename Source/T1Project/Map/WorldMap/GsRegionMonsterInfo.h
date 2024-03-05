#pragma once
#include "CoreMinimal.h"


#include "Map/WorldMap/GsWorldMapDefine.h"
/*
	지역맵 몬스터 정보
*/

struct FGsSchemaNpcData;
struct FGsSchemaItemCommon;
struct FGsSchemaNpcBalance;
struct FGsSchemaFieldBossData;

class FGsRegionMonsterItemInfo
{
public:
	const FGsSchemaItemCommon* _itemTbl;

	EGsRegionItemSecondSortType _itemSortType;

public:
	FGsRegionMonsterItemInfo(const FGsSchemaItemCommon* In_tbl);
	
	virtual ~FGsRegionMonsterItemInfo() = default;

public:
	void Finalize()
	{
		_itemTbl = nullptr;
	}
};


class FGsRegionMonterItemInfoEtc : public FGsRegionMonsterItemInfo
{
	using Super = FGsRegionMonsterItemInfo;
public:
	EGsRegionEtcItemSecondSortType _sortType;

public:
	FGsRegionMonterItemInfoEtc(const FGsSchemaItemCommon* In_tbl);
public:
	virtual ~FGsRegionMonterItemInfoEtc() = default;
};

class FGsRegionMonsterItemInfoSet
{
public:
	// 모든 아이템(통합됨, 월드맵 개선)
	// https://jira.com2us.com/jira/browse/CHR-23756
	TArray<FGsRegionMonsterItemInfo*> _arrayItem;

public:
	virtual ~FGsRegionMonsterItemInfoSet() = default;

public:
	void Finalize();
};

class FGsRegionMonsterInfo
{
public:
	// 몬스터 테이블 정보
	const FGsSchemaNpcData* _npcData;
	// 몬스터 밸런스 정보
	const FGsSchemaNpcBalance* _npcBalance;
	// only use field(or world) boss 
	const FGsSchemaFieldBossData* _fieldBossData;

	int32 level;

	TMap<EGsRegionItemInfoSetType, FGsRegionMonsterItemInfoSet* > _mapItemInfoSet;

	bool _isOffensive;

public:
	void Finalize()
	{
		_npcData = nullptr;
		_npcBalance = nullptr;

		if (_mapItemInfoSet.Num() != 0)
		{
			for (TPair<EGsRegionItemInfoSetType, FGsRegionMonsterItemInfoSet*> iter: _mapItemInfoSet)
			{
				if (iter.Value == nullptr)
				{
					continue;
				}
				iter.Value->Finalize();
				delete iter.Value;
			}
			_mapItemInfoSet.Empty();
		}

	}

	bool GetDropItem(EGsRegionItemInfoSetType In_itemInfoSetType, TArray< FGsRegionMonsterItemInfo*>& Out_findItemInfo)
	{
		FGsRegionMonsterItemInfoSet* findInfo = _mapItemInfoSet.FindRef(In_itemInfoSetType);
		if (findInfo == nullptr)
		{
			return false;
		}

		Out_findItemInfo =  findInfo->_arrayItem;
		return true;
	}
};

// 몬스터 정보 팝업에서 레벨 콤보 박스 출력 요청(도감에서 넘어왔을때 레벨에 따른 정보 볼수 없어서)
// 몬스터 id는 같은데 레벨이 다른정보(추가 정보가 있을수... 있을까)
// https://jira.com2us.com/jira/browse/CHR-24415
class FGsRegionMonsterSameIDInfo
{
public:
	// 몬스터 tbl id
	int _monsterTblId;
	// 레벨 정보
	TArray<int> _arrayLevel;

public:
	// 정리
	void Finalize();
};