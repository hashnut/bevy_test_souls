#include "GsChatManager.h"

#include "DataSchema/SocialEmotion/GsSchemaEmoji.h"
#include "DataSchema/SocialEmotion/GsSchemaSocialEmotion.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/SystemMessage/GsSchemaSystemMessage.h"
#include "DataSchema/Chat/GsSchemaChatConfig.h"
#include "DataSchema/Chat/GsSchemaChatTypeViewInfo.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsUIMessageParam.h"

#include "Net/GsNetSendServiceGuild.h"

#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"

#include "Item/GsItemManager.h"
#include "Costume/GsCostumeData.h"
#include "Fairy/GsFairyData.h"

#include "GameObject/ObjectClass/GsGameObjectPlayer.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Skill/IdleEmotion/GsIdleEmotionLocalRunner.h"

#include "UI/UIContent/Popup/GsUIPopupChat.h"

#include "Option/GsGameOptions.h"

#include "UTIL/GsTimeSyncUtil.h"
#include "UTIL/GsGameObjectUtil.h"
#include "UTIL/GsEnum.h"

#include "Algo/Copy.h"
#include "Algo/Transform.h"

ENABLE_ENUM_OPERATORS(EGsChatViewType)
ENABLE_ENUM_OPERATORS(EGsExChatType)

constexpr EGsChatViewType LastChatViewType = static_cast<EGsChatViewType>(static_cast<int>(EGsChatViewType::Max) - 1);
ENUM_RANGE_BY_FIRST_AND_LAST(EGsChatViewType, EGsChatViewType::ALL, LastChatViewType)

double UGsChatManager::_lastNotiGameTime = 0;
TArray<const TCHAR*> UGsChatManager::worldStartDelim = {
		TEXT("["),
		TEXT("]"),
		TEXT("("),
		TEXT(")"),
};

WorldId UGsChatManager::homeWorldId = INVALID_WORLD_ID;
void UGsChatManager::Initialize()
{
	if (const UGsGlobalConstant* globalData = GData()->GetGlobalData())
	{
		_viewTimeGameRatingSec = globalData->_gameRaitingIconViewSec;
	}

	InitChat();

	LoadSocialMotion();

	LoadChatInfo();

	CheckGameTime(true);
}

void UGsChatManager::Finalize()
{
	_socialMotions.Reset();

	ClearChat(EGsChatViewType::ALL);

	_chatPool.Flush();

	homeWorldId = INVALID_WORLD_ID;
	CheckGameTime(false);
}

bool UGsChatManager::CheckGameTime(bool isCheck)
{
	if (isCheck)
	{
#if !WITH_EDITOR
		auto curGameTime = FApp::GetGameTime();
		if (curGameTime - _lastNotiGameTime >= gameTimeCheckSec)
		{
			FText errorText;
			FText::FindText(TEXT("TitleSceneText"), TEXT("chat_digging"), errorText);

			OnOperatingMessage(errorText.ToString());

			FGsUIMsgParamFloat param = FGsUIMsgParamFloat(_viewTimeGameRatingSec);
			GMessage()->GetUI().SendMessage(MessageUI::PREVENTION_GAME_OVERINDULGENCE, &param);

			_lastNotiGameTime = curGameTime;
		}

		if (_gameTimeHandle.IsValid() == false)
		{
			GetWorld()->GetTimerManager().SetTimer(_gameTimeHandle, FTimerDelegate::CreateWeakLambda(this, [this]() {
				CheckGameTime(true);
				}), gameTimeCheckDelay, true, 0.f);
		}
#endif
		return true;
	}
	else
	{
#if !WITH_EDITOR
		GetWorld()->GetTimerManager().ClearTimer(_gameTimeHandle);
#endif
	}
	return false;
}

void UGsChatManager::InitChat()
{
	_lastInputChatType = EGsExChatType::NONE;
	_chatInputEnableTime = 0;
	_useBanTIme = false;
	_endOperatingBannedTime = 0;
	_lastInputEndTime = 0;
	_isRemovedGuildChat = false;
	_maxRcvGuildCount = guildChatMaxRcv;

	_hudChatType = EGsChatViewType::ALL;
	_groupChatID = 0;

	_chatTabList.Reset();
	_mngChatList.Reset();

	TEnumRange<EGsChatViewType> chatType;
	for (auto eType : chatType)
	{
		_chatTabList.Add(FGsChatList(eType));
	}

	if (auto guildManager = GGuild())
	{
		const TArray<FGsGuildChatList>& list = guildManager->GetChatListRawData();
		if (list.Num() > 0)
		{
			OnRawGuildChattoItem(list);
			guildManager->ResetRawChatList();
		}
	}
}

void UGsChatManager::ClearChat(EGsChatViewType chatType)
{
	if (chatType == EGsChatViewType::ALL)
	{
		Algo::ForEach(_chatTabList, [](auto& e) { e.Reset(); });

		_mngChatList.Reset();
	}
	else 
	{
		Algo::ForEachIf(_chatTabList, [chatType](auto& e) { return e._type == chatType; }, [](auto& e) { e.Reset(); });

		_mngChatList.RemoveAll([](auto e) { return !e.IsValid(); });
	}
}

TSharedPtr<FGsChatItem> UGsChatManager::AddChatItem(FGsChat chat, SystemMessageType optType, TArray<uint32> options)
{
	TSharedPtr<FGsChatItem> chatItem = _chatPool.GetItem();

	bool isImWhisperer = IsWhisperChatSentByMe(chat);
	bool isMyGuildChat = IsAllyGuildChatByMyGuild(chat);
	bool useWorldPrefix = chat._homeWorldId != homeWorldId;

	if (isImWhisperer)
	{
		chatItem->_isSentWhisper = true;
		chat._homeWorldId = _whisperWorldId;
	}

	chatItem->SetData(chat, [](FGsChatItem& item) { UGsChatManager::MakeChatMarkup(item); }, homeWorldId == chat._homeWorldId, isMyGuildChat);
	chatItem->SetSystemData(optType, options, [](FGsChatItem& item) { UGsChatManager::MakeSystemMessageMarkup(item); });

	_lastInputEndTime = chatItem->GetInputTime();

	if(chatItem->_viewTabs.Num() > 0)
	{
		_mngChatList.Add(chatItem);

		for (auto tab : chatItem->_viewTabs)
		{	
			if (const auto dataTable = Cast<UGsTableChatTabViewInfo>(FGsSchemaChatTabViewInfo::GetStaticTable()))
			{
				const FGsSchemaChatTabViewInfo* tabConfig = nullptr;
				if (dataTable->FindRowById(tab, tabConfig))
				{
					_chatTabList[intType(tab)].AddData(chatItem, tabConfig->useRoomId); //popup 전체 채팅에도 적용됨 R1
					const auto& list = _chatTabList[intType(tab)].ChatList(tabConfig->useRoomId ? chatItem->GetRoomId() : 0);

					if (list.Num() > chatTabMax)
					{
						_chatTabList[intType(tab)].RemoveData(list[0].Pin(), tabConfig->useRoomId);
					}
				}
			}
		}

		_mngChatList.RemoveAll([](auto e) { return !e.IsValid(); });
	}

	return chatItem;
}

TSharedPtr<FGsChatItem> UGsChatManager::InsertChatItem(FGsChat chat, SystemMessageType optType, TArray<uint32> options)
{
	TSharedPtr<FGsChatItem> chatItem = _chatPool.GetItem();

	bool isImWhisperer = IsWhisperChatSentByMe(chat);
	bool isMyGuildChat = IsAllyGuildChatByMyGuild(chat);
	bool useWorldPrefix = chat._homeWorldId != homeWorldId;

	if (isImWhisperer)
	{
		chatItem->_isSentWhisper = true;
		chat._homeWorldId = _whisperWorldId;
	}

	chatItem->SetData(chat, [](FGsChatItem& item) { UGsChatManager::MakeChatMarkup(item); }, homeWorldId == chat._homeWorldId, isMyGuildChat);
	chatItem->SetSystemData(optType, options, [](FGsChatItem& item) { UGsChatManager::MakeSystemMessageMarkup(item); });

	TArray<EGsChatViewType> addList;
	if (chatItem->_viewTabs.Num() > 0)
	{
		_mngChatList.Add(chatItem);

		if (TabViewArray(_chatTabList[intType(chatItem->_chatViewType)]).Num() == 0)
		{
			_lastInputEndTime = FGsTimeSyncUtil::GetServerNowSecond();
		}

		for (auto tab : chatItem->_viewTabs)
		{
			if (const auto dataTable = Cast<UGsTableChatTabViewInfo>(FGsSchemaChatTabViewInfo::GetStaticTable()))
			{
				const FGsSchemaChatTabViewInfo* tabConfig = nullptr;
				if (dataTable->FindRowById(tab, tabConfig))
				{
					const auto& list = _chatTabList[intType(tab)].ChatList(tabConfig->useRoomId ? chatItem->GetRoomId() : 0);
					int maxChat = list.Num();

					auto insertTYpe = chatItem->_type;
					int index = 0;
					if (chatItem->GetSeqNumber() > 0)
					{
						auto iter = list.CreateConstIterator();
						for (; iter && index < maxChat; iter++, index++)
						{
							auto seq = (*iter).Pin()->GetSeqNumber();
							if (seq > 0 && insertTYpe == (*iter).Pin()->_type)
							{
								if (chatItem->GetSeqNumber() <= seq)
									break;
							}
							else
							{
								if (chatItem->GetInputTime() <= (*iter).Pin()->GetInputTime())
									break;
							}
						}
					}
					else index = maxChat;

					if (index == maxChat)
						_chatTabList[intType(tab)].AddData(chatItem, tabConfig->useRoomId);
					else _chatTabList[intType(tab)].InsertData(chatItem, index, tabConfig->useRoomId);

					while (list.Num() > chatTabMax)
					{
						_chatTabList[intType(tab)].RemoveData(list[0].Pin(), tabConfig->useRoomId);
					}
				}
			}
			_mngChatList.RemoveAll([](auto e) { return !e.IsValid(); });
		}
	}

	return chatItem;
}

void UGsChatManager::OnCommonChatList(ChatType type, TArray<FGsChat> chatList)
{
	for (auto& chat : chatList)
	{
		OnCommonChat(chat);
	}
}

