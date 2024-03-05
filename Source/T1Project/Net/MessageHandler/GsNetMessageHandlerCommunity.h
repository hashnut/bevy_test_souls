#pragma once
#include "CoreMinimal.h"
#include "Net/GSNet.h"
#include "Net/MessageHandler/GsNetMessageHandleInterface.h"

// 커뮤니티 패킷 수신부
class FGsNetManager;

class T1PROJECT_API FGsNetMessageHandlerCommunity : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerCommunity() = default;
	virtual ~FGsNetMessageHandlerCommunity() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

protected:
	// 커뮤니티 이벤트 리스트 요청 응답
	void CommunityListAck(const FGsNet::Packet& inPacket);
	// 출석 체크 결과
	void ChackAttendanceAck(const FGsNet::Packet& inPacket);
	// 출석부 구매 요청 응답
	void PurchaseAttendanceAck(const FGsNet::Packet& inPacket);

	// 커뮤니티 이벤트 미션 목록
	void CommunityEventMission(const FGsNet::Packet& inPacket);
	// 커뮤니티 이벤트 미션 보상 요청 응답
	void CommunityEventMissionReward(const FGsNet::Packet& inPacket);
	// 커뮤니티 이벤트 미션 임무 진행도
	void CommunityEventMissionProgress(const FGsNet::Packet& inPacket);
};