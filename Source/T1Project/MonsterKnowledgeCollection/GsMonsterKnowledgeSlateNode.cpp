

#include "GsMonsterKnowledgeSlateNode.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeSlateStatNode.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "T1Project.h"




FGsMonsterKnowledgeSlateNode::FGsMonsterKnowledgeSlateNode(const FGsSchemaMonsterKnowledgeSlateStatNode* InData)
	: _schemaData(InData), _state(EMonsterStoneSlateNodeState::Disable), _effectStatSet(BuildEffectStat(InData->passivitySetId)), _prevNodeData(nullptr)
{

}

void FGsMonsterKnowledgeSlateNode::Initialized(const FGsMonsterKnowledgeSlateNode* InPrevNodeData)
{
	_prevNodeData = InPrevNodeData;

	_state = EMonsterStoneSlateNodeState::Max;
}

void FGsMonsterKnowledgeSlateNode::ChangeState(EMonsterStoneSlateNodeState InState)
{
	_state = InState;
}

EMonsterStoneSlateNodeState FGsMonsterKnowledgeSlateNode::State() const
{
	return _state;
}

MonsterKnowledgeSlateSlotId FGsMonsterKnowledgeSlateNode::Id() const
{
	return StaticCast<MonsterKnowledgeSlateSlotId>(_schemaData->id);
}

int32 FGsMonsterKnowledgeSlateNode::ResourceId() const
{
	return _schemaData->resourceId;
}

const FString& FGsMonsterKnowledgeSlateNode::Name() const
{
	return _schemaData->name;
}

const FSoftObjectPath& FGsMonsterKnowledgeSlateNode::ImagePath() const
{
	return _schemaData->imagePath;
}

Currency FGsMonsterKnowledgeSlateNode::AmountOfCurrency() const
{
	return _schemaData->openCost;
}

PassivityId FGsMonsterKnowledgeSlateNode::Passivity() const
{
	return _schemaData->passivitySetId;
}

const TArray<TPair<StatType, StatValue>>& FGsMonsterKnowledgeSlateNode::StatSet() const
{
	return _effectStatSet;
}

MonsterKnowledgeSlateSlotId FGsMonsterKnowledgeSlateNode::PrevNodeId() const
{
	return StaticCast<MonsterKnowledgeSlateSlotId>(_schemaData->prevNodeId);
}

const FGsMonsterKnowledgeSlateNode* FGsMonsterKnowledgeSlateNode::PrevNodeData() const
{
	return _prevNodeData;
}

TArray<TPair<StatType, StatValue>> FGsMonsterKnowledgeSlateNode::BuildEffectStat(PassivityId InPassivityId) const
{
	TArray<TPair<StatType, StatValue>> effectStatSet;

	do 
	{
		if (INVALID_PASSIVITY_ID == InPassivityId)
		{
			break;
		}

		const UGsTablePassivitySet* table = Cast<UGsTablePassivitySet>(FGsSchemaPassivitySet::GetStaticTable());
		if (nullptr == table)
		{
			GSLOG(Error, TEXT("nullptr == table"));
			break;
		}

		const FGsSchemaPassivitySet* passivityData = nullptr;
		if (false == table->FindRowById(InPassivityId, passivityData))
		{
			GSLOG(Error, TEXT("false == table->FindRowById(InPassivityId, passivityData) : [%d]"), InPassivityId);
			break;
		}


		FGsEffectStringHelper::FGsEffectAddData effectAddData;
		if (false == FGsEffectStringHelper::AddPassivityEffect(passivityData, effectAddData))
		{
			GSLOG(Error, TEXT("false == FGsEffectStringHelper::AddPassivityEffect(passivityData, effectAddData) [%d]"), InPassivityId);
			break;
		}

		effectStatSet.Reserve(effectAddData._statMap.Num());

		for (const auto& pair : effectAddData._statMap)
		{
			effectStatSet.Emplace(TPair<StatType, StatValue>(pair.Key, StaticCast<StatValue>(pair.Value)));
		}

	} while (0);

	if (1 < effectStatSet.Num())
	{
		// 2023/1/25 PKT - 한개 이상이면 정렬
		effectStatSet.Sort([](const TPair<StatType, StatValue>& lhr, const TPair<StatType, StatValue>& rhr)
			{
				return (lhr.Key < rhr.Key); 
			}
		);
	}

	return effectStatSet;
}

