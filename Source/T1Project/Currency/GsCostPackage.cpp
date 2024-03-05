#include "GsCostPackage.h"

#include "Runtime/Core/Public/Math/UnrealMathUtility.h"

#include "Classes/ValidatorChain/GsItemCategorySubAmountCachedValueMap.h"
#include "Classes/ValidatorChain/GsItemIdAmountCachedValueMap.h"
#include "Currency/GsCostOption.h"
#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/GsSchemaCurrencyData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "GsCostPackageMultiple.h"
#include "GsSortCost.h"
#include "Item/GsItem.h"
#include "Item/GsItemHelper.h"
#include "Item/GsItemManager.h"
#include "Management/GsScopeGlobal.h"
#include "Management/ScopeGame/GsValidatorChainManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"

/************************************************************************/
/* FGsCostElement::FGsCostElementCurrency                               */
/************************************************************************/

Currency FGsCostElement::FGsCostElementCurrency::PlayerHasAmount() const
{
	FGsValidatorChainManager* ValidatorChainManager = GSValidatorChain();
	if (nullptr == ValidatorChainManager)
	{
		GSLOG(Error, TEXT("nullptr == ValidatorChainManager"));
		return 0;
	}
	return StaticCast<Currency>(ValidatorChainManager->GetValue(_costCurrencyType));
}

FGsCostElement::EGsCostElementType FGsCostElement::FGsCostElementCurrency::GetCostElementType() const
{
	return EGsCostElementType::CostElementCurrency;
}

/************************************************************************/
/* FGsCostElement::FGsCostElementItemBase                               */
/************************************************************************/

ItemId FGsCostElement::FGsCostElementItemBase::GetItemId() const
{
	return INVALID_ITEM_ID;
}

bool FGsCostElement::FGsCostElementItemBase::HasItemLevel() const
{
	return false;
}

Level FGsCostElement::FGsCostElementItemBase::GetItemLevel() const
{
	return INVALID_LEVEL;
}

/************************************************************************/
/* FGsCostElement::FGsCostElementItem                                   */
/************************************************************************/

FGsCostElement::EGsCostElementType FGsCostElement::FGsCostElementItem::GetCostElementType() const
{
	return EGsCostElementType::CostElementItem;
}

Currency FGsCostElement::FGsCostElementItem::PlayerHasAmount() const
{
	FGsValidatorChainManager* ValidatorChainManager = GSValidatorChain();
	if (nullptr == ValidatorChainManager)
	{
		GSLOG(Error, TEXT("nullptr == ValidatorChainManager"));
		return 0;
	}

	return StaticCast<Currency>(ValidatorChainManager->GetValue(_cachedValueItemDelegateType, _costItemId));
}

void FGsCostElement::FGsCostElementItem::AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems) const
{
	FGsValidatorChainManager* ValidatorChainManager = GSValidatorChain();
	if (nullptr == ValidatorChainManager)
	{
		GSLOG(Error, TEXT("nullptr == ValidatorChainManager"));
		return;
	}

	TSharedRef<FGsItemIdAmountCachedValue> CachedValue = ValidatorChainManager->GetCachedValue(_cachedValueItemDelegateType, _costItemId);
	CachedValue->AddAllPassedItems(OutItems);
}

bool FGsCostElement::FGsCostElementItem::IsPassedItem(const FGsItem& InItem) const
{
	FGsValidatorChainManager* ValidatorChainManager = GSValidatorChain();
	if (nullptr == ValidatorChainManager)
	{
		GSLOG(Error, TEXT("nullptr == ValidatorChainManager"));
		return 0;
	}

	TSharedRef<FGsItemIdAmountCachedValue> CachedValue = ValidatorChainManager->GetCachedValue(_cachedValueItemDelegateType, _costItemId);
	return CachedValue->IsPassedItem(InItem, _costItemId);
}

ItemId FGsCostElement::FGsCostElementItem::GetItemId() const
{
	return _costItemId;
}

/************************************************************************/
/* FGsCostElement::FGsCostElementItemLevel                              */
/************************************************************************/

Currency FGsCostElement::FGsCostElementItemLevel::PlayerHasAmount() const
{
	FGsValidatorChainManager* ValidatorChainManager = GSValidatorChain();
	if (nullptr == ValidatorChainManager)
	{
		GSLOG(Error, TEXT("nullptr == ValidatorChainManager"));
		return 0;
	}

	return StaticCast<Currency>(ValidatorChainManager->GetValue(_cachedValueItemDelegateType, _costItemId, _costItemLevel));
}

void FGsCostElement::FGsCostElementItemLevel::AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems) const
{
	FGsValidatorChainManager* ValidatorChainManager = GSValidatorChain();
	if (nullptr == ValidatorChainManager)
	{
		GSLOG(Error, TEXT("nullptr == ValidatorChainManager"));
		return;
	}

	TSharedRef<FGsItemIdLevelAmountCachedValue> CachedValue = ValidatorChainManager->GetCachedValue(_cachedValueItemDelegateType, _costItemId, _costItemLevel);
	CachedValue->AddAllPassedItems(OutItems);
}

bool FGsCostElement::FGsCostElementItemLevel::IsPassedItem(const FGsItem& InItem) const
{
	FGsValidatorChainManager* ValidatorChainManager = GSValidatorChain();
	if (nullptr == ValidatorChainManager)
	{
		GSLOG(Error, TEXT("nullptr == ValidatorChainManager"));
		return 0;
	}

	// 최적화 필요
	TSharedRef<FGsItemIdLevelAmountCachedValue> CachedValue = ValidatorChainManager->GetCachedValue(_cachedValueItemDelegateType, _costItemId, _costItemLevel);
	return CachedValue->IsPassedItem(InItem, _costItemId, _costItemLevel);
}

bool FGsCostElement::FGsCostElementItemLevel::HasItemLevel() const
{
	return true;
}

Level FGsCostElement::FGsCostElementItemLevel::GetItemLevel() const
{
	return _costItemLevel;
}

/************************************************************************/
/* FGsCostElement::FGsCostElementItemCategorySub                        */
/************************************************************************/

Currency FGsCostElement::FGsCostElementItemCategorySub::PlayerHasAmount() const
{
	FGsValidatorChainManager* ValidatorChainManager = GSValidatorChain();
	if (nullptr == ValidatorChainManager)
	{
		GSLOG(Error, TEXT("nullptr == ValidatorChainManager"));
		return 0;
	}

	return StaticCast<Currency>(ValidatorChainManager->GetValue(_costItemCategorySub));
}

