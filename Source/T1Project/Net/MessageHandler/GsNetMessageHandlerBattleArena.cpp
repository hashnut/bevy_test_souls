// Fill out your copyright notice in the Description page of Project Settings.

#include "GsNetMessageHandlerBattleArena.h"
#include "../Guild/GsGuildData.h"
#include "../BattleArenaRank/GsBattleArenaRankData.h"
#include "../Message/GsMessageNet.h"
#include "../Management/ScopeGlobal/GsNetManager.h"
#include "../Management/ScopeGlobal/GsGuildManager.h"
#include "../Management/ScopeGlobal/GsGameFlowManager.h"
#include "../Management/ScopeGlobal/GsGameDataManager.h"
#include "../Management/ScopeGame/GsBattleArenaSeasonRankManager.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaData.h"
#include "../GameFlow/GsGameFlowGame.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaGuildWarHandler.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"
#include "../UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "../GSNet.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "T1Project.h"


void FGsNetMessageHandlerBattleArena::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalWorld& worldProtocal = inManager->GetProtocalWorld();
	InitializeMessageDelegateList(&worldProtocal);

	// Packet
	AddMessageDelegate(worldProtocal.AddRaw(
		PD::SC::SCPacketId::SC_GUILDWAR_INGAME_INFO, this, &FGsNetMessageHandlerBattleArena::Notify_GuildWarScoreInfo
	));

	AddMessageDelegate(worldProtocal.AddRaw(
		PD::SC::SCPacketId::SC_ACK_GUILDWAR_INGAME_DETAIL_INFO, this, &FGsNetMessageHandlerBattleArena::Ack_GuildWarScoreDetailInfo
	));

	AddMessageDelegate(worldProtocal.AddRaw(
		PD::SC::SCPacketId::SC_GUILDWAR_END, this, &FGsNetMessageHandlerBattleArena::Notify_GuildWarEndPlayResult
	));

	AddMessageDelegate(worldProtocal.AddRaw(
		PD::SC::SCPacketId::SC_GUILDWAR_ENTER, this, &FGsNetMessageHandlerBattleArena::Nofity_BattleArenaEnter
	));

	AddMessageDelegate(worldProtocal.AddRaw(
		PD::SC::SCPacketId::SC_GUILDWAR_LEAVE, this, &FGsNetMessageHandlerBattleArena::Nofity_BattleArenaLeave
	));

	AddMessageDelegate(worldProtocal.AddRaw(
		PD::SC::SCPacketId::SC_GUILDWAR_PLAYER_POSITION_LIST, this, &FGsNetMessageHandlerBattleArena::Notify_TeamPlayerPositionInfo
	));

	/**
	 * GateWay -> Client
	 */
	MProtocalGateway& gatewayProtocol = inManager->GetProtocalGateway();	

	_gateWayPackets.Add(gatewayProtocol.AddRaw(
			PD::GC::GCPacketId::GC_GUILDWAR_START_ALARM, this, &FGsNetMessageHandlerBattleArena::Nofity_StartAlarm
		));

	_gateWayPackets.Add(gatewayProtocol.AddRaw(
		PD::GC::GCPacketId::GC_GUILDWAR_INFO_ALL_MAP, this, &FGsNetMessageHandlerBattleArena::Notify_GuildWarInfoList
	));

	_gateWayPackets.Add(gatewayProtocol.AddRaw(
		PD::GC::GCPacketId::GC_GUILDWAR_INFO, this, &FGsNetMessageHandlerBattleArena::Notify_GuildWarInfo
	));

	_gateWayPackets.Add(gatewayProtocol.AddRaw(
		PD::GC::GCPacketId::GC_ACK_GUILDWAR_ENTRY_REGISTER_UPDATE, this, &FGsNetMessageHandlerBattleArena::Ack_RegisterUpdate
	));

	_gateWayPackets.Add(gatewayProtocol.AddRaw(
		PD::GC::GCPacketId::GC_GUILDWAR_MATCH_FAILED, this, &FGsNetMessageHandlerBattleArena::Ack_MatchFailed
	));

	_gateWayPackets.Add(gatewayProtocol.AddRaw(
		PD::GC::GCPacketId::GC_GUILDWAR_ENTER_FAILED, this, &FGsNetMessageHandlerBattleArena::Ack_EnterFailed
	));


	/**
	 * Battle Arena Rank
	 */
	_gateWayPackets.Add(gatewayProtocol.AddRaw(
		PD::GC::GCPacketId::GC_ACK_GUILDWAR_MY_RANK_INFO, this, &FGsNetMessageHandlerBattleArena::AckBattleArenaLocalGuildRankInfo
	));

	_gateWayPackets.Add(gatewayProtocol.AddRaw(
		PD::GC::GCPacketId::GC_ACK_GUILDWAR_RANK_INFO, this, &FGsNetMessageHandlerBattleArena::AckBattleArenaGuildRankInfo
	));
}

