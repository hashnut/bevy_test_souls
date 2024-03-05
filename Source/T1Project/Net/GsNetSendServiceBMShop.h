// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HIVE_IAPV4.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Client_Gateway.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "T1Project.h"

class FGsNetBase;

using namespace std;
using namespace hive;
/**
 * BMShop Packet Send Class
 */
class T1PROJECT_API FGsNetSendServiceBMShop
{
public:
	static void SendBMShopProductList();
	static void SendZpayBMShopProductList();
	static void SendBMShopStorageList();
	static void SendBMShopZpayProductValidCheck(const TArray<BMProductIdAmountPair>& InAmountPairList);
	static void SendBMShopProductValidCheck(TArray<BMProductIdAmountPair> InAmountPairList, const bool InBulkSale, const uint64 InCouponItemDbid = 0);
	static void SendBMShopPurchaseItem(TArray<BMProductIdAmountPair> InAmountPairList, const uint64 InIdentifier, const bool InIsBulkSale, uint64 InCouponItemDbid);
	static void SendBMShopPurchaseItem(FString InMarketPID, std::string InBypassInfo, const uint64 InIdentifier);
#if WITH_EDITOR
	static void SendBMShopPurchaseItemTest(const BMProductId InProductId, std::string InBypassInfo, const uint64 InIdentifier);
#endif
	static void SendReceiveStorageItem(const uint64 InStorageDBID);
	static void SendRestoreItem(FString InMarketPID, std::string InBypassInfo);
	static void SendSaveBulkSaleSetting(const uint16 InPageIndex , TArray<BMProductBulkSale> InPairList);
	static void SendPopupShopOpen();
	static void SendSearchUsingZpay();
	static void SendNothingToRestore();

	// 2023/10/31 PKT - 서버 이주에 대한 정보를 요청
	static void SendServerMigrateConditionList();
	static void SendServerMigrate(PlanetId planetId, WorldId worldId, ServerMigrateType type);

public:
	static void SendReqChangeMigrationNickname(UserDBId dbId, const FString nickName);
	static void SendReqChangeMigrationGuildName(UserDBId dbId, const FString guildName);

};

/** [패킷] 디버그 로깅용 */
FString LexToString(const PD::CS::PKT_CS_REQ_BM_SHOP_ITEM_LIST_WRITE& InPacket);
FString LexToString(const PD::CS::PKT_CS_REQ_BM_SHOP_PREPARE_HIVE_ITEM_WRITE& InPacket);
FString LexToString(const PD::CS::PKT_CS_REQ_BM_SHOP_PURCHASE_ITEM_WRITE& InPacket);
FString LexToString(const PD::CS::PKT_CS_REQ_HIVE_PG_UNCONSUMED_PURCHASE_WRITE& InPacket);
FString LexToString(const PD::CS::PKT_CS_REQ_SAVE_BM_BULK_SALE_LIST_WRITE& InPacket);
FString LexToString(const PD::CS::PKT_CS_REQ_BM_SHOP_POPUP_LIST_WRITE& InPacket);
FString LexToString(const PD::CS::PKT_CS_REQ_BM_SHOP_ITEM_RESTORE_WRITE& InPacket);
FString LexToString(const PD::CS::PKT_CS_REQ_BM_DEPOT_ITEM_LIST_WRITE& InPacket);
FString LexToString(const PD::CS::PKT_CS_REQ_RECIVIE_BM_DEPOT_WRITE& InPacket);

FString LexToString(const PD::CG::PKT_CG_REQ_Z_PAY_PURCHASED_ITEM_LIST_WRITE& InPacket);
FString LexToString(const PD::CG::PKT_CG_REQ_SEACRHING_Z_PAY_USER_WRITE& InPacket);
FString LexToString(const PD::CG::PKT_CG_REQ_PREPARE_Z_PAY_PURCHASE_ITEM_WRITE& InPacket);
FString LexToString(const PD::CG::PKT_CG_REQ_Z_PAY_PURCHASE_ITEM_WRITE& InPacket);
FString LexToString(const PD::CG::PKT_CG_REQ_Z_PAY_ITEM_RESTORE_WRITE& InPacket);
FString LexToString(const PD::CG::PKT_CG_Z_PAY_CANNOT_RESTORE_WRITE& InPacket);
FString LexToString(const PD::CG::PKT_CG_OPEN_PG_PURCHASE_WND_WRITE& InPacket);