void UGsChatManager::OnCommonChat(FGsChat& chat)
{
	IsWhisperChatSentByMe(chat);
	if (chat._homeWorldId == homeWorldId && !IsNoneChatBlockUser(chat._senderName)) return;

	static TArray<EGsExChatType> checkChatType = { EGsExChatType::NORMAL
		, EGsExChatType::WORLD
		, EGsExChatType::PARTY
		, EGsExChatType::GROUP
		, EGsExChatType::GUILD
		, EGsExChatType::WHISPER
		, EGsExChatType::INVADE
		, EGsExChatType::INTERSERVER
	};

	if (chat._text.Contains(TEXT("\n")))
	{
		OnSystemChat(chat._text);
		return;
	}

	if (chat._type == EGsExChatType::NORMAL && OnCheatChat(chat._text, chat._text, chat._gameId)) return;

	if (checkChatType.Find(chat._type) != INDEX_NONE)
	{
		auto thiz = MakeWeakObjectPtr(this);
		GLocalization()->ChangeBannedSentence(chat._text, [chat, thiz](bool success, FString inText, FString outText) {

				if (thiz.IsValid())
				{
					FGsChat newChat = CopyTemp(chat);
					newChat._text = outText;

					if (auto chatItem = thiz->AddChatItem(newChat))
					{
						FGsUIMsgParamChatItem param = FGsUIMsgParamChatItem((uint16)newChat._type, chatItem, thiz->GetChatViewOption(chatItem->_type, chatItem->GetRoomId()));
						GMessage()->GetChat().SendMessage(MessageContentChat::ADD, &param);
						GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CHAT);

						if (newChat._type == EGsExChatType::NORMAL && thiz->GetChatViewOption(EGsOptionChat::TALK_POPUP))
						{
							if (UGsGameObjectPlayer* player = Cast<UGsGameObjectPlayer>(GSGameObject()->FindObject(EGsGameObjectType::Player, newChat._gameId)))
							{
								player->PopChatBubble(FText::FromString(newChat._text));
							}
						}
					}

				}
			});
	}
}

void UGsChatManager::OnEmoticonChat(uint64 uid, FString nickName, int32 emoticonIndex)
{
	if (GetChatViewOption(EGsOptionChat::EMOJI_POPUP)) // && IsNoneChatBlockUser(nickName)
	{
		if (UGsGameObjectPlayer* player = Cast<UGsGameObjectPlayer>(GSGameObject()->FindObject(EGsGameObjectType::Player, uid)))
		{
			player->PopEmojiBubble(emoticonIndex);
		}
	}
}

TWeakPtr<FGsChatItem> UGsChatManager::OnSystemChat(FString text, FString nickName, time_t inputTime, SystemMessageType optType, TArray<uint32> options)
{
	static bool initSystemMessage = false;
	static TMap<SystemMessageType, FText> tableSystemMessage = {
		TPair<SystemMessageType, FText>(SystemMessageType::MAX, FText::GetEmpty()),
		TPair<SystemMessageType, FText>(SystemMessageType::NOTICE, FText::GetEmpty()),
	};

	if (!initSystemMessage)
	{
		if (const UGsTable* table = FGsSchemaSystemMessage::GetStaticTable())
		{
			TArray<const FGsSchemaSystemMessage*> systemMessageList;
			table->GetAllRows<FGsSchemaSystemMessage>(systemMessageList);

			for (const auto e : systemMessageList)
			{
				if (!tableSystemMessage.Find(e->type))
					tableSystemMessage.Emplace(e->type, e->systemMessage);
			}
			initSystemMessage = true;
		}
	}

	if (inputTime == 0) inputTime = FGsTimeSyncUtil::GetServerNowSecond();

	if (nickName.IsValidIndex(0) && nickName[0] == ANONYMOUS_PLAYER)
	{
		static FText anonymousNick = FText::GetEmpty();
		if (anonymousNick.IsEmpty())
		{
			FText::FindText(TEXT("SystemChatText"), TEXT("AnonymousCharacter"), anonymousNick);
		}
		nickName = anonymousNick.ToString();
	}

	auto iter = tableSystemMessage.Find(optType);
	FGsChat chat = { EGsExChatType::SYSTEM, 0, inputTime, nickName, iter && !(*iter).IsEmpty() ? (*iter).ToString() : text };

	if (auto chatItem = AddChatItem(chat, optType, options))
	{
		FGsUIMsgParamChatItem param = FGsUIMsgParamChatItem(intType(chatItem->_chatViewType), chatItem, GetChatViewOption(EGsExChatType::SYSTEM));
		GMessage()->GetChat().SendMessage(MessageContentChat::ADD, &param);
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CHAT);

		return chatItem;
	}

	return nullptr;
}

TWeakPtr<FGsChatItem> UGsChatManager::OnOperatingMessage(FString text, NoticeType type, FString nickName /*= TEXT("SYSTEM")*/, time_t inputTime /*= 0*/)
{
	text = text.TrimStart();
	text = text.TrimEnd();

	if (text.Len() <= 0) return nullptr;

	if (inputTime == 0) inputTime = FGsTimeSyncUtil::GetServerNowSecond();

	FGsChat chat = { EGsExChatType::OPERATOR, 0, inputTime, TEXT(""), text};

	if (auto chatItem = AddChatItem(chat))
	{
		chatItem->SetChatText(chatItem->GetChatText().Replace(TEXT("\n"), TEXT(" ")));

		FGsUIMsgParamChatItem paramOperate = FGsUIMsgParamChatItem(intType(EGsExChatType::OPERATOR), chatItem);
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CHAT);
		GMessage()->GetChat().SendMessage(MessageContentChat::OPERATE_MESSAGE_UPDATE, &paramOperate);

		return chatItem;
	}

	return nullptr;
}

TWeakPtr<FGsChatItem> UGsChatManager::OnGroupChatSystemMessage(FString text, GuildGroupChatRoomId roomId)
{
	text = text.TrimStart();
	text = text.TrimEnd();

	if (text.Len() <= 0) return nullptr;

	FGsChat chat = { EGsExChatType::GROUPSYSTEM, 0, FGsTimeSyncUtil::GetServerNowSecond(), TEXT(""), text };
	chat._roomId = roomId;

	if (auto chatItem = AddChatItem(chat))
	{
		FGsUIMsgParamChatItem param = FGsUIMsgParamChatItem(intType(chatItem->_chatViewType), chatItem, GetChatViewOption(EGsExChatType::GROUP, roomId), roomId);
		GMessage()->GetChat().SendMessage(MessageContentChat::ADD, &param);
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CHAT);

		return chatItem;
	}

	return nullptr;
}

void UGsChatManager::OnGuildChatList(TArray<FGsChat>& list, uint64 seq)
{
	_isRemovedGuildChat = false;
	
	auto blockList = GGameData()->GetChatBlockList();

	if (_reqGuildChatSeq == 0 ) //길드 가입때
	{	
		_chatTabList[intType(EGsChatViewType::ALL)].RemoveAll([](auto e) { return e->_type == EGsExChatType::GUILD; });
		_mngChatList.RemoveAll([](auto e) { return !e.IsValid(); });

		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CHAT);
		GMessage()->GetChat().SendMessage(MessageContentChat::INVALIDATE, nullptr);
	}

	_reqGuildChatSeq = seq;

	if (list.Num() > 0)
	{
		TArray<FGsChat> blockUserExcludedList;
		Algo::CopyIf(list, blockUserExcludedList, [&blockList](auto& chat) {
			return blockList.FindByPredicate([&chat](FGsChatBlockUser& e) { return (e._name == chat._nickName); }) == nullptr; });

		int useCount = blockUserExcludedList.Num();
		int totalCount = useCount;
		auto thiz = MakeWeakObjectPtr(this);
		Algo::ForEach(blockUserExcludedList, [thiz, totalCount, &useCount](auto& chat) {
			useCount--;
			GLocalization()->ChangeBannedSentence(chat._text, [chat /*복사*/, thiz, useCount, totalCount](bool success, const FString& inText, const FString& outText) {
				if (thiz.IsValid())
				{
					FGsChat newChat = CopyTemp(chat);
					newChat._text = outText;

					thiz->InsertChatItem(newChat);

					if (useCount == 0)
					{
						FGsUIMsgParamChat param = FGsUIMsgParamChat(intType(EGsExChatType::GUILD), thiz->GetChatViewOption(EGsOptionChat::GUILD_CHAT), totalCount);
						GMessage()->GetChat().SendMessage(MessageContentChat::ADD_GUILD, &param);

						GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CHAT);
					}
				}
			});
		});

		if (list.Num() < guildChatMaxRcv)
		{
			_reqGuildChatSeq = UINT64_MAX;
		}
	}
	else 
	{
		FGsUIMsgParamChat param = FGsUIMsgParamChat(intType(EGsExChatType::GUILD), GetChatViewOption(EGsOptionChat::GUILD_CHAT), 0);
		GMessage()->GetChat().SendMessage(MessageContentChat::ADD_GUILD, &param);
	}
}

void UGsChatManager::AckGateWayGuildChatList(int result /*= PACKET_RESULT_SUCCESS*/)
{
	if (result == PACKET_RESULT_SUCCESS)
	{
		return;
	}
	else if(result == static_cast<int>(PD::Result::GUILD_ERROR_INVALID_GUILD_CHAT_ID) && _reqGuildChatSeq == 0) //처음임
	{
		_reqGuildChatSeq = UINT64_MAX;
	}

	FGsUIMsgParamChat param = FGsUIMsgParamChat(intType(EGsExChatType::GUILD), GetChatViewOption(EGsOptionChat::GUILD_CHAT), 0);
	GMessage()->GetChat().SendMessage(MessageContentChat::ADD_GUILD, &param);
}

void UGsChatManager::SendReqGuildChat()
{
	if (!IsEnableChatForChatType(ChatType::GUILD)) return;

	TArray<FGsChat> list;
	if (!IsValidPreGuildChatList())
	{
		OnGuildChatList(list, UINT64_MAX);
		return;
	}

	FGsNetSendServiceGuild::SendReqGuildChatList(_reqGuildChatSeq);
}

