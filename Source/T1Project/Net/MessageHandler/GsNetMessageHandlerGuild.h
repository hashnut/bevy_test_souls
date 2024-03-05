#pragma once

#include "CoreMinimal.h"
#include "GsNetMessageHandleInterface.h"

class FGsNetManager;
/**
 * 인게임 길드 전용 네트워크 메시지 핸들러
 */
class T1PROJECT_API FGsNetMessageHandlerGuild : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerGuild() = default;
	virtual ~FGsNetMessageHandlerGuild() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

protected:
	// 기사단 생성 요청 응답
	void CreateGuildAck(const FGsNet::Packet& InPacket);
	// 기사단 생성/즉시 가입/가입신청수락됨 시 받음(나)
	void UserGuildInfo(const FGsNet::Packet& InPacket);
	// 기사단 생성/가입 시 받음(타유저) - 네임플레이트 변경
	void UserGuildInfoNotify(const FGsNet::Packet& InPacket);
	// 기사단 유저 정보 삭제(나 포함 유저들 받음) - 네임플레이트 변경
	void ClearUserGuildInfoNotify(const FGsNet::Packet& InPacket);
	// 기사단 유저 문장 정보 변경(나 포함 유저들 받음) - 네임플레이트 변경
	void ChangeUserGuildEmblemNotify(const FGsNet::Packet& InPacket);
	// 기사단 권한 변경(변경된 유저들 각각 받음)
	void ChangedUserGuildMemberRights(const FGsNet::Packet& InPacket);	
	// 일일활동 결과
	void GuildDailyActivityAck(const FGsNet::Packet& InPacket);	
	// 길드 건물 상태가 변경되는 경우 노티(접속중인 모든 길드원에게 전달)	
	void GuildBuildingNotify(const FGsNet::Packet& InPacket);
	// 길드 문장 변경 요청 응답
	void ChangedGuildEmblemAck(const FGsNet::Packet& InPacket);
	// 길드 유저 가입 노티
	void JoinedGuildNotify(const FGsNet::Packet& InPacket);
	// 길드 유저 탈퇴 노티
	void LeavedGuildNotify(const FGsNet::Packet& InPacket);
	// 길드 유저 로그인 노티
	void LoginGuildNotify(const FGsNet::Packet& InPacket);
	// 길드 유저 로그아웃 노티
	void LogoutGuildNotify(const FGsNet::Packet& InPacket);
	// 길드 가입요청이 들어왔을 때 노티(권한있는 사람에게만 온다)
	void ReqJoinGuildNotify(const FGsNet::Packet& InPacket);

	// 멤버 등급 변경 노티
	void InformChangedGuildMemberGrade(const FGsNet::Packet& InPacket);
	// 길드 공지 변경 노티 
	void InformChangedGuildNotice(const FGsNet::Packet& InPacket);
	// 길드 재화 사용 시 노티(길드원들에게 옴)
	void GuildCurrencyUpdate(const FGsNet::Packet& InPacket);
	// 길드 엠블렘 변경 노티(길드원들에게 옴)
	void InformChangedGuildEmblem(const FGsNet::Packet& InPacket);
	// 길마 장기 미접으로 길마 바뀜 알림
	void InformDelegateGuildCaptin(const FGsNet::Packet& InPacket);	
	// 길드 SNS 주소 변경 알림(기사단원들)
	void GuildChangeSNSLinkNotify(const FGsNet::Packet& InPacket);

	// 2021/10/26 PKT - 길드 아이템 컬렉션 리스트 북마크 생성에 대한 응답
	void ItemCollectionBookMarkActive(const FGsNet::Packet& InPacket);
	// 2021/10/26 PKT - 길드 아이템 컬렉션 리스트 북마크 해제에 대한 응답
	void ItemCollectionBookMarkInActive(const FGsNet::Packet& InPacket);
	// 2021/10/26 PKT - 길드 아이템 컬렉션 아이템 등록(자신)
	void ItemCollectionCompletedAck(const FGsNet::Packet& InPacket);
	// 2021/10/26 PKT - 길드 아이템 컬렉션 아이템 등록(길드원)
	void ItemCollectionRegisterNotify(const FGsNet::Packet& InPacket);
	
	// 기사단 창고 아이템 분배 결과(자신) - 아이템 분배에 대한 응답
	void StorageDistributionAck(const FGsNet::Packet& InPacket);
	// 기사단 창고 슬롯 확장 결과(자신) - 창고 슬롯 확장에 대한 응답
	void StorageExtendSlotAck(const FGsNet::Packet& InPacket);
	// 기사단 창고 아이템 획득 알림(길드원) - 기사단 컨텐츠에 의해 창고에 아이템이 추가되었을 경우
	void StorageAcquiredItemNotify(const FGsNet::Packet& InPacket);
	// 기사단 창고 아이템 분배 알림(길드원) - 창고의 아이템이 분배되었을 경우
	void StorageDistributedItemNotify(const FGsNet::Packet& InPacket);
	// 기사단 창고 슬롯 확장 알림(길드원)
	void StorageExtendSlotNotify(const FGsNet::Packet& InPacket);

