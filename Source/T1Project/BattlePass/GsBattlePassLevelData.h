#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassLevelInfo.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassLevelBonusInfo.h"
#include "Reward/GsRewardHelper.h"

struct FGsBattlePassOpenPay
{
	enum { NONE, DISABLE, ITEM, CURRENCY } _payType{ NONE };
	struct
	{
		ItemId itemId{ INVALID_ITEM_DB_ID };
		int64  amount{ 0 };
	} _item;

	struct
	{
		CurrencyType  type{ CurrencyType::NONE };
		int64		  amount{ 0 };
	} _currency;

	inline FGsBattlePassOpenPay& operator +=(const FGsBattlePassOpenPay& inPay)
	{
		if (inPay._payType == CURRENCY && 
			(_currency.type == CurrencyType::NONE || _currency.type == inPay._currency.type))
		{
			_payType = inPay._payType;
			_currency.amount += inPay._currency.amount;

			if (_currency.type == CurrencyType::NONE)
				_currency.type = inPay._currency.type;
		}
		else if (inPay._payType == ITEM &&
			(_item.itemId == INVALID_ITEM_DB_ID || _item.itemId == inPay._item.itemId))
		{
			_payType = inPay._payType;
			_item.amount += inPay._item.amount;

			if (_item.itemId == INVALID_ITEM_DB_ID)
				_item.itemId = inPay._item.itemId;
		}
		return *this;
	}
};

struct T1PROJECT_API FGsBattlePassLevelData
{
	const FGsSchemaBattlePassLevelInfo* _levelInfo{ nullptr };

	Level _level{ 0 };
	Exp   _curExp{ 0 };

	bool _isOpen{ false };
	bool _isRewarded{ false };

	bool _isAdditionalLock{ true };
	bool _isAdditionalRewarded{ false };

	bool _isProgressLevel{ false };

	FGsRewardUnboxingData _rewardData;
	FGsRewardUnboxingData _addtionalRewardData;

	TWeakPtr<FGsBattlePassLevelData> _dpLevelData;

	FGsBattlePassLevelData() = default;
	FGsBattlePassLevelData(int32 level, const FGsSchemaBattlePassLevelInfo* info,
		bool isOpen = false, bool isRewarded = false, bool isAdditionalRewarded = false)
		: _levelInfo(info), _level(level)
		, _isOpen(isOpen), _isRewarded(isRewarded)
		, _isAdditionalRewarded(isAdditionalRewarded) {}

	void SetLevelOpen()
	{
		_curExp = GetNeedOpenExp();
		_isOpen = true;
	}

	void SetProgressLevel(Exp exp);
	void SetComplete() { _isProgressLevel = false; }

	void SetAdditionalRewardUnLock() { _isAdditionalLock = false; }
	void SetRewarded() { _isRewarded = true; _isOpen = true; }
	void SetAdditionalRewarded() { _isAdditionalRewarded = true; _isOpen = true; }

	void SetForDisplayLevelData(TWeakPtr<FGsBattlePassLevelData> dpLevelData) {
		_dpLevelData = dpLevelData;
	}

	TWeakPtr<FGsBattlePassLevelData> GetForDisplayLevelData() { return _dpLevelData; }

	bool IsEnableLevelReward() { return _isOpen && !_isRewarded; }
	bool IsEnableLevelAdditionalReward() { return !_isAdditionalLock && _isOpen && !_isAdditionalRewarded; }
	bool IsUnlockAddtionalReward() { return _isAdditionalLock == false; }
	bool IsProgressLevel() { return _isProgressLevel; }
	bool IsOpenLevel() { return _isOpen ;  }

	Level GetLevel() { return _level; }
	Exp   GetExp()   { return _isProgressLevel ? _curExp : _isOpen ? GetNeedOpenExp() : 0; }
	FSoftObjectPath IconPath() { return _levelInfo != nullptr ? _levelInfo->icon : FSoftObjectPath(); }
	bool IsLevelForDisplay() { return _levelInfo != nullptr ? _levelInfo->displayLevelFlag : false; }

	bool GetRewardInfo(FGsRewardUnboxingData& reward);
	bool GetAddtionalReward(FGsRewardUnboxingData& reward);
	Exp  GetNeedOpenExp() { return _levelInfo ? _levelInfo->exp : 0; }
	TWeakPtr<FGsBattlePassLevelData> GetDPLevelData() { return _dpLevelData; }

	FGsBattlePassOpenPay GetLevelOpenPay();
};

struct T1PROJECT_API FGsBattlePassLevelBonusData
{
	int32 _count{ 0 };
	int64 _exceedExp{ 0 };

	bool _isValidBonus{ false };
	bool _isRewared{ false };
	int64 _exceedPerExp{ 0 };
	int64 _maxExp{ 0 };
	FGsRewardUnboxingData _rewardData;

	FGsBattlePassLevelBonusData() = default;
	FGsBattlePassLevelBonusData(const FGsSchemaBattlePassLevelBonusInfo& info, int64 lastMaxExp, int32 count = 0)
	{
		_count = count;
		_isValidBonus = info.isPayBonus;
		_exceedPerExp = lastMaxExp + info.exceedPerExp;
		_maxExp = info.maxExp;

		if (const auto rewardBox = info.bonusReward.GetRow())
		{
			FGsRewardHelper::UnboxingRewardData(rewardBox->id, _rewardData);
		}
	}

	bool IsEnableBonus() { return _isValidBonus && _count > 0;  }
	bool IsOpenBonus() { return _isValidBonus && _exceedExp > 0; }
	bool IsRewardedBonus() { return _isValidBonus && _isRewared; }

	void SetRewarded() { _isRewared = true; }

	int32 GetRewardableBonusCount() { return _count; }

	FGsRewardUnboxingData& GetRewardData() { return _rewardData;  }

	void SetExceedBattlePassExp(int64 exceedExp)
	{
		_exceedExp = exceedExp;

		if (_maxExp > 0) exceedExp = FMath::Min(exceedExp, _maxExp);
		_count = _exceedPerExp > 0 ? exceedExp / _exceedPerExp : 0;

		if (_count > 0) _isRewared = false;
	}
};