void UGsChatManager::OnRawGuildChattoItem(TArray<FGsGuildChatList> arr)
{
	_maxRcvGuildCount = _maxRcvGuildCount < arr.Num() ? arr.Num() : _maxRcvGuildCount;

	TArray<FGsChat> chatList;
	GuildChatDBId seq = UINT64_MAX;
	for (auto& e : arr)
	{
		seq = e._guildChatDBId < seq ? e._guildChatDBId : seq;

		chatList.Add(FGsChat{ EGsExChatType::GUILD, 0, e._messageTime, e._userName, e._message,
			0, TEXT(""), AccountGrade::NORMAL, e._rank, e._guildChatDBId, TEXT(""), 0 });
	}

	if (seq == 0) seq = UINT64_MAX; // 진짜 0 일 수도 있다 

	OnGuildChatList(chatList, seq);
}


void UGsChatManager::SetRemainFreeInvasionChat(uint16 remainCount, bool refresh /*= false*/)
{
	_invasionChat.RefreshFree(remainCount);
	if (refresh)
	{
		FGsUIMsgParamInvasionChat param(remainCount, GetInvasionFreeChatCount());
		GMessage()->GetChat().SendMessage(MessageContentChat::UPDATE_INVASIONCHAT_COUNT, &param);
	}
}

void UGsChatManager::ResetFreeInvasionChatCount(ResetTimeTicket InType)
{
	if (InType == ResetTimeTicket::RTT_FREE_INVADE_CHAT_RESET)
	{
		_invasionChat.RefreshFree();
	}
}

void UGsChatManager::SetConsumedFreeInvasionChat()
{
	_invasionChat.SetConsumed();
}

void UGsChatManager::UnlockSocialEmotionList(TArray<SocialEmotionInfo>& emotionList, bool refresh /*= false*/)
{
	Algo::ForEach(_socialMotions, [&emotionList](auto e) {
			auto emotion = e->GetSocialEmotionType();
			if (emotionList.FindByPredicate([&emotion](auto el) { return el.mUserSocialEmotion == emotion && el.mEnable; }))
			{
				e->SetUnlock();
			}
		});

	GMessage()->GetUI().SendMessage(MessageUI::SOCIALMOTION_STATE_UPDATE, nullptr);
}

void UGsChatManager::OnUnlockSocialEmotion(UserSocialEmotion emotion, bool isOpen /*= true*/)
{
	if (auto iter = _socialMotions.FindByPredicate([emotion](auto e) {return e->GetSocialEmotionType() == emotion; }))
	{
		(*iter)->SetUnlock();
	}

	FGsUIMsgParamSocialEmoji param = FGsUIMsgParamSocialEmoji(emotion);
	GMessage()->GetUI().SendMessage(MessageUI::SOCIALMOTION_STATE_UPDATE, &param);
}

void UGsChatManager::UpdateSocialEmotion(int64 gameId, UserSocialEmotion emotionType)
{
	if (auto player = GSGameObject()->FindObject(EGsGameObjectType::Player, gameId))
	{
		if (player->IsObjectType(EGsGameObjectType::LocalPlayer))
		{
			if (auto emotionData = GetSocialMotionByType(emotionType).Pin())
			{
				time_t  endCoolTime = emotionData->GetCoolTime() + FGsTimeSyncUtil::GetServerNowSecond();
				Algo::ForEach(_socialMotions, [endCoolTime](auto e) { e->SetRunCoolTime(endCoolTime); });

				FGsUIMsgParamSocialEmoji param = FGsUIMsgParamSocialEmoji(emotionType);
				GMessage()->GetUI().SendMessage(MessageUI::SOCIALMOTION_STATE_UPDATE, &param);
			}
		}
	}
}

void UGsChatManager::OnAddedBlockList(UserDBId uid, FString userName)
{
	auto myWorldId = GGameData()->GetUserData()->mHomeWorldId;

	for (auto e : TEnumRange<EGsChatViewType>()) //{ EGsChatViewType::NORMAL, EGsChatViewType::SERVER, EGsChatViewType::PARTY, EGsChatViewType::GUILD, EGsChatViewType::WHISPER, EGsChatViewType::INVASION, EGsChatViewType::ALL })
	{
		_chatTabList[intType(e)].RemoveAll([userName, myWorldId](TSharedPtr<FGsChatItem> item) {
			return item->GetHomeWorldId() == myWorldId && item->_senderNick.Compare(userName) == 0;
			});
	}
	_mngChatList.RemoveAll([](TWeakPtr<FGsChatItem> item) { return item.IsValid() == false; });

	FGsUIMsgParamChatBlock param = FGsUIMsgParamChatBlock(uid);
	GMessage()->GetChat().SendMessage(MessageContentChat::INVALIDATE_BLOCK_LIST, &param);
	GMessage()->GetChat().SendMessage(MessageContentChat::INVALIDATE, nullptr);
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CHAT);

	FText findText;
	FText::FindText(TEXT("ChatText"), TEXT("ChatTicker1"), findText);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
}

void UGsChatManager::OnRemovedBlockList(UserDBId uid)
{
	FGsUIMsgParamChatBlock param = FGsUIMsgParamChatBlock(uid);
	GMessage()->GetChat().SendMessage(MessageContentChat::INVALIDATE_BLOCK_LIST, &param);

	FText findText;
	FText::FindText(TEXT("ChatText"), TEXT("ChatTicker2"), findText);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
}

bool UGsChatManager::IsNoneChatBlockUser(FString userNick)
{
	auto blockList = GGameData()->GetChatBlockList();
	return blockList.FindByPredicate([userNick](FGsChatBlockUser& e) { return (e._name == userNick); }) ? false : true;
}

void UGsChatManager::OnCheckPartyChat(bool inparty)
{
	if (GSParty()->GetIsInParty() == false) //파티 탈퇴때
	{
		ClearChat(EGsChatViewType::PARTY);
	}

	if (GSParty()->GetIsInParty() == true) //파티 생성또는 가입때
	{
		_chatTabList[intType(EGsChatViewType::ALL)].RemoveAll([](auto e) { return e->_type == EGsExChatType::PARTY; });
		_mngChatList.RemoveAll([](auto e) { return !e.IsValid(); });

		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CHAT);
		GMessage()->GetChat().SendMessage(MessageContentChat::INVALIDATE, nullptr);
	}

	FGsUIMsgParamChat param = FGsUIMsgParamChat(intType(EGsExChatType::PARTY));
	GMessage()->GetChat().SendMessage(MessageContentChat::CHANGE_STATE, &param);
}

void UGsChatManager::OnCheckGuildChat()
{
	if (GGuild()->IsGuild() == false) // 길드 탈퇴
	{
		ClearChat(EGsChatViewType::GUILD);
		ClearChat(EGsChatViewType::GROUP);

		_isRemovedGuildChat = true;
		_reqGuildChatSeq = 0;
	}
	else
	{
		if (_isRemovedGuildChat) // 길드 탈퇴 -> 길드 가입의 경우 이때 전체 chat 삭제
		{
			// 창설때는 바로 길드 챗이 오지 않아 달라고 요청
			SendReqGuildChat();
		}
		else // guild chat group 에 변화가 있을 때
		{

		}
	}

	FGsUIMsgParamChat param = FGsUIMsgParamChat(intType(EGsExChatType::GUILD));
	GMessage()->GetChat().SendMessage(MessageContentChat::CHANGE_STATE, &param);
}

void UGsChatManager::OnCheckUserExistByName(bool isValidName, WorldId worldId)
{
	_whisperWorldId = 0;

	FGsUIMsgParamChatUser param = FGsUIMsgParamChatUser(worldId, isValidName); //name이 같이 오면 struct 하나 만들자.
	GMessage()->GetChat().SendMessage(MessageContentChat::CHECK_EXIST_BY_NAME, &param);
}

int UGsChatManager::GetChatDataList(EGsChatViewType chatType, OUT TArray<TWeakPtr<FGsChatItem>>& chatList, uint64 groupId /*= 0*/)
{
	if (groupId > 0)
	{
		if (const auto dataTable = Cast<UGsTableChatTabViewInfo>(FGsSchemaChatTabViewInfo::GetStaticTable()))
		{
			const FGsSchemaChatTabViewInfo* tabConfig = nullptr;
			if (dataTable->FindRowById(chatType, tabConfig) && !tabConfig->useRoomId)
			{
				groupId = 0;
			}
		}
	}

	_chatTabList[intType(chatType)].GetListItems(chatList, groupId); //chatList.Append(_chatTabList[intType(chatType)]);
	return chatList.Num();
}

//full chat 만 지원 - M6, 
//PopupChat Full도 동일하게 영향을 받도록 수정되므로 여기에서 옵션에 따른 체크 하지 않음 - R1
int32 UGsChatManager::GetHudChatList(TArray<TWeakPtr<FGsChatItem>>& chatList)
{
	auto nowSec = FGsTimeSyncUtil::GetServerNowSecond();

	int32 count = 0;
	auto iter = _chatTabList[intType(_hudChatType)].ChatList(_groupChatID).CreateConstIterator();
	iter.SetToEnd(); iter--;

	//_lastInputEndTime - iter->Pin()->_inputTime 으로 해야 오래된 기사단 챗이 뜨지않고 사라지거나 하지 않음
	for (; iter && count < chatHudMax; iter--, count++)
	{
		if (iter->IsValid())
		{
			auto offset = iter && iter->IsValid() ? ((*iter).Pin()->_type == EGsExChatType::GUILD) ?
					_lastInputEndTime - (*iter).Pin()->GetInputTime() + hudEraseSec : hudEraseSec : 0; //|| ((*iter).Pin()->_type == EGsExChatType::ALLYGUILD)

			if (offset < 0 || (nowSec - (*iter).Pin()->GetInputTime() > offset)) break;
			chatList.Insert(*iter, 0);
		}
	}

	return chatList.Num();
}

void UGsChatManager::SetSelectedHudChat(EGsChatViewType tab, uint64 roomId /*= 0*/)
{
	_hudChatType = tab;
	_groupChatID = roomId;
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CHAT);
}

