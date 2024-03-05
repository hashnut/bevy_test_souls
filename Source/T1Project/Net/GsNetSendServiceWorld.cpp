#include "GsNetSendServiceWorld.h"

#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Net/GsNetGame.h"

#include "Message/MessageParam/GsEventMessageParam.h"

#include "UTIL/GsGameObjectUtil.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"

#include "GameObject/Fence/GsFenceHandler.h"
#include "GameObject/Quest/GsQuestHandler.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Stat/GsPlayerStatHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Stat/GsPlayerStatDistributionController.h"

#include "T1Project.h"


void FGsNetSendServiceWorld::SendLocalPlayerMove(
	const FVector& inCurrPos, int16 inCurrDir,
	const FVector& inDestPos, float inFallingSpeed, uchar inMotionType, uint64 inClinentTick, uint64 inSeqNum)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_USER_MOVE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				inCurrPos, inCurrDir, inDestPos, inFallingSpeed, inMotionType, inClinentTick, inSeqNum);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendLocalPlayerPosValidate(const FVector& inCurrPos, int16 inCurrDir, const FVector& inDestPos, uchar inMotionType, uint64 inClinentTick, uint64 inSeqNum)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_ACK_POS_VALIDATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				inCurrPos, inCurrDir, inDestPos, inMotionType, inClinentTick, inSeqNum);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

bool FGsNetSendServiceWorld::SendStartSkill(
	int32 inSkillId, const FVector& inCurrPos, int16 inCurrDir,
	const FVector& inAimStartPos, const FVector& inAimTargetPos, int64 inTargetId, bool inAuto
)
{
#ifdef TEST_LOG
	GSLOG(Error, TEXT(">>>> [HIT TEST]: SendStartSkill - inAimTargetPos:%s"), *inAimTargetPos.ToString());
#endif

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_START_SKILL_WRITE builder(sendBuffer->GetBuffer(),
				sendBuffer->GetSize(), inSkillId, inCurrPos,
				inCurrDir, inAimStartPos, inAimTargetPos, inTargetId, inAuto);
			netBase->Send(builder.mPktWriter.GetPktSize());
			return true;
		}
	}
	return false;
}

void FGsNetSendServiceWorld::SendCancelSkill()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_CANCEL_SKILL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendSkillSlotClear(CreatureWeaponType InWeapon, SkillCategorySet InCategorySet)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_SKILL_SLOT_CLEAR_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InWeapon, InCategorySet);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendRequestSkillSlotSet(CreatureWeaponType InWeapon, 
	SkillId InRootSkillId, SkillId InActiveSkillId, SkillSlotId InSkillSlotId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_SKILL_SLOT_SET_WRITE builder(sendBuffer->GetBuffer(),
				sendBuffer->GetSize(), InWeapon, InRootSkillId, InActiveSkillId, InSkillSlotId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendRequestSkillSlotAuto(CreatureWeaponType InWeapon, 
	SkillId InRootSkillId, SkillId InActiveSkillId, bool bInAutoUse)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_SKILL_SLOT_AUTO_WRITE builder(sendBuffer->GetBuffer(),
				sendBuffer->GetSize(), InWeapon, InRootSkillId, InActiveSkillId, bInAutoUse);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendRequestSkillEnchant(SkillId InRootSkillId, SkillId InActiveSkillId,
	TArray<ItemDBIdAmountPair>& InCostList)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_SKILL_ENCHANT_WRITE
				builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InRootSkillId, InActiveSkillId, &InCostList);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendRequestCostumeSkillLearn(CostumeId InCostumeId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_COSTUME_SKILL_LEARN_WRITE
				builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InCostumeId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendRequestCostumeSkillSlotSet(CostumeId InCostumeId, SkillId InRootSkillId,
	SkillId InActiveSkillId, SkillSlotId InSkillSlotId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_COSTUME_SKILL_SLOT_SET_WRITE
				builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), 
					InCostumeId, InRootSkillId, InActiveSkillId, InSkillSlotId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendRequestUseSkillCycle(SkillId InRootSkillId, SkillId InActiveSkillId,
	bool bUseInterval, bool bUseInPVP, int32 InIntervalSecond)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_USE_SKILL_INTERVAL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InRootSkillId, InActiveSkillId, bUseInterval, bUseInPVP, InIntervalSecond);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendResurrect(ResurrectType inType)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_RESURRECT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inType);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendQuestMainAccept(QuestId inQuestId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			// 나의 위치, 방향정보
			FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
			int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_ACCEPT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inQuestId,
				location, dir);
			netBase->Send(builder.mPktWriter.GetPktSize());

//#if WITH_EDITOR  
			GSLOG(Log, TEXT("[Quest] Quest_Send_Main_Accept- ID: %llu"), inQuestId);
//#endif
			FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler();
			if (nullptr != questHandler)
				questHandler->AddRequestList(QuestKey(inQuestId));
		}
	}
}

void FGsNetSendServiceWorld::SendQuestSubAccept(StoryId inStoryId, QuestId inQuestId, SpawnId inSpawnId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			// 나의 위치, 방향정보
			FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
			int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_SUB_ACCEPT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), 
				inStoryId, inQuestId, inSpawnId, location, dir);
			netBase->Send(builder.mPktWriter.GetPktSize());

//#if WITH_EDITOR 
			GSLOG(Log, TEXT("[Quest] Quest_Sub_Send_Accept- StoryId: %d, QuestId: %llu, SpawnId: %d"), inStoryId, inQuestId, inSpawnId);
//#endif
			FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler();
			if (nullptr != questHandler)
				questHandler->AddRequestList(QuestKey(inQuestId));
		}
	}
}

void FGsNetSendServiceWorld::SendQuestRepeatAccept(StoryId inStoryId, QuestId inQuestId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			// 나의 위치, 방향정보
			FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
			int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_REPEAT_ACCEPT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				inStoryId, inQuestId, location, dir);
			netBase->Send(builder.mPktWriter.GetPktSize());

//#if WITH_EDITOR 
			GSLOG(Log, TEXT("[Quest] Quest_Repeat_Send_Accept- StoryId: %d, QuestId: %llu"), inStoryId, inQuestId);
//#endif
			FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler();
			if (nullptr != questHandler)
				questHandler->AddRequestList(QuestKey(inQuestId));
		}
	}	
}

void FGsNetSendServiceWorld::SendQuestMultiLevelRankAccept(StoryId inStoryId, const TArray<QuestId>& InQuestIdList)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			// 나의 위치, 방향정보
			FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
			int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();


			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_MULTI_LEVEL_RANK_ACCEPT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inStoryId );
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();

#if WITH_EDITOR
			GSLOG(Log, TEXT("[Quest] Quest_Rank_Send_Accept- StoryId: %d"), inStoryId);
