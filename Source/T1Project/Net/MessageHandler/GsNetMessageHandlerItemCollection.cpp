#include "GsNetMessageHandlerItemCollection.h"

#include "Currency/GsCurrencyHelper.h"
#include "GsMessageHandlerOneParam.h"
#include "Item/GsItemManager.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeGlobal.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Net/GSNet.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

using namespace PD::SC;

void FGsNetMessageHandlerItemCollection::InitializeMessage(FGsNetManager* InManager)
{
	// 패킷 바인딩
	MProtocalWorld& WorldProtocol = InManager->GetProtocalWorld();

	InitializeMessageDelegateList(&WorldProtocol);

	AddMessageDelegate(WorldProtocol.AddRaw(SCPacketId::SC_ITEM_COLLECTION, this, &FGsNetMessageHandlerItemCollection::Notify_ITEM_COLLECTION));
	AddMessageDelegate(WorldProtocol.AddRaw(SCPacketId::SC_ACK_ITEM_COLLECTION_REGISTER, this, &FGsNetMessageHandlerItemCollection::Ack_ITEM_COLLECTION_REGISTER));
	AddMessageDelegate(WorldProtocol.AddRaw(SCPacketId::SC_ACK_ITEM_COLLECTION_BOOKMARK_CREATE, this, &FGsNetMessageHandlerItemCollection::Ack_ITEM_COLLECTION_BOOKMARK_CREATE));
	AddMessageDelegate(WorldProtocol.AddRaw(SCPacketId::SC_ACK_ITEM_COLLECTION_BOOKMARK_DELETE, this, &FGsNetMessageHandlerItemCollection::Ack_ITEM_COLLECTION_BOOKMARK_DELETE));

	AddMessageDelegate(WorldProtocol.AddRaw(SCPacketId::SC_ITEM_COLLECTION_ADD, this, &FGsNetMessageHandlerItemCollection::Notify_ITEM_COLLECTION_EVENT_ON));
	AddMessageDelegate(WorldProtocol.AddRaw(SCPacketId::SC_ITEM_COLLECTION_EXPIRED, this, &FGsNetMessageHandlerItemCollection::Notify_ITEM_COLLECTION_EVENT_OFF));
}

void FGsNetMessageHandlerItemCollection::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerItemCollection::Notify_ITEM_COLLECTION(const FGsNet::Packet& InPacket)
{
	PKT_SC_ITEM_COLLECTION_READ* Packet = reinterpret_cast<PKT_SC_ITEM_COLLECTION_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	if (FGsItemCollectionManager* ItemCollectionManager = GSItemCollection())
	{
		//<!--완료된 CollectionId 의 리스트-->
		TArray<CollectionData> CompletedFielDatas;
		//<!--진행 중인 CollectionConditionData 의 리스트-->
		TArray<CollectionConditionData> ProgressConditions;
		////<!--즐겨찾기된 CollectionId 의 리스트-->
		TArray<CollectionId> BookMarkFieldIds;

		Packet->MakeCollectionDataListVector<TArray<CollectionData>, CollectionData>(CompletedFielDatas);
		Packet->MakeCollectionConditionDataListVector<TArray<CollectionConditionData>, CollectionConditionData>(ProgressConditions);
		Packet->MakeBookmarkCollectionIdListVector<TArray<CollectionId>, CollectionId>(BookMarkFieldIds);

#if !UE_BUILD_SHIPPING
		FDateTime StartTime = FDateTime::UtcNow();
#endif

		ItemCollectionManager->NotifyInit(CompletedFielDatas, ProgressConditions, BookMarkFieldIds);

#if !UE_BUILD_SHIPPING
		FDateTime EndTime = FDateTime::UtcNow();
		int64 Milliseconds = (EndTime.ToUnixTimestamp() * 1000 + EndTime.GetMillisecond()) - (StartTime.ToUnixTimestamp() * 1000 + StartTime.GetMillisecond());
		GSLOG(Warning, TEXT("[InitializeTimeChecker] ItemCollection Manager NotifyInit : %d"), Milliseconds);
#endif
	}	
}

void FGsNetMessageHandlerItemCollection::Ack_ITEM_COLLECTION_REGISTER(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_ITEM_COLLECTION_REGISTER_READ* Packet = reinterpret_cast<PKT_SC_ACK_ITEM_COLLECTION_REGISTER_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet(PKT_SC_ACK_ITEM_COLLECTION_REGISTER_READ)"));
		return;
	}

	const PD::Result Result = StaticCast<PD::Result>(Packet->Result());
	if (Result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(Packet->Result()));
		return;
	}
	
	if (FGsItemCollectionManager* ItemCollectionManager = GSItemCollection())
	{
		ItemCollectionManager->AckUpdateField(Packet->CollectionData(), Packet->CollectionConditionId(), Packet->RegisterCount());
	}
}

void FGsNetMessageHandlerItemCollection::Ack_ITEM_COLLECTION_BOOKMARK_CREATE(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_ITEM_COLLECTION_BOOKMARK_CREATE_READ* Packet = reinterpret_cast<PKT_SC_ACK_ITEM_COLLECTION_BOOKMARK_CREATE_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet(PKT_CS_REQ_ITEM_COLLECTION_BOOKMARK_DELETE_READ)"));
		return;
	}

	const PD::Result Result = StaticCast<PD::Result>(Packet->Result());
	if (Result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(Packet->Result()));
		return;
	}

	FGsItemCollectionManager* ItemCollectionManager = GSItemCollection();
	if (nullptr == ItemCollectionManager)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	ItemCollectionManager->AckBookMark(Packet->CollectionId(), true);
}

void FGsNetMessageHandlerItemCollection::Ack_ITEM_COLLECTION_BOOKMARK_DELETE(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_ITEM_COLLECTION_BOOKMARK_DELETE_READ* Packet = reinterpret_cast<PKT_SC_ACK_ITEM_COLLECTION_BOOKMARK_DELETE_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet(PKT_CS_REQ_ITEM_COLLECTION_BOOKMARK_DELETE_READ)"));
		return;
	}

	const PD::Result Result = StaticCast<PD::Result>(Packet->Result());
	if (Result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(Packet->Result()));
		return;
	}

	FGsItemCollectionManager* ItemCollectionManager = GSItemCollection();
	if (nullptr == ItemCollectionManager)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	ItemCollectionManager->AckBookMark(Packet->CollectionId(), false);
}

void FGsNetMessageHandlerItemCollection::Notify_ITEM_COLLECTION_EVENT_ON(const FGsNet::Packet& InPacket)
{
	PKT_SC_ITEM_COLLECTION_ADD_READ* Packet = reinterpret_cast<PKT_SC_ITEM_COLLECTION_ADD_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet(PKT_SC_ITEM_COLLECTION_ADD_READ)"));
		return;
	}

	FGsItemCollectionManager* ItemCollectionManager = GSItemCollection();
	if (nullptr == ItemCollectionManager)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	ItemCollectionManager->AckEventChanged(Packet->CollectionId(), true);
}

void FGsNetMessageHandlerItemCollection::Notify_ITEM_COLLECTION_EVENT_OFF(const FGsNet::Packet& InPacket)
{
	PKT_SC_ITEM_COLLECTION_EXPIRED_READ* Packet = reinterpret_cast<PKT_SC_ITEM_COLLECTION_EXPIRED_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet(PKT_SC_ITEM_COLLECTION_ADD_READ)"));
		return;
	}

	FGsItemCollectionManager* ItemCollectionManager = GSItemCollection();
	if (nullptr == ItemCollectionManager)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	ItemCollectionManager->AckEventChanged(Packet->CollectionId(), false);
}