void UGsChatManager::SetChatViewOption(EGsOptionChat type, bool enable, uint64 roomId /*= 0*/, bool modify /*= false*/)
{
	if (auto userSetting = GOption()->GetGameUserSettingsEx())
	{
		if (type == EGsOptionChat::ALLYGUILD_CHAT)
		{
			SetGroupChatViewOption(roomId, enable, modify);
			enable = true; //ALLYGUILD_CHAT 자체틑 enable로 해둔다
		}

		userSetting->SetChatOption(type, enable, modify);

		if (modify)
		{
			static TMap<EGsOptionChat, EGsExChatType> chatOpt2Type = {
					TPair<EGsOptionChat, EGsExChatType>{ EGsOptionChat::NORMAL_CHAT, EGsExChatType::NORMAL },
					TPair<EGsOptionChat, EGsExChatType>{ EGsOptionChat::SERVER_CHAT, EGsExChatType::WORLD },
					TPair<EGsOptionChat, EGsExChatType>{ EGsOptionChat::PARTY_CHAT, EGsExChatType::PARTY },
					TPair<EGsOptionChat, EGsExChatType>{ EGsOptionChat::ALLYGUILD_CHAT, EGsExChatType::GROUP },
					TPair<EGsOptionChat, EGsExChatType>{ EGsOptionChat::GUILD_CHAT, EGsExChatType::GUILD },
					TPair<EGsOptionChat, EGsExChatType>{ EGsOptionChat::WHISPER, EGsExChatType::WHISPER },
					TPair<EGsOptionChat, EGsExChatType>{ EGsOptionChat::INVADE_CHAT, EGsExChatType::INVADE },
					TPair<EGsOptionChat, EGsExChatType>{ EGsOptionChat::SYSTEM, EGsExChatType::SYSTEM },
			};

			TArray<EGsExChatType> enableChatList{ EGsExChatType::OPERATOR };
			for (auto e : chatOpt2Type)
			{
				if (userSetting->GetChatOption(e.Key))
					enableChatList.Add(e.Value);
			}

			//option 변경시 fullList 에서 빼준다. - R1
			_chatTabList[intType(EGsChatViewType::ALL)].RemoveAll([&enableChatList, roomId](auto e) {
				return enableChatList.Find(e->_type) == INDEX_NONE || (e->_type == EGsExChatType::GROUP && e->GetRoomId() == roomId);
			});


			FGsUIMsgParamChat param = FGsUIMsgParamChat(intType(EGsExChatType::Max));
			GMessage()->GetChat().SendMessage(MessageContentChat::INVALIDATE, &param);
			GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CHAT);
		}
	}
}

bool UGsChatManager::GetChatViewOption(EGsExChatType chatType, uint64 roomId /*= 0*/)
{
	static TMap<EGsExChatType, EGsOptionChat> chatType2Option = {
			TPair<EGsExChatType, EGsOptionChat>{ EGsExChatType::NORMAL, EGsOptionChat::NORMAL_CHAT},
			TPair<EGsExChatType, EGsOptionChat>{ EGsExChatType::WORLD,  EGsOptionChat::SERVER_CHAT},
			TPair<EGsExChatType, EGsOptionChat>{ EGsExChatType::PARTY, EGsOptionChat::PARTY_CHAT},
			TPair<EGsExChatType, EGsOptionChat>{ EGsExChatType::GROUP,  EGsOptionChat::ALLYGUILD_CHAT},
			TPair<EGsExChatType, EGsOptionChat>{ EGsExChatType::GUILD, EGsOptionChat::GUILD_CHAT},
			TPair<EGsExChatType, EGsOptionChat>{ EGsExChatType::WHISPER, EGsOptionChat::WHISPER },
			TPair<EGsExChatType, EGsOptionChat>{ EGsExChatType::INVADE, EGsOptionChat::INVADE_CHAT },
			TPair<EGsExChatType, EGsOptionChat>{ EGsExChatType::SYSTEM, EGsOptionChat::SYSTEM },
	};

	if (chatType == EGsExChatType::OPERATOR) return true; //무조건 표시
	if (chatType == EGsExChatType::INTERSERVER) chatType = EGsExChatType::WORLD;

	if (chatType2Option.Find(chatType) == nullptr) return false;

	return GetChatViewOption(chatType2Option[chatType], roomId);
}

bool UGsChatManager::GetChatViewOption(EGsOptionChat type, uint64 roomId /*= 0*/)
{
	if (auto userSetting = GOption()->GetGameUserSettingsEx())
	{
		if (roomId != 0 && type == EGsOptionChat::ALLYGUILD_CHAT)
		{
			return userSetting->GetGroupChatOption(roomId);
		}

		return userSetting->GetChatOption(type);
	}
	return false;
}

void UGsChatManager::SetGroupChatViewOption(uint64 roomId, bool enable, bool modify /*= false*/)
{
	if (auto userSetting = GOption()->GetGameUserSettingsEx())
	{
		userSetting->SetGroupChatOption(roomId, enable, modify);
	}
}

int UGsChatManager::GetChatGroupViewOptions(TArray<uint64>& hiddenGroup)
{
	if (auto userSetting = GOption()->GetGameUserSettingsEx())
	{
		TArray<uint64> groupOptions;
		if (userSetting->GetGroupChatOptions(groupOptions) > 0)
		{
			TArray<TWeakPtr<FGsGuildGroupChatRoom>> rooms;
			GGuild()->GetGroupChatRooms(rooms);

			TArray<uint64> invalidGroups = groupOptions.FilterByPredicate([&rooms](uint64 roomId) {
					return rooms.FindByPredicate([&roomId](auto room) {
						return room.IsValid() && room.Pin()->IsValidGroup() && room.Pin()->GetRoomId() == roomId; }) == nullptr;
				});

			for (auto groupId : invalidGroups)
			{
				userSetting->SetGroupChatOption(groupId, true);
			}

			userSetting->SaveUserSettings();

			groupOptions.Reset();
			userSetting->GetGroupChatOptions(groupOptions);
		}

		hiddenGroup.Append(groupOptions);
	}

	return hiddenGroup.Num();
}

bool UGsChatManager::GetChatGroupViewOption(uint64 roomId)
{
	if (auto userSetting = GOption()->GetGameUserSettingsEx())
	{
		return userSetting->GetGroupChatOption(roomId);
	}
	return false;
}

void UGsChatManager::SetChatMacro(int8 index, const FText& text, bool modify)
{
	if (auto userSetting = GOption()->GetGameUserSettingsEx())
		userSetting->SetChatMacro(index, text, modify);
}

const FText& UGsChatManager::GetChatMacro(int8 index)
{
	if (auto userSetting = GOption()->GetGameUserSettingsEx())
		return userSetting->GetChatMacro(index);

	return FText::GetEmpty();
}

bool UGsChatManager::IsNoSpammingChat(EGsExChatType curType)
{
	_chatInputEnableTime = _chatInputEnableTime > 0 &&
		(_chatInputEnableTime - FGsTimeSyncUtil::GetServerNowSecond()) > 0 ? _chatInputEnableTime : 0;

	FTimespan remainTime = _useBanTIme ? _endOperatingBannedTime - FGsTimeSyncUtil::GetClientNowDateTime() : 0;
	_endOperatingBannedTime = remainTime.GetTotalSeconds() > 0 ? _endOperatingBannedTime : FDateTime(0);
	_useBanTIme = _endOperatingBannedTime.GetTicks() > 0;

	if (_chatInputEnableTime == 0 && !_useBanTIme)
	{
		_lastInputChatType = curType;
		return true;
	}

	time_t curServerTime = FGsTimeSyncUtil::GetServerNowSecond();

	if (_chatInputEnableTime != 0)
	{
		FText banSpammingText;
		FText::FindText(TEXT("ChatText"), TEXT("ChatPenaltyTime"), banSpammingText);

		int32 remainTIme = _chatInputEnableTime - curServerTime;
		auto banSpamming = FString::Format(*(banSpammingText.ToString()), { remainTIme });

		FGsChat chat{ curType, 0, curServerTime, TEXT(""), banSpamming };

		auto chatItem = AddChatItem(chat);
		chatItem->_markupNick.Empty();

		FGsUIMsgParamChatItem param = FGsUIMsgParamChatItem((uint16)chat._type, chatItem);
		GMessage()->GetChat().SendMessage(MessageContentChat::ADD, &param);
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CHAT);

		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, FText::FromString(banSpamming));
	}
	else 
	{
		FText ChatBannedTimeTickerFormat;
		FText::FindText(TEXT("ChatText"), TEXT("ChatTicker13"), ChatBannedTimeTickerFormat);

		FString banEndTime = _endOperatingBannedTime.ToString(*ChatBannedTimeTickerFormat.ToString());

		FGsChat chat{ curType, 0, curServerTime, TEXT(""), banEndTime };

		auto chatItem = AddChatItem(chat);
		chatItem->_markupNick.Empty();

		FGsUIMsgParamChatItem param = FGsUIMsgParamChatItem((uint16)chat._type, chatItem);
		GMessage()->GetChat().SendMessage(MessageContentChat::ADD, &param);
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CHAT);

		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, FText::FromString(banEndTime));
	}

	return false;
}

void UGsChatManager::SetBannedSpammingChat(time_t spammingBaannedTime, time_t operatingBannedTime)
{
	time_t curServerTime = FGsTimeSyncUtil::GetServerNowSecond();

	if (spammingBaannedTime > 0)
	{
		_chatInputEnableTime = spammingBaannedTime;

		FText banSpammingText;
		if (FText::FindText(TEXT("ChatText"), TEXT("ChatPenaltyTime"), banSpammingText))
		{
			int32 remainTIme = spammingBaannedTime - curServerTime;
			auto banSpamming = FString::Format(*(banSpammingText.ToString()), { remainTIme });

			FGsChat chat{ _lastInputChatType, 0, curServerTime, TEXT(""), banSpamming };

			auto chatItem = AddChatItem(chat);
			chatItem->_markupNick.Empty();

			FGsUIMsgParamChatItem param = FGsUIMsgParamChatItem((uint16)chat._type, chatItem);
			GMessage()->GetChat().SendMessage(MessageContentChat::ADD, &param);
			GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CHAT);
		}
	}
	if (operatingBannedTime > 0)
	{
		_endOperatingBannedTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(operatingBannedTime); //FDateTime(2023, 2, 14, 8, 50, 20); //
		_useBanTIme = true;

		if (spammingBaannedTime <= 0)
		{
			FText ChatBannedTimeTickerFormat;
			FText::FindText(TEXT("ChatText"), TEXT("ChatTicker13"), ChatBannedTimeTickerFormat);

			FString banEndTime = _endOperatingBannedTime.ToString(*ChatBannedTimeTickerFormat.ToString());

			FGsChat chat{ _lastInputChatType, 0, curServerTime, TEXT(""), banEndTime };

			auto chatItem = AddChatItem(chat);
			chatItem->_markupNick.Empty();

			FGsUIMsgParamChatItem param = FGsUIMsgParamChatItem((uint16)chat._type, chatItem);
			GMessage()->GetChat().SendMessage(MessageContentChat::ADD, &param);
			GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CHAT);

			//FText tickerMessage = FText::Format(ChatBannedTimeTickerFormat, FText::FromString(banEndTime));
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, FText::FromString(banEndTime));
		}
	}
}

