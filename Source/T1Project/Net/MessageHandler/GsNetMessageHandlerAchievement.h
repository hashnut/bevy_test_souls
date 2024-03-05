#pragma once
#include "CoreMinimal.h"
#include "GsNetMessageHandleInterface.h"
#include "Classes/GsMessageHandlerOneParam.h"

class FGsNetManager;

class T1PROJECT_API FGsNetMessageHandlerAchievement : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerAchievement() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

protected:
	void NotifyAchievementList(const FGsNet::Packet& inPacket); 
	void NotifyUpdateAchievementProgress(const FGsNet::Packet& inPacket);
	void AckAchievementReward(const FGsNet::Packet& inPacket);

	void AckAchievementRewardList(const FGsNet::Packet& inPacket);
	void AckAchievementReceiveRewardAll(const FGsNet::Packet& inPacket);
};