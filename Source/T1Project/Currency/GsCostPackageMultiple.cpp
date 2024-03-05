#include "GsCostPackageMultiple.h"

#include "GsCostPackage.h"
#include "Management/ScopeGame/GsValidatorChainManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"

FGsCostPackageMultiple::FGsCostPackageMultiple(TSharedRef<const FGsCostPackage> InCostPackage, const int32 InMultiple)
	: _costPackage(InCostPackage)
	, _multiple(InMultiple)
{
	CheckValidation();
}

FGsCostPackageMultiple::FGsCostPackageMultiple(FGsCostPackageMultiple&& Rhs)
	: _costPackage(Rhs._costPackage)
	, _multiple(Rhs._multiple)
{
	CheckValidation();
}

FGsCostPackageMultiple::FGsCostPackageMultiple(const FGsCostPackageMultiple& Rhs)
	: _costPackage(Rhs._costPackage)
	, _multiple(Rhs._multiple)
{
	CheckValidation();
}

void FGsCostPackageMultiple::CheckValidation() const
{
	if (0 >= _multiple)
	{
		GSLOG(Warning, TEXT("0 >= _multiple"));
	}
}

void FGsCostPackageMultiple::SetMultiple(const int32 InMultiple)
{
	_multiple = InMultiple;
	CheckValidation();
}

int32 FGsCostPackageMultiple::GetMultiple() const
{
	return _multiple;
}

ECachedValueItemDelegateType FGsCostPackageMultiple::GetCachedValueItemDelegateType() const
{
	return _costPackage->GetCachedValueItemDelegateType();
}

int32 FGsCostPackageMultiple::GetPlayerCraftMaxCount() const
{
	return _costPackage->GetPlayerCanUseMaxCount();
}

CurrencyDelta FGsCostPackageMultiple::GetAmountSingle(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId) const
{
	return _costPackage->GetCostAmount(InCostType, InCurrencyType, InItemId);
}

CurrencyDelta FGsCostPackageMultiple::GetAmountSingle(const ItemId InItemId) const
{
	return _costPackage->GetCostAmount(InItemId);
}

CurrencyDelta FGsCostPackageMultiple::GetAmountSingle(const CurrencyType InCurrencyType) const
{
	return _costPackage->GetCostAmount(InCurrencyType);
}

CurrencyDelta FGsCostPackageMultiple::GetFirstAmountSingle(const ItemId InItemId) const
{
	return _costPackage->GetFirstCostAmount(InItemId);
}

CurrencyDelta FGsCostPackageMultiple::GetFirstAmountSingle(const CurrencyType InCurrencyType) const
{
	return _costPackage->GetFirstCostAmount(InCurrencyType);
}

FString FGsCostPackageMultiple::GetFirstAmountStringSingle(const CurrencyType InCurrencyType) const
{
	return _costPackage->GetFirstCostAmountString(InCurrencyType);
}

CurrencyDelta FGsCostPackageMultiple::GetAmountMultiple(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId) const
{
	return _costPackage->GetCostAmount(InCostType, InCurrencyType, InItemId) * _multiple;
}

CurrencyDelta FGsCostPackageMultiple::GetAmountMultiple(const ItemId InItemId) const
{
	return _costPackage->GetCostAmount(InItemId) * _multiple;
}

CurrencyDelta FGsCostPackageMultiple::GetAmountMultiple(const CurrencyType InCurrencyType) const
{
	return _costPackage->GetCostAmount(InCurrencyType) * _multiple;
}

CurrencyDelta FGsCostPackageMultiple::GetFirstAmountMultiple(const ItemId InItemId) const
{
	return _costPackage->GetFirstCostAmount(InItemId) * _multiple;
}

CurrencyDelta FGsCostPackageMultiple::GetFirstAmountMultiple(const CurrencyType InCurrencyType) const
{
	return _costPackage->GetFirstCostAmount(InCurrencyType) * _multiple;
}

const FGsCostElement* FGsCostPackageMultiple::GetSingleCostElementAt(const int32 InIndex) const
{
	return _costPackage->GetCostElementAt(InIndex);
}

int32 FGsCostPackageMultiple::GetCostElementsCount() const
{
	return _costPackage->GetCostElementsCount();
}

bool FGsCostPackageMultiple::PlayerHasEnoughMultiple() const
{
	return _costPackage->PlayerHasEnough(_multiple);
}

const FGsCostElement* FGsCostPackageMultiple::GetFirstCostElementNotEnough(const CostType InCostType /*= CostType::MAX*/) const
{
	return _costPackage->GetFirstCostElementNotEnough(InCostType, _multiple);
}
