#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "GsManager.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "DataSchema/SocialEmotion/GsSchemaSocialEmotion.h"
#include "DataSchema/GsSchemaEnums.h"

#include "Chat/GsChatItem.h"
#include "Chat/GsChatList.h"
#include "Option/GsGameUserSettings.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "GsChatManager.generated.h"

class FGsBanTextHandler;
struct FGsSchemaEmoji;
struct FGsSchemaSocialEmotion;

#pragma region FGsChatPool
//간단. 범용 아님
template<typename E>
struct T1PROJECT_API FGsChatPool
{
	using TYPE = typename TEnableIf<TIsDerivedFrom<E, IPoolChatType>::IsDerived, E>::Type;
private:
	TArray<TYPE*> _pool;

private:
	TYPE* Get() {
		if (_pool.Num() == 0)
		{
			_pool.Push(new TYPE());
		}
		return _pool.Pop();
	}
	void Release(TYPE* e) { e->Init(); _pool.Push(e);}

public:
	TSharedPtr<TYPE> GetItem() {
		return MakeShareable(Get(), [this](TYPE* item) { Release(item); });
	}
	void Flush() {
		while (_pool.Num() > 0)
		{
			if (auto e = _pool.Pop())
				delete e;
		}
		_pool.Reset();
	}
};
#pragma endregion

#pragma region FSocialEmotionMotion
//emoticon 제외
struct FSocialEmotionMotion
{
#define _MAXUINT32   ((uint32)~((uint32)0))
#define _MAXINT32    ((int32)(_MAXUINT32 >> 1))
#define _MAXUINT16	 ((uint16)~((uint16)0))

	const FGsSchemaSocialEmotion* _data{ nullptr };
	bool _isLock{ true };
	time_t _endCoolTime{ 0 };
	int32 _coolTimeSec{ 0 };

	FSocialEmotionMotion() = default;
	FSocialEmotionMotion(const FGsSchemaSocialEmotion* data, bool isLock) : _data(data), _isLock(isLock) {};

	int32 GetMotionId() { return _data ? _data->socialId : INDEX_NONE; }
	bool IsBaseMotion() { return _data ? _data->isLock == false : false; }
	bool isLoopMotion() { return _data ? _data->isLoopMotion : false; }
	bool IsPlayableMotion() { return IsBaseMotion() || (_data && _isLock == false); }
	int32 GetCoolTime() { return _data ? _data->secCoolTime : 0.f; }
	UserSocialEmotion GetSocialEmotionType() { return _data ? _data->socialCategory : UserSocialEmotion::MAX; }
	FSoftObjectPath GetEmoticonPath() { return _data ? _data->emoticon : FSoftObjectPath(); }
	int32 Order() { return _data ? _data->order : _MAXINT32; }
	ItemId GetAcquireItem() { return _data && _data->itemId.GetRow() ? _data->itemId.GetRow()->id : INVALID_ITEM_ID; }

	void SetUnlock() { _isLock = false; }
	void SetRunCoolTime(time_t coolTime) { _endCoolTime = coolTime; _coolTimeSec = coolTime - FGsTimeSyncUtil::GetServerNowSecond(); }

	int32 GetEndCoolTime() { return _coolTimeSec; }
	int32 GetRemainCoolTime() {
		time_t svrTime = FGsTimeSyncUtil::GetServerNowSecond();
		if (_endCoolTime - svrTime <= 0) _endCoolTime = 0;
		return _endCoolTime > 0 ? _endCoolTime - svrTime : 0;
	}
};
#pragma endregion


constexpr int MaxChatTab = static_cast<int>(EGsChatViewType::Max);
const int MaxGroupChatTab = 5; //나중에 config

UCLASS()
class T1PROJECT_API UGsChatManager : public UObject, public IGsManager
{
	GENERATED_BODY()

public:
	const int32 guildChatMaxRcv = 20;
	const int32 chatTabMax = 100;
	const int32 chatReportMax = 10;
	const int32 chatHudMax = 4;
	const int32 hudEraseSec = 60;
	const int32 gameTimeCheckSec = 3600;
	const float gameTimeCheckDelay = 60.f;

	static TArray<const TCHAR*> worldStartDelim;

protected:
	TArray<FGsChatList> _chatTabList;	// Popup tab으로 보이는 채팅 리스트
	TArray<TSharedPtr<FSocialEmotionMotion>> _socialMotions;

	bool _motionInit{ false };

private:
	FGsChatPool<FGsChatItem> _chatPool;
	ChatArray _mngChatList;
	WorldId _whisperWorldId{ 0 };

	static WorldId homeWorldId;				// 유저 Prefix를 위한 현재 월드
private:
	EGsExChatType _lastInputChatType{ EGsExChatType::NONE }; // 도배 등으로 인한 채팅 금지 타입
	time_t _chatInputEnableTime{ 0 };		// 도배 등으로 인한 채팅 금지 시간

	bool _useBanTIme{ false };
	FDateTime _endOperatingBannedTime;

