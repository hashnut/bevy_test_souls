

#include "GsItemCollectionState.h"

#include "EItemCollectionSlotState.h"
#include "EItemCollectionState.h"
#include "GsItemCollectionSlotState.h"
#include "GsTableManager.h"
#include "Item/GsItem.h"
#include "ItemCollection/GsSchemaItemCollection.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Quest/Table/GsQuestMainTableCacheSet.h"
#include "GsTable.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Skill/Passivity/GsSchemaPassivitySet.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "DataSchema/Quest/Main/GsSchemaQuestMainChapter.h"
#include "ItemCollection/GsSchemaItemCollectionSubCategoryAttainmentUnit.h"
#include "ItemCollection/GsSchemaItemCollectionSubCategoryStatUnit.h"
#include "Item/Collection/EItemCollectionSubCategoryAttainmentUnit.h"

FGsItemCollectionState::FGsItemCollectionState(const FGsSchemaItemCollection* ItemCollectionData)
	: TGsState<EItemCollectionState>(EItemCollectionState::Default)
	, _itemCollectionData(ItemCollectionData)
	, _collectionId(FCString::Atoi(*_itemCollectionData->GetRowName().ToString()))
	, _periodDateTimeUtc(FGsItemCollectionState::ParseDateTime(_itemCollectionData))
	, _innerSlotStates(MakeItemCollectionItemSlotStates(this, _itemCollectionData))
	, _statInfos(MakeStatInfos(_itemCollectionData->passivityIds))
	, _isActivatedBookMark(false)
{
	check(_itemCollectionData);
	check(0 < _innerSlotStates.Num());

	// 2021/07/29 PKT - 카테고리 타입 설정
	const FGsSchemaItemCollectionSubCategoryAttainmentUnit* subAttainmentUnit 
		= _itemCollectionData->AttainmentUnit.GetRow();
	if (subAttainmentUnit)
	{
		_subCategoryAttainmentUnit = subAttainmentUnit->type;
	}

	// 2021/08/02 PKT - 캐쉬 Data 초기화
	_cachedLockReson = FText::GetEmpty();
	_cachedStatEffectText = FText::GetEmpty();

	_isActivatedBookMark = false;
	_isUnLock = true;

	// 2021/08/02 PKT - 이벤트 함수 Bind
	for (FGsItemCollectionSlotState& innerSlot : _innerSlotStates)
	{
		//innerSlot.OnCollectionInnnerSlotChangeState.BindRaw(this, &FGsItemCollectionState::OnResultByInnerSlotUpdate);
	}
}

void FGsItemCollectionState::Reset()
{
	for (FGsItemCollectionSlotState& innerSlot : _innerSlotStates)
	{
		innerSlot.Reset();
	}

	_cachedLockReson = FText::GetEmpty();
	_cachedStatEffectText = MakeStatEffectText(_itemCollectionData->passivityIds);

	_isActivatedBookMark = false;
	_isUnLock = true;

	UpdateSmallestSlotState();
	UpdateByLockState();
	UpdateByType(EItemCollectionState::Default);
}

void FGsItemCollectionState::SetCollectionCompleted()
{
	for (FGsItemCollectionSlotState& innerSlot : _innerSlotStates)
	{
		innerSlot.UpdateByCompleted();
	}

	UpdateByType(EItemCollectionState::Completed);
}

void FGsItemCollectionState::SetInnerSlotCompleated(const CollectionConditionId InIds)
{
	for (FGsItemCollectionSlotState& innerSlot : _innerSlotStates)
	{
		if (InIds == innerSlot.GetItemCollectionConditionId())
		{
			innerSlot.UpdateByCompleted();
		}
	}

	EItemCollectionState nextState = EItemCollectionState::Progressing;
	if (EItemCollectionSlotState::Completed == GetTheSmallestSlotState())
	{
		nextState = EItemCollectionState::Completed;
	}

	UpdateByType(nextState);
}

void FGsItemCollectionState::UpdateByNewItem(const FGsItem* InItem)
{
	for (FGsItemCollectionSlotState& innerSlot : _innerSlotStates)
	{
		if (EItemCollectionSlotState::Completed != innerSlot.GetType())
		{
			innerSlot.UpdateByNewItem(InItem);
		}
	}
}

