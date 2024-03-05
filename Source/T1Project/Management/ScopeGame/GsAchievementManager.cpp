#include "GsAchievementManager.h"
#include "Achievement/GsAchievementData.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsAchievementMessageParam.h"

#include "UI/UIContent/Tray/GsUITrayTickerAchievementComplete.h"
#include "UI/UIContent/Popup/GsUIPopupItemReceive.h"
#include "UI/UIContent/Popup/GsUIPopupReceiveAchievementRewardsAll.h"
#include "Net/GsNetSendServiceWorld.h"
#include "ForEach.h"
#include "Transform.h"
#include "MaxElement.h"

using AchieveProcess = FGsAchievementData::AchieveProcess;

void UGsAchievementManager::Initialize()
{
	InitAchievementData();
}

void UGsAchievementManager::Finalize()
{
	_achievementsTabList.Reset();
	_achievements.Reset();

	_achievementLevelData.Reset();
	//_tabCategorys.Reset();
}

void UGsAchievementManager::OnUpdateAchievementList(TArray<AchievementItem>& achievementItems, bool isComplete /*= false*/, bool isNewAdd /*= false*/) //TArray<AchievementThemeItem>& themesItems, 
{
	for (auto achievementItem : achievementItems)
	{
		UpdateAchievement(achievementItem, isNewAdd);
	}

	//// sort, reference
	for (auto& e : _achievementsTabList)
	{
		SortAchievements(e.Key);
	}
}

void UGsAchievementManager::OnUpdateAchievement(AchievementItem& achievementItem)
{
	auto result = UpdateAchievement(achievementItem, true);
	if (auto achievement = FindAchievementItem(achievementItem.id).Pin())
	{
		if (result == 0)
		{
			FGsAchievementMsgParamUpdateAchievement msgParma(achievement, achievement->PreAchievePoint(), achievementItem.achievePoint);
			GMessage()->GetAchievement().SendMessage(MessageContentAchievement::UPDATE_ACHIEVEMENT_PROGRESS, &msgParma);
		}
		else if (result == 1)
		{
			FGsAchievementMsgParamChangeAchievement msgParam(achievement);
			GMessage()->GetAchievement().SendMessage(MessageContentAchievement::CHANGE_ACHIEVEMENT_LEVEL, &msgParam);
		}

		SortAchievements(achievement->TabIndex());
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::ACHEIEVEMENT, IsRedDot());
	}
	
}

void UGsAchievementManager::OnUpdateAchievementProgress(AchievementId achievementId, uint64  achievePoint)
{
	AchievementItem achievementItem = { achievementId , achievePoint, false};
	if (auto iter = _achievements.Find(achievementId))
	{
		TSharedPtr<FGsAchievementData> currentAchievement = nullptr;
		TSharedPtr<FGsAchievementData> updateAchievement = *iter;

		auto tabIndex = updateAchievement->TabIndex();
		auto achievementKey = updateAchievement->_groupId;

		if (auto goOniter = _achievementsTabList[tabIndex].FindByPredicate([achievementKey](auto e) {
			return e.IsValid() && e.Pin()->_groupId == achievementKey; }))
		{
			currentAchievement = (*goOniter).Pin();
		}

		achievementItem.isRewarded = updateAchievement->IsRewarded(); 
	}

	OnUpdateAchievement(achievementItem);
}

void UGsAchievementManager::OnRemoveAchievement(AchievementId id) //해당 group achievement 삭제
{
	if (auto achievementIter = _achievements.Find(id))
	{
		auto tabIndex = (*achievementIter)->TabIndex();
		auto groupId = (*achievementIter)->_groupId;

		if (auto listIter = _achievementLevelData.Find(groupId))
		{
			for (auto data : *listIter)
			{
				_achievementsTabList[tabIndex].Remove(data);
				_achievements.Remove(data.Pin()->_achievementId);
			}

			_achievementLevelData.Remove(groupId);
		}
	}
}

void UGsAchievementManager::OnAckReceivedAchievementReward(AchievementId id)
{
	if (auto achievementIter = _achievements.Find(id))
	{
		(*achievementIter)->_isRewarded = true;

		FGsAchievementMsgParamAckAchievementReward msgParam(id);
		GMessage()->GetAchievement().SendMessage(MessageContentAchievement::ACK_RECEIVE_ACHIVEMENT_REWARD, &msgParam);
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::ACHEIEVEMENT, IsRedDot());

		FText title = (*achievementIter)->Title();
		FText formatTxt;
		if (FText::FindText(TEXT("AchievementText"), TEXT("completeTicker_achieveTitle_2"), formatTxt))
		{
			title = FText::Format(formatTxt, title, (*achievementIter)->GetLevel());
		}

		if (auto Ticker = Cast<UGsUITrayTickerAchievementComplete>(GUI()->OpenAndGetWidget(TEXT("TrayTickerAchievementComplete")).Get()))
		{
			UGsUITrayTickerAchievementComplete::FInitData InitData
			{
				UGsUITrayTickerAchievementComplete::AchievementCompleteType::Achievement,
				title, (*achievementIter)->Description()
			};
			Ticker->SetData(InitData);
		}

		SortAchievements((*achievementIter)->TabIndex());
	}
}