void FGsNetMessageHandlerBattleArena::FinalizeMessage()
{
	MProtocalGateway& gatewayProtocol = GNet()->GetProtocalGateway();
	for (auto& e : _gateWayPackets)
	{
		gatewayProtocol.Remove(e);
	}
	_gateWayPackets.Empty();

	FinalizeMessageDelegateList();
}

FGsBattleArenaGuildWarHandler* FGsNetMessageHandlerBattleArena::GetGuildWarHandler() const
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return nullptr;
	}
	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	return (hud) ? hud->GetBattleArenaGuildWarHandler() : nullptr;
}

void FGsNetMessageHandlerBattleArena::Nofity_StartAlarm(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_GUILDWAR_START_ALARM_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_GUILDWAR_START_ALARM_READ*>(inPacket.Buffer);

	if (FGsBattleArenaGuildWarHandler* handler = GetGuildWarHandler())
	{
		handler->ArenaStartAlarm(pkt->MapId());
	}
}

void FGsNetMessageHandlerBattleArena::Nofity_BattleArenaEnter(const FGsNet::Packet& inPacket)
{
	// 2023/12/29 PKT - 그림자 전장 입장 시 받음. 현재는 사용하지 않아 주석 처리
	/*PD::SC::PKT_SC_GUILDWAR_ENTER_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_GUILDWAR_ENTER_READ*>(inPacket.Buffer);

	if (FGsBattleArenaGuildWarHandler* handler = GetGuildWarHandler())
	{	
		handler->BattleArenaEnter(pkt->MapId());
	}*/
}

void FGsNetMessageHandlerBattleArena::Nofity_BattleArenaLeave(const FGsNet::Packet& inPacket)
{
	// 2023/12/29 PKT - 그림자 전장 퇴장 시 받음. 현재는 사용하지 않아 주석 처리
	/*PD::SC::PKT_SC_GUILDWAR_LEAVE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_GUILDWAR_LEAVE_READ*>(inPacket.Buffer);

	if (FGsBattleArenaGuildWarHandler* handler = GetGuildWarHandler())
	{
		handler->BattleArenaLeave(pkt->MapId());
	}*/
}

void FGsNetMessageHandlerBattleArena::Notify_GuildWarInfo(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_GUILDWAR_INFO_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_GUILDWAR_INFO_READ*>(inPacket.Buffer);

	if (FGsBattleArenaGuildWarHandler* handler = GetGuildWarHandler())
	{	// 2023/11/21 PKT - Battle Arena  상태 갱신

		TArray<FGsBattleArenaEntryInfo> memberInfo;
		for (auto iter = pkt->GetFirstEntrymemberListIterator(); iter != pkt->GetLastEntrymemberListIterator(); ++iter)
		{
			FGsBattleArenaEntryInfo entry;
			entry._name = FText::GetEmpty();		// 2023/12/4 PKT - 멤버 이름은 아직 필요 없어서 별도로 받지 않음.
			entry._userdbId = iter->Member();

			memberInfo.Emplace(entry);
		}
		handler->UpdateState(pkt->State(), pkt->MapId(), memberInfo, pkt->MaxUserCnt(), pkt->StateEndTime());
	}
}