#endif
			FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler();
			if (nullptr != questHandler)
			{
				for (QuestId questId : InQuestIdList)
					questHandler->AddRequestList(QuestKey(questId));
			}				
		}
	}
}

void FGsNetSendServiceWorld::SendQuestGuideAccept(QuestId inQuestId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			// 나의 위치, 방향정보
			FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
			int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_GUIDE_ACCEPT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				inQuestId, location, dir);
			netBase->Send(builder.mPktWriter.GetPktSize());

//#if WITH_EDITOR 
			GSLOG(Log, TEXT("[Quest] Quest_Guide_Send_Accept- QuestId: %llu"), inQuestId);
//#endif
			FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler();
			if (nullptr != questHandler)
				questHandler->AddRequestList(QuestKey(inQuestId));
		}
	}
}

void FGsNetSendServiceWorld::SendQuestMainReward(QuestId inQuestId, ItemBagIndex inMainIndex, ItemBagIndex inSubIndex)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_REWARD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inQuestId, 
				inMainIndex, inSubIndex);
			netBase->Send(builder.mPktWriter.GetPktSize());

//#if WITH_EDITOR 
			GSLOG(Log, TEXT("[Quest] Quest_Main_Send_Reward- ID: %llu"), inQuestId);
//#endif
			FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler();
			if (nullptr != questHandler)
				questHandler->AddRequestList(QuestKey(inQuestId));
		}
	}
}

void FGsNetSendServiceWorld::SendQuestSubReward(StoryId inStoryId, QuestId inQuestId, ItemBagIndex inMainIndex, ItemBagIndex inSubIndex)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_SUB_REWARD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inStoryId, inQuestId,
				inMainIndex, inSubIndex);
			netBase->Send(builder.mPktWriter.GetPktSize());

//#if WITH_EDITOR 
			GSLOG(Log, TEXT("[Quest] Quest_Sub_Send_Reward- ID: %llu"), inQuestId);
//#endif
			FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler();
			if (nullptr != questHandler)
				questHandler->AddRequestList(QuestKey(inQuestId));
		}
	}
}


void FGsNetSendServiceWorld::SendQuestRepeatReward(StoryId inStoryId, QuestId inQuestId, QuestIndex inQuestIndex, ItemBagIndex inMainIndex, ItemBagIndex inSubIndex)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_REPEAT_REWARD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inQuestIndex, inStoryId, inQuestId,
				inMainIndex, inSubIndex);
			netBase->Send(builder.mPktWriter.GetPktSize());
			netMgr->GetCoolTime().AddCoolTime(static_cast<uint16>(PD::CS::CSPacketId::CS_REQ_QUEST_REPEAT_REWARD));

//#if WITH_EDITOR 
			GSLOG(Log, TEXT("[Quest] Quest_Repeat_Send_Reward- ID: %llu, QuestIndex : %llu"), inQuestId, inQuestIndex);
//#endif
			FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler();
			if (nullptr != questHandler)
				questHandler->AddRequestList(QuestKey(inQuestId, inQuestIndex));
		}
	}
}

void FGsNetSendServiceWorld::SendQuestMultiLevelRankComplete(StoryId InStoryId, const TArray<QuestId> InQuestList)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();

			PD::CS::PKT_CS_REQ_QUEST_MULTI_LEVEL_RANK_FINISH_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InStoryId);
			netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR
			GSLOG(Log, TEXT("[Quest] Quest_Rank_Send_Completed- ID: %d"), InStoryId);
#endif
			FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler();
			if (nullptr != questHandler)
			{
				for (QuestId id : InQuestList)
				{
					if (false == questHandler->IsInRequestList(QuestKey(id)))
					{
						questHandler->AddRequestList(QuestKey(id));
					}
				}
			}
		}
	}
}

void FGsNetSendServiceWorld::SendQuestGuideReward(QuestId InQuestId, ItemBagIndex inMainIndex, ItemBagIndex inSubIndex)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_GUIDE_REWARD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InQuestId, inMainIndex, inSubIndex);
			netBase->Send(builder.mPktWriter.GetPktSize());

//#if WITH_EDITOR 
			GSLOG(Log, TEXT("[Quest] Quest_Guide_Send_Completed- QuestId: %llu"), InQuestId);
//#endif
			if (FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler())
			{
				questHandler->AddRequestList(QuestKey(InQuestId));
			}
		}
	}
}

void FGsNetSendServiceWorld::SendQuestMainUpdate(QuestId inQuestId, QuestIndex inQuestIndex, QuestObjectiveId inQuestObjectiveId, SpawnId inSpawnId, int64 inGameId, MapId inMapId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler();
			if (nullptr != questHandler)
			{
				if (false == questHandler->IsPossibleQuestWarp(QuestKey(inQuestId, inQuestIndex)))
				{
					return;
				}
				questHandler->AddRequestList(QuestKey(inQuestId, inQuestIndex));
			}

			// 나의 위치, 방향정보
			FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
			int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_UPDATE_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), QuestPreocessData(inQuestId, inQuestObjectiveId, inQuestIndex),
				inGameId, inSpawnId, inMapId, location, dir);
			netBase->Send(builder.mPktWriter.GetPktSize());

//#if WITH_EDITOR 
			GSLOG(Log, TEXT("[Quest] Quest_Main_Send_Update- ID: %llu"), inQuestId);
//#endif
		}
	}
}

void FGsNetSendServiceWorld::SendQuestSubUpdate(StoryId inStoryId, QuestIndex inQuestIndex, QuestId inQuestId, QuestObjectiveId inQuestObjectiveId, SpawnId inSpawnId, int64 inGameId, MapId inMapId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			// 나의 위치, 방향정보
			FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
			int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_SUB_UPDATE_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), QuestPreocessData(inQuestId, inQuestObjectiveId, inQuestIndex),
				inGameId, inSpawnId, inMapId, location, dir);
			netBase->Send(builder.mPktWriter.GetPktSize());

//#if WITH_EDITOR 
			GSLOG(Log, TEXT("[Quest] Quest_Sub_Send_Update- ID: %llu"), inQuestId);
//#endif
			FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler();
			if (nullptr != questHandler)
				questHandler->AddRequestList(QuestKey(inQuestId));
		}
	}
}

void FGsNetSendServiceWorld::SendQuestRepeatUpdate(StoryId inStoryId, QuestIndex inQuestIndex, QuestId inQuestId, QuestObjectiveId inQuestObjectiveId, SpawnId inSpawnId, int64 inGameId, MapId inMapId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			// 나의 위치, 방향정보
			FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
			int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_REPEAT_UPDATE_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), QuestPreocessData(inQuestId, inQuestObjectiveId, inQuestIndex),
				inGameId, inSpawnId, inMapId, location, dir);
			netBase->Send(builder.mPktWriter.GetPktSize());

//#if WITH_EDITOR 
			GSLOG(Log, TEXT("[Quest] Quest_Repeat_Send_Update- ID: %llu"), inQuestId);
