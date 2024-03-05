// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildContentDungeon.h"

#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "UMG/Public/Components/ListView.h"
#include "UMG/Public/Components/ProgressBar.h"
#include "UMG/Public/Components/ListViewBase.h"
#include "UMG/Public/Components/Image.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/Guild/GuildDungeon/GsUIPopupGuildDungeonReward.h"

#include "GuildDungeon/GsGuildDungeonContributionListViewEntry.h"
#include "GuildDungeon/GsGuildDungeonParticipationListViewEntry.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"

#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateGuild.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GsGameFlowGame.h"

#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "DataSchema/Guild/GuildDungeon/GsSchemaGuildDungeon.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"

#include "Reward/GsRewardHelper.h"
#include "Guild/GsGuildHelper.h"
#include "Net/GsNetSendServiceGuild.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Abnormality/GsAbnormalityDataDefine.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerLocalPlayer.h"

#include "UTIL/GsGameObjectUtil.h"


#include "Map/GsMapFunc.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"

#include "EffectText/GsEffectTextParser.h"
#include "UTIL/GsItemUtil.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Sequence/GsSequencePlayer.h"

const FTextKey DUNGEON_STAGE_TEXT_KEY = FTextKey(TEXT("Dungeon_Stage"));
const FTextKey DUNGEON_CURRENT_PERSONS_TEXT_KEY = FTextKey(TEXT("Dungeon_CurrentPersons"));
const FTextKey DUNGEON_LEFT_TIME_TEXT_KEY = FTextKey(TEXT("Dungeon_LeftTime"));
const FTextKey DUNGEON_ENTER_BUTTON_TEXT_KEY = FTextKey(TEXT("Dungeon_EnterButton"));
const FTextKey DUNGEON_NOTIC_ENTER = FTextKey(TEXT("Dungeon_Notice_Enter"));
const FTextKey DUNGEON_REWARD_PERSON_LIST = FTextKey(TEXT("Dungeon_RewardPerson_List"));
const FTextKey DUNGEON_REWARD_JOIN_LIST = FTextKey(TEXT("Dungeon_RewardJoin_List"));
const FTextKey DUNGEON_REWARD_ACQUIRE_LIST = FTextKey(TEXT("Dungeon_RewardGuild_List"));
const FTextKey DUNGEON_CANNOT_JOIN_ABNORMALITY_LIST = FTextKey(TEXT("Dungeon_CanNotJoin_Abnormality"));
const FTextKey DUNGEON_CANNOT_JOIN_ALLCLEAR = FTextKey(TEXT("Dungeon_CanNotJoin_AllClear"));
const FTextKey DUNGEON_LIFETIME = FTextKey(TEXT("Dungeon_LeftTime"));
const FTextKey DUNGEON_LIFETIME2 = FTextKey(TEXT("Dungeon_LeftTime2"));
const FTextKey DUNGEON_LIFETIME3 = FTextKey(TEXT("Dungeon_LeftTime3"));
const FTextKey DUNGEON_CONTRIBUTION_RANK = FTextKey(TEXT("Dungeon_ContributionRankN"));

const FName POPUP_GUILD_DUNGEON_REWARD = TEXT("PopupGuildDungeonReward");

void UGsUIGuildContentDungeon::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIGuildContentDungeon::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_enterDungeonButton->OnClicked.AddDynamic(this, &UGsUIGuildContentDungeon::OnClickEnterGuildDungeon);
	_privateRewardGuildButton->OnClicked.AddDynamic(this, &UGsUIGuildContentDungeon::OnClickPrivateReward);	
	_guildAcquireGuildButton->OnClicked.AddDynamic(this, &UGsUIGuildContentDungeon::OnClickGuildAcquireReward);
}

