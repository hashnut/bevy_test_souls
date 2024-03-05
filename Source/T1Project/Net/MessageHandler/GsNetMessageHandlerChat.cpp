#include "GsNetMessageHandlerChat.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "Management/GsMessageHolder.h"
#include "Shared/Client/SharedEnums/SharedChatEnum.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Item/GsSchemaItemEffectSpawnTreasureMonster.h"
#include "DataSchema/Item/GsSchemaItemConsumeDetail.h"
#include "UTIL/GsGameObjectUtil.h"

using namespace PD::SC;
using namespace PD::GC;

void FGsNetMessageHandlerChat::InitializeMessage(FGsNetManager* inManager)
{
	MProtocalWorld& worldProtocal = inManager->GetProtocalWorld();
	InitializeMessageDelegateList(&worldProtocal);

	AddMessageDelegate(worldProtocal.AddRaw(SCPacketId::SC_CHAT,
		this, &FGsNetMessageHandlerChat::NotifyWorldServerChat));

	AddMessageDelegate(worldProtocal.AddRaw(SCPacketId::SC_SYSTEM_MESSAGE_NOTICE,
		this, &FGsNetMessageHandlerChat::NotifySystemMessage));

	AddMessageDelegate(worldProtocal.AddRaw(SCPacketId::SC_SYSTEM_MESSAGE_ITEM_GET_DROP,
		this, &FGsNetMessageHandlerChat::NotifySystemMessageItemGetDrop));

	AddMessageDelegate(worldProtocal.AddRaw(SCPacketId::SC_SYSTEM_MESSAGE_ITEM_GET_BOX,
		this, &FGsNetMessageHandlerChat::NotifySystemMessageItemGetBox));

	AddMessageDelegate(worldProtocal.AddRaw(SCPacketId::SC_SYSTEM_MESSAGE_ITEM_ENCHANT,
		this, &FGsNetMessageHandlerChat::NotifySystemMessageItemEnchant));

	AddMessageDelegate(worldProtocal.AddRaw(SCPacketId::SC_SYSTEM_MESSAGE_FAIRY_GET,
		this, &FGsNetMessageHandlerChat::NotifySystemMessageFairyGet));

	AddMessageDelegate(worldProtocal.AddRaw(SCPacketId::SC_SYSTEM_MESSAGE_COSTUME_GET,
		this, &FGsNetMessageHandlerChat::NotifySystemMessageCostumeGet));

	AddMessageDelegate(worldProtocal.AddRaw(SCPacketId::SC_SYSTEM_MESSAGE_ITEM_GET_FIELD_BOSS,
		this, &FGsNetMessageHandlerChat::NotifySystemMessageFieldBossItemGet));

	AddMessageDelegate(worldProtocal.AddRaw(SCPacketId::SC_SYSTEM_MESSAGE_ITEM_GET_DROP_ON_PARTY,
		this, &FGsNetMessageHandlerChat::NotifySystemMessageItemGetDropOnParty));

	AddMessageDelegate(worldProtocal.AddRaw(SCPacketId::SC_SPAWN_TREASURE_NPC_TICKER,
		this, &FGsNetMessageHandlerChat::NotifySystemMessageTresasureNPC));

	//emotion
	AddMessageDelegate(worldProtocal.AddRaw(SCPacketId::SC_USER_SOCIAL_EMOTION_LIST,
		this, &FGsNetMessageHandlerChat::NotifySocialEmotionList));

	AddMessageDelegate(worldProtocal.AddRaw(SCPacketId::SC_USER_SOCIAL_EMOTION_ACTIVE,
		this, &FGsNetMessageHandlerChat::AckSocialEmotionActive));

	AddMessageDelegate(worldProtocal.AddRaw(SCPacketId::SC_ACK_USER_SOCIAL_EMOTION, 
		this, &FGsNetMessageHandlerChat::SocialEmotionAck));

	AddMessageDelegate(worldProtocal.AddRaw(SCPacketId::SC_ACK_CHECK_USER_EXIST,
		this, &FGsNetMessageHandlerChat::AckInterWorldCheckUserExist));


	//GW Server에서 오는 Packet. but client 흐름상은(world에서만 처리) GW Handler가 처리하기에는 맞지않아, 여기서 처리
	MProtocalGateway& gatewayProtocol = inManager->GetProtocalGateway();

	_gateWayPackets.Add(gatewayProtocol.AddRaw(GCPacketId::GC_INVADE_CHAT_INFO,
		this, &FGsNetMessageHandlerChat::NotifyInvasionChatInfo));

	_gateWayPackets.Add(gatewayProtocol.AddRaw(GCPacketId::GC_CHAT,
		this, &FGsNetMessageHandlerChat::NotifyGateWayChat)); //일반 챗

	_gateWayPackets.Add(gatewayProtocol.AddRaw(GCPacketId::GC_CHAT_WHISPER,
		this, &FGsNetMessageHandlerChat::NotifyGateWayWhisper)); //귓속말

	_gateWayPackets.Add(gatewayProtocol.AddRaw(GCPacketId::GC_GUILD_CHAT_LIST_RECENT,
		this, &FGsNetMessageHandlerChat::NotifyGateWayGuildChatList));

	_gateWayPackets.Add(gatewayProtocol.AddRaw(GCPacketId::GC_ACK_CHAT,
		this, &FGsNetMessageHandlerChat::AckGateWayChat));

	_gateWayPackets.Add(gatewayProtocol.AddRaw(GCPacketId::GC_ACK_CHAT_WHISPER_V2,
		this, &FGsNetMessageHandlerChat::AckGateWayChatWhisper));

	_gateWayPackets.Add(gatewayProtocol.AddRaw(GCPacketId::GC_ACK_GUILD_CHAT_LIST, 
		this, &FGsNetMessageHandlerChat::AckGateWayGuildChatList));


	_gateWayPackets.Add(gatewayProtocol.AddRaw(GCPacketId::GC_ACK_REPORT_BAD_CHAT, 
		this, &FGsNetMessageHandlerChat::AckGateWayChatReport));

	_gateWayPackets.Add(gatewayProtocol.AddRaw(GCPacketId::GC_CHAT_BLACKLIST, 
		this, &FGsNetMessageHandlerChat::NotifyGateWayChatBlockList));

	_gateWayPackets.Add(gatewayProtocol.AddRaw(GCPacketId::GC_CHAT_WHISPER_LIST_OFF_LINE_V2,
		this, &FGsNetMessageHandlerChat::NotifyGateWayOffLineWhisperList));

	_gateWayPackets.Add(gatewayProtocol.AddRaw(GCPacketId::GC_ACK_CHAT_BLACKLIST_ADD,
		this, &FGsNetMessageHandlerChat::AckGateWayAddChatBlockList));

	_gateWayPackets.Add(gatewayProtocol.AddRaw(GCPacketId::GC_ACK_CHAT_BLACKLIST_REMOVE,
		this, &FGsNetMessageHandlerChat::AckGateWayRemoveChatBlockList));

	_gateWayPackets.Add(gatewayProtocol.AddRaw(GCPacketId::GC_ACK_CHECK_USER_EXIST,
		this, &FGsNetMessageHandlerChat::AckGateWayCheckUserExist));

	_gateWayPackets.Add(gatewayProtocol.AddRaw(GCPacketId::GC_NOTICE, 
		this, &FGsNetMessageHandlerChat::NotifyGateWayOperatorMessage));
}

