// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNetMessageHandlerPKBook.h"
#include "../PKBook/GsPKBookHelper.h"
#include "../Guild/GsGuildData.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsPKBookManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Message/GsMessageNet.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/PKBook/GsPKBookhandler.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "../Message/GsMessageContents.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"



void FGsNetMessageHandlerPKBook::InitializeMessage(FGsNetManager* InManager)
{
	// 패킷 바인딩
	MProtocalWorld& WorldProtocol = InManager->GetProtocalWorld();

	InitializeMessageDelegateList(&WorldProtocol);

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_PK_BOOK_LIST
		, this
		, &FGsNetMessageHandlerPKBook::NOTIFY_PK_BOOK_LIST));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ADD_PK_BOOK_INFO
		, this
		, &FGsNetMessageHandlerPKBook::NOTIFY_ADD_PK_BOOK_INFO));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_PK_BOOK_DETAIL
		, this
		, &FGsNetMessageHandlerPKBook::ACK_PK_BOOK_DETAIL));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_PK_BOOK_REVENGE
		, this
		, &FGsNetMessageHandlerPKBook::ACK_PK_BOOK_REVENGE));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_PK_BOOK_TEASE
		, this
		, &FGsNetMessageHandlerPKBook::ACK_PK_BOOK_TEASE));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_MORNITORING_USER_LIST
		, this
		, &FGsNetMessageHandlerPKBook::NOTIFY_MORNITORING_USER_LIST));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_ADD_MONITORING_USER
		, this
		, &FGsNetMessageHandlerPKBook::ACK_ADD_MONITORING_USER));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_DELETE_MONITORING_USER
		, this
		, &FGsNetMessageHandlerPKBook::ACK_DELETE_MONITORING_USER));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_PK_MESSAGE_INFO
		, this
		, &FGsNetMessageHandlerPKBook::NOTIFY_PK_MESSAGE_INFO));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_MONITORING_USER_LOGIN_INFO
		, this
		, &FGsNetMessageHandlerPKBook::NOTIFY_MONITORING_USER_LOGIN_INFO));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_SAVE_PK_MESSAGE
		, this
		, &FGsNetMessageHandlerPKBook::ACK_SAVE_PK_MESSAGE));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_PK_BOOK_TEASE_MESSAGE
		, this
		, &FGsNetMessageHandlerPKBook::NOTIFY_PK_BOOK_TEASE_MESSAGE));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_SAVE_PK_TEASE_MESSAGE
		, this
		, &FGsNetMessageHandlerPKBook::ACK_SAVE_PK_TEASE_MESSAGE));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_INFORM_I_GOT_PK
		, this
		, &FGsNetMessageHandlerPKBook::NOTIFY_PK_GUILD_I_GOT_PK));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_INFORM_I_DID_PK
		, this
		, &FGsNetMessageHandlerPKBook::NOTIFY_PK_GUILD_I_DID_PK));

	
}

void FGsNetMessageHandlerPKBook::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}


void FGsNetMessageHandlerPKBook::NOTIFY_PK_BOOK_LIST(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_PK_BOOK_LIST_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_PK_BOOK_LIST_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	FGsPKBookManager* manager = GsPKBook();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	manager->MakeRecordDataSet(*Packet);
}

void FGsNetMessageHandlerPKBook::NOTIFY_ADD_PK_BOOK_INFO(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ADD_PK_BOOK_INFO_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ADD_PK_BOOK_INFO_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	FGsPKBookManager* manager = GsPKBook();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	manager->AddPKRecordData(*Packet);

	// 2022/09/22 PKT - 레코딩 사유가 처치 / 사망일 경우 세팅되어진 메세지를 서버에 보낸다.
	if (PKBookResult::KILL == Packet->PkBookResult() || PKBookResult::DIE == Packet->PkBookResult())
	{
		// 2022/09/21 PKT - 조롱 메세지를 받음.
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

		FGsPKBookHandler* PKBookHabdler = hud->GetPKBookHandler();
		if (nullptr == PKBookHabdler)
		{
			return;
		}

		PKBookHabdler->SendChatMessagePKBookResult(Packet->PkBookResult());
	}
}

void FGsNetMessageHandlerPKBook::ACK_PK_BOOK_DETAIL(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_PK_BOOK_DETAIL_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ACK_PK_BOOK_DETAIL_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	const PD::Result Result = StaticCast<PD::Result>(Packet->Result());
	if (Result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(Packet->Result()));
		return;
	}

	FGsPKBookManager* manager = GsPKBook();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	manager->SetPKRecordDetailData(*Packet);
}

