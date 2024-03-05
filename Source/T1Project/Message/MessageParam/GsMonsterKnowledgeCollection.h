#pragma once

#include "GsMessageParam.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "MonsterKnowledgeCollection/EMonsterKnowledgeEnumData.h"
#include "MonsterKnowledgeCollection/GsMonsterKnowledgeAreaCategory.h"
#include "MonsterKnowledgeCollection/MonsterKnowledgeCollectionTypeDefine.h"
#include "MonsterKnowledgeCollection/GsMonsterKnowledgeState.h"




/************************************************************************/
/* Monster Knowledge Collection                                         */
/************************************************************************/


struct FGsMessageMonsterKnowledgeCategoryReddot : public IGsMessageParam
{
	const TArray<const FGsMonsterKnowledgeSubCategoryInfo*>* paramDataList = nullptr;

	explicit FGsMessageMonsterKnowledgeCategoryReddot(const TArray<const FGsMonsterKnowledgeSubCategoryInfo*>* InData) : paramDataList(InData)
	{
		/**/
	}

	virtual ~FGsMessageMonsterKnowledgeCategoryReddot()
	{
		/**/
	}
};


struct FGsMessageMonsterKnowledgeMonsterList : public IGsMessageParam
{
	MonsterKnowledgeSubCategoryId paramData = INVALID_MONSTER_KNOWLEDGE_SUB_CATEGORY_ID;

	explicit FGsMessageMonsterKnowledgeMonsterList(MonsterKnowledgeSubCategoryId InData) : paramData(InData)
	{
		/**/
	}

	virtual ~FGsMessageMonsterKnowledgeMonsterList()
	{
		/**/
	}
};


/**
 * 몬스터 지식 아이템 사용으로 인한 경험치 증가 및 레벨업
 */
struct FGsMessageMonsterKnowledgeStateChanged : public IGsMessageParam
{
	const FGsMonsterKnowledgeParamData* paramData;
	explicit FGsMessageMonsterKnowledgeStateChanged(const FGsMonsterKnowledgeParamData* InData) : paramData(InData)
	{
		/**/
	}

	virtual ~FGsMessageMonsterKnowledgeStateChanged()
	{
		/**/
	}
};


/**
 * 일괄 레벨 업 / 일괄 아이템 사용
 */
struct FGsMessageMonsterKnowledgeStateChangedAll : public IGsMessageParam
{
	const TArray<FGsMonsterKnowledgeParamData>* paramDataList;

	explicit FGsMessageMonsterKnowledgeStateChangedAll(const TArray<FGsMonsterKnowledgeParamData>* InData) : paramDataList(InData)
	{
		/**/
	}

	virtual ~FGsMessageMonsterKnowledgeStateChangedAll()
	{
		/**/
	}
};




/************************************************************************/
/* Monster Knowledge Slate                                              */
/************************************************************************/

struct FGsMessageMonsterKnowledgeSlateCategory : public IGsMessageParam
{
	MonsterKnowledgeSlateId slateId = INVALID_MONSTER_KNOWLEDGE_SLATE_ID;

	explicit FGsMessageMonsterKnowledgeSlateCategory(MonsterKnowledgeSlateId InId) : slateId(InId)
	{
		/**/
	}

	virtual ~FGsMessageMonsterKnowledgeSlateCategory()
	{
		/**/
	}
};


/**
 * 지식 석판 활성화
 */
struct FGsMessageMonsterKnowledgeSlateActive : public IGsMessageParam
{
	MonsterKnowledgeSlateId _slateId = INVALID_MONSTER_KNOWLEDGE_SLATE_ID;

	explicit FGsMessageMonsterKnowledgeSlateActive(MonsterKnowledgeSlateId InSlateId) : _slateId(InSlateId)
	{
		/**/
	}

	virtual ~FGsMessageMonsterKnowledgeSlateActive()
	{
		/**/
	}
};

/**
 * 지식 석판 Slot 활성화
 */
struct FGsMessageMonsterKnowledgeSlateSloatActive : public IGsMessageParam
{
	const TArray<MonsterKnowledgeSlateSlotId>* _slateSlostList;

	explicit FGsMessageMonsterKnowledgeSlateSloatActive(const TArray<MonsterKnowledgeSlateSlotId>* InSlateSlostList) 
		: _slateSlostList(InSlateSlostList)
	{
		/**/
	}

	virtual ~FGsMessageMonsterKnowledgeSlateSloatActive()
	{
		/**/
	}
};




