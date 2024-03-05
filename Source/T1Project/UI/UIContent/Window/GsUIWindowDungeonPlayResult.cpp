

#include "GsUIWindowDungeonPlayResult.h"

#include "../Dungeon/GsDungeonGroupBase.h"
#include "../Dungeon/GsDungeonHelper.h"

#include "../Input/GsInputEventMsgBase.h"
#include "KeyMapping/GsKeyMappingEnum.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"

#include "UTIL/GsTimeSyncUtil.h"
#include "UTIL/GsTableUtil.h"
#include "TextBlock.h"
#include "PaperSprite.h"
#include "Image.h"
#include "ListView.h"
#include "WidgetSwitcher.h"
#include "Currency/GsCurrencyHelper.h"
#include "Message/GsMessageInput.h"
#include "Message/MessageParam/GsGuildMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Common/Entry/GsUIItemIconListViewEntry.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UMG/Public/Components/SizeBox.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "GsTable.h"

#include "Data/GsDataContainManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContents.h"
#include "Net/GsNetSendServiceGuild.h"
#include "Net/GsNetSendServiceWorld.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "DataSchema/Guild/GuildDungeon/GsSchemaGuildDungeon.h"
#include "DataSchema/Guild/Agit/GsSchemaAgitDungeonRaid.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "DataSchema/Dungeon/GsSchemaCoopPartyDungeonConfig.h"



void UGsUIWindowDungeonPlayResult::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_exitButton->OnClicked.AddDynamic(this, &UGsUIWindowDungeonPlayResult::OnClickedExit);
}

void UGsUIWindowDungeonPlayResult::NativeConstruct()
{
	Super::NativeConstruct();

	/**
	 * 2024/2/6 PKT - 
	 * 단축키를 막아야 됨(https://jira.com2us.com/jira/browse/C2URWQ-8335)
	 */
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::INPUT_LOCK_ALL, true);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

	ClearData();
}

void UGsUIWindowDungeonPlayResult::NativeDestruct()
{
	/**
	 * 2024/2/6 PKT -
	 * 단축키 막은 거 해제(https://jira.com2us.com/jira/browse/C2URWQ-8335)
	 */
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::INPUT_LOCK_ALL, false);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

	ClearData();

	Super::NativeDestruct();
}

void UGsUIWindowDungeonPlayResult::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FTimespan elapsedTime = FMath::Clamp<FTimespan>(_countRemainingTime.GetTicks() - FDateTime::Now().GetTicks(), 0, _countRemainingTime.GetTicks());

	InvalidExitRemainingTime(elapsedTime.GetSeconds());

	if (GLevel()->IsMapType(MapType::PARTY_DUNGEON) && false == _forceToExit && 0 >= elapsedTime.GetSeconds())
	{
		if (_exitSenderFunc)
		{	// 2022/06/09 PKT - 서버에 나가기 패킷을 보낸다.
			_exitSenderFunc();
			// 2023/6/17 PKT - 패킷이 중복으로 가는 것을 막기 위해 여기서 버튼은 비활성화 시킨다.
			_exitButton->SetIsEnabled(false);
		}
		_forceToExit = true;
	}
}