void FGsNetMessageHandlerChat::FinalizeMessage()
{
	MProtocalGateway& gatewayProtocol = GNet()->GetProtocalGateway();
	for(auto& e : _gateWayPackets)
		gatewayProtocol.Remove(e);
	_gateWayPackets.Empty();

	FinalizeMessageDelegateList();
}


void  FGsNetMessageHandlerChat::NotifySystemMessage(const FGsNet::Packet& InPacket)
{
	if (PKT_SC_SYSTEM_MESSAGE_NOTICE_READ* pkt = reinterpret_cast<PKT_SC_SYSTEM_MESSAGE_NOTICE_READ*>(InPacket.Buffer))
	{
		if (auto chatManager = GSChat())
		{
			chatManager->OnSystemChat(pkt->Message(), TEXT("SYSTEM"), pkt->MessageTime());
		}
	}
}

void  FGsNetMessageHandlerChat::NotifySystemMessageItemGetDrop(const FGsNet::Packet& InPacket)
{
	if (PKT_SC_SYSTEM_MESSAGE_ITEM_GET_DROP_READ* pkt = reinterpret_cast<PKT_SC_SYSTEM_MESSAGE_ITEM_GET_DROP_READ*>(InPacket.Buffer))
	{
		if (auto chatManager = GSChat())
		{
			chatManager->OnNotiMessage(TEXT(""), pkt->UserName(), pkt->MessageTime(),
				SystemMessageType::ITEM_GET_DROP, { pkt->GainItemId(), uint32(pkt->MapId()) });
		}
	}
}

