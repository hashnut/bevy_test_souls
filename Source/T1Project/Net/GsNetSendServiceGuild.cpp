// Fill out your copyright notice in the Description page of Project Settings.

#include "GsNetSendServiceGuild.h"
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Client_Gateway.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#define USE_AGIT_PACKET

void FGsNetSendServiceGuild::SendReqCreateGuild(const FString& InName, GuildEmblemId InEmblemId,
	GuildJoinType InJoinType, const FString& InIntroduce)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_CREATE_GUILD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InJoinType, InEmblemId, InName, InIntroduce);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqDailyActivity(GuildDailyActivityType InType)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_GUILD_DAILY_ACTIVITY_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InType);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqChangeEmblem(GuildEmblemId InEmblemId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_CHANGE_GUILD_EMBLEM_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InEmblemId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendRItemCollectionRegister(CollectionConditionId InCollectionConditionId, const ItemDBId InItemId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_GUILD_ITEM_COLLECTION_REGISTER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InCollectionConditionId, InItemId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}


void FGsNetSendServiceGuild::SendItemCollectionBookMark(CollectionId InCollectionId, bool InActive)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			if (InActive)
			{
				FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
				PD::CS::PKT_CS_REQ_GUILD_ITEM_COLLECTION_BOOKMARK_CREATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InCollectionId);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
				PD::CS::PKT_CS_REQ_GUILD_ITEM_COLLECTION_BOOKMARK_DELETE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InCollectionId);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildStorageDistributeItem(UserDBId InReceiverDBId, ItemId InItemId, ItemDBId InItemDBId, ItemAmount InItemAmount)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_GUILD_STORE_DISTRIBUTE_ITEM_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InReceiverDBId, InItemId, InItemDBId, InItemAmount);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildStorageExtendSlot()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_GUILD_STORE_EXTEND_SLOT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildAuctionBidder(GuildAuctionId InAuctionId, CurrencyType InCurrencyType, Currency InCurrency)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_GUILD_AUCTION_BIDDER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InAuctionId, InCurrencyType, InCurrency);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildAuctionReturnCurrency()
{
	FGsUIHelper::ShowBlockUI();
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_GUILD_AUCTION_RETURN_CURRENCY_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqAgitRental()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_AGIT_RENTAL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqAgitRentalExtension()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_AGIT_RENTAL_EXTENSION_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqAgitEnter()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_AGIT_ENTER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqAgitLeave(bool bInUseRadomScroll)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_AGIT_LEAVE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), 
				bInUseRadomScroll);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqAgitDungeonRaidOpen(AgitDungeonRaidId InRaidId)
{
#ifdef USE_AGIT_PACKET
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_OPEN_AGIT_DUNGEON_RAID_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), 
				InRaidId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
#endif // USE_AGIT_PACKET
}

void FGsNetSendServiceGuild::SendReqAgitDungeonRaidClose(AgitDungeonRaidId InRaidId)
{
#ifdef USE_AGIT_PACKET
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_CLOSE_AGIT_DUNGEON_RAID_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InRaidId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
#endif // USE_AGIT_PACKET
}

void FGsNetSendServiceGuild::SendReqAgitDungeonRaidEnter(AgitDungeonRaidId InRaidId)
{
#ifdef USE_AGIT_PACKET
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ENTER_AGIT_DUNGEON_RAID_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InRaidId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
#endif // USE_AGIT_PACKET
}

void FGsNetSendServiceGuild::SendReqAgitDungeonRaidLeave()
{
#ifdef USE_AGIT_PACKET
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_LEAVE_AGIT_DUNGEON_RAID_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
#endif // USE_AGIT_PACKET
}

void FGsNetSendServiceGuild::SendReqAgitDungeonRaidBookmarkList()
{
#ifdef USE_AGIT_PACKET
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_AGIT_DUNGEON_RAID_BOOKMARK_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
#endif // USE_AGIT_PACKET
}

void FGsNetSendServiceGuild::SendReqAgitDungeonRaidBookmarkAdd(AgitDungeonRaidId InRaidId)
{
#ifdef USE_AGIT_PACKET
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_AGIT_DUNGEON_RAID_BOOKMARK_ADD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InRaidId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
#endif // USE_AGIT_PACKET
}

void FGsNetSendServiceGuild::SendReqAgitDungeonRaidBookmarkDelete(AgitDungeonRaidId InRaidId)
{
	#ifdef USE_AGIT_PACKET
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_AGIT_DUNGEON_RAID_BOOKMARK_DELETE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InRaidId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
#endif // USE_AGIT_PACKET
}

