#include "GsNetSendServiceCraft.h"

#include "Shared/Shared/SharedPackets/PD_Client_Gateway.h"
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"

#include "T1Project.h"

#define WITH_GSSERVER true

#if WITH_EDITOR
#include "Item/GsItemManager.h"
#endif

#if !WITH_GSSERVER
#include "Runtime/Core/Public/Containers/StringFwd.h"

#include "Item/Craft/GsCraftTarget.h"
#include "Management/ScopeGame/GsCraftManager.h"
#include "Message/MessageParam/GsCraftMessageParam.h"
#include "MessageHandler/GsNetMessageHandlerCraft.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#endif

bool FGsNetSendServiceCraft::REQ_CRAFT_STAT_LIST(const CraftTabId InCraftTabId)
{
	GSLOG(Log, TEXT("REQ_CRAFT_STAT_LIST(%d)"), InCraftTabId);

	check(INVALID_CRAFT_TAB_ID != InCraftTabId);

#if WITH_GSSERVER

	if (!FGsNetSendServiceCraft::SendPacket<PD::CG::PKT_CG_REQ_CRAFT_STAT_LIST_WRITE>(InCraftTabId))
	{
		GSLOG(Error, TEXT("!SendPacket<PD::CG::PKT_CG_REQ_CRAFT_STAT_LIST_WRITE>(InCraftTabId), InCraftTabId:%d"), InCraftTabId);
		return false;
	}

#else

	UGsCraftManager* CraftManager = GSCraft();
	if (nullptr != CraftManager && nullptr != CraftManager->GetOuter() && nullptr != CraftManager->GetOuter()->GetWorld())
	{
		CraftManager->GetOuter()->GetWorld()->GetTimerManager().SetTimerForNextTick([InCraftTabId]()
			{
				TArray<CraftIdAmountPair> CraftStatListServer;				// : 현재까지 서버에 제작된 개수 목록.
				TArray<CraftIdAmountPair> CraftStatListServerAccount;		// : 현재까지 서버 계정에 제작된 개수 목록.
				FGsCraftMessageParamAckCraftStatList(InCraftTabId, MoveTemp(CraftStatListServer), MoveTemp(CraftStatListServerAccount)).Send();
			});
	}

#endif

	return true;
}

bool FGsNetSendServiceCraft::REQ_CRAFT_FAVORITES_CREATE(const CraftId InCraftId)
{
	GSLOG(Log, TEXT("REQ_CRAFT_FAVORITES_CREATE(%d)"), InCraftId);

	check(INVALID_CRAFT_ID != InCraftId);

#if WITH_GSSERVER

	if (!FGsNetSendServiceCraft::SendPacket<PD::CS::PKT_CS_REQ_CRAFT_FAVORITES_CREATE_WRITE>(InCraftId))
	{
		GSLOG(Error, TEXT("!SendPacket<PD::CS::PKT_CS_REQ_CRAFT_FAVORITES_CREATE_WRITE>(InCraftTabId), InCraftId:%d"), InCraftId);
		return false;
	}

#else

	UGsCraftManager* CraftManager = GSCraft();
	if (nullptr != CraftManager && nullptr != CraftManager->GetOuter() && nullptr != CraftManager->GetOuter()->GetWorld())
	{
		CraftManager->GetOuter()->GetWorld()->GetTimerManager().SetTimerForNextTick([InCraftId]()
			{
				FGsNetMessageHandlerCraft::ACK_CRAFT_FAVORITES_CREATE_Impl(InCraftId);
			});
	}

#endif

	return true;
}

bool FGsNetSendServiceCraft::REQ_CRAFT_FAVORITES_DELETE(const CraftId InCraftId)
{
	GSLOG(Log, TEXT("REQ_CRAFT_FAVORITES_DELETE(%d)"), InCraftId);

	check(INVALID_CRAFT_ID != InCraftId);

#if WITH_GSSERVER

	if (!FGsNetSendServiceCraft::SendPacket<PD::CS::PKT_CS_REQ_CRAFT_FAVORITES_DELETE_WRITE>(InCraftId))
	{
		GSLOG(Error, TEXT("!SendPacket<PD::CS::PKT_CS_REQ_CRAFT_FAVORITES_DELETE_WRITE>(InCraftTabId), InCraftId:%d"), InCraftId);
		return false;
	}

#else

	UGsCraftManager* CraftManager = GSCraft();
	if (nullptr != CraftManager && nullptr != CraftManager->GetOuter() && nullptr != CraftManager->GetOuter()->GetWorld())
	{
		CraftManager->GetOuter()->GetWorld()->GetTimerManager().SetTimerForNextTick([InCraftId]()
			{
				FGsNetMessageHandlerCraft::ACK_CRAFT_FAVORITES_DELETE_Impl(InCraftId);
			});
	}

#endif

	return true;
}

