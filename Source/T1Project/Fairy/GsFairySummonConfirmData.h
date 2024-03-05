#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Fairy/GsSchemaFairySummonChangeCurrencyInfo.h"
#include "DataSchema/Fairy/GsSchemaFairyProbability.h"
#include "GsFairyData.h"
#include "ForEach.h"

struct T1PROJECT_API FGsFairySummonChangeData
{
	TWeakPtr<FGsFairyData> _fairyData;
	int64 _probability{ 0 };
	float _perProbability{ 0.f };
};

struct T1PROJECT_API FGsFairySummonConfirmData
{
	FairyConfirmId   _fairyConfirmId{ INVALID_FAIRY_CONFIRM_ID };
	FairyProbabilityId _fairyProbabilityId{ INVALID_FAIRY_PROBABILITY_ID };
	 
	uint8           _remainChangeCount{ 0 };                     // ÀÜ¿© ±³Ã¼ È½¼ö
	FDateTime       _confirmTime;

	CurrencyType	_currencyType{ CurrencyType::NONE };
	int32			_currencyValue{ 0 };
	uint8			_maxChangeCount{ 0 };

	TWeakPtr<FGsFairyData>	_fairyData;
	TArray<FGsFairySummonChangeData> _summonChangeFairyList;

	FGsFairySummonConfirmData() = default;
	FGsFairySummonConfirmData(FairyConfirmData& itemData, FDateTime addTime, TWeakPtr<FGsFairyData> fairyData,
		const TArray<FGsFairySummonChangeData>& summonChangeFairyList,
		const FGsSchemaFairyProbability* summonProbability = nullptr)
		: _fairyConfirmId(itemData.mFairyConfirmId)
		, _fairyProbabilityId(itemData.mFairyProbabilityId)
		, _remainChangeCount(itemData.mRemainChangeCount), _fairyData(fairyData)
	{
		_confirmTime = FDateTime(addTime.GetTicks() + itemData.mRemainTime * ETimespan::TicksPerSecond);

		_summonChangeFairyList.Empty();
		_summonChangeFairyList.Append(summonChangeFairyList);

		auto sumProbability = GetSumProbability();
		Algo::ForEach(_summonChangeFairyList, [&sumProbability](auto& e) { e._perProbability = (e._probability * 100.f) / float(sumProbability);});

		if (summonProbability)
		{
			if (auto summonConfirmData = summonProbability->summonConfirmId.GetRow())
			{
				TArray<FGsSchemaFairySummonChangeCurrencyInfo> currencyInfo(summonConfirmData->currencyInfo);
				currencyInfo.Sort([](auto le, auto re) { return le.changeCount > re.changeCount;});

				_maxChangeCount = currencyInfo[0].changeCount + 1; // ¸îÈ¸¸¦ ¹Ù²Û µÚ ±Ý¾×

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

	void UpdateItemData(FairyConfirmData& itemData, FDateTime addTime, TWeakPtr<FGsFairyData> fairyData,
		const TArray<FGsFairySummonChangeData>& summonChangeFairyList,
		const FGsSchemaFairyProbability* summonProbability = nullptr)
	{
		_fairyData = fairyData;
		_fairyProbabilityId = itemData.mFairyProbabilityId;
		_remainChangeCount = itemData.mRemainChangeCount;
		_confirmTime = FDateTime(addTime.GetTicks() + itemData.mRemainTime * ETimespan::TicksPerSecond);
		_currencyType = CurrencyType::NONE;
		_currencyValue = 0;

		_summonChangeFairyList.Empty();
		_summonChangeFairyList.Append(summonChangeFairyList);

		auto sumProbability = GetSumProbability();
		Algo::ForEach(_summonChangeFairyList, [&sumProbability](auto& e) { e._perProbability = (e._probability * 100.f) / float(sumProbability);});

		if (summonProbability)
		{
			if (auto summonConfirmData = summonProbability->summonConfirmId.GetRow())
			{
				if (_remainChangeCount > 0)
				{
					TArray<FGsSchemaFairySummonChangeCurrencyInfo> currencyInfo(summonConfirmData->currencyInfo);
					currencyInfo.Sort([](auto le, auto re) { return le.changeCount > re.changeCount;});

					_maxChangeCount = currencyInfo[0].changeCount + 1;

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
		Algo::ForEach(_summonChangeFairyList, [&sumProbability](auto& e) {sumProbability += e._probability;});
		return sumProbability;
	}

	TWeakPtr<FGsFairyData>	GetFairyData() const { return _fairyData; }
	const TArray<FGsFairySummonChangeData>& GetFairySummonChangeList() const { return _summonChangeFairyList;  }
};