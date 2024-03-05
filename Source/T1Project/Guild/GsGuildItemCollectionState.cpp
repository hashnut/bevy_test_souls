

#include "GsGuildItemCollectionState.h"
#include "T1Project/T1Project.h"
#include "Public/GsTableManager.h"
#include "DataSchema/Guild/GsSchemaGuildItemCollection.h"
#include "DataSchema/Guild/GsSchemaGuildItemCollectionInnerSlotProperty.h"
#include "DataSchema/Guild/GsSchemaGuildItemCollectionRandomTable.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "Misc/AssertionMacros.h"


FGsGuildItemCollectionState::FGsGuildItemCollectionState(const FGsSchemaGuildItemCollection* InSchemaItemCollection)
	: FGsItemCollectionBase(FCString::Atoi(*InSchemaItemCollection->GetRowName().ToString())
		, InSchemaItemCollection->name
		, InSchemaItemCollection->enableTimeSetStart
		, InSchemaItemCollection->enableTimeSetEnd
		, InSchemaItemCollection->completeRewardPassivityList)
	, schemaItemCollection(InSchemaItemCollection)
{
	
}

void FGsGuildItemCollectionState::CreateSlotList(const TArray<GuildCollectionCondition>& InGuildCollectionConditionList)
{

	TArray<const FGsSchemaGuildItemCollectionInnerSlotProperty*> slotPropertyList;
	
	if (false == ParserSlotPropert(InGuildCollectionConditionList.Num(), &slotPropertyList))
	{
		GSLOG(Error, TEXT("false == ParserSlotPropert(InComponentData.Num(), &slotPropertyList)"));
		return;
	}

	if (InGuildCollectionConditionList.Num() != slotPropertyList.Num())
	{
		GSLOG(Error, TEXT("InComponentData.Num() != slotPropertyList.Num() : InComponentData[%d], slotPropertyList[%d]")
			, InGuildCollectionConditionList.Num(), slotPropertyList.Num());
		return;
	}

	/**
	 * Slot 정보 만들기.
	 */
	SlotComponentDataList slotComponentDataList;
	slotComponentDataList.Reserve(InGuildCollectionConditionList.Num());
	
	for (int32 idx = 0; idx < InGuildCollectionConditionList.Num(); ++idx)
	{
		ParserSlotComponentData SlotData;

		SlotData.collectionConditionId = InGuildCollectionConditionList[idx].mCollectionConditionId;
		SlotData.rewardId = slotPropertyList[idx]->rewardId;

		if (false == ParserAvaildItem(InGuildCollectionConditionList[idx].mCollectionRandomDataId, &SlotData.materialItemSet))
		{
			GSLOG(Error, TEXT("false == ParserAvaildItem() : CollectionId[%d] CollectionConditionId[%d]"), GetCollectionId()
				, InGuildCollectionConditionList[idx].mCollectionConditionId);
			return;
		}
		
		slotComponentDataList.Emplace(SlotData);
	}

	CreateCollectionInnerSlotItems(slotComponentDataList);

}

