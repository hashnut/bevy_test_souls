#include "GsNetMessageHandlerBattlePass.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/GsMessageSystem.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsBattlePassManager.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Item/GsItemManager.h"
#include "Currency/GsCurrencyHelper.h"


void FGsNetMessageHandlerBattlePass::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalWorld& protocolWorld = inManager->GetProtocalWorld();

	InitializeMessageDelegateList(&protocolWorld);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_BATTLE_PASS,
		this, &FGsNetMessageHandlerBattlePass::OnNotifyBattlePassList)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_BATTLE_PASS_EXPOSE,
		this, &FGsNetMessageHandlerBattlePass::OnNotifyBattlePassExpose)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INFORM_BATTLE_PASS_PROGRESS,
		this, &FGsNetMessageHandlerBattlePass::OnNotifyUpdateBattlePassLevel)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INFORM_BATTLE_PASS_MISSION_PROGRESS,
		this, &FGsNetMessageHandlerBattlePass::OnNotifyBattlePassMissionProgress)
	);
	AddMessageDelegate(protocolWorld.AddRawPacket<PD::SC::PKT_SC_ACK_BATTLE_PASS_ACTIVATE_ADDITIONAL_REWARD_READ>(
		PD::SC::SCPacketId::SC_ACK_BATTLE_PASS_ACTIVATE_ADDITIONAL_REWARD, this, &FGsNetMessageHandlerBattlePass::OnAckActivateBattlePassAdditionalReward)
	);
	AddMessageDelegate(protocolWorld.AddRawPacket<PD::SC::PKT_SC_ACK_BATTLE_PASS_BUY_LEVEL_READ>(PD::SC::SCPacketId::SC_ACK_BATTLE_PASS_BUY_LEVEL,
		this, &FGsNetMessageHandlerBattlePass::OnAckBuyBattlePassLevel)
	);
	AddMessageDelegate(protocolWorld.AddRawPacket<PD::SC::PKT_SC_ACK_BATTLE_PASS_REWARD_READ>(PD::SC::SCPacketId::SC_ACK_BATTLE_PASS_REWARD,
		this, &FGsNetMessageHandlerBattlePass::OnAckReceiveBattlePassLevelReward)
	);
	AddMessageDelegate(protocolWorld.AddRawPacket<PD::SC::PKT_SC_ACK_BATTLE_PASS_REWARD_ALL_LIST_READ>(PD::SC::SCPacketId::SC_ACK_BATTLE_PASS_REWARD_ALL_LIST,
		this, &FGsNetMessageHandlerBattlePass::OnAckBattlePassRewardAllList)
	);
	AddMessageDelegate(protocolWorld.AddRawPacket<PD::SC::PKT_SC_ACK_BATTLE_PASS_REWARD_ALL_READ>(PD::SC::SCPacketId::SC_ACK_BATTLE_PASS_REWARD_ALL,
		this, &FGsNetMessageHandlerBattlePass::AckBattlePassReceiveLevelRewardAll)
	);
}

void FGsNetMessageHandlerBattlePass::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}


void FGsNetMessageHandlerBattlePass::OnNotifyBattlePassList(const FGsNet::Packet& inPacket)
{
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_BATTLE_PASS_READ*>(inPacket.Buffer))
	{
		TArray<BattlePassItem> battlePassList;

		auto lastIter = pkt->GetLastBattlePassInfoListIterator();
		for (auto iter = pkt->GetFirstBattlePassInfoListIterator(); iter != lastIter; iter++)
		{
			BattlePassItem item{ iter->BattlePassId(), iter->CurrentLevel(), iter->CurrentExp(), iter->IsOpenAdditionalReward() };
			//BattlePassItem item{ iter->BattlePassId(), 2, 100, iter->IsOpenAdditionalReward() }; //test

			const auto lastRewardIter = iter->GetLastRewardedListIterator();
			for (auto rewardIter = iter->GetFirstRewardedListIterator(); rewardIter != lastRewardIter; rewardIter++)
			{
				auto level = rewardIter->RewardedLevel();
				if (rewardIter->IsRewardedAdditional())
				{
					item.additionalRewardedLevels.Add(level);
				}
				if (rewardIter->IsRewardedBasic())
				{
					item.rewardedLevels.Add(level); // 나중에 레벨로...
				}
			}

			const auto lastMissionIter = iter->GetLastMissionListIterator();
			for (auto missionIter = iter->GetFirstMissionListIterator(); missionIter != lastMissionIter; missionIter++)
			{
				auto key = missionIter->BattlePassMissionTabId();
				if (item.passMissionIdPoints.Find(key) == nullptr)
				{
					TArray<TPair<BattlePassMissionId, uint64>> empty;
					item.passMissionIdPoints.Add(key, empty);
				}

				item.passMissionIdPoints[key].Add(TPair<BattlePassMissionId, uint64>{missionIter->BattlePassMissionId(), missionIter->CompletePoint() });
			}

			battlePassList.Add(item);
		}

		if(auto battlePassManager = GSBattlePass())
			battlePassManager->UpdateBattlePassList(battlePassList);
	}
}

