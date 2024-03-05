#pragma once

#include "GsNetMessageHandleInterface.h"
#include "Message/GsMessageNet.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsNetManager;

class FGsNetMessageHandlerCraft final
	: public IGsNetMessageHandler, FGsNetMessageWorld
{

	friend class FGsNetSendServiceCraft;

	/************************************************************************/
	/* instancing															*/
	/************************************************************************/

public:
	void InitializeMessage(FGsNetManager* InManager) override;

	void FinalizeMessage() override;

	/************************************************************************/
	/* acknowledge															*/
	/************************************************************************/

private:
	/**
	 * CS_REQ_CRAFT_FAVORITES_CREATE 의 응답
	 * 
	 * * CraftId craftId: 즐겨찾기 된 제작ID.
	 */
	void ACK_CRAFT_FAVORITES_CREATE(const FGsNet::Packet& InPacket);

	/**
	 * CS_REQ_CRAFT_FAVORITES_DELETE 의 응답
	 * 
	 * * CraftId craftId: 즐겨찾기 삭제 된 제작ID.
	 */
	void ACK_CRAFT_FAVORITES_DELETE(const FGsNet::Packet& InPacket);

	/**
	 * CS_REQ_CRAFT_FAVORITES_LIST 의 응답
	 * 
	 * * TArray<CraftId> craftIdList: 즐겨찾기 제작ID 목록.
	 */
	void ACK_CRAFT_FAVORITES_LIST(const FGsNet::Packet& InPacket);

	/**
	 * CS_REQ_CRAFT 의 응답
	 *
	 * * int result: 결과.
	 * * CraftId craftId: 제작 ID.
	 * * ItemAmount craftCount: 이번에 제작한 개수.
	 * * TArray<ItemDBIdAmountPair> itemDecreased: 차감할 아이템 목록.
	 * * TArray<ItemDBId> itemDBIdsDeleted: 삭제할 아이템 목록.
	 * * TArray<ItemDataCreated>itemCreated: 생성할 아이템 목록.
	 * * TArray<ItemDBIdAmountPair> itemIncreased: 개수 증가할 아이템 목록.
	 * * TArray<ItemDBIdCraftResultPair> craftResultList: 성공 / 대성공... 등 결과 목록.
	 * * TArray<CurrencyPair> currencyDelta: 재화 변화량 목록.
	 * * TArray<CurrencyPair> currencyFinal: 최종 재화량 목록.
	 * * ItemAmount craftStatList0: 현재까지 서버에 제작된 개수. (사용하지 않을 경우의 값: INVALID_CRAFT_STAT_COUNT)
	 * * ItemAmount craftStatList1: 현재까지 서버 계정에 제작된 개수. (사용하지 않을 경우의 값: INVALID_CRAFT_STAT_COUNT)
	 */
	void ACK_CRAFT(const FGsNet::Packet& InPacket);

	/************************************************************************/
	/* implementation                                                       */
	/************************************************************************/

private:
	static void ACK_CRAFT_FAVORITES_CREATE_Impl(const CraftId CurrentCraftId);

	static void ACK_CRAFT_FAVORITES_DELETE_Impl(const CraftId CurrentCraftId);

	static void ACK_CRAFT_FAVORITES_LIST_Impl(const TArray<CraftId>& InFavoritedCraftIds);

	static void ACK_CRAFT_Impl(
		const PD::Result InErrorCode,
		const CraftId InCurrentCraftId,	
		const TArray<ItemDBIdCraftResultPair>& InCraftResultInfos
	);
};