#include "GsBattlePassData.h"
#include "DataCenter/Public/DataSchema/BattlePass/GsSchemaBattlePassCommon.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsBattlePassManager.h"
#include "Algo/ForEach.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"

FGsBattlePassData::FGsBattlePassData(const FGsSchemaBattlePassCommon* data) : _tblData(data)
{
	if (_tblData)
	{
		_id = _tblData->id;
		_detail = _tblData->detail.GetRow();
		_tabDetail = _tblData->tabDetailId.GetRow();

		MakeBattlePassMissionData(_tblData->passMissionGroupId.GetRow());
		MakeBattlePassLevelData(_tblData->passLevelGroupId.GetRow());

		SetOpenPass();
		SetExposePassTab();
	}
}

FGsBattlePassData::~FGsBattlePassData()
{
	_passMissionGroup._missionTabs.Reset();
	_passLevelGroup._levels.Reset();
}

bool FGsBattlePassData::IsOpenPass()
{
	return _isExpose && _isOpen && FGsTimeSyncUtil::GetServerNowDateTime() >= _passOpenTime && !IsFinishedPass();
}

bool FGsBattlePassData::IsFinishedPass()
{
	return FGsTimeSyncUtil::GetServerNowDateTime() >= _passEndTime;
}

FString FGsBattlePassData::GetPassAvailablePeriod()
{
	FText periodFormat;
	if (FText::FindText(TEXT("BattlePassText"), TEXT("UI_PassNormal_DueDate"), periodFormat) && _tblData)
	{
		FDateTime period;
		FDateTime::Parse(_tblData->passPeriodEnd, period);

		period = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(period);
		FString time;
		FGsTimeStringHelper::GetTimeStringNYMDHM(period, time);

		return periodFormat.Format(periodFormat, FText::FromString(time)).ToString();
	}

	return _tblData != nullptr ? _tblData->passPeriodEnd : TEXT(""); //~까지 처리 해야함
}

FSoftObjectPath FGsBattlePassData::GetCurrentIconPath()
{
	if (auto levelData = _passLevelGroup._currentLevel.Pin())
	{
		return levelData->IconPath();
	}
	return FSoftObjectPath();
}

FGsBattlePassOpenPay FGsBattlePassData::GetLevelOpenPay(uint8 openLevel)
{
	auto curLevel = _passLevelGroup._currentLevel.Pin();
	Level minLevel = curLevel ? curLevel->GetLevel() : 0;
	Level maxLevel = _passLevelGroup._levels.Last()->GetLevel();

	FGsBattlePassOpenPay pay{ FGsBattlePassOpenPay::NONE, {INVALID_ITEM_DB_ID, 0}, {CurrencyType::NONE, 0} };

	if (openLevel >= minLevel && openLevel <= maxLevel)
	{
		pay = FGsBattlePassOpenPay{ FGsBattlePassOpenPay::NONE, {INVALID_ITEM_DB_ID, 0}, {CurrencyType::NONE, 0} };

		auto buyLevels = _passLevelGroup._levels.FilterByPredicate([minLevel, openLevel](auto e) {
			return e->GetLevel() > minLevel && e->GetLevel() <= openLevel; });

		Algo::ForEach(buyLevels, [&pay](auto level) { pay += level->GetLevelOpenPay(); });
	}
	else
	{
		pay = FGsBattlePassOpenPay{ FGsBattlePassOpenPay::DISABLE, {INVALID_ITEM_DB_ID, 0}, {CurrencyType::NONE, 0} };
	}
	return pay;
}

FGsBattlePassOpenPay FGsBattlePassData::GetAdditionalRewardOpenPay()
{
	FGsBattlePassOpenPay pay{ FGsBattlePassOpenPay::NONE, {INVALID_ITEM_DB_ID, 0}, {CurrencyType::NONE, 0} };

	if (IsOpenPass() && !_isUnlockAdditionalReward && _tblData != nullptr)
	{
		auto itemInfo = _tblData->additionalRewardOpenItemId.GetRow();
		if (_tblData->additionalRewardOpenCurrencyType != CurrencyType::NONE)
		{
			pay._payType = pay.CURRENCY;
			pay._currency = { _tblData->additionalRewardOpenCurrencyType, _tblData->additionalRewardOpenCurrency };
		}
		else if (itemInfo != nullptr && itemInfo->id != INVALID_ITEM_DB_ID)
		{
			pay._payType = pay.ITEM;
			pay._item = { uint32(itemInfo->id), _tblData->additionalRewardOpenItemAmount };
		}
	}
	else
	{
		pay = FGsBattlePassOpenPay{ FGsBattlePassOpenPay::DISABLE, {INVALID_ITEM_DB_ID, 0}, {CurrencyType::NONE, 0} };
	}
	return pay;
}