void FGsNetMessageHandlerBattlePass::OnNotifyBattlePassExpose(const FGsNet::Packet& inPacket)
{
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_BATTLE_PASS_EXPOSE_READ*>(inPacket.Buffer))
	{
		BattlePassItem item{ pkt->BattlePassId(), 0, 0, false };
		GSBattlePass()->UpdateBattlePassData(item);
	}
}

void FGsNetMessageHandlerBattlePass::OnNotifyUpdateBattlePassLevel(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_INFORM_BATTLE_PASS_PROGRESS_READ*>(inPacket.Buffer))
	{
		GSBattlePass()->UpdateBattlePassLevel(pkt->BattlePassId(), pkt->BattlePassLevel(), pkt->BattlePassExp());
	}
}

void FGsNetMessageHandlerBattlePass::OnNotifyBattlePassMissionProgress(const FGsNet::Packet& inPacket)
{
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_INFORM_BATTLE_PASS_MISSION_PROGRESS_READ*>(inPacket.Buffer))
	{
		GSBattlePass()->UpdatePassMissionPoint(pkt->BattlePassId(), pkt->MissionTabId(), pkt->BattlePassMissionId(), pkt->CompletePoint());
	}
}

void FGsNetMessageHandlerBattlePass::OnAckActivateBattlePassAdditionalReward(const FGsNet::Packet& inPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&inPacket))
	{
		if (data->IsSuccess())
		{
			auto pkt = data->PacketCast<PD::SC::PKT_SC_ACK_BATTLE_PASS_ACTIVATE_ADDITIONAL_REWARD_READ>();
			GSBattlePass()->UpdatePassLevelAdditionalRewardUnlock(pkt->BattlePassId());
		}
	}
}

void FGsNetMessageHandlerBattlePass::OnAckBuyBattlePassLevel(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	if (const auto data = static_cast<const FGsAckPacket*>(&inPacket))
	{
		if (data->IsSuccess())
		{
			auto pkt = data->PacketCast<PD::SC::PKT_SC_ACK_BATTLE_PASS_BUY_LEVEL_READ>();
			GSBattlePass()->UpdateBattlePassLevel(pkt->BattlePassId(), pkt->CurrentLevel(), 0);
		}
	}
}

void FGsNetMessageHandlerBattlePass::OnAckReceiveBattlePassLevelReward(const FGsNet::Packet& inPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&inPacket))
	{
		if (data->IsSuccess())
		{
			auto pkt = data->PacketCast<PD::SC::PKT_SC_ACK_BATTLE_PASS_REWARD_READ>();
			GSBattlePass()->UpdateBattlePassLevelRewarded(pkt->BattlePassId(), pkt->RewardLevel(), pkt->IsAdditionalReward()); //나중에 레벨로

			FText findText;
			FText::FindText(TEXT("BattlePassText"), TEXT("UI_Notice_Success_GetReward"), findText); //보상이 인벤토리로 지급되었습니다.
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		}
	}
}

void FGsNetMessageHandlerBattlePass::OnAckBattlePassRewardAllList(const FGsNet::Packet& inPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&inPacket))
	{
		if (data->IsSuccess())
		{
			auto pkt = data->PacketCast<PD::SC::PKT_SC_ACK_BATTLE_PASS_REWARD_ALL_LIST_READ>();

			pkt->RewardCount();

			TArray<ItemIdAmountPair> itemAmountPairList;
			TArray<CurrencyDeltaPair> currencyPairList;
			pkt->MakeRewardItemIdAmountPairListVector<TArray<ItemIdAmountPair>, ItemIdAmountPair>(itemAmountPairList);
			pkt->MakeCurrencyDeltaVector<TArray<CurrencyDeltaPair>, CurrencyDeltaPair>(currencyPairList);

			GSBattlePass()->OnAckBattlePassRewardAllList(pkt->BattlePassId(), currencyPairList, itemAmountPairList);
		}
	}

}

void FGsNetMessageHandlerBattlePass::AckBattlePassReceiveLevelRewardAll(const FGsNet::Packet& inPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&inPacket))
	{
		if (data->IsSuccess())
		{
			auto pkt = data->PacketCast<PD::SC::PKT_SC_ACK_BATTLE_PASS_REWARD_ALL_READ>();
			BattlePassItem item{ pkt->BattlePassId(),0, 0, false };

			const auto lastRewardIter = pkt->GetLastRewardedListIterator();
			for (auto rewardIter = pkt->GetFirstRewardedListIterator(); rewardIter != lastRewardIter; rewardIter++)
			{
				auto level = rewardIter->RewardedLevel();
				if (rewardIter->IsRewardedAdditional())
				{
					item.additionalRewardedLevels.Add(level);
				}
				if (rewardIter->IsRewardedBasic())
				{
					item.rewardedLevels.Add(level); // 나중에 레벨로...
				}
			}
			GSBattlePass()->OnAckBattlePassReceiveLevelRewardAll(item);

			FText findText;
			FText::FindText(TEXT("BattlePassText"), TEXT("UI_Notice_Success_GetReward"), findText); //보상이 인벤토리로 지급되었습니다.
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		}
	}
}
