#pragma once

#include "CoreMinimal.h"
#include "EMonsterKnowledgeEnumData.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Currency/GsCostPackage.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "MonsterKnowledgeCollection/EMonsterKnowledgeSlateCategory.h"

/**
* file		GsMonsterKnowledgeSlateState.h
* @brief	몬스터 석판 정보( Tree )
* @author	PKT
* @date		2021/11/09
**/

/************************************************************************/
/* 전방선언                                                              */
/************************************************************************/
struct FGsSchemaMonsterKnowledgeSlate;
class FGsMonsterKnowledgeSlateState;
class FGsMonsterKnowledgeSlateNodeState;


DECLARE_DELEGATE_TwoParams(FOnDelegateSlateState, const FGsMonsterKnowledgeSlateState*, EMonsterKnowledgeSlateChangedState);

class FGsMonsterKnowledgeSlateState final
{
private:
	// 2021/11/09 PKT - Data
	const FGsSchemaMonsterKnowledgeSlate* _slateData = nullptr;
	// 2021/11/09 PKT - begine
	const FGsMonsterKnowledgeSlateNodeState* _beginNode = nullptr;
	// 2021/11/10 PKT - node all
	TArray<FGsMonsterKnowledgeSlateNodeState*>	_nodeList;
	// 2021/12/12 PKT - tree by depth
	TMap<int32, TArray<const FGsMonsterKnowledgeSlateNodeState*>> _depthNodeData;
	// 2021/11/09 PKT - State
	EMonsterKnowledgeSlateState _State = EMonsterKnowledgeSlateState::Lock;
	// 2021/12/11 PKT - image path
	FSoftObjectPath _imagePath;
	// 2021/11/19 PKT - passivity
	PassivityId _passivityId;
	// 2021/12/11 PKT - passivity set
	TArray<PassivityId> _passivitySet;
	// 2021/12/13 PKT - cost
	TSharedRef<const FGsCostPackage> _costPackage;
	// 2021/12/13 PKT - have item flag
	bool bIsHaveItem = false;
	
public:
	FOnDelegateSlateState OnDelegateSlateState;

public:
	explicit FGsMonsterKnowledgeSlateState(const FGsSchemaMonsterKnowledgeSlate* InData
		, TArray<FGsMonsterKnowledgeSlateNodeState*>& InNodeList, ItemId InUnLockItem, const FSoftObjectPath& InImagePath);

	bool operator==(const FGsMonsterKnowledgeSlateState& InState) const
	{
		return (this->GetId() == InState.GetId());
	}

protected:
	const FGsMonsterKnowledgeSlateNodeState* MakeLinkkNode(const FGsSchemaMonsterKnowledgeSlate* InData
		, TArray<FGsMonsterKnowledgeSlateNodeState*>& InNodeList);

	void RecursionCalcDepthTree(const FGsMonsterKnowledgeSlateNodeState* InNode
		, const FGsMonsterKnowledgeSlateNodeState* InPreNode
		, OUT int32& InDepth
		, OUT int32& InCount);

	TSharedRef<const FGsCostPackage> MakeCostPackage(const ItemId InUnLockItemId);

public:
	void CheckHaveUnLockItem();

	void SetStateChanged(EMonsterKnowledgeSlateState InNextState, bool InIsDelegate = true);

	void SetForceNodeState(const TArray<MonsterKnowledgeSlateSlotId>& InActiveNodeList, bool InIsDelegate = true);

public:
	EMonsterKnowledgeSlateState GetState() const;

	EMonsterKnowledgeSlateCategory GetCategoryType() const;

	MonsterKnowledgeSlateId GetId() const;

	const FText& GetTitle() const;

	ItemId GetUnLockItemId() const;

	const FSoftObjectPath& GetImagePath() const;
	
	MonsterKnowledgeSlateGrade GetGrade() const;

	const TArray<PassivityId>& GetPassivitySet() const;

	bool IsInclusionStat(StatType InType) const;

	int32 GetDepthCount() const;

	const TArray<const FGsMonsterKnowledgeSlateNodeState*>* GetNodeListByDepth(int32 InDepth) const;	

	const FGsMonsterKnowledgeSlateNodeState* GetBeginNode() const;

	const TArray<FGsMonsterKnowledgeSlateNodeState*>& GetNodeListAll() const;

	TSharedRef<const FGsCostPackage> GetCostPackage() const;

	ItemId GetOpenItemId() const;

	int32 HaveItemAmount() const;
};