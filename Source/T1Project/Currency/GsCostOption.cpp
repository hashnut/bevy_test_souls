#include "Currency/GsCostOption.h"

#include "Currency/GsCostPackage.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

bool FGsCostElementFilter::Passed(const FGsCostElement& InCostElement) const
{
	return false;
}

FGsCostElementFilterCurrencyType::FGsCostElementFilterCurrencyType(const CurrencyType InCurrencyType)
	: _currencyTypes({ InCurrencyType })
{
}

FGsCostElementFilterCurrencyType::FGsCostElementFilterCurrencyType(const TSet<CurrencyType> InCurrencyTypes)
	: _currencyTypes(InCurrencyTypes)
{
}

bool FGsCostElementFilterCurrencyType::Passed(const FGsCostElement& InCostElement) const
{
	if (CostType::CURRENCY != InCostElement._costType)
	{
		return false;
	}

	if (!InCostElement.HasAnyInCandidate(_currencyTypes))
	{
		return false;
	}

	return true;
}

FGsCostElementFilterItemId::FGsCostElementFilterItemId(const ItemId InItem)
	: _items({ InItem })
{
}

FGsCostElementFilterItemId::FGsCostElementFilterItemId(const TSet<ItemId> InItems)
	: _items(InItems)
{
}

bool FGsCostElementFilterItemId::Passed(const FGsCostElement& InCostElement) const
{
	if (CostType::ITEM != InCostElement._costType)
	{
		return false;
	}

	if (!InCostElement.HasAnyInCandidate(_items))
	{
		return false;
	}

	return true;
}

void FGsCostOption::AddCostElementFilter(TSharedRef<FGsCostElementFilter> InCostFilter)
{
	_costElementFilters.Emplace(MoveTemp(InCostFilter));
}

bool FGsCostOption::IsPassedAnyFilter(const FGsCostElement& InCostElement) const
{
	for (const TSharedRef<FGsCostElementFilter>& CostElementFilter : _costElementFilters)
	{
		if (CostElementFilter->Passed(InCostElement))
		{
			return true;
		}
	}

	return false;
}

bool FGsCostOption::ApplyOption(OUT Currency& InOutValue) const
{
	return false;
}

FGsCostOptionPlus::FGsCostOptionPlus(const Currency InOptionValue)
	: _optionValue(InOptionValue)
{
}

bool FGsCostOptionPlus::ApplyOption(OUT Currency& InOutValue) const
{
	InOutValue = FMath::Clamp<Currency>(InOutValue + _optionValue, 0, TNumericLimits<Currency>::Max());
	return true;
}

FGsCostOptionDiscount10kRatio::FGsCostOptionDiscount10kRatio(const uint32 InOptionValue)
	: _optionValue(InOptionValue)
{
}

bool FGsCostOptionDiscount10kRatio::ApplyOption(OUT Currency& InOutValue) const
{
	// 서버 코드와 동일하게 맞춤.
	InOutValue = FMath::Clamp<Currency>(InOutValue - StaticCast<Currency>(StaticCast<double>(InOutValue) * _optionValue * 0.0001), 0, TNumericLimits<Currency>::Max());
	return true;
}

FGsCostOptionClamp::FGsCostOptionClamp(const Currency InMinValue, const Currency InMaxValue)
	: _minValue(InMinValue), _maxValue(InMaxValue)
{
}

bool FGsCostOptionClamp::ApplyOption(OUT Currency& InOutValue) const
{
	InOutValue = FMath::Clamp<Currency>(InOutValue, _minValue, _maxValue);
	return true;
}
