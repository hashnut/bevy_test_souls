

#include "GsPersonItemCollectionState.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "DataSchema/ItemCollection/GsSchemaItemCollection.h"
#include "DataSchema/Quest/Main/GsSchemaQuestMainChapter.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Quest/Table/GsQuestMainTableCacheSet.h"
#include "Item/Collection/EItemCollectionSubCategoryAttainmentUnit.h"

FGsPersonItemCollectionState::FGsPersonItemCollectionState(const FGsSchemaItemCollection* InSchemaItemCollection)
	: FGsItemCollectionBase(FCString::Atoi(*InSchemaItemCollection->GetRowName().ToString()), InSchemaItemCollection->name)
	, _itemCollectionData(InSchemaItemCollection), _subCategoryAttainmentUnit(_itemCollectionData->AttainmentUnit.GetRow()->type)
	, _isUnLock(true), _cachedLockReson(FText::GetEmpty())
{
	// 2021/09/27 PKT - 버프 효과
	BuildStatInfo(_itemCollectionData->passivityIds);

	// 2021/09/27 PKT - 유효기간
	MakeTimeOfValidity(InSchemaItemCollection->startDateTime, InSchemaItemCollection->endDateTime);

	// 2021/09/27 PKT - InnerSlot 정보
	SlotComponentDataList innerSlotItems;
	MakeItemCollectionInnerSlot(_itemCollectionData, &innerSlotItems);	
	// 2021/09/27 PKT - build Collection Items
	
	CreateCollectionInnerSlotItems(innerSlotItems);
}

void FGsPersonItemCollectionState::ForceToReset(bool InIsBookNMakrReset /*= true*/)
{
	Super::ForceToReset(InIsBookNMakrReset);

	_isUnLock = true;
}

void FGsPersonItemCollectionState::ForceToCompleted()
{
	Super::ForceToCompleted();

	_isUnLock = true;
}

