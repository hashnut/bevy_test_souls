
#include "GsBattlePassManager.h"

#include "DataSchema/GsSchemaResetTimeTicketConfig.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsBattlePassMessageParam.h"

#include "UI/UIContent/HUD/GsUIHUDMainMenu.h"
#include "UI/UIContent/Popup/GsUIPopupReceiveAchievementRewardsAll.h"

#include "Net/GsNetSendServiceWorld.h"

#include "UTIL/GsCronUtil.h"

#include "Algo/Copy.h"
#include "Algo/Transform.h"

void UGsBattlePassManager::Initialize()
{
	LoadOpenTabBattlePass();
}

void UGsBattlePassManager::Finalize()
{	
	_battlePass.Reset();
	GetWorld()->GetTimerManager().ClearTimer(_gameTimeHandle);
}

void UGsBattlePassManager::LoadOpenTabBattlePass()
{
	_battlePass.Reset();

	FDateTime svrTime = FGsTimeSyncUtil::GetServerNowDateTime();

	bool enableTimeSet = false;
	bool disableTimeSet = false;

	FDateTime passCheckTime;
	if (auto battlePassTable = Cast<UGsTableBattlePassCommon>(FGsSchemaBattlePassCommon::GetStaticTable()))
	{
		TArray<const FGsSchemaBattlePassCommon*> battlePassList;
		if (battlePassTable->GetAllRows(battlePassList))
		{
			enableTimeSet = GetOpenBattlePassCheck(battlePassList, passCheckTime);

			battlePassList.RemoveAll([this, svrTime](const auto pass) {

					FDateTime passOpenTime;
					FDateTime passEndTime;

					FDateTime::Parse(pass->passPeriodStart, passOpenTime);
					FDateTime::Parse(pass->passPeriodEnd, passEndTime);

					return svrTime < passOpenTime || svrTime > passEndTime;
				});

			for (const auto pass : battlePassList)
			{
				AddDisplayPassData(pass);
			}
		}

		FDateTime passOpenTime = passCheckTime;
		disableTimeSet = GetCloseBattlePassCheck(passOpenTime, passCheckTime, enableTimeSet);
	}

	if (_gameTimeHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(_gameTimeHandle);

	if (disableTimeSet || enableTimeSet)
	{
		auto checkTime = passCheckTime - svrTime; 
		auto totalSec = checkTime.GetTotalSeconds();
		GetWorld()->GetTimerManager().SetTimer(_gameTimeHandle, this, &UGsBattlePassManager::ReloadOpenTabBattlePass, totalSec, false, -1.f);
	}

	if (_battlePass.Num() <= 0)
	{
		GMessage()->GetMainMenuAct().SendMessage(MessageContentHudMainMenuAct::FORCED_DISABLE,
			static_cast<int>(UGsUIHUDMainMenu::EGsMenuType::BATTLEPASS));
	}
}

void UGsBattlePassManager::ReloadOpenTabBattlePass()
{
	bool isForcedDisable = _battlePass.Num() <= 0;

	bool enableTimeSet = false;
	bool disableTimeSet = false;
	bool openNewBattlePass = false;
	bool closeBattlePass = false;

	TMap<BattlePassId, TSharedPtr<FGsBattlePassData>> emptyBattlePass;
	Swap(emptyBattlePass, _battlePass);
	Algo::CopyIf(emptyBattlePass, _battlePass, [](const auto& e) { return e.Value->IsOpenPass(); });

	closeBattlePass = emptyBattlePass.Num() != _battlePass.Num();

	FDateTime svrTime = FGsTimeSyncUtil::GetServerNowDateTime();
	FDateTime passCheckTime;
	if (auto battlePassTable = Cast<UGsTableBattlePassCommon>(FGsSchemaBattlePassCommon::GetStaticTable()))
	{
		TArray<const FGsSchemaBattlePassCommon*> battlePassList;
		if (battlePassTable->GetAllRows(battlePassList))
		{
			enableTimeSet = GetOpenBattlePassCheck(battlePassList, passCheckTime);

			battlePassList.RemoveAll([this, svrTime](const auto pass) {

					FDateTime passOpenTime;
					FDateTime passEndTime;

					FDateTime::Parse(pass->passPeriodStart, passOpenTime);
					FDateTime::Parse(pass->passPeriodEnd, passEndTime);

					return _battlePass.Find(pass->id) || svrTime < passOpenTime || svrTime > passEndTime;
				});

			openNewBattlePass = battlePassList.Num() > 0;
			for (const auto pass : battlePassList)
			{
				AddDisplayPassData(pass);
			}
		}

		FDateTime passOpenTime = passCheckTime;
		disableTimeSet = GetCloseBattlePassCheck(passOpenTime, passCheckTime, enableTimeSet);
	}

	if (_gameTimeHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(_gameTimeHandle);

	if (disableTimeSet || enableTimeSet)
	{
		auto checkTime = passCheckTime - svrTime;
		float totalSec = checkTime.GetTotalSeconds();
		GetWorld()->GetTimerManager().SetTimer(_gameTimeHandle, this, &UGsBattlePassManager::ReloadOpenTabBattlePass, totalSec, false, -1.f);
	}

	if (_battlePass.Num() <= 0)
	{
		GMessage()->GetMainMenuAct().SendMessage(MessageContentHudMainMenuAct::FORCED_DISABLE,
			static_cast<int>(UGsUIHUDMainMenu::EGsMenuType::BATTLEPASS));
	}
	else if (isForcedDisable)
	{
		GMessage()->GetMainMenuAct().SendMessage(MessageContentHudMainMenuAct::NORMAL_RULES,
			static_cast<int>(UGsUIHUDMainMenu::EGsMenuType::BATTLEPASS));
	}

	if (closeBattlePass || openNewBattlePass)
	{
		FGsBattlePassMsgParamPassModify param(openNewBattlePass, closeBattlePass);
		GMessage()->GetBattlePass().SendMessage(MessageContentBattlePass::UPDATE_BATTLEPASS, &param);
	}
}

TWeakPtr<FGsBattlePassData> UGsBattlePassManager::AddDisplayPassData(const FGsSchemaBattlePassCommon* pass)
{
	if (pass == nullptr) return nullptr;

	auto bassData = MakeShared<FGsBattlePassData>(pass);

	bassData->SetOpenPass();
	bassData->SetExposePassTab();
	bassData->SetPassLevelInfo(1, 0);

	_battlePass.Add(pass->id, bassData);

	TArray<TWeakPtr<FGsBattlePassMissionTab>> missionTabs;
	bassData->GetMissionTabList(missionTabs);

	for (auto tab : missionTabs)
	{
		FText descFormat;

		auto resetType = tab.Pin()->GetMissionTabResetType();
		switch (resetType)
		{
		case ResetTimeTicket::RTT_PASS_DAILY: FText::FindText(TEXT("BattlePassText"), TEXT("RTT_PASS_DAILY"), descFormat); break;
		case ResetTimeTicket::RTT_PASS_WEEKLY:  FText::FindText(TEXT("BattlePassText"), TEXT("RTT_PASS_WEEKLY"), descFormat); break;
		default:descFormat = FText::GetEmpty(); break;
		}

		if (descFormat.IsEmpty() == false)
		{
			if (const auto table = Cast<const UGsTableResetTimeTicketConfig>(FGsSchemaResetTimeTicketConfig::GetStaticTable()))
			{
				const FGsSchemaResetTimeTicketConfig* row = nullptr;
				if (table->FindRowById(resetType, row))
				{
					FString outString;
					FGsCronUtil::GetCronString(row->makeCron, outString);

					tab.Pin()->_missionTabDesc = FText::FromString(FString::Format(*descFormat.ToString(), { *outString }));
				}
			}
		}
	}

	return bassData;
}

bool UGsBattlePassManager::GetOpenBattlePassCheck(const TArray<const FGsSchemaBattlePassCommon*>& battlePassList, OUT FDateTime& passCheckTime)
{
	FDateTime svrTime = FGsTimeSyncUtil::GetServerNowDateTime();

	TArray<FDateTime> openableBattlePass;
	Algo::TransformIf(battlePassList, openableBattlePass, [&svrTime](const FGsSchemaBattlePassCommon* pass) {

			FDateTime passOpenTime;
			FDateTime::Parse(pass->passPeriodStart, passOpenTime);
			return passOpenTime > svrTime;

		}, [](const FGsSchemaBattlePassCommon* pass) {

			FDateTime passOpenTime;
			FDateTime::Parse(pass->passPeriodStart, passOpenTime);
			return passOpenTime; 
		});

	if (openableBattlePass.IsValidIndex(0))
	{
		openableBattlePass.StableSort([](auto& ltime, auto& rtime) { return ltime < rtime; });
		passCheckTime = openableBattlePass[0] + FTimespan(0, 0, 5);
		return true;
	}
	return false;
}

bool UGsBattlePassManager::GetCloseBattlePassCheck(IN const FDateTime& passOpenTime, OUT FDateTime& passCheckTime, bool check/* = true*/)
{
	if (_battlePass.Num() > 0)
	{
		TArray<TSharedPtr<FGsBattlePassData>> closeCheckBattlePass;
		_battlePass.GenerateValueArray(closeCheckBattlePass);

		closeCheckBattlePass.StableSort([](const auto lpass, const auto rpass) {
			return lpass->GetBattlePassEndTime() < rpass->GetBattlePassEndTime();
			});

		auto checkTime = closeCheckBattlePass[0]->GetBattlePassEndTime();
		if (check)
		{
			if (checkTime < passOpenTime)
			{
				passCheckTime = checkTime;
				return true;
			}
			else return false;
		}
		else
		{
			passCheckTime = closeCheckBattlePass.Last(0)->GetBattlePassEndTime();
		}
	}
	else return false;

	return true;
}

TWeakPtr<FGsBattlePassData> UGsBattlePassManager::GetBattlePassData(BattlePassId passId)
{
	if (auto iter = _battlePass.Find(passId))
		return *iter;
	return nullptr;
}

int UGsBattlePassManager::GetTabOpendBattlePassList(TArray<TWeakPtr<FGsBattlePassData>>& openBattlePass)
{
	//ReloadOpenTabBattlePass();

	TArray<TSharedPtr<FGsBattlePassData>> battlePassData;
	_battlePass.GenerateValueArray(battlePassData);

	Algo::CopyIf(battlePassData, openBattlePass, [](auto e) { return e->IsExposePassTab(); });

	return openBattlePass.Num();
}

void UGsBattlePassManager::UpdateBattlePassLevel(BattlePassId passId, Level level, Exp levelExp)
{
	if (auto battlePass = GetBattlePassData(passId).Pin())
	{
		battlePass->SetPassLevelInfo(level, levelExp);

		FGsBattlePassMsgParamLevel param(passId, level);
		GMessage()->GetBattlePass().SendMessage(MessageContentBattlePass::UPDATE_LEVEL_EXP, &param);
	}
}

void UGsBattlePassManager::UpdateBattlePassLevelRewarded(BattlePassId passId, Level level, bool isAdditionalReward)
{
	if (auto battlePass = GetBattlePassData(passId).Pin())
	{
		if(isAdditionalReward)
			 battlePass->SetPassLevelAdditionalRewarded(level);
		else battlePass->SetPassLevelRewarded(level);

		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->PlaySound2D(TEXT("UI_RewardOne"));
		}

		FGsBattlePassMsgParamLevelReward param(passId, level, isAdditionalReward);
		GMessage()->GetBattlePass().SendMessage(MessageContentBattlePass::UPDATE_LEVEL_REWARD, &param);
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::BATTLEPASS, true);
	}
}

