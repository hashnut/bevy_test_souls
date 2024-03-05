#include "GsUITrayBattleArenaPlayResult.h"
#include "KeyMapping/GsKeyMappingEnum.h"
#include "../Input/GsInputEventMsgBase.h"
#include "../UTIL/GsTimeSyncUtil.h"
#include "../Guild/GsGuildData.h"
#include "../Helper/GsUIHelper.h"
#include "../Guild/GsGuildHelper.h"
#include "../Reward/GsRewardHelper.h"
#include "../Helper/GsTimeStringHelper.h"
#include "../Message/GsMessageInput.h"
#include "../Management/GsMessageHolder.h"
#include "../Management/ScopeGlobal/GsGuildManager.h"
#include "../Management/ScopeGlobal/GsGameFlowManager.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "DataSchema/BattleArena/GsSchemaGuildWarConfigData.h"
#include "../GameFlow/GsGameFlowGame.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaHelper.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaGuildWarHandler.h"
#include "../Common/RedDot/GsUIRedDotDefault.h"
#include "../Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/ListView.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"
#include "../Net/GsNetSendServiceWorld.h"
#include "T1Project.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaData.h"



void UGsUITrayBattleArenaPlayResult::BeginDestroy()
{
	if (_listViewEntrtyPool)
	{
		_listViewEntrtyPool->RemovePool();
		_listViewEntrtyPool = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUITrayBattleArenaPlayResult::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const static int32 POOL_WARM_UP_SIZE = 5;
	_listViewEntrtyPool = NewObject<UGsPoolUObject>(this);
	_listViewEntrtyPool->WarmUp<UGsUIRewardIconListViewEntryData>(POOL_WARM_UP_SIZE);

	_btnConfirm->OnClicked.AddDynamic(this, &UGsUITrayBattleArenaPlayResult::OnClickedOK);
	_btnDetailResult->OnClicked.AddDynamic(this, &UGsUITrayBattleArenaPlayResult::OnClickedDetailScore);
}

void UGsUITrayBattleArenaPlayResult::NativeConstruct()
{
	Super::NativeConstruct();

	/**
	 * 단축키를 막아야 됨
	 */
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::INPUT_LOCK_ALL, true);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

	ClearTimer();

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		GSLOG(Error, TEXT("nullptr == hud"));
		return;
	}

	if (FGsBattleArenaGuildWarHandler* battleArenaHandler = hud->GetBattleArenaGuildWarHandler())
	{
		const FGSBattleArenaGuilWarResultData* resultData = battleArenaHandler->GetResultData();

		if (nullptr == resultData || false == resultData->_isSaved)
		{
			GSLOG(Error, TEXT("nullptr == resultData || false == resultData->_isSaved"));
			return;
		}

		EResult result = (INVALID_GUILD_DB_ID == resultData->_winGuildDBId)
			? EResult::EDraw : (resultData->_winGuildDBId == GGuild()->GetGuildDBId() ? EResult::EWin : EResult::ELose);

		if (EResult::EDraw == result && GuildWarEndReason::TEAM_MEMBER_MINIMUM_CUT == resultData->_endReason)
		{	// 2023/12/28 PKT - 무승부 일때 무승부 사유가 멤버 인원 부족이면, 모두 패배 처리 함.
			// https://jira.com2us.com/jira/browse/C2URWQ-7864
			result = EResult::ELose;
		}

		SetData(result, (*resultData));
	}
}

void UGsUITrayBattleArenaPlayResult::NativeDestruct()
{
	_listViewReward->ClearListItems();
	_listViewEntrtyPool->ReleaseAll();

	ClearTimer();

	/**
	 * 단축키 막은 거 해제
	 */
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::INPUT_LOCK_ALL, false);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

	Super::NativeDestruct();
}

