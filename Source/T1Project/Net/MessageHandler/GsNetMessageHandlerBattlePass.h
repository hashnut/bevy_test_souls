#pragma once
#include "CoreMinimal.h"
#include "Net/GSNet.h"
#include "Net/MessageHandler/GsNetMessageHandleInterface.h"

// 커뮤니티 패킷 수신부
class FGsNetManager;

class T1PROJECT_API FGsNetMessageHandlerBattlePass : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerBattlePass() = default;
	virtual ~FGsNetMessageHandlerBattlePass() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

protected:
	// 처음 초기 데이터
	void OnNotifyBattlePassList(const FGsNet::Packet& inPacket);
	// 새로 오픈된 배틀 패스
	void OnNotifyBattlePassExpose(const FGsNet::Packet& inPacket);
	// 배틀 패스 미션 진척도. 레벨 진척도 같이...
	void OnNotifyBattlePassMissionProgress(const FGsNet::Packet& inPacket);
	// 배틀 패스 레벨 변화
	void OnNotifyUpdateBattlePassLevel(const FGsNet::Packet& inPacket);
	// 배틀패스 추가보상 활성화
	void OnAckActivateBattlePassAdditionalReward(const FGsNet::Packet& inPacket);
	// 구입하여 배틀 패스 레벨 변화(이것만 쓸 수도)
	void OnAckBuyBattlePassLevel(const FGsNet::Packet& inPacket);
	// 특정 레벨의 배틀 패스 리워드 받기,additional 리워드 받기
	void OnAckReceiveBattlePassLevelReward(const FGsNet::Packet& inPacket);
	// 배틀패스 레벨 리워드 모두 받기 리스트 
	void OnAckBattlePassRewardAllList(const FGsNet::Packet& inPacket);
	// 배틀패스 레벨 리워드 모두 받기 ack
	void AckBattlePassReceiveLevelRewardAll(const FGsNet::Packet& inPacket);
};