void UGsBattlePassManager::UpdatePassLevelAdditionalRewardUnlock(BattlePassId passId)
{
	if (auto battlePass = GetBattlePassData(passId).Pin())
	{
		battlePass->SetAdditionalRewardUnlock(true);

		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->PlaySound2D(TEXT("UI_RewardOne"));
		}

		FGsBattlePassMsgParamPass param(passId);
		param._isAddRewardActive = true;
		GMessage()->GetBattlePass().SendMessage(MessageContentBattlePass::UPDATE_ACTIVE_ADDREWARD, &param);
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::BATTLEPASS, true);
	}
}

void UGsBattlePassManager::UpdatePassMissionPoint(BattlePassId passId, BattlePassMissionTabId missionTabId, BattlePassMissionId passMissionId, uint64 missionPoint)
{
	if (auto pass = GetBattlePassData(passId).Pin())
	{
		pass->OnMissionInfoChanged(missionTabId, passMissionId, missionPoint);

		FGsBattlePassMsgParamMission param(passId, missionTabId, passMissionId);
		GMessage()->GetBattlePass().SendMessage(MessageContentBattlePass::UPDATE_MISSIONTAB_MISSION_EXP, &param);
	}
}

void UGsBattlePassManager::UpdateBattlePassList(TArray<BattlePassItem>& items)
{
	for (auto& item : items)
	{
		UpdateBattlePassData(item);
	}
}

