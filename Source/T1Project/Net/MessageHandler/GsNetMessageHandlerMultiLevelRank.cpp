#include "GsNetMessageHandlerMultiLevelRank.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "MultiLevelRank/GsMultiLevelRankData.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGame/GsMultiLevelRankManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "DataSchema/Quest/Rank/GsSchemaQuestMultiLevelRankStory.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "UI/UIContent/Helper/GsUIHelper.h"


#include "Item/GsItemManager.h"
#include "Currency/GsCurrencyHelper.h"
#include "T1Project.h"


using namespace PD::SC;

void FGsNetMessageHandlerMultiLevelRank::InitializeMessage(FGsNetManager* InManager)
{
	// 패킷 바인딩
	MProtocalWorld& WorldProtocol = InManager->GetProtocalWorld();

	InitializeMessageDelegateList(&WorldProtocol);

	// notify..
	AddMessageDelegate(WorldProtocol.AddRaw(SCPacketId::SC_MULTI_LEVEL_RANK_INFO_LIST
											, this
											, &FGsNetMessageHandlerMultiLevelRank::Notify_MULTI_LEVEL_RANK_INFO_LIST));
	
	// promtion...
	AddMessageDelegate(WorldProtocol.AddRaw(SCPacketId::SC_ACK_MULTI_LEVEL_RANK_PROMOTION
											, this
											, &FGsNetMessageHandlerMultiLevelRank::Ack_MULTI_LEVEL_RANK_PROMOTION));
	
	// change
	AddMessageDelegate(WorldProtocol.AddRaw(SCPacketId::SC_ACK_MULTI_LEVEL_RANK_CHANGE_RANK
											, this
											, &FGsNetMessageHandlerMultiLevelRank::Ack_MULTI_LEVEL_RANK_CHANGE_RANK));
}

void FGsNetMessageHandlerMultiLevelRank::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerMultiLevelRank::Notify_MULTI_LEVEL_RANK_INFO_LIST(const FGsNet::Packet& InPacket)
{
	PKT_SC_MULTI_LEVEL_RANK_INFO_LIST_READ* packet = reinterpret_cast<PKT_SC_MULTI_LEVEL_RANK_INFO_LIST_READ*>(InPacket.Buffer);
	if (nullptr == packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	if (false == packet->Validate())
	{
		GSLOG(Error, TEXT("false == Packet->Validate()"));
		return;
	}

	/**
	 * Add.. Rank Data
	 */
	using RankIterator = PD::SC::PKT_SC_MULTI_LEVEL_RANK_INFO_LIST_READ::MultiLevelRankInfoListIterator;

	RankIterator rankBegine = packet->GetFirstMultiLevelRankInfoListIterator();
	RankIterator rankEnd = packet->GetLastMultiLevelRankInfoListIterator();

	TArray<MultiLevelRankSlotInfo> rankSlotList;
	for (RankIterator it = rankBegine; it != rankEnd; ++it)
	{
		MultiLevelRankSlotInfo item = it->ConstRefSlotInfo();
		rankSlotList.Emplace(item);
	}

	// set game data..
	if (FGsMultiLevelRankManager* multiLevelManager = GSMultiLevelRank())
	{
		multiLevelManager->Notify_MultiLevelRankList(rankSlotList, packet->MaxAccumulateExp());
	}
}

void FGsNetMessageHandlerMultiLevelRank::Ack_MULTI_LEVEL_RANK_PROMOTION(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	FGsMultiLevelRankManager* manager = GSMultiLevelRank();
	if (nullptr == manager)
		return;

	manager->SetCheatSended(false);

	PKT_SC_ACK_MULTI_LEVEL_RANK_PROMOTION_READ* packet = reinterpret_cast<PKT_SC_ACK_MULTI_LEVEL_RANK_PROMOTION_READ*>(InPacket.Buffer);
	if (nullptr == packet)
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("nullptr == Packet"));
#endif
		return;
	}

	if (PACKET_RESULT_SUCCESS != packet->Result())
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("PACKET_RESULT_SUCCESS != packet->Result()"));
#endif
		return;
	}

	// rank id
	MultiLevelRankId rankId = packet->RankId();

	// ack..
	manager->Ack_MultiLevelRankPromotion(rankId);
}

void FGsNetMessageHandlerMultiLevelRank::Ack_MULTI_LEVEL_RANK_CHANGE_RANK(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	FGsMultiLevelRankManager* manager = GSMultiLevelRank();
	if (nullptr == manager)
		return;

	PKT_SC_ACK_MULTI_LEVEL_RANK_CHANGE_RANK_READ* packet = reinterpret_cast<PKT_SC_ACK_MULTI_LEVEL_RANK_CHANGE_RANK_READ*>(InPacket.Buffer);
	if (nullptr == packet)
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("nullptr == Packet"));
#endif
		manager->SetCheatSended(false);
		return;
	}

	if (PACKET_RESULT_SUCCESS != packet->Result())
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("PACKET_RESULT_SUCCESS != packet->Result()"));
#endif
		manager->SetCheatSended(false);
		return;
	}

	// slot info
	MultiLevelRankId prevRank = packet->PreviousRankId();
	MultiLevelRankId nextRank = packet->ChangedRankId();

#if WITH_EDITOR
	GSLOG(Log, TEXT("FGsNetMessageHandlerMultiLevelRank::Ack_MULTI_LEVEL_RANK_CHANGE_RANK : previous [%d], next [%d]"), prevRank, nextRank);
#endif

	// ack...
	manager->Ack_MultiLevelRankChangeRank(prevRank, nextRank);
	manager->SetCheatSended(false);
}
