#include "GsNetSendServiceBoss.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Message/MessageParam/GsEventMessageParam.h"
#include "Net/GsNetGame.h"
#include "Shared/Shared/SharedPackets/PD_Client_Gateway.h"
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "T1Project.h"
#include "UTIL/GsGameObjectUtil.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#define USE_GUILD_KNIGHT_QUEST


void FGsNetSendServiceBoss::SendBossContribution(FieldBossId inBossId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_RESULT_DEFEAT_FILEDBOSS_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inBossId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceBoss::SendReqCompetitionFieldBossLastDead(FieldBossId inFieldBossId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
	{
		return;
	}
				
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	if (nullptr == sendBuffer)
	{
		return;
	}

	PD::CG::PKT_CG_REQ_COMPETITION_FIELD_BOSS_LAST_DEAD_TIME_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inFieldBossId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceBoss::SendReqUpdateFieldBossBookmark(const TArray<FieldBossId>& inFieldBossIdArray)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
	{
		return;
	}

	const TArray<FieldBossId> fieldBossArray = inFieldBossIdArray;
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_UPDATE_FIELD_BOSS_BOOKMARK_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), &fieldBossArray);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceBoss::SendReqWorldBossMapWarp(MapId inMapId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_WORLD_BOSS_WARP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inMapId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceBoss::SendReqWorldBossMapLeave()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_WORLD_BOSS_MAP_LEAVE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceBoss::SendReqWorldBossInfo(WorldBossId inWorldBossId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_WORLD_BOSS_INFO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inWorldBossId);
	netBase->Send(builder.mPktWriter.GetPktSize());

#if WORLD_BOSS_DEBUG
	GSLOG(Log, TEXT("SendReqWorldBossInfo - boss id : %lld"), (int64)inWorldBossId);
#endif
}