void  FGsNetMessageHandlerChat::NotifySystemMessageItemGetBox(const FGsNet::Packet& InPacket)
{
	if (PKT_SC_SYSTEM_MESSAGE_ITEM_GET_BOX_READ* pkt = reinterpret_cast<PKT_SC_SYSTEM_MESSAGE_ITEM_GET_BOX_READ*>(InPacket.Buffer))
	{
		if (auto chatManager = GSChat())
		{
			chatManager->OnNotiMessage(TEXT(""), pkt->UserName(), pkt->MessageTime(),
				SystemMessageType::ITEM_GET_BOX, { pkt->GainItemId(), pkt->BoxItemId() });
		}
	}
}

void  FGsNetMessageHandlerChat::NotifySystemMessageItemEnchant(const FGsNet::Packet& InPacket)
{
	if (PKT_SC_SYSTEM_MESSAGE_ITEM_ENCHANT_READ* pkt = reinterpret_cast<PKT_SC_SYSTEM_MESSAGE_ITEM_ENCHANT_READ*>(InPacket.Buffer))
	{
		if (auto chatManager = GSChat())
		{
			chatManager->OnNotiMessage(TEXT(""), pkt->UserName(), pkt->MessageTime(),
				SystemMessageType::ITEM_ENCHANT, { pkt->ItemId(), uint32(pkt->Level()) });
		}
	}
}

void  FGsNetMessageHandlerChat::NotifySystemMessageFairyGet(const FGsNet::Packet& InPacket)
{
	if (PKT_SC_SYSTEM_MESSAGE_FAIRY_GET_READ* pkt = reinterpret_cast<PKT_SC_SYSTEM_MESSAGE_FAIRY_GET_READ*>(InPacket.Buffer))
	{
		if (auto chatManager = GSChat())
		{
			chatManager->OnNotiMessage(TEXT(""), pkt->UserName(), pkt->MessageTime(),
				SystemMessageType::FAIRY_GET, { pkt->FairyId() });
		}
	}
}

void  FGsNetMessageHandlerChat::NotifySystemMessageCostumeGet(const FGsNet::Packet& InPacket)
{
	if (PKT_SC_SYSTEM_MESSAGE_COSTUME_GET_READ* pkt = reinterpret_cast<PKT_SC_SYSTEM_MESSAGE_COSTUME_GET_READ*>(InPacket.Buffer))
	{
		if (auto chatManager = GSChat())
		{
			chatManager->OnNotiMessage(TEXT(""), pkt->UserName(), pkt->MessageTime(),
				SystemMessageType::COSTUME_GET, { pkt->CostumeId() });
		}
	}
}


void FGsNetMessageHandlerChat::NotifySystemMessageFieldBossItemGet(const FGsNet::Packet& inPacket)
{
	if(PKT_SC_SYSTEM_MESSAGE_ITEM_GET_FIELD_BOSS_READ* pkt = reinterpret_cast<PKT_SC_SYSTEM_MESSAGE_ITEM_GET_FIELD_BOSS_READ*>(inPacket.Buffer))
	{
		if (auto chatManager = GSChat())
		{
			chatManager->OnNotiMessage(TEXT(""), pkt->UserName(), pkt->MessageTime(),
				SystemMessageType::ITEM_GET_FIELD_BOSS, { pkt->ItemId(), pkt->BossId() });
		}
	}
}

void FGsNetMessageHandlerChat::NotifySystemMessageItemGetDropOnParty(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_SYSTEM_MESSAGE_ITEM_GET_DROP_ON_PARTY_READ* pkt = reinterpret_cast<PKT_SC_SYSTEM_MESSAGE_ITEM_GET_DROP_ON_PARTY_READ*>(inPacket.Buffer))
	{
		if (auto chatManager = GSChat())
		{
			chatManager->OnNotiMessage(TEXT(""), pkt->UserName(), pkt->MessageTime(),
				SystemMessageType::ITEM_GET_DROP_ON_PARTY, { pkt->GainItemId() });
		}
	}
}