void UGsUITrayBattleArenaPlayResult::OnClickedDetailScore()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		GSLOG(Error, TEXT("nullptr == hud"));
		return;
	}

	if (FGsBattleArenaGuildWarHandler* battleArenaHandler = hud->GetBattleArenaGuildWarHandler())
	{
		battleArenaHandler->TryToOpenPlayDetailResult();

		// 2023/11/28 PKT - 레드닷 Off
		_redDotDetailScore->SetRedDot(false);
	}
}

void UGsUITrayBattleArenaPlayResult::OnClickedOK()
{
	TryExitDungeon();
}

void UGsUITrayBattleArenaPlayResult::TryExitDungeon()
{
	ClearTimer();

	// 2023/11/27 PKT - 던전 나가기
	FGsNetSendServiceWorld::Send_ExitBattleArena();
}

void UGsUITrayBattleArenaPlayResult::SetGuildInfo(UGsUITrayBattleArenaPlayResult::EResult InResult)
{
	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (const FGsGuildDataMine* myGuildData = guildMgr->GetMyGuildData())
		{
			_textGuildName->SetText(FText::FromString(myGuildData->_guildName));

			if (const FGsSchemaGuildEmblem* schemaGuildEmblem = FGsGuildHelper::GetGuildEmblem(myGuildData->_emblemId))
			{
				_imgGuildEmblem->SetBrushFromTexture(FGsUIHelper::LoadTextureFromPath(schemaGuildEmblem->iconPathBig));
				_imgGuildEmblem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				_imgGuildEmblem->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}


void UGsUITrayBattleArenaPlayResult::SetRewardInfo(const TSet<const FGsSchemaRewardItemBagData*>& InData)
{
	TArray<UGsUIRewardIconListViewEntryData*> rewardEntrySet;
	FGsUIRewardIconData rewardIconData;
	for (const auto& itemBag : InData)
	{
		UGsUIRewardIconListViewEntryData* viewEntryData = _listViewEntrtyPool->Claim<UGsUIRewardIconListViewEntryData>();
		viewEntryData->_rewardItemDataArray.Empty();
		rewardIconData.Init(itemBag, itemBag->currencyAmountMin);
		viewEntryData->_rewardItemDataArray.Add(rewardIconData);
		rewardEntrySet.Emplace(viewEntryData);
	}

	_listViewReward->SetListItems(rewardEntrySet);
	_listViewReward->RegenerateAllEntries();
	_listViewReward->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_listViewReward->ScrollIndexIntoView(0);
}

void UGsUITrayBattleArenaPlayResult::OnTimer()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		GSLOG(Error, TEXT("nullptr == hud"));
		return;
	}

	if (FGsBattleArenaGuildWarHandler* battleArenaHandler = hud->GetBattleArenaGuildWarHandler())
	{
		// 2023/11/28 PKT - message format
		FText textFormat;
		FGsBattleArenaHelper::FindText(TEXT("Ui_BattleArena_kickTime"), textFormat);

		// 2023/11/28 PKT - make text
		FText textTime = FText::Format(textFormat, battleArenaHandler->TextStateRemainingTime());

		_textRemainingTime->SetText(textTime);

		const time_t currentTime = FGsTimeSyncUtil::GetServerNowSecond();
		const time_t endTime = battleArenaHandler->GetStateEndTime();
		if (currentTime >= endTime)
		{
			TryExitDungeon();
		}
	}
}

void UGsUITrayBattleArenaPlayResult::ClearTimer()
{
	UWorld* world = GetWorld();
	if (world && _timerHandle.IsValid())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
	}
}

