// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Market/GsMarketControllerBase.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"

namespace PD
{
	namespace GC
	{
		struct PKT_GC_ACK_WDMARKET_SALES_ITEM_LIST_READ;
		struct PKT_GC_ACK_WDMARKET_TRANSACTION_LIST_INSTACKABLE_READ;
		struct PKT_GC_ACK_WDMARKET_TRANSACTION_LIST_STACKABLE_READ;
		struct PKT_GC_ACK_WDMARKET_CALCULATE_LIST_READ;
		struct PKT_GC_ACK_WDMARKET_MY_TRANSACTION_LIST_READ;		
		struct PKT_GC_ACK_WDMARKET_ITEM_PRICE_READ;
		struct PKT_GC_ACK_WDMARKET_ITEM_SEARCH_READ;
		struct PKT_GC_ACK_WDMARKET_TRANSACTION_DETAIL_EQUIP_READ;
		struct PKT_GC_ACK_WDMARKET_BOOKMARK_LIST_READ;
		struct PKT_GC_ACK_WDMARKET_CALCULATED_LIST_READ;
	}
	namespace SC
	{
		struct PKT_SC_ACK_WDMARKET_TRANSACTION_REGISTER_READ;
	}
}

/**
 * 월드 거래소
 */
class T1PROJECT_API FGsMarketControllerWorld final : public FGsMarketControllerBase
{
public:
	using Super = FGsMarketControllerBase;

private:
	ItemGrade _minItemGrade = ItemGrade::NONE;
	
public:
	FGsMarketControllerWorld()
		: Super()
	{
	}
	virtual ~FGsMarketControllerWorld() {}

public:
	virtual void Initialize() override;

protected:
	virtual bool IsTradableItem(const struct FGsSchemaItemCommon* InRow) const override;
	virtual bool IsWorldMarket() const { return true; }

	// Set
public:
	void SetItemGroupList(PD::GC::PKT_GC_ACK_WDMARKET_SALES_ITEM_LIST_READ* InPacket);
	void SetInstackableItemList(PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_LIST_INSTACKABLE_READ* InPacket);
	void SetStackableItemList(PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_LIST_STACKABLE_READ* InPacket);
	void SetSettlementItemList(PD::GC::PKT_GC_ACK_WDMARKET_CALCULATE_LIST_READ* InPacket); // 정산 리스트
	void SetSellItemList(PD::GC::PKT_GC_ACK_WDMARKET_MY_TRANSACTION_LIST_READ* InPacket); // 판매 등록 리스트	
	void SetItemPrice(PD::GC::PKT_GC_ACK_WDMARKET_ITEM_PRICE_READ* InPacket); // 아이템 정보
	void SetSearchedList(PD::GC::PKT_GC_ACK_WDMARKET_ITEM_SEARCH_READ* InPacket);
	void SetItemDetailInfo(PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_DETAIL_EQUIP_READ* InPacket);
	void SetBookmarkList(PD::GC::PKT_GC_ACK_WDMARKET_BOOKMARK_LIST_READ* InPacket);
	void SetSettlementLogList(PD::GC::PKT_GC_ACK_WDMARKET_CALCULATED_LIST_READ* InPacket);


	// 판매 등록 결과
	void SetSellRegisteredItem(PD::SC::PKT_SC_ACK_WDMARKET_TRANSACTION_REGISTER_READ* InPacket);	
};
