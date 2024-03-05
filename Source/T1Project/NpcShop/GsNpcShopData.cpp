// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNpcShopData.h"
#include "Currency/GsCurrencyHelper.h"


Currency FGsNpcShopItemInfo::GetCurrencyAmount(NpcFunctionType InType) const
{
	if (CurrencyType::NONE != _currencyType)
	{
		if (FGsCurrencyHelper::IsTaxNpcType(InType))
		{
			return FGsCurrencyHelper::GetTaxIncludedPrice(_currencyType, _currencyAmount);
		}
	}

	return _currencyAmount;
}