void UGsUIWindowDungeonPlayResult::ClearData()
{
	_elapsedTime = 0.f;
	_exitLimitTimeS = 0;

	_forceToExit = false;

	_countRemainingTime = FDateTime(0);

	_exitSenderFunc = nullptr;
		
	_resultSwitcher->SetActiveWidgetIndex(RESULT_SWITCHER_SUCCESS);

	_exitButton->SetIsEnabled(true);

	/**
	 * 던전 공용으로 사용하는 거니.. 따로 초기화가 필요한 Widget은 여기서 초기화
	 */
	_subTitleTextBlock->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::NORMAL_TEXT_COLOR));

	_basicGainCurrencyImage->SetVisibility(ESlateVisibility::Collapsed);
	_specialGainCurrencyImage->SetVisibility(ESlateVisibility::Collapsed);

	const static int32 SWITCHER_DEFAULT_INDEX = 0;
	_switcherRewardInfo->SetActiveWidgetIndex(SWITCHER_DEFAULT_INDEX);
	_textEmptyRewardReason->SetVisibility(ESlateVisibility::Collapsed);
	
	_canvasPanelPersonCurrency->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_canvasPanelGuildCurrency->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIWindowDungeonPlayResult::SetCurrencyImage(CurrencyType InBasicType, CurrencyType InSpecialType)
{
	auto LostImage = [](CurrencyType InFirstType, UImage** OutImage)
	{
		(*OutImage)->SetVisibility(ESlateVisibility::Collapsed);

		UPaperSprite* image = nullptr;
		image = FGsUIHelper::LoadSpriteFromPath(FGsCurrencyHelper::GetCurrencyIconPath(InFirstType));
		if (image)
		{
			(*OutImage)->SetBrushFromAtlasInterface(image);
			(*OutImage)->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	};

	LostImage(InBasicType, &_basicGainCurrencyImage);
	LostImage(InSpecialType, &_specialGainCurrencyImage);
}

void UGsUIWindowDungeonPlayResult::InvalidTitle(int32 InLevel)
{
	const UGsTable* table = FGsSchemaGuildDungeon::GetStaticTable();
	const UGsTableGuildDungeon* guildDungeonTable = Cast<UGsTableGuildDungeon>(table);
	if (nullptr == guildDungeonTable)
	{
		GSLOG(Error, TEXT("nullptr == guildDungeonTable"));
		return;
	}

	const FGsSchemaGuildDungeon* guildDungeon = nullptr;	
	guildDungeonTable->FindRowById(InLevel, guildDungeon);
	if (nullptr == guildDungeon)
	{
		GSLOG(Error, TEXT("not find GuildDungeon : [%d]"), InLevel);
		return;
	}

	// 2022/06/09 PKT - 던전 타이틀
	_titleTextBlock->SetText(guildDungeon->name);

	// 2023/4/11 PKT - https://jira.com2us.com/jira/browse/C2URWQ-3697
	_subTitleTextBlock->SetText(FText::GetEmpty());
	//FString textDungeonLevel = TEXT("(") + FString::FromInt(InLevel) + TEXT(")");
	//_subTitleTextBlock->SetText(FText::FromString(textDungeonLevel));
}

void UGsUIWindowDungeonPlayResult::InvalidTitle(DungeonGroupId InDungeonGroupId, DungeonDifficulty InDifficulty)
{
	FGsDungeonManager* dungeonManager = GSDungeon();
	if (nullptr == dungeonManager)
	{
		GSLOG(Error, TEXT("nullptr == dungeonManager"));
		return;
	}

	const FGsDungeonGroupBase* dungeonGroup = dungeonManager->FindGroupDataById(InDungeonGroupId);
	if (nullptr == dungeonGroup)
	{
		GSLOG(Error, TEXT("nullptr == dungeonGroup : ID[%d]"), InDungeonGroupId);
		return;
	}

	const FGsSchemaDungeonGroupData* schemaData = dungeonGroup->GetSchemaData();
	if (nullptr == schemaData)
	{
		GSLOG(Error, TEXT("nullptr == schemaDungeonGroupData : ID[%d]"), InDungeonGroupId);
		return;
	}

	_titleTextBlock->SetText(schemaData->dungeonGroupName);

	FText text;
	if (FGsDungeonHelper::FindTextDifficulty(InDifficulty, false, text))
	{
		_subTitleTextBlock->SetText(text);
		_subTitleTextBlock->SetColorAndOpacity(FGsUIColorHelper::GetDungeonDifficultyType(InDifficulty));
	}
}

void UGsUIWindowDungeonPlayResult::InvalidPlayTime(const FDateTime& InDateTime)
{
	FString textPlayTime = InDateTime.ToString(TEXT("%M:%S"));
	_playTimeTextBlock->SetText(FText::FromString(textPlayTime));
}

void UGsUIWindowDungeonPlayResult::InvalidBasicReward(CurrencyDelta InAmount, const TArray<ItemIdAmountPair>& InRewardSet)
{
	FString textGetCoin = FString::Printf(TEXT("%llu"), InAmount);
	_basicGainCurrencyAmountTextBlock->SetText(FText::FromString(textGetCoin));

	UGsUIItemIconListViewEntryData* data = nullptr;
	FGsItemIconData itemIconData;
	TArray<UGsUIItemIconListViewEntryData*> rewardDataList;
	for (int32 i = 0; i < InRewardSet.Num(); ++i)
	{
		data = NewObject<UGsUIItemIconListViewEntryData>();

		itemIconData.Init(InRewardSet[i].mItemId, InRewardSet[i].mItemAmount);
		data->_itemDataArray.Add(itemIconData);

		rewardDataList.Add(data);
	}
	_basicRewardListView->SetListItems(rewardDataList);
	_basicRewardListView->SetScrollbarVisibility(ESlateVisibility::Hidden);
}

void UGsUIWindowDungeonPlayResult::InvalidSpecialReward(CurrencyDelta InAmount, const TArray<ItemIdAmountPair>& InRewardSet)
{
	FString textGetCoin = FString::Printf(TEXT("%llu"), InAmount);
	_specialGainCurrencyAmountTextBlock->SetText(FText::FromString(textGetCoin));

	UGsUIItemIconListViewEntryData* data = nullptr;
	FGsItemIconData itemIconData;
	TArray<UGsUIItemIconListViewEntryData*> rewardDataList;
	for (int32 i = 0; i < InRewardSet.Num(); ++i)
	{
		data = NewObject<UGsUIItemIconListViewEntryData>();

		itemIconData.Init(InRewardSet[i].mItemId, InRewardSet[i].mItemAmount);
		data->_itemDataArray.Add(itemIconData);

		rewardDataList.Add(data);
	}
	_specialRewardListView->SetListItems(rewardDataList);
	_specialRewardListView->SetScrollbarVisibility(ESlateVisibility::Hidden);
}

void UGsUIWindowDungeonPlayResult::InvalidGuildReward(const TArray<CurrencyPair>& InCurrencyRewardSet, const TArray<ItemIdAmountPair>& InRewardSet)
{
	UGsUIItemIconListViewEntryData* data = nullptr;
	FGsItemIconData itemIconData;
	TArray<UGsUIItemIconListViewEntryData*> rewardDataList;
	for (int32 i = 0; i < InCurrencyRewardSet.Num(); ++i)
	{
		data = NewObject<UGsUIItemIconListViewEntryData>();

		itemIconData.Init(InCurrencyRewardSet[i].mCurrencyType, InCurrencyRewardSet[i].mCurrency);
		data->_itemDataArray.Add(itemIconData);

		rewardDataList.Add(data);
	}

	for (int32 i = 0; i < InRewardSet.Num(); ++i)
	{
		data = NewObject<UGsUIItemIconListViewEntryData>();

		itemIconData.Init(InRewardSet[i].mItemId, InRewardSet[i].mItemAmount);
		data->_itemDataArray.Add(itemIconData);

		rewardDataList.Add(data);
	}

	_specialRewardListView->SetListItems(rewardDataList);
	_specialRewardListView->SetScrollbarVisibility(ESlateVisibility::Hidden);
}

void UGsUIWindowDungeonPlayResult::InvalidPersonReward(const TArray<CurrencyPair>& InCurrencyRewardSet, const TArray<ItemIdAmountPair>& InRewardSet)
{
	UGsUIItemIconListViewEntryData* data = nullptr;
	FGsItemIconData itemIconData;
	TArray<UGsUIItemIconListViewEntryData*> rewardDataList;
	for (int32 i = 0; i < InCurrencyRewardSet.Num(); ++i)
	{
		data = NewObject<UGsUIItemIconListViewEntryData>();

		itemIconData.Init(InCurrencyRewardSet[i].mCurrencyType, InCurrencyRewardSet[i].mCurrency);
		data->_itemDataArray.Add(itemIconData);

		rewardDataList.Add(data);
	}

	for (int32 i = 0; i < InRewardSet.Num(); ++i)
	{
		data = NewObject<UGsUIItemIconListViewEntryData>();

		itemIconData.Init(InRewardSet[i].mItemId, InRewardSet[i].mItemAmount);
		data->_itemDataArray.Add(itemIconData);

		rewardDataList.Add(data);
	}

	_basicRewardListView->SetListItems(rewardDataList);
	_basicRewardListView->SetScrollbarVisibility(ESlateVisibility::Hidden);
}

void UGsUIWindowDungeonPlayResult::InvalidExitRemainingTime(int32 InRemainingTime)
{
	FText textRemainingTime = FText::Format(_exitRemainingTimeTextFormat, InRemainingTime);
	_exitRemainingTimeTextBlock->SetText(textRemainingTime);
}

void UGsUIWindowDungeonPlayResult::OnClickedExit()
{
	if (_exitSenderFunc)
	{	// 2022/06/09 PKT - 서버에 나가기 패킷을 보낸다.
		_exitSenderFunc();
	}
}

void UGsUIWindowDungeonPlayResult::SetData(const FGsGuildDungeonClearMsgParam* inData)
{
	if (nullptr == inData)
	{
		GSLOG(Error, TEXT("nullptr == inData"));
		return;
	}

	ClearData();

	SetGuildDungeonInter(inData);

	// 2022/06/09 PKT - Set Title
	InvalidTitle(inData->_dungeonKey);

	// 2022/06/09 PKT - 제한 시간.
	_exitLimitTimeS = AFTER_CLEAR_GUILD_DUNGEON_TICK;
	_countRemainingTime = FDateTime::Now() + (AFTER_CLEAR_GUILD_DUNGEON_TICK * ETimespan::TicksPerSecond);
	InvalidExitRemainingTime(AFTER_CLEAR_GUILD_DUNGEON_TICK);

	// 2022/06/09 PKT - Sender
	_exitSenderFunc = FGsNetSendServiceGuild::SendReqExitGuildDungeon;
}

void UGsUIWindowDungeonPlayResult::SetDataAgitDungeonRaid(const FGsGuildDungeonClearMsgParam* InData)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("[Agit] SetDataAgitDungeonRaid InData is null"));
		return;
	}

	ClearData();

	SetGuildDungeonInter(InData);

	const FGsSchemaAgitDungeonRaid* agitRaidData = 
		UGsTableUtil::FindRowById<UGsTableAgitDungeonRaid, FGsSchemaAgitDungeonRaid>(InData->_dungeonKey);
	if (agitRaidData)
	{
		if (const FGsSchemaNpcData* npcData = agitRaidData->bossId.GetRow())
		{
			// TEXT: 영지 던전 {0} 공략
			FText textFormat;
			if (FText::FindText(TEXT("GuildText"), TEXT("Raid_Clear_Tray_Title"), textFormat))
			{
				FText textTitle = FText::Format(textFormat, npcData->nameText);
				_titleTextBlock->SetText(textTitle);
			}
		}
	}
	else
	{
		_titleTextBlock->SetText(FText::GetEmpty());
		
	}

	_subTitleTextBlock->SetText(FText::GetEmpty());

	_exitLimitTimeS = AFTER_CLEAR_AGIT_DUNGEON_RAID_TICK;
	_countRemainingTime = FDateTime::Now() + (AFTER_CLEAR_AGIT_DUNGEON_RAID_TICK * ETimespan::TicksPerSecond);
	InvalidExitRemainingTime(AFTER_CLEAR_AGIT_DUNGEON_RAID_TICK);

	_exitSenderFunc = FGsNetSendServiceGuild::SendReqAgitDungeonRaidLeave;
}