//#endif
			FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler();
			if (nullptr != questHandler)
				questHandler->AddRequestList(QuestKey(inQuestId, inQuestIndex));
		}
	}
}

void FGsNetSendServiceWorld::SendQuestWarp(QuestId inQuestId, QuestIndex inQuestIndex, QuestObjectiveId inQuestObjectiveId, bool inIsAccpeted)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("QuestWarp")));

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_WARP_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), inIsAccpeted, QuestPreocessData(inQuestId, inQuestObjectiveId, inQuestIndex));
			netBase->Send(builder.mPktWriter.GetPktSize());

//#if WITH_EDITOR 
			GSLOG(Log, TEXT("[Quest] Quest_Warp_Send- questId: %llu, objectiveId : %llu"), inQuestId, inQuestObjectiveId);
//#endif
		}
	}
}

void FGsNetSendServiceWorld::SendQuestSubGiveUp(StoryId inStoryId, QuestId inQuestId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{	
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_SUB_GIVE_UP_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), inStoryId, inQuestId);
			netBase->Send(builder.mPktWriter.GetPktSize());

//#if WITH_EDITOR 
			GSLOG(Log, TEXT("[Quest] Quest_Sub_GiveUp_Send- storyId: %d, questId : %llu"), inStoryId, inQuestId);
//#endif
		}
	}
}

void FGsNetSendServiceWorld::SendQuestRepeatGiveUp(StoryId inStoryId, QuestId inQuestId, QuestIndex inQuestIndex)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUEST_REPEAT_GIVE_UP_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), inQuestIndex, inStoryId, inQuestId);
			netBase->Send(builder.mPktWriter.GetPktSize());
			netMgr->GetCoolTime().AddCoolTime(static_cast<uint16>(PD::CS::CSPacketId::CS_REQ_QUEST_REPEAT_GIVE_UP));

//#if WITH_EDITOR 
			GSLOG(Log, TEXT("[Quest] Quest_Repeat_GiveUp_Send- storyId: %d, questId : %llu, questIndex : %llu"), inStoryId, inQuestId, inQuestIndex);
//#endif
		}
	}
}

void FGsNetSendServiceWorld::SendWarp(uint32 inWarpId)
{
	if(FGsNetManager * netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid() && false == netMgr->GetCoolTime().IsCoolTime(static_cast<uint16>(PD::CS::CSPacketId::CS_REQ_WARP)))
		{						
			FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("Warp")));

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_WARP_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), inWarpId);
			netBase->Send(builder.mPktWriter.GetPktSize());

			netMgr->GetCoolTime().AddCoolTime(static_cast<uint16>(PD::CS::CSPacketId::CS_REQ_WARP));

			// 포탈을 통한 워프 시도 시, 단축키를 바로 막아준다
			FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_WAIT_SERVER_RESPONSE, true);	
			GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
		}
	}
}

void FGsNetSendServiceWorld::SendAckWarpWithDespawnReady()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	
	FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("WarpWithDespawnReady")));
	FGsUIHelper::SetBlockUISetting(FText::GetEmpty(), true);

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_ACK_WARP_WITH_DESPAWN_READY_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqSpawnMeAction()
{
	// 패킷을 두 번 보내는 것을 방지하기 위한 처리
	if (UGsGameObjectUtil::GetSpawnMeCompleteFinFlag())
	{
		return;
	}

	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	// 스폰 시, 패킷 전송을 막아놨을 경우 해제해 주어야 한다.
	netMgr->ClearPacketSendBlock();

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

#if WITH_EDITOR 
	GSLOG(Log, TEXT("Send CS_REQ_SPAWN_ME_ACTION Packet"));
#endif

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_SPAWN_ME_ACTION_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());

	// 패킷을 두 번 보내는 것을 방지하기 위한 처리
	UGsGameObjectUtil::SetSpawnMeCompleteFinFlag(true);
}

/**
 * 요청 시 FGsNetMessageHandlerEvent::AckEventAction() 응답함.
 */
void FGsNetSendServiceWorld::SendEventAction(const FGsReqEventActionParam& Param)
{
	FGsNetManager* NetManager = GNet();
	if (nullptr == NetManager)
	{
		return;
	}

	TSharedPtr<FGsNetBase> NetBase = NetManager->GetActive();
	if (!NetBase.IsValid())
	{
		return;
	}

	GSLOG(Log, TEXT("Try PKT_CS_REQ_EVENT_ACTION_WRITE, _questId:%llu, _eventActionState:%d, _eventActionGroupId:%d, _lastEventActionIndex:%d"), Param._questId, Param._eventActionState, Param._eventActionGroupId, Param._lastEventActionIndex);
	FGsNetBuffer* SendBuffer = NetBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_EVENT_ACTION_WRITE Packet(SendBuffer->GetBuffer(), SendBuffer->GetSize(), Param._questId, Param._eventActionState, Param._eventActionGroupId, Param._lastEventActionIndex);
	NetBase->Send(Packet.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendEnterTerritory(uint32 inTerritoryId, const FVector& inCurrLoc)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();

		if (netBase.IsValid())
		{			
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ENTER_TERRITORY_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), inTerritoryId, inCurrLoc);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendLeftTerritory(uint32 inTerritoryId, const FVector& inCurrLoc)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();

		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_LEAVE_TERRITORY_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), inTerritoryId, inCurrLoc);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendRequestChannelList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_CHANNEL_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendRequestSelectChannel(ChannelId InChannelId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_SELECT_CHANNEL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InChannelId);
			netBase->Send(builder.mPktWriter.GetPktSize());

		}
	}
}

void FGsNetSendServiceWorld::SendRequestLeaveInstanceSingleMap()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{	
			if (GLevel()->IsInstanceSingleMap())
			{
				// 인던에서 나갈때 퀘스트가 롤백한다. 
				// 퀘스트를 완료해서 나가는거면 커런트 퀘스트가 증가 하겠지만
				// 인던에서 그냥 나가면 커런트 증가가 없으므로
				// 퀘스트를 초기화 해주는게 필요!!!!
				GSQuest()->ClearQuestMain();
			}

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_LEAVE_INSTANCE_SINGLE_MAP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());

		}
	}
}

void FGsNetSendServiceWorld::SendDropObjectLoot(int64 inGameId, bool inAutoLooting)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			// 나의 위치, 방향정보
			FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
			int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_DROP_OBJECT_LOOT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				inGameId, location, dir, inAutoLooting);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}
