

#include "GsMonsterKnowledgeSlateNodeState.h"
#include "../T1Project/T1Project.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeSlateStatNode.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "T1Project/T1Project.h"
#include "MonsterKnowledgeCollection/EMonsterStoneSlateTreeNodeCoordinate.h"


FGsMonsterKnowledgeSlateNodeState::FGsMonsterKnowledgeSlateNodeState(const FGsSchemaMonsterKnowledgeSlateStatNode* InData)
	: _statNodeData(InData), _currentState(EMonsterStoneSlateNodeState::Disable)
{
	// 2021/11/09 PKT - 초기화
	_linker.Empty();

	MakeStatDataList(_statNodeData->passivitySetId, _statDataList);
}

void FGsMonsterKnowledgeSlateNodeState::MakeStatDataList(PassivityId InPassivityId, OUT TArray<TPair<StatType, StatValue>>& OutData)
{
	OutData.Empty();

	if (INVALID_PASSIVITY_ID == InPassivityId)
	{	
		return;
	}

	const UGsTableManager& tableManager = UGsTableManager::GetInstance();

	const UGsTablePassivitySet* passivitySetTable
		= Cast<UGsTablePassivitySet>(tableManager.GetTable(TBaseStructure<FGsSchemaPassivitySet>::Get()));
	if (nullptr == passivitySetTable)
	{
		GSLOG(Error, TEXT("nullptr == passivitySetTable"));
		return;
	}
	
	const FGsSchemaPassivitySet* passivitySetData = nullptr;
	if (false == passivitySetTable->FindRowById(InPassivityId, passivitySetData))
	{
		GSLOG(Error, TEXT("false == passivitySetTable->FindRowById(InPassivityId, passivitySetData)"));
		return;
	}
	
	FGsEffectStringHelper::FGsEffectAddData effectAddData;
	if (false == FGsEffectStringHelper::AddPassivityEffect(passivitySetData, effectAddData))
	{
		GSLOG(Error, TEXT("false == FGsEffectStringHelper::AddPassivityEffect(passivitySetData, effectAddData) [%d]"), passivitySetData->id);
		return;
	}

	_statDataList.Empty(effectAddData._statMap.Num());	
	for (const auto& pair : effectAddData._statMap)
	{
		_statDataList.Emplace(TPair<StatType, StatValue>(pair.Key, StaticCast<StatValue>(pair.Value)));
	}

	_statDataList.Sort([](const TPair<StatType, StatValue>& lhr, const TPair<StatType, StatValue>& rhr)
		{
			return (lhr.Key < rhr.Key);
		}
	);
}

void FGsMonsterKnowledgeSlateNodeState::MakeLinkNode(MonsterKnowledgeSlateSlotDir InDirection
														, FGsMonsterKnowledgeSlateNodeState* InNode) const
{
	if (nullptr == InNode || InDirection == MonsterKnowledgeSlateSlotDir::MAX || InDirection == MonsterKnowledgeSlateSlotDir::NONE)
	{
		GSLOG(Error, TEXT("invalid link node info [%d]"), this->GetId());
		return;
	}
		
	if (InNode == this)
	{	// 2021/11/08 PKT - 링크 타겟이 자신이면 안된다.
		GSLOG(Error, TEXT("InNode == this"));
		return;
	}

	if (true == _linker.Contains(InDirection))
	{
		return;
	}

	// 2021/12/13 PKT - link..
	_linker.FindOrAdd(InDirection) = InNode;


	MonsterKnowledgeSlateSlotDir reverseDirection = (InDirection == MonsterKnowledgeSlateSlotDir::LEFT) ? MonsterKnowledgeSlateSlotDir::RIGHT
		: (InDirection == MonsterKnowledgeSlateSlotDir::RIGHT) ? MonsterKnowledgeSlateSlotDir::LEFT
		: (InDirection == MonsterKnowledgeSlateSlotDir::TOP) ? MonsterKnowledgeSlateSlotDir::BOTTOM : MonsterKnowledgeSlateSlotDir::TOP;

	// 2021/12/12 PKT - const 를 잠시 풀어준다.
	InNode->MakeLinkNode(reverseDirection, const_cast<FGsMonsterKnowledgeSlateNodeState*>(this));
}


void FGsMonsterKnowledgeSlateNodeState::SetStateChanged(EMonsterStoneSlateNodeState InNextState, bool InOnDelegate /*= true*/)
{
	// 2021/12/13 PKT - changed state only
	if (_currentState != InNextState)
	{	
		_currentState = InNextState;
		
		if (InOnDelegate)
		{
			OnDelegateSlateNodeState.ExecuteIfBound(this);
		}		
	}
}


EMonsterStoneSlateNodeState FGsMonsterKnowledgeSlateNodeState::GetCurrentState() const
{
	return _currentState;
}

MonsterKnowledgeSlateSlotId FGsMonsterKnowledgeSlateNodeState::GetId() const
{
	return StaticCast<MonsterKnowledgeSlateSlotId>(_statNodeData->id);
}

const FString& FGsMonsterKnowledgeSlateNodeState::GetName() const
{
	return _statNodeData->name;
}

const FSoftObjectPath& FGsMonsterKnowledgeSlateNodeState::GetImagePath() const
{
	return _statNodeData->imagePath;
}

EMonsterStoneSlateTreeNodeCoordinate FGsMonsterKnowledgeSlateNodeState::GetCoordinate() const
{
	return EMonsterStoneSlateTreeNodeCoordinate::Center/*_statNodeData->cocoordinate*/;
}

Currency FGsMonsterKnowledgeSlateNodeState::GetOpenCost() const
{
	return StaticCast<Currency>(_statNodeData->openCost);
}

PassivityId FGsMonsterKnowledgeSlateNodeState::GetPassivityId() const
{
	return _statNodeData->passivitySetId;
}

const TArray<TPair<StatType, StatValue>>& FGsMonsterKnowledgeSlateNodeState::GetStatList() const
{
	return _statDataList;
}

const FGsMonsterKnowledgeSlateNodeState* FGsMonsterKnowledgeSlateNodeState::FindLinkNodeByDirection(MonsterKnowledgeSlateSlotDir InDirection) const
{
	return (true == _linker.Contains(InDirection)) ? _linker[InDirection] : nullptr;
}

const TMap<MonsterKnowledgeSlateSlotDir, const FGsMonsterKnowledgeSlateNodeState*>& FGsMonsterKnowledgeSlateNodeState::GetLinker() const
{
	return _linker;
}