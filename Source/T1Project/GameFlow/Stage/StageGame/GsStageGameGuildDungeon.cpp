// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStageGameGuildDungeon.h"

#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "GsStageGameBase.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"
#include "Management/ScopeGame/GsObserverManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UIContent/Popup/Guild/GuildDungeon/GsUIPopupGuildDungeonReward.h"
#include "UI/UIContent/Window/GsUIWindowDungeonPlayResult.h"
#include "UI/UIContent/HUD/GsUIHUDDungeonTime.h"

#include "Message/GsMessageContentDungeon.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsGuildMessageParam.h"

#include "Net/GsNetSendServiceGuild.h"
#include "Net/GsNetSendServiceWorld.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Observer/GsObserverHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "Guild/GsGuildContributionData.h"
#include "Guild/GsGuildHelper.h"

#include "Runtime/DataCenter/Public/DataSchema/Guild/GsSchemaGuildConfig.h"

const FName WINDOW_DUNGEON_CLEAR = TEXT("WindowDungeonPlayResult");

FGsStageGameGuildDungeon::FGsStageGameGuildDungeon() : FGsStageGameBase(FGsStageMode::Game::DUNGEON)
{
	ClearContribution();
}

FGsStageGameGuildDungeon::~FGsStageGameGuildDungeon()
{
	ClearContribution();
}

void FGsStageGameGuildDungeon::Enter()
{
	FGsStageGameBase::Enter();

	ClearContribution();

	UGsLevelManager* levelMgr = GLevel();
	if (nullptr == levelMgr)
	{
		return;
	}		

	if (FGsUnlockManager* unlockManager = GSUnlock())
	{
		unlockManager->LockContentsByClient(UnlockCategory::GUILD_DUNGEON);
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("HUDMain"));
	if (widget.IsValid())
	{
		TWeakObjectPtr<class UGsUIHUDFrameMain> hudWidget = Cast<UGsUIHUDFrameMain>(widget.Get());
		if (hudWidget.IsValid())
		{
			_hudDungeonTime = hudWidget->GetDungeonTimeHud();
			if (nullptr != _hudDungeonTime)
			{
				_hudDungeonTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
	}

	if (FGsMessageHolder* msg = GMessage())
	{
		_guildDungeonMsg.Emplace(
			msg->GetGuildDungeon().AddRaw(MessageContentGuildDungeon::EXIT, this,
				&FGsStageGameGuildDungeon::OnExitGuildDungeon));
		_guildDungeonMsg.Emplace(
			msg->GetGuildDungeon().AddRaw(MessageContentGuildDungeon::CLEAR, this,
				&FGsStageGameGuildDungeon::OnGuildDungeonClear));
		_guildDungeonMsg.Emplace(
			msg->GetGuildDungeon().AddRaw(MessageContentGuildDungeon::OBSERVING, this,
				&FGsStageGameGuildDungeon::OnObserving));

		
	}	

	GMessage()->GetGuildDungeon().SendMessage(MessageContentGuildDungeon::ENTER, nullptr);

	FGsUIMsgParamContributionMinimapIcon param(true);	
	GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_CONTRIBUTION_BUTTON_ACTIVE, &param);	
}

void FGsStageGameGuildDungeon::Exit()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		for (const MsgGuildDungeonHandle& messagePair : _guildDungeonMsg)
		{
			msg->GetGuildDungeon().Remove(messagePair);
		}


	}
	_guildDungeonMsg.Empty();	
	

	if (nullptr != _hudDungeonTime)
	{
		_hudDungeonTime->SetVisibility(ESlateVisibility::Collapsed);
	}		
	
	ClearContribution();

	GMessage()->GetGuildDungeon().SendMessage(MessageContentGuildDungeon::EXIT, nullptr);

	FGsUIMsgParamContributionMinimapIcon param(false);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_CONTRIBUTION_BUTTON_ACTIVE, &param);

	if (FGsUnlockManager* unlockManager = GSUnlock())
	{
		unlockManager->UnlockContentsByClient(UnlockCategory::GUILD_DUNGEON);
	}

	FGsStageGameBase::Exit();
}

void FGsStageGameGuildDungeon::Update(float In_deltaTime)
{
	FGsStageGameBase::Update(In_deltaTime);


}

