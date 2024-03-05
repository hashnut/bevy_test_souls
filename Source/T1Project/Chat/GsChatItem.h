#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedChatEnum.h"
#include "Shared/Client/SharedEnums/SharedSystemMessageEnum.h"
#include "DataSchema/GsSchemaEnums.h"
#include "ContentsServer/GsContentsServerHelper.h"


constexpr EGsChatViewType INVALID_CHAT_TAB = EGsChatViewType::Max;
constexpr EGsExChatType INVALID_EXCHAT_TYPE = EGsExChatType::NONE;

struct T1PROJECT_API FGsChat
{
	EGsExChatType _type{ EGsExChatType::NORMAL };
	int64 _gameId{ 0 };	// user id

	time_t _inputTime{ 0 };	// chat input time - 화면 표시
	FString _nickName;
	FString	_text;

	WorldId _homeWorldId{ INVALID_WORLD_ID };
	FString _homeWorldPrefix;

	AccountGrade _accountGrade{ AccountGrade::NORMAL };
	Rank _userRank{ 0 };

	uint64  _seq{ 0 };
	FString _senderName;

	GuildDBId	  _guildId{ INVALID_GUILD_DB_ID };
	GuildEmblemId _emblemId{ INVALID_GUILD_EMBLEM_ID };

	GuildGroupChatRoomId _roomId{ 0 };

	static EGsExChatType ChangeType(ChatType type)
	{
		static TMap<ChatType, EGsExChatType> ExChatTypes =
		{
			TPair<ChatType, EGsExChatType>(ChatType::NONE, EGsExChatType::NONE),
			TPair<ChatType, EGsExChatType>(ChatType::WORLD, EGsExChatType::WORLD),
			TPair<ChatType, EGsExChatType>(ChatType::NORMAL, EGsExChatType::NORMAL),
			TPair<ChatType, EGsExChatType>(ChatType::PARTY, EGsExChatType::PARTY),
			TPair<ChatType, EGsExChatType>(ChatType::EMOTICON, EGsExChatType::EMOTICON),
			TPair<ChatType, EGsExChatType>(ChatType::ALLY, EGsExChatType::GROUP),
			TPair<ChatType, EGsExChatType>(ChatType::GUILD, EGsExChatType::GUILD),
			TPair<ChatType, EGsExChatType>(ChatType::INVADE, EGsExChatType::INVADE),
			TPair<ChatType, EGsExChatType>(ChatType::WHISPER, EGsExChatType::WHISPER),
			TPair<ChatType, EGsExChatType>(ChatType::INTER, EGsExChatType::INTERSERVER),
		};

		if (auto iter = ExChatTypes.Find(type)) return *iter;
		return static_cast<EGsExChatType>(type);
	}

};

struct T1PROJECT_API IPoolChatType
{
	virtual void Init() = 0;
};

struct T1PROJECT_API FGsChatItem : public IPoolChatType
{
	EGsExChatType _type{ EGsExChatType::NORMAL };
	FGsChat _chatData;

	FString _namePrefix;
	FString _senderNick;

	SystemMessageType _optionType = SystemMessageType::NOTICE;
	TArray<uint32> _options;

	FString _markupPrefix;
	FString _markupNick;
	FString _markupText;
	FString _markupTime;

	EGsChatViewType _chatViewType{ EGsChatViewType::NORMAL }; //chat이 보여지는 tab
	bool _isMyWorldPlayer{ true };
	bool _useWorldPrifix{ false };

	FString _markupTextFormat{ TEXT("{0}{1}") };

	bool _isSentWhisper{ false };

	bool _isMyGulidMember{ false };

	TArray<EGsChatViewType> _viewTabs;

	FGsChatItem() = default;
	FGsChatItem(FGsChat chatData, TFunction<void(FGsChatItem& item)> markup = nullptr)
		:  _type(chatData._type), _chatData(chatData), _senderNick(chatData._senderName), _isSentWhisper(false), _isMyGulidMember(false)
	{
		if (markup != nullptr)
		{
			markup(*this);
		}
	}

	void Init() override//재사용 변수이므로, 반환시에 깨끗하게 해줘야 한다.
	{
		_type = EGsExChatType::NORMAL;

		FGsChat emptyChat;
		Swap(emptyChat, _chatData);

		_namePrefix.Empty();
		_senderNick.Empty();

		_markupPrefix.Empty();
		_markupNick.Empty();
		_markupText.Empty();
		_markupTime.Empty();

		_chatViewType = EGsChatViewType::NORMAL;
		_optionType = SystemMessageType::NOTICE;
		_options.Empty(0);

		_viewTabs.Reset();

		_isMyWorldPlayer = true;
		_useWorldPrifix = false;

		_isSentWhisper = false;

		_isMyGulidMember = false;
	}

	void SetData(FGsChat chatData, TFunction<void(FGsChatItem& item)> markup = nullptr, bool isMyWorldPlayer = true, bool isMyGuildMember = false)
	{
		_type = chatData._type ;
		_chatData = CopyTemp(chatData);

		_namePrefix = chatData._homeWorldPrefix;
		_senderNick = chatData._senderName;

		_isMyWorldPlayer	= isMyWorldPlayer;
		_isMyGulidMember	= isMyGuildMember;
		_namePrefix = FGsContentsServerHelper::GetWorldIPrefixdWithWorldId(chatData._homeWorldId).ToString();

		if (markup != nullptr)
		{
			markup(*this);
		}
	}

	void SetSystemData(SystemMessageType optType = SystemMessageType::NOTICE, TArray<uint32> options = { },
		TFunction<void(FGsChatItem& item)> markup = nullptr)
	{	
		_optionType = optType;
		_options = options;

		if (optType != SystemMessageType::MAX)
		{
			_useWorldPrifix = false;
			if (markup != nullptr)
			{
				markup(*this);
			}
		}
	}

	virtual ~FGsChatItem() {};

	FString GetMarkupName() 
	{
		auto markupPrefix = _markupPrefix.IsEmpty() || !_useWorldPrifix ? TEXT("") : _markupPrefix;
		return FString::Format(*_markupTextFormat, { markupPrefix, _markupNick });
	}

	time_t GetInputTime() { return _chatData._inputTime; }
	uint64 GetRoomId() { return _chatData._roomId;  }
	uint64 GetSeqNumber() { return _chatData._seq;  }
	WorldId GetHomeWorldId() { return _chatData._homeWorldId;  }
	AccountGrade GetAccountGrade() { return _chatData._accountGrade; }
	Rank GetUserRank() { return _chatData._userRank;  }
	FString GetNickName() { return _chatData._nickName; }
	int64 GetGameId() { return _chatData._gameId;  }
	GuildEmblemId GetGuildEmblemId() { return _chatData._emblemId;  }

	FString GetChatText() { return _chatData._text; }
	void SetChatText(FString text) { _chatData._text = text; }
};

struct T1PROJECT_API FGsChatHudOption
{
	EGsChatViewType _type = EGsChatViewType::NORMAL;
	bool _isView = true;
};
