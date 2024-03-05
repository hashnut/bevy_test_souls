
#include "GsNetMessageHandlerMonsterKnowledge.h"

#include "Item/GsItemManager.h"
#include "Message/GsMessageUserInfo.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGame/GsMonsterCollectionManager.h"
#include "Management/ScopeGame/GsCodexManager.h"
#include "MonsterKnowledgeCollection/GsMonsterKnowledgeData.h"
#include "MonsterKnowledgeCollection/MonsterKnowledgeCollectionTypeDefine.h"
#include "Currency/GsCurrencyHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "T1Project/T1Project.h"
#include "Net/GSNet.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "UI/UIContent/Helper/GsUIHelper.h"


using namespace PD::SC;

void FGsNetMessageHandlerMonsterKnowledge::InitializeMessage(FGsNetManager* InManager)
{
	// 패킷 바인딩
	MProtocalWorld& WorldProtocol = InManager->GetProtocalWorld();

	InitializeMessageDelegateList(&WorldProtocol);

	/************************************************************************/
	/* notify                                                               */
	/************************************************************************/
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_MONSTER_KNOWLEDGE
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::Notify_MonsterBokkDataSet));
	// 2021/12/01 PKT - 사냥 시 실시간으로 Window에 몬스터 지식 갱신을 위해 정보를 받는다.
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_USER_MONSTER_KNOWLEDGE_UPDATE
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::Notify_USER_MONSTER_KNOWLEDGE_UPDATE));

	// 2021/12/01 PKT - 해금된 지식의 석판 아이디 리스트(최초 월드 접속 시)
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACTIVATED_MONSTER_KNOWLEDGE_SLATE_LIST
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::NOTIFY_ACTIVATED_MONSTER_KNOWLEDGE_SLATE_LIST));
	
	
	/************************************************************************/
	/* Ack                                                                  */
	/************************************************************************/
	// 2021/12/01 PKT - 단일 몬스터 레벨업에 대한 응답
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_MONSTER_KNOWLEDGE_LEVEL_UP
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::ACK_MONSTER_KNOWLEDGE_LELVE_UP));
	
	// 2021/12/01 PKT - 단일 몬스터 Item 사용으로 인한 정보 갱신에 대한 응답
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_MONSTER_KNOWLEDGE_EXP_GAIN_ITEM_USE
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::ACK_MONSTER_KNOWLEDGE_EXP_GAIN_ITEM_USE));
	
	// 2021/12/01 PKT - 다수의 몬스터 레벨업에 요청에 대한 응답
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_MONSTER_KNOWLEDGE_LEVEL_ACHIEVEMENT_ALL
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::ACK_MONSTER_KNOWLEDGE_LEVEL_ACHIEVEMENT_ALL));
	
	// 2021/12/01 PKT - 다수의 몬스터 Item 사용으로 인한 정보 갱신에 대한 응답
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_MONSTER_KNOWLEDGE_EXP_GAIN_ITEM_USE_ALL
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::ACK_MONSTER_KNOWLEDGE_EXP_GAIN_ITEM_USE_ALL));


	// 2021/12/01 PKT - 식의 석판 정보 응답 패킷
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_MONSTER_KNOWLEDGE_SLATE
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::ACK_MONSTER_KNOWLEDGE_SLATE));

	// 2021/12/01 PKT - 지식의 석판 해금 응답(슬롯 활성화) 패킷
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_UNLOCK_MONSTER_KNOWLEDGE_SLATE
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::SC_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT));
	
	// 2021/12/01 PKT - 지식 슬롯 활성화 응답 패킷
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::ACK_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT_ALL
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::SC_ACK_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT_ALL));

	
	
	// 2021/12/01 PKT - 전체 스탯 초기화 응답
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_MONSTER_KNOWLEDGE_SLATE_RESET_ITEM_USE
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::ACK_MONSTER_KNOWLEDGE_SLATE_RESET_ITEM_USE));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_ACTIVATE_SLATE_SPECIAL_NODE
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::ACK_ACTIVATE_SLATE_SPECIAL_NODE));

	// 2023/11/09 ejrrb10 - 도감 리뉴얼 관련 패킷 추가
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_CODEX
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::NOTIFY_CODEX));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_CODEX_MAP_GROUP_EXP_UPDATE
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::NOTIFY_CODEX_MAP_GROUP_EXP_UPDATE));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_CODEX
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::ACK_CODEX));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_MONSTER_BOOK_POINT_EXCHANGE
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::ACK_MONSTER_BOOK_POINT_EXCHANGE));

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_CODEX_CATEGORY_LEVEL_UP
		, this
		, &FGsNetMessageHandlerMonsterKnowledge::ACK_CODEX_CATEGORY_LEVEL_UP));
}