void FGsNetSendServiceWorld::SendBookmarkCreate(const FString& In_name)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("BookmarkCreate")));

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_BOOKMARK_CREATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), In_name);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendBookmarkUpdate(uint64 In_id, const FString& In_name)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("BookmarkUpdate")));

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_BOOKMARK_UPDATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), In_id, In_name);
	netBase->Send(builder.mPktWriter.GetPktSize());
}
void FGsNetSendServiceWorld::SendBookmarkDelete(uint64 In_id)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("BookmarkDelete")));

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_BOOKMARK_DELETE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), In_id);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendBookmarkTeleport(uint64 In_bookmarkId, uint64 In_itemDBId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("BookmarkTeleport")));

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_BOOKMARK_WARP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), In_bookmarkId, In_itemDBId);
	netBase->Send(builder.mPktWriter.GetPktSize());

}
void FGsNetSendServiceWorld::SendBookmarkShare(BookmarkShareType In_type, BookmarkId In_id)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("BookmarkShare")));

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_BOOKMARK_SHARE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), In_type, In_id);
	netBase->Send(builder.mPktWriter.GetPktSize());
}
void FGsNetSendServiceWorld::SendSharedBookmarkWarp(MapId In_mapId, const FVector& In_pos, ItemDBId In_itemDBId, int In_channelId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("SharedBookmarkWarp")));

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_SHARED_BOOKMARK_WARP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
		In_channelId,
		In_mapId, In_pos, In_itemDBId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendSharedBookmarkRegister(const FString& In_name, MapId In_mapId, const FVector& In_pos)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("SharedBookmarkRegister")));

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_BOOKMARK_REGISTER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
		In_name,
		In_mapId, In_pos);
	netBase->Send(builder.mPktWriter.GetPktSize());
}
// 클라 스폰이면, inNpcId(npcData - ID) : inSpawnId(SpawnData - SpawnID)
void FGsNetSendServiceWorld::SendTryInteract(CreatureId inNpcId, SpawnId inSpawnId, int64 inGameId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	// 나의 위치, 방향정보
	FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
	int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_TRY_INTERACT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), 
		inNpcId, inGameId, inSpawnId, location, dir);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendInteractQuestPropStart(int64 In_gameId, TArray<QuestPreocessData>& In_QuestObjectList)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	// 나의 위치, 방향정보
	FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
	int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_INTERACT_QUEST_PROP_START_WRITE builder(sendBuffer->GetBuffer(),
		sendBuffer->GetSize(), In_gameId, &In_QuestObjectList, location, dir);
	netBase->Send(builder.mPktWriter.GetPktSize());

#if PROP_INTERACTION_DEBUG
	GSLOG(Log, TEXT("Prop debug - prop interaction - send quest prop start - prop game id : %lld"), In_gameId);
#endif
}

void FGsNetSendServiceWorld::SendInteractQuestPropEnd(int64 In_gameId, TArray<QuestPreocessData>& In_QuestObjectList)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	// 나의 위치, 방향정보
	FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
	int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_INTERACT_QUEST_PROP_END_WRITE builder(sendBuffer->GetBuffer(),
		sendBuffer->GetSize(), In_gameId, &In_QuestObjectList, location, dir);
	netBase->Send(builder.mPktWriter.GetPktSize());

#if PROP_INTERACTION_DEBUG
	GSLOG(Log, TEXT("Prop debug - prop interaction - send quest prop end - prop game id : %lld"), In_gameId);
#endif
}

void FGsNetSendServiceWorld::SendInteractPropCancel(class UGsGameObjectPropObject* inProp)
{
	if (false == SendInteractPropCancel(inProp->GetGameId()))
	{
		return;
	}

	inProp->SetCanceling(true);
}

bool FGsNetSendServiceWorld::SendInteractPropCancel(int64 inGameId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return false;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return false;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_INTERACT_PROP_CANCEL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inGameId);
	netBase->Send(builder.mPktWriter.GetPktSize());

#if PROP_INTERACTION_DEBUG
	GSLOG(Log, TEXT("Prop debug - prop interaction - send cancel - prop game id : %lld"), inGameId);
#endif

	return true;
}

void FGsNetSendServiceWorld::SendInteractPropStart(int64 In_gameId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = GNet()->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	// 나의 위치, 방향정보
	FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
	int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_INTERACT_PROP_START_WRITE builder(sendBuffer->GetBuffer(),
		sendBuffer->GetSize(), In_gameId, location, dir);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendInteractPropEnd(int64 In_gameId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	// 나의 위치, 방향정보
	FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
	int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_INTERACT_PROP_END_WRITE builder(sendBuffer->GetBuffer(),
		sendBuffer->GetSize(), In_gameId, location, dir);
	netBase->Send(builder.mPktWriter.GetPktSize());
}
void FGsNetSendServiceWorld::SendEventPlayerEffect(CreatureId In_npcId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	// 나의 위치, 방향정보
	FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
	int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_EVENT_PRAYER_EFFECTIVE_WRITE builder(sendBuffer->GetBuffer(),
		sendBuffer->GetSize(), In_npcId, location, dir);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendInteractInvasion(int64 gameId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_CLICK_PROP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), gameId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendTryNpcEventShopOpen(int64 In_gamdId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	// 나의 위치, 방향정보
	FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
	int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_NPC_EVENT_SHOP_OPEN_WRITE builder(sendBuffer->GetBuffer(),
		sendBuffer->GetSize(), In_gamdId, location, dir);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();

	GSLOG(Warning, TEXT("SendNpcEventShopOpen()"));
}


void FGsNetSendServiceWorld::SendRequestBonusStatIncrease()
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

	FGsPlayerStatHandler* handler = FGsPlayerStatHandler::GetInstance();
	if (nullptr == handler)
	{
		return;
	}

	FGsPlayerStatDistributionController* controller = handler->GetStatDistributionController();
	if (nullptr == controller)
	{
		return;
	}

	FGsUIHelper::ShowBlockUI();

	TArray<StatInfo> statList;
	controller->TryGetUsedBonusStatList(statList);

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_BONUS_STAT_INCREASE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), &statList);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqBonusStatExtendCount()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_BONUS_STAT_EXTEND_COUNT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendPortalWarp(int In_mapId, uint32 In_spotId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("PortalWarp")));

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_PORTAL_WARP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), In_mapId, In_spotId);
	netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR	
	GSLOG(Log, TEXT("[Warp] SendPortalWarp MapId: %d, SpotId: %d"), In_mapId, In_spotId);
#endif
}

void FGsNetSendServiceWorld::SendMonsterSpawnWarp(int In_mapId, CreatureId In_npcId, uint32 In_spawnGroupId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("MonsterSpawnWarp")));

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_WARP_TO_SPAWN_GROUP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), In_mapId, In_spawnGroupId, In_npcId);
	netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR	
	GSLOG(Log, TEXT("[Warp] SendMonsterSpawnWarp MapId: %d, SpawnGroupId: %d, NpcId : %d"), In_mapId, In_spawnGroupId, In_npcId);
#endif
}

