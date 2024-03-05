#pragma once
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Costume/GsSchemaCostumeSummonChangeCurrencyInfo.h"
#include "DataSchema/Costume/GsSchemaCostumeProbability.h"
#include "GsCostumeData.h"
#include "ForEach.h"

struct T1PROJECT_API FGsCostumeSummonChangeData
{
	TWeakPtr<FGsCostumeData> _costumeData;
	int64 _probability{ 0 };
	float _perProbability{ 0.f };
};

struct T1PROJECT_API FGsCostumeSummonConfirmData
{
	CostumeConfirmId  _confirmId{ INVALID_COSTUME_CONFIRM_ID };
	CostumeProbabilityId _costumeProbabilityId{ INVALID_COSTUME_PROBABILITY_ID };

	uint8           _remainChangeCount{ 0 };                     // 잔여 교체 횟수
	FDateTime       _confirmTime;

	CurrencyType	_currencyType{ CurrencyType::NONE };
	int32			_currencyValue{ 0 };
	uint8			_maxChangeCount{ 0 };

	TWeakPtr<FGsCostumeData>	_costumeData;
	TArray<FGsCostumeSummonChangeData> _summonChangeCostumeList;

	FGsCostumeSummonConfirmData() = default;
	FGsCostumeSummonConfirmData(CostumeConfirmData& itemData, FDateTime addTime, TWeakPtr<FGsCostumeData> costumeData,
		const TArray<FGsCostumeSummonChangeData>& summonChangeCostumeList,
		const FGsSchemaCostumeProbability* summonProbability = nullptr)
		: _confirmId(itemData.mCostumeConfirmId)
		, _costumeProbabilityId(itemData.mCostumeProbabilityId)
		, _remainChangeCount(itemData.mRemainChangeCount), _costumeData(costumeData)
	{

		_confirmTime = FDateTime(addTime.GetTicks() + itemData.mRemainTime * ETimespan::TicksPerSecond);

		_summonChangeCostumeList.Empty();
		_summonChangeCostumeList.Append(summonChangeCostumeList);

		auto sumProbability = GetSumProbability();
		Algo::ForEach(_summonChangeCostumeList, [&sumProbability](auto& e) { e._perProbability = (e._probability * 100.f) / float(sumProbability);});

		if (summonProbability)
		{
			if (auto summonConfirmData = summonProbability->summonConfirmId.GetRow())
			{
				TArray<FGsSchemaCostumeSummonChangeCurrencyInfo> currencyInfo(summonConfirmData->currencyInfo);
				currencyInfo.Sort([](auto le, auto re) { return le.changeCount > re.changeCount;});

				_maxChangeCount = currencyInfo[0].changeCount + 1; // 몇회를 바꾼 뒤 금액

				auto remainCount = _maxChangeCount - _remainChangeCount;
				if (auto iter = summonConfirmData->currencyInfo.FindByPredicate([remainCount](auto& e) {
					return e.changeCount == remainCount; }))
				{
					_currencyType = iter->currencyType;
					_currencyValue = iter->value;
				}
			}
		}
	}

	void UpdateItemData(CostumeConfirmData& itemData, FDateTime addTime, TWeakPtr<FGsCostumeData> costumeData,
		const TArray<FGsCostumeSummonChangeData>& summonChangeCostumeList,
		const FGsSchemaCostumeProbability* summonProbability = nullptr)
	{
		_costumeData = costumeData;
		_costumeProbabilityId = itemData.mCostumeProbabilityId;
		_remainChangeCount = itemData.mRemainChangeCount;

		_confirmTime = FDateTime(addTime.GetTicks() + itemData.mRemainTime * ETimespan::TicksPerSecond);
		_currencyType = CurrencyType::NONE;
		_currencyValue = 0;

		_summonChangeCostumeList.Empty();
		_summonChangeCostumeList.Append(summonChangeCostumeList);

		auto sumProbability = GetSumProbability();
		Algo::ForEach(_summonChangeCostumeList, [&sumProbability](auto& e) { e._perProbability = (e._probability * 100.f) / float(sumProbability);});

		if (summonProbability)
		{
			if (auto summonConfirmData = summonProbability->summonConfirmId.GetRow())
			{
				if (_remainChangeCount > 0)
				{
					TArray<FGsSchemaCostumeSummonChangeCurrencyInfo> currencyInfo(summonConfirmData->currencyInfo);
					currencyInfo.Sort([](auto le, auto re) { return le.changeCount > re.changeCount;});

					_maxChangeCount = currencyInfo[0].changeCount + 1; // 몇회를 바꾼 뒤 금액

					auto remainCount = _maxChangeCount - _remainChangeCount;
					if (auto iter = summonConfirmData->currencyInfo.FindByPredicate([remainCount](auto& e) {
						return e.changeCount == remainCount; }))
					{
						_currencyType = iter->currencyType;
						_currencyValue = iter->value;
					}
				}
			}
		}
	}

	int64 GetSumProbability()
	{
		int64 sumProbability = 0;
		Algo::ForEach(_summonChangeCostumeList, [&sumProbability](auto& e) {sumProbability += e._probability;});
		return sumProbability;
	}

	TWeakPtr<FGsCostumeData>	GetCostumeData() const { return _costumeData; }
	const TArray<FGsCostumeSummonChangeData>& GetCostumeSummonChangeList() const { return _summonChangeCostumeList; }
};
