// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UTIL/GsIntervalChecker.h"

/**
 * 길드 기본정보 (가입 창에서 필요한 정보)
 */
class T1PROJECT_API FGsGuildSendCoolTimeController
{
public:
	enum EGsCoolTimeType
	{
		GuildList, // 길드 리스트
		WaitingGuildList, // 내가 가입 신청중인 길드 리스트
		SearchGuild, // 길드 검색 요청
		RefreshGuildList, // 길드 리스트 갱신 요청
		MyGuildInfo, // 내 길드 정보 
		MemberList, // 길드원 리스트
		WaitingUserList, // 가입 요청 대기자 리스트
		AuthorityList, // 등급별 권한 리스트
		BuildingList, // 길드 건물 리스트(버프정보 포함)
		JoinType, // 가입방식 변경
		EnemyList, // 적대 길드 리스트
		AllyList, // 동맹 길드 리스트
		AllyStateList, // 동맹 상태 리스트
		QuestInfo, // 퀘스트 정보
		QuestRefresh, // 퀘스트 갱신
		AgitDungeonRaid, // 기사단 영지 공략 던전 정보
		AgitDungeonRaidBookmarkList, // 기사단 영지 공략던전 즐겨찾기 정보

		Max
	};

private:
	TMap<EGsCoolTimeType, FGsIntervalChecker> _checkerMap;

public:
	void Initialize();

	void ResetAllCoolTime();

	// 체커 쿨타임 초기화(없앰)
	void ResetCoolTime(EGsCoolTimeType InType);
	// 체커 쿨타임이 지났는가
	bool IsCoolTimeOver(EGsCoolTimeType InType);
	// 체커 쿨타임 갱신(다시 세팅)
	void UpdateCoolTime(EGsCoolTimeType InType);
	
public:
	// 길드 리스트 요청
	bool SendRequestGuildList();
	// 내가 신청중인 길드 리스트 요청
	bool SendRequestRequestedJoinGuildList();
	// 길드 검색 요청
	bool SendRequestSearchGuild(const FString& InData, bool bSameServerOnly);
	// 길드 리스트 갱신 쿨타임 체크
	bool SendRequestRefreshGuildList();
	// 기사단 오픈 시 내 길드 상세정보 요청
	bool SendRequestOpenGuild();
	// 길드원 리스트 요청
	bool SendRequestMemberList();
	// 가입 대기인원 요청
	bool SendRequestJoinWaitingUserList();
	// 등급별 권한 요청
	bool SendRequestAuthorityList();
	// 빌딩 리스트 요청
	bool SendRequestBuildingList();
	// 가입 방식 변경 쿨타임 체크
	bool SendRequestChangeJoinType(GuildJoinType InType);
	// 적대 길드 리스트 요청
	void SendReqEnemyList();
	// 동맹 길드 리스트 요청
	void SendReqAllyList();
	// 동맹상태 길드 리스트 요청
	void SendReqAllyStateList();
	// 퀘스트 정보 요청
	bool SendReqQuestInfo();
	// 퀘스트 새로고침
	bool SendReqQuestRefresh();
	// 기사단 영지 던전 정보
	bool SendReqAgitDungeonRaid();
	// 기사단 영지 공략 던전 즐겨찾기 리스트
	void SendReqAgitDungeonRaidBookmarkList();
};
