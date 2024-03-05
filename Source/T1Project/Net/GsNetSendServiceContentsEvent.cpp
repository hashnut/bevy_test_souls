#include "GsNetSendServiceContentsEvent.h"

#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"

#include "T1Project.h"
#include "Management/ScopeGame/GsContentsEventManager.h"

// true 면 실제 서버와 연동, false 면 가상 서버로 실행
#define WITH_GSSERVER false

#if !WITH_GSSERVER
#include "Runtime/Core/Public/Containers/StringFwd.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#endif

bool FGsNetSendServiceContentsEvent::REQ_CONTENTS_EVENT_LIST()
{
	GSLOG(Log, TEXT("REQ_CONTENTS_EVENT_LIST(%d)"));

	/*
#if WITH_GSSERVER

	if (!FGsNetSendServiceContentsEvent::SendPacket<PD::CS::PKT_CS_REQ_CONTENTS_EVENT_LIST_WRITE>())
	{
		GSLOG(Error, TEXT("!SendPacket<PD::CS::PKT_CS_REQ_CONTENTS_EVENT_LIST_WRITE>()"));
		return false;
	}

#else

	UGsContentsEventManager* ContentsEventManager = GSContentsEvent();
	if (nullptr != ContentsEventManager && nullptr != ContentsEventManager->GetOuter() && nullptr != ContentsEventManager->GetOuter()->GetWorld())
	{
		ContentsEventManager->GetOuter()->GetWorld()->GetTimerManager().SetTimerForNextTick([InCraftTabId]()
			{
				TArray<CraftIdAmountPair> CraftStatList0;
				TArray<CraftIdAmountPair> CraftStatList1;
				FGsNetMessageHandlerCraft::ACK_CRAFT_STAT_LIST_Impl(InCraftTabId, CraftStatList0, CraftStatList1);
			});
	}

#endif
	//*/

	return true;
}

#undef WITH_GSSERVER
