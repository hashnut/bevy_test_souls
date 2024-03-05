#pragma once

#include "CoreMinimal.h"
#include "MonsterKnowledgeCollectionTypeDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "DataSchema/GsSchemaEnums.h"
#include "T1Project/T1Project.h"

/**
 * 몬스터 지식 수량 카테고리에 필요한 Main Data
 */
struct T1PROJECT_API FGsMonsterKnowledgeMainCategoryInfo
{
	bool operator==(const FGsMonsterKnowledgeMainCategoryInfo& InData)
	{
		return (this->_id == InData._id);
	}

	MonsterKnowledgeMainCategoryId _id;
	FText _name = FText::GetEmpty();
};

/**
* 몬스터 지식 수량 카테고리에 필요한 Sub Data
*/
struct T1PROJECT_API FGsMonsterKnowledgeSubCategoryInfo
{
	bool operator==(const FGsMonsterKnowledgeSubCategoryInfo& InData)
	{
		return (this->_id == InData._id);
	}

	MonsterKnowledgeSubCategoryId _id;			// 2021/11/19 PKT - 서브 카테고리 Id
	const FGsMonsterKnowledgeMainCategoryInfo* _mainCategory;	// 2021/11/19 PKT - 메인 카테고리 Id
	FText _name = FText::GetEmpty();			// 2021/11/16 PKT - 지역 대표 이름(던전 같은 경우 대표 이름이 있음)	
	MonsterKnowledgeAppearType _appearType = MonsterKnowledgeAppearType::MAX;
	TArray<int32> _CategoryElements;			// 2021/11/16 PKT - 카테고리에 그룹되어 있는 Id(던전 일 경우 하나의 대포 ID에 여러 지역 ID가 그룹 된다.)
};