void FGsNetSendServiceWorld::SendReqFairyEquip(FairyId id) 
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_FAIRY_EQUIP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), id);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqFairyEnchant(FairyId id)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_FAIRY_ENCHANT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), id);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqFairyCompose(const TArray<FairyIdAmountPair>& fairyPairList, const TArray<uint8>& fairyCountList)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_FAIRY_COMPOSE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), &fairyPairList, &fairyCountList);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqFairySummonChange(FairyConfirmId fairyConfirmId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_FAIRY_SUMMON_CHANGE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), fairyConfirmId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqFairySummonConfirm(FairyConfirmId fairyConfirmId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_FAIRY_SUMMON_CONFIRM_WRITE  builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), fairyConfirmId);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqFairyCeiling(FairyGrade grade, uint8 phase)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_FAIRY_CEILING_WRITE  builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), phase, grade);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendMountVehicle()
{
	const FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_MOUNT_VEHICLE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendUnmountVehicle()
{
	const FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_UNMOUNT_VEHICLE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendItemCollectionRegister(const CollectionConditionId InCollectionConditionId, const ItemDBId InItemDBId)
{
	const FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

#if WITH_EDITOR
	GSLOG(Log, TEXT("PKT_CS_REQ_ITEM_COLLECTION_REGISTER_WRITE, InCollectionConditionId:%d, InItemDBId:%lld"), InCollectionConditionId, InItemDBId);
#endif

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_ITEM_COLLECTION_REGISTER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InCollectionConditionId, InItemDBId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendItemCollectionBookmark(const CollectionId InCollectionID, bool bInChangedFlag)
{
	const FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

#if WITH_EDITOR
	GSLOG(Log, TEXT("PKT_CS_REQ_ITEM_COLLECTION_BOOKMARK_WRITE, InCollectionID :%d"), InCollectionID);
#endif

	if (true == bInChangedFlag)
	{	// 2021/07/27 PKT - Create BookMark
		FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();		
		PD::CS::PKT_CS_REQ_ITEM_COLLECTION_BOOKMARK_CREATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InCollectionID);
		netBase->Send(builder.mPktWriter.GetPktSize());
	}
	else
	{	// 2021/07/27 PKT - Delete BookMark
		FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
		PD::CS::PKT_CS_REQ_ITEM_COLLECTION_BOOKMARK_DELETE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InCollectionID);
		netBase->Send(builder.mPktWriter.GetPktSize());
	}	
}


void FGsNetSendServiceWorld::SendReqOption(const ClientOptionData& inOptionData)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_CLIENT_OPTION_SAVE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inOptionData);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqGameOption(const GameOption& inGameOptionData)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_GAME_OPTION_SAVE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inGameOptionData);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

//void FGsNetSendServiceWorld::SendReqChat(ChatType chatType, const FString chatMessage)
//{
//	FGsNetManager* netMgr = GNet();
//	if (nullptr == netMgr)
//	{
//		return;
//	}
//	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
//	if (netBase.IsValid() == false)
//	{
//		return;
//	}
//
//#if WITH_EDITOR
//	GSLOG(Log, TEXT("FGsNetSendServiceWorld::SendReqChat type [%d]: %s"), (int)chatType, *chatMessage);
//#endif
//
//	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
//	PD::CS::PKT_CS_REQ_CHAT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), chatType, chatMessage);
//	netBase->Send(builder.mPktWriter.GetPktSize());
//}

//void FGsNetSendServiceWorld::SendReqWhisper(const FString toChatNick, const FString chatMessage)
//{
//	FGsNetManager* netMgr = GNet();
//	if (nullptr == netMgr)
//	{
//		return;
//	}
//	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
//	if (netBase.IsValid() == false)
//	{
//		return;
//	}
//
//#if WITH_EDITOR
//	GSLOG(Log, TEXT("FGsNetSendServiceWorld::SendReqWhisper to %s: %s"), *toChatNick, *chatMessage);
//#endif
//
//	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
//	PD::CS::PKT_CS_REQ_WHISPER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), toChatNick, chatMessage);
//	netBase->Send(builder.mPktWriter.GetPktSize());
//}

void FGsNetSendServiceWorld::SendReqDeathPenaltyRestore(TArray<DeathPenaltyRestore>& inDeathPenaltyId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();

	PD::CS::PKT_CS_REQ_DEATH_PENALTY_RESTORE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), &inDeathPenaltyId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

#ifdef R3_ARCAS_NOT_USE
void FGsNetSendServiceWorld::SendReqUpdateArcasBuff(bool useFairyBless, bool useArcasBless)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();

	PD::CS::PKT_CS_UPDATE_ARCAS_BUFF_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), useFairyBless, useArcasBless);
	netBase->Send(builder.mPktWriter.GetPktSize());
}
#endif

void FGsNetSendServiceWorld::SendReqCostumeEquip(CostumeId id)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();

	PD::CS::PKT_CS_REQ_COSTUME_EQUIP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), id);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqCostumeCompose(const TArray<CostumeIdAmountPair>& pairList, const TArray<uint8>& countList)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_COSTUME_COMPOSE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), &pairList, &countList);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqCostumeSummonChange(CostumeConfirmId confirmId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_COSTUME_SUMMON_CHANGE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), confirmId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqCostumeSummonConfirm(CostumeConfirmId confirmId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_COSTUME_SUMMON_CONFIRM_WRITE  builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), confirmId);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqCostumeCeiling(CostumeGrade grade, uint8 phase)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_COSTUME_CEILING_WRITE  builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), phase, grade);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqCostumeExchange(ItemDBId itemDBId, CostumeId recipeCostumeId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_CHANGE_COSTUME_WRITE  builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), itemDBId, recipeCostumeId, 0);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqMailList()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_MAIL_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[mail] FGsNetSendServiceWorld::SendReqMailList"));
#endif

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqNewSystmeMailRecv()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_NOTIFY_NEW_SYSTEM_MAIL_RECV_READY_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[mail] FGsNetSendServiceWorld::SendReqNewSystmeMailRecv"));
#endif

	//FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqMailRecv(MailBoxType inBoxType, TArray<MailDBId>& inMailDBIdList)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_MAIL_RECV_WRITE	builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inBoxType, &inMailDBIdList);
	netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR
	for (MailDBId id : inMailDBIdList)
	{
		GSLOG(Log, TEXT("[mail] FGsNetSendServiceWorld::SendReqMailRecv MailBoxType : %d, MailDBId: %llu"), (int)inBoxType, id);
	}	
#endif

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqMailDelete(MailBoxType inBoxType, TArray<MailDBId>& inMailDBIdList)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_MAIL_RECV_WRITE
		builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inBoxType, &inMailDBIdList);
	netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR
	for (MailDBId id : inMailDBIdList)
	{
		GSLOG(Log, TEXT("[mail] FGsNetSendServiceWorld::SendReqMailDelete MailBoxType : %d, MailDBId: %llu"), (int)inBoxType, id);
	}