void UGsChatManager::LoadChatInfo()
{
	if (const auto table = Cast<UGsTableChatConfig>(FGsSchemaChatConfig::GetStaticTable()))
	{
		TArray<const FGsSchemaChatConfig*> chatConfigs;
		table->GetAllRows(chatConfigs);

		if (chatConfigs.IsValidIndex(0))
		{
			const auto chatConfig = chatConfigs[0];

			_invasionChat.currency.type = chatConfig->invadeChatCurrencyType;
			_invasionChat.currency.amount = chatConfig->invadeChatCurrencyAmount;
			_invasionChat.free.dailyCount = chatConfig->freeInvadeChatCount;
			_invasionChat.free.remainCount = chatConfig->freeInvadeChatCount;

			_invasionChat.isValid = true;
			_invasionChat.isEnableFree = true;
		}
	}

}

int32 UGsChatManager::GetSocialMotionList(TArray<TWeakPtr<FSocialEmotionMotion>>& motionList)
{
	motionList.Append(_socialMotions);
	return _socialMotions.Num();
}

int32 UGsChatManager::GetEmoticonList(TArray<const FGsSchemaEmoji*>& emoticonList)
{
	if (const UGsTable* dataTable = FGsSchemaEmoji::GetStaticTable())
	{
		dataTable->GetAllRows(emoticonList);
		emoticonList.StableSort([](auto& le, auto& re) { return le.order < re.order; });
	}

	return emoticonList.Num();
}

TWeakPtr<FSocialEmotionMotion> UGsChatManager::GetSocialMotion(int32 socialId)
{
	if (auto socialActIter = _socialMotions.FindByPredicate([socialId](auto e) { return e->GetMotionId() == socialId; }))
	{
		return *socialActIter;
	}
	return nullptr;
}

TWeakPtr<FSocialEmotionMotion> UGsChatManager::GetSocialMotionByType(UserSocialEmotion emotionType)
{
	if (auto socialActIter = _socialMotions.FindByPredicate([emotionType](auto e) { return e->GetSocialEmotionType() == emotionType; }))
	{
		return *socialActIter;
	}
	return nullptr;
}

void UGsChatManager::MakeChatMarkup(FGsChatItem& chatItem)
{	
	//FDateTime inputTime(chatItem._inputTime * ETimespan::TicksPerSecond);
	//const FString timedesc = FString::Printf(TEXT("%d:%02d %s"),
	//inputTime.GetHour12(), inputTime.GetMinute(), inputTime.IsAfternoon() ? TEXT("PM") : TEXT("AM"));
	//
	//chatItem._markupTime = FString::Format(TEXT(" <InputTime>{0}</>"), { timedesc });

	if (const auto dataTable = Cast<UGsTableChatTypeViewInfo>(FGsSchemaChatTypeViewInfo::GetStaticTable()))
	{
		const FGsSchemaChatTypeViewInfo* configInfo = nullptr;
		if (dataTable->FindRowById(chatItem._type, configInfo))
		{
			auto accGrade = configInfo->accountGradeTag.Num() == 1 ? configInfo->accountGradeTag.begin()->Key : static_cast<uint8>(chatItem.GetAccountGrade());
			if (auto iter = configInfo->accountGradeTag.Find(accGrade))
			{
				FString prefixTag = configInfo->prefixFmt.IsEmpty() ? TEXT("") : configInfo->prefixFmt.Replace(TEXT("TAG"), **iter);
				FString nameTag = configInfo->nameFmt.IsEmpty() ? TEXT("") : configInfo->nameFmt.Replace(TEXT("TAG"), **iter);
				FString textTag = configInfo->textFmt.IsEmpty() ? TEXT("") : configInfo->textFmt.Replace(TEXT("TAG"), **iter);

				chatItem._type = configInfo->useChatType;
				chatItem._chatViewType   = configInfo->viewType;
				chatItem._useWorldPrifix = configInfo->useOtherWorldPrefix;
				for (auto viewInfo : configInfo->viewTabs)
				{
					chatItem._viewTabs.Add(viewInfo.GetRow()->tab);
				}
				chatItem._markupPrefix = (!prefixTag.IsEmpty() && !chatItem._namePrefix.IsEmpty()) ? FString::Format(*prefixTag, { chatItem._namePrefix }) : TEXT("");
				chatItem._markupNick = nameTag.IsEmpty() ? TEXT("") : FString::Format(*nameTag, { chatItem.GetNickName() });
				chatItem._markupTextFormat = TEXT("{0}{1}");

				if (chatItem._type == EGsExChatType::WHISPER)
				{
					chatItem._markupTextFormat = TEXT("{1}{0}");
					chatItem._markupText = FString::Format(*textTag, { chatItem.GetChatText(), chatItem.GetGameId() > 0 ? TEXT("<<") : TEXT(">>") });
				}
				else
				{
					chatItem._markupText = textTag.IsEmpty() ? TEXT("") : FString::Format(*textTag, { chatItem.GetChatText() });
				}
			}
		}
	}
}

void UGsChatManager::MakeSystemMessageMarkup(FGsChatItem& chatItem)
{
	static TMap<uint8, FString> tagChangeString =
	{
		{static_cast<uint8>(ItemGrade::NORMAL), TEXT("<GRADE_NORMAL>")},
		{static_cast<uint8>(ItemGrade::MAGIC), TEXT("<GRADE_MAGIC>")},
		{static_cast<uint8>(ItemGrade::RARE), TEXT("<GRADE_RARE>")},
		{static_cast<uint8>(ItemGrade::EPIC), TEXT("<GRADE_EPIC>")},
		{static_cast<uint8>(ItemGrade::UNIQUE), TEXT("<GRADE_UNIQUE>")},
		{static_cast<uint8>(ItemGrade::LEGEND), TEXT("<GRADE_LEGEND>")},
		{static_cast<uint8>(ItemGrade::ETERNAL), TEXT("<GRADE_ETERNAL>")},
	};

	if (chatItem._optionType == SystemMessageType::MAX) return;

	static int fairyGradeOffset = static_cast<int>(FairyGrade::NONE) - static_cast<int>(ItemGrade::NONE);
	static int costumeGradeOffset = static_cast<int>(CostumeGrade::NONE) - static_cast<int>(ItemGrade::NONE);
	//chatItem._gradeIndex 는 ItemGrade 가 기준

	switch (chatItem._optionType)
	{
	case SystemMessageType::COSTUME_GET : //{0} 님이 {1} 을/를 획득하였습니다
		if (auto data = GSCostume()->GetCostumeData(chatItem._options[0]).Pin())
		{
			FString chatText = chatItem.GetChatText();
			if (chatText.Find(TEXT("<Grade>")) > 0)
			{
				auto gradeIndex = static_cast<uint8>(data->GetCostumeGrade()) - costumeGradeOffset;
				if (auto changeTag = tagChangeString.Find(gradeIndex))
				{
					chatText = chatText.Replace(TEXT("<Grade>"), *(*changeTag), ESearchCase::CaseSensitive);
				}
			}
			
			FText markupText = TextPostScriptParser(chatText,{ FText::FromString(chatItem.GetNickName()), data->GetCostumeName() });

			chatItem.SetChatText(chatText);
			chatItem._markupText = AddParseSystemText(markupText.ToString(), TEXT("<System>"));
		}
		break;
	case SystemMessageType::FAIRY_GET :	 //{0} 님이 {1} 을/를 획득하였습니다
		if (auto data = GSFairy()->GetFairyData(chatItem._options[0]).Pin())
		{
			FString chatText = chatItem.GetChatText();
			if (chatText.Find(TEXT("<Grade>")) > 0)
			{
				auto gradeIndex = static_cast<uint8>(data->GetFairyGrade()) - fairyGradeOffset;
				if (auto changeTag = tagChangeString.Find(gradeIndex))
				{
					chatText = chatText.Replace(TEXT("<Grade>"), *(*changeTag), ESearchCase::CaseSensitive);
				}
			}
			FText markupText = TextPostScriptParser(chatText, { FText::FromString(chatItem.GetNickName()), data->GetFairyName() });

			chatItem.SetChatText(chatText);
			chatItem._markupText = AddParseSystemText(markupText.ToString(), TEXT("<System>"));
		}
		break;
	case SystemMessageType::ITEM_GET_DROP : //{0} 님이 {1} 을/를 {2}에서 획득하였습니다
		if (auto data = FGsItem::GetTableDataByID(chatItem._options[0]))
		{
			FString chatText = chatItem.GetChatText();
			if (chatText.Find(TEXT("<Grade>")) > 0)
			{
				if (auto changeTag = tagChangeString.Find(static_cast<uint8>(data->grade)))
				{
					chatText = chatText.Replace(TEXT("<Grade>"), *(*changeTag), ESearchCase::CaseSensitive);
				}
			}

			FText mapName;
			if (auto mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(chatItem._options[1]))
				mapName = mapData->MapStringId;

			FText markupText = TextPostScriptParser(chatText, { FText::FromString(chatItem.GetNickName()), data->name, mapName });

			chatItem.SetChatText(chatText);
			chatItem._markupText = AddParseSystemText(markupText.ToString(), TEXT("<System>"));
		}
		break;
	case SystemMessageType::ITEM_GET_BOX : //{0} 님이 {1} 을/를 {2}에서 획득하였습니다.
		if (auto data = FGsItem::GetTableDataByID(chatItem._options[0]))
		{
			FText boxName;
			if (auto box = FGsItem::GetTableDataByID(chatItem._options[1])) boxName = box->name;

			FString chatText = chatItem.GetChatText();
			if (chatText.Find(TEXT("<Grade>")) > 0)
			{
				if (auto changeTag = tagChangeString.Find(static_cast<uint8>(data->grade)))
				{
					chatText = chatText.Replace(TEXT("<Grade>"), *(*changeTag), ESearchCase::CaseSensitive);
				}
			}
			FText markupText = TextPostScriptParser(chatText, { FText::FromString(chatItem.GetNickName()), data->name, boxName});

			chatItem.SetChatText(chatText);
			chatItem._markupText = AddParseSystemText(markupText.ToString(), TEXT("<System>"));
		}
		break;
	case SystemMessageType::ITEM_GET_FIELD_BOSS : //{0} 님이 <P>{1}</>|hpp(을,를) 처치하여 </><Grade>{2}</>|hpp(을,를) 획득하였습니다
		if (auto data = FGsItem::GetTableDataByID(chatItem._options[0]))
		{
			FString chatText = chatItem.GetChatText();
			if (chatText.Find(TEXT("<Grade>")) > 0)
			{
				if (auto changeTag = tagChangeString.Find(static_cast<uint8>(data->grade)))
				{
					chatText = chatText.Replace(TEXT("<Grade>"), *(*changeTag), ESearchCase::CaseSensitive);
				}
			}
			auto bossName =  UGsGameObjectUtil::GetNpcName(chatItem._options[1]);
			FText markupText = TextPostScriptParser(chatText, { FText::FromString(chatItem.GetNickName()), bossName, data->name });

			chatItem.SetChatText(chatText);
			chatItem._markupText = AddParseSystemText(markupText.ToString(), TEXT("<System>"));
		}
		break;
	case SystemMessageType::ITEM_ENCHANT : //{0} 님이 {1} 의 {2} 강화에 성공하였습니다.
		if (auto data = FGsItem::GetTableDataByID(chatItem._options[0]))
		{
			FString strNameFormat;
			FString chatText = chatItem.GetChatText();
			if (chatText.Find(TEXT("<Grade>")) > 0)
			{
				if (auto changeTag = tagChangeString.Find(static_cast<uint8>(data->grade)))
				{
					chatText = chatText.Replace(TEXT("<Grade>"), *(*changeTag), ESearchCase::CaseSensitive);
				}
			}

			FText markupText = TextPostScriptParser(chatText, { FText::FromString(chatItem.GetNickName()), data->name, chatItem._options[1] });

			chatItem.SetChatText(chatText);
			chatItem._markupText = AddParseSystemText(markupText.ToString(), TEXT("<System>"));
		}
		break;
	case SystemMessageType::ITEM_GET_DROP_ON_PARTY :
		if (auto data = FGsItem::GetTableDataByID(chatItem._options[0]))
		{
			FString strNameFormat;
			FString chatText = chatItem.GetChatText();
			if (chatText.Find(TEXT("<Grade>")) > 0)
			{
				if (auto changeTag = tagChangeString.Find(static_cast<uint8>(data->grade)))
				{
					chatText = chatText.Replace(TEXT("<Grade>"), *(*changeTag), ESearchCase::CaseSensitive);
				}
			}

			FText markupText = TextPostScriptParser(chatText, { FText::FromString(chatItem.GetNickName()), data->name });

			chatItem.SetChatText(chatText);
			chatItem._markupText = AddParseSystemText(markupText.ToString(), TEXT("<Party>"));
			chatItem._chatViewType = EGsChatViewType::PARTY;
		}
		break;
	default: 
		{
		//정해지지 않은 타입은 tag 붙여온 대로 읽고 tag 되지 않은 부분만 system message로 전환. 전체 태그 된 것은 그대로 출력됨
			FString chatText = chatItem.GetChatText();
			chatItem._markupText = AddParseSystemText(chatText, TEXT("<System>"));
			chatItem.SetChatText(chatText);
		}
		break;
	}
}

