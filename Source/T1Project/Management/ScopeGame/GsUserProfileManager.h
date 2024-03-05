#pragma once

#include "CoreMinimal.h"

#include "Classes/GsManager.h"

#include "GameObject/Stat/GsStatInfo.h"

#include "Item/GsItem.h"

#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageSystem.h"

#include "RedDot/GsRedDotCheck.h"

#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "UI/UIContent/Define/GsUIContentDefine.h"

#include "UTIL/GsTimeUtil.h"

#include "EventProgress/GsEventProgressInterface.h"


struct UserComment
{
	UserDBId _userDBId;
	FString _userName;
	FString _commentText;
	time_t _writeTime;

	UserComment(UserDBId InUserDBId, const FString& InUserName, const FString InCommentText, time_t InWriteTime)
		: _userDBId(InUserDBId), _userName(InUserName), _commentText(InCommentText), _writeTime(InWriteTime) 
	{}
};
/**
 *  소셜 컨텐츠 유저 프로필 매니저
 */
class T1PROJECT_API FGsUserProfileManager final : public IGsManager, public IGsRedDotCheck, public IGsEventProgressInterface
{
protected:
	struct UserProfile
	{
		bool _isVisible;	// 정보 공개 여부
		UserDBId _userDBId;	// 유저 ID
		FString _userName;	// 유저 이름
		Level _userLevel;	// 유저 레벨
		TArray<StatInfo> _statInfoList;	// 스탯 정보 목록
		TMap<EGsEquipSlotType, ItemDataEquip> _equippedItemList;	// 장착한 아이템
		CreatureGenderType _creatureGenderType;
		UserLookInfo _userLookInfo;
		CustomizeFaceData _customizingFaceData;
		CustomizeBodyData _customizingBodyData;
		uint64 _mostKeywordId;
		uint64 _mostKeywordCount;
		bool _isExistGuild;
		FString _guildName;
		GuildEmblemId _guildEmblemId;
		GuildMemberGradeType _memberGrade;
		FString _introductionText;
		AccountGrade _accountGrade;	// 계정 등급
		uint32 _rank;	// 레벨 순위 : 뱃지 표시 용도

	public:
		void Clear();
	};

public:
	MsgSystemHandleArray _msgHandleSystemList;
	MsgGameObjHandleArray _msgHandleGameObjectList;

protected:
	TArray<UserDBId> _openedUserList;
	TUniquePtr<UserProfile> _currentProfile;	// 열린 프로필의 정보
	TArray<UserComment> _currentCommentList; // 열린 프로필의 한마디 목록
	TMap<uint64, uint64> _currentKeywordCountMap;	// 열린 프로필의 키워드 정보 (ID, Count)
	uint64 _selectedKeyword;
	TArray<TPair<FString, time_t>> _nickNameChangeList; // 열린 프로필의 닉네임 변경 이력

	bool _isMyProfile = false;

	FGsSimpleTimer _commentTimer;
	bool _canRequestComment = false;
	bool _existOldCommentInServer = true;	// 스크롤 시 받은 코멘트가 없다면 false로 전환
	FDelegateHandle _timerDelegate;

	bool _loadTopoStart = false;

	// 가상 함수
public:
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void Update(float inTick) override;

protected:
	void BindMessages();
	void UnbindMessages();

public:
	virtual bool CheckRedDot(bool bInUpdate = true) override;

public:
	void Set(PD::GC::PKT_GC_ACK_USER_PROFILE_READ& InPacket);
	void Set(PD::GC::PKT_GC_ACK_USER_PROFILE_INTRODUCTION_UPDATE_READ& InPacket);
	void Set(PD::GC::PKT_GC_ACK_USER_PROFILE_KEYWORD_INFO_READ& InPacket);
	void Set(PD::GC::PKT_GC_ACK_USER_PROFILE_KEYWORD_UPDATE_READ& InPacket);
	void Set(PD::GC::PKT_GC_ACK_USER_PROFILE_COMMENT_LIST_READ& InPacket);
	void Set(PD::GC::PKT_GC_ACK_USER_PROFILE_COMMENT_INPUT_READ& InPacket);
	void Set(PD::GC::PKT_GC_ACK_USER_PROFILE_COMMENT_DELETE_READ& InPacket);
	void SetNickNameChangeList(PD::GC::PKT_GC_ACK_USER_PROFILE_NICKNAME_CHANGE_LIST_READ& InPacket);

public:
	bool IsExistProfile();
	bool IsMyProfile() { return _isMyProfile; }
	bool IsExistGuild();
	bool IsVisible();
	bool IsGM();

public:
	UserDBId PopLastOpenedUserDBID();
	UserDBId GetTargetUserDBID();
	FString GetTargetUserName();
	Level GetTargetUserLevel();
	uint64 GetMostKeywordId();
	uint64 GetMostKeywordCount();
	uint64 GetKeywordCount(uint64 InKeywordId);
	uint64 GetSelectedKeyword();
	FString GetTargetGuildName();
	GuildEmblemId GetTargetGuildEmblemId();
	GuildMemberGradeType GetTargetGuildMemberGrade();
	FString GetIntroductionText();
	uint32 GetRank();
	
	bool GetTargetUserStat(OUT TArray<StatInfo>& OutStatInfoList);
	const ItemDataEquip* GetTargetUserItemDataEquip(EGsEquipSlotType InSlotType);
	CreatureGenderType GetGenderType();
	const UserLookInfo* GetUserLookInfo();
	const CustomizeFaceData* GetCustomizeFaceData();
	const CustomizeBodyData* GetCustomizeBodyData();
	TArray<TPair<FString, time_t>> GetNickNameChangeList() const { return _nickNameChangeList; }

public:
	time_t GetLastCommentWriteTime();
	int32 GetCommentCount();
	const UserComment* GetComment(int32 InIndex);

public:
	void SetKeywordCountPlus(uint64 InKeywordId);
	void SetKeywordCountMinus(uint64 InKeywordId);

public:
	bool CanRequestComment();
	void RequestComment();
	void ResetCommentTimer();
	void RefreshComment();
	void DeleteComment(UserDBId InUserDBId);

public:
	void Clear();

public:
	void ErrorInvalidUser();

protected:
	void OnLoadTopoStart(const IGsMessageParam* inParam);
	void OnLocalSpawnMeComplete(const IGsMessageParam*);

public:
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;
};

#define GSUserProfile() UGsScopeHolder::GetGameManagerFType<FGsUserProfileManager>(UGsScopeGame::EManagerType::UserProfile)