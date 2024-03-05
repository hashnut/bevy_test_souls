


#include "GsItemCollectionSlotState.h"
#include "GsItemCollectionBase.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Item/Collection/EItemCollectionEnumData.h"
#include "Currency/GsCostPackageMultiple.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsValidatorChainManager.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "T1Project/T1Project.h"


FGsItemCollectionSlotState::FGsItemCollectionSlotState(CollectionConditionId InCollectionConditionId, const MsterialSet& InMaterialSet)
	: TGsState<EItemCollectionSlotState>(EItemCollectionSlotState::Impossible)
	, _itemCollectionConditionId(InCollectionConditionId)
	, _immediatePossibleCostPackage(MakeCostPackage(InMaterialSet, true))
	, _conditionalPossibleCostPackage(MakeCostPackage(InMaterialSet, false))
	, _cachedMaterialItemName(FText::GetEmpty()), _rewardData(nullptr), _textBuffer(FText::GetEmpty())
{
	if (INVALID_COLLECTION_CONDITION_ID == _itemCollectionConditionId)
	{
		GSLOG(Error, TEXT("INVALID_COLLECTION_CONDITION_ID == _itemCollectionConditionId"));
	}
		
}


FGsItemCollectionSlotState::FGsItemCollectionSlotState(CollectionConditionId InCollectionConditionId
	, const MsterialSet& InMaterialSet, int32 InrewardId)
	: TGsState<EItemCollectionSlotState>(EItemCollectionSlotState::Impossible)
	, _itemCollectionConditionId(InCollectionConditionId)
	, _immediatePossibleCostPackage(MakeCostPackage(InMaterialSet, true))
	, _conditionalPossibleCostPackage(MakeCostPackage(InMaterialSet, false))
	, _cachedMaterialItemName(FText::GetEmpty()), _rewardData(nullptr), _textBuffer(FText::GetEmpty())
{
	if (INVALID_COLLECTION_CONDITION_ID == _itemCollectionConditionId)
	{
		GSLOG(Error, TEXT("INVALID_COLLECTION_CONDITION_ID == _itemCollectionConditionId"));
	}

	MakeRewardData(InrewardId);
}


TSharedRef<const FGsCostPackage> FGsItemCollectionSlotState::MakeCostPackage(const MsterialSet& InMaterialSet, bool IsImmediatePossible)
{
	// 2022/03/10 PKT - 즉시 등록 가능 타입 "ECachedValueItemDelegateType::CONSUMABLE_NOW_WITH_DEPOT" - 인벤토리 and 창고 같이 검사
	//					조건부 등록 가능 타입 "ECachedValueItemDelegateType::CONSUMABLE_NOW" - 인벤토리 '만' 검사
	TSharedRef<FGsCostPackage> costPackage 
		= MakeShared<FGsCostPackage>(IsImmediatePossible ? ECachedValueItemDelegateType::CONSUMABLE_INVEN_DEPOT : ECachedValueItemDelegateType::CONSUMABLE_INVENTORY);

	costPackage->SetJointCostCandidateElements(true);

	const static int32 DEFAULT_CURRENCY_DELTA = 1;
	if (IsImmediatePossible)
	{
		costPackage->AddCostCandidates(InMaterialSet, DEFAULT_CURRENCY_DELTA);
	}
	else
	{
		// 2022/03/07 PKT - 조건부 등록은 Level을 모두 0으로 만들어 줘야 한다.
		// uka - ItemMinLevel 사용하지 않는 대체아이템 입력으로 변경함.
		TArray<ItemId> ItemIds;
		for (const MaterialData& matrerial : InMaterialSet)
		{
			ItemIds.Add(matrerial.Key);
		}
		costPackage->AddCostCandidates(ItemIds, DEFAULT_CURRENCY_DELTA);
	}

	if (0 >= costPackage->GetCostElementsCount())
	{
		GSLOG(Error, TEXT("0 >= costPackage->GetCostElementsCount()"));
	}

	return costPackage;
}

bool FGsItemCollectionSlotState::IsMaterialForItem(const FGsItem* InItem) const
{
	if (nullptr == InItem || EItemCollectionSlotState::Completed == GetType())
	{	// 2021/09/23 PKT - 아이템 정보가 없거나, 현재 슬롯이 완료 상태이거나..
		return false;
	}

	if (InItem->IsEquipped() || InItem->GetLock() || InItem->IsPresetEquipped())
	{	// 2022/03/03 PKT - 잠금 / Lock 상태 안됨.
		return false;
	}

	if (false == _conditionalPossibleCostPackage->HasCostElement(InItem->GetTID()))
	{	// 2022/03/03 PKT - 관련 없는 Item
		return false;
	}

	return true;
}

