
#include "GsMonsterKnowledgeSlate.h"
#include "../Currency/GsCostPackage.h"
#include "GsMonsterKnowledgeSlateNode.h"
#include "MonsterKnowledgeCollection/EMonsterKnowledgeSlateCategory.h"
#include "../Item/GsItemManager.h"
#include "DataSchema/Item/GsSchemaItemEffectUnlockMonsterKnowledgeSlate.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeSlate.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeSlateStatNode.h"
#include "EMonsterKnowledgeEnumData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "T1Project.h"



FGsMonsterKnowledgeSlate::FGsMonsterKnowledgeSlate(const struct FGsSchemaMonsterKnowledgeSlate* InData, TArray<ItemId> InCostItemIds)
	: _schemaData(InData), _beginNode(nullptr), _openCostItem(MakeCostPackage(InCostItemIds)), _isHaveCostItem(false), _state(EMonsterKnowledgeSlateState::Lock), _displayOnlyCostItem(nullptr)
{
	
}

void FGsMonsterKnowledgeSlate::Initialized()
{
	// 2023/3/7 PKT - 초기화
	_isHaveCostItem = false;
	_slateImgPath = FSoftObjectPath();
	_state = EMonsterKnowledgeSlateState::Lock;
	_statSetAll.Empty();
	_displayOnlyCostItem = nullptr;

	/**
	 * 2023/3/7 PKT - Create Node
	 */
	for (auto item : _schemaData->linkNodeList)
	{
		_nodeSet.Emplace(FGsMonsterKnowledgeSlateNode(item.GetRow()));
	}

	// 2023/3/7 PKT - Set Begin Node
	if (_schemaData->beginNode.GetRow())
	{
		_beginNode = FindNodeById(_schemaData->beginNode.GetRow()->id);
	}

	// 2023/3/7 PKT - Prev Node Dat
	for (auto& item : _nodeSet)
	{
		item.Initialized(FindNodeById(item.PrevNodeId()));
	}

	// SlateSpecialNode
	for (const auto& specialData : _schemaData->specialNodeList)
	{
		_specialNodeList.Emplace(FGsSlateSpecialNode(specialData.specialNodeId.GetRow()));
	}
}

void FGsMonsterKnowledgeSlate::Finalized()
{
	_nodeSet.Empty();
	_statSetAll.Empty();
	_specialNodeList.Empty();
}

bool FGsMonsterKnowledgeSlate::CheckedCostItem(ItemId InCostItemId)
{
	bool isChangedCostState = false;
	if (EMonsterKnowledgeSlateState::UnLock != _state)
	{
		bool prevHaveCostItem = _isHaveCostItem;
		_isHaveCostItem = (0 < HaveCandinateCostAmount());

		if (_isHaveCostItem != prevHaveCostItem)
		{
			isChangedCostState = true;
		}
	}

	if (isChangedCostState && OnDelegateSlateDelegate.IsBound())
	{
		OnDelegateSlateDelegate.ExecuteIfBound(this);
	}

	return isChangedCostState;
}

bool FGsMonsterKnowledgeSlate::CheckedSpecialNodeCurrency(ItemId InCostItemId)
{
	for (const auto& item : _specialNodeList)
	{
		if (const FGsSchemaItemCommon* itemCommon = item.GetSchemaData()->requireItemId.GetRow())
		{
			const TSet<ItemId> groupItemSet = UGsItemManager::GetAllItemIdsTheSameGroup(itemCommon->id);
			if (nullptr != groupItemSet.Find(InCostItemId))
			{
				if (OnDelegateSpecialNodeCurrency.IsBound())
				{
					OnDelegateSpecialNodeCurrency.ExecuteIfBound(this, &item);
				}				
				return true;
			}
		}
	}
	return false;
}