void FGsCostElement::FGsCostElementItemCategorySub::AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems) const
{
	FGsValidatorChainManager* ValidatorChainManager = GSValidatorChain();
	if (nullptr == ValidatorChainManager)
	{
		GSLOG(Error, TEXT("nullptr == ValidatorChainManager"));
		return;
	}

	TSharedRef<FGsItemCategorySubAmountCachedValue> CachedValue = ValidatorChainManager->GetCachedValue(_costItemCategorySub);
	CachedValue->AddAllPassedItems(OutItems);
}

bool FGsCostElement::FGsCostElementItemCategorySub::IsPassedItem(const FGsItem& InItem) const
{
	FGsValidatorChainManager* ValidatorChainManager = GSValidatorChain();
	if (nullptr == ValidatorChainManager)
	{
		GSLOG(Error, TEXT("nullptr == ValidatorChainManager"));
		return 0;
	}

	// 최적화 필요
	TSharedRef<FGsItemCategorySubAmountCachedValue> CachedValue = ValidatorChainManager->GetCachedValue(_costItemCategorySub);
	return CachedValue->IsPassedItem(InItem, _costItemCategorySub);
}

FGsCostElement::EGsCostElementType FGsCostElement::FGsCostElementItemCategorySub::GetCostElementType() const
{
	return EGsCostElementType::CostElementItemCategorySub;
}

/************************************************************************/
/* FGsCostElement                                                       */
/************************************************************************/

const TSet<CurrencyType> FGsCostElement::ALWAYS_ENOUGH_CURRENCY_TYPES = TSet<CurrencyType>({
	CurrencyType::NONE,
	CurrencyType::MAX_ACCOUNT_TYPE,
	CurrencyType::EXP_DEATH_PENALTY_FREE,
	CurrencyType::ITEM_DEATH_PENALTY_FREE,
	CurrencyType::MAX_USER_TYPE,
	CurrencyType::CASH,
});

FGsCostElement::FGsCostElement(const FGsCostPackage& InCostPackage, const bool /*InIncludeItemGroup*/, const Currency InCostAmount, const Currency InDisplayCostAmount, const CurrencyType InCurrencyType)
	: _cachedValueItemDelegateType(InCostPackage._cachedValueItemDelegateType)
	, _jointCostCandidateElements(InCostPackage._jointCostCandidateElements)
	, _costType(CostType::CURRENCY)
	, _costAmount(InCostAmount)
	, _displayCostAmount(InDisplayCostAmount)
	, _candidates({ MakeShared<FGsCostElementCurrency>(InCurrencyType) })
{
	CheckValidation();
}

FGsCostElement::FGsCostElement(const FGsCostPackage& InCostPackage, const bool /*InIncludeItemGroup*/, const FString& InCostAmountString, const CurrencyType InCurrencyType)
	: _cachedValueItemDelegateType(InCostPackage._cachedValueItemDelegateType)
	, _jointCostCandidateElements(InCostPackage._jointCostCandidateElements)
	, _costType(CostType::CURRENCY)
	, _costAmountString(InCostAmountString)
	, _candidates({ MakeShared<FGsCostElementCurrency>(InCurrencyType) })
{
	if (InCostAmountString.IsEmpty())
	{
		GSLOG(Warning, TEXT("InCostAmountString.IsEmpty(), _costType:%d, GetCurrencyType():%d, GetItemId():%d, _costAmount:%lld, _costAmountString:%s"), _costType, GetCurrencyType(), GetItemId(), _costAmount, *_costAmountString);
	}

	CheckValidation();
}

FGsCostElement::FGsCostElement(const FGsCostPackage& InCostPackage, const bool InIncludeItemGroup, const Currency InCostAmount, const Currency InDisplayCostAmount, const ItemId InCostItemId)
	: _cachedValueItemDelegateType(InCostPackage._cachedValueItemDelegateType)
	, _jointCostCandidateElements(InCostPackage._jointCostCandidateElements)
	, _costType(CostType::ITEM)
	, _costAmount(InCostAmount)
	, _displayCostAmount(InDisplayCostAmount)
	, _candidates(MakeCandidates({ InCostItemId }, InIncludeItemGroup))
{
	CheckValidation();
}

FGsCostElement::FGsCostElement(const FGsCostPackage& InCostPackage, const bool InIncludeItemGroup, const Currency InCostAmount, const Currency InDisplayCostAmount, const ItemId InCostItemId, const Level InCostItemLevel)
	: _cachedValueItemDelegateType(InCostPackage._cachedValueItemDelegateType)
	, _jointCostCandidateElements(InCostPackage._jointCostCandidateElements)
	, _costType(CostType::ITEM)
	, _costAmount(InCostAmount)
	, _displayCostAmount(InDisplayCostAmount)
	, _candidates(MakeCandidates({ MakeTuple(InCostItemId, InCostItemLevel) }, InIncludeItemGroup))
{
	CheckValidation();
}

FGsCostElement::FGsCostElement(const FGsCostPackage& InCostPackage, const bool /*InIncludeItemGroup*/, const Currency InCostAmount, const Currency InDisplayCostAmount, const ItemCategorySub InItemCategorySub)
	: _cachedValueItemDelegateType(InCostPackage._cachedValueItemDelegateType)
	, _jointCostCandidateElements(InCostPackage._jointCostCandidateElements)
	, _costType(CostType::ITEM)
	, _costAmount(InCostAmount)
	, _displayCostAmount(InDisplayCostAmount)
	, _candidates({ MakeShared<FGsCostElementItemCategorySub>(InItemCategorySub) })
{
	CheckValidation();
}

FGsCostElement::FGsCostElement(const FGsCostPackage& InCostPackage, const bool InIncludeItemGroup, const Currency InCostAmount, const Currency InDisplayCostAmount, const TArray<TTuple<ItemId, Level>>& InItemIdMinLevels)
	: _cachedValueItemDelegateType(InCostPackage._cachedValueItemDelegateType)
	, _jointCostCandidateElements(InCostPackage._jointCostCandidateElements)
	, _costType(CostType::ITEM)
	, _costAmount(InCostAmount)
	, _displayCostAmount(InDisplayCostAmount)
	, _candidates(MakeCandidates(InItemIdMinLevels, InIncludeItemGroup))
{
	CheckValidation();
}

FGsCostElement::FGsCostElement(const FGsCostPackage& InCostPackage, const bool InIncludeItemGroup, const Currency InCostAmount, const Currency InDisplayCostAmount, const TArray<ItemId>& InItemIds)
	: _cachedValueItemDelegateType(InCostPackage._cachedValueItemDelegateType)
	, _jointCostCandidateElements(InCostPackage._jointCostCandidateElements)
	, _costType(CostType::ITEM)
	, _costAmount(InCostAmount)
	, _displayCostAmount(InDisplayCostAmount)
	, _candidates(MakeCandidates(InItemIds, InIncludeItemGroup))
{
	CheckValidation();
}

