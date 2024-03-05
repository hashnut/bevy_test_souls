

#include "GsItemCollectionCondition.h"
#include "../GsItem.h"
#include "../GsItemManager.h"
#include "../Currency/GsCostPackage.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "T1Project.h"



FGsItemCollectionCondition::FGsItemCollectionCondition(CollectionConditionId InCollectionConditionId, const TArray<CollectMaterialData>& InMaterialDataSet, Currency InMaxAmount /*= 1*/)
	: _id(InCollectionConditionId), _conditionCount(InMaxAmount)
	, _registerCount(0), _actionType(ECollectionMaterialActionType::Completed)
	, _materialPackage(nullptr), _collectMaterialSet(InMaterialDataSet)
	, _fixedEntryMaterialIndex(UNFIXED_MATERIAL_INDEX)
{
	/**
	 * 아이템 컬렉션 재료 정보
	 */
}

void FGsItemCollectionCondition::Init()
{
	_registerCount = 0;

	_actionType = ECollectionMaterialActionType::Impossible;
}

void FGsItemCollectionCondition::Register(Currency InRegisterCount)
{
	// 2022/12/29 PKT - 조건 갯수보다 등록 갯수가 더 클수가 없다.(Currency 타입에 유의!! unsigned )
	_registerCount = FMath::Min(InRegisterCount, _conditionCount);

	// 2022/12/29 PKT - 완료에 필요한 수량
	Currency amount = (_conditionCount - _registerCount);

	ChangedActionType((0 >= amount) ? true : false);
}

void FGsItemCollectionCondition::UnRegister()
{
	_registerCount = 0;

	ChangedActionType(false);
}

bool FGsItemCollectionCondition::DetectChangeCost(const FGsItem* InMaterial)
{
	// 2022/12/28 PKT - 해당 재료가 가치가 있는가?
	ECollectionMaterialActionType result = IsWorthMaterial(InMaterial);

	// 2022/12/28 PKT - 가치 없음.( 이미 완료 상태 or 재료가 아님 )
	if (ECollectionMaterialActionType::Impossible == result)
	{	
		// 2023/1/5 PKT - 가치가 없을때 완료 혹은 아무런 상태가 아닐때만 빠져 나감.
		if (ECollectionMaterialActionType::Impossible == _actionType || ECollectionMaterialActionType::Completed == _actionType)
		{	
			return false;
		}		
	}
	
	// 2022/12/28 PKT - Action Type을 다시 구성
	ChangedActionType();

	return true;
}

void FGsItemCollectionCondition::FixedEntryMaterial(ItemId InMaterialId)
{
	_fixedEntryMaterialIndex = UNFIXED_MATERIAL_INDEX;

	for (int32 idx = 0; idx < _collectMaterialSet.Num(); ++idx)
	{
		if (InMaterialId == _collectMaterialSet[idx].Key)
		{
			_fixedEntryMaterialIndex = idx;
		}
	}

	// 2023/1/11 PKT - 관리자에게 내용이 변경 되었음을 알린다.
	if (OnChangedConditionStateDelegate.IsBound())
	{
		OnChangedConditionStateDelegate.Execute();
	}
}

void FGsItemCollectionCondition::DetectChangeCost(ItemId InMaterialId, ItemDBId InMaterialDBId)
{
	// 2022/12/26 PKT - 이미 완료 상태라면 할거 없음.
	if (ECollectionMaterialActionType::Completed == _actionType)
	{	
		return;
	}

	// 2022/12/28 PKT - 재료에 속하는가?
	if (false == IsMaterial(InMaterialId))
	{	
		// 2023/1/5 PKT - 가치가 없을때 완료 혹은 아무런 상태가 아닐때만 빠져 나감.
		if (ECollectionMaterialActionType::Impossible == _actionType || ECollectionMaterialActionType::Completed == _actionType)
		{
			return;
		}
	}

	// 2022/12/28 PKT - Action Type을 다시 구성
	ChangedActionType();
}