	time_t _lastInputEndTime{ 0 };			// 허드 챗 Remove 위한 최종 입력시간

private:
	GuildChatDBId _reqGuildChatSeq{ 0 };	// 요청해야 하는 기사단 채팅 sequence
	bool _isRemovedGuildChat{ false };
	int32 _maxRcvGuildCount = guildChatMaxRcv;

private:
	float _viewTimeGameRatingSec{ 5.0f };
	static double _lastNotiGameTime;
	FTimerHandle _gameTimeHandle;

protected:
	EGsChatViewType _hudChatType{ EGsChatViewType::ALL };
	uint64 _groupChatID{ 0 };

protected:
#pragma region InvasionChat
	struct InvasionChat
	{
		struct _CURRENCY
		{
			CurrencyType type{ CurrencyType::DIA };
			int32		 amount{ 10 };
		} currency;

		struct _COUNT
		{
			uint16 dailyCount{ 5 };
			uint16 remainCount{ 5 };
		} free;

		bool isValid{ false };
		bool isEnableFree{ false };

		void SetCurrency(CurrencyType type, int32 amount) 
		{
			currency.type = type; 
			currency.amount = amount;
		}
		void SetConsumed() 
		{
			if (isEnableFree == false) return;

			free.remainCount--;
			isEnableFree = free.remainCount > 0;
		}
		void RefreshFree(int16 remain = _MAXUINT16)
		{
			if (remain == _MAXUINT16)
				 free.remainCount = free.dailyCount;
			else free.remainCount = remain;

			isEnableFree = free.remainCount > 0;
		}

	} _invasionChat;
#pragma endregion

public:
	using InvasionChatCurrency = InvasionChat::_CURRENCY;

	void Initialize() override;
	void Finalize() override;
	
private:
	bool CheckGameTime(bool isCheck);

	void InitChat();
	void ClearChat(EGsChatViewType chatType = EGsChatViewType::ALL); //all clear
	
protected:
	TSharedPtr<FGsChatItem> AddChatItem(FGsChat chat, SystemMessageType optType = SystemMessageType::MAX, TArray<uint32> options = TArray<uint32>());
	TSharedPtr<FGsChatItem> InsertChatItem(FGsChat chat, SystemMessageType optType = SystemMessageType::MAX, TArray<uint32> options = TArray<uint32>());

public:
	void OnCommonChatList(ChatType type, TArray<FGsChat> chat);
	void OnCommonChat(FGsChat& chat);	// 대다수의 모든 챗
	void OnEmoticonChat(uint64 uid, FString nickName, int32 emoticonIndex);	// 이모티콘 챗

	// 시스템 메세지
	TWeakPtr<FGsChatItem> OnSystemChat(FString text, FString nickName = TEXT("SYSTEM"), time_t inputTime = 0,
		SystemMessageType optType = SystemMessageType::NOTICE, TArray<uint32> options = {});

	TWeakPtr<FGsChatItem> OnOperatingMessage(FString text, NoticeType type = NoticeType::General, FString nickName = TEXT("SYSTEM"), time_t inputTime = 0);

	TWeakPtr<FGsChatItem> OnGroupChatSystemMessage(FString text, GuildGroupChatRoomId roomId);

	// 길드 챗 리스트
public:
	// *seq 다음 요청해야 하는 sequence 거나 더 이상 선저장된 챗이 없을 때
	void OnGuildChatList(TArray<FGsChat>& list, uint64 seq = 0);
	// 길드 챗 리스트 요청에 대한 ACK
	void AckGateWayGuildChatList(int result = PACKET_RESULT_SUCCESS);

	bool IsValidPreGuildChatList() { return UINT64_MAX > _reqGuildChatSeq; }

	void SendReqGuildChat();
	void OnRawGuildChattoItem(TArray<FGsGuildChatList> arr);

	//침공 챗
public:
	void SetRemainFreeInvasionChat(uint16 remainCount, bool refresh = false);
	void ResetFreeInvasionChatCount(ResetTimeTicket InType);
	void SetConsumedFreeInvasionChat();
	void SetIsValidInvasionChat(bool invadeChatValid) { _invasionChat.isValid = invadeChatValid; }

	const InvasionChat& GetInvasionChatInfo() { return _invasionChat; }
	uint16 GetRemainFreeInvasionChat() { return _invasionChat.isEnableFree ? _invasionChat.free.remainCount : 0; } // 남은 free침공 챗 회수
	uint16 GetInvasionFreeChatCount() { return _invasionChat.free.dailyCount; }  // 하루당 free 챗 회수

	bool IsValidInvasionChat() { return _invasionChat.isValid; }
	bool IsEnableFreeChat(EGsExChatType chatType = EGsExChatType::INVADE) { 
		return chatType == EGsExChatType::INVADE ? _invasionChat.isEnableFree : true; }
	InvasionChatCurrency GetPayinvasionChat() { return _invasionChat.isEnableFree ? 
		InvasionChatCurrency{ CurrencyType::NONE, 0 } : _invasionChat.currency; }

	// 감정 이모티콘, social 이모티콘
public:
	void UnlockSocialEmotionList(TArray<SocialEmotionInfo>& emotionList, bool refresh = false);
	void OnUnlockSocialEmotion(UserSocialEmotion emotion, bool isOpen = true);
	void UpdateSocialEmotion(int64 gameId, UserSocialEmotion emotionType);