bool FGsItemCollectionSlotState::IsCanCollectForItem(const FGsItem* InItem) const
{
	if (false == IsMaterialForItem(InItem))
	{
		return false;
	}

	const int32 elementMaxCount = _immediatePossibleCostPackage->GetCostElementsCount();
	for (int32 elementIndex = 0; elementIndex < elementMaxCount; ++elementIndex)
	{
		const FGsCostElement* costElement = _immediatePossibleCostPackage->GetCostElementAt(elementIndex);
		if (nullptr == costElement)
		{
			GSLOG(Error, TEXT("nullptr == costElement"));
			continue;
		}

		const auto& costCandidateElementItems = costElement->GetAllCandidateItems();
		for (const auto& elementItem : costCandidateElementItems)
		{
			if (false == elementItem.IsValid())
			{
				continue;
			}

			if (elementItem.Pin()->IsPassedItem(*InItem))
			{
				return true;
			}
		}
	}

	return false;
}


EItemCollectionSlotState FGsItemCollectionSlotState::IsExpectedStateForItem(const FGsItem* InItem) const
{
	EItemCollectionSlotState expectedState = EItemCollectionSlotState::Impossible;

	do 
	{
		if (EItemCollectionSlotState::Completed == GetType())
		{
			expectedState = EItemCollectionSlotState::Completed;
			break;
		}

		if (false == IsMaterialForItem(InItem))
		{	// 2022/03/03 PKT - 재료 아이템에 속하지 않다면 등록 할 수 없는 Item
			expectedState = EItemCollectionSlotState::Impossible;
			break;
		}

		if (false == IsCanCollectForItem(InItem))
		{	// 2022/03/03 PKT - 즉시 등록 가능하지 않는 상태라면 조건부 등록 가능한 상태
			expectedState = EItemCollectionSlotState::ConditionalPossible;
			break;
		}

		expectedState = EItemCollectionSlotState::ImmediatePossible;

	} while (0);

	return expectedState;
}


void FGsItemCollectionSlotState::UpdateState(bool InIsCompleted /* = false */)
{
	EItemCollectionSlotState nextState = EItemCollectionSlotState::Completed;
	if (false == InIsCompleted)
	{
		if (true == _immediatePossibleCostPackage->PlayerHasEnough())
		{	// 2022/03/03 PKT - 즉시 등록 가능 상태
			nextState = EItemCollectionSlotState::ImmediatePossible;
		}
		else if (true == _conditionalPossibleCostPackage->PlayerHasEnough())
		{	// 2022/03/03 PKT - 조건부 등록 가능 상태
			nextState = EItemCollectionSlotState::ConditionalPossible;
		}
		else
		{	// 2022/03/03 PKT - 아무런 상태도 아님.
			nextState = EItemCollectionSlotState::Impossible;
		}
	}

	// 2022/03/03 PKT - 상태 전환 / 알림.
	SetType(nextState);
	OnCollectionInnnerSlotChangeState.ExecuteIfBound(ECollectionStateChangeType::ECollectionSlot);
}



void FGsItemCollectionSlotState::UpdateByNewItem(const FGsItem* InItem)
{
	EItemCollectionSlotState expectedState = IsExpectedStateForItem(InItem);

	if (EItemCollectionSlotState::Impossible == expectedState || EItemCollectionSlotState::Completed == expectedState)
	{	// 2022/03/03 PKT - 관련 없는 Item or 이미 완료 상태라면 아무것도 하지 않음.
		return;
	}

	UpdateState(false);
}


void FGsItemCollectionSlotState::UpdateByUpdateItem(const FGsItem* InItem)
{
	EItemCollectionSlotState expectedState = IsExpectedStateForItem(InItem);

	if (EItemCollectionSlotState::Completed == expectedState)
	{	// 2022/03/03 PKT - 이미 완료 상태라면 아무것도 하지 않음.
		return;
	}

	UpdateState(false);
}


void FGsItemCollectionSlotState::UpdateByRemoveItem(ItemId InItemId, ItemDBId InItemDBId)
{
	// 2021/09/23 PKT - 현재 아무런 상태가 아니라면 RemoveItem에 대해 대응 할 필요가 없음.
	if (EItemCollectionSlotState::Impossible == GetType() || EItemCollectionSlotState::Completed == GetType())
	{
		return;
	}

	if (false == _conditionalPossibleCostPackage->HasCostElement(InItemId))
	{	// 2022/03/03 PKT - 관련 없는 아이템
		return;
	}

	UpdateState(false);	
}


