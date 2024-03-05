#pragma once

#include "GsNetBase.h"
#include "GsNetBuffer.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"

/**
 *  Item Packet Send Class
 */
class FGsNetSendServiceContentsEvent final
{

	FGsNetSendServiceContentsEvent() = delete;

	template<typename TPacket, typename... TParams>
	static bool SendPacket(TParams... InParams)
	{
		const FGsNetManager* NetManager = GNet();
		if (nullptr == NetManager)
		{
			GSLOG(Error, TEXT("nullptr == NetManager"));
			return false;
		}

		TSharedPtr<FGsNetBase> NetBase = NetManager->GetActive();
		if (!NetBase.IsValid())
		{
			GSLOG(Error, TEXT("!NetBase.IsValid()"));
			return false;
		}

		FGsNetBuffer* NetBuffer = NetBase->GetLocalSendBufferPtr();
		const TPacket Builder(NetBuffer->GetBuffer(), NetBuffer->GetSize(), InParams...);

		NetBase->Send(Builder.mPktWriter.GetPktSize());
		return true;
	}

public:
	/**
	 * 
	 */
	static bool REQ_CONTENTS_EVENT_LIST();

};