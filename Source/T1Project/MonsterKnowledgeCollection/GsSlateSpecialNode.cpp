#include "GsSlateSpecialNode.h"

#include "DataSchema/MonsterKnowledgeCollection/GsSchemaSlateSpecialNode.h"
#include "Currency/GsCostPackage.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Algo/Transform.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "../UI/UIContent/Helper/GsEffectStringHelper.h"

FGsSlateSpecialNode::FGsSlateSpecialNode(const FGsSchemaSlateSpecialNode* InSchema) :
    _schema(InSchema),	
    _requireCost(MakeShared<FGsCostPackage>(ECachedValueItemDelegateType::CONSUMABLE_INVEN_DEPOT))
{
    check(nullptr != _schema);

	if (InSchema->passivityId.GetRow())
	{
		_effectStatSet = BuildEffectStat(InSchema->passivityId.GetRow()->id);
	}
	

    _requireCost->SetJointCostCandidateElements(true);

    const FGsSchemaItemCommon* itemCommon = _schema->requireItemId.GetRow();
    if (nullptr == itemCommon)
        return;

    ItemId itemId = itemCommon->id;

    UGsItemManager* itemManager = GItem();
    if (nullptr == itemManager)
        return;

    TArray<ItemId> itemIdList;
    Algo::Transform(itemManager->GetAllItemIdsTheSameGroup(itemId), itemIdList, [](auto itemId) {
        return itemId;
        });
    _requireCost->AddCostCandidates(itemIdList, _schema->requireItemAmount);
}

void FGsSlateSpecialNode::SetState(EMonsterStoneSlateNodeState InState)
{
    _state = InState;
}

SlateSpecialNodeId FGsSlateSpecialNode::GetId() const
{
    return _schema->id;
}

int32 FGsSlateSpecialNode::GetProbability() const
{
    return _schema->probability;
}

TSharedRef<FGsCostPackage> FGsSlateSpecialNode::GetCostPackage() const
{
    return _requireCost;
}

PassivityId FGsSlateSpecialNode::GetPassivityId() const
{
    if (_schema->passivityId.GetRow())
        return _schema->passivityId.GetRow()->id;
    return INVALID_PASSIVITY_ID;
}

const TPair<StatType, StatValue>& FGsSlateSpecialNode::StatSet() const
{
	return _effectStatSet;
}

const FSoftObjectPath& FGsSlateSpecialNode::GetNodeSpritePath() const
{
    return _schema->nodeSprite;
}

const FSoftObjectPath& FGsSlateSpecialNode::GetNodeSpriteActivePath() const
{
    return _schema->nodeActiveSprite;
}

EMonsterStoneSlateNodeState FGsSlateSpecialNode::GetState() const
{
    return _state;
}

const FGsSchemaSlateSpecialNode* FGsSlateSpecialNode::GetSchemaData() const
{
    return _schema;
}

TPair<StatType, StatValue> FGsSlateSpecialNode::BuildEffectStat(PassivityId InPassivityId) const
{
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

		for (const auto& pair : effectAddData._statMap)
		{
			return TPair<StatType, StatValue>(pair.Key, StaticCast<StatValue>(pair.Value));
		}

	} while (0);

	return TPair<StatType, StatValue>(StatType::NONE, 0);
}