#endif

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqMailRead(MailBoxType inBoxType, MailDBId inMailDBId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
		return;

#if WITH_EDITOR	
	GSLOG(Log, TEXT("[mail] FGsNetSendServiceWorld::SendReqMailDelete MailBoxType : %d, MailDBId: %llu"), (int)inBoxType);	
#endif

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_MAIL_READ_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inBoxType, inMailDBId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

// 정규 던전 그룹 목록 요청
void FGsNetSendServiceWorld::SendReqDungeonGroupList()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
		return;

#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] SendReqDungeonGroupList"));
#endif

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_DUNGEON_GROUP_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

// 2022/06/09 PKT - 던전에서 나가겠다.
void FGsNetSendServiceWorld::Send_ExitDungeon()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
		return;

	FGsNetBuffer* SendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_DUNGEON_LEAVE_WRITE Packet(SendBuffer->GetBuffer(), SendBuffer->GetSize());
	netBase->Send(Packet.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::Send_ExitBattleArena()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
		return;

	FGsNetBuffer* SendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_GUILDWAR_LEAVE_WRITE Packet(SendBuffer->GetBuffer(), SendBuffer->GetSize());
	netBase->Send(Packet.mPktWriter.GetPktSize());
}

// 천마석 ui 키고 끄기
void FGsNetSendServiceWorld::SendReqSpiritShotOnOff(bool In_isOn)
{
	FGsNetManager* NetManager = GNet();
	if (nullptr == NetManager)
	{
		return;
	}

	TSharedPtr<FGsNetBase> NetBase = NetManager->GetActive();
	if (!NetBase.IsValid())
	{
		return;
	}

	GSLOG(Log
		, TEXT("PKT_CS_REQ_SPIRIT_SHOT_ON_OFF_WRITE, In_isOn: %d")
		, In_isOn);

	FGsNetBuffer* SendBuffer = NetBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_SPIRIT_SHOT_ON_OFF_WRITE Packet(SendBuffer->GetBuffer(), SendBuffer->GetSize()
		, In_isOn);
	NetBase->Send(Packet.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendNpcSpecialFunction(int64 inGameId)
{
	FGsNetManager* NetManager = GNet();
	if (nullptr == NetManager)
	{
		return;
	}

	TSharedPtr<FGsNetBase> NetBase = NetManager->GetActive();
	if (false == NetBase.IsValid())
	{
		return;
	}

	GSLOG(Log, TEXT("PKT_CS_REQ_NPC_SPECIAL_FUNCTION_WRITE - game id : %lld"), inGameId);

	FGsNetBuffer* SendBuffer = NetBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_NPC_SPECIAL_FUNCTION_WRITE Packet(SendBuffer->GetBuffer(), SendBuffer->GetSize(), inGameId);
	NetBase->Send(Packet.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqSpaceCrackWeekReward()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_SPACE_CRACK_WEEK_REWARD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqMultiLevelRankPromotion(MultiLevelRankId InRankId)
{
	// 다단계 랭크 승급 요청
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
		return;
	
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_MULTI_LEVEL_RANK_PROMOTION_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InRankId);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqMultiLevelRankChangeRank(MultiLevelRankId InRankId)
{
	// 다단계 랭크 변경 요청
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_MULTI_LEVEL_RANK_CHANGE_RANK_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InRankId);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqOfflinePlayReadyState()		
{	// 2021/06/29 PKT - 오프라인 플레이 레디 상태 요쳥
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_OFF_LINE_READY_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqOfflinePlayStartPlay()
{
	// 2021/06/29 PKT - 오프라인 플레이 시작 상태 요청
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_OFF_LINE_START_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqOfflinePlayCancel()
{
	// 2021/06/29 PKT - 오프라인 플레이 취소
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_OFF_LINE_CANCEL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), 0);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendOfflinePlayResultConfirm()
{
	// 2023/4/7 PKT - 오프라인 플레이 결과 확인 버튼
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_OFF_LINE_CONFIRM_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());	
}

void FGsNetSendServiceWorld::SendReqCommunityList()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_COMMUNITY_EVENT_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqAttendanceCheck(AttendanceId inId, AttendanceCheckIdx inIndex)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_ATTENDANCE_CHECK_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inId, inIndex);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqPurchaseAttendance(AttendanceId inId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_PURCHASE_ATTENDANCE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inId);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendUserDeathPenaltyCheck()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_USER_DEATH_PENALTY_CHECK_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendCinematicStartByEventAction(uint32 inSequenceResId, const FVector& inCurrPos, int16 inCurrDir)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	GSLOG(Log, TEXT("[Cinematic] SendCinematicStartByEventAction s - SequenceResId: %d"), inSequenceResId);

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_CLIENT_CINEMATIC_START_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inSequenceResId, inCurrPos, inCurrDir);
	netBase->Send(builder.mPktWriter.GetPktSize());

	GSLOG(Log, TEXT("[Cinematic] SendCinematicStartByEventAction e - SequenceResId: %d"), inSequenceResId);
}

void FGsNetSendServiceWorld::SendCinematicEndByEventAction(uint32 inSequenceResId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_CLIENT_CINEMATIC_END_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inSequenceResId);
	netBase->Send(builder.mPktWriter.GetPktSize());

	GSLOG(Log, TEXT("[Cinematic] SendCinematicEndByEventAction- SequenceResId: %d"), inSequenceResId);
}

//void FGsNetSendServiceWorld::SendReqAchievementThemeRewards(AchievementThemeId id)
//{
//	FGsNetManager* netMgr = GNet();
//	if (nullptr == netMgr)
//		return;
//
//	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
//	if (false == netBase.IsValid())
//		return;
//
//	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
//	PD::CS::PKT_CS_REQ_ACHIEVEMENT_THEME_REWARD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), id);
//	netBase->Send(builder.mPktWriter.GetPktSize());
//}

void FGsNetSendServiceWorld::SendReqAchievementRewards(AchievementId id)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_ACHIEVEMENT_REWARD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), id);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqAchievementEnableRewardList()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_ACHIEVEMENT_REWARD_ALL_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqAchievementReceiveRewardAll()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_ACHIEVEMENT_REWARD_ALL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

//void FGsNetSendServiceWorld::SendReqChatUserGuildInfo(const FString& nickName)
//{
//	if (FGsNetManager* netMgr = GNet())
//	{
//		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
//		if (netBase.IsValid())
//		{
//			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
//			PD::CS::PKT_CS_REQ_USER_NAME_TO_GUILD_INFO_WRITE builder(
//				sendBuffer->GetBuffer(), sendBuffer->GetSize(), nickName);
//			netBase->Send(builder.mPktWriter.GetPktSize());
//		}
//	}
//}