void FGsNetMessageHandlerMonsterKnowledge::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerMonsterKnowledge::Notify_MonsterBokkDataSet(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_MONSTER_KNOWLEDGE_READ* Packet
		= reinterpret_cast<PD::SC::PKT_SC_MONSTER_KNOWLEDGE_READ*>(InPacket.Buffer);

	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}
	
	int32 length = Packet->MonsterKnowledgeListSize();
	const uchar* bookSetBuf = Packet->MonsterKnowledgeList();

	TArray<const MonsterKnowledge*> dataSet;
	if (length)
	{
		int32 dataSize = sizeof(MonsterKnowledge);
		int32 maxCount = length / dataSize;

		for (int32 idx = 0; idx < maxCount; ++idx)
		{
			const MonsterKnowledge* data = reinterpret_cast<const MonsterKnowledge*>((bookSetBuf + (idx * dataSize)));
			if (data)
			{
				dataSet.Emplace(data);
			}
		}
	}

	GsMonsterCollection()->Notify_BookDataSet(dataSet, Packet->Completed());
}

// 2021/12/01 PKT - 사냥 시 실시간으로 Window에 몬스터 지식 갱신을 위해 정보를 받는다.
void FGsNetMessageHandlerMonsterKnowledge::Notify_USER_MONSTER_KNOWLEDGE_UPDATE(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_USER_MONSTER_KNOWLEDGE_UPDATE_READ* Packet
		= reinterpret_cast<PD::SC::PKT_SC_USER_MONSTER_KNOWLEDGE_UPDATE_READ*>(InPacket.Buffer);

	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	if (GsMonsterCollection())
	{
		GsMonsterCollection()->Notify_BookData(Packet->MonsterKnowledgeId(), Packet->FinalMonsterKnowledgeExp());
	}

	/**
	 * HUD에 지식 이펙트 On...
	 */
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr != msgMgr)
	{
		msgMgr->GetUserBasicInfo().SendMessage(MessageUserInfo::INCREASE_MONSTER_BOOK_PT, Packet->IncreaseMonsterKnowledgeExp());
	}
}

// 2021/12/01 PKT - 단일 몬스터 레벨업에 대한 응답
void FGsNetMessageHandlerMonsterKnowledge::ACK_MONSTER_KNOWLEDGE_LELVE_UP(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_MONSTER_KNOWLEDGE_LEVEL_UP_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ACK_MONSTER_KNOWLEDGE_LEVEL_UP_READ*>(InPacket.Buffer);
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

	if (GsMonsterCollection())
	{
		FGsMonsterCollectionBook bookData;
		bookData.id = Packet->MonsterKnowledgeId();
		bookData.level = Packet->MonsterKnowledgeLevel();
		bookData.exp = Packet->MonsterKnowledgeExp();
		bookData.rewardPoint = Packet->DeltaMonsterBookPoint();

		GsMonsterCollection()->Ack_BookData(bookData);
	}
}

