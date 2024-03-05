#include "GsCurrencyTypeAmountCachedValueMap.h"

#include "Currency/GsCurrencyHelper.h"
#include "GsBaseCachedValue.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageUserInfo.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

/************************************************************************/
/* FGsCurrencyTypeAmountCollector                                       */
/************************************************************************/

Currency FGsCurrencyTypeAmountCollector::GetValue(const CurrencyType& InCurrencyType) const
{
	return FGsCurrencyHelper::GetCurrencyAmount(InCurrencyType);
}

/************************************************************************/
/* Currency Type Amount Cached Value                                    */
/************************************************************************/

FGsCurrencyTypeAmountCachedValue::FGsCurrencyTypeAmountCachedValue(const CurrencyType& InCurrencyType)
	: _currencyType(InCurrencyType)
{
}

void FGsCurrencyTypeAmountCachedValue::SetCollector(FGsCurrencyTypeAmountCollector& InCollector)
{
	_collector = &InCollector;
}

void FGsCurrencyTypeAmountCachedValue::UpdateValue()
{
	SetValue(_collector->GetValue(_currencyType));
}

/************************************************************************/
/* Currency Type Amount Cached Value Map                                */
/************************************************************************/

FGsCurrencyTypeAmountCachedValueMap::FGsCurrencyTypeAmountCachedValueMap(TSharedRef<FGsCurrencyTypeAmountCollector> InCollector)
	: _collector(InCollector)
{
	FGsMessageHolder* Message = GMessage();
	if (nullptr == Message)
	{
		GSLOG(Error, TEXT("nullptr == Message"));
		return;
	}

	_handlerUserInfo = Message->GetUserBasicInfo().AddRaw(MessageUserInfo::CURRENCY, this, &FGsCurrencyTypeAmountCachedValueMap::OnUser_Currency);
}

FGsCurrencyTypeAmountCachedValueMap::~FGsCurrencyTypeAmountCachedValueMap()
{
	FGsMessageHolder* Message = GMessage();
	if (nullptr == Message)
	{
		GSLOG(Warning, TEXT("nullptr == Message"));
		return;
	}

	Message->GetUserBasicInfo().Remove(_handlerUserInfo);
}

void FGsCurrencyTypeAmountCachedValueMap::InitializeCachedValue(FGsBaseCachedValue* InCachedValue) const
{
	StaticCast<FGsCurrencyTypeAmountCachedValue*>(InCachedValue)->SetCollector(_collector.Get());
}

void FGsCurrencyTypeAmountCachedValueMap::OnUser_Currency(uint64 InData)
{
	const CurrencyType TargetCurrencyType = static_cast<CurrencyType>(InData);
	this->SetDirty(TargetCurrencyType);
}