void FGsNetMessageHandlerPKBook::ACK_PK_BOOK_REVENGE(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_PK_BOOK_REVENGE_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ACK_PK_BOOK_REVENGE_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	const PD::Result Result = StaticCast<PD::Result>(Packet->Result());
	if (Result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(Packet->Result()));
		return;
	}

	// 2023/5/18 PKT - 이후 워프가 진행 되므로 여기서 화면을 블럭 처리 힌다.
	if (false == FGsUIHelper::IsOpenBlockUI())
	{
		FGsUIHelper::ShowBlockUI();
	}	

	FGsPKBookManager* manager = GsPKBook();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	manager->SetRevengeRemaindCount(*Packet);

	// 2022/11/02 PKT - 1. 열려 있는 PK BOOK 닫힘
	GMessage()->GetContents().SendMessage(MessageContents::CLOSE_WINDOW_PK_BOOK);
}

void FGsNetMessageHandlerPKBook::ACK_PK_BOOK_TEASE(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_PK_BOOK_TEASE_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ACK_PK_BOOK_TEASE_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	const PD::Result Result = StaticCast<PD::Result>(Packet->Result());
	if (Result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(Packet->Result()));
		return;
	}

	FGsPKBookManager* manager = GsPKBook();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	manager->SetTeaseRemaindCount(*Packet);
}

void FGsNetMessageHandlerPKBook::NOTIFY_MORNITORING_USER_LIST(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_MORNITORING_USER_LIST_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_MORNITORING_USER_LIST_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	FGsPKBookManager* manager = GsPKBook();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	manager->MakePKDetectionTargetSet(*Packet);
}

void FGsNetMessageHandlerPKBook::ACK_ADD_MONITORING_USER(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_ADD_MONITORING_USER_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ACK_ADD_MONITORING_USER_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	const PD::Result Result = StaticCast<PD::Result>(Packet->Result());
	if (Result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(Packet->Result()));
		return;
	}

	FGsPKBookManager* manager = GsPKBook();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	manager->AddPKDetectionTarget(*Packet);

	/**
	 * 2022/10/04 PKT - 게임 아이디에 값이 있으면 현재 로그인 중인 유저임.
	 */
	const static int64 INVALIDATE_GAME_ID = 0;
	if (INVALIDATE_GAME_ID < Packet->GameId())
	{	// 2022/09/30 PKT - 현재 로그인 중인 유저라면 미니맵 표기를 위해 핸들러로 정보를 넘긴다.
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

		FGsPKBookHandler* PKBookHabdler = hud->GetPKBookHandler();
		if (nullptr == PKBookHabdler)
		{
			return;
		}
		
		PKBookHabdler->AddDetectionTarget(Packet->UserDBId(), Packet->GameId());
	}

	
}

void FGsNetMessageHandlerPKBook::ACK_DELETE_MONITORING_USER(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_DELETE_MONITORING_USER_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ACK_DELETE_MONITORING_USER_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	const PD::Result Result = StaticCast<PD::Result>(Packet->Result());
	if (Result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(Packet->Result()));
		return;
	}

	FGsPKBookManager* manager = GsPKBook();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	manager->RemovePKDetectionTarget(*Packet);

	/**
	 * 2022/10/04 PKT - 게임 아이디에 값이 있으면 현재 로그인 중인 유저임.
	 */
	const static int64 INVALIDATE_GAME_ID = 0;
	if (INVALIDATE_GAME_ID < Packet->GameId())
	{	// 2022/09/30 PKT - 현재 로그인 중인 유저라면 미니맵 표기를 위해 핸들러로 정보를 넘긴다.
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

		FGsPKBookHandler* PKBookHabdler = hud->GetPKBookHandler();
		if (nullptr == PKBookHabdler)
		{
			return;
		}

		PKBookHabdler->ClearDetectionTarget(Packet->GameId());
	}
}

void FGsNetMessageHandlerPKBook::NOTIFY_PK_MESSAGE_INFO(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_PK_MESSAGE_INFO_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_PK_MESSAGE_INFO_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	FGsPKBookManager* manager = GsPKBook();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	manager->MakePKMessageConfig(*Packet);
}

void FGsNetMessageHandlerPKBook::NOTIFY_MONITORING_USER_LOGIN_INFO(const FGsNet::Packet& InPacket)
{
	// 2022/09/21 PKT - 경계 대상이 로그인/아웃 했을때.
	PD::SC::PKT_SC_MONITORING_USER_LOGIN_INFO_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_MONITORING_USER_LOGIN_INFO_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	FGsPKBookManager* manager = GsPKBook();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	// 2022/09/28 PKT - Manager
	manager->LogInDetectionTarget(*Packet);

	// 2022/09/21 PKT - 로그인일때는 알림..
	if (Packet->IsLogin())
	{
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

		FGsPKBookHandler* PKBookHabdler = hud->GetPKBookHandler();
		if (nullptr == PKBookHabdler)
		{
			return;
		}

		PKBookHabdler->DetectionTargetLogOnMessage(Packet->UserDBId());
	}
}

