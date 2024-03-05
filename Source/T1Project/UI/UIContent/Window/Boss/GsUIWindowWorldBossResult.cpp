#include "GsUIWindowWorldBossResult.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Components/WidgetSwitcher.h"
#include "DataSchema/WorldBoss/GsSchemaWorldBossData.h"
#include "Components/TextBlock.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsBossMessageParam.h"
#include "Net/GsNetSendServiceBoss.h"
#include "Reward/GsRewardHelper.h"
#include "UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "Components/ListView.h"
#include "GsUIWorldBossClearPanel.h"
#include "UTIL/GsTimeUtil.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "Components/Image.h"
#include "GameObject/Boss/GsWorldBossHandler.h"
#include "GameObject/Boss/GsBossHandler.h"
#include "Engine/Texture2D.h"

const FTextKey RESULT_LEAVE_GUIDE_TEXT_KEY = TEXT("Result_LeaveGuide");

const int32 LEAVE_TIME = 10;

void UGsUIWindowWorldBossResult::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_leaveButton->OnClicked.AddDynamic(this, &UGsUIWindowWorldBossResult::OnLeaveButton);
}

void UGsUIWindowWorldBossResult::NativeConstruct()
{
	Super::NativeConstruct();

	//bind message
	if (FGsMessageHolder* msg = GMessage())
	{
		_bossDelegates.Emplace(msg->GetBoss().AddUObject(MessageBoss::ALARM_WORLD_BOSS_TIME_OVER
			, this, &UGsUIWindowWorldBossResult::OnFail));
		_bossDelegates.Emplace(msg->GetBoss().AddUObject(MessageBoss::WORLD_BOSS_CLEAR
			, this, &UGsUIWindowWorldBossResult::OnSuccess));
	}

	_remainTimeTextBlock->SetVisibility(ESlateVisibility::Hidden);

	FText::FindText(WORLD_BOSS_TEXT_KEY, RESULT_LEAVE_GUIDE_TEXT_KEY, _remainTimeTextFormat);
	FText::FindText(WORLD_BOSS_TEXT_KEY, TEXT("Worldboss_ClearResult"), _clearResultTextFormat);
	FText::FindText(WORLD_BOSS_TEXT_KEY, TEXT("Worldboss_ClearFail"), _clearFailTextFormat);
}

void UGsUIWindowWorldBossResult::NativeDestruct()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_bossDelegates.Num() > 0)
		{
			for (auto iter : _bossDelegates)
			{
				msg->GetBoss().Remove(iter);
			}
			_bossDelegates.Empty();
		}
	}	

	_dateTimer.Stop(GetWorld());
	_dateTimer.Clear();

	Super::NativeDestruct();
}

void UGsUIWindowWorldBossResult::InvalidBossImage(const struct FGsSchemaWorldBossData* inWorldBossData)
{
	if (UObject* image = inWorldBossData->bossImg.TryLoad())
	{
		_bossImage->SetBrushFromTexture(Cast<UTexture2D>(image));
	}	
}

void UGsUIWindowWorldBossResult::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIWindowWorldBossResult::InvalidBossName(const FGsSchemaWorldBossData* inWorldBossData)
{
	FText title = FText::Format(_clearResultTextFormat, inWorldBossData->bossNpcId.GetRow()->nameText);
	_bossNameTextBlock->SetText(title);
}

void UGsUIWindowWorldBossResult::InvalidParticipateReward()
{	
	FGsBossHandler* bossHandler = FGsBossHandler::GetInstance();
	if (nullptr == bossHandler)
	{
		return;
	}

	FGsWorldBossHandler* worldBossHandler = bossHandler->GetWorldBossHandler();
	if (nullptr == worldBossHandler)
	{
		return;
	}

	const FGsWorldBossRankData& playerRankData = worldBossHandler->GetPlayerRankData();
	_rewardCurrency->SetData(CurrencyType::GOLD, playerRankData._basicRewardGold);
	
	//아이템 리스트 만들어 추가 
	TArray<UGsUIItemIconListViewEntryData*> iconListViewEntryDataArray;
	for (const FGsItemIconData& itemData : playerRankData._basicRewardIdList)
	{
		UGsUIItemIconListViewEntryData* listViewEntryData = NewObject<UGsUIItemIconListViewEntryData>();		
		listViewEntryData->_itemDataArray.Add(itemData);		
		iconListViewEntryDataArray.Add(listViewEntryData);
	}

	_rewardListView->SetListItems(iconListViewEntryDataArray);
}

