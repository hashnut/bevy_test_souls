#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

enum class EGsContextContentsMenu : uint16
{
	NONE,
	USER_PROFILE,			// 2023/9/11 PKT - 프로필
	GUILD,					// 2023/9/11 PKT - 길드
	ADD_FRIEND,				// 2023/9/11 PKT - 친구 등록
	INVITE_PARTY,			// 2023/9/11 PKT - 파티 초대
	WHISPER,				// 2023/9/11 PKT - 귓속말
	USER_BLOCK,				// 2023/9/11 PKT - 차단
	ARENA,					// 2023/9/11 PKT - 아레나
	NAME_COPY,				// 2023/9/11 PKT - 이름 복사
	USER_REPORT,			// 2023/9/11 PKT - ???
	USER_BOUNDARYTARGET,	// 2023/9/11 PKT - 경계 대상
	PERSONAL_TRADE,			// 2023/9/11 PKT - 개인 거래
};

enum class EGsUserContentsOwner
{
	NONE,
	HUD_HPBAR,				// 2023/9/12 PKT - HUD HP Bar에서 요청
	GUILD_MEMBER,			// 2023/9/12 PKT - 길드 정보에서 요청
	BUDDY_MEMBER,			// 2023/9/12 PKT - 친구 목록에서 요청
	PARTY_MEMBER,			// 2023/9/12 PKT - 파티 목록에서 요청
	RANKING,				// 2023/9/12 PKT - 랭킹 정보에서 요청
	USER_PROFILE,			// 2023/9/12 PKT - 프로필에서 요청
	USER_CHEAT,				// 2023/9/12 PKT - 채팅에서 요청	
};


class T1PROJECT_API FGsUICommonUserContext
{
public:

	// 2023/9/11 PKT - User Info에서 활성화 되어야 할 정보
	//				 - 플레이어의 서버 위치, User와의 관계등에 따라 활성화 되는 UserInfo 기능들에 대한 정의
	// Return 1. EGsContextContentsMenu - 컨텐츠 타입( 버튼 활성 화 여부 )
	// Return 2. bool - 컨텐츠에 따라 버튼이 Switching이 되어야 하는 경우가 있음.( Default = false )
	const TMap<EGsContextContentsMenu, bool> GetActivationFeature(const struct FGsUserContextInfo* InUserInfo, const class UGsGameObjectRemotePlayer* InRemotePlayer) const;

	// 2023/9/12 PKT - User Info에서 활성화 되어야 할 정보( Ban 정보 포함. 호출 되는 위치에 따라 각 비활성화 되는 컨텐츠는 Ban 시킴 )
	const TMap<EGsContextContentsMenu, bool> GetActivationFeatureWithBan(const EGsUserContentsOwner InOwner, const struct FGsUserContextInfo* InUserInfo, const class UGsGameObjectRemotePlayer* InRemotePlayer) const;

protected:
	bool IsAreanArea(const FVector& InTargetLocated) const;

	/**
	 * 2023/9/11 PKT - Switching( 많을 줄 알았는데.. 경계대상 외에는 스위칭 없음... ㄷㄷ
	 */
	bool IsBoundaryTargetSwitching(const struct FGsUserContextInfo* InUserInfo) const;


	// 2023/9/11 PKT - 클라 -> 서버( Param 인자를 UserConte
protected:
	// 2023/9/11 PKT - 프로필 요청
	void SendUserProfile(UserDBId InUserDBId);
	// 2023/9/13 PKT - 길드 정보 요청
	void SendGuildInfo(const struct FGsUserContextInfo* InUserInfo);
	// 2023/9/11 PKT - 파티 요청
	void SendInviteParty(const FString& InUserName, const WorldId InWorldId);
	// 2023/9/13 PKT - 친구 추가
	void SendAddFriend(UserDBId InUserDBId, const FString& InUserName, WorldId InWorldId);
	// 2023/9/11 PKT - 유저 정보 갱신
	bool SendUserContextRefresh(const FString& InUserName, WorldId InWorldId);
	// 2023/9/12 PKT - 대결 신청(아레나)
	bool SendDuelInvite(UserDBId InUserDBId, Level InLevel, FString InUserName, bool InIsVisible);
	// 2023/9/11 PKT - 귓속말
	void SendWhisper(const UserDBId InUserDBId, const FString& InUserName, const WorldId InWorldId);
	// 2023/9/11 PKT - 이름 카피
	bool SendNameCopy(const FText& InUserName, const WorldId InWorldId);
	// 2023/9/11 PKT - 경계 대상
	void SendBoundaryTarget(bool IsAdd, const UserDBId InUserDBId, const WorldId InWorldId);
	// 2023/9/11 PKT - 개인 거래 요청
	void SendPersonalTrading(int64 InGameId, const FString& InUserName);
	// 2023/9/13 PKT - 차단
	void SendUserBlock(const UserDBId InUserDBId, const FString& InName);
	// 2023/9/13 PKT - 신고
	void SendUserRepot(const FString& InName, const FString& InChatText);

	// 2023/9/13 PKT - 귓속말 / 파티 등 을 요청 할때 내 자신에 대한 요청인지 체크 하기 위함.
protected:
	bool IsTargetSelf(const UserDBId InUserDBId, const FText& InName);
};