void FGsNetMessageHandlerMonsterKnowledge::ACK_MONSTER_KNOWLEDGE_EXP_GAIN_ITEM_USE(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_MONSTER_KNOWLEDGE_EXP_GAIN_ITEM_USE_READ* packet = reinterpret_cast<PD::SC::PKT_SC_ACK_MONSTER_KNOWLEDGE_EXP_GAIN_ITEM_USE_READ*>(InPacket.Buffer);
	if (nullptr == packet)
	{
		GSLOG(Error, TEXT("nullptr == packet"));
		return;
	}

	const PD::Result result = StaticCast<PD::Result>(packet->Result());
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(packet->Result()));
		return;
	}

	CodexMapGroupId mapGroupId = packet->CodexMapGroupId();
	CodexMapGroupLevel mapGroupLevel = packet->CodexMapGroupLevel();
	CodexMapGroupExp mapGroupExp = packet->CodexMapGroupExp();

	GsCodex()->AckMonsterKnowledgeExpGainItemUse(mapGroupId, mapGroupLevel, mapGroupExp);
}

// 2021/12/01 PKT - 다수의 몬스터 레벨업에 요청에 대한 응답
void FGsNetMessageHandlerMonsterKnowledge::ACK_MONSTER_KNOWLEDGE_LEVEL_ACHIEVEMENT_ALL(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_MONSTER_KNOWLEDGE_LEVEL_ACHIEVEMENT_ALL_READ* Packet
		= reinterpret_cast<PD::SC::PKT_SC_ACK_MONSTER_KNOWLEDGE_LEVEL_ACHIEVEMENT_ALL_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	const PD::Result Result = StaticCast<PD::Result>(Packet->Result());
	if (Result != PACKET_RESULT_SUCCESS)
	{
		if (PD::Result::MONSTER_KNOWLEDGE_ERROR_NO_KNOWLEDGE_LEVEL_UP_AVAILABLE == Result)
		{
			FText errorMsg;
			FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("ticker_NoMonsterToLevelUp"), errorMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorMsg);
			return;
		}

		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(Packet->Result()));
		return;
	}

	if (GsMonsterCollection())
	{
		TArray<FGsMonsterCollectionBook> bookDataSet;
		for (auto iter = Packet->GetFirstMonsterKnowledgeListIterator(); iter != Packet->GetLastMonsterKnowledgeListIterator(); ++iter)
		{
			FGsMonsterCollectionBook bookData;
			bookData.id = iter->MonsterKnowledgeId();
			bookData.level = iter->MonsterKnowledgeLevel();
			bookData.exp = iter->MonsterKnowledgeExp();
			bookData.rewardPoint = iter->DeltaMonsterBookPoint();

			bookDataSet.Emplace(bookData);
		}

		GsMonsterCollection()->Ack_BookDataSet(bookDataSet);
	}
}

// 2021/12/01 PKT - 다수의 몬스터 Item 사용으로 인한 정보 갱신에 대한 응답
void FGsNetMessageHandlerMonsterKnowledge::ACK_MONSTER_KNOWLEDGE_EXP_GAIN_ITEM_USE_ALL(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_MONSTER_KNOWLEDGE_EXP_GAIN_ITEM_USE_ALL_READ* Packet
		= reinterpret_cast<PD::SC::PKT_SC_ACK_MONSTER_KNOWLEDGE_EXP_GAIN_ITEM_USE_ALL_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	const PD::Result Result = StaticCast<PD::Result>(Packet->Result());
	if (Result != PACKET_RESULT_SUCCESS)
	{
		if (PD::Result::MONSTER_KNOWLEDGE_ERROR_ALREADY_KNOWLEDGE_MAX_EXP == Result)
		{
			FText errorMsg;
			FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("ticker_AllMonsterKnowledgeMax"), errorMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorMsg);
			return;
		}
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(Packet->Result()));
		return;
	}

	if (GsMonsterCollection())
	{
		TArray<FGsMonsterCollectionBook> bookDataSet;
		for (auto iter = Packet->GetFirstMonsterKnowledgeListIterator(); iter != Packet->GetLastMonsterKnowledgeListIterator(); ++iter)
		{
			FGsMonsterCollectionBook bookData;
			bookData.id = iter->MonsterKnowledgeId();
			bookData.level = iter->MonsterKnowledgeLevel();
			bookData.exp = iter->MonsterKnowledgeExp();
			bookData.rewardPoint = iter->DeltaMonsterBookPoint();

			bookDataSet.Emplace(bookData);
		}

		GsMonsterCollection()->Ack_BookDataSet(bookDataSet);
	}
}