void FGsNetMessageHandlerBattleArena::Notify_GuildWarInfoList(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_GUILDWAR_INFO_ALL_MAP_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_GUILDWAR_INFO_ALL_MAP_READ*>(InPacket.Buffer);

	if (FGsBattleArenaGuildWarHandler* handler = GetGuildWarHandler())
	{
		if (0 < pkt->GetGuildWarInfoListCount())
		{
			auto firstItem = pkt->GetFirstGuildWarInfoListIterator();

			TArray<FGsBattleArenaEntryInfo> memberInfo;
			for (auto iter = firstItem->GetFirstEntrymemberListIterator(); iter != firstItem->GetLastEntrymemberListIterator(); ++iter)
			{
				FGsBattleArenaEntryInfo entry;
				entry._name = FText::GetEmpty();		// 2023/12/4 PKT - 멤버 이름은 아직 필요 없어서 별도로 받지 않음.
				entry._userdbId = iter->Member();

				memberInfo.Emplace(entry);
			}
			handler->UpdateState(firstItem->State(), firstItem->MapId(), memberInfo, pkt->MaxUserCnt(), firstItem->StateEndTime());
		}
		else
		{	// 2023/11/30 PKT - 진행하고 있는 GuildWar Arena 가 없음.
			handler->ContentsShotDown();
		}
	}
}

FText CombineErrorText(const PD::Result InKey)
{
	FString displayKey = FString::Format(TEXT("{0}"), { PD::ResultEnumToDisplayKey(InKey) });

	FText message;
	FText::FindText(TEXT("NetText"), displayKey, message);

#if UE_BUILD_SHIPPING
	FString msg = FString::Format(TEXT("{0}"), { *(message.ToString()) });
#else	// UE_BUILD_SHIPPING	
	FString msg = (message.IsEmpty()) ?
		FString::Format(TEXT("Code({0})\n{1}"), { static_cast<int32>(InKey), PD::ResultEnumToString(InKey) }) :
		FString::Format(TEXT("Code({0})\n{1}"), { static_cast<int32>(InKey), *(message.ToString()) });
#endif	// UE_BUILD_SHIPPING

	return FText::FromString(msg);
}

void FGsNetMessageHandlerBattleArena::Ack_RegisterUpdate(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILDWAR_ENTRY_REGISTER_UPDATE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILDWAR_ENTRY_REGISTER_UPDATE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		GSLOG(Error, TEXT("nullptr == pkt"));
		return;
	}

	if (PD::Result::RESULT_SUCCESS != static_cast<PD::Result>(pkt->Result()))
	{
		FText message = CombineErrorText(static_cast<PD::Result>(pkt->Result()));

		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
	}
	else
	{
		if (FGsBattleArenaGuildWarHandler* handler = GetGuildWarHandler())
		{	// 2023/11/21 PKT - Battle Arena  상태 갱신
			handler->UpdateState(pkt->IsEntry());
		}
	}
}

void FGsNetMessageHandlerBattleArena::Ack_MatchFailed(const FGsNet::Packet& InPacket)
{	
	PD::GC::PKT_GC_GUILDWAR_MATCH_FAILED_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_GUILDWAR_MATCH_FAILED_READ*>(InPacket.Buffer);

	FText message = CombineErrorText(static_cast<PD::Result>(pkt->Reason()));
	
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);

	if (PD::Result::GUILD_WAR_MATCH_FAIL_MEMBERCOUNT_MINIMUM_LIMIT == static_cast<PD::Result>(pkt->Reason())
		|| PD::Result::GUILD_WAR_MATCH_NOT_FOUND_ENEMY_GUILD == static_cast<PD::Result>(pkt->Reason()))
	{	// 2024/2/5 PKT - 채팅으로도 뿌려야 할 메세지
		FGsUIHelper::AddChatMessageSystem(message);
	}
	
	if (FGsBattleArenaGuildWarHandler* handler = GetGuildWarHandler())
	{	// 2023/11/21 PKT - Battle Arena  상태 갱신
		handler->ContentsShotDown();
	}
}

void FGsNetMessageHandlerBattleArena::Ack_EnterFailed(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_GUILDWAR_ENTER_FAILED_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_GUILDWAR_ENTER_FAILED_READ*>(InPacket.Buffer);

	FText message = CombineErrorText(static_cast<PD::Result>(pkt->Reason()));

	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);

	if (FGsBattleArenaGuildWarHandler* handler = GetGuildWarHandler())
	{	// 2023/11/21 PKT - Battle Arena  상태 갱신
		handler->ContentsShotDown();
	}
}

