
#include "GsMonsterKnowledgeSlateState.h"
#include "GsMonsterKnowledgeSlateNodeState.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeSlate.h"
#include "MonsterKnowledgeCollection/EMonsterKnowledgeSlateCategory.h"
#include "../T1Project/T1Project.h"


FGsMonsterKnowledgeSlateState::FGsMonsterKnowledgeSlateState(const FGsSchemaMonsterKnowledgeSlate* InData
	, TArray<FGsMonsterKnowledgeSlateNodeState*>& InNodeList, ItemId InUnLockItem, const FSoftObjectPath& InImagePath)
	: _slateData(InData)
	, _beginNode(MakeLinkkNode(InData, InNodeList))
	, _nodeList(InNodeList)
	, _State(EMonsterKnowledgeSlateState::Lock)
	, _imagePath(InImagePath)
	, _costPackage(MakeCostPackage(InUnLockItem))
{
	// 2021/12/13 PKT - 재귀 함수를 통해 트리 생성
	int32 depth = 0;
	int32 count = 0;
	RecursionCalcDepthTree(_beginNode, nullptr, depth, count);

	// 2021/12/13 PKT - Passivity
	for (const FGsMonsterKnowledgeSlateNodeState* nodeData : _nodeList)
	{
		if (false == _passivitySet.Contains(nodeData->GetPassivityId()))
		{
			_passivitySet.Emplace(nodeData->GetPassivityId());
		}
	}

	_passivitySet.Sort();

	// 2021/12/13 PKT - check have item
	bIsHaveItem = (0 < HaveItemAmount());
}

const FGsMonsterKnowledgeSlateNodeState* FGsMonsterKnowledgeSlateState::MakeLinkkNode(const FGsSchemaMonsterKnowledgeSlate* InData
	, TArray<FGsMonsterKnowledgeSlateNodeState*>& InNodeList)
{
//	if (nullptr == InData->beginNode.GetRow())
//	{
//		GSLOG(Error, TEXT("nullptr == InData->beginNode.GetRow() [%d]"), InData->id);
//		return nullptr;
//	}
//
//	auto findNodeState = [InNodeList](int32 InId) -> FGsMonsterKnowledgeSlateNodeState*
//	{
//		for (FGsMonsterKnowledgeSlateNodeState* state : InNodeList)
//		{
//			if (state->GetId() == InId)
//			{
//				return state;
//			}
//		}
//
//		return nullptr;
//	};
//
//	const TArray<FGsSchemaMonsterKnowledgeSlateStatNodeRow>& linkNodeRowList = InData->linkNodeList;
//	for (FGsSchemaMonsterKnowledgeSlateStatNodeRow nodeRow : linkNodeRowList)
//	{
//		const FGsSchemaMonsterKnowledgeSlateStatNode* nodeData = nodeRow.GetRow();		
//		FGsMonsterKnowledgeSlateNodeState* nodeState = findNodeState(nodeData->id);
//		if (nullptr == nodeState)
//		{
//			GSLOG(Error, TEXT("nullptr == nodeState"));
//			continue;
//		}
//
//		if (MonsterKnowledgeSlateSlotDir::NONE == nodeData->linkNodeDir || MonsterKnowledgeSlateSlotDir::MAX == nodeData->linkNodeDir)
//		{
//			continue;
//		}
//
//		FGsMonsterKnowledgeSlateNodeState* linkNodeState = findNodeState(nodeData->linkNodeId);
//		if (nullptr == linkNodeState)
//		{
//			continue;
//		}
//
//		nodeState->MakeLinkNode(nodeData->linkNodeDir, linkNodeState);
//	}
//	
	return nullptr;
}

void FGsMonsterKnowledgeSlateState::RecursionCalcDepthTree(const FGsMonsterKnowledgeSlateNodeState* InNode
	, const FGsMonsterKnowledgeSlateNodeState* InPreNode
	, OUT int32& InDepth
	, OUT int32& InCount)
{
	if (InCount >= _nodeList.Num())
	{
		return;
	}
	
	// 2021/12/13 PKT - depth.. (라는 의미보다 Floor / Layer ?? )
	_depthNodeData.FindOrAdd(InDepth).AddUnique(InNode);

	++InCount;

	//for (const auto& pair : InNode->GetLinker())
	//{
	//	int32 depth = InDepth;
	//	if (MonsterKnowledgeSlateSlotDir::BOTTOM == pair.Key)
	//	{
	//		--depth;
	//	}
	//	else if (MonsterKnowledgeSlateSlotDir::TOP == pair.Key)
	//	{
	//		++depth;
	//	}

	//	if (InPreNode != pair.Value)
	//	{	// 2021/12/12 PKT - 한번 간곳은 가지 않는다.
	//		RecursionCalcDepthTree(pair.Value, InNode, depth, InCount);
	//	}
	//}
}

TSharedRef<const FGsCostPackage> FGsMonsterKnowledgeSlateState::MakeCostPackage(const ItemId InUnLockItemId)
{
	TSharedRef<FGsCostPackage> CostPackage = MakeShared<FGsCostPackage>();
	CostPackage->SetJointCostCandidateElements(true);

	const static int32 UNLOCK_ITEM_AMOUNT = 1;
	CostPackage->AddCost(InUnLockItemId, UNLOCK_ITEM_AMOUNT);

#if WITH_EDITOR
	if (0 == CostPackage->GetCostElementsCount())
	{
		GSLOG(Error, TEXT("0 == CostPackage->GetCostElementsCount() Invalid UnLock Item TId : [%d]"), InUnLockItemId);
	}
#endif

	return CostPackage;
}

