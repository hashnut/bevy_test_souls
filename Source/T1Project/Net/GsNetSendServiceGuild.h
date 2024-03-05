// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"

/**
 * 길드 관련 패킷 Send 함수 모음 
 */
class T1PROJECT_API FGsNetSendServiceGuild
{
	// 패킷명이 CS인 항목
public:
	// 길드 생성 요청
	static void SendReqCreateGuild(const FString& InName, GuildEmblemId InEmblemId,
		GuildJoinType InJoinType, const FString& InIntroduce);
	// 출석, 기부요청
	static void SendReqDailyActivity(GuildDailyActivityType InType);	
	// 길드문양 변경 요청
	static void SendReqChangeEmblem(GuildEmblemId InEmblemId);	

public:
	// 2021/10/26 PKT - 길드 아이템 콜렉션 리스트를 요청
	static void SendRItemCollectionRegister(CollectionConditionId InCollectionConditionId, const ItemDBId InItemId);
	// 2021/10/26 PKT - 아이템 컬렉션 즐겨찾기 요청
	static void SendItemCollectionBookMark(CollectionId InCollectionId, bool InActive);	

	// 기사단 창고
public:
	// 아이템 분배 요청
	static void SendReqGuildStorageDistributeItem(UserDBId InReceiverDBId, ItemId InItemId, ItemDBId InItemDBId, ItemAmount InItemAmount);
	// 창고 인벤토리 확장 요청
	static void SendReqGuildStorageExtendSlot();

	/*기사단 분배 & 경매*/
	// 경매 입찰 요청
	static void SendReqGuildAuctionBidder(GuildAuctionId InAuctionId, CurrencyType InCurrencyType, Currency InCurrency);
	// 회수금 반환 요청
	static void SendReqGuildAuctionReturnCurrency();

	// 기사단 영지
public:
	static void SendReqAgitRental();
	static void SendReqAgitRentalExtension();
	static void SendReqAgitEnter();
	static void SendReqAgitLeave(bool bInUseRadomScroll);
	// 영지 공략 던전
	static void SendReqAgitDungeonRaidOpen(AgitDungeonRaidId InRaidId);
	static void SendReqAgitDungeonRaidClose(AgitDungeonRaidId InRaidId);
	static void SendReqAgitDungeonRaidEnter(AgitDungeonRaidId InRaidId);
	static void SendReqAgitDungeonRaidLeave();
	static void SendReqAgitDungeonRaidBookmarkList();
	static void SendReqAgitDungeonRaidBookmarkAdd(AgitDungeonRaidId InRaidId);
	static void SendReqAgitDungeonRaidBookmarkDelete(AgitDungeonRaidId InRaidId);

	//------------------------------------------------------------------------------------------------------------------
	// 패킷명이 CG인 항목
public:
	// 가입 신청자 목록 요청
	static void SendReqWaitingJoinGuildUserList();
	// 내가 가입 신청한 목록 요청
	static void SendReqRequestedJoinGuildList();
	// 기사단 가입 리스트 요청
	static void SendReqGuildList();
	// 길드명 검색 요청 (응답이 SendReqGuildList 와 동일한 패킷으로 온다)
	static void SendReqSearchGuild(const FString& InName, bool bSameServerOnly);
	// 길드 세부정보 요청(길드 상세보기) (주의: FGsGuildManager::SendRequestGuildDetail 통해서 호출하자)
	static void SendReqGuildDetail(GuildDBId InGuildDBId);
	
	// 기사단 정보 요청
	static void SendReqOpenGuild();
	// 기사단 가입 요청
	static void SendReqJoinGuild(GuildDBId InGuildDBId);
	// 길드 탈퇴 요청
	static void SendReqLeave();
	// 기사단원 추방 요청
	static void SendReqBanMember(UserDBId InUserDBId);
	// 가입 승인 요청
	static void SendReqJoinApprove(UserDBId InUserDBId);
	// 길드 해산 요청
	static void SendReqDeleteGuild();
	// 기사단 가입 요청 취소
	static void SendReqCancelJoinGuild(GuildDBId InGuildDBId);
	// 가입 거부 요청
	static void SendReqJoinReject(UserDBId InUserDBId);
	// 기사단원 리스트 요청
	static void SendReqGuildMemberList();
	// 멤버 등급 변경 요청
	static void SendReqChangeMemberGrade(UserDBId InUserDBId, GuildMemberGradeType InMemberGrade);
	// 등급별 권한 변경 요청(변경된 내용만 넣기)
	static void SendReqChangeAuthorityList(UserDBId InUserDBId, TArray<GuildMemberRightsbyGrade>& InAuthorityList);	
	// 등급별 권한 리스트 요청
	static void SendReqAuthorityList();
	// 길드 공지글 변경 요청
	static void SendReqChangeNotice(const FString& InMessage);
	// 길드 소개글 변경 요청
	static void SendReqChangeIntroduction(const FString& InMessage);
	// 길드 가입 방식 변경
	static void SendReqChangeJoinType(GuildJoinType InType);
	// 길드 가입 제한 레벨 변경
	static void SendReqChangeLimitLevel(Level InLevel);	
	// 기사단 SNS 정보 저장
	static void SendReqSaveSNSLink(int32 InIndex, const FString& InURL);	
	// 기사단 획득 설정 변경 요청
	static void SendReqSaveGuildAcquisitionSettings(const GuildAcquisitionSettings& InAcquisitionSettings);

	// 길드 건물
public:
	static void SendReqGuildBuildingList(); // 길드 빌딩 리스트 요청	
	static void SendReqGuildBuildingOpen(GuildBuildingId InBuildingId); // 길드 건물 건설(0->1 레벨)	
	static void SendReqGuildBuildingUpgrade(GuildBuildingId InBuildingId); // 길드 건물 레벨업(1 이상 레벨)	
	static void SendReqGuildBuildingPassivity(GuildBuildingId InBuildingId); // 길드버프 활성화
	
