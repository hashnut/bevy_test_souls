#pragma once

#include "CoreMinimal.h"
#include "GsNetMessageHandleInterface.h"
#include "../Message/GsMessageNet.h"

class FGsNetManager;

/**
 *
 */
class T1PROJECT_API FGsNetMessageHandlerMonsterKnowledge : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerMonsterKnowledge() = default;
	virtual ~FGsNetMessageHandlerMonsterKnowledge() = default;

public:
	virtual void InitializeMessage(class FGsNetManager* InManager) override;
	virtual void FinalizeMessage() override;

	/************************************************************************/
	/* notify                                                               */
	/************************************************************************/
	// 2021/12/01 PKT - InGame 접속시 내려 줌.
	void Notify_MonsterBokkDataSet(const FGsNet::Packet& InPacket);

	// 2021/12/01 PKT - 사냥 시 실시간으로 Window에 몬스터 지식 갱신을 위해 정보를 받는다.
	void Notify_USER_MONSTER_KNOWLEDGE_UPDATE(const FGsNet::Packet& InPacket);

	// 2021/12/01 PKT - 해금된 지식의 석판 아이디 리스트(최초 월드 접속 시)
	void NOTIFY_ACTIVATED_MONSTER_KNOWLEDGE_SLATE_LIST(const FGsNet::Packet& InPacket);
	

	/************************************************************************/
	/* Ack                                                                  */
	/************************************************************************/
	// 2021/12/01 PKT - 단일 몬스터 레벨업에 대한 응답
	void ACK_MONSTER_KNOWLEDGE_LELVE_UP(const FGsNet::Packet& InPacket);

	// 2021/12/01 PKT - 다수의 몬스터 레벨업에 요청에 대한 응답
	void ACK_MONSTER_KNOWLEDGE_LEVEL_ACHIEVEMENT_ALL(const FGsNet::Packet& InPacket);

	// 2021/12/01 PKT - 다수의 몬스터 Item 사용으로 인한 정보 갱신에 대한 응답
	void ACK_MONSTER_KNOWLEDGE_EXP_GAIN_ITEM_USE_ALL(const FGsNet::Packet& InPacket);

	// 2021/12/01 PKT - 지식의 석판 정보 응답 패킷
	void ACK_MONSTER_KNOWLEDGE_SLATE(const FGsNet::Packet& InPacket);

	// 2021/12/01 PKT - 지식의 석판 해금 응답(슬롯 활성화) 패킷
	void SC_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT(const FGsNet::Packet& InPacket);

	void SC_ACK_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT_ALL(const FGsNet::Packet& InPacket);

	// 2021/12/01 PKT - 지식 슬롯 활성화 응답 패킷
	void ACK_ACTIVATE_MONSTER_KNOWLEDGE_SLATE_SLOT(const FGsNet::Packet& InPacket);

	// 2021/12/01 PKT - 전체 스탯 초기화 응답
	void ACK_MONSTER_KNOWLEDGE_SLATE_RESET_ITEM_USE(const FGsNet::Packet& InPacket);


#pragma region Codex

	/************************************************************************/
	/* Notify                                                               */
	/************************************************************************/

	/** 월드 접속 시, 몬스터 일람 정보 모두 전송 */
	void NOTIFY_CODEX(const FGsNet::Packet& InPacket);

	/** 월드 접속 시, 몬스터 일람 정보 모두 전송 */
	void NOTIFY_CODEX_MAP_GROUP_EXP_UPDATE(const FGsNet::Packet& InPacket);


	/************************************************************************/
	/* Ack                                                                  */
	/************************************************************************/

	/** 요청 카테고리의 몬스터 일람 정보 및 지식/도감 포인트 정보 응답 */
	void ACK_CODEX(const FGsNet::Packet& InPacket);

	/** 몬스터 지식을 도감 포인트로 변환 응답 */
	void ACK_MONSTER_BOOK_POINT_EXCHANGE(const FGsNet::Packet& InPacket);

	/** 몬스터 일람 대륙 탐사 레벨업 응답 */
	void ACK_CODEX_CATEGORY_LEVEL_UP(const FGsNet::Packet& InPacket);

	/** 석판 > 지식 탐구 특수 노드 개방 응답 */
	void ACK_ACTIVATE_SLATE_SPECIAL_NODE(const FGsNet::Packet& InPacket);

	/** 몬스터 일람에서 지식의 결정 아이템 사용 */
	void ACK_MONSTER_KNOWLEDGE_EXP_GAIN_ITEM_USE(const FGsNet::Packet& InPacket);

#pragma endregion

};