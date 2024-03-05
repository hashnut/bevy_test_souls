// Fill out your copyright notice in the Description page of Project Settings.

#include "GsNetSendServiceMarket.h"
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Client_Gateway.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Market/GsMarketCollectionData.h"


void FGsNetSendServiceMarket::SendReqSalesItemList(bool bIsWorldMarket, MarketCategory InCategory, const TArray<ItemGrade> InGradeList,
	ItemId InLastItemId, bool bInShowBlockUI)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			if (bInShowBlockUI)
			{
				FGsUIHelper::ShowBlockUI();
			}

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			if (bIsWorldMarket)
			{
				PD::CG::PKT_CG_REQ_WDMARKET_SALES_ITEM_LIST_WRITE builder(
					sendBuffer->GetBuffer(), sendBuffer->GetSize(), InCategory, &InGradeList, InLastItemId);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				PD::CG::PKT_CG_REQ_MARKET_SALES_ITEM_LIST_WRITE builder(
					sendBuffer->GetBuffer(), sendBuffer->GetSize(), InCategory, &InGradeList, InLastItemId);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
		}
	}
}

void FGsNetSendServiceMarket::SendReqTransactionListInstackable(bool bIsWorldMarket, ItemId InItemId, time_t InLastUpdateTime)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			if (bIsWorldMarket)
			{
				PD::CG::PKT_CG_REQ_WDMARKET_TRANSACTION_LIST_INSTACKABLE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InItemId, InLastUpdateTime);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				PD::CG::PKT_CG_REQ_MARKET_TRANSACTION_LIST_INSTACKABLE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InItemId, InLastUpdateTime);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
		}
	}
}

void FGsNetSendServiceMarket::SendReqTransactionListStackable(bool bIsWorldMarket, ItemId InItemId, time_t InLastUpdateTime)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			if (bIsWorldMarket)
			{
				PD::CG::PKT_CG_REQ_WDMARKET_TRANSACTION_LIST_STACKABLE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InItemId, InLastUpdateTime);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				PD::CG::PKT_CG_REQ_MARKET_TRANSACTION_LIST_STACKABLE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InItemId, InLastUpdateTime);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
		}
	}
}

void FGsNetSendServiceMarket::SendReqCalculateList(bool bIsWorldMarket)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			if (bIsWorldMarket)
			{
				PD::CG::PKT_CG_REQ_WDMARKET_CALCULATE_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				PD::CG::PKT_CG_REQ_MARKET_CALCULATE_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
		}
	}
}

void FGsNetSendServiceMarket::SendReqMyTransactionList(bool bIsWorldMarket)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			if (bIsWorldMarket)
			{
				PD::CG::PKT_CG_REQ_WDMARKET_MY_TRANSACTION_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				PD::CG::PKT_CG_REQ_MARKET_MY_TRANSACTION_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
		}
	}
}

void FGsNetSendServiceMarket::SendReqItemPrice(bool bIsWorldMarket, ItemId InItemId, Level InEnchantLevel)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			if (bIsWorldMarket)
			{
				PD::CG::PKT_CG_REQ_WDMARKET_ITEM_PRICE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InItemId, InEnchantLevel);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				PD::CG::PKT_CG_REQ_MARKET_ITEM_PRICE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InItemId, InEnchantLevel);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
		}
	}
}

void FGsNetSendServiceMarket::SendReqItemSearch(bool bIsWorldMarket, const TArray<ItemId>& InItemIdList)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			if (bIsWorldMarket)
			{
				PD::CG::PKT_CG_REQ_WDMARKET_ITEM_SEARCH_WRITE
					builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), &InItemIdList);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				PD::CG::PKT_CG_REQ_MARKET_ITEM_SEARCH_WRITE
					builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), &InItemIdList);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}			
		}
	}

	// 2023/2/14 PKT - https://jira.com2us.com/jira/browse/C2URWQ-2819 관련 이슈로 주석 처리( 담당이 아녀서 흔적은 남겨 둠.)
	//FGsUIHelper::ShowBlockUI();
}

void FGsNetSendServiceMarket::SendReqTransactionDetailEquip(bool bIsWorldMarket, MarketTransactionId InTransactionId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			if (bIsWorldMarket)
			{
				PD::CG::PKT_CG_REQ_WDMARKET_TRANSACTION_DETAIL_EQUIP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InTransactionId);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				PD::CG::PKT_CG_REQ_MARKET_TRANSACTION_DETAIL_EQUIP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InTransactionId);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
		}
	}
}

void FGsNetSendServiceMarket::SendReqBookmarkList(bool bIsWorldMarket)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			if (bIsWorldMarket)
			{
				PD::CG::PKT_CG_REQ_WDMARKET_BOOKMARK_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				PD::CG::PKT_CG_REQ_MARKET_BOOKMARK_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
		}
	}
}