void FGsNetMessageHandlerPKBook::ACK_SAVE_PK_MESSAGE(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_SAVE_PK_MESSAGE_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ACK_SAVE_PK_MESSAGE_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	const PD::Result Result = StaticCast<PD::Result>(Packet->Result());
	if (Result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(Packet->Result()));
		return;
	}

	FGsPKBookManager* manager = GsPKBook();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	manager->SavePKMessageConfig(*Packet);
}

void FGsNetMessageHandlerPKBook::NOTIFY_PK_BOOK_TEASE_MESSAGE(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_PK_BOOK_TEASE_MESSAGE_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_PK_BOOK_TEASE_MESSAGE_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	// 2022/09/21 PKT - 조롱 메세지를 받음.
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

	FGsPKBookHandler* PKBookHabdler = hud->GetPKBookHandler();
	if (nullptr == PKBookHabdler)
	{
		return;
	}

	/**
	 * 조롱 하기
	 */
	WorldId killerWorldId = Packet->KillerHomeWorldId();
	WorldId DieWorldId = Packet->DieUserHomeWorldId();
	FString killUserNameAndPrefix = FGsTextUtil::CombineUserAndPrefix(FText::FromString(Packet->KillerName()), killerWorldId);
	FString DieUserrNameAndPrefix = FGsTextUtil::CombineUserAndPrefix(FText::FromString(Packet->DieUserName()), DieWorldId);
	PKBookHabdler->PKBookTeaseActionMessage(FText::FromString(killUserNameAndPrefix), FText::FromString(DieUserrNameAndPrefix), FText::FromString(Packet->TeaseMessage()));
}

void FGsNetMessageHandlerPKBook::ACK_SAVE_PK_TEASE_MESSAGE(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_SAVE_PK_TEASE_MESSAGE_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ACK_SAVE_PK_TEASE_MESSAGE_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	const PD::Result Result = StaticCast<PD::Result>(Packet->Result());
	if (Result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(Packet->Result()));
		return;
	}

	GsPKBook()->SaveMockeryMessage(*Packet);
}

void FGsNetMessageHandlerPKBook::NOTIFY_PK_GUILD_I_GOT_PK(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_INFORM_I_GOT_PK_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_INFORM_I_GOT_PK_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	if (PKNotifyType::GUILD_WAR == Packet->PkType())
	{	// 2024/2/15 PKT - 그림자 전장에서 발생 된 PK는 그림자 전장 내에서만 발생한다.
		if (MapType::GUILD_WAR != GLevel()->GetCurrentMapType())
		{
			return;
		}
	}

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

	FGsPKBookHandler* PKBookHabdler = hud->GetPKBookHandler();
	if (nullptr == PKBookHabdler)
	{
		return;
	}

	const FGsGuildDataDetail* guildData = GGuild()->GetGuildDetailData(true);
	
	/**
	 * 조롱 하기
	 * 죽은 유저와 길드원에게 가는 것. 그렇기 때문에 Enemy는 Killer가 된다.
	 */	
	WorldId killerHomeWorldId = Packet->HomeWorldId();
	FString killerNameAndPrefix = FGsTextUtil::CombineUserAndPrefix(FText::FromString(Packet->EnemyName()), killerHomeWorldId);
	// 2023/9/18 PKT - 죽은 유저의 길드에게 메세지 뿌림.
	PKBookHabdler->PKGuildNotify(Packet->EnemyGuildEmblemId(), FText::FromString(killerNameAndPrefix), guildData->_emblemId, FText::FromString(Packet->UserName()));
}

void FGsNetMessageHandlerPKBook::NOTIFY_PK_GUILD_I_DID_PK(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_INFORM_I_DID_PK_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_INFORM_I_DID_PK_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	if (PKNotifyType::GUILD_WAR == Packet->PkType())
	{	// 2024/2/15 PKT - 그림자 전장에서 발생 된 PK는 그림자 전장 내에서만 발생한다.
		if (MapType::GUILD_WAR != GLevel()->GetCurrentMapType())
		{
			return;
		}
	}

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

	FGsPKBookHandler* PKBookHabdler = hud->GetPKBookHandler();
	if (nullptr == PKBookHabdler)
	{
		return;
	}

	/*
	* 우리 기사단원 중 누군가가 PK에 성공 했을 시 옴. 그렇기 때문에 Enumy는 죽은 유저(Die)가 된다.
	*/
	const FGsGuildDataDetail* guildData = GGuild()->GetGuildDetailData(true);
	if (guildData)
	{
		WorldId dieHomeWorldId = Packet->HomeWorldId();
		FString dieUserNameAndPrefix = FGsTextUtil::CombineUserAndPrefix(FText::FromString(Packet->EnemyName()), dieHomeWorldId);

		// 2023/9/18 PKT - 죽인 유저 기사단에게 메세지 뿌림
		PKBookHabdler->PKGuildNotify(guildData->_emblemId, FText::FromString(Packet->UserName()), Packet->EnemyGuildEmblemId(), FText::FromString(dieUserNameAndPrefix));
	}
}