void FGsNetMessageHandlerBattleArena::Notify_GuildWarScoreInfo(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_GUILDWAR_INGAME_INFO_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_GUILDWAR_INGAME_INFO_READ*>(InPacket.Buffer);

	if (FGsBattleArenaGuildWarHandler* handler = GetGuildWarHandler())
	{	// 2023/11/23 PKT - 팀 점수 정보
		FGsBattleArenaGuildWarTeamInfo redTeamInfo;
		redTeamInfo._guildDBId = pkt->RedTeamGuildDbId();
		redTeamInfo._guildEmblemId = pkt->RedTeamGuildEmblemId();
		redTeamInfo._guildName = FText::FromString(pkt->RedTeamName());
		redTeamInfo._score = pkt->RedTeamPoint();


		FGsBattleArenaGuildWarTeamInfo blueTeamInfo;
		blueTeamInfo._guildDBId = pkt->BlueTeamGuildDbId();
		blueTeamInfo._guildEmblemId = pkt->BlueTeamGuildEmblemId();
		blueTeamInfo._guildName = FText::FromString(pkt->BlueTeamName());
		blueTeamInfo._score = pkt->BlueTeamPoint();

		handler->UpdateState(pkt->State(), redTeamInfo, blueTeamInfo, pkt->StateEndTime());
	}
}

void TeamMemberRankSort(TArray<FGsBattleArenaGuildWarMemberInfo>& InMemberSet)
{
	InMemberSet.Sort([](const FGsBattleArenaGuildWarMemberInfo& lhr, const FGsBattleArenaGuildWarMemberInfo& rhr)
		{
			return lhr._point > rhr._point;
		}
	);
}

void FGsNetMessageHandlerBattleArena::Ack_GuildWarScoreDetailInfo(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_GUILDWAR_INGAME_DETAIL_INFO_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_GUILDWAR_INGAME_DETAIL_INFO_READ*>(InPacket.Buffer);
	
	if (FGsBattleArenaGuildWarHandler* handler = GetGuildWarHandler())
	{
		int32 totalScore = 0;

		FGsBattleArenaGuildWarTeamInfo redTeamInfo;
		redTeamInfo._guildDBId = pkt->RedTeamGuildDbId();
		redTeamInfo._guildEmblemId = pkt->RedTeamGuildEmblemId();
		redTeamInfo._guildName = FText::FromString(pkt->RedTeamName());
		redTeamInfo._worldId = pkt->RedTeamWorldId();
		redTeamInfo._score = pkt->RedTeamPoint();

		for (auto iter = pkt->GetFirstRedTeamMemberPointInfoListIterator(); iter != pkt->GetLastRedTeamMemberPointInfoListIterator(); ++iter)
		{
			FGsBattleArenaGuildWarMemberInfo member;
			member._userDBId = iter->UserDbId();
			member._name = FText::FromString(iter->Name());
			member._killCount = iter->Kill();
			member._assistCount = iter->Assist();
			member._deathCount = iter->Death();
			member._point = iter->Point();

			totalScore += iter->Point();

			redTeamInfo._members.Emplace(member);
		}
		redTeamInfo._score = totalScore;
		TeamMemberRankSort(redTeamInfo._members);

		totalScore = 0;
		FGsBattleArenaGuildWarTeamInfo blueTeamInfo;
		blueTeamInfo._guildDBId = pkt->BlueTeamGuildDbId();
		blueTeamInfo._guildEmblemId = pkt->BlueTeamGuildEmblemId();
		blueTeamInfo._guildName = FText::FromString(pkt->BlueTeamName());
		blueTeamInfo._worldId = pkt->BlueTeamWorldId();
		blueTeamInfo._score = pkt->BlueTeamPoint();

		for (auto iter = pkt->GetFirstBlueTeamMemberPointInfoListIterator(); iter != pkt->GetLastBlueTeamMemberPointInfoListIterator(); ++iter)
		{
			FGsBattleArenaGuildWarMemberInfo member;
			member._userDBId = iter->UserDbId();
			member._name = FText::FromString(iter->Name());
			member._killCount = iter->Kill();
			member._assistCount = iter->Assist();
			member._deathCount = iter->Death();
			member._point = iter->Point();
			totalScore += iter->Point();

			blueTeamInfo._members.Emplace(member);
		}
		blueTeamInfo._score = totalScore;
		TeamMemberRankSort(blueTeamInfo._members);
				
		//handler->DetailScore(pkt->State(), pkt->StateEndTime(), redTeamInfo, blueTeamInfo);
		handler->DetailScore(redTeamInfo, blueTeamInfo);
	}
}