ECollectionMaterialActionType FGsItemCollectionCondition::IsWorthMaterial(const class FGsItem* InMaterial) const
{
	ECollectionMaterialActionType outResult = ECollectionMaterialActionType::Impossible;

	if (true == IsMaterial(InMaterial))
	{	// 2022/12/26 PKT - 재료 아이템 일 경우.
		outResult = ECollectionMaterialActionType::ConditionalPossible;

		if (true == IsCanCollect(InMaterial))
		{	// 2022/12/26 PKT - 즉시 등록이 가능하다면..
			outResult = ECollectionMaterialActionType::ImmediatePossible;
		}
	}

	return outResult;
}

void FGsItemCollectionCondition::ChangedActionType(bool InIsCompleted /*= false*/)
{
	ECollectionMaterialActionType nextActionType = ECollectionMaterialActionType::Impossible;

	if (InIsCompleted)
	{
		nextActionType = ECollectionMaterialActionType::Completed;
	}
	else
	{
		TArray<TWeakPtr<FGsItem>> hasMaterials;
		if (true == HasMaterials(hasMaterials))
		{
			// 2022/12/27 PKT - 재료 아이템이 존재 한다면 조건부 등록 가능 상태로 변경..
			nextActionType = ECollectionMaterialActionType::ConditionalPossible;

			for (const auto& item : hasMaterials)
			{
				if (item.IsValid() && true == IsCanCollect(item.Pin().Get()))
				{
					nextActionType = ECollectionMaterialActionType::ImmediatePossible;
					break;
				}
			}
		}
	}

	_actionType = nextActionType;
		
	// 2022/12/28 PKT - 관리자에게 변경 되었음을 알린다.
	if (OnChangedConditionStateDelegate.IsBound())
	{
		OnChangedConditionStateDelegate.Execute();
	}
}

bool FGsItemCollectionCondition::IsMaterial(const FGsItem* InMaterial) const
{
	// 2022/12/28 PKT - 이미 완료 상태라면 재료로써 가치가 없음.
	if (ECollectionMaterialActionType::Completed == _actionType || nullptr == InMaterial)
	{	
		return false;
	}

	if (InMaterial->IsEquipped() || InMaterial->GetLock() || InMaterial->IsPresetEquipped())
	{	// 2022/03/03 PKT - 잠금 / Lock 상태 안됨.
		return false;
	}
		
	return IsMaterial(InMaterial->GetTID());
}

bool FGsItemCollectionCondition::IsMaterial(ItemId InMaterialId) const
{
	// 2022/12/28 PKT - 이미 완료 상태라면 재료로써 가치가 없음.
	if (ECollectionMaterialActionType::Completed == _actionType)
	{	
		return false;
	}

	const FGsCostPackage* costPackage = GetMaterialPackage();
	return (costPackage) ? _materialPackage->HasCostElement(InMaterialId) : false;
}

bool FGsItemCollectionCondition::IsCanCollect(const FGsItem* InMaterial) const
{
	if (_conditionCount <= _registerCount || nullptr == InMaterial)
	//if (ECollectionMaterialActionType::Completed == _actionType || nullptr == InMaterial)
	{	// 2022/12/26 PKT - 이미 완료 상태라면 무언가 더하는것이 불가능.
		return false;
	}

	// 2022/03/03 PKT - 착용 / 잠금 아이템은 즉시 등록 할 수 없음. DB ID가 존재 하지 않으면 소유 하지 않은 아이템으로 간주.
	if (InMaterial->IsEquipped() || InMaterial->GetLock() || 0 >= InMaterial->GetDBID() || InMaterial->IsPresetEquipped())
	{	
		return false;
	}	

	for (const auto& item : _collectMaterialSet)
	{	
		// 2022/12/28 PKT - 재료 아이템 테이블에 동일한 정보가 있으면 등록 가능.
		if (item.Key == InMaterial->GetTID() && item.Value == InMaterial->GetLevel())
		{
			return true;
		}
	}

	return false;
}