void UGsUIWindowWorldBossResult::InvalidConsolationReward(const struct FGsSchemaWorldBossData* inWorldBossData)
{
	//모든 리워드 박스를 열어 재화와 아이템을 얻어온다.
	FGsRewardUnboxingData totalData;
	int32 num = inWorldBossData->consolationRewardIdList.Num();
	for (int32 i = 0; i < num; ++i)
	{
		FGsRewardUnboxingData data;
		FGsRewardHelper::UnboxingRewardData(inWorldBossData->consolationRewardIdList[i].GetRow(), data);
		totalData.AddData(data);
	}

	int32 currency = 0;
	if (totalData._tempCurrencyMap.Contains(CurrencyType::GOLD))
	{
		currency = totalData._tempCurrencyMap[CurrencyType::GOLD].Value;		
	}
	_rewardCurrency->SetData(CurrencyType::GOLD, currency);

	totalData.ItemSort();

	//아이템 리스트 만들어 추가 
	TArray<UGsUIRewardIconListViewEntryData*> iconListViewEntryDataArray;
	UGsUIRewardIconListViewEntryData* listViewEntryData = nullptr;
	FGsUIRewardIconData rewardIconData;
	for (const FGsSchemaRewardItemBagData* itemBag : totalData._itemDataList)
	{
		listViewEntryData = NewObject<UGsUIRewardIconListViewEntryData>();

		rewardIconData.Init(itemBag);
		listViewEntryData->_rewardItemDataArray.Add(rewardIconData);

		iconListViewEntryDataArray.Add(listViewEntryData);
	}

	_rewardListView->SetListItems(iconListViewEntryDataArray);
}

void UGsUIWindowWorldBossResult::OnLeaveButton()
{
	FGsNetSendServiceBoss::SendReqWorldBossMapLeave();
}

void UGsUIWindowWorldBossResult::OnFail(const struct IGsMessageParam* inParam)
{
	const FGsWorldBossTimeOverParam* param = inParam->Cast<const FGsWorldBossTimeOverParam>();
	const FGsSchemaWorldBossData* worldBossData = UGsTableUtil::FindRowById<UGsTableWorldBossData, FGsSchemaWorldBossData>(param->bossId);
	if (nullptr == worldBossData)
	{
		return;
	}

	InvalidBossImage(worldBossData);	

	_titleWidgetSwitcher->SetActiveWidgetIndex((int32)EGsResultType::FAIL);

	FText title = FText::Format(_clearFailTextFormat, worldBossData->bossNpcId.GetRow()->nameText);
	_clearFailTextBlock->SetText(title);

	_rewardWidgetSwitcher->SetActiveWidgetIndex((int32)EGsResultType::FAIL);

	if (param->isConsolation)
	{
		_rewardPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		InvalidConsolationReward(worldBossData);
	}	
	else
	{
		_rewardPanel->SetVisibility(ESlateVisibility::Hidden);
	}

	_resultWidgetSwitcher->SetActiveWidgetIndex((int32)EGsResultType::FAIL);

	StartTimer();
}

void UGsUIWindowWorldBossResult::OnSuccess(const struct IGsMessageParam* inParam)
{
	const FGsWorldBossParam* param = inParam->Cast<const FGsWorldBossParam>();
	const FGsSchemaWorldBossData* worldBossData = UGsTableUtil::FindRowById<UGsTableWorldBossData, FGsSchemaWorldBossData>(param->bossId);
	if (nullptr == worldBossData)
	{
		return;
	}

	InvalidBossImage(worldBossData);
	InvalidBossName(worldBossData);

	_titleWidgetSwitcher->SetActiveWidgetIndex((int32)EGsResultType::SUCCESS);

	_rewardPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_rewardWidgetSwitcher->SetActiveWidgetIndex((int32)EGsResultType::SUCCESS);
	InvalidParticipateReward();

	//set reuslt 
	_clearPanel->InvalidResult();

	StartTimer();
}

void UGsUIWindowWorldBossResult::StartTimer()
{
#if WORLD_BOSS_DEBUG
	_endTime = FGsTimeSyncUtil::GetClientNowDateTime() + FTimespan(0, 0, 120);
#else
	_endTime = FGsTimeSyncUtil::GetClientNowDateTime() + FTimespan(0, 0, LEAVE_TIME);
#endif
	_remainTimeTextBlock->SetVisibility(ESlateVisibility::Visible);
	_dateTimer.Start(GetWorld(), _endTime, 0.5f
		, [this]() {UpdateTimer();}
	, [this]() {OnLeaveButton();});	
	UpdateTimer();
}

void UGsUIWindowWorldBossResult::UpdateTimer()
{
	FTimespan remainTime = _dateTimer.GetRemainTime();	
	FText mapName = GLevel()->GetCurrentMapInfo()->MapStringId;
	FText remainTimeText = FText::Format(_remainTimeTextFormat, remainTime.GetTotalSeconds(), mapName);
	
	_remainTimeTextBlock->SetText(remainTimeText);
}