bool UGsChatManager::OnCheatChat(FString name, FString text, uint64 uid)
{
#if WITH_EDITOR

	if (uid != GGameData()->GetUserData()->mGameId) return false;

	FString costumeSummonTest = TEXT("##CostumeSummon#");
	FString fairySummonTest = TEXT("##FairySummon#");
	FString emotionTest = TEXT("##Emotion#");
	FString operatorTest = TEXT("##Operator#");
	FString bannedTest = TEXT("##Banned#");
	FString msgTest = TEXT("##");

	if (text.Left(bannedTest.Len()).ToLower() == bannedTest.ToLower())
	{
		auto remain = text.Right(text.Len() - operatorTest.Len());
		remain = remain.TrimStartAndEnd();

		int check = FCString::Atoi(*remain);

		auto bannedTime = FGsTimeSyncUtil::GetServerNowSecond() + 120;
		if(check == 0) SetBannedSpammingChat(0, bannedTime);
		else SetBannedSpammingChat(bannedTime, 0);
	}
	else if (text.Left(operatorTest.Len()).ToLower() == operatorTest.ToLower())
	{
		auto remain = text.Right(text.Len() - operatorTest.Len());
		remain = remain.TrimStartAndEnd();

		OnOperatingMessage(remain);
		return true;
	}
	else if (text.Left(costumeSummonTest.Len()).ToLower() == costumeSummonTest.ToLower())
	{
		auto remain = text.Right(text.Len() - costumeSummonTest.Len());
		remain.TrimStartAndEndInline();

		static int64 id = 899999999;

		int costumeId = FCString::Atoi(*remain);

		TArray<CostumeConfirmData> costumeSummons;
		costumeSummons.Add(CostumeConfirmData(id--, costumeId, 200302, 5, FTimespan(7, 10, 0, 0).GetTotalSeconds()));
		costumeSummons.Add(CostumeConfirmData(id--, costumeId, 200302, 0, FTimespan(7, 10, 0, 0).GetTotalSeconds()));
		GSCostume()->UpdateCostumeSummonWaitingItemList(costumeSummons, FGsTimeSyncUtil::GetServerNowDateTimeUTC(), true);
	}
	else if (text.Left(fairySummonTest.Len()).ToLower() == fairySummonTest.ToLower())
	{
		auto remain = text.Right(text.Len() - fairySummonTest.Len());
		remain.TrimStartAndEndInline();

		static int64 id = 999999999;

		int fairyId = FCString::Atoi(*remain);

		TArray< FairyConfirmData> fairySummons;
		fairySummons.Add(FairyConfirmData(id--, fairyId, 200302, 5, FTimespan(7, 10, 0, 0).GetTotalSeconds()));
		fairySummons.Add(FairyConfirmData(id--, fairyId, 200302, 0, FTimespan(7, 10, 0, 0).GetTotalSeconds()));
		GSFairy()->UpdateFairySummonWaitingItemList(fairySummons, FGsTimeSyncUtil::GetServerNowDateTimeUTC(), true);
	}
	else if (text.Left(emotionTest.Len()) == emotionTest)
	{
		auto remain = text.Right(text.Len() - emotionTest.Len());
		remain.TrimStartAndEndInline();

		static TArray<FString> emotions = {
			TEXT("Laugh"),
			TEXT("Sadness"),
			TEXT("Deny"),
			TEXT("Greeting"),
			TEXT("Favor"),
			TEXT("Agree"),
			TEXT("Brag"),
			TEXT("Fighting"),
			TEXT("HighMotivate"),
			TEXT("Heart"),
			TEXT("Dance"),
			TEXT("Surprise"),
		};

		for (auto emotion : emotions)
		{
			if (remain.Compare(emotion, ESearchCase::IgnoreCase) == 0)
			{
				auto index = emotions.IndexOfByKey(emotion);
				
				if (UGsGameObjectPlayer* player = Cast<UGsGameObjectPlayer>(GSGameObject()->FindObject(EGsGameObjectType::Player,  uid)))
				{
					auto FindNearestPlayer = [](AActor* actor) {

						UGsGameObjectBase* result = nullptr;
						TArray<UGsGameObjectBase*> spawnList = GSGameObject()->FindObjectArray(EGsGameObjectType::Player);

						if (spawnList.Num() > 0)
						{
							FVector2D origin = FVector2D(actor->GetActorLocation());

							spawnList.Sort([&origin](const UGsGameObjectBase& lhs, const UGsGameObjectBase& rhs)
								{
									return (lhs.GetLocation2D() - origin).SizeSquared() < (rhs.GetLocation2D() - origin).SizeSquared();
								});

							result = spawnList.Num() > 1 ? spawnList[1] : spawnList[0];
						}

						return result;
					};

					if (auto nearPlayer = FindNearestPlayer(player->GetActor()))
					{
						if (auto skillHandler = nearPlayer->GetSkillHandler())
						{
							skillHandler->PlaySocialEmotion(UserSocialEmotion(index));
							return true;
						}
					}
				}
			}
		}
	}
	else if (text.Left(msgTest.Len()) == msgTest)
	{
		auto remain = text.Right(text.Len() - msgTest.Len());
		remain = remain.TrimStartAndEnd();

		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::SYSTEM, FText::FromString(remain), TrayCondition::TC_Active, SectionPriority::None);

	}
#endif
	return false;
}