FString LexToString(const PD::SC::PKT_SC_ACK_BM_SHOP_PURCHASE_ITEM_READ& InPacket);
FString LexToString(const PD::SC::PKT_SC_ACK_HIVE_PG_UNCONSUMED_PURCHASE_READ& InPacket);
FString LexToString(const PD::SC::PKT_SC_ACK_BM_SHOP_PREPARE_HIVE_ITEM_READ& InPacket);

FString LexToString(const PD::GC::PKT_GC_ACK_PREPARE_Z_PAY_PURCHASE_ITEM_READ& InPacket);

template<MessageContentStoreBM InMessageId, typename TPacket>
struct TGsBMShopMessageParamNetSendBase final : public TGsBMShopMessageParamBase<InMessageId>
{
private:
	TWeakPtr<FGsNetBase> _netBase;
	TSharedPtr<TPacket> _packet;

public:
	template<typename... TParams>
	explicit TGsBMShopMessageParamNetSendBase(TParams... InParams)
		: _netBase(TGsBMShopMessageParamNetSendBase::GetActiveNetBase())
		, _packet(MakePacket(InParams...))
	{
	}

	~TGsBMShopMessageParamNetSendBase() final
	{
	}

	bool Send() final
	{
		if (!_packet.IsValid() || !_netBase.IsValid())
		{
			GSLOG(Error, TEXT("!_packet.IsValid() || !_netBase.IsValid()"));
			return false;
		}

#if !NO_LOGGING
		GSLOG(Log, TEXT("%s"), *LexToString(*_packet.Get()));
#endif

		_netBase.Pin()->Send(_packet->mPktWriter.GetPktSize());
		return true;
	}

	bool SendAsync() = delete;

private:
	static TWeakPtr<FGsNetBase> GetActiveNetBase()
	{
		TWeakPtr<FGsNetBase> WeakNetBase;
		const FGsNetManager* NetManager = GNet();
		if (nullptr == NetManager)
		{
			GSLOG(Error, TEXT("nullptr == NetManager"));
		}
		else
		{
			TSharedPtr<FGsNetBase> NetBase = NetManager->GetActive();
			if (!NetBase.IsValid())
			{
				GSLOG(Error, TEXT("!NetBase.IsValid()"));
			}
			else
			{
				WeakNetBase = NetBase;
			}
		}
		return WeakNetBase;
	}

	template<typename... TParams>
	TSharedPtr<TPacket> MakePacket(TParams... InParams) const
	{
		TSharedPtr<TPacket> Packet;
		if (_netBase.IsValid())
		{
			TSharedPtr<FGsNetBase> NetBase = _netBase.Pin();
			FGsNetBuffer* NetBuffer = NetBase->GetLocalSendBufferPtr();
			if (nullptr != NetBuffer)
			{
				Packet = MakeShared<TPacket>(NetBuffer->GetBuffer(), NetBuffer->GetSize(), InParams...);
			}
		}
		return MoveTemp(Packet);
	}
};