const TPair<ItemId, Level> FGsItemCollectionCondition::MaterialInfo(ItemId InMaterial) const
{
	for (const auto& material : _collectMaterialSet)
	{
		if (material.Key == InMaterial)
		{
			return TPair<ItemId, Level>(material.Key, material.Value);
		}
	}

	return TPair<ItemId, Level>(INVALID_ITEM_ID, 0);
}

ECollectionMaterialActionType FGsItemCollectionCondition::ActionType() const
{
	return _actionType;
}

CollectionConditionId FGsItemCollectionCondition::Id() const
{
	return _id;
}

Currency FGsItemCollectionCondition::ConditionCount() const
{
	return _conditionCount;
}

Currency FGsItemCollectionCondition::RegisterCount() const
{
	return _registerCount;
}

const TPair<ItemId, Level> FGsItemCollectionCondition::EntryMaterialInfo() const
{
	// 2023/8/23 PKT - 소지하고 있는 아이템 중 귀속 아이템을 최 우선으로 보여 준다.(먼저 소비하기 위함)
	//					https://jira.com2us.com/jira/browse/CHR-21273
	auto belongSort = [](TArray<TWeakPtr<FGsItem>>& InMaterials) -> ItemId
	{
		if (0 >= InMaterials.Num())
		{
			return INVALID_ITEM_ID;
		}

		InMaterials.Sort([](const TWeakPtr<FGsItem>& lhr, const TWeakPtr<FGsItem>& rhr)
			{
				const FGsItem* lItem = lhr.Pin().Get();
				const FGsItem* rItem = rhr.Pin().Get();

				if (true == lItem->IsTradable() && false == rItem->IsTradable())
				{
					return false;
				}

				return true;
			}
		);

		return InMaterials[0].Pin().Get()->GetTID();
	};

	if (false == _collectMaterialSet.IsValidIndex(_fixedEntryMaterialIndex))
	{
		TArray<TWeakPtr<FGsItem>> hasMaterials;
		if (ECollectionMaterialActionType::ImmediatePossible == _actionType && HasCanCollectMaterials(hasMaterials))
		{
			return MaterialInfo(belongSort(hasMaterials));
		}
		else if (ECollectionMaterialActionType::ConditionalPossible == _actionType && HasMaterials(hasMaterials))
		{
			return MaterialInfo(belongSort(hasMaterials));
		}
		else
		{
			// 2023/1/10 PKT - Default Main Material
			const static int32 DEFAULT_MAIN_MATERIAL_INDEX = 0;
			return _collectMaterialSet[DEFAULT_MAIN_MATERIAL_INDEX];
		}
	}

	return _collectMaterialSet[_fixedEntryMaterialIndex];
}

const TArray<FGsItemCollectionCondition::CollectMaterialData>& FGsItemCollectionCondition::MaterialAll() const
{
	return _collectMaterialSet;
}

bool FGsItemCollectionCondition::HasMaterials(TArray<TWeakPtr<FGsItem>>& OutHasMaterials) const
{
	OutHasMaterials.Empty();

	// 2023/11/30 PKT - 기존에는 타입으로 검사하고 있었으나 기간제가 생기면서 타이밍상 문제가 생겨 카운트로 변경
	//					https://jira.com2us.com/jira/browse/C2URWQ-7503
	//if (ECollectionMaterialActionType::Completed == _actionType)
	if (_conditionCount <= _registerCount)
	{
		return false;
	}

	const FGsCostPackage* costPackage = GetMaterialPackage();
	if (costPackage)
	{
		// 2022/12/27 PKT - 재료 아이템이 있다면..
		if (true == costPackage->PlayerHasEnough())
		{
			// 2023/1/4 PKT - 갖고 있는 모든 재료 아이템을 가져 온다.
			TArray<TWeakPtr<FGsItem>> hasMaterials = costPackage->GetAllPassedItems().Array();

			// 2023/1/4 PKT - Data 순으로 정렬
			for (const auto& item : _collectMaterialSet)
			{
				for (TWeakPtr<FGsItem> hasMaterial : hasMaterials)
				{
					if (false == hasMaterial.IsValid())
					{
						continue;
					}

					// 2023/8/24 PKT - 기존에는 강화도 상관 없잉 무조건 소유 한것으로 인지 하였으나, 소유한 아이템의 강화도가 기준 강화도보다 높으면 아이템 컬렉션에서는 취합하지 않는다.
					//				   https://jira.com2us.com/jira/browse/CHR-21782
					if (item.Key == hasMaterial.Pin().Get()->GetTID() && item.Value >= hasMaterial.Pin().Get()->GetLevel())
					{
						OutHasMaterials.Emplace(hasMaterial);
					}
				}
			}
		}
	}
	
	return (0 != OutHasMaterials.Num());
}