void FGsNetMessageHandlerChat::NotifySystemMessageTresasureNPC(const FGsNet::Packet& inPacket)
{

	static auto GetTreasureMonsterName = [](int64 monsterEggId) {

		FText nameText;

		if (auto data = FGsItem::GetTableDataByID(monsterEggId))
		{
			nameText = data->name;
			if (const auto consumeDetail = UGsTableUtil::FindRowById<UGsTableItemConsumeDetail, FGsSchemaItemConsumeDetail>(data->detailId))
			{
				if (consumeDetail->effectIDList.IsValidIndex(0))
				{
					if (const auto treasureMonster = UGsTableUtil::FindRowById<UGsTableItemEffectSpawnTreasureMonster, FGsSchemaItemEffectSpawnTreasureMonster>(consumeDetail->effectIDList[0]))
					{
						nameText = UGsGameObjectUtil::GetNpcName(treasureMonster->npcId);
					}
				}
			}
		}
		return nameText;
	};



	if (PKT_SC_SPAWN_TREASURE_NPC_TICKER_READ* pkt = reinterpret_cast<PKT_SC_SPAWN_TREASURE_NPC_TICKER_READ*>(inPacket.Buffer))
	{
		if (auto chatManager = GSChat())
		{
			// TEXT:{0} 님이 <Grade>{1} 등급 보물 몬스터</>를 {2}에 소환하였습니다.
			FText textFormat;
			if (FText::FindText(TEXT("SystemChatText"), TEXT("SystemChat_Summon_TreasureMonster"), textFormat))
			{
				ItemId itemId = pkt->ItemId();
				auto nameText = GetTreasureMonsterName(itemId);

				if (auto data = FGsItem::GetTableDataByID(itemId))
				{
					FString strMsg = textFormat.ToString();
					if (strMsg.Find(TEXT("<Grade>")) != INDEX_NONE)
					{
						switch (data->grade)
						{
						case ItemGrade::NORMAL: 
							strMsg = strMsg.Replace(TEXT("<Grade>"), TEXT("<GRADE_NORMAL>"), ESearchCase::CaseSensitive);
							break;
						case ItemGrade::MAGIC:
							strMsg = strMsg.Replace(TEXT("<Grade>"), TEXT("<GRADE_MAGIC>"), ESearchCase::CaseSensitive);
							break;
						case ItemGrade::RARE:
							strMsg = strMsg.Replace(TEXT("<Grade>"), TEXT("<GRADE_RARE>"), ESearchCase::CaseSensitive);
							break;
						case ItemGrade::EPIC:
							strMsg = strMsg.Replace(TEXT("<Grade>"), TEXT("<GRADE_EPIC>"), ESearchCase::CaseSensitive);
							break;
						case ItemGrade::UNIQUE:
							strMsg = strMsg.Replace(TEXT("<Grade>"), TEXT("<GRADE_UNIQUE>"), ESearchCase::CaseSensitive);
							break;
						case ItemGrade::LEGEND:
							strMsg = strMsg.Replace(TEXT("<Grade>"), TEXT("<GRADE_LEGEND>"), ESearchCase::CaseSensitive);
							break;
						case ItemGrade::ETERNAL:
							strMsg = strMsg.Replace(TEXT("<Grade>"), TEXT("<GRADE_ETERNAL>"), ESearchCase::CaseSensitive);
							break;
						default:
							strMsg = strMsg.Replace(TEXT("<Grade>"), TEXT("<GRADE_NORMAL>"), ESearchCase::CaseSensitive);
							break;
						}
					}
					//FText mapName;
					//if (auto mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(pkt->MapId()))
					//	mapName = mapData->MapStringId;

					FText markupText = UGsChatManager::TextPostScriptParser(strMsg, { nameText });
					chatManager->OnSystemChat(markupText.ToString(), TEXT("SYSTEM"), pkt->MessageTime());
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, markupText, TrayCondition::TC_Close);
				}
			}
		}
	}
}

void FGsNetMessageHandlerChat::NotifyGateWayOperatorMessage(const FGsNet::Packet& inPacket)
{
	if (PKT_GC_NOTICE_READ* pkt = reinterpret_cast<PKT_GC_NOTICE_READ*>(inPacket.Buffer))
	{
		auto type = pkt->NoticeType();
		auto noticeMsg = pkt->Notice();

		if(auto chatManager = GSChat())
			chatManager->OnOperatingMessage(noticeMsg, type, TEXT("SYSTEM"));

		FGsUIHelper::TrayOperatorMessageTicker(type, FText::FromString(noticeMsg));
	}
}

void FGsNetMessageHandlerChat::NotifyInvasionChatInfo(const FGsNet::Packet& InPacket)
{
	if (PKT_GC_INVADE_CHAT_INFO_READ* pkt = reinterpret_cast<PKT_GC_INVADE_CHAT_INFO_READ*>(InPacket.Buffer))
	{
		GSChat()->SetIsValidInvasionChat(pkt->IsChatAvailable());
		GSChat()->SetRemainFreeInvasionChat(pkt->FreeChatRemainCount(), false);
	}
}