void FGsStageGameGuildDungeon::OnExitGuildDungeon(const IGsMessageParam* inParam)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("PopupYesNo"));
	if (false == widget.IsValid())
	{
		return;
	}

	UGsUIPopupYesNo* popup = Cast<UGsUIPopupYesNo>(widget.Get());
	if (popup)
	{
		FText textDesc;		
		// TEXT: 이벤트 공간에서 퇴장하는 경우, ... 퇴장하시겠습니까?			
		FText::FindText(TEXT("GuildText"), TEXT("Dungeon_Leave_Notice"), textDesc);

		popup->SetData(textDesc, [](bool bYes)
			{
				if (bYes)
				{
					// 관전 모드 중이면
					if (true == GSObserver()->GetIsObserveStart())
					{						
						FGsNetSendServiceWorld::SendResurrect(ResurrectType::LASTPOSTION);

						FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
						if (nullptr == gameFlow)
						{
							return;
						}

						FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
						if (nullptr == contents)
						{
							return;
						}

						FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
						if (nullptr == hud)
						{
							return;
						}

						FGsObserverHandler* observerHandler = hud->GetObserverHandler();
						if (observerHandler == nullptr)
						{
							return;
						}
						observerHandler->EndObserve();

					}
					else
					{
						// 이벤트 채널 나가기 패킷 전송					
						FGsNetSendServiceGuild::SendReqExitGuildDungeon();
					}
				}
			});
	}
}

void FGsStageGameGuildDungeon::OnGuildDungeonClear(const IGsMessageParam* inParam)
{	
	const FGsGuildDungeonClearMsgParam* param = inParam->Cast<const FGsGuildDungeonClearMsgParam>();
	if (nullptr == param)
	{
		return;
	}
	
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(WINDOW_DUNGEON_CLEAR);
	UGsUIWindowDungeonPlayResult* castWidget = Cast<UGsUIWindowDungeonPlayResult>(widget);
	if (nullptr == castWidget)
	{
		return;
	}

	castWidget->SetData(param);
}

void FGsStageGameGuildDungeon::ClearContribution()
{
	for (int32 i = 0; i < _contributionArray.Num(); ++i)
	{
		if (_contributionArray[i].IsValid())
		{
			_contributionArray[i].Reset();
		}
	}
	_contributionArray.Empty();
}

void FGsStageGameGuildDungeon::OnObserving(const IGsMessageParam* inParam)
{
	GSLOG(Log, TEXT("%s"), TEXT("Observing start"));
}

void FGsStageGameGuildDungeon::SetContributionArray(PD::SC::PKT_SC_INFORM_GUILD_DUNGEON_RANK_READ* inPkt)
{
	ClearContribution();

	//get guild dungeon stage
	TArray<TSharedPtr<FGsContributionData>> contributionList;
	for (PD::SC::PKT_SC_INFORM_GUILD_DUNGEON_RANK_READ::RankingListIterator iter = inPkt->GetFirstRankingListIterator();
		iter != inPkt->GetLastRankingListIterator();
		iter++)
	{
		_contributionArray.Emplace(MakeShareable(new FGsContributionData(0, iter->ContributeRate(), iter->UserName())));		
	}
	
	GMessage()->GetGuildDungeon().SendMessage(MessageContentGuildDungeon::UPDATE_GUILD_DUNGEON_RANKING, nullptr);
}

void FGsStageGameGuildDungeon::SetExpireTime(FDateTime& inTime)
{
	_expireTime = inTime;
	FGsUIMsgParamDungeonTime param = FGsUIMsgParamDungeonTime(inTime);
	GMessage()->GetDungeonHUDTime().SendMessage(MessageContentDungeonHUDTime::HUD_TIME, &param);
}
void FGsStageGameGuildDungeon::GetSortedRankingList(OUT TArray<TWeakPtr<FGsContributionData>>& OutList)
{
	// 리스트 소팅
	_contributionArray.Sort([](const TSharedPtr<FGsContributionData> InA, const TSharedPtr<FGsContributionData> InB)
		{
			return (InA->_contribution >= InB->_contribution) ? true : false;
		});

	// 랭킹입력
	for (int32 i = 0; i < _contributionArray.Num(); ++i)
	{
		_contributionArray[i]->_rank = i + 1;
	}

	OutList.Append(_contributionArray);
}