void FGsMonsterKnowledgeSlate::ActiveSlate(bool InIsActive)
{
	_state = (InIsActive) ? EMonsterKnowledgeSlateState::UnLock : EMonsterKnowledgeSlateState::Lock;

	for (auto& item : _nodeSet)
	{
		item.ChangeState((InIsActive) ? EMonsterStoneSlateNodeState::Normal : EMonsterStoneSlateNodeState::Disable);
	}

	if (EMonsterKnowledgeSlateState::UnLock == _state)
	{	// 2023/11/9 PKT - 석판이 해제가 되었다면 특수 노드 1번째꺼는 자동으로 UnLock 상태가 되어야 한다.
		const static int32 FIRST_INDEX = 0;
		if (_specialNodeList.IsValidIndex(FIRST_INDEX) && EMonsterStoneSlateNodeState::Disable == _specialNodeList[FIRST_INDEX].GetState())
		{
			_specialNodeList[FIRST_INDEX].SetState(EMonsterStoneSlateNodeState::Normal);
		}
	}

	if (OnDelegateSlateDelegate.IsBound())
	{
		OnDelegateSlateDelegate.ExecuteIfBound(this);
	}
}

void FGsMonsterKnowledgeSlate::ActiveNode(MonsterKnowledgeSlateSlotId InActiveNodeId)
{
	bool isSendMessage = false;
	for (auto& item : _nodeSet)
	{
		if (item.Id() == InActiveNodeId)
		{
			_state = EMonsterKnowledgeSlateState::UnLock;
			
			item.ChangeState(EMonsterStoneSlateNodeState::Complete);

			isSendMessage = true;
		}
	}

	if (isSendMessage && OnDelegateSlateDelegate.IsBound())
	{
		OnDelegateSlateDelegate.ExecuteIfBound(this);
	}
}

void FGsMonsterKnowledgeSlate::ActiveNodeSet(const TArray<MonsterKnowledgeSlateSlotId>& InActiveNodeIdSet)
{
	for (auto& item : _nodeSet)
	{
		if (EMonsterKnowledgeSlateState::UnLock == _state)
		{
			if (InActiveNodeIdSet.Contains(item.Id()))
			{
				item.ChangeState(EMonsterStoneSlateNodeState::Complete);
			}
			else
			{
				item.ChangeState(EMonsterStoneSlateNodeState::Normal);
			}
		}
		else
		{
			item.ChangeState(EMonsterStoneSlateNodeState::Disable);
		}
	}

	if (OnDelegateSlateDelegate.IsBound())
	{
		OnDelegateSlateDelegate.ExecuteIfBound(this);
	}
}

void FGsMonsterKnowledgeSlate::ActiveSpecialNode(SlateSpecialNodeId InNodeId)
{
	if (EMonsterKnowledgeSlateState::Lock == _state)
	{	// 2023/11/15 PKT - 노드가 열리지 않았다면 특수노드도 열릴 수 없다.
		return;
	}

	const int32 findIndex = _specialNodeList.IndexOfByPredicate([InNodeId](const FGsSlateSpecialNode& InData)
		{
			return InNodeId == InData.GetId();
		}
	);

	if (INDEX_NONE >= findIndex)
	{
		return;
	}

	_specialNodeList[findIndex].SetState(EMonsterStoneSlateNodeState::Complete);
	if (_specialNodeList.IsValidIndex(findIndex + 1))
	{
		_specialNodeList[findIndex + 1].SetState(EMonsterStoneSlateNodeState::Normal);
	}

	if (OnDelegateSlateDelegate.IsBound())
	{
		OnDelegateSlateDelegate.ExecuteIfBound(this);
	}
}

EMonsterKnowledgeSlateState FGsMonsterKnowledgeSlate::State() const
{
	return _state;
}

EMonsterKnowledgeSlateCategory FGsMonsterKnowledgeSlate::CategoryType() const
{
	return _schemaData->categoryType;
}

MonsterKnowledgeSlateId FGsMonsterKnowledgeSlate::Id() const
{
	return _schemaData->id;
}