void FGsNetMessageHandlerBattleArena::Notify_GuildWarEndPlayResult(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_GUILDWAR_END_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_GUILDWAR_END_READ*>(InPacket.Buffer);

	if (FGsBattleArenaGuildWarHandler* handler = GetGuildWarHandler())
	{
		const UserDBId userDBId = GGameData()->GetUserData()->mUserDBId;

		int32 myPoint = 0;
		int32 totalScore = 0;

		FGsBattleArenaGuildWarTeamInfo redTeamInfo;
		redTeamInfo._guildDBId = pkt->RedTeamGuildDbId();
		redTeamInfo._guildEmblemId = pkt->RedTeamGuildEmblemId();
		redTeamInfo._guildName = FText::FromString(pkt->RedTeamName());
		redTeamInfo._worldId = pkt->RedTeamWorldId();

		for (auto iter = pkt->GetFirstRedTeamMemberPointInfoListIterator(); iter != pkt->GetLastRedTeamMemberPointInfoListIterator(); ++iter)
		{
			FGsBattleArenaGuildWarMemberInfo member;
			member._userDBId = iter->UserDbId();
			member._name = FText::FromString(iter->Name());
			member._killCount = iter->Kill();
			member._assistCount = iter->Assist();
			member._deathCount = iter->Death();
			member._point = iter->Point();

			totalScore += iter->Point();

			redTeamInfo._members.Emplace(member);

			if (userDBId == member._userDBId)
			{
				myPoint = member._point;
			}
		}
		redTeamInfo._score = totalScore;
		TeamMemberRankSort(redTeamInfo._members);

		totalScore = 0;
		FGsBattleArenaGuildWarTeamInfo blueTeamInfo;
		blueTeamInfo._guildDBId = pkt->BlueTeamGuildDbId();
		blueTeamInfo._guildEmblemId = pkt->BlueTeamGuildEmblemId();
		blueTeamInfo._guildName = FText::FromString(pkt->BlueTeamName());
		blueTeamInfo._worldId = pkt->BlueTeamWorldId();

		for (auto iter = pkt->GetFirstBlueTeamMemberPointInfoListIterator(); iter != pkt->GetLastBlueTeamMemberPointInfoListIterator(); ++iter)
		{
			FGsBattleArenaGuildWarMemberInfo member;
			member._userDBId = iter->UserDbId();
			member._name = FText::FromString(iter->Name());
			member._killCount = iter->Kill();
			member._assistCount = iter->Assist();
			member._deathCount = iter->Death();
			member._point = iter->Point();
			totalScore += iter->Point();

			blueTeamInfo._members.Emplace(member);

			if (userDBId == member._userDBId)
			{
				myPoint = member._point;
			}
		}
		blueTeamInfo._score = totalScore;
		TeamMemberRankSort(blueTeamInfo._members);
		
		handler->PlayEndResult(pkt->GameEndReason(), pkt->WinnerGuildDbId(), myPoint, redTeamInfo, blueTeamInfo);
	}
}

void FGsNetMessageHandlerBattleArena::Notify_TeamPlayerPositionInfo(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_GUILDWAR_PLAYER_POSITION_LIST_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_GUILDWAR_PLAYER_POSITION_LIST_READ*>(InPacket.Buffer);

	if (FGsBattleArenaGuildWarHandler* handler = GetGuildWarHandler())
	{
		// 2023/12/27 PKT - Red Member
		FGsBattleArenaMemberPositionInfos redMember;
		for (auto iter = pkt->GetFirstRedTeamMemberPositionInfoListIterator(); iter != pkt->GetLastRedTeamMemberPositionInfoListIterator(); ++iter)
		{
			redMember._memberSet.Emplace(TPair<UserDBId, FVector>(iter->UserDbId(), iter->UserPos()));
		}

		// 2023/12/27 PKT - Blue Member
		FGsBattleArenaMemberPositionInfos blueMember;
		for (auto iter = pkt->GetFirstBlueTeamMemberPositionInfoListIterator(); iter != pkt->GetLastBlueTeamMemberPositionInfoListIterator(); ++iter)
		{
			blueMember._memberSet.Emplace(TPair<UserDBId, FVector>(iter->UserDbId(), iter->UserPos()));
		}

		handler->MemberPositionInfo(redMember, blueMember);
	}
}