void FGsPersonItemCollectionState::UpdateByActiveState()
{
	if (true == _isUnLock)
	{	// 2021/09/27 PKT - 현재 활성화 상태라면 빠져 나감.
		return;
	}

	bool nextUnLockState = true;

	//do
	//{
	//	ItemCollectionUnlockType unLockType = _itemCollectionData->unlockType;
	//	int32 unLockValue = _itemCollectionData->unlockValue;
	//	if (ItemCollectionUnlockType::NONE == unLockType)
	//	{
	//		GSLOG(Warning, TEXT("ItemCollectionUnlockType::NONE == condition.Key"));
	//		break;;
	//	}

	//	switch (unLockType)
	//	{
	//		case ItemCollectionUnlockType::LEVEL:
	//		{
	//			const FGsGameDataManager* GameData = GGameData();
	//			const FGsNetUserData* UserData = (nullptr != GameData) ? GameData->GetUserData() : nullptr;
	//			if (nullptr == UserData)
	//			{
	//				GSLOG(Error, TEXT("nullptr == UserData"));
	//				return;
	//			}
	//
	//			if (unLockValue > UserData->mLevel)
	//			{
	//				if (_cachedLockReson.IsEmpty())
	//				{
	//					FText outText;
	//					FText::FindText(TEXT("CollectionText"), TEXT("UnLock_Condition_Level"), outText);
	//					_cachedLockReson = FText::Format(outText, { unLockValue });
	//				}
	//
	//				nextUnLockState = false;
	//				break;
	//			}
	//		} break;
	//		case ItemCollectionUnlockType::QUEST:
	//		{
	//			const UGsQuestManager* QuestManager = GSQuest();
	//			check(QuestManager);
	//
	//			FGsQuestMainTableCacheSet* QuestMainData = QuestManager->GetQuestMainTableCache();
	//			if (nullptr == QuestMainData)
	//			{
	//				// 어떤 상태인지 알 수 없음.
	//				GSLOG(Error, TEXT("nullptr == QuestMainData"));
	//				break;
	//			}
	//
	//			int32 storyIndex = QuestMainData->GetStoryIndex();
	//			TSharedPtr<FGsQuestStoryTableCache> storyTableCache = QuestMainData->GetQuestStoryTableCache(storyIndex);
	//			if (unLockValue >= storyTableCache->GetSchemaQuestStory()->id)
	//			{
	//				TArray<const FGsSchemaQuestMainChapter*> questChapterList;
	//				if (const UGsTable* table = FGsSchemaQuestMainChapter::GetStaticTable())
	//				{
	//					table->GetAllRows<FGsSchemaQuestMainChapter>(questChapterList);
	//				}
	//
	//				if (0 >= questChapterList.Num())
	//				{
	//					GSLOG(Warning, TEXT("0 >= questChapterList.Num()"));
	//					break;
	//				}
	//
	//				for (const FGsSchemaQuestMainChapter* mainChapter : questChapterList)
	//				{
	//					for (const FGsSchemaQuestMainStoryRow& itemRow : mainChapter->questStoryList)
	//					{
	//						if (const FGsSchemaQuestMainStory* schemaStoryItem = itemRow.GetRow())
	//						{
	//							if (unLockValue == schemaStoryItem->id)
	//							{
	//								FText outText;
	//								FText::FindText(TEXT("CollectionText"), TEXT("UnLock_Condition_Quest"), outText);
	//								_cachedLockReson = FText::Format(outText, { schemaStoryItem->storyTitleText });
	//
	//								break;
	//							}
	//						}
	//					}
	//				}
	//
	//				nextUnLockState = false;
	//				break;
	//			}
	//		}break;
	//	}

	//} while (0);

	if (_isUnLock != nextUnLockState)
	{	// 2021/08/06 PKT - Lock이 UnLock되었으면
		_isUnLock = nextUnLockState;
		
		// 2021/09/27 PKT - ProPerty 변경
		UpdateByState(ECollectionStateChangeType::EActive);

		// 2021/09/27 PKT - 잠김 해제에 대한 RedDot은 한번 보고 꺼야 하기 때문에 Reload를 활성화 한다.
		ActiveVolatilityRedDot();
	}
}

bool FGsPersonItemCollectionState::IsActive() const
{
	return _isUnLock;
}


bool FGsPersonItemCollectionState::IsEventItemCollection() const
{
	return (ItemCollectionEventType::EVENT == _itemCollectionData->eventType);
}

const FText& FGsPersonItemCollectionState::InActiveMessage() const 
{
	if (true == _isUnLock)
	{
		FText::GetEmpty();
	}

	return _cachedLockReson;
}

EItemCollectionSubCategoryAttainmentUnit FGsPersonItemCollectionState::GetSubCategoryAttainmentUnit() const
{
	return _subCategoryAttainmentUnit;
}


