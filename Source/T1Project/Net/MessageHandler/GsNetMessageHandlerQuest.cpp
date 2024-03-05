// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNetMessageHandlerQuest.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Engine/World.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsMultiLevelRankManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/Quest/GsQuestHandler.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupOKTwoMessage.h"

#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsGuildMessageParam.h"
#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "Message/GsMessageUI.h"

#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"

#include "Item/GsItemManager.h"

#include "Quest/Table/GsQuestRankTableCacheSet.h"
#include "Quest/Table/GsQuestGuildTableCacheSet.h"
#include "Quest/GsQuestData.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateQuestBoard.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"

#include "DataSchema/Quest/Guild/GsSchemaQuestGuildStory.h"
#include "UTIL/GsText.h"
#include "T1Project.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Sound/GsSoundPlayer.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

using namespace PD::SC;

void FGsNetMessageHandlerQuest::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalWorld& world = inManager->GetProtocalWorld();

	InitializeMessageDelegateList(&world);

	world.AddRaw(SCPacketId::SC_QUEST, this, &FGsNetMessageHandlerQuest::UserQuestAck);

	world.AddRaw(SCPacketId::SC_ACK_QUEST_ACCEPT, this, &FGsNetMessageHandlerQuest::QuestMainAcceptAck);
	world.AddRaw(SCPacketId::SC_ACK_QUEST_SUB_ACCEPT, this, &FGsNetMessageHandlerQuest::QuestSubAcceptAck);
	world.AddRaw(SCPacketId::SC_ACK_QUEST_REPEAT_ACCEPT, this, &FGsNetMessageHandlerQuest::QuestRepeatAcceptAck);

	world.AddRaw(SCPacketId::SC_QUEST_UPDATE, this, &FGsNetMessageHandlerQuest::QuestUpDateNotify);
	
	world.AddRaw(SCPacketId::SC_ACK_QUEST_REWARD, this, &FGsNetMessageHandlerQuest::QuestMainCompleteRewardAck);
	world.AddRaw(SCPacketId::SC_ACK_QUEST_SUB_REWARD, this, &FGsNetMessageHandlerQuest::QuestSubCompleteRewardAck);
	world.AddRaw(SCPacketId::SC_ACK_QUEST_REPEAT_REWARD, this, &FGsNetMessageHandlerQuest::QuestRepeatCompleteRewardAck);

	world.AddRaw(SCPacketId::SC_ACK_QUEST_SUB_GIVE_UP, this, &FGsNetMessageHandlerQuest::QuestSubGiveUpAck);
	world.AddRaw(SCPacketId::SC_ACK_QUEST_REPEAT_GIVE_UP, this, &FGsNetMessageHandlerQuest::QuestRepeatGiveUpAck);

	world.AddRaw(SCPacketId::SC_ACK_QUEST_MULTI_LEVEL_RANK_ACCEPT, this, &FGsNetMessageHandlerQuest::QuestRankAcceptAck);

	world.AddRaw(SCPacketId::SC_ACK_QUEST_GUIDE_ACCEPT, this, &FGsNetMessageHandlerQuest::QuestGuideAcceptAck);
	world.AddRaw(SCPacketId::SC_ACK_QUEST_GUIDE_REWARD, this, &FGsNetMessageHandlerQuest::QuestGuideCompleteRewardAck);

	//quest board
	world.AddRaw(SCPacketId::SC_ACK_BOARD_QUEST_LIST, this, &FGsNetMessageHandlerQuest::QuestBoardListAck);
	world.AddRaw(SCPacketId::SC_BOARD_QUEST_LIST, this, &FGsNetMessageHandlerQuest::QuestBoardList);
	world.AddRaw(SCPacketId::SC_ACK_BOARD_QUEST_REFRESH, this, &FGsNetMessageHandlerQuest::QuestBoardRefreshAck);
	world.AddRaw(SCPacketId::SC_ACK_BOARD_QUEST_ACCEPT, this, &FGsNetMessageHandlerQuest::QuestBoardAcceptAck);

	// Guild Quest
	world.AddRaw(SCPacketId::SC_ACK_QUEST_GUILD_ACCEPT, this, &FGsNetMessageHandlerQuest::QuestGuildAcceptAck);
	world.AddRaw(SCPacketId::SC_ACK_QUEST_GUILD_REWARD, this, &FGsNetMessageHandlerQuest::QuestGuildCompleteRewardAck);
	world.AddRaw(SCPacketId::SC_ACK_QUEST_GUILD_GIVE_UP, this, &FGsNetMessageHandlerQuest::QuestGuildGiveUpAck);
	world.AddRaw(SCPacketId::SC_ACK_QUEST_GUILD_REFRESH, this, &FGsNetMessageHandlerQuest::QuestGuildRefreshAck);

	world.AddRaw(SCPacketId::SC_QUEST_REPEAT_REWARD_BOX, this, &FGsNetMessageHandlerQuest::QuestRepeatRewardBoxAck);
	world.AddRaw(SCPacketId::SC_QUEST_REPEAT_REWARD_BOX_LIST, this, &FGsNetMessageHandlerQuest::QuestRepeatRewardBoxListAck);
}

void FGsNetMessageHandlerQuest::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerQuest::UserQuestAck(const FGsNet::Packet& inPacket)
{	
	PKT_SC_QUEST_READ* pkt = reinterpret_cast<PKT_SC_QUEST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsGameDataManager* gameData = GGameData())
	{
		gameData->Set(*pkt);
	}
}

void FGsNetMessageHandlerQuest::QuestMainAcceptAck(const FGsNet::Packet & inPacket)
{	
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_MAIN_ACCEPT Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_MAIN_ACCEPT Failed, nullptr == questHandler"));
		return;
	}

	PKT_SC_ACK_QUEST_ACCEPT_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUEST_ACCEPT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	QuestId questId = pkt->QuestId();
	questHandler->RemoveRequestList(QuestKey(questId));

	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("[Quest] ACK_QUEST_ACCEPT Failed, Result = %d"), result);
		if (questManager->IsAutoProgressQuest(QuestKey(questId)))
		{
			questManager->ClearAutoProgressQuestKey();
			GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
		}	

		return QuestErrorMsg(static_cast<PD::Result>(result));
	}

