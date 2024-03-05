#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"

class FGsNetSendService
{
	//Auth
public:
	//static void SendAuthPing();
	static void SendAuthReqPassword(const FString& inId, const FString& inPassword);
	static void SendAuthReqCheckVersion();
	static void SendAuthReqPreLogin(const uint64 inVid);
	static void SendAuthReqLogin(const bool InUseZpay = false);
	static bool SendAuthReqSelectServer(uint16 inPlanetWorldId);
	static bool SendAuthReqAppGuardAuth();
	static bool SendAuthReqDeleteAccount();

	// 계정충돌 팝업 정보 요청
	static void SendAuthReqIdpConflictPlayer();

	//Gateway
public:
	//static void SendGatewayPing();
	static void SendGatewayReqLogin();
	static void SendGatewayReqLoginWaitingInfo();
	static void SendGatewayReqLoginWaitingCancel();
	static void SendGatewayGameGuardCSA(const uchar* InContents, const uint32 InContentsSize);
	static void SendGatewayReqUserList();
	static void SendGatewayReqCreateUser(
		const FString& name, CreatureGenderType genderType, const CustomizeFaceData& faceDataSize, const CustomizeBodyData& bodyDataSize
	);
	static void SendGatewayReqCustomizing(CreatureGenderType genderType, const CustomizeFaceData& faceDataSize, const CustomizeBodyData& bodyDataSize);
	static void SendGatewayReqSelectUser(UserDBId userId);
	static void SendGatewayReqDeleteUser(UserDBId userId);
	static void SendGatewayReqDeleteUserCancel(UserDBId userId);	
	static void SendGatewayLoadTopoFin(int32 areaId);
	static void SendGatewayReturnToLobby();

	static void SendWorldAdminCommandQA(const FString& command, bool inSendBlock = false);
	static void SendRequestReconnect();
	static void SendRequestWorldTime();

	static void SendGatewayServerPong(uint64 pingSeq, uint64 inServerTickCount);

	// 친구	v1														
	static void SendBuddyList();								// 현재 친구, 요청 , 초대 리스트 한번에 요청 
	static void SendBuddyInvite(const FString& inUserName);		// 친구 초대
	static void SendBuddyDelete(const UserDBId inUserId);		// 친구 삭제
	static void SendBuddyAccept(const UserDBId inUserId);		// 친구 요청 수락
																
	static void SendBuddyDeleteInvitation(const UserDBId inSenderUserDBId, 
											const UserDBId inReceiverUserDBId);		// 친구 요청 거절	

	// 친구	v2
	static void SendBuddyList_v2();														// 현재 친구, 요청 , 초대 리스트 한번에 요청 
	static void SendBuddyInvite_v2(WorldId inWorldId, const FString& inUserName);		// 친구 초대
	static void SendBuddyDelete_v2(const UserDBId inUserId);							// 친구 삭제
	static void SendBuddyAccept_v2(const UserDBId inUserId);							// 친구 요청 수락

	static void SendBuddyDeleteInvitation_v2(const UserDBId inSenderUserDBId,
		const UserDBId inReceiverUserDBId);												// 친구 요청 거절	

	static void SendBuddyTabRemoveRedDot(const BuddyTabType inTabType);					// 친구 레드닷 삭제

	// 채팅
	static void SendReqChat(ChatType chatType, const FString chatMessage, bool isPayChat = false, uint64 roomId = 0);
	static void SendReqWhisper(const FString toChatNick, const FString chatMessage, WorldId worldId = 0);
	static void SendReqReportChat(const FString& nickName, const FString& chatContent, ReportBadChatType type);
	static void SendReqUnblockChatUser(UserDBId uid);
	static void SendReqBlockChatUser(const FString& nickName);
	static void SendReqCheckHomeUserExistByName(const FString& nickName);

	// 랭킹 시스템
	static void SendReqRankingList(RankType InRankingType, WorldId InWorldId, int32 InMinRank);

	// party
public:
	static void SendPartyCreate(PartyDropOwnershipType In_type);
	static void SendPartyInvite(const FString& In_inviteeName, WorldId In_homeWorldId);
	static void SendPartyConfirmInvitation(bool In_accept);
	static void SendPartyLeave();
	static void SendPartyKick(int In_partySlotIndex);
	static void SendPartyChangeLeader(int In_partySlotIndex);
	static void SendPartyChangeDropOwnershipType(PartyDropOwnershipType In_type);
	

