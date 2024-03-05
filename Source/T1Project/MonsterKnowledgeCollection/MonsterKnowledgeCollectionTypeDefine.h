#pragma once

/**
* file		MonsterKnowledgeCollectionTypeDefine.h
* @brief	몬스터 도감에서 편의상 사용하기 위한 타입 재정의
* @author	PKT
* @date		2021/11/19
**/

#include "CoreMinimal.h"
#include "EMonsterKnowledgeEnumData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "T1Project/T1Project.h"
#include "MonsterKnowledgeCollection/EMonsterKnowledgeSlateCategory.h"

// 2021/11/19 PKT - Main Category ID
using MonsterKnowledgeMainCategoryId = int32;
constexpr MonsterKnowledgeMainCategoryId INVALID_MONSTER_KNOWLEDGE_MAIN_CATEGORY_ID = 0;


// 2021/11/19 PKT - Sub Category ID
using MonsterKnowledgeSubCategoryId = int32;
constexpr MonsterKnowledgeSubCategoryId INVALID_MONSTER_KNOWLEDGE_SUB_CATEGORY_ID = 0;

/**
 * 몬스터 지식 정보 갱신용 Data
 */
struct FGsMonsterKnowledgeNewData
{
	FGsMonsterKnowledgeNewData(): id(0), level(0), exp(0), deltaPoint(0) {/**/ }

	explicit FGsMonsterKnowledgeNewData(uint32 InId
										, Level InLevel = 0
										, MonsterKnowledgeExp InExp = 0
										, int32 InPoint = 0)
		: id(InId), level(InLevel), exp(InExp)
	{/**/
	}

	uint32 id;
	Level level;
	MonsterKnowledgeExp exp;
	int32 deltaPoint;
};


/**
 * 몬스터 지식 Exp 변경 시 타입
 */
struct FGsMonsterKnowledgeParamData
{
	FGsMonsterKnowledgeParamData()
		: id(0), amountLevel(0), getPoint(0)
	{/**/}

	explicit FGsMonsterKnowledgeParamData(uint32 InId
										, Level InAmountLevel = 0
										, int32 InGetPoint = 0)
										: id(InId), amountLevel(InAmountLevel), getPoint(InGetPoint)
	{/**/ }

	uint32 id;
	Level amountLevel;
	int32 getPoint;
};


/**
 * 아이템 사용으로 인한 일괄 레벨업 옵션
 */
struct FGsMonsterKnowledgeUseItemOption
{
	ItemDBId selectedItemDBId = INVALID_ITEM_DB_ID;
	ItemAmount amountItem = MIN_ITEM_AMOUNT;
	int32 gradeFlag = 0;
	int32 togglePriority = 1;
	TArray<CreatureRankType> gradeCheckList;

	void CleanUp()
	{
		selectedItemDBId = INVALID_ITEM_DB_ID;
		amountItem = MIN_ITEM_AMOUNT;
		gradeFlag = 0;
		togglePriority = 1;
		gradeCheckList.Empty();
	}
};

/**
 * 몬스터 지식 아이템 정보
 */
struct FGsMonsterKnowledgeIncreaseExpItem
{
	ItemId itemId = INVALID_ITEM_ID;
	int32 orderNum;			// 2021/12/10 PKT - 정렬 번호
	int32 increment = 0;	// 2021/12/10 PKT - 증가량
};

/**
 * 몬스터 지식 석판 아이템 정보
 */
struct FGsMonsterKnowledgeSlateUnLockItem
{
	ItemId itemId = INVALID_ITEM_ID;
	MonsterKnowledgeSlateId slateId = INVALID_MONSTER_KNOWLEDGE_SLATE_ID;
	FSoftObjectPath imagePath;
};

/**
 * 몬스터 지식 아이템 유효 정보
 */
struct FGsMonsterKnowledgeExpItemVaild
{
	ItemId itemId = INVALID_ITEM_ID;
	const class FGsItem* ownedItem = nullptr;
	int32 increment = 0;
};

// 2022/02/16 PKT - 몬스터 지식 석판 리스트 FilterSet
struct FGsMonsterKnowledgeSlateFilter
{
	EMonsterKnowledgeSlateCategory categoryType = EMonsterKnowledgeSlateCategory::ATTACK;
	TArray<StatType> statTypeSet;

	void CleanUp()
	{
		categoryType = EMonsterKnowledgeSlateCategory::ATTACK;
		statTypeSet.Empty();
	}
};

/**
 * 몬스터 지식 석판 리스트 Filter Option
 */
struct FGsMonsterKnowledgeSlatFilterParam
{
	TArray<MonsterKnowledgeSlateGrade> gradeList;
	TArray<StatType> statTypesList;

	void CleanUp()
	{
		gradeList.Empty();
		statTypesList.Empty();
	}
};