void UGsUIGuildContentDungeon::NativeConstruct()
{
	Super::NativeConstruct();	

	FText::FindText(GUILD_TEXT_KEY, DUNGEON_STAGE_TEXT_KEY, _dungeonStageText);
	FText::FindText(GUILD_TEXT_KEY, DUNGEON_CURRENT_PERSONS_TEXT_KEY, _dungeonCurrentPersonsText);
	
	FText::FindText(GUILD_TEXT_KEY, DUNGEON_LEFT_TIME_TEXT_KEY, _dungeonResetTicketTimeTextFormat);
	FText::FindText(GUILD_TEXT_KEY, DUNGEON_ENTER_BUTTON_TEXT_KEY, _dungeonEnterButtonText);
	
	FText::FindText(GUILD_TEXT_KEY, DUNGEON_NOTIC_ENTER, _dungeonNoticEnterText);

	FText::FindText(GUILD_TEXT_KEY, DUNGEON_REWARD_JOIN_LIST, _dungeonRewardJoinListText);
	FText::FindText(GUILD_TEXT_KEY, DUNGEON_REWARD_PERSON_LIST, _dungeonRewardPersonListText);
	FText::FindText(GUILD_TEXT_KEY, DUNGEON_REWARD_ACQUIRE_LIST, _dungeonRewardAcquireListText);

	FText::FindText(GUILD_TEXT_KEY, DUNGEON_CANNOT_JOIN_ABNORMALITY_LIST, _abnoramlTickerText);
	FText::FindText(GUILD_TEXT_KEY, DUNGEON_CANNOT_JOIN_ALLCLEAR, _dungoenCanNotJoinAllClearText);

	FText::FindText(GUILD_TEXT_KEY, DUNGEON_LIFETIME, _minutFormat);
	FText::FindText(GUILD_TEXT_KEY, DUNGEON_LIFETIME2, _hourFormat);
	FText::FindText(GUILD_TEXT_KEY, DUNGEON_LIFETIME3, _dayFormat);
	FText::FindText(GUILD_TEXT_KEY, DUNGEON_CONTRIBUTION_RANK, _contributionRankText);

	_maxEntriesCount = FGsGuildHelper::GetGuildConfig()->dungeonTicketLimitCount;	
	_remainTimeTextBlock->SetText(FText());

	InitializeWidget();
	
	if (FGsMessageHolder* msg = GMessage())
	{
		_guildDungeonMsgHandleArray.Emplace(
			msg->GetGuild().AddUObject(MessageContentGuild::INVALID_DUNGOEN_TAB, this,
				&UGsUIGuildContentDungeon::OnInvalidGuildDungeon));
	}	
}

void UGsUIGuildContentDungeon::NativeDestruct()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		for (const TPair<MessageContentGuild, FDelegateHandle> messagePair : _guildDungeonMsgHandleArray)
		{
			msg->GetGuild().Remove(messagePair);
		}
	}

	Super::NativeDestruct();
}

void UGsUIGuildContentDungeon::NativeTick(const FGeometry& inMyGeometry, float inDeltaTime)
{
	Super::NativeTick(inMyGeometry, inDeltaTime);

	if (0 < _checkRemainSec)
	{
		_checkRemainSec -= inDeltaTime;
		if (0 >= _checkRemainSec)
		{
			InvalidateRemainTime();
		}
	}
}

FReply UGsUIGuildContentDungeon::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled();
}

FReply UGsUIGuildContentDungeon::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled();
}