void FGsNetMessageHandlerMonsterKnowledge::ACK_CODEX(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_CODEX_READ* packet = reinterpret_cast<PD::SC::PKT_SC_ACK_CODEX_READ*>(InPacket.Buffer);
	if (nullptr == packet)
	{
		GSLOG(Error, TEXT("nullptr == packet"));
		return;
	}

	const PD::Result result = StaticCast<PD::Result>(packet->Result());
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(packet->Result()));
		return;
	}

	CodexCategoryId categoryId = packet->CodexCategoryId();
	CodexCategoryLevel categoryLevel = packet->CodexCategoryLevel();

	TArray<TTuple<int32, int32, int32>> mapGroupIdLevelExpTuple;
	for (auto iter = packet->GetFirstCodexMapGroupListIterator(); iter != packet->GetLastCodexMapGroupListIterator(); ++iter)
	{
		mapGroupIdLevelExpTuple.Emplace(MakeTuple(iter->CodexMapGroupId(), iter->CodexMapGroupLevel(), iter->CodexMapGroupExp()));
	}

	MonsterKnowledgeExp accumulateMonsterKnowledgeExp = packet->AccumulateMonsterKnowledgeExp();
	MonsterKnowledgeExp finalMonsterKnowledgeExp = packet->FinalMonsterKnowledgeExp();
	Currency finalMonsterBookPoint = packet->FinalMonsterBookPoint();

	GsCodex()->AckCodex(categoryId, categoryLevel, MoveTemp(mapGroupIdLevelExpTuple), accumulateMonsterKnowledgeExp, finalMonsterKnowledgeExp, finalMonsterBookPoint);
}

void FGsNetMessageHandlerMonsterKnowledge::ACK_MONSTER_BOOK_POINT_EXCHANGE(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_MONSTER_BOOK_POINT_EXCHANGE_READ* packet = reinterpret_cast<PD::SC::PKT_SC_ACK_MONSTER_BOOK_POINT_EXCHANGE_READ*>(InPacket.Buffer);
	if (nullptr == packet)
	{
		GSLOG(Error, TEXT("nullptr == packet"));
		return;
	}

	Currency fianlMonsterBookPoint = packet->FinalMonsterBookPoint();
	MonsterKnowledgeExp finalMonsterKnowledgeExp = packet->FinalMonsterKnowledgeExp();

	GsCodex()->AckMonsterBookPointExchange(fianlMonsterBookPoint, finalMonsterKnowledgeExp);
}

void FGsNetMessageHandlerMonsterKnowledge::ACK_CODEX_CATEGORY_LEVEL_UP(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_CODEX_CATEGORY_LEVEL_UP_READ* packet = reinterpret_cast<PD::SC::PKT_SC_ACK_CODEX_CATEGORY_LEVEL_UP_READ*>(InPacket.Buffer);
	if (nullptr == packet)
	{
		GSLOG(Error, TEXT("nullptr == packet"));
		return;
	}

	const PD::Result result = StaticCast<PD::Result>(packet->Result());
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(packet->Result()));
		return;
	}

	CodexCategoryId categoryId = packet->CodexCategoryId();
	CodexCategoryLevel categoryLevel = packet->CodexCategoryLevel();

	GsCodex()->AckCodexCategoryLevelUp(categoryId, categoryLevel);
}

