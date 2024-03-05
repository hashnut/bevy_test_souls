#pragma once

#include "GsNetMessageHandleInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsNetManager;

class T1PROJECT_API FGsNetMessageHandlerMultiLevelRank : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerMultiLevelRank() = default;

protected:
	//void RemoveRequestRankQuest(StoryId InStory);

	/************************************************************************/
	/* instancing                                                           */
	/************************************************************************/

public:
	void InitializeMessage(FGsNetManager* InManager) override;
	void FinalizeMessage() override;


	/************************************************************************/
	/* notify                                                               */
	/************************************************************************/
protected:
	void Notify_MULTI_LEVEL_RANK_INFO_LIST(const FGsNet::Packet& InPacket);

	
	/************************************************************************/
	/* acknowledge                                                          */
	/************************************************************************/
protected:
	void Ack_MULTI_LEVEL_RANK_PROMOTION(const FGsNet::Packet& InPacket);		// 승급에 대한 응답
	void Ack_MULTI_LEVEL_RANK_CHANGE_RANK(const FGsNet::Packet& InPacket);		// 랭크 변경에 대한 응답
};
