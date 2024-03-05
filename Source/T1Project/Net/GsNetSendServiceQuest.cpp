// Fill out your copyright notice in the Description page of Project Settings.

#include "GsNetSendServiceQuest.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UTIL/GsGameObjectUtil.h"


void FGsNetSendServiceQuest::SendReqQuestList(const CreatureId inNpcId, const QuestBoardType inQuestBoardType)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_BOARD_QUEST_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inNpcId, inQuestBoardType);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceQuest::SendReqBoardQuestRefresh(const CreatureId inNpcId, const QuestBoardType inQuestBoardType)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_BOARD_QUEST_REFRESH_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inNpcId, inQuestBoardType);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceQuest::SendReqBoardQuestAccept(const CreatureId inNpcId, const QuestStoryIdPair& inQuest, const QuestBoardType inQuestBoardType)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_BOARD_QUEST_ACCEPT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize()
			, inNpcId
			, inQuestBoardType
			, inQuest);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceQuest::SendReqQuestRepeatGiveUp(const QuestStoryIdPair& inQuest)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_REPEAT_GIVE_UP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				inQuest.mQuestIndex, inQuest.mStoryId, inQuest.mQuestId);
			netBase->Send(builder.mPktWriter.GetPktSize());

			GSLOG(Log, TEXT("[QUEST] CS_REQ_QUEST_REPEAT_GIVE_UP - QuestStory : %d, QuestId : %llu, QuestIndex : %llu"), 
				inQuest.mStoryId, inQuest.mQuestId, inQuest.mQuestIndex);
		}
	}
}


void FGsNetSendServiceQuest::SendReqQuestRepeatRefresh(const QuestIndex inQuestIndex)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_REPEAT_REFRESH_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inQuestIndex);
			netBase->Send(builder.mPktWriter.GetPktSize());

			GSLOG(Log, TEXT("[QUEST] REQ_QUEST_REPEAT_REFRESH - QuestIndex : %llu"), inQuestIndex);
		}
	}
}

void FGsNetSendServiceQuest::SendReqQuestRepeatRewardBox(const QuestIndex inQuestIndex)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_REPEAT_REWARD_BOX_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inQuestIndex);
			netBase->Send(builder.mPktWriter.GetPktSize());

			GSLOG(Log, TEXT("[QUEST] REQ_QUEST_REPEAT_REWARD_BOX - QuestIndex : %llu"), inQuestIndex);
		}
	}
}

void FGsNetSendServiceQuest::SendReqQuestRepeatRewardBoxList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_REPEAT_REWARD_BOX_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());

			GSLOG(Log, TEXT("[QUEST] REQ_QUEST_REPEAT_REWARD_BOX_LIST"));
		}
	}
}

void FGsNetSendServiceQuest::SendReqQuestGuildAccept(QuestId InQuestId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			// 나의 위치, 방향정보
			FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
			int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_GUILD_ACCEPT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InQuestId, location, dir);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceQuest::SendReqQuestGuildReward(QuestId InQuestId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_GUILD_REWARD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InQuestId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceQuest::SendReqQuestGuildGiveUp(QuestId InQuestId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_GUILD_GIVE_UP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InQuestId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceQuest::SendReqQuestGuildRefresh()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_GUILD_REFRESH_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}