void FGsNetMessageHandlerBattleArena::AckBattleArenaLocalGuildRankInfo(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILDWAR_MY_RANK_INFO_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILDWAR_MY_RANK_INFO_READ*>(InPacket.Buffer);

	if (FGsBattleArenaSeasonRankManager* manager = GsBattleArenaRank())
	{
		FGsBattleArenaRankLocalData localData;
		localData._playCount = pkt->PlayCount();
		localData._nextUpdateTime = pkt->NextUpdateTime();

		FGsBattleArenaRankListItem rankData;
		if (GGuild() && GGuild()->GetMyGuildData())
		{			
			rankData._guildEmblemId = GGuild()->GetMyGuildData()->_emblemId;
			rankData._guildName = FText::FromString(GGuild()->GetMyGuildData()->_guildName);
			rankData._worldId = GGuild()->GetMyGuildData()->GetWorldId();
		}
		else
		{
			rankData._guildEmblemId = INVALID_GUILD_DB_ID;
			rankData._guildName = FText::GetEmpty();
			rankData._worldId = INVALID_WORLD_ID;
		}
		rankData._isValidate = ValidRankDataState::Validate;
		rankData._index = pkt->Index();
		rankData._rank = pkt->NowRank();
		rankData._prevRank = pkt->PrevRank();
		rankData._score = pkt->Score();		
		rankData._winCount = pkt->WinCount();
		rankData._loseCount = pkt->LoseCount();
		rankData._tieCount = pkt->TieCount();		
		rankData._userName = FText::GetEmpty();
		rankData._tierDataId = INVALID_GUILD_WAR_TIER_DATA_ID;
		if (0 < pkt->NowRank())
		{	// 2024/2/20 PKT - 경기에 한번도 참가하지 않았다면 
			rankData._tierDataId = manager->GetTierDataIdByScore(RankSearchType::KNIGHTS, pkt->NowRank(), pkt->Score());
		}
		// 2024/2/13 PKT - My Rank Data
		localData._rankData = rankData;
		
		TArray<TPair<GuildWarTierDataId, int32>> tierSizeSet;
		for (auto iter = pkt->GetFirstTierMaxRankListIterator(); iter != pkt->GetLastTierMaxRankListIterator(); ++iter)
		{
			tierSizeSet.Emplace(TPair<GuildWarTierDataId, int32>(iter->TierId(), iter->MaxRank()));
		}

		manager->UpdateLocalData(RankSearchType::KNIGHTS, localData, tierSizeSet);
	}
}

void FGsNetMessageHandlerBattleArena::AckBattleArenaGuildRankInfo(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILDWAR_RANK_INFO_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILDWAR_RANK_INFO_READ*>(InPacket.Buffer);

	if (FGsBattleArenaSeasonRankManager* manager = GsBattleArenaRank())
	{
		TArray<FGsBattleArenaRankListItem> tierRankDataSet;
		for (auto iter = pkt->GetFirstTierMaxRankListIterator(); iter != pkt->GetLastTierMaxRankListIterator(); ++iter)
		{
			FGsBattleArenaRankListItem data;
			data._index = iter->Index();
			data._rank = iter->NowRank();
			data._prevRank = iter->PrevRank();
			data._score = iter->Score();
			data._guildName = FText::FromString(iter->GuildName());
			data._guildEmblemId = iter->GuildEmblemId();
			data._winCount = iter->WinCount();
			data._loseCount = iter->LoseCount();
			data._tieCount = iter->TieCount();
			data._tierDataId = manager->GetTierDataIdByScore(RankSearchType::KNIGHTS, iter->NowRank(), iter->Score());

			PlanetWorldId planetWorldId(iter->PlanetWorldId());
			data._worldId = planetWorldId.st.worldId;

			tierRankDataSet.Emplace(data);
		}
		
		manager->UpdateRankData(RankSearchType::KNIGHTS, pkt->NextUpdateTime(), pkt->TierId(), tierRankDataSet);
	}
}