//#if WITH_EDITOR 
	GSLOG(Log, TEXT("[QUEST] Quest_Main_Ack_Accept- ID: %llu"), questId);
//#endif

	questHandler->AcceptQuest(0, QuestKey(questId));

	// ui 갱신
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_WINDOW_UPDATE, nullptr);
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
}

void FGsNetMessageHandlerQuest::QuestSubAcceptAck(const FGsNet::Packet & inPacket)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_SUB_ACCEPT Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest]QUEST_SUB_ACCEPT Failed, nullptr == questHandler"));
		return;
	}

	PKT_SC_ACK_QUEST_SUB_ACCEPT_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUEST_SUB_ACCEPT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	QuestId questId = pkt->QuestId();
	questHandler->RemoveRequestList(QuestKey(questId));

	StoryId storyId = pkt->StoryId();
	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("[Quest] ACK_QUEST_SUB_ACCEPT Failed, Result = %d"), result);

		// Quest 인터랙션이 조건에 부합하지 않아 캔들됨
		GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_QUEST_CANCEL, nullptr);

		if (questManager->IsAutoProgressQuest(QuestKey(questId)))
		{
			questManager->ClearAutoProgressQuestKey();
			GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
		}

		return QuestErrorMsg(static_cast<PD::Result>(result));
	}

//#if WITH_EDITOR 
	GSLOG(Log, TEXT("[QUEST] Quest_Sub_Ack_Accept- StoryID : %d, QuestID : %llu"), storyId, questId);
//#endif

	questHandler->AcceptQuestSub(storyId, QuestKey(questId));

	// ui 갱신
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_WINDOW_UPDATE, nullptr);
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
}

void FGsNetMessageHandlerQuest::QuestRepeatAcceptAck(const FGsNet::Packet & inPacket)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_REPEAT_ACCEPT Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_REPEAT_ACCEPT Failed, nullptr == questHandler"));
		return;
	}

	PKT_SC_ACK_QUEST_REPEAT_ACCEPT_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUEST_REPEAT_ACCEPT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	QuestId questId = pkt->QuestId();
	QuestIndex questIndex = pkt->QuestIndex();
	questHandler->RemoveRequestList(QuestKey(questId, questIndex));

	StoryId storyId = pkt->StoryId();
	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("[Quest] ACK_QUEST_REPEAT_ACCEPT Failed, Result = %d"), result);

		if (questManager->IsAutoProgressQuest(QuestKey(questId)))
		{
			questManager->ClearAutoProgressQuestKey();
			GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
		}

		return QuestErrorMsg(static_cast<PD::Result>(result));
	}

//#if WITH_EDITOR 
	GSLOG(Log, TEXT("[QUEST] Quest_Repeat_Ack_Accept- StoryID : %d, QuestID : %llu, QuestIndex : %llu"), storyId, questId, questIndex);
//#endif

	questHandler->AcceptQuestRepeat(storyId, QuestKey(questId, questIndex));
}


void FGsNetMessageHandlerQuest::QuestRankAcceptAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_RANK_ACCEPT Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest]QUEST_RANK_ACCEPT Failed, nullptr == questHandler"));
		return;
	}

	PKT_SC_ACK_QUEST_MULTI_LEVEL_RANK_ACCEPT_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUEST_MULTI_LEVEL_RANK_ACCEPT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("[Quest] ACK_QUEST_RANK_ACCEPT Failed, Result = %d"), result);
		return QuestErrorMsg(static_cast<PD::Result>(result));
	}

	StoryId storyId = pkt->StoryId();
	TSharedPtr<FGsQuestMultiLevelRankStoryTableCache> rankStoryTableCache = questManager->GetQuestRankStoryTableCache(storyId);
	if (false == rankStoryTableCache.IsValid())
		return;

	for (QuestId questId : rankStoryTableCache->GetQuestIdList())
	{
		questHandler->RemoveRequestList(QuestKey(questId));
		questHandler->AcceptQuest(storyId, QuestKey(questId));
//#if WITH_EDITOR 
		GSLOG(Log, TEXT("[QUEST] Quest_Rank_Ack_Accept- StoryID : %d, QuestID : %llu"), storyId, questId);
//#endif
	}

	// 다단계 랭크 시스템에 수락 요청 결과를 알린다.
	if (nullptr != GSMultiLevelRank())
		GSMultiLevelRank()->Ack_QuestMultiLevelRankAccept(pkt->MultiLevelRankId());
}

void FGsNetMessageHandlerQuest::QuestGuideAcceptAck(const FGsNet::Packet& inPacket)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_GUIDE_ACCEPT_READ Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_GUIDE_ACCEPT_READ Failed  nullptr == questHandler"));
		return;
	}

	PKT_SC_ACK_QUEST_GUIDE_ACCEPT_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUEST_GUIDE_ACCEPT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	QuestId questId = pkt->QuestId();
	questHandler->RemoveRequestList(QuestKey(questId));
	
	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_GUIDE_ACCEPT_READ Failed, Result = %d"), result);

		if (questManager->IsAutoProgressQuest(QuestKey(questId)))
		{
			questManager->ClearAutoProgressQuestKey();
			GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
		}

		return QuestErrorMsg(static_cast<PD::Result>(result));
	}

//#if WITH_EDITOR 
	GSLOG(Log, TEXT("[QUEST] Quest_Guide_Ack_Accept- QuestID : %llu"), questId);
//#endif

	questHandler->AcceptQuestGuide(QuestKey(questId));

	// ui 갱신
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);	
	GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_GUIDE_ACCEPTED, nullptr);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_WINDOW_UPDATE, nullptr);
	//GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_WINDOW_UPDATE_OBJECTIVE, nullptr);
}

void FGsNetMessageHandlerQuest::QuestUpDateNotify(const FGsNet::Packet & inPacket)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_UPDATE Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_UPDATE Failed, nullptr == questHandler"));
		return;
	}

	PKT_SC_QUEST_UPDATE_READ* pkt = reinterpret_cast<PKT_SC_QUEST_UPDATE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_UPDATE Failed, Result = %d"), result);
		return QuestErrorMsg(static_cast<PD::Result>(result));
	}