FGsCostElement::FGsCostElement(FGsCostElement&& Rhs)
	: _cachedValueItemDelegateType(Rhs._cachedValueItemDelegateType)
	, _jointCostCandidateElements(Rhs._jointCostCandidateElements)
	, _costType(Rhs._costType)
	, _costAmountString(Rhs._costAmountString)
	, _costAmount(Rhs._costAmount)
	, _displayCostAmount(Rhs._displayCostAmount)
	, _candidates(Forward<TArray<TSharedRef<FGsCostElementBase>>>(Rhs._candidates))
	, _linkedCostOptions(Forward<TOptional<TArray<TWeakPtr<FGsCostOption>>>>(Rhs._linkedCostOptions))
{
}

const FText& FGsCostElement::GetCostName() const
{
	if (CostType::CURRENCY == _costType)
	{
		const FGsSchemaCurrencyData* CurrencyData = FGsCurrencyHelper::GetCurrencyData(GetCurrencyType());
		if (nullptr == CurrencyData)
		{
			GSLOG(Error, TEXT("nullptr == CurrencyData, GetCurrencyType():%d"), static_cast<int32>(GetCurrencyType()));
		}
		else
		{
			return CurrencyData->nameText;
		}
	}
	else if (CostType::ITEM == _costType)
	{
		const FGsSchemaItemCommon* ItemCommon = FGsItem::GetTableDataByID(GetItemId());
		if (nullptr == ItemCommon)
		{
			GSLOG(Error, TEXT("nullptr == ItemCommon, GetItemId():%d"), GetItemId());
		}
		else
		{
			return ItemCommon->name;
		}
	}

	GSLOG(Error, TEXT("Invalid _costType:%d"), _costType);
	return FText::GetEmpty();
}

Currency FGsCostElement::GetCostAmount(const bool InApplyCostOption /*= true*/) const
{
	if (InApplyCostOption && _linkedCostOptions.IsSet())
	{
		for (auto i = _linkedCostOptions->Num() - 1; 0 <= i; i--)
		{
			const TWeakPtr<FGsCostOption>& CostOptionWeak = (*_linkedCostOptions)[i];
			if (!CostOptionWeak.IsValid())
			{
				const_cast<TArray<TWeakPtr<FGsCostOption>>*>(&*_linkedCostOptions)->RemoveAt(i);
				continue;
			}

			const FGsCostOption& CostOption = *CostOptionWeak.Pin();
			if (CostOption.IsPassedAnyFilter(*this))
			{
				Currency CopiedAmount = _costAmount;
				if (CostOption.ApplyOption(CopiedAmount))
				{
					return CopiedAmount;
				}
			}
		}
	}

	return _costAmount;
}

Currency FGsCostElement::GetDisplayCostAmount(const bool InApplyCostOption /*= true*/) const
{
	if (INVALID_DISPLAY_COST_AMOUNT == _displayCostAmount)
	{
		// 출력 전용 값을 사용하지 않는 경우, 원래 값을 사용함.
		return GetCostAmount(InApplyCostOption);
	}

	if (InApplyCostOption && _linkedCostOptions.IsSet())
	{
		for (auto i = _linkedCostOptions->Num() - 1; 0 <= i; i--)
		{
			const TWeakPtr<FGsCostOption>& CostOptionWeak = (*_linkedCostOptions)[i];
			if (!CostOptionWeak.IsValid())
			{
				const_cast<TArray<TWeakPtr<FGsCostOption>>*>(&*_linkedCostOptions)->RemoveAt(i);
				continue;
			}

			const FGsCostOption& CostOption = *CostOptionWeak.Pin();
			if (CostOption.IsPassedAnyFilter(*this))
			{
				Currency CopiedAmount = _displayCostAmount;
				if (CostOption.ApplyOption(CopiedAmount))
				{
					return CopiedAmount;
				}
			}
		}
	}
	
	return _displayCostAmount;
}

Currency FGsCostElement::GetCostLackAmount(const bool InApplyCostOption /*= true*/) const
{
	return FMath::Clamp<Currency>(GetCostAmount(InApplyCostOption) - PlayerHasAmountMax(), 0, TNumericLimits<Currency>::Max());
}

CurrencyType FGsCostElement::GetCurrencyType() const
{
	TSharedPtr<const FGsCostElementBase> GsCostElementBase = GetFirstCandidate();
	if (CostType::CURRENCY != _costType || !GsCostElementBase.IsValid())
	{
		return CurrencyType::NONE;
	}

	return StaticCastSharedPtr<const FGsCostElementCurrency>(GsCostElementBase)->_costCurrencyType;
}

ItemId FGsCostElement::GetItemId() const
{
	if (CostType::ITEM != _costType)
	{
		return INVALID_ITEM_ID;
	}

	TSharedPtr<const FGsCostElementBase> CostElementBase = GetFirstCandidate();
	if (!CostElementBase.IsValid())
	{
		return INVALID_ITEM_ID;
	}

	TSharedPtr<const FGsCostElementItemBase> CostElementItemBase = StaticCastSharedPtr<const FGsCostElementItemBase>(CostElementBase);
	if (!CostElementItemBase.IsValid())
	{
		return INVALID_ITEM_ID;
	}

	return CostElementItemBase->GetItemId();
}

Level FGsCostElement::GetItemLevel() const
{
	if (CostType::ITEM != _costType)
	{
		return INVALID_LEVEL;
	}

	TSharedPtr<const FGsCostElementBase> CostElementBase = GetFirstCandidate();
	if (!CostElementBase.IsValid())
	{
		return INVALID_LEVEL;
	}

	TSharedPtr<const FGsCostElementItemBase> CostElementItemBase = StaticCastSharedPtr<const FGsCostElementItemBase>(CostElementBase);
	if (!CostElementItemBase.IsValid())
	{
		return INVALID_LEVEL;
	}

	return CostElementItemBase->GetItemLevel();
}

Currency FGsCostElement::PlayerHasAmountMax() const
{
	Currency Amount = 0;
	if (_jointCostCandidateElements)
	{
		for (const TSharedRef<FGsCostElementBase>& CostElementData : _candidates)
		{
			Amount += CostElementData->PlayerHasAmount();
		}
	}
	else
	{
		for (const TSharedRef<FGsCostElementBase>& CostElementData : _candidates)
		{
			if (Amount < CostElementData->PlayerHasAmount())
			{
				Amount = CostElementData->PlayerHasAmount();
			}
		}
	}

	return Amount;
}