	bool IsResetMotion() { return _motionInit; }
	void ResetMotionFlag(bool set) { _motionInit = set; }

	// 월드가 다른 유저와의 귓속말이 유지되고 있을 때, worldId를 매번 입력하지 않도록 기록해둠
public:
	void SetToWhisperWorldId(WorldId worldId) { _whisperWorldId = worldId; }
	WorldId GetWhisperTargetWorldId() { return _whisperWorldId; }

	// 차단 리스트
public:
	void OnAddedBlockList(UserDBId uid, FString userName);
	void OnRemovedBlockList(UserDBId uid);
	bool IsNoneChatBlockUser(FString userNick); // 차단 block user check

public:
	void OnCheckPartyChat(bool inParty);	//파티 채팅 상태 갱신
	void OnCheckGuildChat();				//길드 채팅 상태 갱신
	void OnCheckUserExistByName(bool isValidName, WorldId worldId = 0);	//귓속말 전에 유저가 실제하는지 체크 결과

	// 채팅 리스트 
public:
	int  GetChatDataList(EGsChatViewType chatType, OUT TArray<TWeakPtr<FGsChatItem>>& chatList, uint64 groupId = 0);
	int32 GetHudChatList(TArray<TWeakPtr<FGsChatItem>>& chatList); //HudChat List

	void SetSelectedHudChat(EGsChatViewType tab, uint64 roomId = 0);

	// 채팅 옵션
public:
	void SetChatViewOption(EGsOptionChat type, bool enable, uint64 roomId = 0, bool modify = false);
	bool GetChatViewOption(EGsOptionChat type, uint64 roomId = 0);
	bool GetChatViewOption(EGsExChatType chatType, uint64 roomId = 0);

	void SetGroupChatViewOption(uint64 roomId, bool enable, bool modify = false);
	int GetChatGroupViewOptions(TArray<uint64>& hiddenGroup);
	bool GetChatGroupViewOption(uint64 roomId);
	// 채팅 매크로
public:
	void SetChatMacro(int8 index, const FText& text, bool modify = false);
	const FText& GetChatMacro(int8 index);

public:
	int32 GetSocialMotionList(TArray<TWeakPtr<FSocialEmotionMotion>>& motionList);
	int32 GetEmoticonList(TArray<const FGsSchemaEmoji*>& emoticonList);

	TWeakPtr<FSocialEmotionMotion> GetSocialMotion(int32 socialId);
	TWeakPtr<FSocialEmotionMotion> GetSocialMotionByType(UserSocialEmotion emotionType);

public:
	bool IsNoSpammingChat(EGsExChatType curType);
	void SetBannedSpammingChat(time_t spammingBaannedTime, time_t operatingBannedTime = 0);

private:
	void LoadSocialMotion();
	void LoadChatInfo();

	bool OnCheatChat(FString name, FString text, uint64 uid);

	//bool GetMessageAppendableTabs(TSharedPtr<FGsChatItem> chatItem, OUT TArray<EGsChatViewType>& addTabs);
	//static const ChatDeco* LoadChatTags(AccountGrade grade = AccountGrade::NORMAL, EGsExChatType type = EGsExChatType::NORMAL, bool reset = false);

	static void MakeChatMarkup(FGsChatItem& chat);
	static void MakeSystemMessageMarkup(FGsChatItem& chat);

	static bool IsWhisperChatSentByMe(FGsChat& chat);
	static bool IsAllyGuildChatByMyGuild(FGsChat chat);

	// 채팅 Utils
public:
	// 시스템 메세지 Tag Parse
	static FString AddParseSystemText(const FString& inText, FString header = TEXT("<System>"));
	// 시스템 메세지 post Compiler & Tag 파싱 후 처리
	static FText TextPostScriptParser(FString& useString, const FFormatOrderedArguments& InOrderedArguments);

	// popup 입력된 채팅->서버 채팅으로 바꿈 ( 채팅 타입으로 분류되어 있지 않고 따로 패킷을 사용하는 채팅 타입 처리 )
	ChatType ExChatTypeToChatType(EGsExChatType exChatType);

	bool IsEnableChatForChatType(ChatType chatType);
	bool IsEnableGuildGroupChatInRoom(uint64 roomId);

	EGsChatViewType GetCurrentHudChatViewType() { return _hudChatType; }

public:
	//정보창에서 요청시 귓속말 채팅 popup open
	void OpenWhisperInput(FString nickName, WorldId worldId = 0);	//정보창에서 요청시 귓속말 채팅 popup open

	// 티커까지 같이 출력해야 하는 시스템 메세지
	void OnNotiMessage(FString text, FString nickName = TEXT("SYSTEM"), time_t inputTime = 0,
		SystemMessageType optType = SystemMessageType::NOTICE, TArray<uint32> options = {},
		EGsNotiSection SectionType = EGsNotiSection::NOTIFICATION, SectionPriority priority = SectionPriority::None);
};

#define GSChat() UGsScopeHolder::GetGameManager<UGsChatManager>(UGsScopeGame::EManagerType::Chat)