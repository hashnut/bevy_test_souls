// Fill out your copyright notice in the Description page of Project Settings.


#include "GsAgitHandler.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "Guild/GsGuildHelper.h"
#include "DataSchema/Guild/Agit/GsSchemaAgitDungeonRaid.h"
#include "DataSchema/Guild/Agit/GsSchemaAgitDungeonRaidConfig.h"
#include "Message/MessageParam/GsGuildMessageParam.h"
#include "UTIL/GsTableUtil.h"
#include "Net/GsNetSendServiceGuild.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"


bool FGsAgitHandler::CheckAgitError(int32 InResult)
{
	FGsUIHelper::HideBlockUI();

	FText textMsg;

	PD::Result result = static_cast<PD::Result>(InResult);
	switch (result)
	{
	case PD::Result::WARP_ERROR_INVALID_STATUS:
		// TEXT: 이동할 수 없는 상태입니다.
		FText::FindText(TEXT("WorldMapText"), TEXT("ErrorCannotmove"), textMsg);
		break;
	case PD::Result::WARP_ERROR_CAN_NOT_DO_ACTION_WARP:
		// TEXT: 해당 아이템을 사용할 수 없는 상태이거나 사용할 수 없는 장소입니다.
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), textMsg);
		break;
	case PD::Result::AGIT_WARP_ERROR_EXPIRE:
		// TEXT: 기사단 영지를 먼저 대여해주세요.
		FText::FindText(TEXT("GuildText"), TEXT("Agit_EnterButton_Empty"), textMsg);
		break;
	case PD::Result::AGIT_INVALID_RENTAL:
		// TEXT: 대여가 불가능한 상태입니다.
		FText::FindText(TEXT("NetText"), TEXT("AGIT_INVALID_RENTAL"), textMsg);
		break;
	case PD::Result::AGIT_ERROR_NOT_RENTAING:
		// TEXT: 영지 임대 중이 아닙니다
		FText::FindText(TEXT("NetText"), TEXT("AGIT_ERROR_NOT_RENTAING"), textMsg);
		break;
	case PD::Result::AGIT_ERROR_NO_MORE_RENTAL_EXPANSION:
		// TEXT: 더이상 대여 기간을 연장할 수 없습니다
		FText::FindText(TEXT("NetText"), TEXT("AGIT_ERROR_NO_MORE_RENTAL_EXPANSION"), textMsg);
		break;
	case PD::Result::GUILD_ERROR_GRADE:
		// TEXT: 권한이 없습니다
		FText::FindText(TEXT("GuildText"), TEXT("Alert_NoAuthority"), textMsg);
		break;
	case PD::Result::GUILD_ERROR_UNKNOWN_GUILD_ID:
		// TEXT: 기사단이 존재하지 않습니다
		FText::FindText(TEXT("GuildText"), TEXT("UI_Diplomacy_EmptyName"), textMsg);
		break;
	case PD::Result::GUILD_DUNGEON_WARP_ERROR_THE_DAY_JOINED_GUILD:
		// TEXT: 기사단 가입 후 7일 경과 후 입장할 수 있습니다
		FText::FindText(TEXT("GuildText"), TEXT("Dungeon_CanNotJoin_7days"), textMsg);
		break;
	}

	if (false == textMsg.IsEmpty())
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		return true;
	}

	return false;
}

