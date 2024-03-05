#pragma once

#include "CoreMinimal.h"
#include "GsMonsterKnowledgeData.h"
#include "EMonsterKnowledgeEnumData.h"
#include "GsMonsterKnowledgeSlateNode.h"
#include "GsSlateSpecialNode.h"
#include "Currency/GsCostPackage.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "MonsterKnowledgeCollection/EMonsterKnowledgeSlateCategory.h"



class FGsMonsterKnowledgeSlate final
{
private:
	// 2021/11/09 PKT - Data
	const struct FGsSchemaMonsterKnowledgeSlate* _schemaData = nullptr;
	// 2021/11/10 PKT - node all
	TArray<class FGsMonsterKnowledgeSlateNode>	_nodeSet;
	// 2023/2/16 PKT - Begine
	const FGsMonsterKnowledgeSlateNode* _beginNode = nullptr;
	// 2021/12/13 PKT - cost
	TSharedRef<const FGsCostPackage> _openCostItem;
	// 2023/1/30 PKT - Have Cost Item Flag
	bool _isHaveCostItem = false;
	// 2021/11/09 PKT - State
	EMonsterKnowledgeSlateState _state = EMonsterKnowledgeSlateState::Lock;
	// 2023/4/21 PKT - Display Cost Item
	mutable const struct FGsSchemaItemCommon* _displayOnlyCostItem = nullptr;
	// 2023/2/8 PKT - Image Path
	mutable FSoftObjectPath _slateImgPath;
	// 2021/12/11 PKT - passivity set
	mutable TMap<StatType, StatValue> _statSetAll;
	
	/** ejrrb10 | 특수 노드 관련 정보 추가 */
	TArray<FGsSlateSpecialNode> _specialNodeList;

public:
	DECLARE_DELEGATE_OneParam(FOnSlateDelegate, const FGsMonsterKnowledgeSlate*);
	FOnSlateDelegate OnDelegateSlateDelegate;

	DECLARE_DELEGATE_TwoParams(FOnSpecialDelegate, const FGsMonsterKnowledgeSlate*, const FGsSlateSpecialNode*);
	FOnSpecialDelegate OnDelegateSpecialNodeCurrency;

public:
	explicit FGsMonsterKnowledgeSlate(const struct FGsSchemaMonsterKnowledgeSlate* InData, TArray<ItemId> InCostItemIds);

	void Initialized();
	void Finalized();

	bool CheckedCostItem(ItemId InCostItemId);
	bool CheckedSpecialNodeCurrency(ItemId InCostItemId);
	//void CheckedCostItem(ItemId InCostItemId);
	void ActiveSlate(bool InIsActive);
	void ActiveNode(MonsterKnowledgeSlateSlotId InActiveNodeId);
	void ActiveNodeSet(const TArray<MonsterKnowledgeSlateSlotId>& InActiveNodeIdSet);

	void ActiveSpecialNode(SlateSpecialNodeId InNodeId);

public:
	EMonsterKnowledgeSlateState State() const;

	EMonsterKnowledgeSlateCategory CategoryType() const;

	MonsterKnowledgeSlateId Id() const;

	const FText& Title() const;

	const FSoftObjectPath& ImagePath() const;

	const FSoftObjectPath& NodeSetPath() const;

	MonsterKnowledgeSlateGrade Grade() const;

	const TMap<StatType, StatValue> StatSetAll() const;

	bool IsInclusionStat(StatType InType) const;

	const FGsMonsterKnowledgeSlateNode* BeginNode() const;

	int32 NodeMaxCount() const;

	const FGsMonsterKnowledgeSlateNode* NodeAt(int32 InIndex) const;
	
	const FGsMonsterKnowledgeSlateNode* FindNodeById(MonsterKnowledgeSlateSlotId InId) const;

	TSet<const FGsMonsterKnowledgeSlateNode*> ChainNodeSet(const FGsMonsterKnowledgeSlateNode* InData) const;

	ItemId DisplayCostItemId() const;

	bool IsCostItem(const ItemId InItemId) const;

	int32 HaveCandinateCostAmount() const;

	const TArray<TWeakPtr<FGsItem>> HaveCandinateCostItems() const;
	
	TArray<PassivityId> ActivePassivtySet() const;

	int32 GetSlateSpecialNodeCount() const;

	const FGsSlateSpecialNode* FindSpecialNodeByIndex(int32 InIndex) const;

	const FGsSlateSpecialNode* FindSpecialNodeByNodeId(SlateSpecialNodeId InId) const;

	/*TArray<FGsSlateSpecialNode>& GetSlateSpecialNodeList() const;

	const FGsSlateSpecialNode* FindSpecialNodeById(SlateSpecialNodeId InId) const;*/

private:
	class FGsMonsterKnowledgeSlateNode* FindNode(MonsterKnowledgeSlateSlotId InId);

private:
	TSharedRef<const FGsCostPackage> MakeCostPackage(TArray<ItemId> InCostItemIds);

	const struct FGsSchemaItemCommon* FindDisplayItemData() const;
};