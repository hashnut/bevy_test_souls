#pragma once

#include "CoreMinimal.h"
#include "EMonsterKnowledgeEnumData.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "MonsterKnowledgeCollection/EMonsterStoneSlateTreeNodeCoordinate.h"

/**
* file		GsMonsterKnowledgeStoneSlateNode.h
* @brief	몬스터 지식 석판 Tree에 대한 Node 정보
* @author	PKT
* @date		2021/11/06
**/

/************************************************************************/
/* 전방선언                                                              */
/************************************************************************/
struct FGsSchemaMonsterKnowledgeSlateStatNode;
class FGsMonsterKnowledgeSlateNodeState;

DECLARE_DELEGATE_OneParam(FOnDelegateSlateNodeState, const FGsMonsterKnowledgeSlateNodeState*);

class FGsMonsterKnowledgeSlateNodeState final
{
private:
    // 2021/12/13 PKT - schema data
    const FGsSchemaMonsterKnowledgeSlateStatNode* _statNodeData = nullptr;
    // 2021/11/09 PKT - state
    EMonsterStoneSlateNodeState _currentState = EMonsterStoneSlateNodeState::Max;
	// 2021/12/12 PKT - stat
	TArray<TPair<StatType, StatValue>> _statDataList;
	// 2021/11/09 PKT - link nodes
	mutable TMap<MonsterKnowledgeSlateSlotDir, const FGsMonsterKnowledgeSlateNodeState*> _linker;
    
private:
    // 2021/12/13 PKT - make stat list
    void MakeStatDataList(PassivityId InPassivityId, OUT TArray<TPair<StatType, StatValue>>& OutData);

public:
    FOnDelegateSlateNodeState OnDelegateSlateNodeState;

public:
    explicit FGsMonsterKnowledgeSlateNodeState(const FGsSchemaMonsterKnowledgeSlateStatNode* InData);

    bool operator==(const FGsMonsterKnowledgeSlateNodeState& InNode) const
    {
        return (this->GetId() == InNode.GetId());
    }

    /************************************************************************/
    /* Logic                                                                */
    /************************************************************************/
public:
    void MakeLinkNode(MonsterKnowledgeSlateSlotDir InDirection, FGsMonsterKnowledgeSlateNodeState* InNode) const;

    void SetStateChanged(EMonsterStoneSlateNodeState InNextState, bool InOnDelegate = true);


    /************************************************************************/
    /* Get / Find                                                           */
    /************************************************************************/
public:
    EMonsterStoneSlateNodeState GetCurrentState() const;

    MonsterKnowledgeSlateSlotId GetId() const;

    const FString& GetName() const;

    const FSoftObjectPath& GetImagePath() const;

    EMonsterStoneSlateTreeNodeCoordinate GetCoordinate() const;

    Currency GetOpenCost() const;

    PassivityId GetPassivityId() const;

    const TArray<TPair<StatType, StatValue>>& GetStatList() const;

    const FGsMonsterKnowledgeSlateNodeState* FindLinkNodeByDirection(MonsterKnowledgeSlateSlotDir InDirection) const;

    const TMap<MonsterKnowledgeSlateSlotDir, const FGsMonsterKnowledgeSlateNodeState*>& GetLinker() const;
};