const FText& FGsMonsterKnowledgeSlate::Title() const
{
	return _schemaData->name;
}

const FSoftObjectPath& FGsMonsterKnowledgeSlate::ImagePath() const
{
	if (false == _slateImgPath.IsValid())
	{
		if (nullptr == _displayOnlyCostItem)
		{
			_displayOnlyCostItem = FindDisplayItemData();
		}

		if (_displayOnlyCostItem)
		{
			_slateImgPath = _displayOnlyCostItem->icon;
		}
	}

	return _slateImgPath;
}

const FSoftObjectPath& FGsMonsterKnowledgeSlate::NodeSetPath() const
{
	return _schemaData->resourcePath;
}

MonsterKnowledgeSlateGrade FGsMonsterKnowledgeSlate::Grade() const
{
	return _schemaData->grade;
}

const TMap<StatType, StatValue> FGsMonsterKnowledgeSlate::StatSetAll() const
{
	if (0 >= _statSetAll.Num())
	{
		for (auto& item : _nodeSet)
		{
			for (auto statData : item.StatSet())
			{
				_statSetAll.FindOrAdd(statData.Key) += statData.Value;
			}
		}
		// 2023/3/7 PKT - Stat 정렬은 필요한 곳에서..
	}

	return _statSetAll;
}

bool FGsMonsterKnowledgeSlate::IsInclusionStat(StatType InType) const
{
	return StatSetAll().Contains(InType);
}

const FGsMonsterKnowledgeSlateNode* FGsMonsterKnowledgeSlate::BeginNode() const
{
	return _beginNode;
}

int32 FGsMonsterKnowledgeSlate::NodeMaxCount() const
{
	return _nodeSet.Num();
}

const FGsMonsterKnowledgeSlateNode* FGsMonsterKnowledgeSlate::NodeAt(int32 InIndex) const
{
	return _nodeSet.IsValidIndex(InIndex) ? &_nodeSet[InIndex] : nullptr;
}

const FGsMonsterKnowledgeSlateNode* FGsMonsterKnowledgeSlate::FindNodeById(MonsterKnowledgeSlateSlotId InId) const
{
	return _nodeSet.FindByPredicate([InId](const FGsMonsterKnowledgeSlateNode& InData)
		{
			return InData.Id() == InId;
		}
	);
}

TSet<const FGsMonsterKnowledgeSlateNode*> FGsMonsterKnowledgeSlate::ChainNodeSet(const FGsMonsterKnowledgeSlateNode* InData) const
{
	TSet<const FGsMonsterKnowledgeSlateNode*> outData;

	const FGsMonsterKnowledgeSlateNode* goalNode = BeginNode();
	if (nullptr == InData || nullptr == goalNode)
	{	// 2023/3/3 PKT - Error
		return outData;
	}

	TFunction<bool(const FGsMonsterKnowledgeSlateNode*)> ChainNodes = [&](const FGsMonsterKnowledgeSlateNode* InNode) -> bool
	{
		if (INVALID_MONSTER_KNOWLEDGE_SLATE_SLOT_ID != InNode->PrevNodeId())
		{
			const auto& data = this->FindNodeById(InNode->PrevNodeId());
			if (ChainNodes(data))
			{
				outData.Emplace(data);
			}
			else
			{
				return false;
			}
		}
		else if (goalNode->Id() != InNode->Id())
		{
			return false;
		}

		return true;
	};

	if (ChainNodes(InData))
	{
		outData.Emplace(InData);
	}
	
	return outData;
}

ItemId FGsMonsterKnowledgeSlate::DisplayCostItemId() const
{
	if (nullptr == _displayOnlyCostItem)
	{
		_displayOnlyCostItem = FindDisplayItemData();
	}

	return (nullptr != _displayOnlyCostItem) ? _displayOnlyCostItem->id : INVALID_ITEM_ID;
}