void UGsUIGuildContentDungeon::InvalidPrivateReward()
{	
	bool isShow = GetReward(_guildDungeonData->rewardPersonList
	, _privateRewardItemDataList
	, _privateRewardUnboxingData);

	if (isShow)
	{
		_privateRewardPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		InvalidRewardInternal(_privateRewardListView, _privateRewardUnboxingData, _privateRewardItemDataList);
	}
	else
	{
		_privateRewardPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIGuildContentDungeon::InvalidGuildAcquireReward()
{	
	bool isShow = GetReward(_guildDungeonData->rewardGuildList
		, _rewardAcquireItemDataList, _guildAcquireRewardUnboxingData);

	if (isShow)
	{
		_guildAcquireRewardPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		InvalidRewardInternal(_guildAcquireRewardListView, _guildAcquireRewardUnboxingData, _rewardAcquireItemDataList);
	}
	else
	{
		_guildAcquireRewardPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool UGsUIGuildContentDungeon::GetReward(const TArray<FGsSchemaRewardDataRow>& inRewardArray
, TArray<const struct FGsSchemaRewardItemBagData*>& outRewardItemDataArray
, FGsRewardUnboxingData& outRewardData)
{
	outRewardItemDataArray.Empty();
	outRewardData.Clear();

	if (0 == inRewardArray.Num())
	{
		return false;
	}

	int32 num = inRewardArray.Num();	
	FGsRewardUnboxingData totalRewardData;
	int32 itemId;
	bool isfind;
	for (int32 i = 0; i < num; ++i)
	{		
		if(nullptr == inRewardArray[i].GetRow())
		{
			continue;
		}

		FGsRewardUnboxingData rewardData;
		if (false == FGsRewardHelper::UnboxingRewardData(inRewardArray[i].GetRow()->id, rewardData))
		{
			continue;
		}

		totalRewardData.AddData(rewardData);

		for (const struct FGsSchemaRewardItemBagData* itemBag : rewardData._itemDataList)
		{
			itemId = itemBag->itemId;
			isfind = outRewardItemDataArray.ContainsByPredicate([itemId](const struct FGsSchemaRewardItemBagData* el)
				{
					return el->itemId == itemId;
				});

			if (false == isfind)
			{
				outRewardItemDataArray.Emplace(itemBag);
			}
		}
	}

	outRewardData = totalRewardData;

	const UGsTableItemCommon* itemTable = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	const FGsSchemaItemCommon* sourceItem = nullptr;
	const FGsSchemaItemCommon* destItem = nullptr;
	outRewardItemDataArray.Sort([&](const FGsSchemaRewardItemBagData& inSource, const FGsSchemaRewardItemBagData& inDest)->bool {

		if (itemTable)
		{
			itemTable->FindRowById(inSource.itemId, sourceItem);
			itemTable->FindRowById(inDest.itemId, destItem);

			if (UGsItemUtil::GetItemSortOrder(sourceItem->type) > UGsItemUtil::GetItemSortOrder(destItem->type))
			{
				return true;
			}
			else if(UGsItemUtil::GetItemSortOrder(sourceItem->type) == UGsItemUtil::GetItemSortOrder(destItem->type))
			{
				if ((int32)sourceItem->grade > (int32)destItem->grade)
				{
					return true;
				}
				else if((int32)sourceItem->grade == (int32)destItem->grade)
				{
					if (inSource.itemId > inDest.itemId)
					{
						return true;
					}
				}
			}					
		}

		return false;
		});

	return true;
}

void UGsUIGuildContentDungeon::OnInvalidGuildDungeon(const IGsMessageParam* inParam)
{
	_dungeonResetTime = GGameData()->GetUserData()->GetDungeonResetTime();

	InvalidateAllInternal();
}

void UGsUIGuildContentDungeon::SetGuildDungeonHandler()
{
	//get guild contents
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contentsManagerGame = gameFlow->GetContentsManagerGame();
	if (nullptr == contentsManagerGame)
	{
		return;
	}

	if (FGsContentsMode::InGame::ContentsGuild != contentsManagerGame->GetCurrentContentsMode())
	{
		return;
	}

	_gameStateGuild = contentsManagerGame->GetContentsStateObject<FGsGameStateGuild>(FGsContentsMode::InGame::ContentsGuild);
	_guildDungeonHandler = _gameStateGuild->GetGuildDungeonHandler();
}

void UGsUIGuildContentDungeon::SetGuildDungeonData()
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	_dungeonLevel = userData->GetGuildDungeonLevel();
	_guildDungeonData = FGsGuildHelper::GetGuildDungeonData(_dungeonLevel);
}

void UGsUIGuildContentDungeon::InvalidateAllInternal()
{	
	SetGuildDungeonHandler();
	SetGuildDungeonData();

	if (nullptr == _guildDungeonData)
	{
		return;
	}

	_isClear = _guildDungeonHandler->GetClearAllDungeon();	
	
	InvalidDungoenName();
	InvalidParticipantUser();	
	InvalidEntriesCount();	
	InvalidBossHp();
	InvalidContribution();
	InvalidReward();
	InvalidateRemainTime();
}

void UGsUIGuildContentDungeon::SendRequestPacket()
{
	FGsNetSendServiceGuild::SendReqOpenGuildDungeonInfo();
}

void UGsUIGuildContentDungeon::Show(bool inIsMyGuild, bool inIsStackRecoverFlag /*= false*/)
{
	Super::Show(inIsMyGuild, inIsStackRecoverFlag);		

	InvalidateAllInternal();
}

void UGsUIGuildContentDungeon::Hide()
{
	_checkRemainSec = 0.f;
	_dungeonResetTime = 0.f;

	Super::Hide();
}

void UGsUIGuildContentDungeon::InvalidDungoenName()
{
	if (nullptr == _guildDungeonData)
	{
		return;
	}

	_guildDungeonNameTextBlock->SetText(_guildDungeonData->name);
	FText levelText = FText::Format(_dungeonStageText, _guildDungeonData->level);
	_guildDungeonLevelTextBlock->SetText(levelText);
}

void UGsUIGuildContentDungeon::InvalidParticipantUser()
{
	TArray<TSharedPtr<FGsUserData>> userList = _guildDungeonHandler->GetCurrentEnterUserList();
	_currentUserCount = userList.Num();
	TArray<UGsGuildDungeonParticipationListViewEntryData*> listItemArray;
	UGsGuildDungeonParticipationListViewEntryData* data = nullptr;
	for (int32 i = 0; i < _currentUserCount; ++i)
	{
		data = NewObject<UGsGuildDungeonParticipationListViewEntryData>();
		data->_name = userList[i].Get()->_userName;
		data->_level = userList[i].Get()->_level;
		listItemArray.Add(data);
	}

	_currentParticipantsUserListView->SetListItems(listItemArray);
	FText userCountText = FText::Format(_dungeonCurrentPersonsText, _currentUserCount);
	_currentParticipantsUserCountTextBlock->SetText(userCountText);

	_panelEmptyUserList->SetVisibility((0 == listItemArray.Num()) ? 
		ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	_currentParticipantsUserListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
}

void UGsUIGuildContentDungeon::InvalidRemainTime(FTimespan& inTimeSpan)
{
	if (inTimeSpan.GetTotalSeconds() > 0)
	{
		FText textTime;
		if (inTimeSpan.GetDays() > 0)
		{
			textTime = FText::Format(_dayFormat, inTimeSpan.GetDays(), inTimeSpan.GetHours());
		}
		else if (inTimeSpan.GetHours() > 0)
		{
			textTime = FText::Format(_hourFormat, inTimeSpan.GetHours(), inTimeSpan.GetMinutes());
		}
		else
		{
			textTime = FText::Format(_minutFormat, inTimeSpan.GetMinutes(), inTimeSpan.GetSeconds());
		}

		_remainTimeTextBlock->SetText(textTime);
		_remainTimeTextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_remainTimeTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}


#if GUILD_DUNGEON_DEBUG
	GSLOG(Log, TEXT("guild dungeon debug : %s"), *inTimeSpan.ToString());
#endif
}

void UGsUIGuildContentDungeon::InvalidateRemainTime()
{
	if (0 == _dungeonResetTime)
	{
		_enterDungeonButton->SetIsEnabled(false);
		FTimespan emptyTime = FTimespan::FromSeconds(0);

		InvalidRemainTime(emptyTime);

		return;
	}

	// 버튼 활성화 체크
	_enterDungeonButton->SetIsEnabled((0 < _currentEntriesCount) ? true : false);
		
	time_t remainSec = _dungeonResetTime - FGsTimeSyncUtil::GetServerNowSecond();
	if (0 >= remainSec)
	{
		FTimespan emptyTime = FTimespan::FromSeconds(0);
		InvalidRemainTime(emptyTime);

		_checkRemainSec = 0.f;

		// 시간이 다했으므로 강제 갱신
		FGsNetSendServiceGuild::SendReqOpenGuildDungeonInfo();
	}
	else
	{
		FTimespan remainTime = FTimespan::FromSeconds(remainSec);
		InvalidRemainTime(remainTime);

		_checkRemainSec = TIME_UPDATE_SEC;
	}
}

void UGsUIGuildContentDungeon::InvalidEntriesCount()
{
	_currentEntriesCount = _guildDungeonHandler->GetTicketCount();	
	FText remainEntriesCountText = FText::Format(_dungeonEnterButtonText, _currentEntriesCount, _maxEntriesCount);	
	_enteriesCountTextBlock->SetText(remainEntriesCountText);
}

void UGsUIGuildContentDungeon::InvalidBossHp()
{
	_bossHpWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (_isClear)
	{
		_bossHpProgressBar->SetPercent(0);
		_bossHpPercentText = FString::Printf(TEXT("%.2f%%"), 0);
		_bossHpTextBlock->SetText(FText::FromString(_bossHpPercentText));
	}
	else
	{
		if (nullptr == _guildDungeonData)
		{
			return;
		}

		_bossHpProgressBar->SetPercent(_guildDungeonHandler->GetBossHpPercent() / 100.0f);
		_bossHpPercentText = FString::Printf(TEXT("%.2f%%"), _guildDungeonHandler->GetBossHpPercent());
		_bossHpTextBlock->SetText(FText::FromString(_bossHpPercentText));
	}	
}

void UGsUIGuildContentDungeon::InvalidContribution()
{	
	FText contributionRankTitle = FText::Format(_contributionRankText, _dungeonLevel);

	_contributionRankTextBlock->SetText(contributionRankTitle);

	TArray<TSharedPtr<FGsContributionUserData>> constributionUserList = _guildDungeonHandler->GetContributionUserList();
	_contributionUserCount = constributionUserList.Num();	
	TArray<UGsGuildDungeonContributionListViewEntryData*> listItemArray;
	UGsGuildDungeonContributionListViewEntryData* data = nullptr;
	for (int32 i = 0; i < _contributionUserCount; ++i)
	{
		data = NewObject<UGsGuildDungeonContributionListViewEntryData>();
		data->_rank = constributionUserList[i].Get()->_rank;
		data->_name = constributionUserList[i].Get()->_userName;
		data->_contribution = constributionUserList[i].Get()->_contribution;
		listItemArray.Add(data);
	}

	if (0 < listItemArray.Num())
	{
		_panelEmptyContributoinList->SetVisibility(ESlateVisibility::Collapsed);
		_contributionListView->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_contributionListView->SetListItems(listItemArray);
	}
	else
	{
		_panelEmptyContributoinList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_contributionListView->SetVisibility(ESlateVisibility::Collapsed);
	}

	TSharedPtr<FGsContributionUserData> userData = _guildDungeonHandler->GetPlayerContribution();
	if (userData.IsValid())
	{
		data = NewObject<UGsGuildDungeonContributionListViewEntryData>();
		data->_rank = userData->_rank;
		data->_name = userData->_userName;
		data->_contribution = userData->_contribution;
		_playerContribution->NativeOnListItemObjectSet(data);
	}	
	else
	{
		_playerContribution->Clear();
	}

	_contributionListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
}

void UGsUIGuildContentDungeon::InitializeWidget()
{
	_bossImage->SetVisibility(ESlateVisibility::Hidden);
	_bossHpWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIGuildContentDungeon::InvalidBossImage()
{
	if (false == _guildDungeonData->dungeonBossImg.IsValid())
	{
		return;
	}

	_bossImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	UTexture2D* image = Cast<UTexture2D>(UAssetManager::GetStreamableManager().LoadSynchronous(_guildDungeonData->dungeonBossImg));
	if (image)
	{
		_bossImage->SetBrushFromTexture(image);
	}	
}

void UGsUIGuildContentDungeon::InvalidReward()
{	
	InvalidPrivateReward();	
	InvalidGuildAcquireReward();	
	InvalidBossImage();
}

void UGsUIGuildContentDungeon::InvalidRewardInternal(class UListView* inList, FGsRewardUnboxingData& inReward, TArray<const struct FGsSchemaRewardItemBagData*>& inBagData)
{	
	TArray<UGsUIRewardIconListViewEntryData*> iconListViewEntryDataArray;
	UGsUIRewardIconListViewEntryData* listViewEntryData = nullptr;
	
	for (const TPair<CurrencyType, TPair<Currency, Currency>>& currencyPair : inReward._tempCurrencyMap)
	{
		listViewEntryData = NewObject<UGsUIRewardIconListViewEntryData>();
		FGsUIRewardIconData rewardIconData;
		rewardIconData.Init(currencyPair.Key, currencyPair.Value.Key, currencyPair.Value.Value);
		listViewEntryData->_rewardItemDataArray.Add(rewardIconData);

		iconListViewEntryDataArray.Add(listViewEntryData);
	}

	for (const FGsSchemaRewardItemBagData* itemBag : inBagData)
	{
		listViewEntryData = NewObject<UGsUIRewardIconListViewEntryData>();	
		FGsUIRewardIconData rewardIconData;
		rewardIconData.Init(itemBag, itemBag->itemCountMin, itemBag->itemCountMax);
		listViewEntryData->_rewardItemDataArray.Add(rewardIconData);

		iconListViewEntryDataArray.Add(listViewEntryData);
	}

	inList->SetListItems(iconListViewEntryDataArray);
	inList->ScrollIndexIntoView(0);
	inList->SetScrollbarVisibility(ESlateVisibility::Collapsed);
}

void UGsUIGuildContentDungeon::OnClickEnterGuildDungeon()
{
	//시네마틱 영상 플레이 중 길드 던전 진입 막음
	if (UGsSequencePlayer::IsSequencePlaying())
	{
		return;
	}

	if (_isClear)
	{
		FGsUIHelper::TrayMessageTicker(_dungoenCanNotJoinAllClearText);
		return;
	}

	//If player is in abnormal state, player can't enter guild dungeon
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == local)
	{
		return;
	}	

	if (local->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_WARP))
	{		
		FGsUIHelper::TrayMessageTicker(_abnoramlTickerText);
		return;
	}

	FText findText;
	FText::FindText(GUILD_TEXT_KEY, DUNGEON_NOTIC_ENTER, findText);
	
	FGsUIHelper::PopupYesNo(findText,
		[this](bool inSuccess) {
			//send enter guild packet
			if (inSuccess)
			{
				// https://jira.com2us.com/jira/browse/C2URWQ-2476
				// check input lock(quest dialog server send -> try warp)
				if (true == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE))
				{
					return;
				}

				FGsNetSendServiceGuild::SendReqEnterGuildDungeon();
				// 컨텐츠에서 워프가 발생여부 알림
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CONTENTS_WARP_REQUEST, nullptr);

				if (FGsInteractionManager* interactionManager = GSInteraction())
				{
					interactionManager->SetGuildDungeonWarpReq(true);
				}
			}			
		});
}

void UGsUIGuildContentDungeon::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

void UGsUIGuildContentDungeon::OnClickPrivateReward()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(POPUP_GUILD_DUNGEON_REWARD);
	UGsUIPopupGuildDungeonReward* rewardWidget = Cast<UGsUIPopupGuildDungeonReward>(widget.Get());
	if (nullptr == rewardWidget)
	{
		return;
	}

	rewardWidget->SetData(_dungeonRewardPersonListText, _privateRewardItemDataList, _privateRewardUnboxingData);
}

void UGsUIGuildContentDungeon::OnClickGuildAcquireReward()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(POPUP_GUILD_DUNGEON_REWARD);
	UGsUIPopupGuildDungeonReward* rewardWidget = Cast<UGsUIPopupGuildDungeonReward>(widget.Get());
	if (nullptr == rewardWidget)
	{
		return;
	}

	rewardWidget->SetData(_dungeonRewardAcquireListText, _rewardAcquireItemDataList, _guildAcquireRewardUnboxingData);
}