bool FGsCostElement::PlayerHasEnough(const int32 InMultiple /*= 1*/) const
{
	if (CostType::CURRENCY == _costType)
	{
		for (const TSharedRef<FGsCostElementBase>& CostElementData : _candidates)
		{
			if (ALWAYS_ENOUGH_CURRENCY_TYPES.Contains(StaticCastSharedRef<FGsCostElementCurrency>(CostElementData)->_costCurrencyType))
			{
				// currency 이고, 대체재화 중 하나라도 ALWAYS_ENOUGH_CURRENCY 인 경우라면 항상 true 반환함.
				return true;
			}
		}
	}

	return PlayerHasAmountMax() >= (GetCostAmount() * InMultiple);
}

bool FGsCostElement::PlayerHasEnoughJustFirstCandidate(const int32 InMultiple /*= 1*/) const
{
	TSharedPtr<const FGsCostElementBase> CostElementBase = GetFirstCandidate();
	if (!CostElementBase.IsValid())
	{
		return false;
	}

	if (CostType::CURRENCY == _costType)
	{
		TSharedPtr<const FGsCostElementCurrency> CostElementCurrency = StaticCastSharedPtr<const FGsCostElementCurrency>(CostElementBase);
		if (CostElementCurrency.IsValid() && ALWAYS_ENOUGH_CURRENCY_TYPES.Contains(CostElementCurrency->_costCurrencyType))
		{
			return true;
		}
	}

	return CostElementBase->PlayerHasAmount() >= (GetCostAmount() * InMultiple);
}

bool FGsCostElement::IsValid() const
{
	return (GetCurrencyType() != CurrencyType::NONE || INVALID_ITEM_ID != GetItemId()) && 0 < GetCostAmount();
}

void FGsCostElement::LinkCostOption(TSharedPtr<FGsCostOption> InCostOption)
{
	if (!_linkedCostOptions.IsSet())
	{
		_linkedCostOptions.Emplace();
	}

	_linkedCostOptions->Add(InCostOption);
}

bool FGsCostElement::CheckValidation() const
{
	if (0 == _candidates.Num())
	{
		GSLOG(Error, TEXT("0 == _candidates.Num()"));
		return false;
	}

	if (CostType::CURRENCY == _costType)
	{
		if (CurrencyType::NONE == GetCurrencyType())
		{
			GSLOG(Error, TEXT("CurrencyType::NONE == GetCurrencyType(), _costType:%d, GetCurrencyType():%d, GetItemId():%d, _costAmount:%lld, _costAmountString:%s"), _costType, GetCurrencyType(), GetItemId(), _costAmount, *_costAmountString);
			return false;
		}
	}
	else if (CostType::ITEM == _costType)
	{
		if (INVALID_ITEM_ID == GetItemId())
		{
			GSLOG(Error, TEXT("INVALID_ITEM_ID == GetItemId(), _costType:%d, GetCurrencyType():%d, GetItemId():%d, _costAmount:%lld, _costAmountString:%s"), _costType, GetCurrencyType(), GetItemId(), _costAmount, *_costAmountString);
			return false;
		}
	}
	else
	{
		GSLOG(Error, TEXT("Invalid _costType, _costType:%d, GetCurrencyType():%d, GetItemId():%d, _costAmount:%lld, _costAmountString:%s"), _costType, GetCurrencyType(), GetItemId(), _costAmount, *_costAmountString);
		return false;
	}

	return true;
}

void FGsCostElement::AddAmount(const Currency InCostAmount, const Currency InDisplayCostAmount)
{
	_costAmount += InCostAmount;

	if (INVALID_DISPLAY_COST_AMOUNT != InDisplayCostAmount)
	{
		if (INVALID_DISPLAY_COST_AMOUNT == InDisplayCostAmount)
		{
			// 기본값이 연산에 영향을 줄 수 있으므로, 기본값인 경우 0으로 강제 변경
			_displayCostAmount = 0;
		}

		_displayCostAmount += InDisplayCostAmount;
	}
}

TArray<TSharedRef<FGsCostElement::FGsCostElementBase>> FGsCostElement::MakeCandidates(const TArray<TTuple<ItemId, Level>>& InItemIdMinLevels, const bool InUseCandidateItemGroup)
{
	TArray<TSharedRef<FGsCostElementBase>> CostElementDatas;
	if (InUseCandidateItemGroup)
	{
		// 아이템그룹이 동일한 경우 아이템 중복 추가를 막기 위해 선처리
		TMap<ItemId, TSet<Level>> AllItemIdMinLevels;
		for (const TTuple<ItemId, Level>& ItemIdMinLevel : InItemIdMinLevels)
		{
			const Level CurrentLevel = ItemIdMinLevel.Get<1>();
			for (const ItemId CurrentItemId : UGsItemManager::GetAllItemIdsTheSameGroup(ItemIdMinLevel.Get<0>()))
			{
				AllItemIdMinLevels.FindOrAdd(CurrentItemId).Add(CurrentLevel);
			}
		}

		for (const TPair<ItemId, TSet<Level>>& ItemIdMinLevels : AllItemIdMinLevels)
		{
			const ItemId CurrentItemId = ItemIdMinLevels.Key;
			for (const Level CurrentLevel : ItemIdMinLevels.Value)
			{
				CostElementDatas.Emplace(MakeShared<FGsCostElementItemLevel>(_cachedValueItemDelegateType, CurrentItemId, CurrentLevel));
			}
		}
	}
	else
	{
		for (const TTuple<ItemId, Level>& ItemIdMinLevel : InItemIdMinLevels)
		{
			CostElementDatas.Emplace(MakeShared<FGsCostElementItemLevel>(_cachedValueItemDelegateType, ItemIdMinLevel.Get<0>(), ItemIdMinLevel.Get<1>()));
		}
	}

	return CostElementDatas;
}

TArray<TSharedRef<FGsCostElement::FGsCostElementBase>> FGsCostElement::MakeCandidates(const TArray<ItemId>& InItemIds, const bool InUseCandidateItemGroup)
{
	TArray<TSharedRef<FGsCostElementBase>> CostElementDatas;
	if (InUseCandidateItemGroup)
	{
		// 아이템그룹이 동일한 경우 아이템 중복 추가를 막기 위해 선처리
		TSet<ItemId> AllItemIds;
		for (const ItemId CurrentItemId : InItemIds)
		{
			for (const ItemId InnerCurrentItemId : UGsItemManager::GetAllItemIdsTheSameGroup(CurrentItemId))
			{
				AllItemIds.Add(InnerCurrentItemId);
			}
		}

		for (const ItemId CurrentItemId : AllItemIds)
		{
			CostElementDatas.Emplace(MakeShared<FGsCostElementItem>(_cachedValueItemDelegateType, CurrentItemId));
		}
	}
	else
	{
		for (const ItemId CurrentItemId : InItemIds)
		{
			CostElementDatas.Emplace(MakeShared<FGsCostElementItem>(_cachedValueItemDelegateType, CurrentItemId));
		}
	}

	return CostElementDatas;
}

