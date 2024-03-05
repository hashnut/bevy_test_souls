#pragma once

#include "GsNetBase.h"
#include "GsNetBuffer.h"
#include "Item/Craft/GsCraftTab.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"

/**
 *  Item Packet Send Class
 */
class T1PROJECT_API FGsNetSendServiceCraft
{

	FGsNetSendServiceCraft() = delete;

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
	 * 제작 목록 요청
	 * (좀 더 정확히는 제작 갯수 제한의 현황을 요청함)
	 * CraftCategoryId craftCategoryId: 제작 카테고리 Id.
	 */
	static bool REQ_CRAFT_STAT_LIST(const CraftTabId InCraftTabId);

	/**
	 * CraftId craftId: 즐겨찾기 할 제작ID.
	 */
	static bool REQ_CRAFT_FAVORITES_CREATE(const CraftId InCraftId);

	/**
	 * CraftId craftId: 즐겨찾기 삭제 할 제작ID.
	 */
	static bool REQ_CRAFT_FAVORITES_DELETE(const CraftId InCraftId);

	/**
	 * 즐겨찾기 목록을 요청
	 */
	static bool REQ_CRAFT_FAVORITES_LIST();

	/**
	 * 제작 요청
	 * CraftId craftId: 제작 ID.
	 * int craftCount: 제작 개수.
	 * TArray<ItemDBIdAmountPair> materialItemList: 재료 목록.
	 */
	static bool REQ_CRAFT(const CraftId InCraftId, const ItemAmount InCraftCount, TArray<ItemDBIdAmountPair> InMaterials);

};