protected:
	// 권한 관련 에러일 경우 메시지 티커 띄우고 true 리턴
	bool CheckAuthorityError(PD::Result InResult);
	// 멤버 관련 에러일 경우 메시지 티커 띄우고 true 리턴
	bool CheckMemberError(PD::Result InResult);
	// 주변 유저 외교 상황 체크 후 갱신
	void CheckDiplomacyRemotePlayer(bool bInvalidateNamePlate);

	void TrayJoinGuildTicker(const FString& InGuildName, int32 InEmblemId, bool bIsCreate);
	void ShowMessageTicker(const FTextKey& InNamespace, const FTextKey& InKey);
	void ShowMessageTicker(const FString& InFormat);
	void PopupOkTwoMessage(const FText& InTextFirst, const FText& InTextSecond);
	void PopupOkTwoMessage(StoryId InStoryId, const FText& InTextSecond);

protected:
	void AckFailedEnterGuildDungeon(const FGsNet::Packet & InPacket);
	void AckFailedLeaveGuildDungeon(const FGsNet::Packet & InPacket);
	//void UseUildDungeonTicket(const FGsNet::Packet & InPacket);
	void InformGuildDungeonRank(const FGsNet::Packet & InPacket);
	void GuildDungeonTicketExpireTime(const FGsNet::Packet& InPacket);
	void GuildDungeonClear(const FGsNet::Packet& InPacket);
	void GuildDungeonTicketTimeOut(const FGsNet::Packet & InPacket);

	/* 기사단 획득 설정 & 분배 & 경매 */
	// 멤버들에게 보내지는 정보
	void GuildStoreNotify(const FGsNet::Packet& InPacket);
	// 획득 설정 변경 알림 패킷
	void SaveGuildAcquisitionNotify(const FGsNet::Packet& InPacket);

	// 아이템 입찰
	void GuildAuctionBidder(const FGsNet::Packet& InPacket);
	// 잔여 재화 회수
	void GuildAuctionReturnCurrency(const FGsNet::Packet& InPacket);
	
	// 기사단 영지
protected:
	void AgitRentalAck(const FGsNet::Packet& InPacket);
	void AgitRentalExtensionAck(const FGsNet::Packet& InPacket);
	void AgitEnterAck(const FGsNet::Packet& InPacket);
	void AgitLeaveAck(const FGsNet::Packet& InPacket);

	// 영지 공략 던전
	void AgitDungeonRaidOpenAck(const FGsNet::Packet& InPacket);
	void AgitDungeonRaidCloseAck(const FGsNet::Packet& InPacket);
	void AgitDungeonRaidEnterAck(const FGsNet::Packet& InPacket);
	void AgitDungeonRaidLeaveAck(const FGsNet::Packet& InPacket);
	void AgitDungeonRaidClearNotify(const FGsNet::Packet& InPacket);
	void AgitDungeonRaidBookmarkListAck(const FGsNet::Packet& InPacket);
	void AgitDungeonRaidBookmarkAddAck(const FGsNet::Packet& InPacket);
	void AgitDungeonRaidBookmarkDeleteAck(const FGsNet::Packet& InPacket);

	void AgitDungeonRaidInformRank(const FGsNet::Packet& InPacket);
};