bool FGsMonsterKnowledgeSlate::IsCostItem(const ItemId InItemId) const
{
	return _openCostItem->HasCostElement(InItemId);
}

int32 FGsMonsterKnowledgeSlate::HaveCandinateCostAmount() const
{
	//UGsItemManager* itemMgr = GItem();
	//if (nullptr == itemMgr)
	//	return 0;

	int32 count = 0;
	const int32 maxElementCount = _openCostItem->GetCostElementsCount();
	for (int32 idx = 0; idx < maxElementCount; ++idx)
	{
		const FGsCostElement* elementAt = _openCostItem->GetCostElementAt(idx);
		if (nullptr == elementAt)
		{
			continue;
		}

	// C2URWQ-7459 | ejrrb10 | 잠금된 아이템은 갯수 체크 X
		//if (itemMgr->IsItemLockByTId(elementAt->GetItemId()))
		//	continue;

		count += elementAt->PlayerHasAmountMax();
	}

	return count;	
}

const TArray<TWeakPtr<FGsItem>> FGsMonsterKnowledgeSlate::HaveCandinateCostItems() const
{
	TArray<TWeakPtr<FGsItem>> temp;

	return temp;
}

TArray<PassivityId> FGsMonsterKnowledgeSlate::ActivePassivtySet() const
{
	TArray<PassivityId> outSet;

	for (const auto& item : _nodeSet)
	{
		if (EMonsterStoneSlateNodeState::Complete == item.State())
		{
			outSet.Emplace(item.Passivity());
		}
	}

	return outSet;
}

int32 FGsMonsterKnowledgeSlate::GetSlateSpecialNodeCount() const
{
	return _specialNodeList.Num();
}

const FGsSlateSpecialNode* FGsMonsterKnowledgeSlate::FindSpecialNodeByIndex(int32 InIndex) const
{
	return (_specialNodeList.IsValidIndex(InIndex)) ? &_specialNodeList[InIndex] : nullptr;
}

const FGsSlateSpecialNode* FGsMonsterKnowledgeSlate::FindSpecialNodeByNodeId(SlateSpecialNodeId InId) const
{
	return _specialNodeList.FindByPredicate([InId](const FGsSlateSpecialNode& InData)
		{
			return InData.GetId() == InId;
		}
	);
}

FGsMonsterKnowledgeSlateNode* FGsMonsterKnowledgeSlate::FindNode(MonsterKnowledgeSlateSlotId InId)
{
	for (int32 idx = 0; idx < _nodeSet.Num(); ++idx)
	{
		if (_nodeSet[idx].Id() == InId)
		{
			return &(_nodeSet[idx]);
		}
	}

	return nullptr;
}

TSharedRef<const FGsCostPackage> FGsMonsterKnowledgeSlate::MakeCostPackage(TArray<ItemId> InCostItemIds)
{
	TSharedRef<FGsCostPackage> CostPackage = MakeShared<FGsCostPackage>();

	CostPackage->SetJointCostCandidateElements(true);

	const static int32 UNLOCK_ITEM_AMOUNT = 1;
	CostPackage->AddCostCandidates(InCostItemIds, UNLOCK_ITEM_AMOUNT);

	return CostPackage;
}

const FGsSchemaItemCommon* FGsMonsterKnowledgeSlate::FindDisplayItemData() const
{
	const int32 maxElementCount = _openCostItem->GetCostElementsCount();
	for (int32 idx = 0; idx < maxElementCount; ++idx)
	{
		const FGsCostElement* elementAt = _openCostItem->GetCostElementAt(idx);
		if (nullptr == elementAt)
		{
			continue;
		}

		ItemId displayItemId = UGsItemManager::GetFirstItemIdInGroup(elementAt->GetItemId());
		const FGsSchemaItemCommon* findItemData = GItem()->GetItemTableDataByTID(displayItemId);

		if (nullptr != findItemData)
		{
			return findItemData;
		}
	}
	return nullptr;
}