//#if WITH_EDITOR
//	GSLOG(Log, TEXT("[QUEST] QUEST_Ack_UpDate"));
//#endif

	bool isUpdate = pkt->IsUpdate();
	bool isDebugTest = questManager->GetDebugTest();

	bool isQuestStateCompleted = false;

	for (PD::SC::PKT_SC_QUEST_UPDATE_READ::QuestUpdateDataIterator iter = pkt->GetFirstQuestUpdateDataIterator();
		iter != pkt->GetLastQuestUpdateDataIterator(); ++iter)
	{
		QuestId questId = iter->QuestId();
		QuestIndex questIndex = iter->QuestIndex();
		StoryId storyId = iter->StoryId();
		QuestState state = iter->QuestState();
		QuestType questType = iter->QuestType();
		EventActionIndex preEventActionIndex = iter->PreEventActionIndex();
		EventActionIndex postEventActionIndex = iter->PostEventActionIndex();
		EventActionIndex postStoryEventActionIndex = iter->PostStoryEventActionIndex();

		if (false == isQuestStateCompleted && questType == QuestType::REPEAT && state == QuestState::COMPLETED){
			isQuestStateCompleted = true;
		}

		const FString strState = QuestStateEnumToString(state);
		const FString strType = QuestTypeEnumToString(questType);

		GSLOG(Log, TEXT("[Quest] SC_QUEST_UPDATE -> questId : %llu, questIndex : %d, storyId : %d, state : %s, questType : %s, preEventActionIndex : %d, postEventActionIndex : %d"),
			questId, questIndex, storyId, *strState, *strType, preEventActionIndex, postEventActionIndex);
				
		if (isDebugTest && state == QuestState::REWARDED)
		{
			// 보상 받은 퀘스트의 업데이트 패킷이 왔다.
			// 메인퀘스트의 경우 보상 받고 나면 다음 스토리의 첫번째 퀘스트로 전진한 상태
			// 종전의 퀘스트가 업데이트로 들어오면
			// 스토리의 마지막 퀘스트일 경우 다음 스토리의 첫번째 퀘스트를 검색할때
			// 스토리를 물리적으로 다음 스토리로 전진다.
			// 이 상태로 계속 업데이트가 불리게 되면 스토리가 계속 전진하게 되어
			// 현재는 습득할 수 없는 퀘스트까지 상태가 옴겨갈수있다.
			GSLOG(Log, TEXT("[Quest] QUEST_Ack_UpDate_info QuestState Error ID: %llu, STATE: %d"), questId, static_cast<int>(state));
			continue;
		}
#//if WITH_EDITOR
		GSLOG(Log, TEXT("[Quest] QUEST_Ack_UpDate_info ID: %llu, STATE: %d"), questId, static_cast<int>(state));
//#endif
		TArray<int32> objectiveValueList;
		int32 objectiveNum = iter->GetQuestUpdateObjectiveDataCount();
		for (PD::SC::PKT_SC_QUEST_UPDATE_READ::QuestUpdateData::QuestUpdateObjectiveDataIterator ObjectiveIter = iter->GetFirstQuestUpdateObjectiveDataIterator();
			ObjectiveIter != iter->GetLastQuestUpdateObjectiveDataIterator(); ++ObjectiveIter)
		{
			int32 value = ObjectiveIter->Value();
			objectiveValueList.Emplace(value);

#if WITH_EDITOR
			GSLOG(Log, TEXT("[Quest] QUEST_Ack_UpDate_info Value: %d"), value);
#endif
		}
		questHandler->RemoveRequestList(QuestKey(questId, questIndex));
		questHandler->UpdateQuest(storyId, QuestKey(questId, questIndex), state, objectiveValueList, isUpdate, preEventActionIndex, postEventActionIndex, postStoryEventActionIndex);
	}

	if (isQuestStateCompleted)
		questHandler->OnQuestRepeatComplete();

	// ui 갱신
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_WINDOW_UPDATE_OBJECTIVE, nullptr);
}

void FGsNetMessageHandlerQuest::QuestMainCompleteRewardAck(const FGsNet::Packet & inPacket)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_MAIN_REWARD Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_MAIN_REWARD Failed  nullptr == questHandler"));
		return;
	}

	PKT_SC_ACK_QUEST_REWARD_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUEST_REWARD_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	QuestId questId = pkt->QuestId();
	questHandler->RemoveRequestList(QuestKey(questId));

	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_REWARD Failed, Result = %d"), result);

		QuestRecvErrorMsg(static_cast<PD::Result>(result));
		return;
	}

	// 랜덤보상 연출 메시지
	FGsUIMsgParamRewardRandomItemBag param = FGsUIMsgParamRewardRandomItemBag(pkt->QuestId(),
		pkt->MainRewardItemBagIndex(), pkt->SubRewardItemBagIndex());
	GMessage()->GetUI().SendMessage(MessageUI::REWARD_RANDOM_ITEMBAG_INDEX, &param);

	// 재화 갱신
	GGameData()->Set(*pkt);

//#if WITH_EDITOR  
	GSLOG(Log, TEXT("[Quest] QUEST_ACK_QUEST_REWARD- ID: %llu"), questId);
//#endif
	
	questHandler->RewardQuest(0, questId);

	// ui 갱신
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_WINDOW_UPDATE, nullptr);
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
}

void FGsNetMessageHandlerQuest::QuestSubCompleteRewardAck(const FGsNet::Packet & inPacket)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_SUB_REWARD Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_SUB_REWARD Failed  nullptr == questHandler"));
		return;
	}

	PKT_SC_ACK_QUEST_SUB_REWARD_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUEST_SUB_REWARD_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	QuestId questId = pkt->QuestId();
	questHandler->RemoveRequestList(QuestKey(questId));

	StoryId storyId = pkt->StoryId();
	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_SUB_REWARD Failed, Result = %d"), result);

		QuestRecvErrorMsg(static_cast<PD::Result>(result));
		return;
	}

	// 랜덤보상 연출 메시지
	FGsUIMsgParamRewardRandomItemBag param = FGsUIMsgParamRewardRandomItemBag(pkt->QuestId(),
		pkt->MainRewardItemBagIndex(), pkt->SubRewardItemBagIndex());
	GMessage()->GetUI().SendMessage(MessageUI::REWARD_RANDOM_ITEMBAG_INDEX, &param);

	// 재화 갱신
	GGameData()->Set(*pkt);