// 파티장에게로 이동
void FGsNetSendServiceWorld::SendWarpToPartyLeader()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("WarpToPartyLeader")));

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_WARP_TO_PARTY_LEADER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::Send_MonsterKnowledgeLevelUp(uint32 InId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_MONSTER_KNOWLEDGE_LEVEL_UP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::Send_MonsterKnowledgeLevelAll()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_MONSTER_KNOWLEDGE_LEVEL_ACHIEVEMENT_ALL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
	
	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::Send_MonsterKnowledgeExpIncrease()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_MONSTER_KNOWLEDGE_LEVEL_ACHIEVEMENT_ALL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::Send_MonsterKnowledgeExpGainItemUse(ItemDBId InItemDBId, ItemAmount InAmount, CodexMapGroupId InId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_MONSTER_KNOWLEDGE_EXP_GAIN_ITEM_USE_WRITE builder(sendBuffer->GetBuffer()
		, sendBuffer->GetSize()
		, InItemDBId
		, InAmount, InId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}


void FGsNetSendServiceWorld::Send_MonsterKnowledgeSlate(MonsterKnowledgeSlateId InId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_MONSTER_KNOWLEDGE_SLATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}


void FGsNetSendServiceWorld::Send_MonsterKnowledgeUnLock(ItemDBId InItemDBId)
{

}

void FGsNetSendServiceWorld::Send_MonsterKnowledgeExpGainItemUseAll( ItemDBId InItemDBId, ItemAmount InAmount, const TArray<MonsterKnowledgeBookGrade>& InGrades, int32 InPriority )
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_MONSTER_KNOWLEDGE_EXP_GAIN_ITEM_USE_ALL_WRITE builder(
		sendBuffer->GetBuffer(), sendBuffer->GetSize(), InItemDBId, InAmount, &InGrades, (0 == InPriority)
	);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}


void FGsNetSendServiceWorld::Send_ActivateMonsterKnowledgeSlate(MonsterKnowledgeSlateId InSlateId, MonsterKnowledgeSlateSlotId InSlotId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InSlateId, InSlotId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::Send_ActivateMonsterKnowledgeSlateAll(MonsterKnowledgeSlateId InSlateId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT_ALL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InSlateId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::Send_MonsaterKnowledgeSlateReset(ItemDBId InItemDBId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_MONSTER_KNOWLEDGE_SLATE_RESET_ITEM_USE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InItemDBId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::Send_Codex(CodexCategoryId InCategoryId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_CODEX_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InCategoryId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::Send_MonsterBookPointExchange()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_MONSTER_BOOK_POINT_EXCHANGE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::Send_CodexCategoryLevelUp(CodexCategoryId InCategoryId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_CODEX_CATEGORY_LEVEL_UP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InCategoryId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::Send_ActivateSlateSpecialNode(MonsterKnowledgeSlateId InSlateId, SlateSpecialNodeId InSpecialNodeId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_ACTIVATE_SLATE_SPECIAL_NODE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
		InSlateId, InSpecialNodeId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqStatisticsInfo(StatisticsTabId InTabId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_STATISTICS_INFO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), 
				InTabId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendReqStatisticsDailyInfo(StatisticsId InId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_STATISTICS_DAILY_INFO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), 
				InId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::Send_TitleEquip(TitleId InEquipTitleId, TitleEquipSlot InEquipTitleSlot)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_TITLE_EQUIP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InEquipTitleId, InEquipTitleSlot);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::Send_TitleConfirm(const TArray<TitleId>& InList)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_TITLE_CONFIRM_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), &InList);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendCancelCamp(CampId InCampId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_GUILD_CAMP_DESTRUCTION_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InCampId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendReqSanctumNexusInstall(SanctumAreaId InSanctumId, int64 InInteractNpcGameId, FVector InCurrPos, int16 InCurrDir)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_SANCTUM_NEXUS_INSTALL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InSanctumId, InInteractNpcGameId, InCurrPos, InCurrDir);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

//void FGsNetSendServiceWorld::SendReqSanctumMapInfo(MapId InMapId)
//{
//	if (FGsNetManager* netMgr = GNet())
//	{
//		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
//		if (netBase.IsValid())
//		{
//			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
//			PD::CS::PKT_CS_REQ_MAP_SANCTUM_INFO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
//				InMapId);
//			netBase->Send(builder.mPktWriter.GetPktSize());
//		}
//	}
//}

void FGsNetSendServiceWorld::SendWarpGuildCamp(CampId InCampId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("WarpGuildCamp")));

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_GUILD_CAMP_WARP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InCampId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::Send_ReqInvadeInfo()
{
	if (FGsNetManager* netMgr = GNet())
	{
		if (auto netBase = netMgr->GetActive())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_INVADE_INFO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::Send_ReqInvasionPortalInfo()
{
	if (FGsNetManager* netMgr = GNet())
	{
		if (auto netBase = netMgr->GetActive())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_INVADE_PORTAL_INFO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());

			GSLOG(Log, TEXT("Send_ReqInvasionPortalInfo SENT"));
		}
		//else if(auto interserverManager = GSInterServer())
		//{
		//	GSLOG(Log, TEXT("Send_ReqInvasionPortalInfo WAIT"));
		//	interserverManager->NotReadyInvasionPortalSpot();
		//}

		GSLOG(Log, TEXT("Send_ReqInvasionPortalInfo"));
	}
}

void FGsNetSendServiceWorld::Send_ReqActiveInvasionPortalList()
{
	if(FGsNetManager * netMgr = GNet())
	{
		if (auto netBase = netMgr->GetActive())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ACTIVE_INVADE_PORTAL_INFO_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendReqTutorialSave(TutorialId InTutorialId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_TUTORIAL_SAVE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InTutorialId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendReqDuelInvite(UserDBId InUserDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_DUEL_INVITE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InUserDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendReqDuelInviteCancel()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_DUEL_INVITE_CANCEL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendReqDuelAccept(bool InIsAccept)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_DUEL_ACCEPT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InIsAccept);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceWorld::SendReqActiveBattlePassAddReward(BattlePassId passId, ItemDBId itemId /*= 0*/)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	if (auto netBase = netMgr->GetActive())
	{
		FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
		PD::CS::PKT_CS_REQ_BATTLE_PASS_ACTIVATE_ADDITIONAL_REWARD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), passId, itemId);
		netBase->Send(builder.mPktWriter.GetPktSize());
	}
}

void FGsNetSendServiceWorld::SendReqBuyBattlePassLevel(BattlePassId passId, Level targetLevel)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	if (auto netBase = netMgr->GetActive())
	{
		FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
		PD::CS::PKT_CS_REQ_BATTLE_PASS_BUY_LEVEL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), passId, targetLevel);
		netBase->Send(builder.mPktWriter.GetPktSize());
	}

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqGetBattlePassLevelReward(BattlePassId passId, Level rewardLevel, bool isaddReward /*= false*/)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	if (auto netBase = netMgr->GetActive())
	{
		FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
		PD::CS::PKT_CS_REQ_BATTLE_PASS_REWARD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), passId, rewardLevel, isaddReward);
		netBase->Send(builder.mPktWriter.GetPktSize());
	}
}