TSharedPtr<const FGsCostElement::FGsCostElementBase> FGsCostElement::GetFirstCandidate() const
{
	if (0 < _candidates.Num())
	{
		return _candidates[0];
	}
	return nullptr;
}

Currency FGsCostElement::GetFirstCandidateAmount() const
{
	if (0 < _candidates.Num())
	{
		return _candidates[0]->PlayerHasAmount();
	}
	return 0;
}

bool FGsCostElement::HasAnyInCandidate(const CurrencyType& InCurrencyType) const
{
	for (const TSharedRef<FGsCostElementBase>& CostElementData : _candidates)
	{
		if (EGsCostElementType::CostElementCurrency == CostElementData->GetCostElementType() && InCurrencyType == StaticCastSharedRef<FGsCostElementCurrency>(CostElementData)->_costCurrencyType)
		{
			return true;
		}
	}
	return false;
}

bool FGsCostElement::HasAnyInCandidate(const TSet<CurrencyType>& InCurrencyTypes) const
{
	for (const TSharedRef<FGsCostElementBase>& CostElementData : _candidates)
	{
		if (EGsCostElementType::CostElementCurrency == CostElementData->GetCostElementType() && InCurrencyTypes.Contains(StaticCastSharedRef<FGsCostElementCurrency>(CostElementData)->_costCurrencyType))
		{
			return true;
		}
	}
	return false;
}

bool FGsCostElement::HasAnyInCandidate(const ItemId& InItemId) const
{
	for (const TSharedRef<FGsCostElementBase>& CostElementData : _candidates)
	{
		if (EGsCostElementType::CostElementItem == CostElementData->GetCostElementType() && InItemId == StaticCastSharedRef<FGsCostElementItemBase>(CostElementData)->GetItemId())
		{
			return true;
		}
	}
	return false;
}

bool FGsCostElement::HasAnyInCandidate(const ItemId& InItemId, const Level& InItemLevel) const
{
	for (const TSharedRef<FGsCostElementBase>& CostElementData : _candidates)
	{
		if (EGsCostElementType::CostElementItem == CostElementData->GetCostElementType())
		{
			const TSharedRef<FGsCostElementItem>& CostElementItem = StaticCastSharedRef<FGsCostElementItem>(CostElementData);
			if (InItemId == CostElementItem->GetItemId() && InItemLevel <= CostElementItem->GetItemLevel())
			{
				return true;
			}
		}
	}
	return false;
}

bool FGsCostElement::HasAnyInCandidate(const TSet<ItemId>& InItemIds) const
{
	for (const TSharedRef<FGsCostElementBase>& CostElementData : _candidates)
	{
		if (EGsCostElementType::CostElementItem == CostElementData->GetCostElementType() && InItemIds.Contains(StaticCastSharedRef<FGsCostElementItem>(CostElementData)->GetItemId()))
		{
			return true;
		}
	}
	return false;
}

bool FGsCostElement::HasAnyInCandidate(const ItemCategorySub& InItemCategorySub) const
{
	for (const TSharedRef<FGsCostElementBase>& CostElementData : _candidates)
	{
		if (EGsCostElementType::CostElementItemCategorySub == CostElementData->GetCostElementType() && InItemCategorySub == StaticCastSharedRef<FGsCostElementItemCategorySub>(CostElementData)->_costItemCategorySub)
		{
			return true;
		}
	}
	return false;
}

uint8 FGsCostElement::GetCandidateCount() const
{
	return _candidates.Num();
}

TArray<TWeakPtr<const FGsCostElement::FGsCostElementCurrency>> FGsCostElement::GetAllCandidateCurrencies() const
{
	TArray<TWeakPtr<const FGsCostElementCurrency>> Elements;

	if (CostType::CURRENCY == _costType)
	{
		for (const TSharedRef<FGsCostElementBase> CostElement : _candidates)
		{
			Elements.Emplace(StaticCastSharedRef<const FGsCostElementCurrency>(CostElement));
		}
	}

	return Elements;
}

TArray<TWeakPtr<const FGsCostElement::FGsCostElementItem>> FGsCostElement::GetAllCandidateItems() const
{
	TArray<TWeakPtr<const FGsCostElementItem>> Elements;

	if (CostType::ITEM == _costType)
	{
		for (const TSharedRef<FGsCostElementBase> CostElement : _candidates)
		{
			Elements.Emplace(StaticCastSharedRef<const FGsCostElementItem>(CostElement));
		}
	}

	return Elements;
}

const FGsCostElement::FGsCostElementBase* FGsCostElement::GetCandidateAt(const uint8 InIndex) const
{
	if (!_candidates.IsValidIndex(InIndex))
	{
		return nullptr;
	}

	return &(_candidates[InIndex].Get());;
}

void FGsCostElement::MakeAllPassedCandidateItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems) const
{
	if (CostType::ITEM != _costType)
	{
		return;
	}

	for (const TSharedRef<FGsCostElementBase> CostElement : _candidates)
	{
		StaticCastSharedRef<const FGsCostElementItem>(CostElement)->AddAllPassedItems(OutItems);
	}
}

static bool SortUserSpecific(const TSharedRef<FGsCostElement::FGsCostElementBase>& InA, const TSharedRef<FGsCostElement::FGsCostElementBase>& InB)
{
	return InA->_priority < InB->_priority;
}

void FGsCostElement::SortCandidates(const EGsSortItemDisposal InSortItemDisposal /*= EGsSortItemDisposal::Efficiency*/)
{
	if (CostType::ITEM != _costType || 1 >= _candidates.Num())
	{
		// 아이템 타입이고, 대체갯수가 정렬이 필요한 경우만 정렬
		return;
	}

	switch (InSortItemDisposal)
	{
	case EGsSortItemDisposal::Efficiency:
	{
		_candidates.StableSort(FGsComparerItemIdDisposalEfficiency(_cachedValueItemDelegateType));
	}
		break;

	case EGsSortItemDisposal::UserSpecific:
		_candidates.StableSort(&SortUserSpecific);
		break;

	case EGsSortItemDisposal::CraftMaterial:
	{
		_candidates.StableSort(FGsComparerItemIdDisposalEfficiencyTradableFirst(_cachedValueItemDelegateType));
	}
		break;
	}
}

