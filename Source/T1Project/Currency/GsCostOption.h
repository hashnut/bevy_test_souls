#pragma once

#include "Currency/GsCostPackage.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsCostElement;

/**
 * 비용 필터
 */

struct FGsCostElementFilter
{
	virtual ~FGsCostElementFilter() {}

	virtual bool Passed(const FGsCostElement& InCostElement) const;
};

struct FGsCostElementFilterCurrencyType final : public FGsCostElementFilter
{
	const TSet<CurrencyType> _currencyTypes;

	explicit FGsCostElementFilterCurrencyType(const CurrencyType InCurrencyType);

	explicit FGsCostElementFilterCurrencyType(const TSet<CurrencyType> InCurrencyTypes);

	bool Passed(const FGsCostElement& InCostElement) const final;
};

struct FGsCostElementFilterItemId final : public FGsCostElementFilter
{
	const TSet<ItemId> _items;

	explicit FGsCostElementFilterItemId(const ItemId InItem);

	explicit FGsCostElementFilterItemId(const TSet<ItemId> InItems);

	bool Passed(const FGsCostElement& InCostElement) const final;
};

/**
 * 각종 비용 옵션(할인 등)
 */

enum class EGsCostOptionType
{
	None,

	// 값 더하기/빼기
	Plus,

	// 만분율 곱
	Discount10kRatio,

	// 최소/최대 내의 값으로 변환
	Clamp,
};

/**
 * FGsCostPackage 에서 공용으로 사용할 비용 할인 등의 옵션들을 지정함
 */

struct FGsCostOption
{
private:
	TArray<TSharedRef<FGsCostElementFilter>> _costElementFilters;

public:
	virtual ~FGsCostOption() {}

	void AddCostElementFilter(TSharedRef<FGsCostElementFilter> InCostFilter);

	bool IsPassedAnyFilter(const FGsCostElement& InCostElement) const;

	virtual bool ApplyOption(OUT Currency& InOutValue) const;
};

template<EGsCostOptionType CostOptionType>
struct TGsCostOption : public FGsCostOption
{
	const EGsCostOptionType COST_OPTION_TYPE = CostOptionType;
};

/**
 * 각종 비용 옵션 중, 비용에 _optionValue 를 Add 함. 마이너스가 필요한 경우, 마이너스 값을 입력.
 */
struct FGsCostOptionPlus final : TGsCostOption<EGsCostOptionType::Plus>
{
	const Currency _optionValue;

	explicit FGsCostOptionPlus(const Currency InOptionValue);

	bool ApplyOption(OUT Currency& InOutValue) const final;
};

/**
 * 각종 비용 옵션 중, 비용에서 _optionValue의 만분율을 제외함.
 */
struct FGsCostOptionDiscount10kRatio final : TGsCostOption<EGsCostOptionType::Discount10kRatio>
{
	const uint32 _optionValue;

	explicit FGsCostOptionDiscount10kRatio(const uint32 InOptionValue);

	bool ApplyOption(OUT Currency& InOutValue) const final;
};

/**
 * 각종 비용 옵션 중, 비용 계산 시, 최소/최대 사이의 값으로 반환함.
 */
struct FGsCostOptionClamp final : TGsCostOption<EGsCostOptionType::Clamp>
{
	const Currency _minValue;
	const Currency _maxValue;

	explicit FGsCostOptionClamp(const Currency InMinValue, const Currency InMaxValue);

	bool ApplyOption(OUT Currency& InOutValue) const final;
};
