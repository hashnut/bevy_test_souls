#include "GsNetMessageHandlerCraft.h"

#include "Currency/GsCurrencyHelper.h"
#include "Item/GsItemManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsCraftManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Message/MessageParam/GsCraftMessageParam.h"
#include "Net/GSNet.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#if WITH_EDITOR
#include "Item/Craft/GsCraftTab.h"
#include "Item/GsItem.h"
#endif

void FGsNetMessageHandlerCraft::InitializeMessage(FGsNetManager* InManager)
{
	// 패킷 바인딩
	MProtocalWorld& WorldProtocol = InManager->GetProtocalWorld();

	InitializeMessageDelegateList(&WorldProtocol);

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_CRAFT, this, &FGsNetMessageHandlerCraft::ACK_CRAFT));
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_CRAFT_FAVORITES_CREATE, this, &FGsNetMessageHandlerCraft::ACK_CRAFT_FAVORITES_CREATE));
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_CRAFT_FAVORITES_DELETE, this, &FGsNetMessageHandlerCraft::ACK_CRAFT_FAVORITES_DELETE));
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_CRAFT_FAVORITES_LIST, this, &FGsNetMessageHandlerCraft::ACK_CRAFT_FAVORITES_LIST));
}

void FGsNetMessageHandlerCraft::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerCraft::ACK_CRAFT_FAVORITES_CREATE(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_CRAFT_FAVORITES_CREATE_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ACK_CRAFT_FAVORITES_CREATE_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	FGsNetMessageHandlerCraft::ACK_CRAFT_FAVORITES_CREATE_Impl(Packet->CraftId());
}

void FGsNetMessageHandlerCraft::ACK_CRAFT_FAVORITES_DELETE(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_CRAFT_FAVORITES_DELETE_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ACK_CRAFT_FAVORITES_DELETE_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	FGsNetMessageHandlerCraft::ACK_CRAFT_FAVORITES_DELETE_Impl(Packet->CraftId());
}

void FGsNetMessageHandlerCraft::ACK_CRAFT_FAVORITES_LIST(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_CRAFT_FAVORITES_LIST_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ACK_CRAFT_FAVORITES_LIST_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	TArray<CraftId> FavoritedCraftIds;
	Packet->MakeFavoriteCraftIdListVector<TArray<CraftId>, CraftId>(FavoritedCraftIds);
	FGsNetMessageHandlerCraft::ACK_CRAFT_FAVORITES_LIST_Impl(FavoritedCraftIds);
}

void FGsNetMessageHandlerCraft::ACK_CRAFT(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_CRAFT_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ACK_CRAFT_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	TArray<ItemDBIdCraftResultPair> CraftResultInfos;	// 성공 / 대성공 등 결과 목록{ ItemDBIdCraftResultPair } 의 리스트.
	Packet->MakeCraftResultListVector<TArray<ItemDBIdCraftResultPair>, ItemDBIdCraftResultPair>(CraftResultInfos);

#if WITH_EDITOR
	FString PacketStr;
	PacketStr.Append(TEXT("ACK_CRAFT (\n"));
	PacketStr.Appendf(TEXT(" - InErrorCode: %d\n"), Packet->Result());
	PacketStr.Appendf(TEXT(" - InCurrentCraftId: %d\n"), Packet->CraftId());
	for (const ItemDBIdCraftResultPair& It : CraftResultInfos)
	{
		PacketStr.Appendf(TEXT("   - mCraftResult:%u\n"), (uint8)It.mCraftResult);
	}

	PacketStr.Append(TEXT(")\n"));
	GSLOG(Log, TEXT("%s"), *PacketStr);
#endif // WITH_EDITOR

	FGsNetMessageHandlerCraft::ACK_CRAFT_Impl(
		StaticCast<PD::Result>(Packet->Result()),
		Packet->CraftId(),
		CraftResultInfos
	);
}

void FGsNetMessageHandlerCraft::ACK_CRAFT_FAVORITES_CREATE_Impl(const CraftId CurrentCraftId)
{
	GSLOG(Log, TEXT("CurrentCraftId:%d"), CurrentCraftId);

	UGsCraftManager* CraftManager = GSCraft();
	if (nullptr	== CraftManager)
	{
		GSLOG(Error, TEXT("nullptr	== CraftManager"));
		return;
	}

	CraftManager->RegisterFavorite(CurrentCraftId);
}

void FGsNetMessageHandlerCraft::ACK_CRAFT_FAVORITES_DELETE_Impl(const CraftId CurrentCraftId)
{
	GSLOG(Log, TEXT("CurrentCraftId:%d"), CurrentCraftId);

	UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr	== CraftManager"));
		return;
	}

	CraftManager->UnregisterFavorite(CurrentCraftId);
}

void FGsNetMessageHandlerCraft::ACK_CRAFT_FAVORITES_LIST_Impl(const TArray<CraftId>& InFavoritedCraftIds)
{
// #if WITH_EDITOR
	GSLOG(Log, TEXT("FGsNetMessageHandlerCraft::ACK_CRAFT_FAVORITES_LIST_Impl()"));
	for (int32 i = 0; i < InFavoritedCraftIds.Num(); ++i)
	{
		GSLOG(Log, TEXT("InFavoritedCraftIds[%d]: %d"), i, InFavoritedCraftIds[i]);
	}
// #endif // WITH_EDITOR

	UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr	== CraftManager"));
		return;
	}

	CraftManager->ResetFavoriteAll(InFavoritedCraftIds);
}

void FGsNetMessageHandlerCraft::ACK_CRAFT_Impl(
	const PD::Result InErrorCode,
	const CraftId InCurrentCraftId,
	const TArray<ItemDBIdCraftResultPair>& InCraftResultInfos)
{
	bool IsSuccess = (PACKET_RESULT_SUCCESS == InErrorCode);
	if (!IsSuccess)
	{
		FText errorText;
		FString key = FString::Format(TEXT("{0}"), { PD::ResultEnumToDisplayKey(static_cast<PD::Result>(InErrorCode)) });
		FText::FindText(TEXT("NetText"), key, errorText);
		if (errorText.IsEmpty())
		{
			FGsUIHelper::PopupNetError(InErrorCode);
		}
		// 서버 수량 제한이 걸린 아이템에 대하여 일부 수량만 성공했을 경우
		else if (PD::Result::CRAFT_ERROR_ONLY_SOME_ITEMS_WERE_CRAFTED == InErrorCode)
		{
			FGsUIHelper::PopupNetError(errorText);
			IsSuccess = true;
		}
		else
		{
			FGsUIHelper::TrayMessageTicker(errorText);
		}
	}

	// 메세지
	FGsCraftMessageParamOnCraftResult(IsSuccess, InCurrentCraftId, InCraftResultInfos).Send();
}