bool FGsItemCollectionCondition::HasCanCollectMaterials(TArray<TWeakPtr<FGsItem>>& OutHasMaterials) const
{
	OutHasMaterials.Empty();

	TArray<TWeakPtr<FGsItem>> hasMaterials;
	if (true == HasMaterials(hasMaterials))
	{
		for (const auto& item : hasMaterials)
		{
			if (item.IsValid() && true == IsCanCollect(item.Pin().Get()))
			{
				OutHasMaterials.Emplace(item);
			}
		}
	}

	return (0 != OutHasMaterials.Num());
}

bool FGsItemCollectionCondition::ContainsSerachWord(const FString& InKeyWord) const
{
	if (_serachKeyWord.IsEmpty())
	{
		TArray<FString> materialNames;

		/**
		 * Cost Package에서 대체 아이템에 대한 이름까지는 현재 뽑아 올수 없기 때문에 다른 정보를 이용.
		 */
		for (const auto& item : _collectMaterialSet)
		{
			const FGsSchemaItemCommon* ItemCommon = FGsItem::GetTableDataByID(item.Key);
			if (nullptr == ItemCommon)
			{
				GSLOG(Error, TEXT("nullptr == ItemCommon, GetItemId():%d"), item.Key);
				continue;
			}
			materialNames.Emplace(ItemCommon->name.ToString());
		}

		_serachKeyWord = FString::Join(materialNames, TEXT(","));
		_serachKeyWord.ReplaceInline(TEXT(" "), TEXT(""));
	}
	
	return _serachKeyWord.Contains(InKeyWord);
}

ECollectionMaterialActionType FGsItemCollectionCondition::IsPossiveAction(ItemId InMateriId) const
{
	if (ECollectionMaterialActionType::Completed != _actionType)
	{
		TArray<TWeakPtr<FGsItem>> hasMaterials;
		if (HasMaterials(hasMaterials) && hasMaterials.FindByPredicate([InMateriId](const TWeakPtr<FGsItem>& InData)
			{
				return InData.IsValid() && InData.Pin().Get()->GetTID() == InMateriId;
			}
		))
		{
			if (HasCanCollectMaterials(hasMaterials) && hasMaterials.FindByPredicate([InMateriId](const TWeakPtr<FGsItem>& InData)
				{
					return InData.IsValid() && InData.Pin().Get()->GetTID() == InMateriId;
				}
			))
			{
				return ECollectionMaterialActionType::ImmediatePossible;
			}
			else
			{
				return ECollectionMaterialActionType::ConditionalPossible;
			}
		}
	}	

	return ECollectionMaterialActionType::Impossible;
}

const FGsCostPackage* FGsItemCollectionCondition::GetMaterialPackage() const
 {
	if (false == _materialPackage.IsValid())
	{
		_materialPackage = MakeShared<FGsCostPackage>(ECachedValueItemDelegateType::CONSUMABLE_INVEN_DEPOT);

		TArray<ItemId> materials;
		for (const CollectMaterialData& item : _collectMaterialSet)
		{
			materials.Add(item.Key);
		}

		static int32 COST_AMOUNT = 1;
		_materialPackage->AddCostCandidates(materials, COST_AMOUNT);

		if (0 >= _materialPackage->GetCostElementsCount())
		{
			GSLOG(Error, TEXT("0 >= costPackage->GetCostElementsCount()"));
		}
			
	}	
	return _materialPackage.IsValid() ? _materialPackage.Get() : nullptr;
 }