//#if WITH_EDITOR 
	GSLOG(Log, TEXT("[Quest] QUEST_SUB_REWARD - StoryID : %d, QuestID : %llu"), storyId, questId);
//#endif

	questHandler->RewardQuestSub(storyId, questId);

	// ui 갱신
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_WINDOW_UPDATE, nullptr);
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::QUEST, false);
}

void FGsNetMessageHandlerQuest::QuestRepeatCompleteRewardAck(const FGsNet::Packet & inPacket)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_REPEAT_REWARD Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_REPEAT_REWARD Failed  nullptr == questHandler"));
		return;
	}

	PKT_SC_ACK_QUEST_REPEAT_REWARD_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUEST_REPEAT_REWARD_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	QuestId questId = pkt->QuestId();
	QuestIndex questIndex = pkt->QuestIndex();
	questHandler->RemoveRequestList(QuestKey(questId, questIndex));

	StoryId storyId = pkt->StoryId();
	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_REPEAT_REWARD Failed, Result = %d"), result);
		
		QuestRecvErrorMsg(static_cast<PD::Result>(result));
		return;
	}

	FGsUIMsgParamRewardRandomItemBag param = FGsUIMsgParamRewardRandomItemBag(pkt->QuestId(),
		pkt->MainRewardItemBagIndex(), pkt->SubRewardItemBagIndex());
	GMessage()->GetUI().SendMessage(MessageUI::REWARD_RANDOM_ITEMBAG_INDEX, &param);

	// 재화 갱신
	GGameData()->Set(*pkt);

#//if WITH_EDITOR
	GSLOG(Log, TEXT("[Quest] QUEST_REPEAT_REWARD - StoryID : %d, QuestID : %llu, QuestIndex : %llu"), storyId, questId, questIndex);
//#endif

	questHandler->RewardQuestRepeat(storyId, QuestKey(questId, questIndex));

	if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
	{
		if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
		{
			if (contents->GetCurrentContentsMode() == FGsContentsMode::ContentsQuestBoard)
			{
				FGsGameStateQuestBoard* questBoard = FGsGameStateQuestBoard::GetInstance();
				if (questBoard)
				{						
#if QUEST_BOARD_DEBUG
					GSLOG(Log, TEXT("quest board debug - complete - quest id : %lld\t story id : %lld\tindex : %lld")
						, (int64)questId
						, (int64)storyId
						, (int64)questIndex);
#endif

					questBoard->CompleteQuestBoard(QuestStoryIdPair(storyId, questId, questIndex));
					questBoard->SelectFirstQuestBoard();					
				}
			}
		}
	}	

	// ui 갱신
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_WINDOW_UPDATE, nullptr);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_REPEAT_WINDOW_UPDATE, nullptr);
	GMessage()->GetUI().SendMessage(MessageUI::INITIALIZE_QUEST_BOARD, nullptr);
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::QUEST, false);
}

void FGsNetMessageHandlerQuest::QuestGuideCompleteRewardAck(const FGsNet::Packet& inPacket)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_GUIDE_REWARD Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_GUIDE_REWARD Failed  nullptr == questHandler"));
		return;
	}

	PKT_SC_ACK_QUEST_GUIDE_REWARD_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUEST_GUIDE_REWARD_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	QuestId questId = pkt->QuestId();
	questHandler->RemoveRequestList(QuestKey(questId));
	
	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_GUIDE_REWARD Failed, Result = %d"), result);

		if (result == PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL) // 인벤토리 풀
		{
			// 가방이 가득찼습니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		if (result == PD::Result::ITEM_ERROR_ADD_WEIGHT_FULL) // 인벤토리 무게 풀
		{
			// 소지 무게가 최대치입니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	FGsUIMsgParamRewardRandomItemBag param = FGsUIMsgParamRewardRandomItemBag(pkt->QuestId(),
		pkt->MainRewardItemBagIndex(), pkt->SubRewardItemBagIndex());
	GMessage()->GetUI().SendMessage(MessageUI::REWARD_RANDOM_ITEMBAG_INDEX, &param);

//#if WITH_EDITOR 
	GSLOG(Log, TEXT("[Quest] QUEST_GUIDE_REWARD - QuestID : %llu"), questId);
//#endif

	questHandler->RewardQuestGuide(0, questId);

	// ui 갱신	
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::QUEST, false);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_WINDOW_UPDATE, nullptr);
}

void FGsNetMessageHandlerQuest::QuestSubGiveUpAck(const FGsNet::Packet& inPacket)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_SUB_GIVE_UP Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_SUB_GIVE_UP Failed  nullptr == questHandler"));
		return;
	}

	PKT_SC_ACK_QUEST_SUB_GIVE_UP_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUEST_SUB_GIVE_UP_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	StoryId storyId = pkt->StoryId();
	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("[Quest] ACK_QUEST_SUB_GIVE_UP Failed, Result = %d"), result);
		return FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
	}

//#if WITH_EDITOR 
	GSLOG(Log, TEXT("[QUEST] Quest_Sub_Give_Up_Ack- StoryID : %d"), storyId);
//#endif

	questHandler->GiveUpQuestSub(storyId);

	// ui 갱신
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_WINDOW_UPDATE, nullptr);
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
}

void FGsNetMessageHandlerQuest::QuestRepeatGiveUpAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_REPEAT_GIVE_UP Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_REPEAT_GIVE_UP Failed  nullptr == questHandler"));
		return;
	}

	PKT_SC_ACK_QUEST_REPEAT_GIVE_UP_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUEST_REPEAT_GIVE_UP_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}
	
	QuestId questId = pkt->QuestId();
	QuestIndex questIndex = pkt->QuestIndex();	

	StoryId storyId = pkt->StoryId();
	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("[Quest] ACK_QUEST_REPEAT_GIVE_UP Failed, Result = %d"), result);
		return FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
	}

//#if WITH_EDITOR 
	GSLOG(Log, TEXT("[QUEST] Quest_Repeat_Give_Up_Ack- StoryID : %d"), storyId);