//----------------------------------------------------------------------------------------------------------------------
void FGsNetSendServiceGuild::SendReqWaitingJoinGuildUserList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_WAITING_JOIN_GUILD_USER_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqRequestedJoinGuildList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_REQUESTED_JOIN_GUILD_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqSearchGuild(const FString& InName, bool bSameServerOnly)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_SEARCH_NAME_GUILD_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InName, bSameServerOnly);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildDetail(GuildDBId InGuildDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_DETAIL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InGuildDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqOpenGuild()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_OPEN_GUILD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqJoinGuild(GuildDBId InGuildDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_JOIN_GUILD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InGuildDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqLeave()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_LEAVE_GUILD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqBanMember(UserDBId InUserDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_EXPEL_GUILD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InUserDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqJoinApprove(UserDBId InUserDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_APPROVAL_TO_JOIN_GUILD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InUserDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqDeleteGuild()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_DELETE_GUILD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqCancelJoinGuild(GuildDBId InGuildDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_CANCEL_JOIN_GUILD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InGuildDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqJoinReject(UserDBId InUserDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_REJECT_USER_JOIN_GUILD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InUserDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildMemberList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_MEMBER_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqChangeMemberGrade(UserDBId InUserDBId, GuildMemberGradeType InMemberGrade)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_CHANGE_GUILD_MEMBER_GRADE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InUserDBId, InMemberGrade);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqChangeAuthorityList(UserDBId InUserDBId, TArray<GuildMemberRightsbyGrade>& InAuthorityList)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_CHANGE_GUILD_MEMBER_RIGHTS_BY_GRADE_WRITE
				builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InUserDBId, &InAuthorityList);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqAuthorityList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_MEMBER_RIGHTS_BY_GRADE_LIST_WRITE builder(sendBuffer->GetBuffer(),
				sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqChangeNotice(const FString& InMessage)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_CHANGE_GUILD_NOTICE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InMessage);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqChangeIntroduction(const FString& InMessage)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_CHANGE_GUILD_DESC_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InMessage);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqChangeJoinType(GuildJoinType InType)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_CHANGE_GUILD_JOIN_TYPE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InType);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqChangeLimitLevel(Level InLevel)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_CHANGE_GUILD_LIMITED_USER_LEVEL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InLevel);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqSaveSNSLink(int32 InIndex, const FString& InURL)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_SNS_URL_SAVE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InIndex, InURL);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqSaveGuildAcquisitionSettings(const GuildAcquisitionSettings& InAcquisitionSettings)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_SAVE_GUILD_ACQUISITION_SETTINGS_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InAcquisitionSettings);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildBuildingList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_BUILDING_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildBuildingOpen(GuildBuildingId InBuildingId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_BUILDING_OPEN_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InBuildingId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildBuildingUpgrade(GuildBuildingId InBuildingId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_BUILDING_UPGRADE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InBuildingId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildBuildingPassivity(GuildBuildingId InBuildingId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_BUILDING_PASSIVITY_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InBuildingId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqHostileList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_HOSTILE_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqAllyList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_ALLY_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqAllyStateList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_ALLY_REQUEST_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendHostileAdd(GuildDBId InGuildDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_HOSTILE_ADD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InGuildDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendHostileDelete(GuildDBId InGuildDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_HOSTILE_DELETE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InGuildDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendAllyAddList(GuildDBId InGuildDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_ALLY_ADD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InGuildDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendBreakAlly(GuildDBId InGuildDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_BREAK_GUILD_ALLY_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InGuildDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendCancelRejectAlly(GuildDBId InGuildDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_CANCEL_GUILD_ALLY_ADD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InGuildDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendAcceptAlly(GuildDBId InGuildDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_ACCEPT_GUILD_ALLY_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InGuildDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildChatList(GuildChatDBId seq)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_CHAT_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), seq);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqAllyGuildChatList(GuildChatDBId seq)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_ALLY_CHAT_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), seq);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildQuestInfo()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_QUEST_INFO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendRItemCollectionSlotList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_ITEM_COLLECTION_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqOpenGuildStorage()
{
	if (FGsNetManager* netMgr = GNet())
	{
		FGsUIHelper::ShowBlockUI();
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_OPEN_GUILD_STORE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildStorageMemberList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_STORE_GUILD_MEMBER_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildStorageItemList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_STORE_ITEM_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildStorageAcquiredHistory()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_STORE_ACQUIRED_ITEM_HISTORY_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildStorageDistributedHistory()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_STORE_DISTRIBUTED_ITEM_HISTORY_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqEnterGuildDungeon()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ENTER_GUILD_DUNGEON_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqExitGuildDungeon()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_LEAVE_GUILD_DUNGEON_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqOpenGuildDungeonInfo()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REG_OPEN_GUILD_DUNGEON_INFO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildRedDotSave(GuildRedDotDetail& InData)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_RED_DOT_SAVE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InData);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildDivisionList(ItemDBId InLastItemDBId, bool InIsScrolled)
{
	if (false == InIsScrolled)
	{
		FGsUIHelper::ShowBlockUI();
	}

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_STORE_ITEM_LIST_V2_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InLastItemDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildAuctionList(GuildAuctionId InLastAuctionId, bool InIsScrolled)
{
	if (false == InIsScrolled)
	{
		FGsUIHelper::ShowBlockUI();
	}

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_AUCTION_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InLastAuctionId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildCalculateList(GuildAuctionId InLastAuctionId, bool InIsScrolled)
{
	if (false == InIsScrolled)
	{
		FGsUIHelper::ShowBlockUI();
	}
	
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_AUCTION_CALCULATE_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InLastAuctionId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildAuctionRegister(const TArray<ItemDBId>& InItemList, GuildAuctionType InAuctionType, GuildAuctionResultType InResultType, const TArray<UserDBId>& InUserList, CurrencyType InCurrencyType, const TArray<Currency>& InCurrencyList)
{
	Currency currency = 0;
	if (InCurrencyList.Num() > 0)
	{
		currency = InCurrencyList[0];
	}

	FGsUIHelper::ShowBlockUI();
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_STORE_ITEM_AUCTION_REGISTER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), 
				&InItemList, InAuctionType, InResultType, &InUserList, InCurrencyType, currency);

			if (InCurrencyList.Num() > 0 && InCurrencyList.Num() == InItemList.Num())
			{
				for (int32 i = 0; i < InCurrencyList.Num(); ++i)
				{
					builder.CreateItemActionRegister(InCurrencyList[i], InItemList[i]);
				}
			}

			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildAuctionDetail(ItemDBId InItemDBId)
{
	FGsUIHelper::ShowBlockUI();
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_STORE_ITEM_DETAIL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InItemDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqSanctumNexusDestruction()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_SANCTUM_NEXUS_DESTRUCTION_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqAgitDungeonRaid()
{
#ifdef USE_AGIT_PACKET
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_AGIT_DUNGEON_RAID_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
#endif // USE_AGIT_PACKET
}

void FGsNetSendServiceGuild::SendReqAgitDungeonRaidDetail(AgitDungeonRaidId InRaidId)
{
#ifdef USE_AGIT_PACKET
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_AGIT_DUNGEON_RAID_DETAIL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), 
				InRaidId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
#endif // USE_AGIT_PACKET
}

void FGsNetSendServiceGuild::SendReqGuildGroupChatList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_GROUP_CHAT_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildGroupChatMemberList(GuildGroupChatRoomId roomId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_GROUP_CHAT_MEMBER_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), roomId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildGroupChatInvitaionList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_GROUP_CHAT_INVITE_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildGroupChatInviteGuestList(GuildGroupChatRoomId roomId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_GROUP_CHAT_REQUEST_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), roomId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildGroupChatRoomCreate(FString roomName)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_GROUP_CHAT_CREATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), roomName);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildGroupChatInvite(GuildGroupChatRoomId roomId, FString guildName, PlanetWorldId guildGroupWorldId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_GROUP_CHAT_INVITE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), guildGroupWorldId, guildName, roomId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildGroupChatKickOut(GuildGroupChatRoomId roomId, GuildDBId guildId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_GROUP_CHAT_KICK_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), guildId, roomId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildGroupChatWithDraw(GuildGroupChatRoomId roomId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_GROUP_CHAT_WITHDRAWAL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), roomId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildGroupChatInvitationAccept(GuildGroupChatRoomId roomId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_GROUP_CHAT_ACCEPT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), roomId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildGroupChatInvitationReject(GuildGroupChatRoomId roomId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_GROUP_CHAT_INVITE_REJCET_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), roomId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildGroupChatCancelInvitation(GuildGroupChatRoomId roomId, GuildDBId guildId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_GROUP_CHAT_INVITE_CANCEL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), roomId, guildId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildGroupChatEntrustWithRoomMaster(GuildGroupChatRoomId roomId, GuildDBId guildId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_GROUP_CHAT_DELEGATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), roomId, guildId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceGuild::SendReqGuildGroupChatChangeAuthorizedGrade(GuildGroupChatRoomId roomId, GuildMemberGradeType grade)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILD_GROUP_CHAT_MODIFY_CHAT_GRADE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), roomId, static_cast<uint8>(grade));
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}