void FGsItemCollectionState::UpdateByUpdateItem(const FGsItem* InItem)
{
	for (FGsItemCollectionSlotState& innerSlot : _innerSlotStates)
	{
		if (EItemCollectionSlotState::Completed != innerSlot.GetType())
		{
			innerSlot.UpdateByUpdateItem(InItem);
		}
	}
}

void FGsItemCollectionState::UpdateByRemoveItem(ItemId InItemId, ItemDBId InItemDBId)
{
	for (FGsItemCollectionSlotState& innerSlot : _innerSlotStates)
	{
		if (EItemCollectionSlotState::Completed != innerSlot.GetType())
		{
			innerSlot.UpdateByRemoveItem(InItemId, InItemDBId);
		}
	}
}

void FGsItemCollectionState::UpdateByLockState()
{
	if (true == _isUnLock)
	{
		return;
	}

	bool nextUnLockState = true;
//
//	do 
//	{
//		TPair<ItemCollectionUnlockType, int32> condition = GetUnLockCondition();
//		if (ItemCollectionUnlockType::NONE == condition.Key)
//		{
//#if WITH_EDITOR
//			GSLOG(Warning, TEXT("ItemCollectionUnlockType::NONE == condition.Key"));
//#endif
//			break;
//		}
//
//		switch (condition.Key)
//		{
//			case ItemCollectionUnlockType::LEVEL:
//			{
//				const FGsGameDataManager* GameData = GGameData();
//				const FGsNetUserData* UserData = (nullptr != GameData) ? GameData->GetUserData() : nullptr;
//				if (nullptr == UserData)
//				{
//					GSLOG(Error, TEXT("nullptr == UserData"));
//					return;
//				}
//
//				if (condition.Value > UserData->mLevel)
//				{
//					if (_cachedLockReson.IsEmpty())
//					{
//						FText outText;
//						FText::FindText(TEXT("CollectionText"), TEXT("UnLock_Condition_Level"), outText);
//						_cachedLockReson = FText::Format(outText, { condition.Value });
//					}
//
//					nextUnLockState = false;
//					break;
//				}
//			} break;
//			case ItemCollectionUnlockType::QUEST:
//			{
//				const UGsQuestManager* QuestManager = GSQuest();
//				check(QuestManager);
//
//				FGsQuestMainTableCacheSet* QuestMainData = QuestManager->GetQuestMainTableCache();
//				if (nullptr == QuestMainData)
//				{
//					// 어떤 상태인지 알 수 없음.
//					GSLOG(Error, TEXT("nullptr == QuestMainData"));
//					break;
//				}
//
//				int32 storyIndex = QuestMainData->GetStoryIndex();
//				TSharedPtr<FGsQuestStoryTableCache> storyTableCache = QuestMainData->GetQuestStoryTableCache(storyIndex);
//				if (condition.Value >= storyTableCache->GetSchemaQuestStory()->id)
//				{				
//					TArray<const FGsSchemaQuestMainChapter*> questChapterList;
//					if (const UGsTable* table = FGsSchemaQuestMainChapter::GetStaticTable())
//					{
//						table->GetAllRows<FGsSchemaQuestMainChapter>(questChapterList);
//					}
//
//					if (0 >= questChapterList.Num())
//					{
//						GSLOG(Warning, TEXT("0 >= questChapterList.Num()"));
//						break;
//					}
//
//					for (const FGsSchemaQuestMainChapter* mainChapter : questChapterList)
//					{
//						for (const FGsSchemaQuestMainStoryRow& itemRow : mainChapter->questStoryList)
//						{
//							if (const FGsSchemaQuestMainStory* schemaStoryItem = itemRow.GetRow())
//							{
//								if (condition.Value == schemaStoryItem->id)
//								{
//									FText outText;
//									FText::FindText(TEXT("CollectionText"), TEXT("UnLock_Condition_Quest"), outText);
//									_cachedLockReson = FText::Format(outText, { schemaStoryItem->storyTitleText });
//
//									break;
//								}
//							}
//						}
//					}
//								
//					nextUnLockState = false;
//					break;
//				}
//			}break;
//		}
//
//	} while (0);

	if (_isUnLock != nextUnLockState)
	{	// 2021/08/06 PKT - Lock이 UnLock되었으면
		_isUnLock = nextUnLockState;
		UpdateByProperty();
	}
}