//#endif

	questHandler->GiveUpQuestRepeat(storyId, QuestKey(questId, questIndex));

	// ui 갱신
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_REPEAT_WINDOW_UPDATE, nullptr);
	
	if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
	{
		if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
		{
			if (contents->GetCurrentContentsMode() == FGsContentsMode::ContentsQuestBoard)
			{
				if (FGsGameStateQuestBoard* board = FGsGameStateQuestBoard::GetInstance())
				{
					board->GiveUpQuestBoard(QuestStoryIdPair(storyId, questId, questIndex), pkt->BoardQuestIndex());

#if QUEST_BOARD_DEBUG
					GSLOG(Log, TEXT("FGsNetMessageHandlerQuest::QuestRepeatGiveUpAck - Acceptable quest count : %d"), pkt->QuestBoardAccetpableCount());
#endif

					board->SetAvailableQuestCount(pkt->QuestBoardAccetpableCount());

					QuestStoryIdPair questStory = QuestStoryIdPair(pkt->StoryId(), pkt->QuestId(), pkt->QuestIndex());
					FGsUIMsgParamSelectQuestBoard param = FGsUIMsgParamSelectQuestBoard(questStory, pkt->BoardQuestIndex());

					GMessage()->GetUI().SendMessage(MessageUI::INVALID_QUEST_BOARD_INDEX, &param);
					GMessage()->GetUI().SendMessage(MessageUI::INVALID_QUEST_BOARD, &param);
				}
			}
		}
	}
}

//void FGsNetMessageHandlerQuest::QuestRankCompleteAck(const FGsNet::Packet& inPacket)
//{
//	UGsQuestManager* questManager = GSQuest();
//	if (nullptr == questManager)
//	{
//		GSLOG(Log, TEXT("[Quest] QUEST_REPEAT_GIVE_UP Failed, nullptr == questManager"));
//		return;
//	}
//
//	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
//	if (nullptr == questHandler)
//	{
//		GSLOG(Log, TEXT("[Quest] QUEST_REPEAT_GIVE_UP Failed  nullptr == questHandler"));
//		return;
//	}
//
//	PKT_SC_ACK_QUEST_MULTI_LEVEL_RANK_FINISH_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUEST_MULTI_LEVEL_RANK_FINISH_READ*>(inPacket.Buffer);
//	if (nullptr == pkt)
//	{
//		return;
//	}
//
//	int32_t result = pkt->Result();
//	if (result != PACKET_RESULT_SUCCESS)
//	{
//		GSLOG(Log, TEXT("[Quest] ACK_QUEST_MULTI_LEVEL_RANK_FINISH_READ Failed, Result = %d"), result);
//		return FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
//	}
//	
//	// 퀘스트 삭제
//	questHandler->RemoveRankQuest(pkt->StoryId());
//
//	// Data 처리
//	GSMultiLevelRank()->Ack_QuestMultiLevelRankComplete(pkt->MultiLevelRankId());
//}

void FGsNetMessageHandlerQuest::QuestBoardListAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PKT_SC_ACK_BOARD_QUEST_LIST_READ* pkt = reinterpret_cast<PKT_SC_ACK_BOARD_QUEST_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	int64 result = pkt->Result();
	if (result == PACKET_RESULT_SUCCESS)
	{		
		return;
	}

	FGsGameStateQuestBoard::PopupNetError(static_cast<PD::Result>(result), pkt->NpcId());
}

void FGsNetMessageHandlerQuest::QuestBoardList(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PKT_SC_BOARD_QUEST_LIST_READ* pkt = reinterpret_cast<PKT_SC_BOARD_QUEST_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}	

	FGsGameStateQuestBoard* questBoard = FGsGameStateQuestBoard::GetInstance();
	if (questBoard)
	{
		questBoard->SetQuestBoardData(pkt);
		questBoard->SelectFirstQuestBoard();
	}	
	
	//퀘스트 보드 상태로 변경 
	FGsInteractionMessageParamContents sendParam(pkt->NpcId(), 0, 0);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_NPC_START, &sendParam);	
	
	GMessage()->GetUI().SendMessage(MessageUI::INITIALIZE_QUEST_BOARD, nullptr);
}

void FGsNetMessageHandlerQuest::QuestBoardRefreshAck(const FGsNet::Packet& inPacket)
{
	PKT_SC_ACK_BOARD_QUEST_REFRESH_READ* pkt = reinterpret_cast<PKT_SC_ACK_BOARD_QUEST_REFRESH_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}	

	int64 result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));		
	}	

	if (FGsGameStateQuestBoard* board = FGsGameStateQuestBoard::GetInstance())
	{		
		board->SetSelectFirst(true);
	}

	GMessage()->GetUI().SendMessage(MessageUI::INITIALIZE_QUEST_BOARD, nullptr);
}

void FGsNetMessageHandlerQuest::QuestBoardAcceptAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PKT_SC_ACK_BOARD_QUEST_ACCEPT_READ* pkt = reinterpret_cast<PKT_SC_ACK_BOARD_QUEST_ACCEPT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}
	
	int64 result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	//https://jira.com2us.com/jira/secure/Dashboard.jspa
	//퀘스트 보드 리스트에 원래 인덱스가 0이였는데
	//기획자들이 데이터를 여러번 치기 어려워 하나의 퀘스트로 여러개의 퀘스트를 만들어달라고 요청했다.
	//예를 들어 최대 의뢰게시판 획득 수량이 6개였는데 기획자가 3개만 데이터를 넣었다고 하자
	//화면에는 6개의 반복 퀘스트를 표현해야하는데 이렇게 하려면 퀘스트에 인덱스가 있어야한다. 
	//기존에는 같은 퀘스트더라도 퀘스트 id가 달랐기 때문에 같은 아이디 처럼 보였다.
	//이제 인덱스로 퀘스트 게시판을 구별한다.
	//서버에서는 퀘스트 게시판에 진행하지 않은 반복 퀘스트의 인덱스는 100 이상으로 넣는다고 한다. 
	//현재 진행 중인 퀘스트는 1 ~ 7사이에 진행한다고 한다. 
	//따라서 퀘스트 인덱스를 변경해줘야하는 이슈가 있다. 
	FGsUIMsgParamSelectQuestBoard param = FGsUIMsgParamSelectQuestBoard(pkt->QuestStoryId(), pkt->QuestIndex());
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_QUEST_BOARD_INDEX, &param);

	if (FGsGameStateQuestBoard* questBoard = FGsGameStateQuestBoard::GetInstance())
	{
		questBoard->SetAvailableQuestCount(pkt->AcceptableCount());
		questBoard->AcceptQuestBoard(pkt->QuestStoryId(), pkt->QuestIndex());
		questBoard->SelectFirstQuestBoard();

#if QUEST_BOARD_DEBUG
		GSLOG(Log, TEXT("FGsNetMessageHandlerQuest::QuestBoardAcceptAck - acceptable quest count : %d"), pkt->AcceptableCount());
#endif		
	}	

	UGsQuestManager* questManager = GSQuest();
	if (questManager)
	{
		FGsQuestHandler* questHandler = questManager->GetQuestHandler();
		if (questHandler)
		{
			questHandler->AcceptQuestBoard(pkt->QuestStoryId().mStoryId, QuestKey(pkt->QuestStoryId().mQuestId, pkt->QuestIndex()));			
		}
	}	