TWeakPtr<FGsBattlePassMissionTab> FGsBattlePassData::GetMissionTabInfo(BattlePassMissionTabId tabId)
{
	if (auto tab = _passMissionGroup._missionTabs.FindByPredicate([tabId](auto e) { return e->GetTabId() == tabId; }))
	{
		return (*tab);
	}

	return nullptr;
}

TWeakPtr<FGsBattlePassMissionTab> FGsBattlePassData::GetMissionTabInfoByIndex(int32 index)
{
	if (_passMissionGroup._missionTabs.IsValidIndex(index))
	{
		return _passMissionGroup._missionTabs[index];
	}

	return nullptr;
}

TWeakPtr<FGsBattlePassLevelData> FGsBattlePassData::FindLevelData(Level level)
{
	auto iter = _passLevelGroup._levels.FindByPredicate([level](auto e) {  return e && e->GetLevel() == level; });

	return iter ? *iter : nullptr;
}

Level FGsBattlePassData::GetMaxLevel()
{
	return _passLevelGroup._levels.Last()->GetLevel();
}

bool FGsBattlePassData::IsLastLevel()
{
	bool isLast = false;
	if (auto level = _passLevelGroup._currentLevel.Pin())
	{
		if (auto lastLevel = _passLevelGroup._levels.Last())
		{
			return lastLevel->GetLevel() == level->GetLevel();
		}
	}
	return false;
}

bool FGsBattlePassData::IsBonusLevel()
{
	bool isLast = IsLastLevel();

	return isLast && _passLevelGroup._bonusLevel->_isValidBonus ? false : isLast;
}

bool FGsBattlePassData::IsEligibleForRemainRewards()
{
	return _passLevelGroup._levels.FindByPredicate([](auto e) {
		return e.IsValid() && (e->IsEnableLevelReward() || e->IsEnableLevelAdditionalReward()); }) != nullptr ?
		true : _passLevelGroup._bonusLevel && _passLevelGroup._bonusLevel->IsEnableBonus();
	return false;
}

int FGsBattlePassData::GetPassLevelList(OUT TArray<TWeakPtr<FGsBattlePassLevelData>>& levels)
{
	levels.Append(_passLevelGroup._levels);
	return levels.Num();
}

int FGsBattlePassData::GetMissionTabList(OUT TArray<TWeakPtr<FGsBattlePassMissionTab>>& missionTabs)
{
	missionTabs.Append(_passMissionGroup._missionTabs);
	return missionTabs.Num();
}

int FGsBattlePassData::GetMissionList(IN BattlePassMissionTabId tabId, OUT TArray<TWeakPtr<FGsBattlePassMissionData>>& missions)
{
	if (auto tab = GetMissionTabInfo(tabId).Pin())
	{
		return tab->GetMissionData(missions);
	}
	return 0;
}

TWeakPtr<FGsBattlePassLevelBonusData> FGsBattlePassData::GetPassLevelBonusData()
{
	if (_passLevelGroup._bonusLevel && _passLevelGroup._bonusLevel->_isValidBonus)
	{
		return _passLevelGroup._bonusLevel;
	}
	return nullptr;
}

void FGsBattlePassData::SetExposePassTab()
{
	_isExpose = true; // 오픈과 동시에 노출됨 : 기획변경 2023-04-13
	if (_tblData)
	{
		FDateTime::Parse(_tblData->passPeriodStart, _passOpenTime);
		FDateTime::Parse(_tblData->passPeriodEnd, _passEndTime);
	}

	if (IsOpenPass()) // 오픈과 동시에 노출됨 : 기획변경 2023-04-13
	{
		// 오픈 시간이어도 패스가 오픈되기 전까지 미션은 오픈되지 않음
		Algo::ForEach(_passMissionGroup._missionTabs, [](auto e) { e->SetPassOpenNotiToMissionTab(); });
		if (auto levelData = _passLevelGroup._currentLevel.Pin())
		{
			SetPassLevelInfo(levelData->GetLevel(), levelData->GetExp());
		}
	}
}
void FGsBattlePassData::SetPassLevelInfo(Level curLevel, Exp curExp)
{
	auto openLevels = _passLevelGroup._levels.FilterByPredicate([curLevel](auto e) {
		return e && e->GetLevel() <= curLevel && e->_isOpen == false;
		});

	for (auto level : openLevels)
	{
		level->SetLevelOpen();
	}
	if (_passLevelGroup._currentLevel.IsValid() && 
		_passLevelGroup._currentLevel.Pin()->GetLevel() != curLevel)
	{
		_passLevelGroup._currentLevel.Pin()->SetComplete();
	}

	_passLevelGroup._currentLevel = FindLevelData(curLevel);
	if (auto levelData = _passLevelGroup._currentLevel.Pin())
	{
		levelData->SetProgressLevel(curExp);
		if (IsLastLevel())
		{
			_passLevelGroup._bonusLevel->SetExceedBattlePassExp(curExp);
		}
	}
}