void FGsItemCollectionState::SetActivatedBookMark(bool InFlag)
{
	_isActivatedBookMark = InFlag;
}

const FString& FGsItemCollectionState::GetCachedAllItemNamesForSearch() const
{
	if (_cachedAllItemNamesForSearch.IsEmpty())
	{
		TArray<FText> ItemNames;

		for (const FGsItemCollectionSlotState& slotState : _innerSlotStates)
		{
			ItemNames.Emplace(slotState.GetMaterialItemNames());
		}

		_cachedAllItemNamesForSearch = FText::Join(FText::FromString(TEXT("\n")), ItemNames).ToString();
	}

	return _cachedAllItemNamesForSearch;
}

//TPair<ItemCollectionUnlockType, int32> FGsItemCollectionState::GetUnLockCondition() const
//{
//	TPair<ItemCollectionUnlockType, int32> outResult;
//	outResult.Key = _itemCollectionData->unlockType;
//	outResult.Value = _itemCollectionData->unlockValue;
//	return outResult;
//}

const FText& FGsItemCollectionState::GetItemNameByTID(ItemId InItemId) const
{
	const UGsTableItemCommon* Table = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	if (nullptr == Table)
	{
		return FText::GetEmpty();
	}

	const FGsSchemaItemCommon* Row = nullptr;
	if (!Table->FindRowById(InItemId, Row))
	{
		return FText::GetEmpty();
	}

	return Row->name;
}

void FGsItemCollectionState::UpdateSmallestSlotState()
{
	_smallestInnerSlotState = EItemCollectionSlotState::Completed;
	for (const FGsItemCollectionSlotState& innerSlot : _innerSlotStates)
	{
		_smallestInnerSlotState = (_smallestInnerSlotState < innerSlot.GetType()) ? _smallestInnerSlotState : innerSlot.GetType();
	}
}

void FGsItemCollectionState::OnResultByInnerSlotUpdate(const FGsItemCollectionSlotState* InInnerSlot 
	, bool InIsMatchCondition 
	, ItemDBId InItemDBId)
{
	
	EItemCollectionSlotState prevSmallestState = GetTheSmallestSlotState();
	UpdateSmallestSlotState();
	EItemCollectionSlotState currentSmallestState = GetTheSmallestSlotState();

	// 2021/08/03 PKT - ItemCollection으로 이벤트를 날린다.
	OnUpdateByItemCollection.ExecuteIfBound(this, prevSmallestState, InIsMatchCondition, InItemDBId);
}

void FGsItemCollectionState::UpdateByProperty()
{
	// 2021/08/03 PKT - ItemCollection으로 이벤트를 날린다.
	OnUpdateByItemCollection.ExecuteIfBound(this, GetTheSmallestSlotState(), false, INVALID_ITEM_DB_ID);
}

void FGsItemCollectionState::UpdateByType(EItemCollectionState InState)
{
	SetType(InState);

	EItemCollectionSlotState prevSmallestState = GetTheSmallestSlotState();
	UpdateSmallestSlotState();
	EItemCollectionSlotState currentSmallestState = GetTheSmallestSlotState();

	// 2021/08/03 PKT - ItemCollection으로 이벤트를 날린다.
	OnUpdateByItemCollection.ExecuteIfBound(this, prevSmallestState, false, INVALID_ITEM_DB_ID);
}

CollectionId FGsItemCollectionState::GetCollectionId() const
{
	return _collectionId;
}

EItemCollectionSubCategoryAttainmentUnit FGsItemCollectionState::GetSubCategoryAttainmentUnit() const
{
	return _subCategoryAttainmentUnit;
}

int32 FGsItemCollectionState::GetInnerSlotCount() const
{
	return _innerSlotStates.Num();
}

const FGsItemCollectionSlotState* FGsItemCollectionState::GetInnerSlotByIndex(const int32 InIndex) const
{
	return (_innerSlotStates.IsValidIndex(InIndex)) ? &(_innerSlotStates[InIndex]) : nullptr;
}

const FGsItemCollectionSlotState* FGsItemCollectionState::GetInnerSlotByCollectionConditionId(const CollectionConditionId InId) const
{
	int32 index = _innerSlotStates.IndexOfByPredicate([InId](const FGsItemCollectionSlotState& InInnerSlot)
		{
			return (InInnerSlot.GetItemCollectionConditionId() == InId);
		}
	);

	return GetInnerSlotByIndex(index);
}

