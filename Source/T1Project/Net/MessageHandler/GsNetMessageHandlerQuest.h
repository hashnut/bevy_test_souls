// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GsNetMessageHandleInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsNetManager;
/**
 * 퀘스트 네트워크 메세지 핸들러
 */
class T1PROJECT_API FGsNetMessageHandlerQuest : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	CreatureId creatureId;

public:
	FGsNetMessageHandlerQuest() = default;
	virtual ~FGsNetMessageHandlerQuest() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

	// Ack
public:
	// Quest	
	void UserQuestAck(const FGsNet::Packet& inPacket);				// 월드입장 시 진행 중인 모든 퀘스트 리스트를 받는다.

	void QuestMainAcceptAck(const FGsNet::Packet& inPacket);		// 클라가 수락 요청을 보내면 서버에서 수락 응답 받는다.
	void QuestSubAcceptAck(const FGsNet::Packet& inPacket);			// 클라가 수락 요청을 보내면 서버에서 수락 응답 받는다.
	void QuestRepeatAcceptAck(const FGsNet::Packet& inPacket);		// 클라가 수락 요청을 보내면 서버에서 수락 응답 받는다.
	void QuestRankAcceptAck(const FGsNet::Packet& inPacket);		// 클라가 수락 요청을 보내면 서버에서 수락 응답 받는다.	
	void QuestGuideAcceptAck(const FGsNet::Packet& inPacket);		// 클라가 수락 요청을 보내면 서버에서 수락 응답 받는다.

	void QuestMainCompleteRewardAck(const FGsNet::Packet& inPacket);	// 퀘스트 메인 보상 패킷을 받는다.
	void QuestSubCompleteRewardAck(const FGsNet::Packet& inPacket);		// 퀘스트 서브 보상 패킷을 받는다.
	void QuestRepeatCompleteRewardAck(const FGsNet::Packet& inPacket);	// 퀘스트 반복 보상 패킷을 받는다.	
	void QuestGuideCompleteRewardAck(const FGsNet::Packet& inPacket);	// 길드퀘스트 보상을 받는다.

	void QuestSubGiveUpAck(const FGsNet::Packet& inPacket);			// 퀘스트 서브 스토리를 포기한다.
	void QuestRepeatGiveUpAck(const FGsNet::Packet& inPacket);		// 퀘스트 반복 스토리를 포기한다.

	// Notify
protected:	
	void QuestUpDateNotify(const FGsNet::Packet& inPacket);			// 퀘스트 업데이트 패킷을 받는다.	

public:
	void QuestBoardListAck(const FGsNet::Packet& inPacket);
	void QuestBoardList(const FGsNet::Packet& inPacket);
	void QuestBoardRefreshAck(const FGsNet::Packet& inPacket);
	void QuestBoardAcceptAck(const FGsNet::Packet& inPacket);

	// Guild Quest (가이드랑 스펠링이 비슷하여 헷갈려서 따로 분리)
public:
	void QuestGuildAcceptAck(const FGsNet::Packet& inPacket);
	void QuestGuildCompleteRewardAck(const FGsNet::Packet& inPacket);
	void QuestGuildGiveUpAck(const FGsNet::Packet& inPacket);
	void QuestGuildRefreshAck(const FGsNet::Packet& inPacket);

public:
	void QuestRepeatRewardBoxAck(const FGsNet::Packet& inPacket);		// 반복 퀘스트 보상목록 단일로 받을 때
	void QuestRepeatRewardBoxListAck(const FGsNet::Packet& inPacket);	// 모든 반복 퀘스트 보상목록 받을 때

protected:
	void QuestRecvErrorMsg(PD::Result inResult);
	void QuestErrorMsg(PD::Result InResult);							// 퀘스트 에러
	void GuildQuestErrorMsg(PD::Result inResult);
};