#if QUEST_BOARD_DEBUG
	GSLOG(Log, TEXT("quest board debug - accept quest index - quest id : %lld\t story id : %lld\tindex : %lld")
		, (int64)pkt->QuestStoryId().mQuestId
		, (int64)pkt->QuestStoryId().mStoryId
		, (int64)pkt->QuestIndex());
#endif	

	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (soundPlayer)
	{
		soundPlayer->PlaySound2D("UI_RequestAccept");
	}
		
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_QUEST_BOARD, &param);
}

void FGsNetMessageHandlerQuest::QuestGuildAcceptAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PKT_SC_ACK_QUEST_GUILD_ACCEPT_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUEST_GUILD_ACCEPT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		GuildQuestErrorMsg(static_cast<PD::Result>(pkt->Result()));
		return;
	}	

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_GUILD_ACCEPT Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_GUILD_ACCEPT Failed, nullptr == questHandler"));
		return;
	}

	// 주의: ACCEPT의 경우 진행도 갱신을 위해 MessageContentGuild::UPDATE_QUEST 메시지를 
	// FGsQuestGuildInstance::SetQuestState 에서 수행하므로
	// questHandler의 처리보다 앞에 위치해야 함
	if (FGsGuildQuestDataHandler* guildQuestDataHandler = GGuild()->GetQuestHandler())
	{
		guildQuestDataHandler->SetGuildQuestState(pkt->StoryId(), pkt->QuestId(), QuestState::ACCEPTED);
	}

	questHandler->RemoveRequestList(QuestKey(pkt->QuestId()));

	// B2때 동일 퀘스트를 계속 받을 경우, 이전 퀘스트 상태, 카운트가 남아 문제되던 이슈가 있었음(C2URWQ-5588)
	// U1: 위의 이슈를 해결하기 위해 GsQuestGuildInstance의 Initialize쪽에서 AllResetDynamicData를 호출했더니
	// 첫 접속 시 카운트 안나오는 문제가 있어서, Initialize 보다 전에 초기화 하면서 다른 퀘스트 로직에 영향이 없도록 여기에서 수정
	TSharedPtr<FGsQuestData> questData = questManager->GetQuestData(pkt->QuestId());
	if (questData.IsValid())
	{
		questData->AllResetDynamicData();
	}

	questHandler->AcceptQuest(pkt->StoryId(), QuestKey(pkt->QuestId()));

	// TEXT: 개인 목표를 수락하였습니다
	FText textMsg;
	if (FText::FindText(TEXT("GuildText"), TEXT("UI_GuildQuest_Ticker_Accept"), textMsg))
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}	

#if WITH_EDITOR
	GSLOG(Log, TEXT("[QUEST] QUEST_GUILD_ACCEPT - StoryID : %d, QuestID : %llu"), pkt->StoryId(), pkt->QuestId());
#endif
}

void FGsNetMessageHandlerQuest::QuestGuildCompleteRewardAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PKT_SC_ACK_QUEST_GUILD_REWARD_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUEST_GUILD_REWARD_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}	

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_GUILD_REWARD Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_GUILD_REWARD Failed, nullptr == questHandler"));
		return;
	}

	QuestId questId = pkt->QuestId();
	questHandler->RemoveRequestList(QuestKey(questId));

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		GuildQuestErrorMsg(static_cast<PD::Result>(pkt->Result()));
		return;
	}

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Quest] QUEST_GUILD_REWARD - StoryID : %d, QuestID : %llu"), pkt->StoryId(), pkt->QuestId());
#endif

	// 퀘스트 정보 갱신
	questHandler->RewardQuestGuild(pkt->StoryId(), pkt->QuestId());
	
	FGsGuildManager* guildMgr = GGuild();	
	// zealPoint 갱신
	GuildZealPoint zealPoint = guildMgr->GetGuildZealPoint();;
	TSharedPtr<FGsQuestGuildStoryTableCache> cacheTable = questManager->GetQuestGuildStoryTableCache(pkt->StoryId());
	if (cacheTable.IsValid())
	{
		if (const FGsSchemaQuestGuildStory* guildQuestStory = cacheTable->_schemaStoryItem)
		{
			zealPoint += guildQuestStory->zealPointAmount;
		}
	}
	guildMgr->SetGuildZealPoint(zealPoint);

	// 길드 퀘스트 데이타 갱신
	if (FGsGuildQuestDataHandler* guildQuestDataHandler = guildMgr->GetQuestHandler())
	{
		guildQuestDataHandler->SetGuildQuestState(pkt->StoryId(), pkt->QuestId(), QuestState::REWARDED);
		guildQuestDataHandler->SetCompleteCount(pkt->QuestCompleteCount());

		FGsGuildMsgParamQuest param(pkt->StoryId(), pkt->QuestId(), true);
		GMessage()->GetGuild().SendMessage(MessageContentGuild::UPDATE_QUEST, &param);
	}

	// ui 갱신
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::QUEST, false);
}