void FGsCostElement::SortCandidatesForCraft(const EGsSortItemDisposal InSortItemDisposal /*= EGsSortItemDisposal::Efficiency*/)
{
	if (CostType::ITEM != _costType || 1 >= _candidates.Num())
	{
		// 아이템 타입이고, 대체갯수가 정렬이 필요한 경우만 정렬
		return;
	}

	_candidates.StableSort(FGsComparerItemIdLevelForDisposalEfficiency(_cachedValueItemDelegateType));
}

/************************************************************************/
/* Cost Package                                                         */
/************************************************************************/

FGsCostPackage::FGsCostPackage()
	: _cachedValueItemDelegateType(ECachedValueItemDelegateType::DEFAULT)
{
}

FGsCostPackage::FGsCostPackage(const ECachedValueItemDelegateType InCachedValueItemDelegateType)
	: _cachedValueItemDelegateType(InCachedValueItemDelegateType)
{
}

FGsCostPackage::FGsCostPackage(FGsCostPackage&& Rhs)
	: _cachedValueItemDelegateType(Rhs._cachedValueItemDelegateType)
	, _jointCostCandidateElements(Rhs._jointCostCandidateElements)
	, _allCostElements(Forward<TArray<FGsCostElement>>(Rhs._allCostElements))
	, _allCostOptions(Forward<TOptional<TArray<TSharedRef<FGsCostOption>>>>(Rhs._allCostOptions))
{
}

TSharedRef<FGsCostPackage> FGsCostPackage::CreateSimple(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId, const Currency InCostAmount)
{
	TSharedRef<FGsCostPackage> CostPackage = MakeShared<FGsCostPackage>();
	CostPackage->AddCost(InCostType, InCurrencyType, InItemId, InCostAmount);
	return MoveTemp(CostPackage);
}

TSharedRef<FGsCostPackage> FGsCostPackage::CreateSimple(const CurrencyType InCurrencyType, const Currency InCostAmount)
{
	TSharedRef<FGsCostPackage> CostPackage = MakeShared<FGsCostPackage>();
	CostPackage->AddCost(InCurrencyType, InCostAmount);
	return MoveTemp(CostPackage);
}

TSharedRef<FGsCostPackage> FGsCostPackage::CreateSimple(const ItemId InItemId, const Currency InCostAmount)
{
	TSharedRef<FGsCostPackage> CostPackage = MakeShared<FGsCostPackage>();
	CostPackage->AddCost(InItemId, InCostAmount);
	return MoveTemp(CostPackage);
}

void FGsCostPackage::SetJointCostCandidateElements(const bool InJointCostCandidateElements)
{
	if (_jointCostCandidateElements == InJointCostCandidateElements)
	{
		return;
	}

	_jointCostCandidateElements = InJointCostCandidateElements;

#if WITH_EDITOR
	if (0 < _allCostElements.Num())
	{
		GSLOG(Error, TEXT("이 함수는 AddCost() 이전에 실행해야 함."));
	}
#endif
}

FGsCostPackage& FGsCostPackage::AddCost(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InCostItemId, const Currency InCostAmount, const Currency InDisplayCostAmount /*= INVALID_DISPLAY_COST_AMOUNT*/)
{
	if (CostType::CURRENCY == InCostType)
	{
		AddCost(InCurrencyType, InCostAmount);
	}
	else if (CostType::ITEM == InCostType)
	{
		AddCost(InCostItemId, InCostAmount);
	}
	else
	{
		GSLOG(Error, TEXT("Invalid InCostType, InCostType:%d"), InCostType);
	}

	return *this;
}

FGsCostPackage& FGsCostPackage::AddCost(const CurrencyType InCurrencyType, const Currency InCostAmount, const Currency InDisplayCostAmount /*= INVALID_DISPLAY_COST_AMOUNT*/)
{
	if (CurrencyType::NONE != InCurrencyType)
	{
		AddCostInternal(false, InCostAmount, InDisplayCostAmount, InCurrencyType);
	}
	return *this;
}

FGsCostPackage& FGsCostPackage::AddCost(const CurrencyType InCurrencyType, const FString& InCostAmountString)
{
	if (CurrencyType::NONE != InCurrencyType)
	{
		FGsCostElement* CostElement = FindCostElement(InCurrencyType);
		if (nullptr == CostElement)
		{
			// AmountString 에는 Cost Option 사용 안함.
			_allCostElements.Emplace(*this, false, InCostAmountString, InCurrencyType);
		}
		else
		{
			GSLOG(Error, TEXT("AmountString can not add amount to existed cost"));
		}
	}
	return *this;
}

FGsCostPackage& FGsCostPackage::AddCost(const ItemId InItemId, const Currency InCostAmount, const bool InIncludeItemGroup /*= false*/, const Currency InDisplayCostAmount /*= INVALID_DISPLAY_COST_AMOUNT*/)
{
	if (INVALID_ITEM_ID != InItemId)
	{
		AddCostInternal(InIncludeItemGroup, InCostAmount, InDisplayCostAmount, InItemId).SortCandidates();
	}
	return *this;
}

FGsCostPackage& FGsCostPackage::AddCost(const ItemId InItemId, const Level InItemLevel, const Currency InCostAmount, const bool InIncludeItemGroup /*= false*/, const Currency InDisplayCostAmount /*= INVALID_DISPLAY_COST_AMOUNT*/)
{
	if (INVALID_ITEM_ID != InItemId)
	{
		AddCostInternal(InIncludeItemGroup, InCostAmount, InDisplayCostAmount, InItemId, InItemLevel).SortCandidates();
	}
	return *this;
}

FGsCostPackage& FGsCostPackage::AddCost(const ItemCategorySub InItemCategorySub, const Currency InCostAmount, const Currency InDisplayCostAmount /*= INVALID_DISPLAY_COST_AMOUNT*/)
{
	if (ItemCategorySub::NONE != InItemCategorySub)
	{
		AddCostInternal(false, InCostAmount, InDisplayCostAmount, InItemCategorySub);
	}
	return *this;
}

