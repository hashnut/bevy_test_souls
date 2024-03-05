#include "GsNetMessageHandlerAchievement.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGame/GsAchievementManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Item/GsItemManager.h"
#include "Currency/GsCurrencyHelper.h"

using namespace PD::SC;
//PKT_SC_ACK_ACHIEVEMENT_REWARD_ALL_READ
#define ADDPACKET(PacketId, Object, Func) AddPacket<PKT_##PacketId##_READ>( PD::SC::SCPacketId::##PacketId, Object, Func)

void FGsNetMessageHandlerAchievement::InitializeMessage(FGsNetManager* inManager)
{
	MProtocalWorld& worldProtocal = inManager->GetProtocalWorld();
	InitializeMessageDelegateList(&worldProtocal);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACHIEVEMENT, this, &FGsNetMessageHandlerAchievement::NotifyAchievementList)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_INFORM_ACHIEVEMENT_PROGRESS, this, &FGsNetMessageHandlerAchievement::NotifyUpdateAchievementProgress)
	);

	AddMessageDelegate(worldProtocal.AddRawPacket<PD::SC::PKT_SC_ACK_ACHIEVEMENT_REWARD_READ>(
		SCPacketId::SC_ACK_ACHIEVEMENT_REWARD, this, &FGsNetMessageHandlerAchievement::AckAchievementReward)
	);

	AddMessageDelegate(worldProtocal.AddRawPacket<PD::SC::PKT_SC_ACK_ACHIEVEMENT_REWARD_ALL_LIST_READ>(
		SCPacketId::SC_ACK_ACHIEVEMENT_REWARD_ALL_LIST, this, &FGsNetMessageHandlerAchievement::AckAchievementRewardList)
	);
	AddMessageDelegate(worldProtocal.AddRawPacket<PD::SC::PKT_SC_ACK_ACHIEVEMENT_REWARD_ALL_READ>(
		SCPacketId::SC_ACK_ACHIEVEMENT_REWARD_ALL, this, &FGsNetMessageHandlerAchievement::AckAchievementReceiveRewardAll)
	);
}

void FGsNetMessageHandlerAchievement::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerAchievement::NotifyAchievementList(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_ACHIEVEMENT_READ* pkt = reinterpret_cast<PKT_SC_ACHIEVEMENT_READ*>(inPacket.Buffer))
	{
		TArray<AchievementItem> achievements;
		auto achieveLastIter = pkt->GetLastAchievementInfoListIterator();
		for (auto iter = pkt->GetFirstAchievementInfoListIterator(); iter != achieveLastIter; iter++)
		{
			achievements.Add(AchievementItem{ iter->AchievementId(), iter->CompletePoint(), iter->IsRewarded()});
		}

		GSAchivement()->OnUpdateAchievementList(achievements); //themes, 
	}
}

void FGsNetMessageHandlerAchievement::NotifyUpdateAchievementProgress(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_INFORM_ACHIEVEMENT_PROGRESS_READ* pkt = reinterpret_cast<PKT_SC_INFORM_ACHIEVEMENT_PROGRESS_READ*>(inPacket.Buffer))
	{
		GSAchivement()->OnUpdateAchievementProgress(pkt->AchievementId(), pkt->CompletePoint());
	}
}

void FGsNetMessageHandlerAchievement::AckAchievementReward(const FGsNet::Packet& inPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&inPacket))
	{
		if (data->IsSuccess())
		{
			auto pkt = data->PacketCast<PKT_SC_ACK_ACHIEVEMENT_REWARD_READ>();
			GSAchivement()->OnAckReceivedAchievementReward(pkt->AchievementId());
		}
		else
		{
			if (!data->IsProcessErrorUI())
			{
				auto result = data->Result();
				if (result == PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL)
				{
					FGsUIHelper::TraySectionMessageTickerNetError(PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL);
				}
				else
				{
					FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
				}
			}
		}
	}
}

void FGsNetMessageHandlerAchievement::AckAchievementRewardList(const FGsNet::Packet& inPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&inPacket))
	{
		if (PKT_SC_ACK_ACHIEVEMENT_REWARD_ALL_LIST_READ* pkt = data->PacketCast<PKT_SC_ACK_ACHIEVEMENT_REWARD_ALL_LIST_READ>())
		{
			pkt->RewardCount();

			TArray<ItemIdAmountPair> itemAmountPairList;
			TArray<CurrencyDeltaPair> currencyPairList;
			pkt->MakeRewardItemIdAmountPairListVector<TArray<ItemIdAmountPair>, ItemIdAmountPair>(itemAmountPairList);
			pkt->MakeCurrencyDeltaVector<TArray<CurrencyDeltaPair>, CurrencyDeltaPair>(currencyPairList);

			GSAchivement()->OnAckAchievementRewardList(currencyPairList, itemAmountPairList);
		}
	}
}

void FGsNetMessageHandlerAchievement::AckAchievementReceiveRewardAll(const FGsNet::Packet& inPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&inPacket))
	{
		if (PKT_SC_ACK_ACHIEVEMENT_REWARD_ALL_READ* pkt = data->PacketCast<PKT_SC_ACK_ACHIEVEMENT_REWARD_ALL_READ>())
		{
			auto result = pkt->Result();
			if (result == PACKET_RESULT_SUCCESS)
			{
				TArray<AchievementId> achievementIdList;
				TArray<AchievementThemeId> achievementThemeIdList;
				pkt->MakeRewardAchievementIdListVector<TArray<AchievementId>, AchievementId>(achievementIdList);

				GSAchivement()->OnAckReceivedRewardAll(achievementIdList); //achievementThemeIdList
			}
			else
			{
				if (result == PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL)
				{
					FText errorText;
					FText::FindText(TEXT("AchievementText"), TEXT("messageTicker_invenFull"), errorText);
					FString msg = (errorText.IsEmpty()) ? PD::ResultEnumToString(static_cast<PD::Result>(result)) : *(errorText.ToString());

					GSLOG(Error, TEXT("[Net] %s"), *msg);

					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, FText::FromString(msg));
				}
				else
				{
					FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
				}
			}
		}
	}
}