	// 2022/06/13 PKT - 파티 던전 리스트
	static void SendReqPartyDungeonpartyList(DungeonGroupId inId);
	// 2022/06/13 PKT - 파티 던전 파티 생성
	static void SendReqPartyDungeonCreateParty(DungeonGroupId InId, DungeonDifficulty InDifficulty);
	// 2022/06/13 PKT - 파티 던전 자동 매칭
	static void SendPartyDungeonAutoMatching(DungeonGroupId InId, DungeonDifficulty InDifficulty);
	// 2022/06/15 PKT - 자동 매칭 취소
	static void SendPartyDungeonAutoMatchingCancel();
	// 2022/06/13 PKT - 파티 던전 참가
	static void SendPartyDungeonJoin(PartyId InPartyId);
	// 2022/06/13 PKT - 파티 던전 난입
	static void SendPartyDungeonBreakInto(PartyId InPartyId);
	// 2022/06/13 PKT - 파티 던전 입장하기 위해 준비 상태 설정
	static void SendPartyDungeonReady(bool InIsReady);
	// 2022/06/13 PKT - 파티 던전 입장
	static void SendPartyDungeonEntrance();
	// 2022/06/13 PKT - 파티 던전 도움 요청
	static void SendPartyDungeonAskHelp();
	
	// 컨텐츠 언락 시스템
public:
	static void SendReqUpdateUnlockInfo(int32 InSerialNumber, uint64 InLevel);

	// 플레이어블 이벤트
public:
	static void SendReqPlayableEventUpdate(int In_checkPoint);

	// 소셜 컨텐츠 : 유저 프로필
public:
	static void SendReqUserProfile(UserDBId InProfileUserDBId);	// 유저 소셜 프로필 요청
	static void SendReqUserProfileIntroductionUpdate(const FString& InIntroductionText);					// 유저 프로필 자기소개 편집 요청
	static void SendReqUserProfileKeywordInfo(UserDBId InProfileUserDBId);									// 유저 소셜 키워드 정보 요청
	static void SendReqUserProfileKeywordUpdate(UserDBId InProfileUserDBId, uint64 InKeywordId);			// 유저 소셜 키워드 투표 요청
	static void SendReqUserProfileCommentList(UserDBId InProfileUserDBId, time_t InLastCommentTime);		// 유저 한마디 정보 요청
	static void SendReqUserProfileCommentInput(UserDBId InProfileUserDBId, const FString& InCommentText);	// 유저 한마디 작성 요청
	static void SendReqUserProfileCommentDelete(UserDBId InProfileUserDBId, UserDBId InCommentUserDBId);	// 유저 한마디 삭제 요청
	static void SendReqUserProfileNickNameChangeList(UserDBId InUserDBId); // 유저 닉네임 변경이력 요청

	// 유저 컨텍스트 정보 요청
public:
	static void SendReqUserContext(const FString& InUserName);

	// 커뮤니티 소녀 말풍선 정보 요청
public:
	static void SendReqCommunityTalkRank();
	static void SendReqCommunityTalkMarket();

	// window KeyboardMapping
	static void SendKeyboardMappingSave();
	static void SendKeyboardMappingLoad();

	// 계정 삭제
public:
	static bool SendReqDeleteAccount();

	// Z Pay
public:
	static void SendZpayLogin();

public:
	// 2023/5/25 PKT - 클라이언트 자동 종료시 오프라인 플레이를 진행 하기 위해 미리 Auto상태 여부를 서버에 보낸다.
	static void SendCombatAutoPlay(bool InIsAuto);

	// 계정충돌 팝업 정보 요청
	static void SendReqIdpConflictPlayer();
	
public:
	// 접속 종료 요청
	static void SendReqLeaveToInActivity(int32 InIdleTimeSecond);

	static void SendUserGameExit();

	// 캐릭터 닉네임 변경
public:
	static void SendReqCheckNickNameChange(const FString& InNickName);
	static void SendReqDecideNickNameChange(const FString& InNickName, ItemDBId InTicketItemDBId);

	// 성소(지도 정보 요청)
	// 처음엔 FGsNetSendServiceWorld 에 있었는데
	// gateway로 변경(차준규차석님 요청)
public:
	static void SendReqMapSanctumInfo(MapId In_mapId);

public:
	static void SendReqBattleArenaApply(MapId InMapId, bool InIsApply);
	static void SendReqBattleArenaEnter(MapId InMapId);	
	//static void SendReqBattleArenaLeave(MapId InMapId);

	/**
	 * Battle Arena Rank
	 */
	static void SendReqBattleArenaLocalInfo(GuildDBId InLocalGuildDBId);
	static void SendReqBattleArenaRankInfo(GuildWarTierDataId InTierDataId, TArray<int32> InIndexSet);
};