void FGsBattlePassData::SetPassLevelRewarded(Level level, bool rewarded /*= true*/)
{
	if (auto levelData = FindLevelData(level).Pin())
	{
		levelData->SetRewarded();
	}
}

void FGsBattlePassData::SetPassLevelAdditionalRewarded(Level level, bool rewarded /*= true*/)
{
	if (auto levelData = FindLevelData(level).Pin())
	{
		levelData->SetAdditionalRewarded();
	}
}

void FGsBattlePassData::SetAdditionalRewardUnlock(bool unlock /*= true*/)
{
	_isUnlockAdditionalReward = unlock;
	if (unlock)
	{
		for (auto level : _passLevelGroup._levels)
		{
			level->SetAdditionalRewardUnLock();
		}
	}
}

void FGsBattlePassData::OnMissionInfoChanged(BattlePassMissionTabId missionTabId, BattlePassMissionId missionId, uint64 missionPoint)
{
	if (auto tab = GetMissionTabInfo(missionTabId).Pin())
	{
		tab->OnMissionInfoChanged(missionId, missionPoint);
	}
}

void FGsBattlePassData::MakeBattlePassMissionData(const FGsSchemaBattlePassMissionGroup* missionGroup)
{
	if (missionGroup)
	{
		_passMissionGroup._missionGroupId = missionGroup->passMissionGroupId;

		for (auto& tabDataRow : missionGroup->passMissionTab)
		{
			if (const auto tabData = tabDataRow.GetRow())
			{
				auto tab = _passMissionGroup._missionTabs[_passMissionGroup._missionTabs.Add(MakeShared<FGsBattlePassMissionTab>(tabData))];
				for (auto& missionDataRow : tabData->passMission)
				{
					if (auto missionData = missionDataRow.GetRow())
					{
						auto index = _passMissionGroup._missions.Add(MakeShared<FGsBattlePassMissionData>(missionData));
						tab->_missions.Emplace(_passMissionGroup._missions[index]);
					}
				}

				Algo::ForEach(tab->_missions, [tab](auto e) { tab->_completeExp += e.IsValid() ? e.Pin()->GetCompletePoint() : 0; });
				tab->UpdateMissionTab();
			}
		}
		_passMissionGroup._missionTabs.StableSort([](auto le, auto re) { return le->GetTabIndex() < re->GetTabIndex(); });
	}
}

void FGsBattlePassData::MakeBattlePassLevelData(const FGsSchemaBattlePassLevel* levelGroup)
{
	if (levelGroup)
	{
		TArray<TWeakPtr<FGsBattlePassLevelData>> displayPassLevels;

		_passLevelGroup._levelGroupId = levelGroup->passLevelGroupId;
		for (auto& levelInfo : levelGroup->level)
		{
			int32 levelIndex = _passLevelGroup._levels.Add(MakeShared<FGsBattlePassLevelData>(levelInfo.Key, &(levelInfo.Value)));
			auto passLevelData = _passLevelGroup._levels[levelIndex];
			if (passLevelData->IsLevelForDisplay())
				displayPassLevels.Add(passLevelData);
		}
		_passLevelGroup._levels.StableSort([](auto le, auto re) { return le->GetLevel() < re->GetLevel(); });
		displayPassLevels.StableSort([](auto le, auto re) { return le.Pin()->GetLevel() < re.Pin()->GetLevel(); });

		Level setDPlevel = -1;
		if (auto iter = displayPassLevels.CreateConstIterator())
		{
			setDPlevel = (*iter).Pin()->GetLevel();

			for (auto levelData : _passLevelGroup._levels)
			{
				if (iter)
				{
					auto levelDataLevel = levelData->GetLevel();

					if (setDPlevel > 0 && levelDataLevel < setDPlevel)
					{
						levelData->SetForDisplayLevelData(*iter);
					}
					else
					{
						iter++;
						setDPlevel = iter ? (*iter).Pin()->GetLevel() : -1;
						if (iter) levelData->SetForDisplayLevelData(*iter);
					}
				}
				else break;
			}
		}

		auto maxExp = _passLevelGroup._levels.Last(0)->GetNeedOpenExp();
		_passLevelGroup._bonusLevel = MakeShared<FGsBattlePassLevelBonusData>(levelGroup->bonus, maxExp);
	}
}
