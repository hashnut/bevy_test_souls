#include "GsNetMessageHandlerContentsEvent.h"

#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "Management/ScopeGame/GsContentsEventManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Message/GsMessageNet.h"
#include "Net/GSNet.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_Client_Auth.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "T1Project.h"
#include "UTIL/GsTimeSyncUtil.h"

void FGsNetMessageHandlerContentsEvent::InitializeMessage(FGsNetManager* InManager)
{
	// 패킷 바인딩
	MProtocalGateway& ProtocalGateway = InManager->GetProtocalGateway();

	InitializeMessageDelegateList(&ProtocalGateway);

	AddMessageDelegate(ProtocalGateway.AddRaw(PD::GC::GCPacketId::GC_CONTENTS_EVENT_GROUP_ACTIVATED_LIST, this, &FGsNetMessageHandlerContentsEvent::CONTENTS_EVENT_GROUP_ACTIVATED_LIST));
	AddMessageDelegate(ProtocalGateway.AddRaw(PD::GC::GCPacketId::GC_CONTENTS_EVENT_GROUP_ACTIVATE, this, &FGsNetMessageHandlerContentsEvent::CONTENTS_EVENT_GROUP_ACTIVATE));
	AddMessageDelegate(ProtocalGateway.AddRaw(PD::GC::GCPacketId::GC_CONTENTS_EVENT_GROUP_DEACTIVATE, this, &FGsNetMessageHandlerContentsEvent::CONTENTS_EVENT_GROUP_DEACTIVATE));
	AddMessageDelegate(ProtocalGateway.AddRaw(PD::GC::GCPacketId::GC_CONTENTS_EVENT_ACTIVATED_LIST, this, &FGsNetMessageHandlerContentsEvent::CONTENTS_EVENT_ACTIVATED_LIST));
	AddMessageDelegate(ProtocalGateway.AddRaw(PD::GC::GCPacketId::GC_CONTENTS_EVENT_ACTIVATE, this, &FGsNetMessageHandlerContentsEvent::CONTENTS_EVENT_ACTIVATE));
	AddMessageDelegate(ProtocalGateway.AddRaw(PD::GC::GCPacketId::GC_CONTENTS_EVENT_DEACTIVATE, this, &FGsNetMessageHandlerContentsEvent::CONTENTS_EVENT_DEACTIVATE));
}

void FGsNetMessageHandlerContentsEvent::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerContentsEvent::CONTENTS_EVENT_GROUP_ACTIVATED_LIST(const FGsNet::Packet& InPacket)
{
	const PD::GC::PKT_GC_CONTENTS_EVENT_GROUP_ACTIVATED_LIST_READ* Packet = reinterpret_cast<PD::GC::PKT_GC_CONTENTS_EVENT_GROUP_ACTIVATED_LIST_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	GSLOG(Log, TEXT("CONTENTS_EVENT_GROUP_ACTIVATED_LIST()"));
	TMap<ContentsEventGroupId, FGsNetMessageHandlerContentsEvent::FGsContentsEventGroupData> ContentsEventGroupDataMap;

	const PD::GC::PKT_GC_CONTENTS_EVENT_GROUP_ACTIVATED_LIST_READ::ContentsEventListIterator Last(Packet->GetLastContentsEventListIterator());
	for(PD::GC::PKT_GC_CONTENTS_EVENT_GROUP_ACTIVATED_LIST_READ::ContentsEventListIterator It(Packet->GetFirstContentsEventListIterator()); It != Last; It.Advance())
	{
		GSLOG(Log, TEXT(" - ContentsEventList, GroupId:%d, StartTime:%s(), EndTime:%s()"), It->GroupId(), *FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(It->StartTime()).ToString(), *FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(It->EndTime()).ToString());

		ContentsEventGroupDataMap.Emplace(It->GroupId(), *It);
	}

	UGsContentsEventManager* ContentsEventManager = GSContentsEvent();
	if (nullptr == ContentsEventManager)
	{
		GSLOG(Error, TEXT("nullptr	== ContentsEventManager"));
		return;
	}

	ContentsEventManager->ResetActivatedContentsEventGroupDatas(ContentsEventGroupDataMap);
}

void FGsNetMessageHandlerContentsEvent::CONTENTS_EVENT_GROUP_ACTIVATE(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_CONTENTS_EVENT_GROUP_ACTIVATE_READ* Packet = reinterpret_cast<PD::GC::PKT_GC_CONTENTS_EVENT_GROUP_ACTIVATE_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	GSLOG(Log, TEXT("CONTENTS_EVENT_GROUP_ACTIVATE(%d, %s, %s)"), Packet->GroupId() , *FDateTime(Packet->StartTime()).ToString(), *FDateTime(Packet->EndTime()).ToString());

	UGsContentsEventManager* ContentsEventManager = GSContentsEvent();
	if (nullptr == ContentsEventManager)
	{
		GSLOG(Error, TEXT("nullptr	== ContentsEventManager"));
		return;
	}

	const FGsNetMessageHandlerContentsEvent::FGsContentsEventGroupData ContentsEventGroupData(*Packet);

	ContentsEventManager->ActivateContentsEventGroup(ContentsEventGroupData);
}