bool FGsNetSendServiceCraft::REQ_CRAFT_FAVORITES_LIST()
{
	GSLOG(Log, TEXT("REQ_CRAFT_FAVORITES_LIST()"));

#if WITH_GSSERVER

	if (!FGsNetSendServiceCraft::SendPacket<PD::CS::PKT_CS_REQ_CRAFT_FAVORITES_LIST_WRITE>())
	{
		GSLOG(Error, TEXT("!SendPacket<PD::CS::PKT_CS_REQ_CRAFT_FAVORITES_LIST_WRITE>()"));
		return false;
	}

#else

	UGsCraftManager* CraftManager = GSCraft();
	if (nullptr != CraftManager && nullptr != CraftManager->GetOuter() && nullptr != CraftManager->GetOuter()->GetWorld())
	{
		CraftManager->GetOuter()->GetWorld()->GetTimerManager().SetTimerForNextTick([]()
			{
				TArray<CraftId> InFavoritedCraftIds;
				FGsNetMessageHandlerCraft::ACK_CRAFT_FAVORITES_LIST_Impl(InFavoritedCraftIds);
			});
	}

#endif

	return true;
}

bool FGsNetSendServiceCraft::REQ_CRAFT(const CraftId InCraftId, const ItemAmount InCraftCount, TArray<ItemDBIdAmountPair> InMaterials)
{
	check(INVALID_CRAFT_ID != InCraftId);
	check(0 < InCraftCount);
	// check(0 < InMaterials.Num());

#if WITH_EDITOR
	UGsItemManager* ItemManager = GItem();
	FString PacketStr;
	PacketStr.Append(TEXT("REQ_CRAFT (\n"));
	PacketStr.Appendf(TEXT(" - InCraftId: %d\n"), InCraftId);
	PacketStr.Appendf(TEXT(" - InCraftCount: %d\n"), InCraftCount);
	PacketStr.Appendf(TEXT(" - InMaterials.Num(): %d\n"), InMaterials.Num());
	for (int32 i = 0; i < InMaterials.Num(); ++i)
	{
		if (nullptr != ItemManager)
		{
			TWeakPtr<const FGsItem> Item = ItemManager->FindItem(InMaterials[i].mItemDBId);
			const FString ItemName = (true == Item.IsValid()) ? Item.Pin()->GetName().ToString() : TEXT("NO_ITEM_NAME");
			const ItemAmount Amount = (true == Item.IsValid()) ? Item.Pin()->GetAmount() : 0;
			PacketStr.Appendf(TEXT("   InMaterials[%d] { mItemDBId:%lld, mItemAmount:%d } (%s:%d)\n"), i, InMaterials[i].mItemDBId, InMaterials[i].mItemAmount, *ItemName, Amount);
		}
	}
	PacketStr.Append(TEXT(")\n"));
	GSLOG(Log, TEXT("%s"), *PacketStr);
#endif

#if WITH_GSSERVER

	if (!FGsNetSendServiceCraft::SendPacket<PD::CS::PKT_CS_REQ_CRAFT_WRITE>(InCraftId, InCraftCount, &InMaterials))
	{
		GSLOG(Error, TEXT("!FGsNetSendServiceCraft::SendPacketWithContainer<PD::CS::PKT_CS_REQ_CRAFT_WRITE, TArray<ItemDBIdAmountPair>>(%d, %d, %d)"), InCraftId, InCraftCount, InMaterials.Num());
		return false;
	}

#else

	UGsCraftManager* CraftManager = GSCraft();
	if (nullptr != CraftManager && nullptr != CraftManager->GetOuter() && nullptr != CraftManager->GetOuter()->GetWorld())
	{
		CraftManager->GetOuter()->GetWorld()->GetTimerManager().SetTimerForNextTick([InCraftId, InCraftCount]()
			{
				const PD::Result ErrorCode = (PD::Result)PACKET_RESULT_SUCCESS;
				TArray<ItemDBIdCraftResultPair> CraftResultInfos;		// 성공 / 대성공 등 결과 목록{ ItemDBIdCraftResultPair } 의 리스트.
				const FGsCraftTarget* CraftTarget = GSCraft()->GetCraftTarget(InCraftId);
				for (ItemAmount i = 0; i < InCraftCount; ++i)
				{
					CraftResult CurrentCraftResult = CraftResult::CRAFT_SUCCEEDED;
					if (nullptr != CraftTarget && !CraftTarget->CanBeFail())
					{
						const CraftResult Candidates[] { CraftResult::CRAFT_SUCCEEDED, CraftResult::CRAFT_SUCCEEDED_GREAT };
						CurrentCraftResult = Candidates[FMath::RandHelper(2)];
					}
					else if (nullptr != CraftTarget && CraftTarget->CanBeFail())
					{
						const CraftResult Candidates[]{ CraftResult::CRAFT_SUCCEEDED, CraftResult::CRAFT_SUCCEEDED_GREAT, CraftResult::CRAFT_FAIL };
						CurrentCraftResult = Candidates[FMath::RandHelper(3)];
					}

					ItemDBIdCraftResultPair CraftResultInfo;
					CraftResultInfo.mCraftResult = CurrentCraftResult;
					CraftResultInfos.Emplace(CraftResultInfo);
				}

				FGsNetMessageHandlerCraft::ACK_CRAFT_Impl(
					ErrorCode,
					InCraftId,
					CraftResultInfos
				);
			});
	}

#endif

	return true;
}