void UGsAchievementManager::OnAckAchievementRewardList(const TArray<CurrencyDeltaPair>& currencyPairList, const TArray<ItemIdAmountPair>& itemAmountPairList)
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
		popup->SetData(currencyList, itemList);
		popup->_delegeteClosed = []() {
			FGsNetSendServiceWorld::SendReqAchievementReceiveRewardAll();
		};
	}
}

void UGsAchievementManager::OnAckReceivedRewardAll(const TArray<AchievementId>& achievementIds)
{
	TArray<TSharedPtr<FGsAchievementData>> rewardedData;

	Algo::TransformIf(achievementIds, rewardedData, [this](auto id) {
		return _achievements.Find(id) != nullptr; }, [this](auto id) { return _achievements[id]; });

	for (auto data : rewardedData)
	{
		data->_isRewarded = true;
	}

	if (auto Ticker = Cast<UGsUITrayTickerAchievementComplete>(GUI()->OpenAndGetWidget(TEXT("TrayTickerAchievementComplete")).Get()))
	{
		FText titleText;
		FText descText;
		FText::FindText(TEXT("AchievementText"), TEXT("UI_Ticker_AllReceive_Title2"), titleText);
		FText::FindText(TEXT("AchievementText"), TEXT("Tray_Ticker_Description"), descText);

		UGsUITrayTickerAchievementComplete::FInitData InitData
		{
			UGsUITrayTickerAchievementComplete::AchievementCompleteType::AchievementAll,
			FText::Format(titleText, achievementIds.Num()), descText
		}; 

		Ticker->SetData(InitData);
	}

	FGsAchievementMsgParamAckRewardAll msgParam(achievementIds); //themeIds
	GMessage()->GetAchievement().SendMessage(MessageContentAchievement::ACK_RECEIVE_REWARDALL, &msgParam);
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::ACHEIEVEMENT, IsRedDot());
}

bool UGsAchievementManager::IsRedDot()
{
	bool isRedDotOn = false;
	for (auto e : _achievementsTabList)
	{
		isRedDotOn = isRedDotOn || IsTabRedDot(e.Key);
		if (isRedDotOn) break;
	}
	return isRedDotOn;
}

bool UGsAchievementManager::IsTabRedDot(int32 tabIndex)
{
//	auto themeCategory = _tabCategorys.Find(tabIndex) ? _tabCategorys[tabIndex] : INVALID_ACHIEVEMENT_CATEGORY_ID;
	if (auto iter = _achievementsTabList.Find(tabIndex))
	{
		return (*iter).FindByPredicate([](auto e) { return e.IsValid() && e.Pin()->IsEnableRecvReward(); }) != nullptr;
	}
	return false;
}

int UGsAchievementManager::GetGoOnAchievementsList(int32 tabIndex, OUT MainAchievementPair& pairList) const
{
	//auto themeCategory = _tabCategorys.Find(tabIndex) ? _tabCategorys[tabIndex] : INVALID_ACHIEVEMENT_CATEGORY_ID;

	if (auto iter = _achievementsTabList.Find(tabIndex))
	{
		pairList.Key = nullptr;
		pairList.Value.Append(*iter);
		if (auto mainiter = (*iter).FindByPredicate([](auto e) { return e.IsValid() && e.Pin()->IsMainAchievement(); }))
		{
			pairList.Key = *mainiter;
			pairList.Value.Remove(pairList.Key);
		}

		return pairList.Value.Num();
	}

	return 0;
}

int UGsAchievementManager::GetGroupAchievements(AchievementGroupId id, OUT AchievementList& achievements) const
{
	if (auto iter = _achievementLevelData.Find(id))
	{
		achievements.Append(*iter);
	}
	return achievements.Num();
}

TWeakPtr<FGsAchievementData> UGsAchievementManager::FindAchievementItem(AchievementId achievementId)
{
	if (auto iter = _achievements.Find(achievementId))
		return *iter;
	return nullptr;
}

int32 UGsAchievementManager::GetMaxTabIndex()
{
	static int32 MAXTAB = 0;

	if (MAXTAB == 0)
	{
		if (auto mazIter = Algo::MaxElement(_achievements, [](auto& lPair, auto&rPair) { return lPair.Value->TabIndex() < rPair.Value->TabIndex(); }))
		{
			MAXTAB = (*mazIter).Value->TabIndex();
		}
	}
	return MAXTAB;
}

TWeakPtr<FGsAchievementData> UGsAchievementManager::FindAchievementDataByConditionType(AchievementType condType)
{
	TArray<AchievementList> lists;
	_achievementsTabList.GenerateValueArray(lists);

	AchievementList achievements;
	for (auto& list : lists) achievements.Append(list);

	if (auto iter = achievements.FindByPredicate([condType](auto e) {

		if (const FGsSchemaAchievementCondition* condition = e.IsValid() ? e.Pin()->GetConditionDBData() : nullptr)
			return condition->conditionType == condType;
		return false;  }))
	{
		return (*iter);
	}
	return nullptr;
}

