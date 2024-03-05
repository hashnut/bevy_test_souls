// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/GSNet.h"
#include "GsNetMessageHandleInterface.h"
#include "SharedEnums/PD_ResultEnum.h"


/**
 * 게이트웨이 패킷 중 기사단 관련 패킷 핸들러
 */
class T1PROJECT_API FGsNetMessageHandlerGatewayGuild : public IGsNetMessageHandler, FGsNetMessageGateway
{
public:
	FGsNetMessageHandlerGatewayGuild() = default;
	virtual ~FGsNetMessageHandlerGatewayGuild() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

public:	
	void GuildListAck(const FGsNet::Packet &inPacket); // 가입창 리스트 요청, 길드 검색 결과	
	void GuildWaitingJoinUserListAck(const FGsNet::Packet &inPacket); // 가입 신청자 목록
	void GuildRequestedJoinListAck(const FGsNet::Packet &inPacket); // 내가 가입 신청한 목록
	void GuildWaitingJoinUserCount(const FGsNet::Packet& inPacket);	// 내 길드에 가입 신청한 인원 수 노티(권한 있는 사람만)
	void GuildMyGuildInfo(const FGsNet::Packet& InPacket); // 월드 입장 시 받는 기사단 정보
	void GuildOpenAck(const FGsNet::Packet& InPacket); // 내 길드 정보 요청
	void GuildOpenFailedAck(const FGsNet::Packet& InPacket);
	void GuildJoinAck(const FGsNet::Packet& InPacket); // 가입 요청
	void GuildLeaveAck(const FGsNet::Packet& InPacket); // 탈퇴 요청
	void GuildExpelAck(const FGsNet::Packet& InPacket); // 추방 요청
	void GuildApprovalToJoinAck(const FGsNet::Packet& InPacket); // 가입 승인
	void GuildDeleteAck(const FGsNet::Packet& InPacket); // 기사단 삭제 요청
	void GuildCancelJoinAck(const FGsNet::Packet& InPacket); // 가입 요청 취소
	void GuildRejectedUserJoinAck(const FGsNet::Packet& InPacket); // 가입 거절
	void GuildMemeberListAck(const FGsNet::Packet& InPacket); // 기사단원 리스트 요청
	void GuildMemeberListFailedAck(const FGsNet::Packet& InPacket); // 기사단원 리스트 요청 실패
	void GuildChangedMemberGradeAck(const FGsNet::Packet& InPacket); // 멤버 등급 변경
	void GuildChangedMemberRightsByGradeAck(const FGsNet::Packet& InPacket); // 멤버 등급별 권한 변경
	void GuildMemberRightsByGradeListAck(const FGsNet::Packet& InPacket); // 등급별 권한 리스트	
	void GuildChangedNoticeAck(const FGsNet::Packet& InPacket); // 길드 공지 변경 요청
	void GuildChangedDescAck(const FGsNet::Packet& InPacket); // 길드 소개글 변경 요청
	void GuildChangedJoinTypeAck(const FGsNet::Packet& InPacket); // 길드 가입방식 변경 요청
	void GuildChangedLimitedUserLevelAck(const FGsNet::Packet& InPacket); // 길드 가입 제한레벨 변경
	void GuildJoinRejectedNotify(const FGsNet::Packet& InPacket); // 길드 가입 요청이 거절됨
	void GuildContributionUpdate(const FGsNet::Packet& InPacket); // 길드 공헌도 및 레벨 변경 노티
	void GuildSaveSNSLinkAck(const FGsNet::Packet& InPacket); // 길드 SNS 주소 저장 요청 응답	

public:
	void GuildBuildingListAck(const FGsNet::Packet& InPacket); // 길드 건물 리스트 요청 응답	
	void GuildBuildingOpenAck(const FGsNet::Packet& InPacket); // 길드 건물 건설 요청 응답(0->1 레벨)	
	void GuildBuildingUpgradeAck(const FGsNet::Packet& InPacket); // 길드 건물 레벨업 요청 응답(1 이상 레벨)	
	void GuildBuildingPassivityAck(const FGsNet::Packet& InPacket); // 길드 버프 활성화 노티	

public:
	void GuildAllyRequestListAck(const FGsNet::Packet& InPacket); // 상태 리스트
	void GuildHostileListAck(const FGsNet::Packet& InPacket); // 적대 리스트
	void GuildAllyListAck(const FGsNet::Packet& InPacket); // 동맹 리스트
	void GuildHostileAddAck(const FGsNet::Packet& InPacket); // 적대 요청
	void GuildHostileDeleteAck(const FGsNet::Packet& InPacket); // 적대 제거 요청
	void GuildAllyAddAck(const FGsNet::Packet& InPacket); // 동맹 요청
	void GuildAllyBreakAck(const FGsNet::Packet& InPacket); // 동맹파기 요청
	void GuildAllyCancelRejectAck(const FGsNet::Packet& InPacket); // 동맹 취소/거절 요청
	void GuildAllyAcceptAck(const FGsNet::Packet& InPacket); // 동맹 수락	
	void InformGuildHostileAdd(const FGsNet::Packet& InPacket); // 적대 추가 노티
	void InformGuildHostileDelete(const FGsNet::Packet& InPacket); // 적대 제거 노티
	void InformGuildAllyRequestFailed(const FGsNet::Packet& InPacket); // 동맹 요청 실패 노티
	void InformGuildAllyAcceptFailed(const FGsNet::Packet& InPacket); // 동맹 수락 실패 노티
	void InformGuildAllyAccept(const FGsNet::Packet& InPacket); // 동맹 신청이 왔다고 외교 권한있는 유저들에게 알림
	void InformGuildAlly(const FGsNet::Packet& InPacket); // 동맹이 되었다고 기사단원들에게 알림	

public:
	void GuildQuestInfoAck(const FGsNet::Packet& InPacket); // 길드 퀘스트 정보