void FGsNetMessageHandlerChat::NotifyGateWayChat(const FGsNet::Packet& inPacket)
{
	if (PKT_GC_CHAT_READ* pkt = reinterpret_cast<PKT_GC_CHAT_READ*>(inPacket.Buffer))
	{
		if (auto chatManager = GSChat())
		{
			if (pkt->ChatType() == ChatType::EMOTICON)
			{
				FString emojiTxt = pkt->ChatMessage();
				if (emojiTxt.IsNumeric())
				{
					int32 emojiIndex = FCString::Atoi(*emojiTxt);
					chatManager->OnEmoticonChat(pkt->GameId(), pkt->UserName(), emojiIndex);
				}
			}
			else
			{
				GSLOG(Log, TEXT("GC_CHAT type[%d] : %s"), (int)pkt->ChatType(), *(pkt->ChatMessage()));

				FGsChat chat{ FGsChat::ChangeType(pkt->ChatType()), pkt->GameId(), pkt->TimeNow(), pkt->UserName(), pkt->ChatMessage(),
					pkt->HomeWorldId(), pkt->HomeWorldPrefix(), pkt->AccountGrade(), pkt->UserRank(), 0, pkt->UserName() };
				chatManager->OnCommonChat(chat);
			}
		}
	}
}

void FGsNetMessageHandlerChat::NotifyGateWayWhisper(const FGsNet::Packet& inPacket)
{
	if (PKT_GC_CHAT_WHISPER_READ* pkt = reinterpret_cast<PKT_GC_CHAT_WHISPER_READ*>(inPacket.Buffer))
	{
		if (auto chatManager = GSChat())
		{
			GSLOG(Log, TEXT("SC_WHISPER from %s to %s : %s"), *(pkt->SenderUserName()), *(pkt->ReceiverUserName()), *(pkt->Message()));

			int64 gameId = 0;
			FString userName = pkt->SenderUserName();
			WorldId homeWorldId = pkt->WorldId();
			if (auto userData = GGameData()->GetUserData())
			{
				if (userData->mUserName.Equals(pkt->SenderUserName()))
				{
					gameId = userData->mGameId;
					userName = pkt->ReceiverUserName();
					homeWorldId = userData->mHomeWorldId;
				}
			}

			FGsChat chat{ EGsExChatType::WHISPER, gameId, pkt->TimeNow(), userName, pkt->Message(), homeWorldId, TEXT(""), pkt->AccountGrade(), pkt->UserRank(), 0, pkt->SenderUserName() };
			chatManager->OnCommonChat(chat);
		}
	}
}

void FGsNetMessageHandlerChat::NotifyGateWayGuildChatList(const FGsNet::Packet& inPacket)
{
	if (PKT_GC_GUILD_CHAT_LIST_RECENT_READ* pkt = reinterpret_cast<PKT_GC_GUILD_CHAT_LIST_RECENT_READ*>(inPacket.Buffer))
	{
		auto guildManager = GGuild();
		check(guildManager);
		guildManager->SetGuildChatList(*pkt);

		if (auto chatManager = GSChat())
		{
			chatManager->OnRawGuildChattoItem(guildManager->GetChatListRawData());
			guildManager->ResetRawChatList();
		}
	}
}

void FGsNetMessageHandlerChat::AckGateWayChat(const FGsNet::Packet& inPacket)
{
	if (PKT_GC_ACK_CHAT_READ* pkt = reinterpret_cast<PKT_GC_ACK_CHAT_READ*>(inPacket.Buffer))
	{
		auto result = pkt->Result();
		if (result == PD::Result::RESULT_SUCCESS)
		{
			if (pkt->ChatType() == ChatType::INVADE && pkt->IsUseCurrency() == false)
			{
				GSChat()->SetConsumedFreeInvasionChat();
			}
		}
		else if (result == PD::Result::CHAT_ERROR_DETECTED_AS_SPAM)
		{
			GSChat()->SetBannedSpammingChat(pkt->SpamPenaltyExpireTime(), 0);
		}
		else if (result == PD::Result::CHAT_ERROR_USER_BANNED)
		{
			GSChat()->SetBannedSpammingChat(0, pkt->ChatBanExpireTime());
		}
		else if (result == PD::Result::CHAT_ERROR_EXCEED_FREE_INVADE_CHAT)
		{
			//침공 채팅 무료 횟수를 초과 했습니다.
		}
		else if (result == PD::Result::CHAT_ERROR_RESET_FREE_INVADE_CHAT)
		{
			//침공 채팅 무료 횟수가 초기화 되었습니다
		}

	}
}