FText FGsItemCollectionState::GetNameText() const
{
	return _itemCollectionData->name;
}

FText FGsItemCollectionState::GetStatEffectText() const
{		
	return _cachedStatEffectText;
}

FText FGsItemCollectionState::GetDateTimeText() const
{
	if (_periodDateTimeUtc.IsEmpty())
	{
		return FText::GetEmpty();
	}

	const FString DateTimeStr = FString::Printf(TEXT("~ %s"), *_periodDateTimeUtc.GetUpperBoundValue().ToString());
	return FText::FromString(*DateTimeStr);
}

const TArray<StatInfo>& FGsItemCollectionState::GetAllStatInfos() const
{
	return _statInfos;
}

const TArray<int32>& FGsItemCollectionState::GetPassivityId() const
{
	return _itemCollectionData->passivityIds;
}

EItemCollectionSlotState FGsItemCollectionState::GetTheSmallestSlotState() const
{
	return _smallestInnerSlotState;
}

bool FGsItemCollectionState::SearchByItemName(const FString& InItemName) const
{
	FString names = GetCachedAllItemNamesForSearch();
	names.ReplaceInline(TEXT(" "), TEXT(""));
	return names.Contains(InItemName);
}

bool FGsItemCollectionState::IsInPeriod(const FDateTime& _dateTimeUtc) const
{
	if (_periodDateTimeUtc.IsEmpty())
	{
		return true;
	}

	return _periodDateTimeUtc.Contains(_dateTimeUtc);
}

bool FGsItemCollectionState::CanCollectAnyInnerSlot(const FGsItem* InItem) const
{
	if (GetType() == EItemCollectionState::Completed)
	{
		return false;
	}

	for (const FGsItemCollectionSlotState& innerSlot : _innerSlotStates)
	{
		if (true == innerSlot.IsCanCollect(InItem))
		{
			return true;
		}
	}

	return false;
}

bool FGsItemCollectionState::IsEventItemCollection() const
{
	return (ItemCollectionEventType::EVENT == _itemCollectionData->eventType);
}

bool FGsItemCollectionState::IsActivatedBookMark() const
{
	return _isActivatedBookMark;
}

bool FGsItemCollectionState::IsUnLock() const
{
	return _isUnLock;
}

FText FGsItemCollectionState::ReasonLockMessage() const
{
	return _cachedLockReson;
}

FDateRange FGsItemCollectionState::ParseDateTime(const FGsSchemaItemCollection* ItemCollectionData)
{
	FDateRange outReturn = FDateRange::Empty();

	do
	{	// 2021/07/07 PKT - 명시적으로 기간제 컬렉션인지 확인.
		/*if (ItemCollectionType::FIXED_TERM != ItemCollectionData->durationType)
		{
			break;
		}*/

		const FString& lowerDateTimeStr = ItemCollectionData->startDateTime;
		const FString& upperDateTimeStr = ItemCollectionData->endDateTime;
		// 2021/07/07 PKT - 기간제 컬렉션에는 기간이 기입되어 있어야 한다.
		if (lowerDateTimeStr.IsEmpty() || upperDateTimeStr.IsEmpty())
		{
			GSLOG(Error, TEXT("FGsItemCollectionState::ParseDateTime : invaild durationType"));
			break;
		}

		FDateTime LowerDateTimeUtc;
		if (!FDateTime::Parse(*lowerDateTimeStr, LowerDateTimeUtc))
		{
			GSLOG(Error, TEXT("!FDateTime::Parse(lowerDateTimeStr, LowerDateTimeUtc), InLowerDateTimeStr:%s"), *lowerDateTimeStr);
			break;
		}

		FDateTime UpperDateTimeUtc;
		if (!FDateTime::Parse(*upperDateTimeStr, UpperDateTimeUtc))
		{
			GSLOG(Error, TEXT("!FDateTime::Parse(*upperDateTimeStr, UpperDateTimeUtc), InUpperDateTimeStr:%s"), *upperDateTimeStr);
			break;
		}

		outReturn = FDateRange(LowerDateTimeUtc, UpperDateTimeUtc);

	} while (0);

	return outReturn;
}

