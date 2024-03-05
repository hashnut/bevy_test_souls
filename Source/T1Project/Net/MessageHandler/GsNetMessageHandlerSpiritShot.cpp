#include "GsNetMessageHandlerSpiritShot.h"

#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "Management/ScopeGame/GsSpiritShotManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Message/GsMessageNet.h"
#include "Message/MessageParam/GsSpiritShotMessageParam.h"
#include "Net/GSNet.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "../Management/GsScopeGame.h"

void FGsNetMessageHandlerSpiritShot::InitializeMessage(FGsNetManager* InManager)
{
	MProtocalWorld& worldProtocol = InManager->GetProtocalWorld();

	InitializeMessageDelegateList(&worldProtocol);

	AddMessageDelegate(worldProtocol.AddRaw(PD::SC::SCPacketId::SC_SPIRIT_SHOT_ENCHANT_INFO, this, &FGsNetMessageHandlerSpiritShot::SPIRIT_SHOT_INFO));
	AddMessageDelegate(worldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_SPIRIT_SHOT_ENCHANT, this, &FGsNetMessageHandlerSpiritShot::ACK_SPIRIT_SHOT_ENCHANT));
}

void FGsNetMessageHandlerSpiritShot::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerSpiritShot::SPIRIT_SHOT_INFO(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_SPIRIT_SHOT_ENCHANT_INFO_READ* packet = reinterpret_cast<PD::SC::PKT_SC_SPIRIT_SHOT_ENCHANT_INFO_READ*>(InPacket.Buffer);
	if (nullptr == packet)
	{
		GSLOG(Error, TEXT("nullptr == packet"));
		return;
	}

	TArray<SpiritShotLevelId> spiritShotLevelIdList;

	// Iterator 순회하면서 LevelId 읽기
	using LevelIterator = PD::SC::PKT_SC_SPIRIT_SHOT_ENCHANT_INFO_READ::SpiritShotLevelIdListIterator;
	for (LevelIterator iter = packet->GetFirstSpiritShotLevelIdListIterator();
		iter != packet->GetLastSpiritShotLevelIdListIterator(); ++iter)
	{
		spiritShotLevelIdList.Emplace(iter->SpiritShotLevelId());
	}
	
	const FGsSpiritShotMessageParamInfo messageParam(spiritShotLevelIdList);
	FGsSpiritShotManager* spiritShotManager = GSSpiritShot();
	if (nullptr != spiritShotManager)
	{
		spiritShotManager->SpiritShotInfo(&messageParam);
	}
	messageParam.Send();
}

void FGsNetMessageHandlerSpiritShot::ACK_SPIRIT_SHOT_ENCHANT(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_SPIRIT_SHOT_ENCHANT_READ* packet = reinterpret_cast<PD::SC::PKT_SC_ACK_SPIRIT_SHOT_ENCHANT_READ*>(InPacket.Buffer);
	if (nullptr == packet)
	{
		GSLOG(Error, TEXT("nullptr == packet"));
		return;
	}
	
	const PD::Result currentResult = StaticCast<PD::Result>(packet->Result());
	bool packetNoError = (PACKET_RESULT_SUCCESS == currentResult);
	if (!packetNoError)
	{
		HandleErrorPacket(currentResult);
	}

	TArray<ItemIdAmountPair> itemIdAmountPairList;
	packet->MakeFailRewardItemsVector<TArray<ItemIdAmountPair>, ItemIdAmountPair>(itemIdAmountPairList);

	const FGsSpiritShotMessageParamAckEnchant messageParam(packetNoError, packet->SpiritShotCategoryId(), packet->SpiritShotLevelId(), itemIdAmountPairList);

	// itemIdAmountPairList (실패 보상 데이터) 출력
#if WITH_EDITOR
	for (const ItemIdAmountPair& pair : itemIdAmountPairList)
	{
		GSLOG(Log, TEXT("FailReward!  ItemId : %d, ItemAmount : %d"), pair.mItemId, pair.mItemAmount);
	}
#endif

	FGsSpiritShotManager* spiritShotManager = GSSpiritShot();
	if (nullptr != spiritShotManager)
	{
		spiritShotManager->AckEnchant(&messageParam);
	}
	messageParam.Send();
}

void FGsNetMessageHandlerSpiritShot::HandleErrorPacket(const PD::Result InResult) const
{
	// NetText 테이블에서 사용할 티커값 조회
	FText errorText;

	FString key = FString::Format(TEXT("{0}"), { PD::ResultEnumToDisplayKey(static_cast<PD::Result>(InResult)) });
	FText::FindText(TEXT("NetText"), key, errorText);

	// NetText 를 사용했는데도 에러 메시지를 찾을 수 없다면
	if (errorText.IsEmpty())
	{
		FGsUIHelper::PopupNetError(InResult);
	}
	else
	{
		FGsUIHelper::TrayMessageTicker(errorText);
	}
}

