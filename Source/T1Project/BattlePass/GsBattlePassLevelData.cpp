#include "GsBattlePassLevelData.h"

void FGsBattlePassLevelData::SetProgressLevel(Exp exp)
{
	_isProgressLevel = true;
	_isOpen = true;
	_curExp = exp;
}

bool FGsBattlePassLevelData::GetRewardInfo(FGsRewardUnboxingData& reward)
{
	if (_levelInfo)
	{
		if (const auto rewardBox = _levelInfo->levelBasicReward.GetRow())
		{
			FGsRewardHelper::UnboxingRewardData(rewardBox->id, reward);
		}
	}

	return IsEnableLevelReward();
}

bool FGsBattlePassLevelData::GetAddtionalReward(FGsRewardUnboxingData& reward)
{
	if (_levelInfo)
	{
		if (const auto rewardBox = _levelInfo->levelAdditionalReward.GetRow())
		{
			FGsRewardHelper::UnboxingRewardData(rewardBox->id, reward);
		}
	}

	return IsEnableLevelAdditionalReward();
}

FGsBattlePassOpenPay FGsBattlePassLevelData::GetLevelOpenPay()
{
	FGsBattlePassOpenPay pay{ FGsBattlePassOpenPay::NONE, {INVALID_ITEM_DB_ID, 0}, {CurrencyType::NONE, 0} };

	if (_levelInfo)
	{
		auto itemInfo = _levelInfo->levelPurchaseItemId.GetRow();
		if (itemInfo != nullptr && itemInfo->id != INVALID_ITEM_DB_ID)
		{
			pay._payType = pay.ITEM;
			pay._item = { uint32(itemInfo->id), _levelInfo->levelPurchaseItemCount };
		}
		else
		{
			pay._payType = pay.CURRENCY;
			pay._currency = { _levelInfo->levelPurchaseCurrencyType, _levelInfo->levelPurchaseCurrency };
		}
	}
	else
	{
		pay = FGsBattlePassOpenPay{ FGsBattlePassOpenPay::DISABLE, {INVALID_ITEM_DB_ID, 0}, {CurrencyType::NONE, 0} };
	}
	return pay;
}