	// 기사단 외교
public:
	static void SendReqHostileList(); // 적대 목록 요청
	static void SendReqAllyList(); // 동맹 목록 요청
	static void SendReqAllyStateList(); // 동맹상태 목록 요청
	static void SendHostileAdd(GuildDBId InGuildDBId); // 적대 요청
	static void SendHostileDelete(GuildDBId InGuildDBId); // 적대 제거
	static void SendAllyAddList(GuildDBId InGuildDBId);	// 동맹 요청	
	static void SendBreakAlly(GuildDBId InGuildDBId); // 동맹 파기
	static void SendCancelRejectAlly(GuildDBId InGuildDBId); // 동맹 요청 취소/거절
	static void SendAcceptAlly(GuildDBId InGuildDBId); // 동맹 수락	

	// 접속 전 길드 채팅 리스트 요청
	static void SendReqGuildChatList(GuildChatDBId seq);
	// 접속 전 동맹 채팅 리스트 요청
	static void SendReqAllyGuildChatList(GuildChatDBId seq);
public:
	// 기사단 퀘스트
	static void SendReqGuildQuestInfo();

	// 2021/10/26 PKT - 길드 아이템 콜렉션 리스트를 요청
	static void SendRItemCollectionSlotList();	

	// 기사단 창고
public:
	// 기사단 창고 오픈 요청
	static void SendReqOpenGuildStorage();
	// 멤버 정보 요청
	static void SendReqGuildStorageMemberList();
	// 창고 인벤토리 정보 요청
	static void SendReqGuildStorageItemList();
	// 획득 내역 요청
	static void SendReqGuildStorageAcquiredHistory();
	// 분배 내역 요청
	static void SendReqGuildStorageDistributedHistory();

	//길드 던전
public:
	static void SendReqEnterGuildDungeon();	
	static void SendReqExitGuildDungeon();
	static void SendReqOpenGuildDungeonInfo();

	// 길드 레드닷 체크 정보 서버 저장
public:
	static void SendReqGuildRedDotSave(GuildRedDotDetail& InData);

	/*기사단 분배 & 경매*/
	static void SendReqGuildDivisionList(ItemDBId InLastItemDBId = INVALID_ITEM_DB_ID, bool InIsScrolled = false);
	static void SendReqGuildAuctionList(GuildAuctionId InLastAuctionId = INVALID_GUILD_AUCTION_ID, bool InIsScrolled = false);
	static void SendReqGuildCalculateList(GuildAuctionId InLastAuctionId = INVALID_GUILD_AUCTION_ID, bool InIsScrolled = false);
	// 경매 등록 요청
	static void SendReqGuildAuctionRegister(const TArray<ItemDBId>& InItemList, GuildAuctionType InAuctionType, GuildAuctionResultType InResultType, const TArray<UserDBId>& InUserList, CurrencyType InCurrencyType, const TArray<Currency>& InCurrencyList);
	static void SendReqGuildAuctionDetail(ItemDBId InItemDBId);

	// 기사단 성소
public:
	static void SendReqSanctumNexusDestruction();

	// 기사단 영지
public:
	// 영지 공략던전 정보 요청
	static void SendReqAgitDungeonRaid();
	// 영지 공략던전 상세 정보 요청(랭킹, 참여자 등 포함)
	static void SendReqAgitDungeonRaidDetail(AgitDungeonRaidId InRaidId);

	// 기사단 그룹 채팅 관리
public:
	// 기사단 그룹 채팅 관리 창 요청
	static void SendReqGuildGroupChatList();
	// 기사단 그룹 기사단 관리 창 멤버 길드 리스트 요청
	static void SendReqGuildGroupChatMemberList(GuildGroupChatRoomId roomId);
	// 기사단 그룹 기사단 관리 창 초대장 리스트 요청
	static void SendReqGuildGroupChatInvitaionList();
	// 기사단 그룹 기사단 관리 창 보낸 초대장 리스트 요청
	static void SendReqGuildGroupChatInviteGuestList(GuildGroupChatRoomId roomId);

	// 기사단 그룹 채팅 룸 Create
	static void SendReqGuildGroupChatRoomCreate(FString roomName);
	// 기사단 그룹 채팅 초대 요청
	static void SendReqGuildGroupChatInvite(GuildGroupChatRoomId roomId, FString guildName, PlanetWorldId guildGroupWorldId);
	// 기사단 그룹 채팅에서 추방 요청
	static void SendReqGuildGroupChatKickOut(GuildGroupChatRoomId roomId, GuildDBId guildId);
	// 기사단 그룹 채팅에서 탈퇴 요청
	static void SendReqGuildGroupChatWithDraw(GuildGroupChatRoomId roomId);

	// 기사단 그룹 채팅 참여 수락 요청
	static void SendReqGuildGroupChatInvitationAccept(GuildGroupChatRoomId roomId);
	// 기사단 그룹 채팅 참여 거부 요청
	static void SendReqGuildGroupChatInvitationReject(GuildGroupChatRoomId roomId);
	// 기사단 그룹 채팅 초대 취소
	static void SendReqGuildGroupChatCancelInvitation(GuildGroupChatRoomId roomId, GuildDBId guildId);

	// 기사단 그룹 채팅 방장 권한 위임
	static void SendReqGuildGroupChatEntrustWithRoomMaster(GuildGroupChatRoomId roomId, GuildDBId guildId);
	// 기사단 그룹 채팅 방 입력 권한 변경
	static void SendReqGuildGroupChatChangeAuthorizedGrade(GuildGroupChatRoomId roomId, GuildMemberGradeType grade);
};