void UGsUIWindowDungeonPlayResult::SetData(DungeonGroupId InDungeonGroupId, DungeonDifficulty InDifficulty
	, const FGsUIMsgParamPartyDungeonResult* inData)
{
	if (nullptr == inData)
	{
		GSLOG(Error, TEXT("nullptr == inData"));
		return;
	}

	ClearData();
	
	// 2024/1/10 PKT - https://jira.com2us.com/jira/browse/C2URWQ-5627 관련 이슈로 주요 보상 재화표기는 숨김.
	_canvasPanelGuildCurrency->SetVisibility(ESlateVisibility::Hidden);

	// 2022/06/09 PKT - Set Text Format...
	FText::FindText(TEXT("PartyDungeonText"), TEXT("Result_KickTime"), _exitRemainingTimeTextFormat);

	FGsDungeonManager* dungeonManager = GSDungeon();
	if (nullptr == dungeonManager)
	{
		GSLOG(Error, TEXT("nullptr == dungeonManager"));
	}

	FText textDesc;

	// 2022/06/10 PKT - 성공 혹은 실패	
	if (inData->_result)
	{
		_resultSwitcher->SetActiveWidgetIndex(RESULT_SWITCHER_SUCCESS);
		FText::FindText(TEXT("PartyDungeonText"), TEXT("Result_Success"), textDesc);
		_successTextBlock->SetText(textDesc);
	}
	else
	{
		_resultSwitcher->SetActiveWidgetIndex(RESULT_SWITCHER_FAILED);
		FText::FindText(TEXT("PartyDungeonText"), TEXT("Result_Failure"), textDesc);
		_failedTextBlock->SetText(textDesc);
	}
	
	// 2022/06/10 PKT - 플레이 시간
	FText::FindText(TEXT("PartyDungeonText"), TEXT("Result_PlayTime_Desc"), textDesc);
	_playTimDescTextBlock->SetText(textDesc);

	// 2022/06/09 PKT - 보상 설명	
	FGsDungeonHelper::FindText(TEXT("PartyDungeon_BasicReward"), textDesc);	
	_basicRewardDescTextBlock->SetText(textDesc);

	FGsDungeonHelper::FindText(TEXT("PartyDungeon_MainReward"), textDesc);
	_specialRewardDescTextBlock->SetText(textDesc);

	// 2022/06/09 PKT - 보상 코인 Load Image
	SetCurrencyImage(inData->_basicCurrencyPair.mCurrencyType, inData->_personCurrencyPair.mCurrencyType);

	// 2022/06/09 PKT - Set Title
	InvalidTitle(InDungeonGroupId, InDifficulty);

	// 2022/06/09 PKT - Play Time
	InvalidPlayTime(inData->_playTime);

	// 2022/06/09 PKT - basic Reward
	InvalidBasicReward(inData->_basicCurrencyPair.mCurrencyDelta, inData->_basicRewardList);

	// 2022/06/09 PKT - special Reward
	InvalidSpecialReward(inData->_personCurrencyPair.mCurrencyDelta, inData->_mainRewardList);

	// 2022/06/09 PKT - remaining Time
	_countRemainingTime = FDateTime::Now() + (dungeonManager->GetPartyDungeonConfig()->afterClearKickTimeS * ETimespan::TicksPerSecond);

	InvalidExitRemainingTime(dungeonManager->GetPartyDungeonConfig()->afterClearKickTimeS);

	if (0 >= inData->_basicRewardList.Num() && 0 >= inData->_mainRewardList.Num())
	{
		const static int32 SWITCHER_EMPTY_REWARD = 1;
		_switcherRewardInfo->SetActiveWidgetIndex(SWITCHER_EMPTY_REWARD);

		if (true == inData->_result)
		{
			_textEmptyRewardReason->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}		
	}
	else
	{
		const static int32 SWITCHER_ENOUGH_REWARD = 0;
		_switcherRewardInfo->SetActiveWidgetIndex(SWITCHER_ENOUGH_REWARD);
	}

	// 2022/06/09 PKT - Sender
	_exitSenderFunc = FGsNetSendServiceWorld::Send_ExitDungeon;
}

