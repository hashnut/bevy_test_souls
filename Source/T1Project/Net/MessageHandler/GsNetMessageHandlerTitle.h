#pragma once

#include "CoreMinimal.h"
#include "GsNetMessageHandleInterface.h"

class FGsNetManager;

/**
 *
 */
class T1PROJECT_API FGsNetMessageHandlerTitle : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerTitle() = default;
	virtual ~FGsNetMessageHandlerTitle() = default;

public:
	virtual void InitializeMessage(class FGsNetManager* InManager) override;
	virtual void FinalizeMessage() override;

	/************************************************************************/
	/* notify                                                               */
	/************************************************************************/
	// 2022/01/21 PKT - 칭호 정보
	void Notify_TITLE_LIST(const FGsNet::Packet& InPacket);

	// 2022/01/21 PKT - 타 유저의 칭호 장착정보
	void Notify_USER_TITLE_INFO(const FGsNet::Packet& InPacket);

	// 2022/08/26 PKT - 침공 관련 배틀 등급
	void Notify_BattleLevel(const FGsNet::Packet& InPacket);

	// 2022/08/26 PKT - 타 유저의 침공 관련 배틀 등급
	void Notify_User_BattleLevel(const FGsNet::Packet& InPacket);

	/************************************************************************/
	/* Ack                                                                  */
	/************************************************************************/
	// 2022/01/21 PKT - 칭호 정보 업데이트
	void Ack_TITLE_UPDATED(const FGsNet::Packet& InPacket);

	// 2022/01/21 PKT - 칭호 장착 응답
	void Ack_TITLE_EQUIP(const FGsNet::Packet& InPacket);
};