void FGsNetMessageHandlerMonsterKnowledge::ACK_ACTIVATE_SLATE_SPECIAL_NODE(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_ACTIVATE_SLATE_SPECIAL_NODE_READ* packet = reinterpret_cast<PD::SC::PKT_SC_ACK_ACTIVATE_SLATE_SPECIAL_NODE_READ*>(InPacket.Buffer);
	if (nullptr == packet)
	{
		GSLOG(Error, TEXT("nullptr == packet"));
		return;
	}

	const PD::Result result = StaticCast<PD::Result>(packet->Result());
	if (result != PACKET_RESULT_SUCCESS && result != PD::Result::SLATE_PROBABILITY_FAIL_TO_ACTIVATE_SPECIAL_NODE)
	{
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(packet->Result()));
		return;
	}

	MonsterKnowledgeSlateId slateId = packet->MonsterKnowledgeSlateId();
	SlateSpecialNodeId specialNodeId = packet->SlateSpecialNodeId();

	GsMonsterCollection()->AckActivateSlateSpecialNode(result == PACKET_RESULT_SUCCESS, slateId, specialNodeId);
}

// 2021/12/01 PKT - 해금된 지식의 석판 아이디 리스트(최초 월드 접속 시)
void FGsNetMessageHandlerMonsterKnowledge::NOTIFY_ACTIVATED_MONSTER_KNOWLEDGE_SLATE_LIST(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACTIVATED_MONSTER_KNOWLEDGE_SLATE_LIST_READ* Packet
		= reinterpret_cast<PD::SC::PKT_SC_ACTIVATED_MONSTER_KNOWLEDGE_SLATE_LIST_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	TArray<MonsterKnowledgeSlateId> slateList;
	TArray<MonsterKnowledgeSlateSlotId> slateSlotList;
	TArray<SlateSpecialNodeId> specialNodeList;
	
	const int32 maxCount = Packet->GetActivatedSlateListCount();
	for (auto slate = Packet->GetFirstActivatedSlateListIterator(); slate != Packet->GetLastActivatedSlateListIterator(); ++slate)
	{
		slateList.Emplace(slate->SlateId());

		for (auto slateSlot = slate->GetFirstSlotListIterator(); slateSlot != slate->GetLastSlotListIterator(); ++slateSlot)
		{
			slateSlotList.Emplace(slateSlot->SlotId());			
		}

		for (auto specialNode = slate->GetFirstSpecialNodeListIterator(); specialNode != slate->GetLastSpecialNodeListIterator(); ++specialNode)
		{
			specialNodeList.Emplace(specialNode->SpecialNodeId());
		}
	}

	GsMonsterCollection()->Notify_ActiveSlate(slateList, slateSlotList, specialNodeList);
}

void FGsNetMessageHandlerMonsterKnowledge::NOTIFY_CODEX(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_CODEX_READ* packet = reinterpret_cast<PD::SC::PKT_SC_CODEX_READ*>(InPacket.Buffer);
	if (nullptr == packet)
	{
		GSLOG(Error, TEXT("nullptr == packet"));
		return;
	}

	for (auto iter = packet->GetFirstCodexCategoryListIterator(); iter != packet->GetLastCodexCategoryListIterator(); ++iter)
	{
		CodexCategoryId categoryId = iter->CodexCategoryId();
		CodexCategoryLevel categoryLevel = iter->CodexCategoryLevel();

		TArray<TTuple<int32, int32, int32>> mapGroupIdLevelExpTuple;
		for (auto mapGroupListIter = iter->GetFirstCodexMapGroupListIterator(); mapGroupListIter != iter->GetLastCodexMapGroupListIterator(); ++mapGroupListIter)
		{
			mapGroupIdLevelExpTuple.Emplace(MakeTuple(mapGroupListIter->CodexMapGroupId(), mapGroupListIter->CodexMapGroupLevel(), mapGroupListIter->CodexMapGroupExp()));
		}

		GsCodex()->NotifyCodex(categoryId, categoryLevel, MoveTemp(mapGroupIdLevelExpTuple));
	}
}

void FGsNetMessageHandlerMonsterKnowledge::NOTIFY_CODEX_MAP_GROUP_EXP_UPDATE(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_CODEX_MAP_GROUP_EXP_UPDATE_READ* packet = reinterpret_cast<PD::SC::PKT_SC_CODEX_MAP_GROUP_EXP_UPDATE_READ*>(InPacket.Buffer);
	if (nullptr == packet)
	{
		GSLOG(Error, TEXT("nullptr == packet"));
		return;
	}

	GsCodex()->NotifyCodexMapGroupExpUpdate(packet->MonsterKnowledgeExp(), packet->CodexMapGroupId(), packet->CodexMapGroupLevel(), packet->CodexMapGroupExp());
}


