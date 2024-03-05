#pragma once
#include "CoreMinimal.h"
#include "Net/MessageHandler/GsNetMessageHandleInterface.h"

// 시공의 틈새 패킷 수신부
class FGsNetManager;

class T1PROJECT_API FGsNetMessageHandlerSpaceCrack : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerSpaceCrack() = default;
	virtual ~FGsNetMessageHandlerSpaceCrack() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

	// ack	
public:	
	// 시공의 틈새  생성 포탈 알림 
	void SpaceCrackSpawnAlarm(const FGsNet::Packet& inPacket);
	// 시공의 틈새 삭제 포탈 알림
	void SpaceCrackDespawnAlarm(const FGsNet::Packet& inPacket);

	void SpaceCrackActivationList(const FGsNet::Packet& inPacket);

	void SpaceCrackActivationRefresh(const FGsNet::Packet& inPacket);

	//카운트 다운
protected:	
	// 특정 UI OPEN 해당하는 UI 실행 
	void OpenUI(const FGsNet::Packet& inPacket);
	// 타이머 시작
	void TimerStart(const FGsNet::Packet& inPacket);
	// 카운트 다운 시작 
	void CountDownStart(const FGsNet::Packet& inPacket);
};