bool FGsGuildItemCollectionState::ParserSlotPropert(const int32 InMaxCount
	, OUT TArray<const FGsSchemaGuildItemCollectionInnerSlotProperty*>* OutSlotPropertyList)
{
	static const FString SLOT_ITEM_PROPERTY_ROW_NAME = FString(GET_MEMBER_NAME_STRING_CHECKED(FGsSchemaGuildItemCollection, slotProperty0)).LeftChop(1);

	const UScriptStruct* Struct = TBaseStructure<FGsSchemaGuildItemCollection>().Get();
	if (nullptr == Struct)
	{
		GSLOG(Error, TEXT("nullptr == Struct"));
		return false;
	}

	if (nullptr == schemaItemCollection)
	{
		GSLOG(Error, TEXT("nullptr == schemaItemCollection"));
		return false;
	}

	const void* ContainerPtr = reinterpret_cast<const void*>(schemaItemCollection);

	for (int32 idx = 0; idx < InMaxCount; idx++)
	{
		const FGsSchemaGuildItemCollectionInnerSlotProperty* propertData = nullptr;
		{
			FString SlotPropertyRowName = SLOT_ITEM_PROPERTY_ROW_NAME;
			SlotPropertyRowName.AppendInt(idx);

			const FProperty* SlotPropertyDataProperty = Struct->FindPropertyByName(*SlotPropertyRowName);
			if (nullptr == SlotPropertyDataProperty)
			{
				GSLOG(Error, TEXT("nullptr == SlotPropertyDataProperty : Collection Id[%d], SlotIndex[%d]"), GetCollectionId(), idx);
				continue;
			}
			
			const FGsSchemaGuildItemCollectionInnerSlotPropertyRow* slotPropertyRow
				= SlotPropertyDataProperty->ContainerPtrToValuePtr<FGsSchemaGuildItemCollectionInnerSlotPropertyRow>(ContainerPtr);
			if (nullptr != slotPropertyRow)
			{
				propertData = (*slotPropertyRow).GetRow();
			}
		}

		if (nullptr == propertData)
		{
			GSLOG(Error, TEXT("nullptr == propertData : Collection Id[%d], SlotIndex[%d]"), GetCollectionId(), idx);
			return false;
		}

		(*OutSlotPropertyList).Emplace(propertData);
	}

	return true;
}


bool FGsGuildItemCollectionState::ParserAvaildItem(CollectionRandomDataId InRandomTableKey, OUT TArray<TPair<ItemId, Level>>* OutMaterialItemSet)
{
	const UGsTableManager& tableManager = UGsTableManager::GetInstance();

	const UGsTableGuildItemCollectionRandomTable* table
		= Cast<UGsTableGuildItemCollectionRandomTable>(tableManager.GetTable(TBaseStructure<FGsSchemaGuildItemCollectionRandomTable>::Get()));

	if (nullptr == table)
	{
		GSLOG(Error, TEXT("nullptr == table"));
		return false;
	}
	
	const FGsSchemaGuildItemCollectionRandomTable* availdItemData;
	if (false == table->FindRow<FGsSchemaGuildItemCollectionRandomTable>(FName(*FString::Format(TEXT("{0}"), { InRandomTableKey })), availdItemData))
	{
		GSLOG(Error, TEXT("false == table->FindRow"));
		return false;
	}

	// 2021/10/08 PKT - 대체 아이템 + 메인 아이템
	(*OutMaterialItemSet).Empty(availdItemData->subSituteItemData.Num() + 1);
	// 2021/10/08 PKT - Add Main Item
	(*OutMaterialItemSet).Emplace(TPair<ItemId, Level>(availdItemData->mainItemId, availdItemData->mainEnchantLevel));

	const int32 MaxSubSituteItemCount = availdItemData->subSituteItemData.Num();
	// 2021/10/08 PKT - Add SubSitute Item
	for (int idx = 0; idx < MaxSubSituteItemCount; ++idx)
	{
		(*OutMaterialItemSet).Emplace(
			TPair<ItemId, Level>(availdItemData->subSituteItemData[idx].itemId, availdItemData->subSituteItemData[idx].enchantLevel)
		);
	}
	
	return true;
}


GuildItemCollectionType FGsGuildItemCollectionState::GetCollectionType() const
{
	return schemaItemCollection->type;
}

void FGsGuildItemCollectionState::RegisterCollection(const TPair<CollectionConditionId, const FText>& InRegisterInfo)
{
	for (FGsItemCollectionSlotState& innerSlot : _innerSlots)
	{
		if (innerSlot.GetItemCollectionConditionId() == InRegisterInfo.Key)
		{	// 2021/09/30 PKT - 완료 처리
			innerSlot.UpdateByCompleted(InRegisterInfo.Value);
		}
	}
}

bool FGsGuildItemCollectionState::IsHighlightBackGround() const
{
	return (GuildItemCollectionType::EVENT_SHARING == schemaItemCollection->type); 
}