	void GuildBuildItemCollenctionListAck(const FGsNet::Packet& InPacket);	// 2021/10/26 PKT - Item Collection Build List

	void GuildDetailAck(const FGsNet::Packet& InPacket); // 타 길드 상세보기
	void GuildDetailErrorAck(const FGsNet::Packet& InPacket); // 타 길드 상세보기 에러

public:
	void GuildStorageOpenAck(const FGsNet::Packet& InPacket); // 기사단 창고 오픈
	void GuildStorageItemListAck(const FGsNet::Packet& InPacket); // 기사단 창고 인벤토리 정보
	void GuildStorageMemberListAck(const FGsNet::Packet& InPacket); // 기사단 창고 멤버 정보
	void GuildStorageAcquiredHistoryAck(const FGsNet::Packet& InPacket); // 기사단 창고 획득 내역 정보
	void GuildStorageDistributedHistoryAck(const FGsNet::Packet& InPacket); // 기사단 창고 분배 내역 정보	

public:
	// 던전
	void AckOpenGuildDungeonInfo(const FGsNet::Packet& InPacket);
	void InformGuildDungeonLevel(const FGsNet::Packet& InPacket);

	// 경매 & 분배
	void GuildAuctionListAck(const FGsNet::Packet& InPacket); // 경매 탭 목록 정보
	void GuildAuctionBidderInfoList(const FGsNet::Packet& InPacket); // 경매 탭 입찰자 정보
	void GuildAuctionItemDetail(const FGsNet::Packet& InPacket); // 경매 & 분배 탭 목록 부가 정보
	void GuildDivisionListAck(const FGsNet::Packet& InPacket); // 분배 탭 목록 정보
	void GuildDivisionListDetail(const FGsNet::Packet& InPacket); // 분배 탭 목록 부가 정보
	void GuildAuctionReturnCurrencyNotify(const FGsNet::Packet& InPacket); // 회수금 정보
	void GuildAuctionRegisterAck(const FGsNet::Packet& InPacket); // 경매 등록
	void GuildCalculateListAck(const FGsNet::Packet& InPacket); // 정산 탭 목록 정보

	// 성소
public:
	void SanctumNexusDestructionAck(const FGsNet::Packet& InPacket); // 성소 파괴 요청 응답
	
