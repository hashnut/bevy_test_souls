// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"

/**
 * 길드 관련 편의 함수 및 정의 모음
 */
struct T1PROJECT_API FGsGuildHelper
{
public:
	static const struct FGsSchemaGuildConfig* GetGuildConfig();
	static const struct FGsSchemaGuildEmblem* GetGuildEmblem(int32 InEmblemId);
	static const struct FGsSchemaGuildLevelDetailData* GetGuildLevelData(int32 InGuildLevel);
	static const struct FGsSchemaGuildLevelDetailData* GetGuildMaxLevelData();
	static const struct FGsSchemaGuildBuilding* GetGuildBuilding(int32 InBuildingId);
	static const struct FGsSchemaGuildBuildingLevelData* GetBuildingLevelData(const struct FGsSchemaGuildBuilding* InData, 
		int32 InBuildingLevel);
	static const struct FGsSchemaGuildBuildingLevelData* GetBuildingLevelData(int32 InBuildingId, int32 InBuildingLevel);	
	static const struct FGsSchemaGuildQuestConfig* GetQuestConfig();
	static const struct FGsSchemaAgitConfig* GetAgitConfig();
	static const struct FGsSchemaAgitDungeonRaidConfig* GetAgitDungeonRaidConfig();

	static bool GetMemberGradeText(GuildMemberGradeType InType, OUT FText& OutText);

	// "길드명(서버1)"로 포맷 맞춰서 길드명 표시. 나와 같은 서버면 서버명을 표시 안함. bForcedShowPrefix가 true일 경우엔 같아도 강제로 표시
public:
	static void GetFormattedGuildName(const FString& InGuildName, WorldId InWorldId, bool bForcedShowPrefix, OUT FText& OutText);

public:
	// 길드 생성가입 가능레벨인지 체크
	static bool CanEnterGuildContent(Level InLevel, bool bInMessage);

	// 권한	체크
	static bool IsMasterAuthority(uint32 InAuthority); // 길드장 권한(모든 권한)
	static bool CanManageMember(uint32 InAuthority); // 단원 관리 권한
	static bool CanManagePost(uint32 InAuthority); // 게시글 관리 권한
	static bool CanManageBuilding(uint32 InAuthority); // 기사단 운영(건물) 권한
	static bool CanDiplomacy(uint32 InAuthority); // 길드 외교 권한
	static bool CanDistribute(uint32 InAuthority); // 기사단 창고 권한
	static bool CanManageCamp(uint32 InAuthority); // 수호탑 권한
	static bool CanManageAcquisition(uint32 InAuthority, GuildMemberGradeType InGradeType); // 획득 설정 관리 권한
	static bool CanDivision(uint32 InAuthority); // 기사단 분배 권한

	// 건물 효과 텍스트
	static bool GetBuildingEffectString(const struct FGsSchemaGuildBuilding* InData, int32 InBuildingLevel, 
		OUT FText& OutText);
	static bool GetBuildingEffectStringList(const struct FGsSchemaGuildBuilding* InData, int32 InBuildingLevel,
		OUT TArray<TPair<FText, FText>>& OutList);	

	// 2021/09/29 PKT - ItemCollection UI Category Name
	static void FindItemCollectionSubTabName(const FString& InKey, OUT FText& OutText);

	// 기사단 획득 아이템 출처 텍스트
	static bool GetGuildItemAcquireTypeText(GuildItemAcquireType InType, OUT FText& OutText);

	// 주의: 가입 당일 출석등의 컨텐츠를 막기 위한 검사로, 날짜로 판정하는게 아님에 유의. 
	// 단, 출석 등은 일일 리셋(ResetTimeTicket::RTT_DAILY) 시 리셋되므로, 이 함수는 가입 당일 일일 리셋 타임 기준으로 검사한다.	
	// 예: 일일 리셋 시간이 5:00인 경우(UTC), 12월 1일 13:00(UTC)가입 -> 12월 2일 5:00(UTC)전까지는 True, 후에는 False 판정
	static bool IsGuildJoinedDay(const time_t& InJoinedTime);

	// 해당 퀘스트를 현재 보유중인지  
	static bool IsExistQuest(QuestId InQuestId);

	// 보상
	static bool IsWaitRewardQuest();

	// 기사단 외교대상 길드 이름 체크
	static bool CheckDiplomacyGuildName(const FString& InGuildName, bool bInShowTicker, bool bInMyServerOnly);
	static bool CheckDiplomacyGuildDBId(const GuildDBId InGuildDBId, bool bInShowTicker);

	// 기사단 주화관련
	static ItemId GetGuildCoinItemId();
	static const struct FGsSchemaItemCommon* GetGuildCoinItemTable();

	// 캠프 관련
	// 지을 수 있는 캠프 레벨
	static int32 GetConstructableCampLevel(CampId InCampId);

	//dungeon
public:
	static bool TryGetGuildDungeonMinMaxLevel(OUT int32& outMin, OUT int32& outMax);
	static const struct FGsSchemaGuildDungeon* GetGuildDungeonData(int32 inLevel);

	// 기사단 컨텐츠 사용 가능 확인
	static bool CanOpenGuildMainOther();

	// Agit
public:
	static bool IsInAgit();
	static bool IsAgitMap(MapId InMapId);
};