int UGsAchievementManager::UpdateAchievement(AchievementItem& achievementItem, bool isNewAdd )
{
	int result = -1;
	if(auto iter = _achievements.Find(achievementItem.id))
	{
		TSharedPtr<FGsAchievementData> currentAchievement = nullptr;
		TSharedPtr<FGsAchievementData> updateAchievement = *iter;

		auto tabIndex = updateAchievement->TabIndex();
		auto achievementKey = updateAchievement->_groupId;
		auto achievementId = achievementItem.id;

		if (auto goOniter = _achievementsTabList[tabIndex].FindByPredicate([achievementKey](auto e) {
			return e.IsValid() && e.Pin()->_groupId == achievementKey; }))
		{
			currentAchievement = (*goOniter).Pin();
		}

		if (updateAchievement != currentAchievement)
		{
			_achievementsTabList[tabIndex].Remove(currentAchievement);
			_achievementsTabList[tabIndex].Add(updateAchievement);

			auto curlevelIndex = _achievementLevelData[achievementKey].IndexOfByPredicate([currentAchievement](auto e) { return e == currentAchievement; });
			auto newlevelIndex = _achievementLevelData[achievementKey].IndexOfByPredicate([updateAchievement](auto e) { return e == updateAchievement; });
			auto preLevelIndex = _achievementLevelData[achievementKey].IsValidIndex(newlevelIndex - 1) ? newlevelIndex - 1 : 0;

			if (preLevelIndex != curlevelIndex) currentAchievement = _achievementLevelData[achievementKey][preLevelIndex].Pin();

			updateAchievement->SetProcess(AchieveProcess::GoOn);
			updateAchievement->SetInfo(achievementItem.achievePoint, achievementItem.isRewarded, 
				currentAchievement ? currentAchievement->CompletePoint() : 0, isNewAdd);

			for ( int index = curlevelIndex; index < newlevelIndex; index++)
			{
				_achievementLevelData[achievementKey][index].Pin()->SetProcess(AchieveProcess::Complete);
			}
			
			result = isNewAdd == false ? 0 : 1;
		}
		else
		{
			updateAchievement->SetInfo(achievementItem.achievePoint, achievementItem.isRewarded, 0, isNewAdd);
			result = 0;
		}
	}
	return result;
}

void UGsAchievementManager::InitAchievementData()
{
	if (const auto table = Cast<UGsTableAchievementCommon>(FGsSchemaAchievementCommon::GetStaticTable()))
	{
		_achievements.Reset();
		_achievementLevelData.Reset();
		_achievementsTabList.Reset();

		TArray<const FGsSchemaAchievementCommon*> achievementList;
		table->GetAllRows<FGsSchemaAchievementCommon>(achievementList);

		Algo::Transform(achievementList, _achievements, [](const auto dbData) { 
			return TPair<AchievementId, TSharedPtr<FGsAchievementData>>{ dbData->id, MakeShared<FGsAchievementData>(dbData) }; }); // group 분류

		for (auto& pair : _achievements)
		{
			_achievementLevelData.FindOrAdd(pair.Value->_groupId).Add(pair.Value);
		}

		Algo::ForEach(_achievementLevelData, [](auto& pair) { 
			pair.Value.StableSort([](auto le, auto re) { return le.Pin()->GetLevel() < re.Pin()->GetLevel(); }); }); // level sort

		// 가장 낮은 레벨로 current 업적 base Data를 만듬
		TArray<TSharedPtr<FGsAchievementData>> runAchievements;
		Algo::TransformIf(_achievementLevelData, runAchievements, [](auto& pair) { return pair.Value.IsValidIndex(0); }, [](auto& pair) { return pair.Value[0].Pin(); });

		//Tab 별로 업적을 나눈다
		for (auto val : runAchievements)
		{
			_achievementsTabList.FindOrAdd(val->TabIndex()).Add(val);
			val->SetProcess(AchieveProcess::GoOn);
		}
	}
}

void UGsAchievementManager::SortAchievements(int32 tabIndex)
{
	if (auto categoryIter = _achievementsTabList.Find(tabIndex))
	{
		auto& achieveList = *categoryIter;
		achieveList.StableSort([](TWeakPtr<FGsAchievementData> le, TWeakPtr<FGsAchievementData> re) {

			auto leData = le.Pin();
			auto reData = re.Pin();

			auto leLastState = !leData || (leData->IsComplete() && leData->IsRewarded());
			auto reLastState = !reData || (reData->IsComplete() && reData->IsRewarded());

			auto leFirstState = !leLastState && leData->IsComplete();
			auto reFirstState = !reLastState && reData->IsComplete();

			if (reData->IsMainAchievement()) return true;

			return (leFirstState && !reFirstState) || (!leLastState && reLastState);
			});
	}
}