void FGsMonsterKnowledgeSlateState::CheckHaveUnLockItem()
{
	if (EMonsterKnowledgeSlateState::UnLock == GetState())
	{	
		return;
	}

	bool preIsHaveItem = bIsHaveItem;
	bIsHaveItem = (0 < HaveItemAmount());

	if (preIsHaveItem != bIsHaveItem)
	{	
		OnDelegateSlateState.ExecuteIfBound(this, EMonsterKnowledgeSlateChangedState::MaterialItem);
	}
}

void FGsMonsterKnowledgeSlateState::SetStateChanged(EMonsterKnowledgeSlateState InNextState, bool InIsDelegate /* = true */)
{
	if (_State != InNextState)
	{
		_State = InNextState;
		
		if (EMonsterKnowledgeSlateState::UnLock == _State)
		{
			for (FGsMonsterKnowledgeSlateNodeState* node : _nodeList)
			{	// 2021/12/13 PKT - unlock
				node->SetStateChanged(EMonsterStoneSlateNodeState::Normal, false);
			}
		}
		if (InIsDelegate)
		{
			OnDelegateSlateState.ExecuteIfBound(this, EMonsterKnowledgeSlateChangedState::StateChanged);
		}		
	}
}

void FGsMonsterKnowledgeSlateState::SetForceNodeState(const TArray<MonsterKnowledgeSlateSlotId>& InActiveNodeList, bool InIsDelegate /* = true */)
{
	for (FGsMonsterKnowledgeSlateNodeState* node : _nodeList)
	{		
		if (EMonsterKnowledgeSlateState::UnLock == _State)
		{
			if (InActiveNodeList.Contains(node->GetId()))
			{
				node->SetStateChanged(EMonsterStoneSlateNodeState::Complete, InIsDelegate);
			}
			else
			{
				node->SetStateChanged(EMonsterStoneSlateNodeState::Normal, InIsDelegate);
			}
		}		
		else
		{
			node->SetStateChanged(EMonsterStoneSlateNodeState::Disable, InIsDelegate);
		}
	}
}


EMonsterKnowledgeSlateState FGsMonsterKnowledgeSlateState::GetState() const
{
	return _State;
}

EMonsterKnowledgeSlateCategory FGsMonsterKnowledgeSlateState::GetCategoryType() const
{
	return _slateData->categoryType;
}

MonsterKnowledgeSlateId FGsMonsterKnowledgeSlateState::GetId() const
{
	return StaticCast<MonsterKnowledgeSlateId>(_slateData->id);
}

const FText& FGsMonsterKnowledgeSlateState::GetTitle() const
{
	return _slateData->name;
}

ItemId FGsMonsterKnowledgeSlateState::GetUnLockItemId() const
{
	const FGsCostElement* elementAt = _costPackage->GetCostElementAt(0);
	if (nullptr == elementAt)
	{
		return INVALID_ITEM_ID;
	}
	return elementAt->GetItemId();
}

const FSoftObjectPath& FGsMonsterKnowledgeSlateState::GetImagePath() const
{
	return _imagePath;
}

MonsterKnowledgeSlateGrade FGsMonsterKnowledgeSlateState::GetGrade() const
{
	return _slateData->grade;
}

const TArray<PassivityId>& FGsMonsterKnowledgeSlateState::GetPassivitySet() const
{
	return _passivitySet;
}

bool FGsMonsterKnowledgeSlateState::IsInclusionStat(StatType InType) const
{
	for (const FGsMonsterKnowledgeSlateNodeState* node : _nodeList)
	{
		if (nullptr != node->GetStatList().FindByPredicate([InType](const TPair<StatType, StatValue>& InData)
			{
				return InType == InData.Key;
			}
		))
		{
			return true;
		}
	}

	return false;
}

int32 FGsMonsterKnowledgeSlateState::GetDepthCount() const
{
	return _depthNodeData.Num();
}

const TArray<const FGsMonsterKnowledgeSlateNodeState*>* FGsMonsterKnowledgeSlateState::GetNodeListByDepth(int32 InDepth) const
{
	if (false == _depthNodeData.Contains(InDepth))
	{
		GSLOG(Error, TEXT("false == _depthNodeData.Contains(InDepth) [%d]"), InDepth);
		return nullptr;
	}
	return &(_depthNodeData[InDepth]);
}

const FGsMonsterKnowledgeSlateNodeState* FGsMonsterKnowledgeSlateState::GetBeginNode() const
{
	return _beginNode;
}

const TArray<FGsMonsterKnowledgeSlateNodeState*>& FGsMonsterKnowledgeSlateState::GetNodeListAll() const
{
	return _nodeList;
}

TSharedRef<const FGsCostPackage> FGsMonsterKnowledgeSlateState::GetCostPackage() const
{
	return _costPackage;
}

ItemId FGsMonsterKnowledgeSlateState::GetOpenItemId() const
{
	const FGsCostElement* elementAt = _costPackage->GetCostElementAt(0);
	if (nullptr == elementAt)
	{
		GSLOG(Error, TEXT("nullptr == elementAt"));
		return INVALID_ITEM_ID;
	}

	return elementAt->GetItemId();
}


int32 FGsMonsterKnowledgeSlateState::HaveItemAmount() const
{
	const FGsCostElement* elementAt = _costPackage->GetCostElementAt(0);
	if (nullptr == elementAt)
	{
		GSLOG(Error, TEXT("nullptr == elementAt"));
		return 0;
	}
	return elementAt->PlayerHasAmountMax();
}