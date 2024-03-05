#pragma once

#include "CoreMinimal.h"
#include "EMonsterKnowledgeEnumData.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "MonsterKnowledgeCollection/EMonsterStoneSlateTreeNodeCoordinate.h"


class FGsMonsterKnowledgeSlateNode 
{
private:
	// 2021/12/13 PKT - schema data
	const struct FGsSchemaMonsterKnowledgeSlateStatNode* _schemaData = nullptr;
	// 2021/11/09 PKT - state
	EMonsterStoneSlateNodeState _state = EMonsterStoneSlateNodeState::Max;
	// 2021/12/12 PKT - stat
	TArray<TPair<StatType, StatValue>> _effectStatSet;
	// 2023/3/7 PKT - Link Data
	const FGsMonsterKnowledgeSlateNode* _prevNodeData = nullptr;
	
public:
	explicit FGsMonsterKnowledgeSlateNode(const struct FGsSchemaMonsterKnowledgeSlateStatNode* InData);

	bool operator==(const FGsMonsterKnowledgeSlateNode* InData) const
	{
		return this->Id() == InData->Id();
	}

	bool operator==(const FGsMonsterKnowledgeSlateNode& InData) const
	{
		return this->Id() == InData.Id();
	}

	// 2023/3/7 PKT - 초기화
	void Initialized(const FGsMonsterKnowledgeSlateNode* InPrevNodeData);

	// 2023/2/16 PKT - 상태 변경
	void ChangeState(EMonsterStoneSlateNodeState InNextState);
	/**
	 * Getter
	 */
public:
	EMonsterStoneSlateNodeState State() const;

	MonsterKnowledgeSlateSlotId Id() const;

	int32 ResourceId() const;

	const FString& Name() const;

	const FSoftObjectPath& ImagePath() const;

	Currency AmountOfCurrency() const;

	PassivityId Passivity() const;

	const TArray<TPair<StatType, StatValue>>& StatSet() const;

	MonsterKnowledgeSlateSlotId PrevNodeId() const;

	const FGsMonsterKnowledgeSlateNode* PrevNodeData() const;

	/**
	 * Builder
	 */
private:
	TArray<TPair<StatType, StatValue>> BuildEffectStat(PassivityId InPassivityId) const;
};