void FGsItemCollectionSlotState::UpdateByCompleted()
{
	if (EItemCollectionSlotState::Completed == GetType())
	{	// 2021/09/23 PKT - 이미 완료 상태라면 추가적으로 할게 없음.
		return;
	}

	UpdateState(true);
}

void FGsItemCollectionSlotState::UpdateByCompleted(const FText& InName)
{
	if (EItemCollectionSlotState::Completed == GetType())
	{	// 2021/09/23 PKT - 이미 완료 상태라면 추가적으로 할게 없음.
		return;
	}

	_textBuffer = InName;

	UpdateState(true);
}


void FGsItemCollectionSlotState::Reset()
{
	UpdateState(false);
}

CollectionConditionId FGsItemCollectionSlotState::GetItemCollectionConditionId() const
{
	return _itemCollectionConditionId;
}

const FGsItemCollectionSlotState::MaterialData FGsItemCollectionSlotState::GetMainMaterialData() const
{
	MaterialData outValue = MaterialData(INVALID_ITEM_ID, INVALID_LEVEL);

	const int32 elementMaxCount = _immediatePossibleCostPackage->GetCostElementsCount();
	for (int32 elementIndex = 0; elementIndex < elementMaxCount; ++elementIndex)
	{
		const FGsCostElement* costElement = _immediatePossibleCostPackage->GetCostElementAt(elementIndex);
		if (nullptr == costElement)
		{
			GSLOG(Error, TEXT("nullptr == costElement"));
			continue;
		}

		const static int32 MAIN_MATERIAL_INDEX = 0;
		const auto& costCandidateElementItems = costElement->GetAllCandidateItems();
		if (MAIN_MATERIAL_INDEX >= costCandidateElementItems.Num())
		{
			GSLOG(Error, TEXT("MAIN_MATERIAL_INDEX >= costCandidateElementItems.Num()"));
			continue;
		}

		const FGsCostElement::FGsCostElementItemLevel* costElementItemLevel
			= StaticCast<const FGsCostElement::FGsCostElementItemLevel*>(costCandidateElementItems[MAIN_MATERIAL_INDEX].Pin().Get());

		if (costElementItemLevel)
		{
			outValue = MaterialData(costElementItemLevel->_costItemId, costElementItemLevel->_costItemLevel); 
		}
	}

	return outValue;
}

const FGsItemCollectionSlotState::MsterialSet& FGsItemCollectionSlotState::GetMaterialDataSet() const
{
	if (0 >= _cachedMaterialDataSet.Num())	
	{
		_cachedMaterialDataSet.Empty();

		const int32 elementMaxCount = _immediatePossibleCostPackage->GetCostElementsCount();
		for (int32 elementIndex = 0; elementIndex < elementMaxCount; ++elementIndex)
		{
			const FGsCostElement* costElement = _immediatePossibleCostPackage->GetCostElementAt(elementIndex);
			if (nullptr == costElement)
			{
				GSLOG(Error, TEXT("nullptr == costElement"));
				continue;
			}

			const auto& costCandidateElementItems = costElement->GetAllCandidateItems();
			for (const auto& elementItem : costCandidateElementItems)
			{
				if (false == elementItem.IsValid())
				{
					continue;
				}

				const FGsCostElement::FGsCostElementItemLevel* costElementItemLevel
					= StaticCast<const FGsCostElement::FGsCostElementItemLevel*>(elementItem.Pin().Get());

				if (costElementItemLevel)
				{
					_cachedMaterialDataSet.Emplace(MaterialData(costElementItemLevel->_costItemId, costElementItemLevel->_costItemLevel));
				}
			}
		}
	}

	return _cachedMaterialDataSet;
}

