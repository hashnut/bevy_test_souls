// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStageGameAgitDungeon.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Window/GsUIWindowDungeonPlayResult.h"
#include "Net/GsNetSendServiceGuild.h"
#include "Message/MessageParam/GsGuildMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Guild/GsAgitDungeonRaidData.h"
#include "UTIL/GsTimeSyncUtil.h"


FGsStageGameAgitDungeon::FGsStageGameAgitDungeon() : FGsStageGameBase(FGsStageMode::Game::AGIT_DUNGEON)
{
	
}

FGsStageGameAgitDungeon::~FGsStageGameAgitDungeon()
{
	
}

void FGsStageGameAgitDungeon::Enter()
{
	FGsStageGameBase::Enter();

	BindMessages();

	if (FGsUnlockManager* unlockMgr = GSUnlock())
	{
		unlockMgr->LockContentsByClient(UnlockCategory::AGIT_DUNGEON_RAID);
	}

	// HUD 시간 UI 세팅
	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (FGsAgitDataController* agitController = guildMgr->GetAgitDataController())
		{
			if (FGsAgitDungeonRaidData* raidData = agitController->GetRaidData())
			{
				// 시간세팅
				FDateTime closeDateTime = FGsTimeSyncUtil::ConvertToDateTime(raidData->GetDungeonCloseTime());
				FGsUIMsgParamDungeonTime param(closeDateTime);
				GMessage()->GetDungeonHUDTime().SendMessage(MessageContentDungeonHUDTime::HUD_TIME, &param);
				
				// 타이머 시작
				GMessage()->GetDungeonHUDTime().SendMessage(MessageContentDungeonHUDTime::ON_HUD_TIME, nullptr);
			}
		}
	}

	// 기여도 확인 버튼 보여주기
	FGsUIMsgParamContributionMinimapIcon param(true);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_CONTRIBUTION_BUTTON_ACTIVE, &param);

	GUI()->SetHUDMode(EGsUIHUDMode::NORMAL);
}

void FGsStageGameAgitDungeon::Exit()
{
	UnbindMessages();

	// HUD 시간 UI 세팅
	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (FGsAgitDataController* agitController = guildMgr->GetAgitDataController())
		{
			if (FGsAgitDungeonRaidData* raidData = agitController->GetRaidData())
			{
				raidData->ClearDungeonData();
			}
		}
	}

	// 타이머 종료
	GMessage()->GetDungeonHUDTime().SendMessage(MessageContentDungeonHUDTime::OFF_HUD_TIME, nullptr);

	// 기여도 확인 버튼 감추기
	FGsUIMsgParamContributionMinimapIcon param(false);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_CONTRIBUTION_BUTTON_ACTIVE, &param);

	if (FGsUnlockManager* unlockMgr = GSUnlock())
	{
		unlockMgr->UnlockContentsByClient(UnlockCategory::AGIT_DUNGEON_RAID);
	}

	FGsStageGameBase::Exit();
}

//void FGsStageGameAgitDungeon::Update(float In_deltaTime)
//{
//	FGsStageGameBase::Update(In_deltaTime);
//}

void FGsStageGameAgitDungeon::BindMessages()
{
	FGsMessageHolder* msgHolder = GMessage();
	if (nullptr == msgHolder)
	{
		return;
	}

	MGuild& msgGuild = msgHolder->GetGuild();
	_msgHandleGuildList.Emplace(msgGuild.AddRaw(MessageContentGuild::AGIT_DUNGEON_RAID_LEAVE,
		this, &FGsStageGameAgitDungeon::OnDungeonRaidLeave));
	_msgHandleGuildList.Emplace(msgGuild.AddRaw(MessageContentGuild::AGIT_DUNGEON_RAID_RESULT,
		this, &FGsStageGameAgitDungeon::OnDungeonRaidResult));
}

void FGsStageGameAgitDungeon::UnbindMessages()
{
	FGsMessageHolder* msgHolder = GMessage();
	if (nullptr == msgHolder)
	{
		return;
	}

	MGuild& msgGuild = msgHolder->GetGuild();
	for (MsgHandleGuild& msgHandle : _msgHandleGuildList)
	{
		msgGuild.Remove(msgHandle);
	}
	_msgHandleGuildList.Empty();
}

void FGsStageGameAgitDungeon::OnDungeonRaidLeave(const IGsMessageParam*)
{
	// TEXT: 퇴장 시 개인 획득 보상은 지급되지 않습니다...
	FText textMsg;
	FText::FindText(TEXT("GuildText"), TEXT("Raid_PopUp_Out"), textMsg);
	FGsUIHelper::PopupYesNo(textMsg, [](bool bIsOk)
		{
			if (bIsOk)
			{
				FGsNetSendServiceGuild::SendReqAgitDungeonRaidLeave();
			}
		});
}

void FGsStageGameAgitDungeon::OnDungeonRaidResult(const IGsMessageParam* InParam)
{
	const FGsGuildDungeonClearMsgParam* param = InParam->Cast<const FGsGuildDungeonClearMsgParam>();
	if (nullptr == param)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("WindowDungeonPlayResult"));
	if (widget.IsValid())
	{
		if (UGsUIWindowDungeonPlayResult* window = Cast<UGsUIWindowDungeonPlayResult>(widget.Get()))
		{
			window->SetDataAgitDungeonRaid(param);
		}
	}
}