void UGsUIWindowDungeonPlayResult::SetGuildDungeonInter(const FGsGuildDungeonClearMsgParam* InData)
{
	_canvasPanelPersonCurrency->SetVisibility(ESlateVisibility::Hidden);
	_canvasPanelGuildCurrency->SetVisibility(ESlateVisibility::Hidden);

	// 2022/06/10 PKT - 클리어
	_resultSwitcher->SetActiveWidgetIndex(RESULT_SWITCHER_SUCCESS);	

	// 2022/06/09 PKT - Set Text Format...
	FText::FindText(GUILD_TEXT_KEY, TEXT("Dungeon_Clear_LeaveMessage1"), _exitRemainingTimeTextFormat);

	// 2022/06/10 PKT - "클리어"
	FText textDesc;
	FText::FindText(GUILD_TEXT_KEY, TEXT("Dungeon_Clear_Title"), textDesc);
	_successTextBlock->SetText(textDesc);

	FText::FindText(GUILD_TEXT_KEY, TEXT("Dungeon_Clear_Time1"), textDesc);
	_playTimDescTextBlock->SetText(textDesc);

	// 2022/06/09 PKT - 보상 설명	
	FText::FindText(GUILD_TEXT_KEY, TEXT("Dungeon_RewardPerson"), textDesc);
	_basicRewardDescTextBlock->SetText(textDesc);

	FText::FindText(GUILD_TEXT_KEY, TEXT("Dungeon_RewardGuild"), textDesc);
	_specialRewardDescTextBlock->SetText(textDesc);

	InvalidPlayTime(InData->_clearTime);

	InvalidGuildReward(InData->_guildRewardCurrencyList, InData->_guildRewardList);

	InvalidPersonReward(InData->_personRewardCurrencyList, InData->_personRewardList);
}