void FGsNetMessageHandlerChat::AckGateWayChatWhisper(const FGsNet::Packet& inPacket)
{
	if (PKT_GC_ACK_CHAT_WHISPER_V2_READ* pkt = reinterpret_cast<PKT_GC_ACK_CHAT_WHISPER_V2_READ*>(inPacket.Buffer))
	{
		auto result = pkt->Result();
		if (result != PACKET_RESULT_SUCCESS)
		{
			if (result == PD::Result::CHAT_ERROR_DETECTED_AS_SPAM)
			{
				//도배 처리
				GSChat()->SetBannedSpammingChat(pkt->SpamPenaltyExpireTime());
			}
			else if (result == PD::Result::CHAT_ERROR_NOT_FOUND_USER) //대상이 존재하지 않습니다. 아이디를 다시 확인해 주세요.
			{
				FText errorText;
				FText::FindText(TEXT("ChatText"), TEXT("WhisperTicker2"), errorText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
			}
			else if (result == PD::Result::CHECK_USER_EXIST_ERROR_USER_NOT_EXIST_IN_WORLD)
			{
				FText errorText;
				FText::FindText(TEXT("ChatText"), TEXT("WhisperTicker3"), errorText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
			}
			else if (result == PD::Result::CHAT_ERROR_CAN_NOT_SEND_TO_ME)
			{
				FText errorText;
				FText::FindText(TEXT("ChatText"), TEXT("WhisperTicker1"), errorText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
			}
			else if (result == PD::Result::CHAT_ERROR_DETECTED_AS_SPAM)
			{
				GSChat()->SetBannedSpammingChat(pkt->SpamPenaltyExpireTime(), 0);
			}
			else if (result == PD::Result::CHAT_ERROR_USER_BANNED)
			{
				GSChat()->SetBannedSpammingChat(0, pkt->ChatBanExpireTime());
			}
			else
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}
		}
	}
}

void FGsNetMessageHandlerChat::AckGateWayGuildChatList(const FGsNet::Packet& inPacket)
{
	if (PKT_GC_ACK_GUILD_CHAT_LIST_READ* pkt = reinterpret_cast<PKT_GC_ACK_GUILD_CHAT_LIST_READ*>(inPacket.Buffer))
	{
		if (auto chatManager = GSChat())
		{
			chatManager->AckGateWayGuildChatList(pkt->Result());
		}
	}
}

void FGsNetMessageHandlerChat::NotifyGateWayChatBlockList(const FGsNet::Packet& inPacket)
{
	if (PKT_GC_CHAT_BLACKLIST_READ* pkt = reinterpret_cast<PKT_GC_CHAT_BLACKLIST_READ*>(inPacket.Buffer))
	{
		if(pkt->Validate() && GGameData())
			GGameData()->Set(*pkt);
	}
}

void FGsNetMessageHandlerChat::NotifyGateWayOffLineWhisperList(const FGsNet::Packet& inPacket)
{
	if (PKT_GC_CHAT_WHISPER_LIST_OFF_LINE_V2_READ* pkt = reinterpret_cast<PKT_GC_CHAT_WHISPER_LIST_OFF_LINE_V2_READ*>(inPacket.Buffer))
	{
		if (auto chatManager = GSChat())
		{
			TArray<FGsChat> chatList;

			auto userData = GGameData()->GetUserData();

			auto count = pkt->GetWhisperChatListCount();
			auto iter = pkt->GetFirstWhisperChatListIterator();
			auto enditer = pkt->GetLastWhisperChatListIterator();
			for (; iter != enditer; ++iter)
			{
				int64 gameId = 0;
				FString userName = iter->SenderUserName();
				if (userData->mUserName.Equals(iter->SenderUserName()))
				{
					gameId = userData->mGameId;
					userName = iter->ReceiverUserName();
				}

				chatList.Add(FGsChat{ EGsExChatType::WHISPER, 0, iter->TimeNow(), userName, iter->Message(), 
					iter->WorldId(), TEXT(""), iter->AccountGrade(), iter->UserRank(), 0, iter->SenderUserName()});
			}
			chatManager->OnCommonChatList(ChatType::WHISPER, chatList);
		}
	}
}

void FGsNetMessageHandlerChat::AckGateWayAddChatBlockList(const FGsNet::Packet& InPacket)
{
	if (PKT_GC_ACK_CHAT_BLACKLIST_ADD_READ* pkt = reinterpret_cast<PKT_GC_ACK_CHAT_BLACKLIST_ADD_READ*>(InPacket.Buffer))
	{
		auto result = pkt->Result();
		if (result == PACKET_RESULT_SUCCESS)
		{
			GGameData()->Set(*pkt);

			if (auto chatManager = GSChat())
			{
				chatManager->OnAddedBlockList(pkt->UserDBId(), pkt->UserName());
			}
		}
		else
		{
			if (result == PD::Result::CHAT_BLACKLIST_ADD_ERROR_DB ||
				result == PD::Result::CHAT_BLACKLIST_REMOVE_ERROR_DB) // DB 추가 실패 ChatTicker9.
			{
				FText errorText;
				FText::FindText(TEXT("ChatText"), TEXT("ChatTicker9"), errorText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
			}
			else if (result == PD::Result::CHAT_BLACKLIST_ADD_ERROR_DUPLICATION)
			{
				FText errorText;
				FText::FindText(TEXT("ChatText"), TEXT("ChatTicker7"), errorText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
			}
			else if (result == PD::Result::CHAT_BLACKLIST_ADD_ERROR_UNKNOWN_USER)
			{
				FText errorText;
				FText::FindText(TEXT("ChatText"), TEXT("ChatTicker8"), errorText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
			}
			else if (result == PD::Result::CHAT_BLACKLIST_ADD_ERROR_OVER)
			{
				FText errorText;
				FText::FindText(TEXT("ChatText"), TEXT("ChatTicker6"), errorText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
			}
			else
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}
		}
	}
}

void FGsNetMessageHandlerChat::AckGateWayRemoveChatBlockList(const FGsNet::Packet& InPacket)
{
	if (PKT_GC_ACK_CHAT_BLACKLIST_REMOVE_READ* pkt = reinterpret_cast<PKT_GC_ACK_CHAT_BLACKLIST_REMOVE_READ*>(InPacket.Buffer))
	{
		auto result = pkt->Result();
		if (result == PACKET_RESULT_SUCCESS)
		{
			GGameData()->Set(*pkt);

			if (auto chatManager = GSChat())
			{
				chatManager->OnRemovedBlockList(pkt->UserDBId());
			}
		}
		else
		{
			if (result == PD::Result::CHAT_BLACKLIST_ADD_ERROR_DB ||
				result == PD::Result::CHAT_BLACKLIST_REMOVE_ERROR_DB) // DB 삭제 실패 ChatTicker9.
			{
				FText errorText;
				FText::FindText(TEXT("ChatText"), TEXT("ChatTicker9"), errorText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
			}
			else
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}
		}
	}
}

void FGsNetMessageHandlerChat::AckGateWayCheckUserExist(const FGsNet::Packet& InPacket)
{
	if (PKT_GC_ACK_CHECK_USER_EXIST_READ* pkt = reinterpret_cast<PKT_GC_ACK_CHECK_USER_EXIST_READ*>(InPacket.Buffer))
	{
		auto result = pkt->Result();
		if (result == PACKET_RESULT_SUCCESS)
		{
			GSChat()->OnCheckUserExistByName(true);
		}
		else
		{
			if (result == PD::Result::CHECK_USER_EXIST_ERROR_USER_NAME_EMPTY ||
				result == PD::Result::CHECK_USER_EXIST_ERROR_USER_NAME_LENGTH )
			{
				FGsUIHelper::TraySectionMessageTickerNetError(static_cast<PD::Result>(result));
			}
			else if (result == PD::Result::CHECK_USER_EXIST_ERROR_USER_NOT_EXIST)
			{
				FText errorText;
				FText::FindText(TEXT("ChatText"), TEXT("WhisperTicker2"), errorText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
			}
			else if (result == PD::Result::CHECK_USER_EXIST_ERROR_USER_NOT_EXIST_IN_WORLD)
			{
				FText errorText;
				FText::FindText(TEXT("ChatText"), TEXT("WhisperTicker3"), errorText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
			}
			else
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}

			GSChat()->OnCheckUserExistByName(false);
		}
	}

}

void FGsNetMessageHandlerChat::NotifyWorldServerChat(const FGsNet::Packet& InPacket)
{
	if (PKT_SC_CHAT_READ* pkt = reinterpret_cast<PKT_SC_CHAT_READ*>(InPacket.Buffer))
	{
		if (auto chatManager = GSChat())
		{
			if (pkt->ChatType() == ChatType::EMOTICON)
			{
				FString emojiTxt = pkt->ChatMessage();
				if (emojiTxt.IsNumeric())
				{
					int32 emojiIndex = FCString::Atoi(*emojiTxt);
					chatManager->OnEmoticonChat(pkt->GameId(), pkt->UserName(), emojiIndex);
				}
			}
			else
			{
				GSLOG(Log, TEXT("SC_CHAT type[%d] : %s"), (int)pkt->ChatType(), *(pkt->ChatMessage()));

				FGsChat chat{ FGsChat::ChangeType(pkt->ChatType()), pkt->GameId(), pkt->TimeNow(), pkt->UserName(), pkt->ChatMessage(),
					pkt->HomeWorldId(), pkt->HomeWorldPrefix(), pkt->AccountGrade(), pkt->UserRank(), 0, pkt->UserName() };
				chatManager->OnCommonChat(chat);
			}
		}
	}
}

void FGsNetMessageHandlerChat::AckGateWayChatReport(const FGsNet::Packet& InPacket)
{
	if (PKT_GC_ACK_REPORT_BAD_CHAT_READ* pkt = reinterpret_cast<PKT_GC_ACK_REPORT_BAD_CHAT_READ*>(InPacket.Buffer))
	{
		auto result = pkt->Result();
		if (result != PACKET_RESULT_SUCCESS)
		{
			if (result == PD::Result::REPORT_BAD_CHAT_ERROR_OVER) //하루에 신고 가능한 횟수를 초과했습니다.
			{
				FText errorText;
				FText::FindText(TEXT("ChatText"), TEXT("ChatTicker3"), errorText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
			}
			else if (result == PD::Result::REPORT_BAD_CHAT_ERROR_DB) //DB 추가 실패 ChatTicker9
			{
				FText errorText;
				FText::FindText(TEXT("ChatText"), TEXT("ChatTicker9"), errorText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
			}
			else if (result == PD::Result::REPORT_BAD_CHAT_ERROR_UNKNOWN_USER) //잘못 된 유저정보
			{
				FText errorText;
				FText::FindText(TEXT("ChatText"), TEXT("ChatTicker8"), errorText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
			}
			else
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}
		}
	}
}


void FGsNetMessageHandlerChat::NotifySocialEmotionList(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_USER_SOCIAL_EMOTION_LIST_READ* pkt = reinterpret_cast<PKT_SC_USER_SOCIAL_EMOTION_LIST_READ*>(inPacket.Buffer))
	{
		if (auto chatManager = GSChat())
		{
			TArray<SocialEmotionInfo> emotionList;
			pkt->MakeSocialEmotionInfoVector<TArray<SocialEmotionInfo>, SocialEmotionInfo>(emotionList);
			chatManager->UnlockSocialEmotionList(emotionList, false);
		}
	}
}

void FGsNetMessageHandlerChat::AckSocialEmotionActive(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_USER_SOCIAL_EMOTION_ACTIVE_READ* pkt = reinterpret_cast<PKT_SC_USER_SOCIAL_EMOTION_ACTIVE_READ*>(inPacket.Buffer))
	{
		if (auto chatManager = GSChat())
		{
			chatManager->OnUnlockSocialEmotion(pkt->UserSocialEmotion(), true); //패킷 수정이후 수정
		}
	}
}

void FGsNetMessageHandlerChat::SocialEmotionAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_ACK_USER_SOCIAL_EMOTION_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_USER_SOCIAL_EMOTION_READ*>(inPacket.Buffer))
	{
		if (PACKET_RESULT_SUCCESS == pkt->Result())
		{	
			GSChat()->UpdateSocialEmotion(pkt->GameId(), pkt->UserSocialEmotion());
		}
		else
		{
			FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		}
	}
}

void FGsNetMessageHandlerChat::AckInterWorldCheckUserExist(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_ACK_CHECK_USER_EXIST_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_CHECK_USER_EXIST_READ*>(inPacket.Buffer))
	{
		auto result = pkt->Result();
		if (result == PACKET_RESULT_SUCCESS)
		{
			GSChat()->OnCheckUserExistByName(true);
		}
		else
		{
			if (result == PD::Result::CHECK_USER_EXIST_ERROR_USER_NAME_EMPTY ||
				result == PD::Result::CHECK_USER_EXIST_ERROR_USER_NAME_LENGTH)
			{
				FGsUIHelper::TraySectionMessageTickerNetError(static_cast<PD::Result>(result));
			}
			else if (result == PD::Result::CHECK_USER_EXIST_ERROR_USER_NOT_EXIST)
			{
				FText errorText;
				FText::FindText(TEXT("ChatText"), TEXT("WhisperTicker2"), errorText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
			}
			else if (result == PD::Result::CHECK_USER_EXIST_ERROR_USER_NOT_EXIST_IN_WORLD)
			{
				FText errorText;
				FText::FindText(TEXT("ChatText"), TEXT("WhisperTicker3"), errorText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
			}
			else
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}

			GSChat()->OnCheckUserExistByName(false);
		}
	
	}
}