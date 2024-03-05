// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"


struct FGsSchemaRewardItemBagData;
class UGsUIIconItem;

/**
 * 보상 슬롯 표시 시 편의를 위해 사용하는 클래스
 */
struct T1PROJECT_API FGsRewardIconDataWrapper
{
public:
	enum class EGsRewardIconDataType
	{
		NONE = 0,
		ITEM_BAG,
		CURRENCY,
		EXP
	};

public:
	EGsRewardIconDataType _dataType = EGsRewardIconDataType::NONE;

	const FGsSchemaRewardItemBagData* _itemBag = nullptr;

	// FGsSchemaRewardData의 재화 정보를 포함할 때만 사용
	CurrencyType _currencyType = CurrencyType::NONE;
	TPair<Currency, Currency> _currencyAmountPair; // Key: Min값, Value: Max값

	double _expPercent = 0;

public:
	FGsRewardIconDataWrapper() = default;

	explicit FGsRewardIconDataWrapper(const FGsSchemaRewardItemBagData* InItemBag)
		: _itemBag(InItemBag)
	{
		_dataType = EGsRewardIconDataType::ITEM_BAG;

		_currencyType = CurrencyType::NONE;
		_currencyAmountPair.Key = 0;
		_currencyAmountPair.Value = 0;
	}

	explicit FGsRewardIconDataWrapper(CurrencyType InType, Currency InMin, Currency InMax)
		: _currencyType(InType)
	{
		_dataType = EGsRewardIconDataType::CURRENCY;

		_itemBag = nullptr;
		_currencyAmountPair.Key = InMin;
		_currencyAmountPair.Value = InMax;
	}

	explicit FGsRewardIconDataWrapper(double InExpPercent)
		: _expPercent(InExpPercent)
	{
		_dataType = EGsRewardIconDataType::EXP;

		_itemBag = nullptr;
		_currencyType = CurrencyType::NONE;
	}

public:
	static void SetUIIconItem(FGsRewardIconDataWrapper* InData, OUT UGsUIIconItem* OutIcon);

public:
	bool IsValid() const;
	
	void Clear();
	void CopyData(FGsRewardIconDataWrapper* InData);
	
	void SetDataToUIIconItem(OUT UGsUIIconItem* OutIcon);
};