FGsCostPackage& FGsCostPackage::AddCostCandidates(const TArray<TTuple<ItemId, Level>>& InItemIdMinLevels, const Currency InCostAmount, const bool InIncludeItemGroup /*= false*/, const Currency InDisplayCostAmount /*= INVALID_DISPLAY_COST_AMOUNT*/)
{
	if (0 == InItemIdMinLevels.Num())
	{
		GSLOG(Error, TEXT("비용 정보가 0개 입니다."));
		return *this;
	}

	// 아무거나 일치하는 요소를 찾으면 되므로, 첫번째 요소로 검색.
	const ItemId CurrentItemId = InItemIdMinLevels[0].Get<0>();
	if (INVALID_ITEM_ID != CurrentItemId)
	{
		FGsCostElement* CostElement = FindCostElement(CurrentItemId);
		if (nullptr == CostElement)
		{
			const int32 Index = _allCostElements.Emplace(*this, InIncludeItemGroup, InCostAmount, InDisplayCostAmount, InItemIdMinLevels);
			LinkCostOptionIfPassedElement(_allCostElements[Index]);
			CostElement = &_allCostElements[Index];
		}
		else
		{
			// offset 을 목적으로 하는 경우의 호출 시,  InIncludeItemGroup 는 무시함
			CostElement->AddAmount(InCostAmount, InDisplayCostAmount);
		}

		// [U1] | ejrrb10 | 현재 실제로 보유하고 있지 않은 아이템의 경우를 정확히 체크하여 정렬한다.
		// 이는 강화 수치를 고려해야 원하지 않은 아이템이 날라가는 것을 막을 수 있기 때문이다. (C2URWQ-6232)
		CostElement->SortCandidatesForCraft();
	}

	return *this;
}

FGsCostPackage& FGsCostPackage::AddCostCandidates(const TArray<ItemId>& InItemIds, const Currency InCostAmount, const bool InIncludeItemGroup /*= true*/, const Currency InDisplayCostAmount /*= INVALID_DISPLAY_COST_AMOUNT*/)
{
	if (0 == InItemIds.Num())
	{
		GSLOG(Error, TEXT("비용 정보가 0개 입니다."));
		return *this;
	}

	// 아무거나 일치하는 요소를 찾으면 되므로, 첫번째 요소로 검색.
	FGsCostElement* CostElement = FindCostElement(TSet<ItemId>(InItemIds));
	if (nullptr == CostElement)
	{
		const int32 Index = _allCostElements.Emplace(*this, InIncludeItemGroup, InCostAmount, InDisplayCostAmount, InItemIds);
		LinkCostOptionIfPassedElement(_allCostElements[Index]);
		CostElement = &_allCostElements[Index];
	}
	else 
	{
		// 후순위 InIncludeItemGroup 는 무시함
		CostElement->AddAmount(InCostAmount, InDisplayCostAmount);
	}
	CostElement->SortCandidates();

	return *this;
}

FGsCostPackage& FGsCostPackage::AddCostOption(TSharedRef<FGsCostOption> InCostOption)
{
	if (!_allCostOptions.IsSet())
	{
		// 유효하지 않다면 생성
		_allCostOptions.Emplace();
	}

	// 옵션 추가
	TSharedRef<FGsCostOption> CostOption = (*_allCostOptions)[_allCostOptions->Add(InCostOption)];

	// 기존 설정된 모든 비용 요소를 돌면서, 이 비용 옵션을 적용할 수 있는 경우, 본 옵션을 링크
	for (FGsCostElement& CostElement : _allCostElements)
	{
		if (CostOption->IsPassedAnyFilter(CostElement))
		{
			CostElement.LinkCostOption(CostOption);
		}
	}

	return *this;
}

void FGsCostPackage::LinkCostOptionIfPassedElement(FGsCostElement& InCostElement)
{
	if (_allCostOptions.IsSet())
	{
		for (TSharedRef<FGsCostOption> CostOption : *_allCostOptions)
		{
			if (CostOption->IsPassedAnyFilter(InCostElement))
			{
				InCostElement.LinkCostOption(CostOption);
			}
		}
	}
}

bool FGsCostPackage::GetJointCostCandidateElements() const
{
	return _jointCostCandidateElements;
}

ECachedValueItemDelegateType FGsCostPackage::GetCachedValueItemDelegateType() const
{
	return _cachedValueItemDelegateType;
}

int32 FGsCostPackage::GetPlayerCanUseMaxCount() const
{
	int32 MaxMultiple = TNumericLimits<int32>::Max();

	for (const FGsCostElement& CostElement : _allCostElements)
	{
		const Currency RequireCostAmount = CostElement.GetCostAmount();
		if (0 == RequireCostAmount)
		{
			continue;
		}

		const int32 ElementMaxMultiple = StaticCast<int32>(CostElement.PlayerHasAmountMax() / RequireCostAmount);
		if (MaxMultiple > ElementMaxMultiple)
		{
			MaxMultiple = ElementMaxMultiple;
		}
	}

	// 비용 요소가 비정상이거나, 그 외 알 수 없는 예외 시 처리
	if (MaxMultiple == TNumericLimits<int32>::Max())
	{
		GSLOG(Error, TEXT("MaxMultiple == TNumericLimits<int32>::Max()"));
		MaxMultiple = 0;
	}

	return MaxMultiple;
}

Currency FGsCostPackage::GetCostAmount(const CostType& InCostType, const CurrencyType& InCurrencyType, const ItemId& InItemId) const
{
	if (InCostType == CostType::CURRENCY)
	{
		return GetCostAmount(InCurrencyType);
	}
	else if (InCostType == CostType::ITEM)
	{
		return GetCostAmount(InItemId);
	}
	return 0;
}

Currency FGsCostPackage::GetCostAmount(const ItemId& InItemId) const
{
	return GetCostAmountInternal<CostType::ITEM>(InItemId);
}

Currency FGsCostPackage::GetCostAmount(const ItemId& InItemId, const Level& InItemLevel) const
{
	return GetCostAmountInternal<CostType::ITEM>(InItemId, InItemLevel);
}

Currency FGsCostPackage::GetCostAmount(const ItemCategorySub& InItemCategorySub) const
{
	return GetCostAmountInternal<CostType::ITEM>(InItemCategorySub);
}

Currency FGsCostPackage::GetCostAmount(const CurrencyType& InCurrencyType) const
{
	return GetCostAmountInternal<CostType::CURRENCY>(InCurrencyType);
}

Currency FGsCostPackage::GetFirstCostAmount(const ItemId& InItemId) const
{
	return GetFirstCostAmountInternal<CostType::ITEM>(InItemId);
}

Currency FGsCostPackage::GetFirstCostAmount(const ItemId& InItemId, const Level& InItemLevel) const
{
	return GetFirstCostAmountInternal<CostType::ITEM>(InItemId, InItemLevel);
}

Currency FGsCostPackage::GetFirstCostAmount(const ItemCategorySub& InItemCategorySub) const
{
	return GetFirstCostAmountInternal<CostType::ITEM>(InItemCategorySub);
}

Currency FGsCostPackage::GetFirstCostAmount(const CurrencyType& InCurrencyType) const
{
	return GetFirstCostAmountInternal<CostType::CURRENCY>(InCurrencyType);
}