//bool UGsChatManager::GetMessageAppendableTabs(TSharedPtr<FGsChatItem> chatItem, OUT TArray<EGsChatViewType>& addTabs)
//{
//	auto accountGrade = chatItem->GetAccountGrade();
//	auto chatType = chatItem->_type;
//	if (chatItem->_type == EGsExChatType::SYSTEM && chatItem->_optionType == SystemMessageType::ITEM_GET_DROP_ON_PARTY)
//	{
//		chatType = EGsExChatType::PARTY;
//	}
//
//	if (const auto config = LoadChatTags(accountGrade, chatType))
//	{
//		addTabs.Append(config->info.viewTabs);
//
//		if (IsEnableChatForChatType(ChatType::GUILD) == false)
//		{
//			addTabs.Remove(EGsChatViewType::GUILD);
//			addTabs.Remove(EGsChatViewType::GROUP);
//		}
//		else if (IsEnableChatForChatType(ChatType::ALLY) == false)
//		{
//			addTabs.Remove(EGsChatViewType::GROUP);
//		}
//
//		if (IsEnableChatForChatType(ChatType::PARTY) == false)		addTabs.Remove(EGsChatViewType::PARTY);
//		if (IsEnableChatForChatType(ChatType::INVADE) == false)		addTabs.Remove(EGsChatViewType::INVASION);
//
//		if(addTabs.Contains(EGsChatViewType::ALL) && !GetChatViewOption(chatItem->_type, chatItem->GetRoomId()))
//		{
//			addTabs.Remove(EGsChatViewType::ALL);
//		}
//	}
//	return addTabs.Num() > 0;
//}

bool UGsChatManager::IsWhisperChatSentByMe(FGsChat& chat)
{
	static FString myUserName;

	if (homeWorldId == INVALID_WORLD_ID)
	{
		homeWorldId = GGameData()->GetUserData()->mHomeWorldId;
		myUserName = GGameData()->GetUserData()->mUserName;
	}

	if (chat._homeWorldId == 0) chat._homeWorldId = homeWorldId;
	if (chat._type == EGsExChatType::WHISPER && chat._homeWorldId == homeWorldId && myUserName.Equals(chat._senderName))
	{
		return true;
	}

	return false;
}

bool UGsChatManager::IsAllyGuildChatByMyGuild(FGsChat chat)
{
	auto myGuildId = GGuild()->IsGuild() ? GGuild()->GetMyGuildData()->_guildDBId : INVALID_GUILD_DB_ID;

	return	chat._type == EGsExChatType::GROUP &&
			homeWorldId == chat._homeWorldId &&
			myGuildId != INVALID_GUILD_DB_ID &&
			chat._guildId == myGuildId;
}


/*==================================================================*/
/*	이미 Header가 붙어 있으면 거르고, 없으면 헤더를 붙인다			*/
/*	<Knight> <Party> 등으로 이미 붙어있을 수 있다					*/
/*	안 붙어서 system message로 출력되는 메세지가 있을 수 있어 파싱	*/
/*==================================================================*/
FString UGsChatManager::AddParseSystemText(const FString& inText, FString header)
{
	if (header.IsEmpty()) return inText;

	const TCHAR* markUpEnd = TEXT("</>");
	auto resultText = inText.Replace(TEXT("\n"), TEXT("</>\n</>"));

	TArray<FString> procStr;
	resultText.ParseIntoArray(procStr, markUpEnd, false); //위치정보

	if (procStr.Num() < 1)
	{
		return header + resultText + markUpEnd;
	}

	resultText.Empty();
	for (auto str : procStr)
	{
		if (str.IsEmpty()) continue;
		if (str == TEXT("\n"))
		{
			resultText += str;
			continue;
		}

		FString procText = str;
		FString secDefText = str;
		int pos = 0;

		while (!procText.IsEmpty() && !procText.StartsWith(TEXT("<")))
		{
			procText = procText.Right(str.Len() - pos);
			pos++;
		}

		if (pos >= 0)
		{
			resultText += (header + str.Left(pos - 1) + markUpEnd);
			if (!procText.IsEmpty()) resultText += (procText + markUpEnd);
		}
		else
		{
			resultText += (header + str + markUpEnd);
		}
	}

	return resultText;
}

/*==================================================================*/
/*	string 뒤의 후첨 인수 수정자가 tag를 문자로 인식하여			*/
/*	제대로 을/를, 이/가 등이 붙지 않는 문제가 있다.					*/
/*	그 부분에 대한 Parsing 처리한다	FText::Format으로만 처리됨		*/
/*==================================================================*/
FText UGsChatManager::TextPostScriptParser(FString& useString, const FFormatOrderedArguments& InOrderedArguments)
{
	static const TCHAR ARG_MOD = TEXT('|');
	static const TCHAR FMT_START = TEXT('{');
	static const TCHAR FMT_END  = TEXT('}');
	static const TCHAR TAG_MODS = TEXT('<');
	static const TCHAR TAG_MODE = TEXT('>');

	int32 excEndIndex = 0;

	TMap<FString, FString> procs;
	FString orgString = useString; //<System>{0} 님이 </><Grade>{1}</><System>|hpp(을,를) 획득하였습니다.</>

	while (orgString.FindChar(ARG_MOD, excEndIndex))
	{
		auto tmpString = orgString.Left(excEndIndex); //<System>{0} 님이 </><Grade>{1}</><System>
		int32 tagStart = 0;
		int32 paramStart = 0;
		int32 paramEnd = 0;
		if (tmpString.FindLastChar(FMT_START, paramStart) && tmpString.Right(tmpString.Len() - paramStart).FindChar(TAG_MODS, tagStart))
		{
			tmpString.Left(paramStart).FindLastChar(TAG_MODS, paramStart);
			auto changeString = tmpString.Right(tmpString.Len() - paramStart); //{1}</><System>
			changeString.FindLastChar(FMT_END, paramEnd); paramEnd++;
			auto findString = changeString.Left(paramEnd); //{1}

			procs.Add(findString, changeString);
		}

		orgString = orgString.Right(orgString.Len() - excEndIndex - 1); //hpp(을,를) 획득하였습니다.</>
	}

	TArray<TPair<FText, FText>> resProcs;
	for (auto& e : procs)
	{
		useString = useString.Replace(*e.Value, *e.Key); //<System>{0} 님이 </><Grade>{1}|hpp(을,를) 획득하였습니다.</>

		FText findText = FText::Format(FText::FromString(e.Key), InOrderedArguments); //레페
		FText changedText = FText::Format(FText::FromString(e.Value), InOrderedArguments); //레페</><SYSTEM>
		resProcs.Add(TPair<FText, FText>{ findText, changedText });
	}

	FString RightString = FText::Format(FText::FromString(useString), InOrderedArguments).ToString(); //<System>지나 님이 </><Grade>레페를 획득하였습니다.</>

	useString.Empty();
	for (auto& e : resProcs)
	{
		auto split = RightString.Find(*e.Key.ToString()) + (e.Key.ToString()).Len();
		auto LeftString = RightString.Left(split).Replace(*e.Key.ToString(), *e.Value.ToString());
		RightString = RightString.Right(RightString.Len() - split);
		
		useString += LeftString; //같은 이름이 여러번 들어가는 경우가 있음. "인내의 주문서님이 인내의 주문서를 인내의 주문서 10에서 획득하였습니다"
		//useString = useString.Replace(*e.Key.ToString(), *e.Value.ToString()); //<System>지나 님이 </><Grade>레페</><SYSTEM>를 획득하였습니다.</>
	}

	useString += RightString;

	return FText::FromString(useString);
}

void UGsChatManager::LoadSocialMotion()
{
	if (const UGsTable* dataTable = FGsSchemaSocialEmotion::GetStaticTable())
	{
		TArray<const FGsSchemaSocialEmotion*> emojiList;
		dataTable->GetAllRows<FGsSchemaSocialEmotion>(emojiList);

		Algo::Transform(emojiList, _socialMotions, [](const auto e) { return MakeShared<FSocialEmotionMotion>(e, e->isLock); });
		_socialMotions.StableSort([](auto le, auto re) { return le->Order() < re->Order(); });

		ResetMotionFlag(true);
	}
}