using FGsREQ_BM_SHOP_ITEM_LIST = TGsBMShopMessageParamNetSendBase<MessageContentStoreBM::REQ_BM_SHOP_ITEM_LIST, PD::CS::PKT_CS_REQ_BM_SHOP_ITEM_LIST_WRITE>;
using FGsREQ_BM_SHOP_PREPARE_HIVE_ITEM = TGsBMShopMessageParamNetSendBase<MessageContentStoreBM::REQ_BM_SHOP_PREPARE_HIVE_ITEM, PD::CS::PKT_CS_REQ_BM_SHOP_PREPARE_HIVE_ITEM_WRITE>;
using FGsREQ_BM_SHOP_PURCHASE_ITEM = TGsBMShopMessageParamNetSendBase<MessageContentStoreBM::REQ_BM_SHOP_PURCHASE_ITEM, PD::CS::PKT_CS_REQ_BM_SHOP_PURCHASE_ITEM_WRITE>;
using FGsREQ_HIVE_PG_UNCONSUMED_PURCHASE = TGsBMShopMessageParamNetSendBase<MessageContentStoreBM::REQ_HIVE_PG_UNCONSUMED_PURCHASE, PD::CS::PKT_CS_REQ_HIVE_PG_UNCONSUMED_PURCHASE_WRITE>;
using FGsREQ_SAVE_BM_BULK_SALE_LIST = TGsBMShopMessageParamNetSendBase<MessageContentStoreBM::REQ_SAVE_BM_BULK_SALE_LIST, PD::CS::PKT_CS_REQ_SAVE_BM_BULK_SALE_LIST_WRITE>;
using FGsREQ_BM_SHOP_POPUP_LIST = TGsBMShopMessageParamNetSendBase<MessageContentStoreBM::REQ_BM_SHOP_POPUP_LIST, PD::CS::PKT_CS_REQ_BM_SHOP_POPUP_LIST_WRITE>;
using FGsREQ_BM_SHOP_ITEM_RESTORE = TGsBMShopMessageParamNetSendBase<MessageContentStoreBM::REQ_BM_SHOP_ITEM_RESTORE, PD::CS::PKT_CS_REQ_BM_SHOP_ITEM_RESTORE_WRITE>;
using FGsREQ_BM_DEPOT_ITEM_LIST = TGsBMShopMessageParamNetSendBase<MessageContentStoreBM::REQ_BM_DEPOT_ITEM_LIST, PD::CS::PKT_CS_REQ_BM_DEPOT_ITEM_LIST_WRITE>;
using FGsREQ_RECIVIE_BM_DEPOT = TGsBMShopMessageParamNetSendBase<MessageContentStoreBM::REQ_RECIVIE_BM_DEPOT, PD::CS::PKT_CS_REQ_RECIVIE_BM_DEPOT_WRITE>;

using FGsREQ_Z_PAY_PURCHASED_ITEM_LIST = TGsBMShopMessageParamNetSendBase<MessageContentStoreBM::REQ_Z_PAY_PURCHASED_ITEM_LIST, PD::CG::PKT_CG_REQ_Z_PAY_PURCHASED_ITEM_LIST_WRITE>;
using FGsREQ_SEACRHING_Z_PAY_USER = TGsBMShopMessageParamNetSendBase<MessageContentStoreBM::REQ_SEACRHING_Z_PAY_USER, PD::CG::PKT_CG_REQ_SEACRHING_Z_PAY_USER_WRITE>;
using FGsREQ_PREPARE_Z_PAY_PURCHASE_ITEM = TGsBMShopMessageParamNetSendBase<MessageContentStoreBM::REQ_PREPARE_Z_PAY_PURCHASE_ITEM, PD::CG::PKT_CG_REQ_PREPARE_Z_PAY_PURCHASE_ITEM_WRITE>;
using FGsREQ_Z_PAY_PURCHASE_ITEM = TGsBMShopMessageParamNetSendBase<MessageContentStoreBM::REQ_Z_PAY_PURCHASE_ITEM, PD::CG::PKT_CG_REQ_Z_PAY_PURCHASE_ITEM_WRITE>;
using FGsREQ_Z_PAY_ITEM_RESTORE = TGsBMShopMessageParamNetSendBase<MessageContentStoreBM::REQ_Z_PAY_ITEM_RESTORE, PD::CG::PKT_CG_REQ_Z_PAY_ITEM_RESTORE_WRITE>;
using FGsZ_PAY_CANNOT_RESTORE = TGsBMShopMessageParamNetSendBase<MessageContentStoreBM::Z_PAY_CANNOT_RESTORE, PD::CG::PKT_CG_Z_PAY_CANNOT_RESTORE_WRITE>;
using FGsOPEN_PG_PURCHASE_WND = TGsBMShopMessageParamNetSendBase<MessageContentStoreBM::OPEN_PG_PURCHASE_WND, PD::CG::PKT_CG_OPEN_PG_PURCHASE_WND_WRITE>;