void FGsNetMessageHandlerQuest::QuestGuildGiveUpAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_GUILD_GIVE_UP Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_GUILD_GIVE_UP Failed  nullptr == questHandler"));
		return;
	}

	PKT_SC_ACK_QUEST_GUILD_GIVE_UP_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUEST_GUILD_GIVE_UP_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}
	
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		GuildQuestErrorMsg(static_cast<PD::Result>(pkt->Result()));
		return;
	}

#if WITH_EDITOR 
	GSLOG(Log, TEXT("[QUEST] QUEST_GUILD_GIVE_UP - StoryID : %d, QuestID : %llu"), pkt->StoryId(), pkt->QuestId());
#endif

	questHandler->GiveUpQuestGuild(pkt->StoryId());

	// 주의: GiveUp의 경우 FGsQuestGuildInstance::SetQuestState 가 안불리므로 갱신메시지를 따로 보냄
	if (FGsGuildQuestDataHandler* guildQuestDataHandler = GGuild()->GetQuestHandler())
	{
		guildQuestDataHandler->SetGuildQuestState(pkt->StoryId(), pkt->QuestId(), QuestState::NONE);

		FGsGuildMsgParamQuest param(pkt->StoryId(), pkt->QuestId(), false);
		GMessage()->GetGuild().SendMessage(MessageContentGuild::UPDATE_QUEST, &param);
	}

	// ui 갱신
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_WINDOW_UPDATE, nullptr);
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);

	/*FGsUIMsgParamSortQuestBoard sortParam(EGsQuestBoardSortType::Unaccept);
	GMessage()->GetUI().SendMessage(MessageUI::SORT_QUEST_BOARD, &sortParam);*/
}

void FGsNetMessageHandlerQuest::QuestGuildRefreshAck(const FGsNet::Packet& inPacket)
{
	PKT_SC_ACK_QUEST_GUILD_REFRESH_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUEST_GUILD_REFRESH_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		FGsUIHelper::HideBlockUI();
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::HideBlockUI();
		GuildQuestErrorMsg(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsGuildQuestDataHandler* guildQuestDataHandler = GGuild()->GetQuestHandler())
	{
		guildQuestDataHandler->SetQuestRefresh(pkt);
	}

	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_QUEST_REFRESH, nullptr);
	FGsUIHelper::HideBlockUI();
}

void FGsNetMessageHandlerQuest::QuestRepeatRewardBoxAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PKT_SC_QUEST_REPEAT_REWARD_BOX_READ* pkt = reinterpret_cast<PKT_SC_QUEST_REPEAT_REWARD_BOX_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	int64 result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	QuestIndex questIndex = pkt->QuestIndex();
	uint8 refreshCount = pkt->RefreshCount();
	int rewardListSize = pkt->RewardBoxItemIdListSize();

	using rewardBoxItemIdArray = TArray<RewardBoxItemIdPair>;
	rewardBoxItemIdArray rewardBoxItemIdList;

	if (0 < rewardListSize)
	{		
		pkt->MakeRewardBoxItemIdListVector<rewardBoxItemIdArray, RewardBoxItemIdPair>(rewardBoxItemIdList);
	}

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_REPEAT_REWARD_BOX Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_REPEAT_REWARD_BOX Failed, nullptr == questHandler"));
		return;
	}

	questHandler->RewardBoxListQuestRepeat(questIndex, refreshCount, pkt->QuestRepeatDataId(), rewardBoxItemIdList);

	GSLOG(Log, TEXT("[QUEST] QUEST_REPEAT_REWARD_BOX- QuestIndex : %llu, RefreshCount : %d, rewardListSize : %d"), questIndex, refreshCount, rewardListSize);

	// ui 갱신
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_WINDOW_UPDATE, nullptr);
}

void FGsNetMessageHandlerQuest::QuestRepeatRewardBoxListAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PKT_SC_QUEST_REPEAT_REWARD_BOX_LIST_READ* pkt = reinterpret_cast<PKT_SC_QUEST_REPEAT_REWARD_BOX_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_REPEAT_REWARD_BOX_LIST Failed, nullptr == questManager"));
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		GSLOG(Log, TEXT("[Quest] QUEST_REPEAT_REWARD_BOX_LIST Failed, nullptr == questHandler"));
		return;
	}

	using RepeatRewardBoxListIterator = PktIterator<PD::SC::PKT_SC_QUEST_REPEAT_REWARD_BOX_LIST_READ::RepeatRewardBoxList>;
	for (RepeatRewardBoxListIterator it = pkt->GetFirstRepeatRewardBoxListIterator();
		it != pkt->GetLastRepeatRewardBoxListIterator(); ++it)
	{
		QuestIndex questIndex = it->QuestIndex();
		uint8 refreshCount = it->RefreshCount();
		
		TArray<RewardBoxItemIdPair> rewardBoxItemIdList;
		int rewardListSize = it->GetRewardBoxItemIdListCount();
		if (0 < rewardListSize)
		{
			using RewardBoxItemIdListIterator = PktIterator<PD::SC::PKT_SC_QUEST_REPEAT_REWARD_BOX_LIST_READ::RepeatRewardBoxList::RewardBoxItemIdList>;
			for (RewardBoxItemIdListIterator itItem = it->GetFirstRewardBoxItemIdListIterator();
				itItem != it->GetLastRewardBoxItemIdListIterator(); ++itItem)
			{
				rewardBoxItemIdList.Emplace(itItem->RewardBoxItemIdPair());
			}
		}

		questHandler->RewardBoxListQuestRepeat(questIndex, refreshCount, it->QuestRepeatDataId(), rewardBoxItemIdList);

		GSLOG(Log, TEXT("[QUEST] QUEST_REPEAT_REWARD_BOX_LIST- QuestIndex : %llu, RefreshCount : %d "), questIndex, refreshCount);
	}
}

void FGsNetMessageHandlerQuest::QuestRecvErrorMsg(PD::Result inResult)
{
	FText findText;

	switch (inResult)
	{
	case PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL: // 인벤토리 풀
		// 가방이 가득찼습니다.
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), findText);
		break;
	case PD::Result::MAIL_ERROR_CURRENCY_EXCEEDED: // 인벤토리 무게 풀
		// 소지 무게가 최대치입니다.
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), findText);
		break;
	}

	if (!findText.IsEmpty())
	{
		FGsUIHelper::TrayMessageTicker(findText);
	}
	else
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(inResult));
	}

	GMessage()->GetUI().SendMessage(MessageUI::QUEST_REWARD_FAIL, nullptr);
}