//const ChatDeco* UGsChatManager::LoadChatTags(AccountGrade grade, EGsExChatType type, bool reset)
//{
//	static TArray<ChatDeco> baseTags;
//	static TMap<AccountGrade, TArray<ChatDeco>> gradeTags;
//
//	if (reset)
//	{
//		baseTags.Reset();
//		gradeTags.Reset();
//	}
//
//	if (baseTags.Num() <= 0)
//	{
//		//TMap<int, AccountGrade> changedGrade;
//		//changedGrade.Add(0, AccountGrade::NORMAL);
//		//changedGrade.Add(1, AccountGrade::OPERATOR);
//		//changedGrade.Add(2, AccountGrade::QATESTER);
//		//changedGrade.Add(3, AccountGrade::ADMIN);
//		//changedGrade.Add(4, AccountGrade::GM);
//		if (const UGsTable* dataTable = FGsSchemaChatDecoConfig::GetStaticTable())
//		{
//			TArray<const FGsSchemaChatDecoConfig*> configList;
//			dataTable->GetAllRows<FGsSchemaChatDecoConfig>(configList);
//
//			baseTags.Reset();
//			if (auto baseConfig = configList.FindByPredicate([](const auto data) { return data->useAccountGrade == false; }))
//			{
//				Algo::Transform((*baseConfig)->tags, baseTags, [](auto& pair) { return ChatDeco{ pair.Key, pair.Value }; });
//			}
//
//			for (const auto e : configList)
//			{
//				if (e->useAccountGrade)
//				{
//					auto& iter = gradeTags.FindOrAdd(static_cast<AccountGrade>(e->accountGrade));
//					Algo::Transform(e->tags, iter, [](auto& pair) { return ChatDeco{ pair.Key, pair.Value }; });
//				}
//			}
//		}
//
//		if (baseTags.Num() <= 0)
//		{
//			ChatDeco deco;
//			deco.chatType = EGsExChatType::NORMAL;
//			deco.info.chatViewType = EGsChatViewType::NORMAL;
//			deco.info.tagHeader = TEXT("Normal");
//			deco.info.viewTabs.Append({ EGsChatViewType::NORMAL, EGsChatViewType::ALL });
//
//			baseTags.Add(deco);
//
//			deco.chatType = EGsExChatType::WORLD;
//			deco.info.chatViewType = EGsChatViewType::SERVER;
//			deco.info.tagHeader = TEXT("World");
//			deco.info.viewTabs.Append({ EGsChatViewType::SERVER, EGsChatViewType::ALL });
//			baseTags.Add(deco);
//
//			deco.chatType = EGsExChatType::PARTY;
//			deco.info.chatViewType = EGsChatViewType::PARTY;
//			deco.info.tagHeader = TEXT("Party");
//			deco.info.viewTabs.Append({ EGsChatViewType::PARTY, EGsChatViewType::ALL });
//			baseTags.Add(deco);
//
//			deco.chatType = EGsExChatType::GUILD;
//			deco.info.chatViewType = EGsChatViewType::GUILD;
//			deco.info.tagHeader = TEXT("Knight");
//			deco.info.viewTabs.Append({ EGsChatViewType::GUILD, EGsChatViewType::ALL });
//			baseTags.Add(deco);
//
//			deco.chatType = EGsExChatType::GROUP;
//			deco.info.chatViewType = EGsChatViewType::GROUP;
//			deco.info.tagHeader = TEXT("Knight");
//			deco.info.viewTabs.Append({ EGsChatViewType::GROUP, EGsChatViewType::ALL });
//			baseTags.Add(deco);
//
//			deco.chatType = EGsExChatType::INVADE;
//			deco.info.chatViewType = EGsChatViewType::INVASION;
//			deco.info.tagHeader = TEXT("Invasion");
//			deco.info.viewTabs.Append({ EGsChatViewType::INVASION, EGsChatViewType::ALL });
//			baseTags.Add(deco);
//
//			deco.chatType = EGsExChatType::WHISPER;
//			deco.info.chatViewType = EGsChatViewType::WHISPER;
//			deco.info.tagHeader = TEXT("Whisper");
//			deco.info.viewTabs.Append({ EGsChatViewType::WHISPER, EGsChatViewType::ALL });
//			baseTags.Add(deco);
//
//			deco.chatType = EGsExChatType::SYSTEM;
//			deco.info.chatViewType = EGsChatViewType::SYSTEM;
//			deco.info.tagHeader = TEXT("System");
//			deco.info.viewTabs.Append({ EGsChatViewType::SYSTEM, EGsChatViewType::ALL });
//			baseTags.Add(deco);
//
//			deco.chatType = EGsExChatType::OPERATOR;
//			deco.info.chatViewType = EGsChatViewType::SYSTEM;
//			deco.info.tagHeader = TEXT("Operator");
//			deco.info.viewTabs.Append({ EGsChatViewType::SYSTEM, EGsChatViewType::NORMAL,
//					EGsChatViewType::SERVER, EGsChatViewType::WHISPER,
//					EGsChatViewType::GUILD, EGsChatViewType::INVASION, EGsChatViewType::PARTY,
//					EGsChatViewType::ALL });
//			baseTags.Add(deco);
//
//			deco.chatType = EGsExChatType::INTERSERVER;
//			deco.info.chatViewType = EGsChatViewType::SERVER;
//			deco.info.tagHeader = TEXT("World");
//			deco.info.viewTabs.Append({ EGsChatViewType::SERVER, EGsChatViewType::ALL });
//			baseTags.Add(deco);
//		}
//	}
//
//	if (gradeTags.Find(grade))
//	{
//		if (auto iter = gradeTags[grade].FindByPredicate([type](auto& tag) { return tag.chatType == type; }))
//			return iter;
//	}
//	if (auto iter = baseTags.FindByPredicate([type](auto& tag) { return tag.chatType == type; }))
//		return iter;
//	return nullptr;
//}

ChatType UGsChatManager::ExChatTypeToChatType(EGsExChatType exChatType)
{
	static TMap<EGsExChatType, ChatType> chatTypes =
	{
		TPair<EGsExChatType, ChatType>(EGsExChatType::NONE, ChatType::NONE),
		TPair<EGsExChatType, ChatType>(EGsExChatType::WORLD, ChatType::WORLD),
		TPair<EGsExChatType, ChatType>(EGsExChatType::NORMAL, ChatType::NORMAL),
		TPair<EGsExChatType, ChatType>(EGsExChatType::PARTY, ChatType::PARTY),
		TPair<EGsExChatType, ChatType>(EGsExChatType::EMOTICON, ChatType::EMOTICON),
		TPair<EGsExChatType, ChatType>(EGsExChatType::GUILD, ChatType::GUILD),
		TPair<EGsExChatType, ChatType>(EGsExChatType::INVADE, ChatType::INVADE),
		TPair<EGsExChatType, ChatType>(EGsExChatType::WHISPER, ChatType::WHISPER),
		TPair<EGsExChatType, ChatType>(EGsExChatType::INTERSERVER, ChatType::INTER),
		TPair<EGsExChatType, ChatType>(EGsExChatType::GROUP, ChatType::ALLY),
	};

	exChatType = (exChatType == EGsExChatType::WORLD && GGameData()->IsInInterWorld()) ? EGsExChatType::INTERSERVER : exChatType;

	if (auto iter = chatTypes.Find(exChatType)) return *iter;
	return static_cast<ChatType>(exChatType);
};

bool UGsChatManager::IsEnableChatForChatType(ChatType chatType)
{
	switch (chatType)
	{
	case ChatType::PARTY:	return GSParty()->GetIsInParty();
	case ChatType::WORLD:	return !FGsContentsServerHelper::IsLocatedOnBattleArenaGuildWar();
	case ChatType::INVADE:	return IsValidInvasionChat();
	case ChatType::GUILD:	return GGuild()->IsGuild();
	case ChatType::ALLY:	
	{
		bool enable = false;
		TArray<TWeakPtr<FGsGuildGroupChatRoom>> rooms;
		if (GGuild()->IsGuild() && GGuild()->GetGroupChatRooms(rooms) > 0)
		{	
			for (auto e : rooms)
			{
				enable = IsEnableGuildGroupChatInRoom(e.Pin()->GetRoomId());
				if (enable) break;
			}
		}
		return enable;
	}
	default: break;
	}
	return true;
}

bool UGsChatManager::IsEnableGuildGroupChatInRoom(uint64 roomId)
{
	if (auto room = GGuild()->GetGroupChatRoom(roomId).Pin())
	{
		auto authorGrade = static_cast<int>(room->GetAuthorizedGrade());
		auto myGrade = static_cast<int>(GGuild()->GetMyGuildData()->_memberGrade);

		return authorGrade <= myGrade;
	}
	return false;
}

void UGsChatManager::OpenWhisperInput(FString nickName, WorldId worldId /*= 0*/)
{
	TWeakObjectPtr<UGsUIPopupChat> chatPopup = Cast<UGsUIPopupChat>(GUI()->OpenAndGetWidget(TEXT("PopupChat")));
	if (auto chatUI = chatPopup.Get())
	{
		UGsUIPopupChat::PopupInitData initData{ EGsChatViewType::ALL, EGsExChatType::WHISPER, nickName, worldId };
		chatUI->InitializeData(&initData);
	}
}

void UGsChatManager::OnNotiMessage(FString text, FString nickName, time_t inputTime, SystemMessageType optType, TArray<uint32> options,
	EGsNotiSection SectionType, SectionPriority priority)
{
	bool isTicker = true;
	bool isDelayTicker = false;
	bool isTickerExcludeMine = true;
	bool isEnableGradeCheck = false;
	if (const UGsTable* table = FGsSchemaSystemMessage::GetStaticTable())
	{
		TArray<const FGsSchemaSystemMessage*> systemMessageList;
		table->GetAllRows<FGsSchemaSystemMessage>(systemMessageList);
		if (auto iter = systemMessageList.FindByPredicate([optType](const auto e) { return e->type == optType; }))
		{
			isTicker = (*iter)->useTicker;
			isDelayTicker = (*iter)->useDelayTicker;
			isTickerExcludeMine = (*iter)->excludeMineTicker;
			isEnableGradeCheck = (*iter)->useCofigGradeLimit;
		}
	}
	FString myUserName;
	if (auto userData = GGameData()->GetUserData())
	{
		myUserName = userData->mUserName;
	}

	int nameIndex;
	auto cmpName = nickName;
	if (nickName.IsValidIndex(0) && nickName.FindLastChar(ANONYMOUS_PLAYER, nameIndex))
	{
		auto nameLen = nickName.Len() - (nameIndex + 1);
		if (nameLen > 0) cmpName = nickName.Right(nameLen);
	}

	static auto CheckConfig = [](SystemMessageType type, uint32 optionVal)->bool {

		static int fairyGradeOffset = static_cast<int>(FairyGrade::NONE) - static_cast<int>(ItemGrade::NONE);
		static int costumeGradeOffset = static_cast<int>(CostumeGrade::NONE) - static_cast<int>(ItemGrade::NONE);

		int32 useGradeIndex = -1;
		switch (type)
		{
		case SystemMessageType::COSTUME_GET:
			if (auto data = GSCostume()->GetCostumeData(optionVal).Pin())
			{
				useGradeIndex = static_cast<int>(data->GetCostumeGrade()) - costumeGradeOffset;
			}
			break;
		case SystemMessageType::FAIRY_GET:
			if (auto data = GSFairy()->GetFairyData(optionVal).Pin())
			{
				useGradeIndex = static_cast<int>(data->GetFairyGrade()) - fairyGradeOffset;
			}
			break;
		default:
			if (auto data = FGsItem::GetTableDataByID(optionVal))
			{
				useGradeIndex = static_cast<int>(data->grade);
			}
			break;
		}
		if (auto setting = UGsGameOptions::GetGameUserOptions())
			return useGradeIndex >= 0 && setting->GetSystemMessageGradeOption(useGradeIndex);
		return true;
		};

	bool displayMessage = !isEnableGradeCheck || myUserName.Equals(cmpName) || !options.IsValidIndex(0) || CheckConfig(optType, options[0]);
	if (displayMessage)
	{
		//채팅출력 메세지는  TrayCondition::TC_Close로 해둔다 - movie 재생시 출력하지 않고 제거됨
		auto weakItem = OnSystemChat(text, nickName, inputTime, optType, options);
		if (isTicker || isDelayTicker)
		{
			if (!isTickerExcludeMine || !myUserName.Equals(cmpName))
			{
				if (auto chatItem = weakItem.Pin())
				{
					if (isDelayTicker)
					{
						FGsUIHelper::TraySectionMessageDelayTicker(SectionType, FText::FromString(chatItem->GetChatText()),
							0.2f, TrayCondition::TC_Close, priority);
					}
					else
					{
						FGsUIHelper::TraySectionMessageTicker(SectionType, FText::FromString(chatItem->GetChatText()), TrayCondition::TC_Close, priority);
					}
				}
			}
		}
	}
}