FString FGsCostPackage::GetFirstCostAmountString(const CurrencyType& InCurrencyType) const
{
	for (const FGsCostElement& CostElement : _allCostElements)
	{
		if (CostElement._costType == CostType::CURRENCY && CostElement.HasAnyInCandidate(InCurrencyType))
		{
			return CostElement._costAmountString;
		}
	}

	// default text 설정 필요?
	return FString();
}

Currency FGsCostPackage::GetDisplayCostAmount(const CostType& InCostType, const CurrencyType& InCurrencyType, const ItemId& InItemId) const
{
	if (InCostType == CostType::CURRENCY)
	{
		return GetDisplayCostAmount(InCurrencyType);
	}
	else if (InCostType == CostType::ITEM)
	{
		return GetDisplayCostAmount(InItemId);
	}
	return 0;
}

Currency FGsCostPackage::GetDisplayCostAmount(const ItemId& InItemId) const
{
	return GetDisplayCostAmountInternal<CostType::ITEM>(InItemId);
}

Currency FGsCostPackage::GetDisplayCostAmount(const ItemId& InItemId, const Level& InItemLevel) const
{
	return GetDisplayCostAmountInternal<CostType::ITEM>(InItemId, InItemLevel);
}

Currency FGsCostPackage::GetDisplayCostAmount(const ItemCategorySub& InItemCategorySub) const
{
	return GetDisplayCostAmountInternal<CostType::ITEM>(InItemCategorySub);
}

Currency FGsCostPackage::GetDisplayCostAmount(const CurrencyType& InCurrencyType) const
{
	return GetDisplayCostAmountInternal<CostType::CURRENCY>(InCurrencyType);
}

TSet<TWeakPtr<FGsItem>> FGsCostPackage::GetAllPassedItems() const
{
	TSet<TWeakPtr<FGsItem>> Items;

	for (const FGsCostElement& CostElement : _allCostElements)
	{
		CostElement.MakeAllPassedCandidateItems(Items);
	}

	return Items;
}

void FGsCostPackage::SortAllElements(const EGsSortItemDisposal InSortItemDisposal /*= EGsSortItemDisposal::Default*/)
{
	for (FGsCostElement& CostElement : _allCostElements)
	{
		CostElement.SortCandidates(InSortItemDisposal);
	}
}

void FGsCostPackage::SortElementAtIndex(const uint8 InIndex, const EGsSortItemDisposal InSortItemDisposal /*= EGsSortItemDisposal::Efficiency*/)
{
	if (_allCostElements.IsValidIndex(InIndex))
	{
		_allCostElements[InIndex].SortCandidates(InSortItemDisposal);
	}
}

TArray<FGsCostElement>::TConstIterator FGsCostPackage::CreateConstIteratorCostElements() const
{
	return _allCostElements.CreateConstIterator();
}

bool FGsCostPackage::IsValidCostElementsIndex(int32 InIndex) const
{
	return _allCostElements.IsValidIndex(InIndex);
}

int32 FGsCostPackage::GetCostElementsCount() const
{
	return _allCostElements.Num();
}

const FGsCostElement* FGsCostPackage::GetCostElementAt(const int32 InIndex) const
{
	if (!IsValidCostElementsIndex(InIndex))
	{
		return nullptr;
	}

	return &_allCostElements[InIndex];
}

const FGsCostElement* FGsCostPackage::GetFirstCostElement() const
{
	return GetCostElementAt(0);
}

const FGsCostElement* FGsCostPackage::FindFirstCostElementBy(const CostType InCostType) const
{
	for (const FGsCostElement& CostElement : _allCostElements)
	{
		if (InCostType == CostElement._costType)
		{
			return &CostElement;
		}
	}
	return nullptr;
}

bool FGsCostPackage::HasCostElement(const CurrencyType InCurrencyType) const
{
	return nullptr != const_cast<FGsCostPackage*>(this)->FindCostElement(InCurrencyType);
}

bool FGsCostPackage::HasCostElement(const ItemId InItemId) const
{
	return nullptr != const_cast<FGsCostPackage*>(this)->FindCostElement(InItemId);
}

const FGsCostElement* FGsCostPackage::GetFirstCostElementNotEnough(const CostType InCostType /*= CostType::MAX*/, const int32 InMultiple /*= 1*/) const
{
	const bool NeedToCheckAllCostElement = CostType::MAX == InCostType;
	for (const FGsCostElement& CostElement : _allCostElements)
	{
		if ((NeedToCheckAllCostElement || InCostType == CostElement._costType) && !CostElement.PlayerHasEnough(InMultiple))
		{
			return &CostElement;
		}
	}
	return nullptr;
}

FGsCostElement* FGsCostPackage::FindCostElement(const CurrencyType& InCurrencyType)
{
	return FindCostElementInternal<CostType::CURRENCY>(InCurrencyType);
}

FGsCostElement* FGsCostPackage::FindCostElement(const ItemId& InItemId)
{
	return FindCostElementInternal<CostType::ITEM>(InItemId);
}

FGsCostElement* FGsCostPackage::FindCostElement(const ItemId& InItemId, const Level& InItemLevel)
{
	return FindCostElementInternal<CostType::ITEM>(InItemId, InItemLevel);
}

FGsCostElement* FGsCostPackage::FindCostElement(const TSet<ItemId>& InItemIds)
{
	return FindCostElementInternal<CostType::ITEM>(InItemIds);
}

FGsCostElement* FGsCostPackage::FindCostElement(const ItemCategorySub& InItemCategorySub)
{
	return FindCostElementInternal<CostType::ITEM>(InItemCategorySub);
}

bool FGsCostPackage::PlayerHasEnough(const int32 InMultiple /*= 1*/) const
{
	check(0 < InMultiple);

	for (const FGsCostElement& CostElement : _allCostElements)
	{
		if (!CostElement.PlayerHasEnough(InMultiple))
		{
			return false;
		}
	}

	return true;
}

bool FGsCostPackage::IsValidCostAtLeast() const
{
	for (const FGsCostElement& CostElement : _allCostElements)
	{
		if (CostElement.IsValid())
		{
			return true;
		}
	}
	return false;
}

TSharedRef<FGsCostPackageMultiple> FGsCostPackage::MakeCostPackageMultiple(const int32 InDefaultMultiple /*= 1*/) const
{
	if (0 >= InDefaultMultiple)
	{
		GSLOG(Warning, TEXT("0 >= InDefaultMultiple, InDefaultMultiple:%d"), InDefaultMultiple);
	}

	return MakeShared<FGsCostPackageMultiple>(AsShared(), InDefaultMultiple);
}