void UGsBattlePassManager::UpdateBattlePassData(BattlePassItem& item)
{
	static auto SetLevelData = [](BattlePassItem& item, TWeakPtr<FGsBattlePassLevelData> weakLevel)
	{
		if (auto level = weakLevel.Pin())
		{
			if (item.rewardedLevels.Find(level->GetLevel()) != INDEX_NONE)
				level->SetRewarded();
			if (item.additionalRewardedLevels.Find(level->GetLevel()) != INDEX_NONE)
				level->SetAdditionalRewarded();
		}
	};

	static auto SetMissionData = [](BattlePassItem& item, TWeakPtr<FGsBattlePassMissionTab> weakMissionTab)
	{
		if (auto missionTab = weakMissionTab.Pin())
		{
			if (auto missioniter = item.passMissionIdPoints.Find(missionTab->GetTabId()))
			{
				TArray<TWeakPtr<FGsBattlePassMissionData>> missions;
				missionTab->GetMissionData(missions);

				for (auto weakMission : missions)
				{
					if (auto mission = weakMission.Pin())
					{
						auto missionId = mission->GetMissionId();
						if (auto iter = (*missioniter).FindByPredicate([missionId](auto& e) { return e.Key == missionId; }))
						{
							mission->SetAchieveMissionPoint(iter->Value);
						}
					}
				}
			}
		}
	};

	if(auto battlePass = GetBattlePassData(item.passId).Pin())
	{
		TArray<TWeakPtr<FGsBattlePassLevelData>> levels;
		battlePass->GetPassLevelList(levels);

		for (auto level : levels)
		{
			SetLevelData(item, level);
		}

		TArray<TWeakPtr<FGsBattlePassMissionTab>> missionTabs;
		battlePass->GetMissionTabList(missionTabs);

		for (auto missionTab : missionTabs)
		{
			SetMissionData(item, missionTab);
		}

		battlePass->SetPassLevelInfo(item.level, item.exp);
		battlePass->SetAdditionalRewardUnlock(item.isUseAdditionalReward);
		battlePass->SetOpenPass();
	}
}