void FGsNetSendServiceMarket::SendReqBookmarkRegister(bool bIsWorldMarket, ItemId InItemId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			if (bIsWorldMarket)
			{
				PD::CG::PKT_CG_REQ_WDMARKET_BOOKMARK_REGISTER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InItemId);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				PD::CG::PKT_CG_REQ_MARKET_BOOKMARK_REGISTER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InItemId);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
		}
	}
}

void FGsNetSendServiceMarket::SendReqBookmarkDeregistered(bool bIsWorldMarket, ItemId InItemId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			if (bIsWorldMarket)
			{
				PD::CG::PKT_CG_REQ_WDMARKET_BOOKMARK_DEREGISTERED_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InItemId);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				PD::CG::PKT_CG_REQ_MARKET_BOOKMARK_DEREGISTERED_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InItemId);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
		}
	}
}

void FGsNetSendServiceMarket::SendReqCalculatedList(bool bIsWorldMarket, MarketTransactionId InTransitionId, time_t InLastSalesCompletedTime,
	bool bInShowBlockUI)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			if (bInShowBlockUI)
			{
				FGsUIHelper::ShowBlockUI();
			}

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			if (bIsWorldMarket)
			{
				PD::CG::PKT_CG_REQ_WDMARKET_CALCULATED_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InTransitionId, InLastSalesCompletedTime);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				PD::CG::PKT_CG_REQ_MARKET_CALCULATED_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InTransitionId, InLastSalesCompletedTime);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}			
		}
	}
}

void FGsNetSendServiceMarket::SendReqCollectionItemMarketPriceLowest(bool bInIsPricePerUnit, CollectionId InCollectionId,
	TArray<FGsMarketLowestPriceRequestInfo>& InDataList)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();

			PD::CG::PKT_CG_REQ_COLLECTION_ITEM_MARKET_PRICE_LOWEST_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), bInIsPricePerUnit, InCollectionId);

			for (FGsMarketLowestPriceRequestInfo& iter : InDataList)
			{
				builder.CreateItemList(iter._itemId, iter._enchantLevel, iter._transactionCountRequired);
			}

			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendServiceMarket::SendReqContinentTax()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_CONTINENT_TAX_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void FGsNetSendServiceMarket::SendReqTransactionPurchase(bool bIsWorldMarket, MarketTransactionId InTransactionId)
{
	TArray<MarketTransactionId> idList;
	idList.Emplace(InTransactionId);

	SendReqTransactionPurchase(bIsWorldMarket, idList);
}

void FGsNetSendServiceMarket::SendReqTransactionPurchase(bool bIsWorldMarket, TArray<MarketTransactionId>& InTransactionIdList)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			// 동일 아이템 구매 시도를 막기 위해 화면 블록
			FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("TransactionPurchase")));

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			if (bIsWorldMarket)
			{
				PD::CS::PKT_CS_REQ_WDMARKET_TRANSACTION_PURCHASE_WRITE
					builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), &InTransactionIdList);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				PD::CS::PKT_CS_REQ_MARKET_TRANSACTION_PURCHASE_WRITE
					builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), &InTransactionIdList);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
		}
	}
}

void FGsNetSendServiceMarket::SendReqTransactionRegister(bool bIsWorldMarket, ItemDBId InItemDBId, ItemAmount InAmount, MarketPrice InPrice)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			if (bIsWorldMarket)
			{
				PD::CS::PKT_CS_REQ_WDMARKET_TRANSACTION_REGISTER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InItemDBId, InAmount, InPrice);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				PD::CS::PKT_CS_REQ_MARKET_TRANSACTION_REGISTER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InItemDBId, InAmount, InPrice);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
		}
	}
}

void FGsNetSendServiceMarket::SendReqTransactionCancel(bool bIsWorldMarket, MarketTransactionId InTransactionId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			if (bIsWorldMarket)
			{
				PD::CS::PKT_CS_REQ_WDMARKET_TRANSACTION_CANCEL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InTransactionId);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				PD::CS::PKT_CS_REQ_MARKET_TRANSACTION_CANCEL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					InTransactionId);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
		}
	}
}

void FGsNetSendServiceMarket::SendReqCalculate(bool bIsWorldMarket, bool bIsAll, MarketTransactionId InTransactionId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			if (bIsWorldMarket)
			{
				PD::CS::PKT_CS_REQ_WDMARKET_CALCULATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					bIsAll, InTransactionId);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}
			else
			{
				PD::CS::PKT_CS_REQ_MARKET_CALCULATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
					bIsAll, InTransactionId);
				netBase->Send(builder.mPktWriter.GetPktSize());
			}			
		}
	}
}