void FGsNetMessageHandlerContentsEvent::CONTENTS_EVENT_GROUP_DEACTIVATE(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_CONTENTS_EVENT_GROUP_DEACTIVATE_READ* Packet = reinterpret_cast<PD::GC::PKT_GC_CONTENTS_EVENT_GROUP_DEACTIVATE_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	UGsContentsEventManager* ContentsEventManager = GSContentsEvent();
	if (nullptr == ContentsEventManager)
	{
		GSLOG(Error, TEXT("nullptr	== ContentsEventManager"));
		return;
	}

	GSLOG(Log, TEXT("CONTENTS_EVENT_GROUP_DEACTIVATE(%d)"), Packet->GroupId());

	ContentsEventManager->DeactivateContentsEventGroup(Packet->GroupId());
}

void FGsNetMessageHandlerContentsEvent::CONTENTS_EVENT_ACTIVATED_LIST(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_CONTENTS_EVENT_ACTIVATED_LIST_READ* Packet = reinterpret_cast<PD::GC::PKT_GC_CONTENTS_EVENT_ACTIVATED_LIST_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	GSLOG(Log, TEXT("CONTENTS_EVENT_ACTIVATED_LIST()"));

	UGsContentsEventManager* ContentsEventManager = GSContentsEvent();
	if (nullptr == ContentsEventManager)
	{
		GSLOG(Error, TEXT("nullptr	== ContentsEventManager"));
		return;
	}

	TMap<ContentsEventType, TMap<ContentsEventId, FGsNetMessageHandlerContentsEvent::FGsContentsEventData>> ContentsEventDatasMap;

	const PD::GC::PKT_GC_CONTENTS_EVENT_ACTIVATED_LIST_READ::ActivatedListIterator Last(Packet->GetLastActivatedListIterator());
	for (PD::GC::PKT_GC_CONTENTS_EVENT_ACTIVATED_LIST_READ::ActivatedListIterator It(Packet->GetFirstActivatedListIterator()); It != Last; It.Advance())
	{
		GSLOG(Log, TEXT(" - ActivatedList, ContentsEventType:%d, ContentsEventId:%d"), It->ContentsEventType(), It->ContentsEventId());

		TMap<ContentsEventId, FGsNetMessageHandlerContentsEvent::FGsContentsEventData>& ContentsEventDatas = ContentsEventDatasMap.FindOrAdd(It->ContentsEventType());
		ContentsEventDatas.Emplace(It->ContentsEventId(), *It);
	}

	ContentsEventManager->ResetActivatedContentsEventDatas(ContentsEventDatasMap);
}

void FGsNetMessageHandlerContentsEvent::CONTENTS_EVENT_ACTIVATE(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_CONTENTS_EVENT_ACTIVATE_READ* Packet = reinterpret_cast<PD::GC::PKT_GC_CONTENTS_EVENT_ACTIVATE_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	GSLOG(Log, TEXT("CONTENTS_EVENT_ACTIVATE(%d, %d)"), Packet->ContentsEventType(), Packet->ContentsEventId());

	UGsContentsEventManager* ContentsEventManager = GSContentsEvent();
	if (nullptr == ContentsEventManager)
	{
		GSLOG(Error, TEXT("nullptr	== ContentsEventManager"));
		return;
	}

	ContentsEventManager->ActivateContentsEvent(Packet->ContentsEventType(), Packet->ContentsEventId());
}

void FGsNetMessageHandlerContentsEvent::CONTENTS_EVENT_DEACTIVATE(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_CONTENTS_EVENT_DEACTIVATE_READ* Packet = reinterpret_cast<PD::GC::PKT_GC_CONTENTS_EVENT_DEACTIVATE_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	GSLOG(Log, TEXT("CONTENTS_EVENT_DEACTIVATE(%d, %d)"), Packet->ContentsEventType(), Packet->ContentsEventId());

	UGsContentsEventManager* ContentsEventManager = GSContentsEvent();
	if (nullptr == ContentsEventManager)
	{
		GSLOG(Error, TEXT("nullptr	== ContentsEventManager"));
		return;
	}

	ContentsEventManager->DeactivateContentsEvent(Packet->ContentsEventType(), Packet->ContentsEventId());
}