void FGsNetSendServiceWorld::SendReqBattlePassLevelRewardList(BattlePassId passId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	if (auto netBase = netMgr->GetActive())
	{
		FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
		PD::CS::PKT_CS_REQ_BATTLE_PASS_REWARD_ALL_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), passId);
		netBase->Send(builder.mPktWriter.GetPktSize());
	}
}

void FGsNetSendServiceWorld::SendReqBattlePassReceiveRewardAll(BattlePassId passId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (false == netBase.IsValid())
		return;

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_BATTLE_PASS_REWARD_ALL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), passId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendPartyTargetShare(int64 In_targetGameId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_PARTY_TARGET_SHARE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), In_targetGameId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendPartyTargetShareCancel()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_PARTY_TARGET_SHARE_CANCEL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqUserStatBookmarkList()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_USER_STAT_BOOKMARK_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqUserStatBookmarkAdd(StatType inStatType)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_USER_STAT_BOOKMARK_ADD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), (uint16)inStatType);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqUserStatBookmarkDelete(StatType inStatType)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_USER_STAT_BOOKMARK_DELETE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), (uint16)inStatType);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}


void FGsNetSendServiceWorld::Send_PKBookCheckOutTime(int64 InCheckOutTime)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_PK_BOOK_CHECKOUT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InCheckOutTime);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::Send_ReqPKBookDetailInfo(PKRecordId InRecordId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_PK_BOOK_DETAIL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InRecordId);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::Send_ReqPKBookGoForRevenge(PKRecordId InRecordId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_PK_BOOK_REVENGE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InRecordId);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::Send_ReqPKBookSendTeaseMessage(PKRecordId InRecordId, const FText& InMessage)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_PK_BOOK_TEASE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InRecordId, InMessage.ToString());
	netBase->Send(builder.mPktWriter.GetPktSize());

	//FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::Send_ReqPKBookAddDetectionTarget(WorldId InHomeWorldId, UserDBId InUserId, const FText& InTargetName /* = FText::GetEmpty() */)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_ADD_MONITORING_USER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InUserId, InTargetName.ToString(), InHomeWorldId);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::Send_ReqPKBookDeleteDetectionTarget(UserDBId InUserId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_DELETE_MONITORING_USER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InUserId);
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::Send_ReqPKBookSavePKMessage(bool InLogInOption, bool InUseMessage, const FText& InKillMessage, const FText& InDieMessage)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_SAVE_PK_MESSAGE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize()
		, InLogInOption, InUseMessage, InKillMessage.ToString(), InDieMessage.ToString());
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::Send_ReqSavedMockeryMessage(bool InUsed, const FText& InMessage)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_SAVE_PK_TEASE_MESSAGE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InUsed, InMessage.ToString());
	netBase->Send(builder.mPktWriter.GetPktSize());

	FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceWorld::SendReqUserSocialEmotion(UserSocialEmotion emotion)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_USER_SOCIAL_EMOTION_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), emotion); 
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqUserPresetChange(PresetId InPresetId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_USER_PRESET_CHANGE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InPresetId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqUserPresetCopyTo(PresetId InPresetId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_USER_PRESET_COPY_TO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InPresetId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqUserPresetName(PresetId InPresetId, const FString& InPresetName)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_USER_PRESET_NAME_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InPresetId, InPresetName);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqPersonalTradingTry(int64 InTargetGameId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_PERSONAL_TRADING_TRY_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InTargetGameId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqPersonalTradingAccept(int64 InOwnerGameId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_PERSONAL_TRADING_ACCEPT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InOwnerGameId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqPersonalTradingCancel(int64 InOwnerGameId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_PERSONAL_TRADING_CANCEL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InOwnerGameId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqPersonalTradingAbort()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_PERSONAL_TRADING_ABORT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqPersonalTradingLock(bool InIsLock)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_PERSONAL_TRADING_LOCK_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InIsLock);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqPersonalTradingConfirm(bool InIsConfirm)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_PERSONAL_TRADING_CONFIRM_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), !InIsConfirm);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqPersonalTradingPutItem(ItemDBId InItemDBId, ItemAmount InItemAmount)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_PERSONAL_TRADING_PUT_ITEM_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InItemDBId, InItemAmount);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqPersonalTradingPutDia(Gem InDiaAmount)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_PERSONAL_TRADING_PUT_DIA_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InDiaAmount);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqLatency(const FString& InTimeStamp)
{
//#if WITH_EDITOR  
	GSLOG(Log, TEXT("[Latency] CS_REQ_LATENCY : %s"), *InTimeStamp);
//#endif

	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_LATENCY_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InTimeStamp);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqUserContextOtherServer(const FString& InUserName, WorldId InWorldId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_USER_CONTEXT_UI_OTHER_SERVER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InUserName, InWorldId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}
// 뽑기 마일리지 페어리 보상 아이템 요청
void FGsNetSendServiceWorld::SendReqFairySummonMileage()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_FAIRY_SUMMON_MILEAGE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}
// 뽑기 마일리지 코스튬 보상 아이템 요청
void FGsNetSendServiceWorld::SendReqCostumeSummonMileage()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_COSTUME_SUMMON_MILEAGE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}
// 뽑기 마일리지 유니크 페어리 보상 아이템 요청
void FGsNetSendServiceWorld::SendReqUniqueFairySummonMileage()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_UNIQUE_FAIRY_SUMMON_MILEAGE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}
// 뽑기 마일리지 유니크 코스튬 보상 아이템 요청
void FGsNetSendServiceWorld::SendReqUniqueCostumeSummonMileage()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_UNIQUE_COSTUME_SUMMON_MILEAGE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}
// 커뮤니티소녀 미션 보상요청
void FGsNetSendServiceWorld::SendReqCommunityEventMissionReward(CommunityEventMissionId InMissionId,
	CommunityEventMissionTabId InTabId, CommunityEventMissionCommonId InCommonId, bool InIsBonus)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	GSLOG(Log, TEXT("[CommunityEventMission] COMMUNITY_EVENT_MISSION_REWARD - MissionId: %llu, TabId: %llu, CommonId: %llu, IsBonus: %d"), 
		InMissionId, InTabId, InCommonId, InIsBonus);

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_COMMUNITY_EVENT_MISSION_REWARD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InMissionId, InTabId, InCommonId, InIsBonus);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

// InterServer 간 유저 정보 확인-채팅
void FGsNetSendServiceWorld::SendReqCheckInterUserExistByName(const FString& nickName, WorldId worldId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_CHECK_USER_EXIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), nickName, worldId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendServiceWorld::SendReqBattleArenaGuildWarDetailScore()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CS::PKT_CS_REQ_GUILDWAR_INGAME_DETAIL_INFO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}