void UGsUITrayBattleArenaPlayResult::SetData(EResult InResult, const FGSBattleArenaGuilWarResultData& InResultData)
{
	// 2023/11/28 PKT - 레드닷 On
	_redDotDetailScore->SetRedDot(true);

	_switcherResult->SetActiveWidgetIndex(StaticCast<int32>(InResult));

	// 2023/12/6 PKT - Sound Effect
	switch (InResult)
	{
	case EResult::EWin:
		FGsBattleArenaHelper::PlayEffectSound(TEXT("UI_KnightArena_Result_Win"));
		break;
	case EResult::ELose:
		FGsBattleArenaHelper::PlayEffectSound(TEXT("UI_KnightArena_Result_Defeat"));
		break;
	case EResult::EDraw:
		FGsBattleArenaHelper::PlayEffectSound(TEXT("UI_KnightArena_Result_Draw"));
		break;
	}

	SetGuildInfo(InResult);

	if (GuildWarEndReason::TEAM_MEMBER_MINIMUM_CUT == InResultData._endReason)
	{	// 2023/12/7 PKT - 부전승으로 승리 하거나 배패 하였다면..
		_textNoticeMinimumMember->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_textNoticeMinimumMember->SetVisibility(ESlateVisibility::Collapsed);
	}

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		GSLOG(Error, TEXT("nullptr == hud"));
		return;
	}

	const FGsBattleArenaGuildWarHandler* battleArenaHandler = hud->GetBattleArenaGuildWarHandler();
	const FGsSchemaGuildWarConfigData* schemaConfigData = (battleArenaHandler) ? battleArenaHandler->GetConfigData(battleArenaHandler->GetMapId()) : nullptr;
	if (schemaConfigData)
	{
		if (InResultData._myPointScore >= schemaConfigData->limitMinRewardPoint || GuildWarEndReason::TEAM_MEMBER_MINIMUM_CUT == InResultData._endReason)
		{	// 2023/12/8 PKT - 보상 받을 수 있음.
			_textNoticeRewardCondition->SetVisibility(ESlateVisibility::Collapsed);

			auto FillRewardSet = [](const TArray<FGsSchemaRewardDataRow>& InData, TSet<const FGsSchemaRewardItemBagData*>& OutData)
			{
				for (int32 idx = 0; idx < InData.Num(); ++idx)
				{
					FGsRewardUnboxingData unboxingData;
					if (InData[idx].GetRow() && true == FGsRewardHelper::UnboxingRewardData(InData[idx].GetRow()->id, unboxingData))
					{
						for (const auto& itemBagData : unboxingData._itemDataList)
						{
							OutData.Emplace(itemBagData);
						}
					}
				}
			};

			TSet<const FGsSchemaRewardItemBagData*> rewardItemBag;

			switch (InResult)
			{
			case EResult::EWin:
				FillRewardSet(schemaConfigData->winRewardIdSet, rewardItemBag);
				break;
			case EResult::ELose:
				FillRewardSet(schemaConfigData->defeatRewardIdSet, rewardItemBag);
				break;
			case EResult::EDraw:
				FillRewardSet(schemaConfigData->drawRewardIdSet, rewardItemBag);
				break;
			}

			SetRewardInfo(rewardItemBag);
		}
		else
		{
			_listViewReward->SetVisibility(ESlateVisibility::Collapsed);

			// 2023/12/7 PKT - "아레나 포인트{ 0 }점 미만은 일반 보상이 지급되지 않습니다."
			FText format;
			FGsBattleArenaHelper::FindText(TEXT("UI_BattleBoard_Reward_Condition"), format);

			FText message = FText::FromString(FString::Format(*format.ToString(), { schemaConfigData->limitMinRewardPoint }));
			_textNoticeRewardCondition->SetText(message);
			_textNoticeRewardCondition->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	if (false == _timerHandle.IsValid())
	{
		_textRemainingTime->SetText(FText::GetEmpty());

		if (UWorld* world = GetWorld())
		{	// 2023/12/8 PKT - Timer를 사용하면 Text가 잠깐 튀는 현상이 발생. 미리 한번 불러 준다.
			OnTimer();
			world->GetTimerManager().SetTimer(_timerHandle, this, &UGsUITrayBattleArenaPlayResult::OnTimer, 0.5f, true);
		}
	}
}