void UGsBattlePassManager::ResetTimeContents(ResetTimeTicket rttType)
{
	using ParamType = TPair<BattlePassId, BattlePassMissionTabId>;

	TArray<ParamType> passReseTabs;

	TArray<TSharedPtr<FGsBattlePassData>> passDataList;
	_battlePass.GenerateValueArray(passDataList);

	for (auto pass : passDataList)
	{
		TArray<TWeakPtr<FGsBattlePassMissionTab>> missionTabs;
		pass->GetMissionTabList(missionTabs);

		TArray<TWeakPtr<FGsBattlePassMissionTab>> resetMissionTabs;
		resetMissionTabs = missionTabs.FilterByPredicate([rttType](auto missionTab) { return
			missionTab.IsValid() && missionTab.Pin()->GetMissionTabResetType() == rttType; });

		for (auto tab : resetMissionTabs)
		{
			FText descFormat;

			auto resetType = tab.Pin()->GetMissionTabResetType();
			switch (resetType)
			{
			case ResetTimeTicket::RTT_PASS_DAILY: FText::FindText(TEXT("BattlePassText"), TEXT("RTT_PASS_DAILY"), descFormat); break;
			case ResetTimeTicket::RTT_PASS_WEEKLY:  FText::FindText(TEXT("BattlePassText"), TEXT("RTT_PASS_WEEKLY"), descFormat); break;
			default:descFormat = FText::GetEmpty(); break;
			}

			if (descFormat.IsEmpty() == false)
			{
				if (const auto table = Cast<const UGsTableResetTimeTicketConfig>(FGsSchemaResetTimeTicketConfig::GetStaticTable()))
				{
					const FGsSchemaResetTimeTicketConfig* row = nullptr;
					if (table->FindRowById(resetType, row))
					{
						FString outString;
						FGsCronUtil::GetCronString(row->makeCron, outString);

						tab.Pin()->_missionTabDesc = FText::FromString(FString::Format(*descFormat.ToString(), { *outString }));
					}
				}
			}

			passReseTabs.Add(ParamType(pass->GetBattlePassId(), tab.Pin()->GetTabId()));
			tab.Pin()->ResetMissionTab();
		}
	}

	if (passReseTabs.IsValidIndex(0))
	{
		FGsBattlePassMsgParamMissionTab param(passReseTabs);
		GMessage()->GetBattlePass().SendMessage(MessageContentBattlePass::UPDATE_RESET_MISSIONTAB, &param);
	}

}