bool FGsAgitHandler::CheckAgitDungeonError(int32 InResult)
{
	FGsUIHelper::HideBlockUI();

	PD::Result result = static_cast<PD::Result>(InResult);
	switch (result)
	{
	case PD::Result::AGIT_DUNGEON_RAID_NOT_GUILD_MEMBER: // 기사단 가입 유저가 아님		
	case PD::Result::AGIT_DUNGEON_RAID_OPEN_GRADE_ERROR: // 오픈 가능 등급이 아님		
	case PD::Result::AGIT_DUNGEON_RAID_CONTENTS_GUILD_LEVEL_ERROR: // 컨텐츠 이용 가능한 기사단 레벨이 아님
	case PD::Result::AGIT_DUNGEON_RAID_GUILD_LEVEL_ERROR: // 오픈 가능 기사단 레벨이 아님
	case PD::Result::AGIT_DUNGEON_RAID_ALREADY_OPENED: // 이미 열린 던전이 있음
	case PD::Result::AGIT_DUNGEON_RAID_NOT_ENOUGH_GUILD_ASSET: // 기사단 운영비 부족
	case PD::Result::AGIT_DUNGEON_RAID_GUILD_JOINED_DAY_ERROR: // 가입 후 일정 기간이 지나지 않았음
	case PD::Result::AGIT_DUNGEON_RAID_MAX_CLEAR_COUNT: // 최대 클리어 횟수 추가
	case PD::Result::AGIT_DUNGEON_RAID_NOT_AVAILABLE_TIME: // 이용 가능 시간 이 아님
	case PD::Result::AGIT_DUNGEON_RAID_DUNGEON_NOT_CLOSED: // 던전이 닫혀있지 않음
	case PD::Result::AGIT_DUNGEON_RAID_DUNGEON_NOT_OPENED: // 던전이 열려있지 않음
	case PD::Result::AGIT_DUNGEON_RAID_WARP_ERROR_ABNORMALITY_EFFECT: // 행동 불가 상태로 워프 불가능
	case PD::Result::AGIT_DUNGEON_RAID_NOT_ENOUGH_USER_TICKET_COUNT: // 유저 티켓 개수 부족
	case PD::Result::AGIT_DUNGEON_RAID_DESTROYING: // 던전이 파괴 중(기사단 던전과 동일)	
	{
		FGsUIHelper::TraySectionMessageTickerNetError(result);
		return true;
	}
	break;
	case PD::Result::AGIT_DUNGEON_RAID_FAIL_TO_CLOSE_USER_EXISTS:
	{
		// TEXT: 잔류중인 기사단원이 있어 원정을 종료할 수 없습니다
		FText textMsg;
		if (FText::FindText(TEXT("GuildText"), TEXT("Raid_Close_Fail_Remain"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			return true;
		}
	}
	break;
	}

	return false;
}

void FGsAgitHandler::Init()
{
	BindMessages();
}

void FGsAgitHandler::Close()
{
	UnbindMessages();
}

void FGsAgitHandler::OnReconnection()
{

}

void FGsAgitHandler::BindMessages()
{
	FGsMessageHolder* msgHolder = GMessage();
	if (nullptr == msgHolder)
	{
		return;
	}

	MGuild& msgGuild = msgHolder->GetGuild();
	_msgHandleGuildList.Emplace(msgGuild.AddRaw(MessageContentGuild::AGIT_LEAVE,
		this, &FGsAgitHandler::OnAgitLeave));
	_msgHandleGuildList.Emplace(msgGuild.AddRaw(MessageContentGuild::AGIT_ALERT_DESTROY,
		this, &FGsAgitHandler::OnAgitAlertDestroy));
	_msgHandleGuildList.Emplace(msgGuild.AddRaw(MessageContentGuild::AGIT_DUNGEON_RAID_START_NOTIFY,
		this, &FGsAgitHandler::OnAgitDungeonRaidStart));
	_msgHandleGuildList.Emplace(msgGuild.AddRaw(MessageContentGuild::AGIT_DUNGEON_RAID_CLOSE_NOTIFY,
		this, &FGsAgitHandler::OnAgitDungeonRaidClose));
	_msgHandleGuildList.Emplace(msgGuild.AddRaw(MessageContentGuild::AGIT_DUNGEON_RAID_CLEAR_NOTIFY,
		this, &FGsAgitHandler::OnAgitDungeonRaidClear));
	_msgHandleGuildList.Emplace(msgGuild.AddRaw(MessageContentGuild::AGIT_DUNGEON_RAID_YET_RESET,
		this, &FGsAgitHandler::OnAgitDungeonResetYet));
}

void FGsAgitHandler::UnbindMessages()
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

void FGsAgitHandler::OnAgitLeave(const IGsMessageParam*)
{
	// TEXT: 기사단 영지에서 퇴장하시겠습니까? (마르샤 해변...)
	FText textMsg;
	FText::FindText(TEXT("GuildText"), TEXT("Agit_OutButton_Desc"), textMsg);
	FGsUIHelper::PopupYesNo(textMsg, [](bool bIsOk)
		{
			if (bIsOk)
			{
				FGsUIHelper::ShowBlockUI();
				FGsNetSendServiceGuild::SendReqAgitLeave(false);
			}
		});
}

void FGsAgitHandler::OnAgitAlertDestroy(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	if (const FGsGuildMsgParamInt* param = InParam->Cast<const FGsGuildMsgParamInt>())
	{
		if (0 >= param->_data)
		{
			return;
		}

		// TEXT: {0}이후 기사단 영지 대여가 만료됩니다.
		FText textFormat;
		if (FText::FindText(TEXT("GuildText"), TEXT("Agit_RentalExpiration_Alert"), textFormat))
		{
			FString strTime;
			FTimespan timespan = FTimespan::FromSeconds(param->_data);			
			FGsTimeStringHelper::GetTimeString(timespan, strTime);

			FText textMsg = FText::Format(textFormat, FText::FromString(strTime));

			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}
	}
}

void FGsAgitHandler::OnAgitDungeonRaidStart(const IGsMessageParam* InParam)
{
	FText textBossName;
	if (InParam)
	{
		if (const FGsGuildMsgParamInt* param = InParam->Cast<const FGsGuildMsgParamInt>())
		{
			GetBossNameByRaidId(param->_data, textBossName);
		}
	}

	FSoftObjectPath iconPath;
	if (const FGsSchemaAgitDungeonRaidConfig* raidConfig = FGsGuildHelper::GetAgitDungeonRaidConfig())
	{
		iconPath = raidConfig->tickerIconPathStart;

		if (UGsSoundPlayer* soundPlayer = GSound()->GetOrCreateSoundPlayer())
		{
			soundPlayer->PlaySound2D(raidConfig->tickerSoundStart.GetRow());
		}
	}

	// TEXT: 기사단 원정대 {0} 원정이 시작되었습니다.
	FText textFormat;
	if (FText::FindText(TEXT("GuildText"), TEXT("Raid_Open_Success"), textFormat))
	{
		FText textMsgFx = FText::Format(textFormat, textBossName);

		FGsUIHelper::TrayTickerInvadeOneText(textMsgFx, iconPath);
		FGsUIHelper::AddChatMessageSystem(textMsgFx);
	}
}

void FGsAgitHandler::OnAgitDungeonRaidClose(const IGsMessageParam* InParam)
{
	FText textBossName;
	if (InParam)
	{
		if (const FGsGuildMsgParamInt* param = InParam->Cast<const FGsGuildMsgParamInt>())
		{
			GetBossNameByRaidId(param->_data, textBossName);
		}
	}

	// TEXT: 기사단 원정대 {0} 원정을 종료했습니다.
	FText textFormat;
	if (FText::FindText(TEXT("GuildText"), TEXT("Raid_Close_Success_Chat"), textFormat))
	{
		FText textMsgFx = FText::Format(textFormat, textBossName);

		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsgFx);
		FGsUIHelper::AddChatMessageSystem(textMsgFx);
	}
}

void FGsAgitHandler::OnAgitDungeonRaidClear(const IGsMessageParam* InParam)
{
	FText textBossName;
	if (InParam)
	{
		if (const FGsGuildMsgParamInt* param = InParam->Cast<const FGsGuildMsgParamInt>())
		{
			GetBossNameByRaidId(param->_data, textBossName);
		}
	}

	FSoftObjectPath iconPath;
	if (const FGsSchemaAgitDungeonRaidConfig* sanctumConfig = FGsGuildHelper::GetAgitDungeonRaidConfig())
	{
		iconPath = sanctumConfig->tickerIconPathClear;

		if (UGsSoundPlayer* soundPlayer = GSound()->GetOrCreateSoundPlayer())
		{
			soundPlayer->PlaySound2D(sanctumConfig->tickerSoundClear.GetRow());
		}
	}

	// TEXT: {0} 원정에 성공했습니다.
	FText textFormat;
	if (FText::FindText(TEXT("GuildText"), TEXT("Raid_Clear_Success"), textFormat))
	{
		FText textMsgFx = FText::Format(textFormat, textBossName);

		FGsUIHelper::TrayTickerInvadeOneText(textMsgFx, iconPath);
		FGsUIHelper::AddChatMessageSystem(textMsgFx);
	}
}

void FGsAgitHandler::GetBossNameByRaidId(int32 InRaidId, OUT FText& OutText)
{
	if (const FGsSchemaAgitDungeonRaid* row =
		UGsTableUtil::FindRowById<UGsTableAgitDungeonRaid, FGsSchemaAgitDungeonRaid>(InRaidId))
	{
		if (const FGsSchemaNpcData* npcData = row->bossId.GetRow())
		{
			OutText = npcData->nameText;
		}
	}
}

void FGsAgitHandler::OnAgitDungeonResetYet(const IGsMessageParam*)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}
	
	// 영지 던전 UI가 열려있을 때만 수행
	if (false == uiMgr->IsActiveWidget("WindowAgitDungeon"))
	{
		return;
	}

	// 영지 던전 초기화가 덜 끝났을 경우인데, 이미 1초 지연요청을 했는데 실패한 상황이다.
	// 따라서 꼬이지 않게 창을 전부 닫고 다시 시도하라고 안내함
	uiMgr->CloseAllStack();

	// TEXT: 컨텐츠가 초기화 되었습니다. 잠시 뒤 다시 시도해주세요.
	FText textMsg;
	if (FText::FindText(TEXT("GuildText"), TEXT("AgitDungeon_ResetTime_Error"), textMsg))
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}
}