void FGsNetMessageHandlerQuest::QuestErrorMsg(PD::Result InResult)
{
	FText findText;
	switch (InResult)
	{
	case PD::Result::QUEST_ERROR_INVALID_NOT_SPAWNED_NOT_ALIVE:
		// QUEST - 스폰되지 않았거나, 죽어있는 상태
		FText::FindText(TEXT("QuestUIText"), TEXT("UI_Ticker_SpawnedNotAlive"), findText);
		break;
	case PD::Result::QUEST_ERROR_INVALID_ACCEPT_QUEST_ACTIVE_COUNT_MAX:
		FText::FindText(TEXT("NetText"), TEXT("QUEST_ERROR_INVALID_ACCEPT_QUEST_ACTIVE_COUNT_MAX"), findText);
		break;
	case PD::Result::QUEST_ERROR_INVALID_WARP_QUEST_NOT_TELEPORTABLE:
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorTeleportInvalidState"), findText);
		break;
	case PD::Result::QUEST_ERROR_ALREADY_ACCEPT_QUEST_ID_NOT_POP_UP:
		FText::FindText(TEXT("NetText"), TEXT("QUEST_ERROR_ALREADY_ACCEPT_QUEST_ID"), findText);
	break;
	case PD::Result::QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_WARP_TO_INSTANCE_FIELD:
	case PD::Result::QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_WARP_TO_LAST_PUBLIC_FIELD:
	case PD::Result::QUEST_ERROR_INVALID_UPDATE_NOT_ACCEPTED_TELEPORT:
		{
			// 이 에러는 티커나 팝업을 띄우지 않음
			
			// Jira : https://jira.com2us.com/jira/browse/C2URWQ-5347			
			// WARP_TO_INSTANCE_FIELD
			// WARP_TO_LAST_PUBLIC_FIELD
			// TELEPORT
			// 3개의 타입은 워프가 발생
			// 워프가 정상 진행 했다면 COMPLETED가 오고 다음 퀘스트를 습득할 수 있다.
			// 하지만 워프 중 재연결, 재접속이 발생하면 스폰 시 퀘스트 상태가 ACCEPTED로 올때가 있다.
			
			// 퀘스트에서는 A퀘스트 완료 후 B퀘스트 습득 패킷을 받을때만 서버로 워프 업데이트 요청 했는데 
			// 스폰 시 ACCEPTED 이면 업데이트 요청을 안해서 진행 불가 상태가 생겼다.

			// 해결을 위해 ACCEPTED 무조건 업데이트 요청을 하도록 처리
			// 사이드 이펙트로 워프를 두번요청하는 경우가 생겨
			// 두번째 요청은 error로 내려오게 된다.
			// 첫번쨰 요청으로 퀘스트는 정상진행이 되는 상황
			// 두번째 요청에 대한 error는 표기하지 않기로 한다.

			return;
		}
		break;
	}

	if (!findText.IsEmpty())
	{
		FGsUIHelper::TrayMessageTicker(findText);
	}
	else
	{
		FGsUIHelper::PopupNetError(InResult);
	}
}

void FGsNetMessageHandlerQuest::GuildQuestErrorMsg(PD::Result inResult)
{
	switch (inResult)
	{
	case PD::Result::QUEST_ERROR_GUILD_EXCEED_ACCEPT_COUNT:
	case PD::Result::QUEST_ERROR_GUILD_EXCEED_COMPLETE_COUNT:
	{
		// TEXT: 퀘스트 수행 가능 횟수가 부족합니다.
		FText textMsg;
		if (FText::FindText(TEXT("GuildText"), TEXT("Ticker_NotEnough_QuestCount"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			return;
		}
	}
	break;
	case PD::Result::QUEST_ERROR_NO_REFRESHABLE_GUILD_QUEST:
	{
		// TEXT: 새로고침 할 수 있는 퀘스트가 없습니다.
		FText textMsg;
		if (FText::FindText(TEXT("GuildText"), TEXT("GuildQuest_Ticker_CannotRefresh"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			return;
		}
	}
	break;
	case PD::Result::QUEST_ERROR_GUILD_NOT_ACCEPTABLE_TODAY:
	{
		// TEXT: 다음 초기화 시점부터, 기사단 퀘스트를 수행하실 수 있습니다.
		FText textMsg;		
		if (FText::FindText(TEXT("GuildText"), TEXT("UI_GuildQuest_CanNotDoQuest"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			return;
		}
	}
	break;
	case PD::Result::QUEST_ERROR_GUILD_REFRESH:
	{
		// TEXT: 새로고침에 실패하였습니다
		FText textMsg;
		if (FText::FindText(TEXT("GuildText"), TEXT("UI_GuildQuest_RefreshFail"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			return;
		}
	}
	break;
	case PD::Result::QUEST_ERROR_EXPIRED_GUILD_QUEST:
	{
		// TEXT: 만료된 퀘스트입니다
		FText textMsg;
		if (FText::FindText(TEXT("GuildText"), TEXT("UI_GuildQuest_ExpiredQuest"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			return;
		}
	}
	break;
	case PD::Result::QUEST_ERROR_GUILD_NOT_ACCEPTABLE_QUEST:
	{
		// TEXT: 수행할 수 있는 기사단 퀘스트가 아닙니다
		FText textMsg;
		if (FText::FindText(TEXT("GuildText"), TEXT("UI_GuildQuest_Ticker_CantAcceptQuest"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			return;
		}
	}
	break;
	case PD::Result::QUEST_ERROR_INVALID_REWARD_GUILD_QUEST_ID:
	case PD::Result::QUEST_ERROR_NOT_ACCEPTED_GUILD_QUEST:
	case PD::Result::QUEST_ERROR_INVALID_GUILD_QUEST:
	{
		FGsUIHelper::TraySectionMessageTickerNetError(inResult);
		return;
	}
	break;
	}

	// 그 외의 경우
	QuestRecvErrorMsg(inResult);
}