	// 기사단 영지
public:
	void AgitDungeonRaidAck(const FGsNet::Packet& InPacket); // 영지 공략 던전 정보 응답
	void AgitDungeonRaidDetailAck(const FGsNet::Packet& InPacket); // 영지 공략 던전 상세 정보 응답
	void AgitDungeonRaidOpen(const FGsNet::Packet& InPacket);
	void AgitDungeonRaidClose(const FGsNet::Packet& InPacket);
	void AgitDungeonRaidClear(const FGsNet::Packet & InPacket);

	void AgitAlertDestory(const FGsNet::Packet& InPacket);
	void AgitExpireTimeUpdate(const FGsNet::Packet& InPacket);

protected:
	void ShowMessageTicker(const FTextKey& InNamespace, const FTextKey& InKey);
	// 권한 관련 에러일 경우 메시지 티커 띄우고 true 리턴
	bool CheckAuthorityError(PD::Result InResult);
	// 멤버 관련 에러일 경우 메시지 티커 띄우고 true 리턴
	bool CheckMemberError(PD::Result InResult);
	// 주변 유저 외교 상황 체크 후 갱신
	void CheckDiplomacyRemotePlayer(bool bInvalidateNamePlate);
	// 외교 관련에러 
	bool CheckGuildDiplomacyError(PD::Result InResult);

protected:
	// 기사단 그룹 채팅 리스트
	void AckGuildGroupChatList(const FGsNet::Packet& InPacket); //GC_ACK_GUILD_GROUP_CHAT_LIST
	// 기사단 그룹 초대장을 내게 보낸 길드 리스트
	void AckGuildGroupChatInvitationList(const FGsNet::Packet& InPacket); //GC_ACK_GUILD_GROUP_CHAT_INVITE_LIST
	// 기사단 그룹 멤버 리스트
	void AckGuildGroupChatMemberList(const FGsNet::Packet& InPacket); //GC_ACK_GUILD_GROUP_CHAT_MEMBER_LIST
	// 기사단 그룹 초대장을 내가 보낸 길드 리스트
	void AckGuildGroupChatReqInviteList(const FGsNet::Packet& InPacket); //GC_ACK_GUILD_GROUP_CHAT_REQUEST_LIST

	// 기사단 챗 그룹 만들기 ack
	void AckGuildGroupChatRoomCreate(const FGsNet::Packet& InPacket); // GC_ACK_GUILD_GROUP_CHAT_CREATE
	// 기사단 챗 그룹 초대 ack - 보낸 외교권자만 받음
	void AckGuildGroupChatInvite(const FGsNet::Packet& InPacket); //GC_ACK_GUILD_GROUP_CHAT_INVITE
	// 기사단 챗 그룹 초대거절 ack
	void AckGuildGroupChatInviteAccepted(const FGsNet::Packet& InPacket); //GC_ACK_GUILD_GROUP_CHAT_ACCEPT
	// 기사단 챗 그룹 초대거절 ack
	void AckGuildGroupChatInviteReject(const FGsNet::Packet& InPacket); //GC_ACK_GUILD_GROUP_CHAT_INVITE_REJECT
	// 기사단 챗 그룹 초대거절 ack
	void AckGuildGroupChatInviteCancel(const FGsNet::Packet& InPacket); //GC_ACK_GUILD_GROUP_CHAT_INVITE_CANCEL
	// 기사단 그룹 채팅 추방요구에 대한 응답 ack - 보낸 외교권자
	void AckGuildGroupChatKickOut(const FGsNet::Packet& InPacket); //GC_ACK_GUILD_GROUP_CHAT_KICK
	// 기사단 그룹 채팅 탈퇴 요구에 대한 응답 ack - 보낸 외교권자
	void AckGuildGroupChatWithDraw(const FGsNet::Packet& InPacket); //GC_ACK_GUILD_GROUP_CHAT_WITHDRAWAL
	// 기사단 그룹 방장 위임
	void AckGuildGroupChatEntrustMaster(const FGsNet::Packet& InPacket); //GC_ACK_GUILD_GROUP_CHAT_DELEGATE
	// 기사단 그룹 채팅 가능 제한 수정
	void AckGuildGroupChatAuthChanged(const FGsNet::Packet& InPacket); //GC_ACK_GUILD_GROUP_CHAT_MODIFY_CHAT_GRADE
	