const FText& FGsItemCollectionSlotState::GetMaterialItemNames() const
{
	if (_cachedMaterialItemName.IsEmpty())
	{	// 2021/10/01 PKT - 콜렉션 가능한 아이템 이름을 추출. 
		do 
		{
			TArray<FText> itemNames;

			const int32 elementMaxCount = _immediatePossibleCostPackage->GetCostElementsCount();
			for (int32 elementIndex = 0; elementIndex < elementMaxCount; ++elementIndex)
			{
				const FGsCostElement* costElement = _immediatePossibleCostPackage->GetCostElementAt(elementIndex);
				if (nullptr == costElement)
				{
					GSLOG(Error, TEXT("nullptr == costElement"));
					continue;
				}

				using CostElementItems = FGsCostElement::FGsCostElementItem;
				const TArray<TWeakPtr<const CostElementItems>>& elementItems = costElement->GetAllCandidateItems();
				for (const TWeakPtr<const CostElementItems>& item : elementItems)
				{
					if (item.IsValid())
					{
						ItemId findItemID = item.Pin().Get()->_costItemId;
						const FGsSchemaItemCommon* itemData = UGsItemManager::GetItemTableDataByTID(findItemID);
						if (itemData)
						{
							itemNames.Emplace(itemData->name);
						}
					}
				}
			}

			if (0 < itemNames.Num())
			{
				_cachedMaterialItemName = FText::Join(FText::FromString(TEXT("\n")), itemNames);
			}

		} while (0);
	}

	return _cachedMaterialItemName;
}


bool FGsItemCollectionSlotState::IsHaveReward() const
{
	return (nullptr != _rewardData);
}


const FGsSchemaRewardData* FGsItemCollectionSlotState::GetRewardData() const
{
	return _rewardData;
}

const FText& FGsItemCollectionSlotState::GetTextBuffer() const
{
	return _textBuffer;
}


bool FGsItemCollectionSlotState::IsCanCollect(const FGsItem* InItem) const
{
	return IsCanCollectForItem(InItem);
}

bool FGsItemCollectionSlotState::IsMaterial(const FGsItem* InItem) const
{
	return IsMaterialForItem(InItem);
}


void FGsItemCollectionSlotState::GetHasMaterialItemIdSet(TArray<TPair<EItemCollectionSlotState, MaterialData>>& OutMaterialSet) const
{
	if (EItemCollectionSlotState::Completed == GetType() || EItemCollectionSlotState::Impossible == GetType())
	{
		return;
	};

	using PairMaterialData = TPair<ItemDBId, TPair<EItemCollectionSlotState, MaterialData>>;
	TArray<PairMaterialData> MaterialDataSet;

	if (true == _immediatePossibleCostPackage->PlayerHasEnough())
	{
		TSet<TWeakPtr<FGsItem>> Items = _immediatePossibleCostPackage->GetAllPassedItems();
		for (TWeakPtr<FGsItem> Item : Items)
		{
			if (!Item.IsValid())
			{
				continue;
			}

			const FGsItem& CurrentItem = *Item.Pin().Get();

			PairMaterialData pairMaterialData;
			pairMaterialData.Key = CurrentItem.GetDBID();
			pairMaterialData.Value 
				= TPair<EItemCollectionSlotState, MaterialData>(EItemCollectionSlotState::ImmediatePossible, MaterialData(CurrentItem.GetTID(), CurrentItem.GetLevel()));

			MaterialDataSet.Emplace(pairMaterialData);
		}
	}

	if (true == _conditionalPossibleCostPackage->PlayerHasEnough())
	{
		TSet<TWeakPtr<FGsItem>> Items = _conditionalPossibleCostPackage->GetAllPassedItems();
		for (TWeakPtr<FGsItem> Item : Items)
		{
			if (!Item.IsValid())
			{
				continue;
			}

			const FGsItem& CurrentItem = *Item.Pin().Get();

			if (nullptr != MaterialDataSet.FindByPredicate([CurrentItem](const PairMaterialData& InData)
				{
					return (CurrentItem.GetDBID() == InData.Key);
				}
			))
			{
				continue;
			}

			PairMaterialData pairMaterialData;
			pairMaterialData.Key = CurrentItem.GetDBID();
			pairMaterialData.Value
				= TPair<EItemCollectionSlotState, MaterialData>(EItemCollectionSlotState::ConditionalPossible, MaterialData(CurrentItem.GetTID(), CurrentItem.GetLevel()));

			MaterialDataSet.Emplace(pairMaterialData);
		}
	}

	for (const auto& data : MaterialDataSet)
	{
		OutMaterialSet.Emplace(data.Value);
	}
}

void FGsItemCollectionSlotState::MakeRewardData(const int32 InRewardId)
{	
	const UGsTableRewardData* Table = Cast<UGsTableRewardData>(FGsSchemaRewardData::GetStaticTable());
	if (nullptr == Table)
	{
		GSLOG(Error, TEXT("nullptr == Table"));
		return;
	}

	_rewardData = nullptr;
	const FGsSchemaRewardData* rewardData = nullptr;
	if (0 < InRewardId && Table->FindRowById(InRewardId, rewardData))
	{
		_rewardData = rewardData;
	}	
}