TArray<FGsItemCollectionSlotState> FGsItemCollectionState::MakeItemCollectionItemSlotStates(
	const FGsItemCollectionState* InItemCollection, const FGsSchemaItemCollection* InItemCollectionData)
{
	static const FString SLOT_ITEM_CONDITION_ID_NAME = FString(GET_MEMBER_NAME_STRING_CHECKED(FGsSchemaItemCollection, slotItemConditionId0)).LeftChop(1);
	static const FString SLOT_ITEM_IDS_NAME = FString(GET_MEMBER_NAME_STRING_CHECKED(FGsSchemaItemCollection, slotItemIds0)).LeftChop(1);
	static const FString SLOT_ITEM_LEVELS_NAME = FString(GET_MEMBER_NAME_STRING_CHECKED(FGsSchemaItemCollection, slotItemLevels0)).LeftChop(1);

	check(InItemCollectionData);

	const UScriptStruct* Struct = TBaseStructure<FGsSchemaItemCollection>().Get();
	check(Struct);

	TArray<FGsItemCollectionSlotState> ItemSlotStates;
	ItemSlotStates.Reset(InItemCollectionData->slotCount);

	const void* ContainerPtr = reinterpret_cast<const void*>(InItemCollectionData);

	for (int32 i = 0; i < InItemCollectionData->slotCount; i++)
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

		FGsItemCollectionSlotState::MsterialSet materialSet;
		materialSet.Reserve(count);

		for (int32 idx = 0; count > idx; ++idx)
		{
			materialSet.Emplace(FGsItemCollectionSlotState::MaterialData(SlotItemIds[idx], SlotItemLevels[idx]));
		};

		ItemSlotStates.Emplace(SlotItemConditionId, materialSet);
	}

	return ItemSlotStates;
}

TArray<StatInfo> FGsItemCollectionState::MakeStatInfos(const TArray<int32>& InPassivityIds)
{
	TArray<StatInfo> StatInfos;

	const UGsTableManager& TableManager = UGsTableManager::GetInstance();

	const UGsTablePassivitySet* PassivitySetTable = Cast<UGsTablePassivitySet>(TableManager.GetTable(TBaseStructure<FGsSchemaPassivitySet>().Get()));
	if (nullptr == PassivitySetTable)
	{
		GSLOG(Error, TEXT("nullptr == PassivitySetTable"));
		return StatInfos;
	}

	for (const PassivityId PassivityIdIt : InPassivityIds)
	{
		if (INVALID_PASSIVITY_ID == PassivityIdIt)
		{
			continue;
		}

		const FGsSchemaPassivitySet* PassivitySet = nullptr;
		if (!PassivitySetTable->FindRowById(PassivityIdIt, PassivitySet) || nullptr == PassivitySet)
		{
			GSLOG(Warning, TEXT("!PassivitySetTable->FindRowById(PassivityIdIt, PassivitySet) || nullptr == PassivitySet, PassivityIdIt:%d"), PassivityIdIt);
			continue;
		}

		FGsEffectStringHelper::FGsEffectAddData EffectAddData;
		if (!FGsEffectStringHelper::AddPassivityEffect(PassivitySet, EffectAddData))
		{
			if (0 < PassivitySet->effectInfoList.Num())
			{
				GSLOG(Error, TEXT("!FGsEffectStringHelper::AddPassivityEffect(PassivitySet, EffectAddData), id:%d, type:%d")
					, PassivitySet->id, PassivitySet->type);
			}
			continue;
		}

		for (auto& StatTypeAndValue : EffectAddData._statMap)
		{
			StatInfos.Emplace(StatTypeAndValue.Key, StatTypeAndValue.Value);
		}
	}

	return StatInfos;
}

FText FGsItemCollectionState::MakeStatEffectText(const TArray<int32>& PassivityIds)
{
	FString AllStatEffectString;
	FString StatEffectString;
	for (const PassivityId PassivityIdIt : PassivityIds)
	{
		if (INVALID_PASSIVITY_ID == PassivityIdIt)
		{
			continue;
		}

		if (!FGsEffectStringHelper::GetPassivityEffectString(PassivityIdIt, StatEffectString))
		{
			GSLOG(Error, TEXT("!FGsEffectStringHelper::GetPassivityEffectString(%d, StatEffectString)"), PassivityIdIt);
			continue;
		}

		AllStatEffectString.Append(StatEffectString);
	}

	return FText::FromString(AllStatEffectString);
}