void UGsBattlePassManager::OnAckBattlePassRewardAllList(BattlePassId passId, const TArray<CurrencyDeltaPair>& currencyPairList, const TArray<ItemIdAmountPair>& itemAmountPairList)
{
	TMap<uint32, Currency> currencyList;
	for (auto e : currencyPairList)
	{
		currencyList.Add(static_cast<uint32>(e.mCurrencyType), e.mCurrencyDelta);
	}

	TMap<ItemId, ItemAmount> itemList;
	for (auto e : itemAmountPairList)
	{
		itemList.Add(e.mItemId, e.mItemAmount);
	}

	itemList.KeyStableSort([](auto& le, auto& re) {

		uint8 inc = 0;
		uint8 cmp[2] = { 0, 0 };
		for (ItemId e : {le, re})
		{
			if (const FGsSchemaItemCommon* data = FGsItem::GetTableDataByID(e))
			{
				switch (data->type)
				{
				case  ItemType::EQUIP:			cmp[inc] = 3; break;
				case  ItemType::INGREDIENT:		cmp[inc] = 2; break;
				case  ItemType::CONSUME:		cmp[inc] = 1; break;
				default: break;
				}
			}
			inc++;
		}

		return cmp[0] == cmp[1] ? le < re : cmp[0] > cmp[1];
		});

	auto widget = GUI()->OpenAndGetWidget(TEXT("PopupReceiveAchievementRewardAll")).Get();
	if (auto popup = Cast<UGsUIPopupReceiveAchievementRewardsAll>(widget))
	{
		FText title;
		FText btnName;
		FText::FindText(TEXT("BattlePassText"), TEXT("UI_PopUp_GetRewardAll_Title"), title);
		FText::FindText(TEXT("UICommonText"), TEXT("UI_PopUp_AllReceive_Receive"), btnName);

		popup->SetDetail(title, btnName);
		popup->SetData(currencyList, itemList);
		popup->_delegeteClosed = [passId]() {
			FGsNetSendServiceWorld::SendReqBattlePassReceiveRewardAll(passId);
		};
	}
}

void UGsBattlePassManager::OnAckBattlePassReceiveLevelRewardAll(BattlePassItem& rewardResult)
{
	if (auto battlePass = GetBattlePassData(rewardResult.passId).Pin())
	{
		TArray<TWeakPtr<FGsBattlePassLevelData>> levels;
		battlePass->GetPassLevelList(levels);

		TArray<Level> rewardLevels;
		for (auto e : levels)
		{
			if (auto levelData = e.Pin())
			{
				auto level = levelData->GetLevel();
				if (rewardResult.rewardedLevels.Find(level) != INDEX_NONE)
				{
					levelData->SetRewarded();
					rewardLevels.Emplace(level);
				}
				if (rewardResult.additionalRewardedLevels.Find(level) != INDEX_NONE)
				{
					levelData->SetAdditionalRewarded();
					rewardLevels.Emplace(level);
				}
			}
		}

		FGsBattlePassMsgParamLevelRewardAll param(rewardResult.passId, rewardLevels);
		GMessage()->GetBattlePass().SendMessage(MessageContentBattlePass::UPDATE_LEVEL_REWARDALL, &param);
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::BATTLEPASS, true);
	}
}

bool UGsBattlePassManager::IsRedDot()
{
	bool isRedDotOn = false;
	for (auto& pair : _battlePass)
	{
		isRedDotOn = IsRedDotPass(pair.Value->GetBattlePassId());
		if (isRedDotOn) break;
	}
	return isRedDotOn;
}

bool UGsBattlePassManager::IsRedDotPass(BattlePassId passId)
{
	if (auto battlePass = GetBattlePassData(passId).Pin())
	{
		TArray<TWeakPtr<FGsBattlePassLevelData>> levels;
		battlePass->GetPassLevelList(levels);

		return levels.FindByPredicate([](auto e) { 
				return e.IsValid() && (e.Pin()->IsEnableLevelReward() || e.Pin()->IsEnableLevelAdditionalReward()); }) != nullptr ? 
			true : battlePass->GetPassLevelBonusData().IsValid() && battlePass->GetPassLevelBonusData().Pin()->IsEnableBonus();
	}
	return false;

}