	// 같은 그룹의 외교권자들에게 길드 invitiaon 발생을 알림
	void NotiGuildGroupChatInvitaionRequest(const FGsNet::Packet& InPacket); //GC_GUILD_GROUP_CHAT_REQUEST, 
	// 초대한 기사단 외교권자들에게 초대 거부를 알림
	//void NotiHostGuildGroupChatInvitaionRejected(const FGsNet::Packet& InPacket);  //현재 없음
	// 초대한 기사단 외교권자들에게 초대 취소를 알림
	//void NotiHostGuestGuildGroupChatInvitationCanceled(const FGsNet::Packet& InPacket); //현재 없음

	// 초대 승인 또는 생성시 모든 우리 기사단원에게 알림
	void NotiMyGuildGroupChatRoomCreated(const FGsNet::Packet& InPacket); //GC_GUILD_GROUP_CHAT_ENTERED_FOR_MY_GUILD
	// 초대 되어 생성된 룸 정보
	//void NotiMyGuildGroupChatGuildList(const FGsNet::Packet& InPacket); //GC_GUILD_GROUP_CHAT_GUILD_LIST - 사용안함
	// 그룹에서 추방 되었음을 알림 - 내 길드원
	void NotiMyGuildGuildGroupChatRoomKickedOut(const FGsNet::Packet& InPacket); //GC_GUILD_GROUP_CHAT_KICK
	// 그룹에서 추방 되었음을 알림 - 남은 그룹 길드원
	void NotiOtherGuildGuildGroupChatRoomKickedOut(const FGsNet::Packet& InPacket); //GC_GUILD_GROUP_CHAT_KICK
	// 그룹에서 탈퇴 하였음을 알림 - 내 길드원
	void NotiMyGuildGuildGroupChatRoomWithDraw(const FGsNet::Packet& InPacket); //GC_GUILD_GROUP_CHAT_WITHDRAWAL
	// 그룹에서 탈퇴 하였음을 알림 - 모든 길드원
	void NotiOtherGuildGuildGroupChatRoomWithDraw(const FGsNet::Packet& InPacket); //GC_GUILD_GROUP_CHAT_WITHDRAWAL

	// 초대장을 받았음을 받은 길드 외교권자들에게 알림
	void NotiGuestGuildGroupChatInvited(const FGsNet::Packet& InPacket); //GC_GUILD_GROUP_CHAT_INVITED
	// 초대 거부를 받은 길드 외교권자들에게 알림
	void NotiGuestGuildGroupChatInvitationRejected(const FGsNet::Packet& InPacket); //GC_GUILD_GROUP_CHAT_INVITE_REJECT
	// 초대 거부를 초대한 길드 외교권자들에게 알림
	void NotiHostGuildGroupChatInvitationRejected(const FGsNet::Packet& InPacket); //아직 없음
	// 초대 취소를 받은 길드 외교권자들에게 알림
	void NotiGuestGuildGroupChatInvitationCanceled(const FGsNet::Packet& InPacket); //GC_GUILD_GROUP_CHAT_INVITE_CANCEL
	// 초대 취소를 초대한 길드 외교권자들에게 알림
	void NotiHostGuildGroupChatInvitationCanceled(const FGsNet::Packet& InPacket); //아직 없음
	
	// 초대 승인을 그 그룹에 참여중인 모든 길드원에게 알림
	void NotiAllGuestGuildGroupChatInvitationAccepted(const FGsNet::Packet& InPacket); //GC_GUILD_GROUP_CHAT_ENTERED_FOR_OTHER_GUILD	

	// 길드 그룹 채팅 장의 권한이 위임되었음을 모든 그룹의 길드원에게 알림
	void NotiAllGuildGroupChatMembersEntrutMaster(const FGsNet::Packet& InPacket);
	// 길드 그룹 챝킹 입력 권한이 변경되었음을 모든 그룹의 길드원에게 알림
	void NotiAllGuildGroupChatMembersAuthChanged(const FGsNet::Packet& InPacket);
};