void FGsPersonItemCollectionState::MakeItemCollectionInnerSlot(const FGsSchemaItemCollection* InSchemaItemCollection, OUT SlotComponentDataList* OutInnerSlots)
{
	(*OutInnerSlots).Empty();

	static const FString SLOT_ITEM_CONDITION_ID_NAME = FString(GET_MEMBER_NAME_STRING_CHECKED(FGsSchemaItemCollection, slotItemConditionId0)).LeftChop(1);
	static const FString SLOT_ITEM_IDS_NAME = FString(GET_MEMBER_NAME_STRING_CHECKED(FGsSchemaItemCollection, slotItemIds0)).LeftChop(1);
	static const FString SLOT_ITEM_LEVELS_NAME = FString(GET_MEMBER_NAME_STRING_CHECKED(FGsSchemaItemCollection, slotItemLevels0)).LeftChop(1);

	check(InSchemaItemCollection);

	const UScriptStruct* Struct = TBaseStructure<FGsSchemaItemCollection>().Get();
	check(Struct);

	TArray<FGsItemCollectionSlotState> ItemSlotStates;
	ItemSlotStates.Reset(InSchemaItemCollection->slotCount);

	const void* ContainerPtr = reinterpret_cast<const void*>(InSchemaItemCollection);

	for (int32 i = 0; i < InSchemaItemCollection->slotCount; i++)
	{
		// item collection condition id
		CollectionConditionId SlotItemConditionId = INVALID_COLLECTION_CONDITION_ID;
		{
			FString SlotItemConditionIdName = SLOT_ITEM_CONDITION_ID_NAME;
			SlotItemConditionIdName.AppendInt(i);

			const FProperty* SlotItemConditionIdProperty = Struct->FindPropertyByName(*SlotItemConditionIdName);
			if (nullptr == SlotItemConditionIdProperty)
			{
				continue;
			}

			const CollectionConditionId* SlotItemConditionIdPtr = SlotItemConditionIdProperty->ContainerPtrToValuePtr<CollectionConditionId>(ContainerPtr);
			if (nullptr != SlotItemConditionIdPtr)
			{
				SlotItemConditionId = *SlotItemConditionIdPtr;
			}
		}

		if (INVALID_COLLECTION_CONDITION_ID == SlotItemConditionId)
		{
			continue;
		}

		TArray<ItemId> SlotItemIds;
		{
			FString SlotItemIdsName = SLOT_ITEM_IDS_NAME;
			SlotItemIdsName.AppendInt(i);

			const FProperty* SlotItemIdsProperty = Struct->FindPropertyByName(*SlotItemIdsName);
			if (nullptr == SlotItemIdsProperty)
			{
				continue;
			}

			FScriptArrayHelper SlotItemIdsHelper(CastField<FArrayProperty>(SlotItemIdsProperty), SlotItemIdsProperty->ContainerPtrToValuePtr<void>(ContainerPtr));
			for (int32 j = 0; j < SlotItemIdsHelper.Num(); ++j)
			{
				const ItemId* SlotItemIdPtr = reinterpret_cast<const ItemId*>(SlotItemIdsHelper.GetRawPtr(j));
				if (nullptr != SlotItemIdPtr)
				{
					SlotItemIds.Add(*SlotItemIdPtr);
				}
			}
		}

		TArray<Level> SlotItemLevels;
		{
			FString SlotItemLevelsName = SLOT_ITEM_LEVELS_NAME;
			SlotItemLevelsName.AppendInt(i);

			const FProperty* SlotItemLevelsProperty = Struct->FindPropertyByName(*SlotItemLevelsName);
			if (nullptr == SlotItemLevelsProperty)
			{
				continue;
			}

			FScriptArrayHelper SlotItemLevelsHelper(CastField<FArrayProperty>(SlotItemLevelsProperty), SlotItemLevelsProperty->ContainerPtrToValuePtr<void>(ContainerPtr));
			for (int32 j = 0; j < SlotItemLevelsHelper.Num(); ++j)
			{
				const Level* SlotItemLevelPtr = reinterpret_cast<const Level*>(SlotItemLevelsHelper.GetRawPtr(j));
				if (nullptr != SlotItemLevelPtr)
				{
					SlotItemLevels.Add(*SlotItemLevelPtr);
				}
			}
		}

		const int32 count = FMath::Min(SlotItemIds.Num(), SlotItemLevels.Num());

		ParserSlotComponentData slotData;
		slotData.collectionConditionId = SlotItemConditionId;
		for (int32 idx = 0; count > idx; ++idx)
		{
			slotData.materialItemSet.Emplace(FGsItemCollectionSlotState::MaterialData(SlotItemIds[idx], SlotItemLevels[idx]));
		}		
		// 2021/10/14 PKT - 개인 아이템 콜렉션은 Slot마다의 보상이 없음.
		slotData.rewardId = -1;

		(*OutInnerSlots).Emplace(slotData);
	}
}