/**
 * 지식 석판 활성화 아이템 사용
*/
//struct FGsMessageMonsterKnowledgeSlateUseActiveItem : public IGsMessageParam
//{
//	ItemId _useItemId = INVALID_ITEM_ID;
//	ItemDBId _useItemDbId = INVALID_ITEM_DB_ID;
//
//	explicit FGsMessageMonsterKnowledgeSlateUseActiveItem(ItemId InItemId, ItemDBId InItemDBId) : _useItemId(InItemId), _useItemDbId(InItemDBId)
//	{
//		/**/ 
//	}
//	
//	virtual ~FGsMessageMonsterKnowledgeSlateUseActiveItem() 
//	{
//		/**/ 
//	}
//};


/**
 * 지식 석판 활성화 아이템 사용에 대한 Window 바로가기
 */
struct FGsMessageMonsterKnowledgeOpenSlateForUseItem : public IGsMessageParam
{
	MonsterKnowledgeSlateId _Id;

	explicit FGsMessageMonsterKnowledgeOpenSlateForUseItem(MonsterKnowledgeSubCategoryId InId) : _Id(InId)
	{
		/**/ 
	}

	virtual ~FGsMessageMonsterKnowledgeOpenSlateForUseItem()
	{
		/**/
	}
};


/**
 * 지식 석판 노드 초기화
 */
struct FGsMessageMonsterKnowledgeSlateNodeReset : public IGsMessageParam
{
	explicit FGsMessageMonsterKnowledgeSlateNodeReset()
	{
		/**/
	}

	virtual ~FGsMessageMonsterKnowledgeSlateNodeReset()
	{
		/**/
	}
};


struct FGsMessageMonsterCollectionBookResultSet : public IGsMessageParam
{
	const TArray<FGsMonsterCollectionBookResult>* _data = nullptr;

	explicit FGsMessageMonsterCollectionBookResultSet(const TArray<FGsMonsterCollectionBookResult>* InData) : _data(InData) {/**/ }
	virtual ~FGsMessageMonsterCollectionBookResultSet() {/**/ }
};

struct FGsMessageMonsterCollectionSlate : public IGsMessageParam
{
	MonsterKnowledgeSlateId _data;	

	explicit FGsMessageMonsterCollectionSlate(MonsterKnowledgeSlateId InData) : _data(InData)	{/**/}
	virtual ~FGsMessageMonsterCollectionSlate(){/**/}
};

struct FGsMessageCodexKnowledgePoint : public IGsMessageParam
{
	/** 총 획득 도감 포인트 */
	Currency _fianlMonsterBookPoint;

	/** 현재 지식 포인트 */
	MonsterKnowledgeExp _finalMonsterKnowledgeExp;

	explicit FGsMessageCodexKnowledgePoint(Currency InFinalBookPoint, MonsterKnowledgeExp InFinalKnowledgeExp) :
		_fianlMonsterBookPoint(InFinalBookPoint), _finalMonsterKnowledgeExp(InFinalKnowledgeExp)
	{
	}
};

struct FGsMessageCodexMapGroupExpUpdate : public IGsMessageParam
{
	/** 현재 획득한 몬스터 지식 */
	MonsterKnowledgeExp _monsterKnowledgeExp;

	explicit FGsMessageCodexMapGroupExpUpdate(MonsterKnowledgeExp InMonsterKnowledgeExp) : _monsterKnowledgeExp(InMonsterKnowledgeExp)
	{
	}
};

struct FGsMessageCodexCategoryLevelUp : public IGsMessageParam
{
	/** 현재 레벨업 한 카테고리 Id */
	CodexCategoryId _codexCategoryId;

	/** 현재 레벨업 한 카테고리의 레벨 */
	CodexCategoryLevel _codexCategoryLevel;

	/** 현재 레벨업 한 카테고리의 레벨 */
	CodexCategoryLevel _mapGroupLevelUpCount;

	explicit FGsMessageCodexCategoryLevelUp(CodexCategoryId InCodexCategoryId, CodexCategoryLevel InCodexCategoryLevel, int32 InMapGroupLevelUpCount) : _codexCategoryId(InCodexCategoryId), _codexCategoryLevel(InCodexCategoryLevel), _mapGroupLevelUpCount(InMapGroupLevelUpCount)
	{
	}
};

struct FGsMessageSpecialNodeResult: public IGsMessageParam
{
	bool _result;

	explicit FGsMessageSpecialNodeResult(bool InResult) : _result(InResult)
	{
	}
};