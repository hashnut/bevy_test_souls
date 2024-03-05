#pragma once

#include "GsBaseCachedValue.h"
#include "Message/GsMessageUserInfo.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "TGsBaseCachedValueMap.h"

/**
 * FGsCurrencyTypeAmountCachedValueGetValue
 */
struct FGsCurrencyTypeAmountCollector final
{
	Currency GetValue(const CurrencyType& InCurrencyType) const;
};

/**
 * Currency Type Amount Cached Value
 */
class FGsCurrencyTypeAmountCachedValue final : public TGsBaseCachedValue<Currency>
{
private:
	FGsCurrencyTypeAmountCollector* _collector;
	const CurrencyType _currencyType;

public:
	FGsCurrencyTypeAmountCachedValue(const CurrencyType& InCurrencyType);

	void SetCollector(FGsCurrencyTypeAmountCollector& InCollector);

public:
	void UpdateValue() final;
};

/**
 * Currency Type Amount Cached Value Map
 */
class FGsCurrencyTypeAmountCachedValueMap final : public TGsBaseCachedValueMap<FGsCurrencyTypeAmountCachedValue, CurrencyType>
{
private:
	TSharedRef<FGsCurrencyTypeAmountCollector> _collector;
	TPair<MessageUserInfo, FDelegateHandle> _handlerUserInfo;

public:
	FGsCurrencyTypeAmountCachedValueMap(TSharedRef<FGsCurrencyTypeAmountCollector> InCollector);

	~FGsCurrencyTypeAmountCachedValueMap() final;

protected:
	void InitializeCachedValue(FGsBaseCachedValue* InCachedValue) const final;

private:
	void OnUser_Currency(uint64 InData);
};