// 2021/12/01 PKT - 지식의 석판 정보 응답 패킷
void FGsNetMessageHandlerMonsterKnowledge::ACK_MONSTER_KNOWLEDGE_SLATE(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_MONSTER_KNOWLEDGE_SLATE_READ* Packet
		= reinterpret_cast<PD::SC::PKT_SC_ACK_MONSTER_KNOWLEDGE_SLATE_READ*>(InPacket.Buffer);
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

	TArray<MonsterKnowledgeSlateSlotId> slateSlotList;
	TArray<SlateSpecialNodeId> specialNodeList;

	Packet->MakeMonsterKnowledgeSlateSlotIdListVector<TArray<MonsterKnowledgeSlateSlotId>, MonsterKnowledgeSlateSlotId>(slateSlotList);
	Packet->MakeSlateSpecialNodeIdListVector<TArray<SlateSpecialNodeId>, SlateSpecialNodeId>(specialNodeList);

	// ejrrb10 | 리뉴얼 전에 여기가 주석처리 되어 있었는데... 사이드 이펙트는 없는지 한번 확인은 해 봐야 함
	GsMonsterCollection()->Ack_SlateNodeSet(Packet->MonsterKnowledgeSlateId(), slateSlotList, specialNodeList);
}

// 2021/12/01 PKT - 지식의 석판 해금 응답(슬롯 활성화) 패킷
void FGsNetMessageHandlerMonsterKnowledge::SC_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_UNLOCK_MONSTER_KNOWLEDGE_SLATE_READ* Packet
		= reinterpret_cast<PD::SC::PKT_SC_UNLOCK_MONSTER_KNOWLEDGE_SLATE_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}
	
	GsMonsterCollection()->Ack_OpenSlate(Packet->MonsterKnowledgeSlateId());
}

// 2021/12/01 PKT - 지식 슬롯 활성화 응답 패킷
void FGsNetMessageHandlerMonsterKnowledge::ACK_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT_READ* Packet
		= reinterpret_cast<PD::SC::PKT_SC_ACK_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT_READ*>(InPacket.Buffer);
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

	MonsterKnowledgeSlateId slateId = Packet->MonsterKnowledgeSlateId();
	TArray<MonsterKnowledgeSlateSlotId> list;
	Packet->MakeMonsterKnowledgeSlateSlotIdListVector<TArray<MonsterKnowledgeSlateSlotId>, MonsterKnowledgeSlateSlotId>(list);
	
	GsMonsterCollection()->Ack_SlateNodeSet(slateId, list);
}

void FGsNetMessageHandlerMonsterKnowledge::SC_ACK_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT_ALL(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT_ALL_READ* Packet
		= reinterpret_cast<PD::SC::PKT_SC_ACK_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT_ALL_READ*>(InPacket.Buffer);
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

	MonsterKnowledgeSlateId slateId = Packet->MonsterKnowledgeSlateId();
	TArray<MonsterKnowledgeSlateSlotId> list;
	Packet->MakeMonsterKnowledgeSlateSlotIdListVector<TArray<MonsterKnowledgeSlateSlotId>, MonsterKnowledgeSlateSlotId>(list);

	GsMonsterCollection()->Ack_SlateNodeSet(slateId, list);
}

// 2021/12/01 PKT - 전체 스탯 초기화 응답
void FGsNetMessageHandlerMonsterKnowledge::ACK_MONSTER_KNOWLEDGE_SLATE_RESET_ITEM_USE(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_MONSTER_KNOWLEDGE_SLATE_RESET_ITEM_USE_READ* Packet
		= reinterpret_cast<PD::SC::PKT_SC_ACK_MONSTER_KNOWLEDGE_SLATE_RESET_ITEM_USE_READ*>(InPacket.Buffer);
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
	GsMonsterCollection()->Ack_SlatActiveNodeReset();
}